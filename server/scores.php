<?php

//define("TEAM_NAME", "Washington");

/* Takes a long-form team name and returns a three-letter abbreviation */
function shorten_team_name($team_name) {
	$short_name = "";
	
	switch($team_name) {
		case "Anaheim":
			$short_name = "ANA";
			break;
		case "Boston":
			$short_name = "BOS";
			break;
		case "Buffalo":
			$short_name = "BUF";
			break;
		case "Calgary":
			$short_name = "CGY";
			break;
		case "Carolina":
			$short_name = "CAR";
			break;
		case "Chicago":
			$short_name = "CHI";
			break;
		case "Colorado":
			$short_name = "COL";
			break;
		case "Columbus":
			$short_name = "CBS";
			break;
		case "Dallas":
			$short_name = "DAL";
			break;
		case "Detroit":
			$short_name = "DET";
			break;
		case "Edmonton":
			$short_name = "EDM";
			break;
		case "Florida":
			$short_name = "FLA";
			break;
		case "Los Angeles":
			$short_name = "LOS";
			break;
		case "Minnesota":
			$short_name = "MIN";
			break;
		case "Montreal":
		case "Montréal":
			$short_name = "MON";
			break;
		case "Nashville":
			$short_name = "NSH";
			break;
		case "New Jersey":
			$short_name = "NJD";
			break;
		case "NY Islanders":
			$short_name = "NYI";
			break;
		case "NY Rangers":
			$short_name = "NYR";
			break;
		case "Ottawa":
			$short_name = "OTT";
			break;
		case "Philadelphia":
			$short_name = "PHI";
			break;
		case "Phoenix":
			$short_name = "PHO";
			break;
		case "Pittsburgh":
			$short_name = "PIT";
			break;
		case "San Jose":
			$short_name = "SJS";
			break;
		case "St. Louis":
			$short_name = "STL";
			break;
		case "Tampa Bay":
			$short_name = "TAM";
			break;
		case "Toronto":
			$short_name = "TOR";
			break;
		case "Vancouver":
			$short_name = "VAN";
			break;
		case "Washington":
			$short_name = "WAS";
			break;
		case "Winnipeg":
			$short_name = "WPG";
			break;
	}
	
	return $short_name;
}

/* Takes a three-letter abbreviation and returns the long name */
function lengthen_team_name($team_name) {
	$long_name = "";
	
	switch($team_name) {
		case "ANA":
			$long_name = "Anaheim";
			break;
		case "BOS":
			$long_name = "Boston";
			break;
		case "BUF":
			$long_name = "Buffalo";
			break;
		case "CGY":
			$long_name = "Calgary";
			break;
		case "CAR":
			$long_name = "Carolina";
			break;
		case "CHI":
			$long_name = "Chicago";
			break;
		case "COL":
			$long_name = "Colorado";
			break;
		case "CBS":
			$long_name = "Columbus";
			break;
		case "DAL":
			$long_name = "Dallas";
			break;
		case "DET":
			$long_name = "Detroit";
			break;
		case "EDM":
			$long_name = "Edmonton";
			break;
		case "FLA":
			$long_name = "Florida";
			break;
		case "LOS":
			$long_name = "Los Angeles";
			break;
		case "MIN":
			$long_name = "Minnesota";
			break;
		case "MON":
			$long_name = "Montreal";
			break;
		case "NSH":
			$long_name = "Nashville";
			break;
		case "NJD":
			$long_name = "New Jersey";
			break;
		case "NYI":
			$long_name = "NY Islanders";
			break;
		case "NYR":
			$long_name = "NY Rangers";
			break;
		case "OTT":
			$long_name = "Ottawa";
			break;
		case "PHI":
			$long_name = "Philadelphia";
			break;
		case "PHO":
			$long_name = "Phoenix";
			break;
		case "PIT":
			$long_name = "Pittsburgh";
			break;
		case "SJS":
			$long_name = "San Jose";
			break;
		case "STL":
			$long_name = "St. Louis";
			break;
		case "TAM":
			$long_name = "Tampa Bay";
			break;
		case "TOR":
			$long_name = "Toronto";
			break;
		case "VAN":
			$long_name = "Vancouver";
			break;
		case "WAS":
			$long_name = "Washington";
			break;
		case "WPG":
			$long_name = "Winnipeg";
			break;
	}
	
	return $long_name;
}

