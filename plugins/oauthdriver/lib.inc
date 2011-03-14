<?php
namespace Craft\Plugins {

class oAuthDriver extends \Craft\Object implements \Craft\Interfaces\Plugin {
	public static function installPlugin() {
		return true; // there is nothing to install.
	}
	public static function isInstalled() {
		$dir = realpath( dirname( __FILE__ ) ) . '/';
    define('OAUTHDIR', $dir);
    include_once(OAUTHDIR . 'oauth.inc');
		return true;
	}

}


}
?>