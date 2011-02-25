<?php

namespace Craft\Interfaces;

// we tend to just do "print $template";
interface Template {
  public function __toString ( );
  public function setVars(array $vars);
  public function set($variableName, $value);
}

interface Plugin {
  public static function installPlugin ( );
  public static function isInstalled ( );
}



?>