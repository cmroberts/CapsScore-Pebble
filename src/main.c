#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "http.h"
#include "config.h"

#ifdef IOS
#define MY_UUID HTTP_UUID
#else
#define MY_UUID { 0x2E, 0x35, 0xA6, 0x01, 0xBA, 0x0C, 0x45, 0xED, 0xB8, 0x14, 0xED, 0x2A, 0x9E, 0xB5, 0x3C, 0x86 }
#endif

PBL_APP_INFO(MY_UUID,
             "Caps Score", "Jason Ramboz",
             1, 0, /* App version */
             RESOURCE_ID_ICON,
             APP_INFO_WATCH_FACE);

#ifdef WHITE_BG
#define BG_COLOR GColorWhite
#define TEXT_COLOR GColorBlack
#define IMAGE_RES_ID RESOURCE_ID_LOGO_WHITEBG
#else
#define BG_COLOR GColorBlack
#define TEXT_COLOR GColorWhite
#define IMAGE_RES_ID RESOURCE_ID_LOGO_BLACKBG
#endif

Window window;

BmpContainer image_container;

TextLayer time_layer;
TextLayer date_layer;
TextLayer home_team_layer;
TextLayer away_team_layer;
TextLayer status_layer;

GFont arial_narrow_bold_30;
GFont droid_sans_mono_18;

AppTimerHandle handle;

void request_score(AppContextRef ctx) {
	DictionaryIterator *body;
	HTTPResult result = http_out_get(SERVER_URL, HTTP_COOKIE, &body);
	if(result != HTTP_OK) {
		switch (result) {
			case HTTP_BUSY:
				text_layer_set_text(&status_layer, "BUSY");
				break;
			case HTTP_INVALID_ARGS:
				text_layer_set_text(&status_layer, "INARG");
				break;
			case HTTP_NOT_ENOUGH_STORAGE:
				text_layer_set_text(&status_layer, "NSPC");
				break;
			case HTTP_INTERNAL_INCONSISTENCY:
				text_layer_set_text(&status_layer, "BRKD");
				break;
			default:
				break;
		}
		handle = app_timer_send_event(ctx, 120000, HTTP_COOKIE); // Try again in 2 min
		return;
	}
	
	// Team preference
	dict_write_cstring(body, 1, MY_TEAM);
	
	// Date format preference
	#ifdef DATE_MMDD
		dict_write_cstring(body, 2, "MMDD");
	#else
		dict_write_cstring(body, 2, "DDMM");
	#endif
	
	// Time format preference
	if(clock_is_24h_style()) {
		dict_write_int32(body, 3, 24);
	} else {
		dict_write_int32(body, 3, 12);
	}
	
	if(http_out_send() != HTTP_OK) {
		return;
	}
}

void reconnect(void* context) {
	request_score(context);
}

void failed(int32_t cookie, int http_status, void* context) {
	if(cookie == 0 || cookie == HTTP_COOKIE) {
		static char error_text[] = "00000";
		if (http_status > 1000) {
			text_layer_set_text(&away_team_layer, "ERROR");
			static char error_num[] = "00000";
			snprintf(error_num, sizeof(error_num), "%d", http_status-1000);
			text_layer_set_text(&home_team_layer, error_num);
			switch (http_status-1000) {
				case HTTP_SEND_TIMEOUT:
					snprintf(error_text, sizeof(error_text), "TMT");
					break;
				case HTTP_SEND_REJECTED:
					snprintf(error_text, sizeof(error_text), "RJCT");
					break;
				case HTTP_NOT_CONNECTED:
					snprintf(error_text, sizeof(error_text), "NCON");
					break;
				case HTTP_BRIDGE_NOT_RUNNING:
					snprintf(error_text, sizeof(error_text), "NBRG");
					break;
				case HTTP_INVALID_ARGS:
					snprintf(error_text, sizeof(error_text), "INARG");
					break;
				case HTTP_BUSY:
					snprintf(error_text, sizeof(error_text), "BUSY");
					break;
				case HTTP_BUFFER_OVERFLOW:
					snprintf(error_text, sizeof(error_text), "BFOV");
					break;
				case HTTP_ALREADY_RELEASED:
					snprintf(error_text, sizeof(error_text), "AREL");
					break;
				case HTTP_CALLBACK_ALREADY_REGISTERED:
					snprintf(error_text, sizeof(error_text), "AREG");
					break;
				case HTTP_CALLBACK_NOT_REGISTERED:
					snprintf(error_text, sizeof(error_text), "NREG");
					break;
				case HTTP_NOT_ENOUGH_STORAGE:
					snprintf(error_text, sizeof(error_text), "NSPC");
					break;
				case HTTP_INVALID_DICT_ARGS:
					snprintf(error_text, sizeof(error_text), "INDAR");
					break;
				case HTTP_INTERNAL_INCONSISTENCY:
					snprintf(error_text, sizeof(error_text), "BRKD");
					break;
				case HTTP_INVALID_BRIDGE_RESPONSE:
					snprintf(error_text, sizeof(error_text), "IBRSP");
					break;
				default:
					snprintf(error_text, sizeof(error_text), "E%d", http_status-1000);
					break;
			}
		} else {
			text_layer_set_text(&away_team_layer, "HTTP");
			text_layer_set_text(&away_team_layer, "ERROR");
			snprintf(error_text, sizeof(error_text), "%d", http_status);
		}
		text_layer_set_text(&status_layer, error_text);
	}
	// Try again in 2 minutes
	handle = app_timer_send_event(context, 120000, HTTP_COOKIE);
}