/* *****************************************************************************
 * JSON FORMAT
 * 
 * The data can be in one of four possible formats, depending on what data was
 * found. If no game was found in the XML, the JSON file will specift the date
 * and time of the next game. If a game is found but not started, the JSON file
 * will have the start time as the status. If a game is in progress, we'll also
 * have scores and period status. If a game is finished, the data will be
 * identical to a game in progress, just with a different status code. The reason
 * for this is so that the Pebble watchapp can adjust its polling time; if the
 * game is in progress, you want to poll more frequently. If it's finished, you
 * can stop doing that.
 * 
 * The first node will have the type of data:
 * 		0 - No game found in XML. Schedule data follows.
 * 		1 - Game today, but not started yet.
 * 		2 - GAME ON!
 * 		3 - GAME OVER!
 * 
 * The Pebble watchapp will have to implement separate parsers for each data
 * type.
 * 
 * NO GAME FOUND (DATA FROM SCHEDULE) FORMAT
 * {
 * 	"1": 0,
 * 	"2": Opponent team,
 * 	"3": Date,
 * 	"4": Time
 * }
 * 
 * GAME TODAY, NOT STARTED FORMAT
 * {
 * 	"1": 1,
 * 	"2": Home Team,
 * 	"3": Away Team,
 * 	"4": Start time
 * }
 * 
 * GAME ON! FORMAT
 * {
 * 	"1": 2,
 * 	"2": Home team,
 * 	"3": Home team score,
 * 	"4": Away team,
 * 	"5": Away team score,
 * 	"6": Period status
 * }
 * 
 * GAME OVER! FORMAT
 * {
 * 	"1": 3,
 * 	"2": Home team,
 * 	"3": Home team score,
 * 	"4": Away team,
 * 	"5": Away team score,
 * 	"6": Final status (F, F/OT, F/SO)
 * }
 */

$options = json_decode(file_get_contents('php://input'), true);
if(!$options) die();

$scores = simplexml_load_file('http://wu.apple.com/nhl/bottomline/xml/scores');
if (!$scores) {
	exit('Failed to load');
}

$my_team = lengthen_team_name($options[1]);
$home_team = "";
$away_team = "";
$home_score = 0;
$away_score = 0;
$game_status = "";
$game_found = false;
$response = array();

foreach($scores->GAME as $game) { // Loop through the games to find the one for TEAM_NAME
	if($game->AWAY->TEAM == $my_team or $game->HOME->TEAM == $my_team) { // Only care about games where Washington is playing
		$game_found = true;
		
		// Get the team names, shortened to three-letter abbreviations
		$home_team = shorten_team_name($game->HOME->TEAM);
		$away_team = shorten_team_name($game->AWAY->TEAM);
		
		// Get the scores
		$home_score = (int) $game->HOME->SCORE;
		$away_score = (int) $game->AWAY->SCORE;
		
		// Get the status of the game
		switch($game->STATUSID) {
			/* Game hasn't started yet. STATUS will hold start time of game. */
			case 1:
				$response[1] = 1;
				// All times appear to be given in ET. Let's just strip that off.
				$game_status = str_replace(" ET", "", (string) $game->STATUS); 
				if($options[3] == 12) { // Display time in 12 hr format
					// Date is already in proper format. Just need to shorten it to fit on the Pebble display.
					// Yeah, I don't know of a case when a game will start in the AM, but I put it in just in case. Might also be useful for later timezone conversion.
					$game_status = str_replace(" PM", "P", $game_status);
					$game_status = str_replace(" AM", "A", $game_status);
				} else { // convert to 24 hr format
					$game_time = date_create_from_format("h:i a", $game_status);
					$game_status = date_format($game_time, "H:i");
				}
				break;
			/* Game in progress. Need to determine which period. STATUS will be
			 * in the form 20:00 IN 1ST. Have seen weird cases where first
			 * number will be negative.
			 * 
			 * Possible period designators are 1ST, 2ND, 3RD, OT, 2ND OT. 2ND OT
			 * is a shootout in the regular season. Multiple overtimes possible
			 * for postseason.
			 * 
			 * For now, we'll assume regular (or pre) season games. Will update
			 * to deal with postseason later. */
			case 2:
				$response[1] = 2;
				$status_pieces = explode(" ", $game->STATUS);
				switch(count($status_pieces)) { // Determine if we're in mutli-overtime
					case 3: // In regulation or 1st OT
						$game_status = $status_pieces[2]; // 1ST, 2ND, 3RD, or OT. For post-season, should update so that OT reads 1OT.
						break;
					case 4: // In SO or multiple OT
						$game_status = "SO"; // In regular season, the only time we'll get more than 3 pieces is for 2ND OT == SO.
						break;
				}
				break;
			/* Intermission. STATUS is "END OF [1ST|2ND|3RD|OT]". Guessing on
			 * the OT, haven't actually seen it occur yet.*/
			case 22: 
				$response[1] = 2;
				$status_pieces = explode(" ", $game->STATUS);
				switch($status_pieces[2]) {
					case "1ST":
						$game_status = "1INT";
						break;
					case "2ND":
						$game_status = "2INT";
						break;
					case "3RD":
						$game_status = "3INT";
						break;
					case "OT":
						$game_status = "E-OT";
						break;
				}
				break;
			/* Game finished. Possible values for STATUS are "FINAL",
			 * "FINAL - OT", or "FINAL - 2ND OT" (or more OTs for postseason) */
			case 3:
				$response[1] = 3;
				$status_pieces = explode(" ", $game->STATUS);
				switch(count($status_pieces)) {
					case 1: // FINAL
						$game_status = "FINAL";
						break;
					case 3: // FINAL - OT
						$game_status = "F/OT";
						break;
					case 4: // FINAL - 2ND OT
						$game_status = "F/SO";
						break;
				}
				break;
		}
		
		break; // No need to bother checking the other games for the day
	}
}

