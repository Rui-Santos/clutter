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
	// It's up to the mixin if they care about the containing object. But it will be set no matter what.
	public function setContainer(&$container);
	// This should return an array of features provided by the mixin.
	public function provides($name = NULL);
}

interface Driver extends Mixin {
	//function to return a result set.
	public function query();
}

?>