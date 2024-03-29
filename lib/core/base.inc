<?php

namespace Craft {
function enablePlugin($name) {
    $loaded = FALSE;
    if (file_exists(APP_LIB."plugins".DIRECTORY_SEPARATOR.$name.DIRECTORY_SEPARATOR."lib.inc")) {
      //internal plugin.
      $loaded = include_once(APP_LIB."plugins".DIRECTORY_SEPARATOR.$name.DIRECTORY_SEPARATOR."lib.inc");
    } else if (APP_PLUGIN_DIR !== FALSE && file_exists(APP_PLUGIN_DIR.$name.DIRECTORY_SEPARATOR."lib.inc")) {
      $loaded = include_once(APP_PLUGIN_DIR.$name."/lib.inc");
    }
    if (!$loaded) {
      throw new CraftPluginException("Unable to locate plugin $name.");
    } else {
      $ns = "\\Craft\\Plugins\\".$name;
      $plugin = new $ns;
      if ($plugin::isInstalled()) {
        // do nothing.
      } else {
        $plugin::installPlugin();
      }
    }
  }

  function loadConfig($name) {
    if (! loadConfigINC($name.".inc")) {
      //check for plugin specific configs. After we write plugin handling code.
      //then check to see if we've got a directory...
      if (is_dir($name)) {
        loadConfigDir($name);
      }
    }
  }

  function loadConfigDir($name) {
    $d = dir($name);
    if (substr($name,-1) != DIRECTORY_SEPARATOR) {
      $name .= DIRECTORY_SEPARATOR;
    }
    while (false !== ($file = $d->read())) {
      if (preg_match('/^.*\.inc$/', $file)) {
        loadConfigINC($name.$file);
      } elseif (preg_match('/^.*\.ini$/',$file)) {
        loadConfigINI($name.$file);
      }
    }
    $d->close();
  }

  function loadConfigINC($file) {
    /* it would be nice to add sanity checking here to make sure config classes extend Config and are in the \Config namespace.
     * Unfortunately, that would add too much overhead to run every single time, so it'll have to wait until later.
     */
    return include_once($file);
  }

  function loadConfigINI($file) {
    $vals = parse_ini_file($file,TRUE);
    $isEvil = 'namespace \Config;
  class '.basename($file).' extends \Config\INI\Prototype {
  ';
    foreach ($vals as $key => $val) {
      if (is_array($val)) {
        $isEvil .= 'public static $'.$key.' = '.var_export($val, TRUE).';
        ';
      } else {
        $isEvil .= 'const '.$key.' = '.var_export($val, TRUE).';
        ';        
      }
    }
    $isEvil .= '};';
    eval($isEvil);
    return true;
  }

  function addIncludePath($directory, $recurse = TRUE, $exclude = array()) {
    if (is_array($directory)) {
      foreach($directory as $dir) {
        addIncludePath($dir, $recurse);
      }
    } else {
      if (is_dir($directory)) {
        set_include_path(get_include_path() . PATH_SEPARATOR . $directory);
        if ($recurse) {
          if ($handle = opendir($directory)){
            while (FALSE !== ($filename = readdir($handle))) {
              if ($filename != '.' && $filename != '..') {
                if (is_dir($directory.'/'.$filename)) {
                  if (!in_array($filename, $exclude)) {
                    addIncludePath($directory.'/'.$filename, $recurse);
                  }
                }
              }
            }
          }
        }
      }
    }
  } // End addIncludePath

	function Register($class, $myOptions = array()) {
		if ( is_array( $class ) ) {
			foreach ( $class as $className => $myOptions ) {
				if ( is_string( $myOptions ) && is_numeric( $className ) ) {
					\Craft\Register( $myOptions );
				} else {
					\Craft\Register( $className, $myOptions );
				}
			}
		} else {
			// Behold the evals. Evals are evil. However, since class_alias is a load of hogwash, this will have to do. If you do not want evals done, then you can manually extend the thing.
    if ( is_string( $class ) ) {
      $extended = explode( '\\', $class );
      $evil = '';
      if ( count( $extended ) > 1 ) {
        $class = array_pop( $extended );
        $evil = 'namespace ' . implode( '\\', $extended ) . ';
';
      } 
      $inflected = \Craft\util\Inflect::pluralize( $class );
      if ($class == $inflected) {
        $class = \Craft\util\Inflect::singularize($inflected);
      }
      $evil .= 'class ' . $class . ' extends \Craft\Object { };
class ' . $inflected . ' extends \Craft\Collection { };';
      eval( $evil );
      
      \Craft\Collection::associate( $inflected, $class ); //Shortcut to not have to singluarize inside the factory.
      if ( is_array( $myOptions ) ) {
        $defaultOptions = array ( 
          'factoryType' => $inflected,
					'jsonOptions' => JSON_FORCE_OBJECT, 
					'format' => 'json', //format can also be a lambda that gets $this as the argument
					//					'idField' => strtolower($class) . 'ID', 
					//					'table' => strtolower( $inflected ) 
        );
        $class::$options = array_merge( $defaultOptions, $myOptions );
				if (isset($myOptions['driver'])) {
					if (is_array($myOptions['driver'])) {
						$driver = $myOptions['driver']['class'];
						$dConfig = $myOptions['driver']['config'];
					} else {
						$driver = $myOptions['driver'];
						$dConfig = array();
					}
					$driver = "\\Craft\\Drivers\\".$driver;
					$class::$driver = new $driver($dConfig);
				}
      } else {
        throw new CraftConfigurationException( 'Unable to set options for class: ' . $class );
      }
    } else {
      throw new CraftConfigurationException( 'Unable to create class: ' . $class );
    }


		}
	} // End Register

  interface Exception {};
  class CraftException extends \Exception implements Exception {};
	class CraftConfigurationException extends CraftException implements Exception {};
  class CraftPluginException extends \RuntimeException implements Exception {};

/**
 * This is used by the \Craft\Object interface when using closures as mixins.
 */
class dummyMixin implements \Craft\Interfaces\Mixin {
	public $name = "";
	public $closure = NULL;
	private $container;
	
	public function __construct($name, $closure) {
		$this->name = $name;
		$this->closure = $closure;
	}

	public function __call($m, $a) {
		return call_user_func_array($this->closure, $a);
	}

	public function setContainer(&$container) {
		$this->container &= $container;
	}

	public function provides($name = NULL) {
		if ($name != NULL) {
			return ($name == $this->name);
		}
		return array($this->name);
	}

}

}
?>