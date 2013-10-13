/*******************************************************************************
 * CONFIG.H
 *
 * Set or unset the varuous options to control the look and behavior of the 
 * watch face.
 ******************************************************************************/

/* The team for which the watch face will display data, specified as a three-
 * letter abbreviation. You still have to replace the logo image manually,
 * however. */
#define MY_TEAM "WAS"

/* Compiling for IOS or ANDROID. If compiling for iOS, the httpebble UUID will
 * be used. For Android, a unique UUID will be used. */
#define IOS

/* Specifies light or dark theme:
 * LIGHT_BG : White background, black text, image resource named LOGO_WHITEBG
 * BLACK_BG : Black backgorund, white text, image resource named LOGO_BLACKBG */
#define BLACK_BG

/* Specifies whether the day or month comes first in dates:
 * DATE_MMDD : "12/31" or "Dec 31"
 * DATE_DDMM : "31/12" or "31 Dec" */
#define DATE_MMDD

/* The URL of the server-side php file. */
#define SERVER_URL "http://chooch.us/jramboz/scores.php"

/* The http cookie. Set to some unique 32-bit integer to avoid misdirected
 * http responses. */
#define HTTP_COOKIE 954657632

