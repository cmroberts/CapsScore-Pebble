CapsScore-Pebble
================

This is a Pebble watch face that displays scores and schedules for the Washington Capitals NHL team. It uses [httpebble](https://github.com/Katharine/httpebble-watch) to communicate with the smartphone and retrieve data. You must have httpebble installed on your phone in order for this watch face to retrieve data.

On a personal note, I'm only an amateur coder at best. I'm sure that many of you will look at the code here (both the PHP for the server and the C for the watch face) and gnash your teeth and wail at my inefficiency. All I can say is that I'm open to feedback and suggestions for improvement. Making this watch face has been a learning experience for me, and I hope it will continue to be one in the future.

## Screenshots

(Apologies for lack of decent screenshots. I'm working on getting some better ones.)

Day of a game, white background:

![](http://i.imgur.com/E6E3ZrX.jpg)

Day of a game, black background:

![](http://i.imgur.com/OYzrNRa.jpg)

Game in progress, white background:

![](http://i.imgur.com/5mjGV9J.jpg)

Game in progress, black background:

![](http://i.imgur.com/EsqWr73.jpg)

Game scheduled, white background:

![](http://i.imgur.com/2kdEdeg.jpg)

Game scheduled, black background:

![](http://i.imgur.com/I8WbGIZ.jpg)

## Features

* Available with black or white backgrounds
* Fetches and displays scores and schedules via httpebble
* Can run side-by-side with other httpebble watchfaces
* Three display modes:
    - During a game, the watch face displays the current score and period (1ST, 2ND, 3RD, 1INT (1st intermission), 2INT, 3INT, OT, SO, FINAL, F/OT, F/SO)
    - On the day of a game, the watch face displays the teams and start time
    - If no game is scheduled for the day, the watch face displays the date, start time, and opponent for the next scheduled game
* Fonts used:
    - Clock: Roboto Bold
    - Date: Roboto Condensed
    - Scores: Droid Mono
    - Game Status / Scheduled Date/Time: Arial Narrow Bold
* Update time automatically adjusts depending on whether a game is in progress or scheduled:
    - If there's no game for the day, data is updated every hour
    - Before a game starts, data is updated every 15 minutes (the better to catch the start time)
    - During a game, scores are updated every 2 minutes
    - After a game is over, updates go back to every hour
* Supports 12 or 24 hour displays (and will also display game start times in the preferred format!)
* Displays upcoming dates in MM/DD format, though it can be recompiled to DD/MM format with a simple setting change (see the Customization section below)

----

## Known Issues

* Cannot currently handle post-season data. I plan to update it before the playoffs begin, but I wanted to get an ititial release out.
* All game start times are displayed in Eastern Time. I'd like to try to handle converting to the Pebble's timezone in the future, but this is a low priority unless people clamor for it.
* Every once in a while, a timer seems not to get set correctly, or the watch face doesn't seem to get back its initial data pull. I can't figure out what's causing this, but you can always manually refresh by switching out of then back into the watchface.

----

## Download

### v1.0

[White Background](http://chooch.us/jramboz/CapsScore-1.0-WhiteBG.pbw)

[Black Background](http://chooch.us/jramboz/CapsScore-1.0-BlackBG.pbw)

----

## Customization

If for some crazy reason the Washington Capitals are not your favorite NHL team, you're welcome to customize this watch face for your preferred franchise. Below are some notes to help you with this.

**Note:** This watch face only handles NHL scores. If you'd like to build on this to create watch faces for other sports, please feel free! You'll need to find your own data source, however.

----

### Pebble Side

Most of the changes you'll need to make to the Pebble code are covered in `config.h`. Specifically, you'll want to change the following `#define`d items, as necessary:

#### Team Name

`#define MY_TEAM "WAS"`

The quoted string should be the three-letter abbreviation (in all caps) for the NHL team for which you want to display data. I believe the team abbreviations are pretty standard, but I've put a list of them at the end of this file, just in case.

#### Operating System Support

`#define IOS`

Set to either `IOS` or `ANDROID`. Due to iOS limitations, all httpebble apps have to use the same UUID, though Android doesn't have this restriction. Setting this to `IOS` will use the pre-defined iOS UUID, while setting it to `ANDROID` will use a unique UUID. For more information, see the [httpebble readme](https://github.com/Katharine/httpebble-watch/blob/master/README.md).

#### Color Scheme

`#define BLACK_BG`

Set to either `BLACK_BG` for white text on black background or `WHITE_BG` for black text on white background. See also the notes on customizing the image resources, below.

#### Date Format

`#define DATE_MMDD`

This defines what format you want dates for upcoming games to be displayed in. `DATE_MMDD` produces dates such as 12/31, and `DATE_DDMM` gives 31/12. I hear some of you prefer your dates that way, eh?

**Note:** There's no need to set an option for 12 or 24 hour time. The watch face will read this preference automatically from the watch's settings. (As an added bonus, that means it can be toggled without having to recompile. Hooray!)

#### Server URL

`#define SERVER_URL "http://chooch.us/jramboz/scores.php"`

This is the URL of the PHP script that will deliver the data. More info on customizing this below. If you want to use my URL, you can, but it won't have schedule data for your team unless you want to send me a CSV file adhering to the same format as the Washington schedule data. Again, see below.

#### HTTP Cookie

`#define HTTP_COOKIE 954657632`

Set this to some random 32-bit integer. This helps keep the watch from handling misdirected http responses. For more information, see the [httpebble readme](https://github.com/Katharine/httpebble-watch/blob/master/README.md).

#### Resources

Obviously, you're going to want to put your team's logo on the watch face. The logo used for white background should have a resource name of `LOGO_WHITEBG` and the one for a black background should be named `LOGO_BLACKBG`. You'll also likely want to replace the `ICON` as well.

The current logos are 109 x 80, and the icon is 24 x 28. You may need to adjust the size and positioning depending on your logo. The current logo area overlaps the date, but since the Caps logo has a nice open space at the top this isn't an issue for me.

I believe there's a more clever way to do this using transparent PNGs, but I must have been doing it wrong because I couldn't manage to make it work.

----

### Server Side

The PHP file the watch face interacts with is `server\scores.php`. It requires PHP 5.3 or greater (though with some tweaking you could get it to work with earlier versions).

#### Server / Pebble Communications

As is the case with all httpebble applications, the server and watch face communicate with each other via JSON. There's not really much to customize here, unless you plan to add functionality; I'm detailing it here for ease of reference.

The Pebble to server JSON is formatted as follows:

```JSON
{
   "1":"WAS",  // Three-letter abbreviation for your team
   "2":"MMDD", // Date format preference. MMDD or DDMM.
   "3":12      // Time format preference. 12 or 24.
}
```

The server script will automatically parse these in to the `$options` array, such that they will be available as `$options[1]`, `$options[2]`, and `$options[3]`. The team name is automatically used when parsing the live score data, so all you have to do is make sure the correct abbreviation is passed in. However, this is *only for games currently in progress or scheduled for the same day*; you will have to customize the parsing of schedule data (see below).

The response from the server to the Pebble will depend on what kind of data is being returned (i.e., game in progress, game finished, game later today, or game scheduled for the future). The first element of the JSON response indicates what type of data is being returned:

* `0`: Data for scheduled future
* `1`: Game later today
* `2`: Game in progress
* `3`: Game completed

Game scheduled format:

```JSON
{
   "1":0,       // Data type code
   "2":"@CBS",  // Opponent team, prefixed by @ if an away game
   "3":"10/19", // Date of next game
   "4":"7:00P"  // Time of next game, eastern time
}
```

Game later today format:

```JSON
{
   "1":1,      // Data type code
   "2":"WAS",  // Home Team
   "3":"CBS",  // Away Team
   "4":"7:00P" // Start time
}
```

Game in progress format:

```JSON
{
   "1":2,     // Data type code
   "2":"WAS", // Home team
   "3":3,     // Home team score
   "4":"CBS", // Away team
   "5":1,     // Away team score
   "6":"2ND"  // Period status
}
```

Game finished format:

```JSON
{
   "1": 3,      // Data type code
   "2":"WAS",   // Home team
   "3":5,       // Home team score
   "4":"CBS",   // Away team,
   "5":3,       // Away team score,
   "6":"FINAL", // Final status (FINAL, F/OT, F/SO)
}
```

#### Schedule Data

By default, the script looks for schedule data in a CSV file named `schedule_XXX.csv`, where `XXX` is the three-letter abbreviation (in all caps) for the team. I got the data for Washington from the schedule page on their website, and wrote the parser based on that. I thought I'd be nice and download the other teams' schedules as well. But, to my surprise, it turns out that teams use a wide variety of CSV formats: columns are in different places, data is formatted differently, etc. Rather than try to write a separate parser for each one, reformat all of them, or otherwise find a workaround, I decided to just throw my hands up and leave it to whoever wants to do the customization.

The magic function in the PHP script is `parse_schedule($options)`. You can get your team's CSV schedule from their website and parse it, or you can put all the info in mysql database, or you can cast mystical runes to divine the future. All that matters is that you return an array as follows:

* `$result[2]`: Opposing team abbreviation
* `$result[3]`: Next game date
* `$result[4]`: Next game time

The elements in `$options` should be the same as the ones passed to the script from the Pebble, as described above. I recommend you respect the preferred date and time format options, though I suppose that's up to you.

----

## Contact Me

If you have any questions or need help with customization, send me a message through [GitHub](https://github.com/jramboz) or the [Pebble Forums](http://forums.getpebble.com/profile/33804/JasonRamboz).

Have fun, and Go Caps!!

----

## Appendix - Team Abbreviations

Here are the three-letter abbreviations used by the watch face and server.

* `ANA`: Anaheim
* `BOS`: Boston
* `BUF`: Buffalo
* `CGY`: Calgary
* `CAR`: Carolina
* `CHI`: Chicago
* `COL`: Colorado
* `CBS`: Columbus
* `DAL`: Dallas
* `DET`: Detroit
* `EDM`: Edmonton
* `FLA`: Florida
* `LOS`: Los Angeles
* `MIN`: Minnesota
* `MON`: Montreal
* `NSH`: Nashville
* `NJD`: New Jersey
* `NYI`: NY Islanders
* `NYR`: NY Rangers
* `OTT`: Ottawa
* `PHI`: Philadelphia
* `PHO`: Phoenix
* `PIT`: Pittsburgh
* `SJS`: San Jose
* `STL`: St. Louis
* `TAM`: Tampa Bay
* `TOR`: Toronto
* `VAN`: Vancouver
* `WAS`: Washington
* `WPG`: Winnipeg
