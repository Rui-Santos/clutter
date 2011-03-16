<?php
namespace Craft\Plugins {

class rdioDriver extends \Craft\Object implements \Craft\Interfaces\Plugin {
	public static function installPlugin() {
		return true; // there is nothing to install.
	}
	public static function isInstalled() {
		$dir = realpath( dirname( __FILE__ ) ) . '/';
    define('RDIODIR', $dir);
    include_once(RDIODIR . 'rdio.inc');
		return true;
	}

}


}
?>