void success(int32_t cookie, int http_status, DictionaryIterator* received, void* context) {
	if(cookie != HTTP_COOKIE) return;
	
	// Get key 1, which tells what kind of data follows.
	// See the JSON spec in the php server file.
	static char home_text[] = "XXXXX";
	static char away_text[] = "XXXXX";
	static char status_text[] = "XXXXX";
	
	Tuple *tuple = dict_find(received, 1);
	
	if (!tuple) {
		text_layer_set_text(&away_team_layer, "NO");
		text_layer_set_text(&home_team_layer, "DATA");
		text_layer_set_text(&status_layer, "ERR");
		handle = app_timer_send_event(context, 120000, HTTP_COOKIE); // Try again in 2 min
		return;
	}
	
	int game_status = tuple->value->int32;
	
	switch (game_status) {
		case 0:
			tuple = dict_find(received, 2);
			snprintf(away_text, sizeof(away_text), "%s", tuple->value->cstring);
			tuple = dict_find(received, 3);
			snprintf(status_text, sizeof(status_text), "%s", tuple->value->cstring);
			tuple = dict_find(received, 4);
			snprintf(home_text, sizeof(home_text), "%s", tuple->value->cstring);
			// reset timer - 1 hr
			handle = app_timer_send_event(context, 3600000, HTTP_COOKIE);
			break;
		case 1:
			tuple = dict_find(received, 2);
			snprintf(home_text, sizeof(home_text), "%s", tuple->value->cstring);
			tuple = dict_find(received, 3);
			snprintf(away_text, sizeof(away_text), "%s", tuple->value->cstring);
			tuple = dict_find(received, 4);
			snprintf(status_text, sizeof(status_text), "%s", tuple->value->cstring);
			//set timer - 15 min
			handle = app_timer_send_event(context, 900000, HTTP_COOKIE);
			break;
		case 2:
		case 3:
			tuple = dict_find(received, 2);
			char *team = tuple->value->cstring;
			tuple = dict_find(received, 3);
			snprintf(home_text, sizeof(home_text), "%s %d", team, (int) tuple->value->int32);
			tuple = dict_find(received, 4);
			team = tuple->value->cstring;
			tuple = dict_find(received, 5);
			snprintf(away_text, sizeof(home_text), "%s %d", team, (int) tuple->value->int32);
			tuple = dict_find(received, 6);
			snprintf(status_text, sizeof(status_text), "%s", tuple->value->cstring);
			// Status 2 - reset timer 2 min. Status 3 - reset timer 1 hr
			if(game_status == 2) {
				handle = app_timer_send_event(context, 120000, HTTP_COOKIE);
			} else {
				handle = app_timer_send_event(context, 3600000, HTTP_COOKIE);
			}
			break;
		default:
			snprintf(away_text, sizeof(away_text), "BAD");
			snprintf(home_text, sizeof(home_text), "DATA");
			if (tuple->type == TUPLE_CSTRING) {
				snprintf(status_text, sizeof(status_text), ":%s", tuple->value->cstring);
			} else {
				snprintf(status_text, sizeof(status_text), ":%d", (int) tuple->value->int32);
			}
			handle = app_timer_send_event(context, 120000, HTTP_COOKIE); // Try again in 2 min
	}
	
	text_layer_set_text(&home_team_layer, home_text);
	text_layer_set_text(&away_team_layer, away_text);
	text_layer_set_text(&status_layer, status_text);
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
	static char time_text[] = "00:00";
	static char date_text[] = "Xxx Xxx 00";
	char *time_format;
	
	if(t->units_changed & DAY_UNIT) { // Only need to update the day if it's changed
		#ifdef DATE_MMDD
			string_format_time(date_text, sizeof(date_text), "%a %b %e", t->tick_time);
		#else
			string_format_time(date_text, sizeof(date_text), "%a %e %b", t->tick_time);
		#endif
		text_layer_set_text(&date_layer, date_text);
	}
	
	if(clock_is_24h_style()) {
		time_format = "%R";
	} else {
		time_format = "%I:%M";
	}
	
	string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);
	
	// Hack to deal with leading 0. Borrowed from Simplicity watchface
	if(!clock_is_24h_style() && (time_text[0] == '0')) {
		memmove(time_text, &time_text[1], sizeof(time_text)-1);
	}
	
	text_layer_set_text(&time_layer, time_text);
}

