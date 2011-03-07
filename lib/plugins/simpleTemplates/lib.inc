<?php

namespace Craft\Plugins;

class simpleTemplates extends \Craft\Object implements \Craft\Interfaces\iPlugin {
  public function __construct() {
  }

  public static function installPlugin ( ) {
    // we have no specific install instructions.
    return true;
  }
  public static function isInstalled ( ) {
    $dir = realpath( dirname( __FILE__ ) ) . '/';
    include_once ($dir . 'template.inc');
    // and thus nothing to test to determine whether we're installed beyond just including this file.
    return true;
  }
}



?>