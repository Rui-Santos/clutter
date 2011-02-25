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

interface Mixin {
	// This should return an array of features provided by the mixin.
	public static function provides();
}

?>