void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {
	request_score(ctx);
}

void handle_init(AppContextRef ctx) {
	(void)ctx;
	PblTm tm;
	PebbleTickEvent t;
	
	http_set_app_id(221359718); // Randomly generated number for app id
	
	// Initialize window and resources
	window_init(&window, "Caps Score");
	window_stack_push(&window, true /* Animated */);
	window_set_background_color(&window, BG_COLOR);
	resource_init_current_app(&WAS_APP_RESOURCES);

	// Initialize background image
	bmp_init_container(IMAGE_RES_ID, &image_container);
	layer_set_frame(&image_container.layer.layer, GRect(18, 52, 109, 80));
	layer_add_child(&window.layer, &image_container.layer.layer);

	// Initialize clock face
	text_layer_init(&time_layer, window.layer.frame);
	text_layer_set_text_color(&time_layer, TEXT_COLOR);
	text_layer_set_background_color(&time_layer, GColorClear);
	text_layer_set_text_alignment(&time_layer, GTextAlignmentCenter);
	layer_set_frame(&time_layer.layer, GRect(0, -8, 144, 49));
	text_layer_set_font(&time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
	layer_add_child(&window.layer, &time_layer.layer);
	
	// Initialize date
	text_layer_init(&date_layer, window.layer.frame);
	text_layer_set_text_color(&date_layer, TEXT_COLOR);
	text_layer_set_background_color(&date_layer, GColorClear);
	text_layer_set_text_alignment(&date_layer, GTextAlignmentCenter);
	layer_set_frame(&date_layer.layer, GRect(0, 38, 144, 21));
	text_layer_set_font(&date_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	layer_add_child(&window.layer, &date_layer.layer);
	
	// Initialize Home Team Layer
	text_layer_init(&home_team_layer, window.layer.frame);
	text_layer_set_text_color(&home_team_layer, TEXT_COLOR);
	text_layer_set_background_color(&home_team_layer, GColorClear);
	layer_set_frame(&home_team_layer.layer, GRect(2, 148, 55, 18));
	droid_sans_mono_18 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DROID_SANS_MONO_18));
	text_layer_set_font(&home_team_layer, droid_sans_mono_18);
	text_layer_set_text(&home_team_layer, "SCORE"); // Initial text
	layer_add_child(&window.layer, &home_team_layer.layer);
	
	// Initialize Away Team Layer
	text_layer_init(&away_team_layer, window.layer.frame);
	text_layer_set_text_color(&away_team_layer, TEXT_COLOR);
	text_layer_set_background_color(&away_team_layer, GColorClear);
	layer_set_frame(&away_team_layer.layer, GRect(2, 130, 55, 18));
	text_layer_set_font(&away_team_layer, droid_sans_mono_18);
	text_layer_set_text(&away_team_layer, "CAPS"); // Initial text
	layer_add_child(&window.layer, &away_team_layer.layer);
	
	// Initialize Status Layer
	text_layer_init(&status_layer, window.layer.frame);
	text_layer_set_text_color(&status_layer, TEXT_COLOR);
	text_layer_set_background_color(&status_layer, GColorClear);
	text_layer_set_text_alignment(&status_layer, GTextAlignmentCenter);
	layer_set_frame(&status_layer.layer, GRect(57, 130, 85, 36));
	layer_set_bounds(&status_layer.layer, GRect(0, 0, 85, 36));
	arial_narrow_bold_30 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ARIAL_NARROW_BOLD_30));
	text_layer_set_font(&status_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ARIAL_NARROW_BOLD_30)));//fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD));
	text_layer_set_text(&status_layer, "V1.0"); // Initial text
	layer_add_child(&window.layer, &status_layer.layer);
	
	// Set initial date & time
	get_time(&tm);
	t.tick_time = &tm;
	t.units_changed = MINUTE_UNIT | HOUR_UNIT | DAY_UNIT;
	handle_minute_tick(ctx, &t);
	
	// Register http callbacks
	http_register_callbacks((HTTPCallbacks){
		.failure=failed,
		.success=success,
		.reconnect=reconnect
	}, (void*)ctx);
	
	// Get scores
	request_score(ctx);
}

void handle_deinit(AppContextRef ctx) {
	bmp_deinit_container(&image_container);
	fonts_unload_custom_font(arial_narrow_bold_30);
	fonts_unload_custom_font(droid_sans_mono_18);
	app_timer_cancel_event(ctx, handle);
}

void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init,
		.deinit_handler = &handle_deinit,
		.tick_info = {
			.tick_handler = &handle_minute_tick,
			.tick_units = MINUTE_UNIT
		},
		.timer_handler = &handle_timer,
		.messaging_info = {
			.buffer_sizes = {
				.inbound = 124,
				.outbound = 256,
			}
		}
	};
	app_event_loop(params, &handlers);
}
