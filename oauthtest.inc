<?php
session_start();
if (!isset($_GET['oauth_token']) && $_SESSION['state']==1) {
	$_SESSION['state'] = 0; // no authentication yet.
}

$test = new oauthtest();


if(!isset($_GET['oauth_token']) && !$_SESSION['state']) {
$_SESSION['state'] = 1; // 1 = we have tried to authenticate
$test::$driver->init();
$requestToken = $test::$driver->getRequestToken();
/* Save temporary credentials to session. */
$_SESSION['oauth_token'] = $token = $requestToken['oauth_token'];
$_SESSION['oauth_token_secret'] = $requestToken['oauth_token_secret'];
$_SESSION['rdio_login_url'] = $loginURL = $requestToken['login_url'];
/* If last connection failed don't display authorization link. */
switch ($test::$driver->http_code) {
  case 200:
    /* Build authorize URL and redirect user to Twitter. */
    $url = $test::$driver->getAuthorizeURL($token, $loginURL);
    header('Location: ' . $url);
    break;
  default:
    /* Show notification if something went wrong. */
    echo 'Could not connect to Rdio. Refresh the page or try again later.';
		var_dump($test::$driver);
}
exit;
/*
$test::$driver->get_request_token()
      ->get_user_authorization();
exit;
*/
} else if (isset($_GET['oauth_token']) && $_SESSION['state'] == 1) {
	var_dump($_GET);
	$_SESSION['state'] = 2;
	$test::$driver->init($_SESSION['oauth_token'], $_SESSION['oauth_token_secret']);
	/* Request access tokens from twitter */
$access_token = $test::$driver->getAccessToken($_REQUEST['oauth_verifier']);
echo "Tok:";
var_dump($access_token);
/* Save the access tokens. Normally these would be saved in a database for future use. */
$_SESSION['access_token'] = $access_token;

/* Remove no longer needed request tokens */
unset($_SESSION['oauth_token']);
unset($_SESSION['oauth_token_secret']);

	/*$test::$driver->get_access_token($_GET['oauth_token'], $_GET['oauth_verifier']);
	$_SESSION['token'] = $test::$driver->access_token;
	$_SESSION['secret'] = $test::$driver->token_secret;
	$_SESSION['state'] = 2;
	var_dump($test::$driver->access_token);
	//	header("Location: ".APP_URI);
	exit;
*/
} else {
	var_dump($_GET);
$access_token = $_SESSION['access_token'];
$test::$driver->init($access_token['oauth_token'], $access_token['oauth_token_secret']);


	/*
	$test::$driver->access_token = $_SESSION['token'];
	$test::$driver->token_secret = $_SESSION['secret'];
*/
}
var_dump($_SESSION);

$user = $test::$driver->post('http://api.rdio.com/1/',
																	 array(
																				 'method' => 'currentUser'
																	      )
																	 );
var_dump($user);
/*
$playlists = $test::$driver->post('http://api.rdio.com/1/',
																	 array(
																				 'method' => 'getPlaylists'
																	      )
																	 );
var_dump($playlists);
foreach($playlists->result->owned as $playlist) {
print $playlist->name.' - '.$playlist->key.'<br />';

}
*/

/* uncomment and add keys to the album array
$albumArray = array();
foreach($albumArray as $albumID) {
  $album = $test::$driver->post('http://api.rdio.com/1/',
                                     array(
                                           'method' => 'getTracksForAlbumInCollection',
                                           'album' => $albumID,
                                           'user' => $user->result->key
                                          )
                                     );

  //var_dump($album);
  $trackList = array();
  foreach($album->result as $track) {
    $trackList[] = $track->key;
  }

  $results = $test::$driver->post('http://api.rdio.com/1/',
                                     array(
                                           'method' => 'addToPlaylist',
                                           'playlist' => 'p86083',
                                           'tracks' => implode(",", $trackList)
                                          )
                                     );
  $trackList = array();
  var_dump($results);
}
end album array function */

/*
$results = $test::$driver->post('http://api.rdio.com/1/',
																	 array(
																				 'method' => 'getAlbumsInCollection',
																				 'types' => 'Album',
																				 'user' => $user->result->key
																	      )
																	 );
//var_dump($results->result[0]);
// var_dump($results);

foreach($results->result as $album) {
print $album->artist.': '.$album->name.' - '.$album->key.'<br />';

}


*/



/*
print "Driver is: ";
var_dump($test::$driver);
print "Result is: ";
var_dump($results->result->results[0]);
$tracks = $results->result->results[0]->trackIDs;

$playlist = $test::$driver->post('http://api.rdio.com/1/',
																	 array(
																				 'method' => 'createPlaylist',
																				 'name' => 'TestClutter',
																				 'description' => 'Cross fingers',
																				 'tracks' =>  "t".implode(',t',$tracks)
																	      )
																	 );
var_dump($test::$driver);
var_dump($playlist);
*/
?>