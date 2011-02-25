<?php

namespace Config {

	class Prototype {};

  class Routes {
    /**
     * List of Routes for the Broker
     *
     * @var array
     */
    public static $list = array(
    );
    /**
     * File to display when requested resource is not found.
     *
     * @var string
     */
    public static $notFound = '404.inc';
    /**
     * Add Route To The Broker
     * The first route matched is executed, all others are ignored.
     * @param int $priority Higher number means higher priority.
     * @param regex $pattern Regular Expression to match against APP_URI
     * @param closure $action Anonymous function to call if matched. Passes the $matches array from preg_match.
     */
    public static function add ($priority, $pattern, $action, $label = "No Label") {
      self::$list[$priority][$pattern] = array(
        	'action' => $action,
        	'label' => $label);
      arsort(self::$list);
    }
    /**
     * Add Default Routes
     * You should never need to call this. It adds our default routes to the route list.
     * Any ending directory on a request (such as info in http://foo.bar/baz/info) will check for "info.inc" and "info.php".
     * Attempting to go to the app root will look for index.inc or index.php
     * @return boolean
     */
    public static function addDefaultRoutes() {
      self::add(0, '/([A-Za-z0-9]+)[\/]*$/',
      function($matches) {
        if (!@include_once($matches[1].".inc")) {
          if (!@include_once($matches[1].".php")) {
            return false;
          }
        }
        return true;
      },
        'Default URI Component'
      );
      self::add(0, '/^[\/]*$/', function($matches) {
            if (!@include_once('index.inc')) {
              if (!@include_once('index.php')) {
                return false;
              }
            }
            return true;
          }, 'Default Index Component');
    }
  }
}

?>