// If no TEAM_NAME game was found in the scores, get the next scheduled game.
if(!$game_found) {
	$response[1] = 0;
		
	// Schedule is stored in a CSV file named "schedule_XXX.csv", where XXX is 
	// the three-letter abbreviation for the team name.
	// If you want to customize this for your team, you'll need to tweak this parser. Looks like every team has their csv in a different format.
	$short_name = shorten_team_name($my_team);
	$filename = "schedule_{$short_name}.csv";
	$schedule = file($filename);
	if ($schedule) {
		unset($schedule[0]); // We don't need the column names.
		
		foreach($schedule as $row_csv) {
			$row = str_getcsv($row_csv);
			$game_date_time = date_create_from_format("m/d/Y h:i a", "{$row[0]} {$row[2]}");
			if($game_date_time > date_create()) {
				// Put the date and time into the JSON file
				if($options[2] == "MMDD") {
					$response[3] = date_format($game_date_time, "n/j"); // Date: MM/DD
				} else {
					$response[3] = date_format($game_date_time, "j/n"); // Date: DD/MM
				}
				if ($options[3] == 12) {
					$response[4] = date_format($game_date_time, "g:i A"); // Time - 12 hr format
					// Need to shorten the time to fit the Pebble display
					$response[4] = str_replace(" PM", "P", $response[4]);
					$response[4] = str_replace(" AM", "A", $response[4]);
				} else {
					$response[4] = date_format($game_date_time, "H:i"); // Time - 24 hr format
				}
				//Figure out opposing team
				preg_match("/(.+) at (.+)/", $row[3], $teams);
				if($my_team == "Montreal") { $my_team = "Montréal"; } // Why you gotta be like that, Montreal?
				if($teams[1] == $my_team) {
					$response[2] = "@" . shorten_team_name($teams[2]);
				} else {
					$response[2] = shorten_team_name($teams[1]);
				}
				
				break; // only need the first game greater than the current date
			}
		}
	}
}

// If a game was found, build the response array and print it as a JSON file. 
// $response[1] will hold the format to encode the data in, as specified above.
switch($response[1]) {
	case 0:
		break; // Already set everything when we parsed the schedule above
	case 1:
		$response[2] = $home_team;
		$response[3] = $away_team;
		$response[4] = $game_status;
		break;
	default: // 2 or 3, both encoded the same
		$response[2] = $home_team;
		$response[3] = $home_score;
		$response[4] = $away_team;
		$response[5] = $away_score;
		$response[6] = $game_status;
		break;
}

// If you're running PHP 5.4 or greater, you can use the JSON_UNESCAPED_SLASHES argument and get rid of the str_replace.
$json = json_encode($response);//, JSON_UNESCAPED_SLASHES);
echo str_replace('\/', '/', $json);

?>
