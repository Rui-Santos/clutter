<?php
namespace Config;
if (!class_exists('\Config\Debug', FALSE)) {
  class Debug extends \Config\prototype {
  	/**
	 * Enable Debugging.
     * Enable debugging when there are no other configs.
     * @var bool
     */

    const enable = TRUE;
    /**
     * Debug Output
     * Enter "console" for old school firephp output. Enter "companion" for fancy schmancy output.
     * @var unknown_type
     */
    const output = "console";
  }
}

if (!class_exists('\Config\Broker', FALSE)) {
  class Broker extends \Config\prototype {
  	/**
		 * Enable Default Routes.
		 * Use one of the default CraftPHP routes.
     * @var bool
     */
    const enableDefaultRoutes = TRUE;
  }
}

?>