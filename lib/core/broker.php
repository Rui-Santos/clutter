<?php
/* Welcome to the CraftPHP broker. This is the file that sets up everything else.
 * It is what makes sure everything is automatically included without your
 * having to worry your pretty little head. Also, you should never have to
 * change anything in here.
 */


/* Who are we and where did we come from? */
list($uri, $query) = explode('?', $_SERVER['REQUEST_URI']);
$path = str_replace('\\','/',realpath(__DIR__."/../.."));
if (strstr($path, ':')) {
	list($drive, $path) = explode(':', $path); // windows compatibility
}
/**
 * APP_URI
 * Request URI minus the query string ...
 * @var unknown_type
 */
define('APP_URI', $uri);

/**
 * APP_PATH
 * The local filesystem path for the application ...
 * @var string
 */
define('APP_PATH', $path.DIRECTORY_SEPARATOR); //ALL PATHS end with a slash.

/**
 * APP_LIB
 * CraftPHP core location ...
 * @var string
 */
define('APP_LIB', APP_PATH."lib".DIRECTORY_SEPARATOR);

/**
 * APP_REQ
 * The local filesystem path to the requested URI
 * @var string
 */
define('APP_REQ', realpath(APP_PATH.$uri));

$pathinfo = pathinfo(APP_REQ);


include_once(APP_LIB."core".DIRECTORY_SEPARATOR."lib.inc");

\Craft\addIncludePath(APP_PATH, FALSE);
\Craft\addIncludePath(APP_LIB, TRUE, array('firebug'));

if (file_exists(APP_PATH.DIRECTORY_SEPARATOR.'plugins')) {
	/**
	 * APP_PLUGIN_DIR
	 * CraftPHP plugins directory ...
	 * @var string
	 */
	define('APP_PLUGIN_DIR', APP_PATH."plugins".DIRECTORY_SEPARATOR);
	\Craft\addIncludePath(APP_PLUGIN_DIR);
} else {
	define('APP_PLUGIN_DIR', FALSE);
}
unset($uri,$query,$drive,$path);


/* Include default Configs here. You can always include others by hand. */
if (file_exists(APP_PATH."config.inc")) {
  \Craft\loadConfigINC(APP_PATH."config.inc");
} elseif (file_exists(APP_PATH."config.ini")) {
  \Craft\loadConfigINI(APP_PATH."config.ini");
}
if (is_dir(APP_PATH."config")) {
  /**
   * APP_CONFIG_DIR
   * Where application configuration files live ...
   * @var unknown_type
   */
  define('APP_CONFIG_DIR', APP_PATH."config".DIRECTORY_SEPARATOR);
  \Craft\loadConfigDir(APP_CONFIG_DIR);
} else {
  define('APP_CONFIG_DIR', FALSE);
}

\Craft\loadConfigINC(APP_LIB."core".DIRECTORY_SEPARATOR."defaultConfig.inc");


//TODO
/* if (\Config\Debug::enable) { */
/*   include_once(APP_LIB."debug".DIRECTORY_SEPARATOR."Debug.inc"); */
/* } else { */
/*   include_once(APP_LIB."debug".DIRECTORY_SEPARATOR."noDebug.inc"); */
/* } */



/* Include default application stuff here. */
if (file_exists(APP_PATH."app.inc")) {
  unset($pathinfo);
  include_once(APP_PATH."app.inc");
}


/* Insinuate into existing .php files... */
if (!stristr($pathinfo['dirname'], APP_LIB)) { //as long as we're not in the APP_LIB directory
  if ((preg_match('/\.php$/', APP_REQ) && file_exists(APP_REQ))) {
    unset($pathinfo);
    include_once(APP_REQ);
    exit;
  } elseif (is_dir(APP_REQ) && file_exists(realpath(APP_REQ.'/index.php'))) {
    unset($pathinfo);
    include_once(realpath(APP_REQ.'/index.php'));
    exit;
  }
}

unset($pathinfo);

/* If we've gotten this far, time to do our thing and include an .inc file. */


if (\Config\Broker::enableDefaultRoutes === TRUE) {
	\Config\Routes::addDefaultRoutes();
}

$matched = FALSE;
foreach (array_reverse(\Config\Routes::$list) as $priority => $routes) {
	if (!$matched) { 
 foreach ($routes as $pattern => $action) {
    $pMatches = array();
    if (!$matched && preg_match($pattern, APP_URI, $pMatches)) {
			$matched = $action['action']($pMatches); // we just called the
																							 // lambda that matches
																							 // our url pattern. If
																							 // it returns false, it
																							 // wasn't a real match.
		}
			if (!$matched) {
				//Insert debug info here.
			} else {
			break;
			}
  }
}
}
  if (!$matched) {
    header("HTTP/1.0 404 Not Found");
    if (!@include(\Config\Routes::$notFound)) {
      echo "<h1>404 -- Requested file not found.</h1>";
    }
    exit;
  }

?>