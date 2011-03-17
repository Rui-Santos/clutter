<?php
namespace Craft;

abstract class Object {
	public $template = NULL;
	public static $mixins = NULL;
	public static $driver = NULL;
	public static $options = NULL;

	public function getParents($class=null, $plist=array()) {
    $class = $class ? $class : $this;
    $parent = get_parent_class($class);
    if($parent) {
      $plist[] = $parent;
      /*
			 * Do not use $this. Use 'self' here instead, or you will get an
       * infinite loop. Yes, you can call a non-static method
       * statically. Dontcha love php?
			 */
      $plist = self::getParents($parent, $plist);
    }
    return $plist;
  }
  
  public function __toString() {
    if (isset($this->_data) && is_array($this->_data)) {
      //smartObject style vars;
      $vars = $this->_data;
    } else {
      $vars = get_object_vars($this);
    }
    if ( $this->template ) {
      $tpl = $this->template;
      $class = strtolower(get_class($this));
      if (!($tpl instanceof \Craft\Interfaces\iTemplate)) {
        $tpl = new \SimpleTemplate\Template($tpl);
				$this->template = $tpl;
      }
      $tpl->$class = $this;
      $tpl->setVars($vars);
      return $tpl->__toString();
    } else {
      return json_encode($vars, JSON_FORCE_OBJECT);
    }
  }

	public function provides ($implementsP) {
		if (isset($this->driver) && $this->driver->provides($implementsP)) {
			return TRUE;
		} else if (is_array($this->mixins)) {
			foreach ($this->mixins as $mixin) {
				if (
						$mixin->provides($implementsP) || 
						in_array($implementsP, $mixin->provides())
						) {
					return TRUE;
					}
			}
		} else {
			return $this instanceof $implementsP; //objects & interfaces
		}
	}

	public static function mix($mixin, $name = NULL) {
		if (is_null(self::$mixins)) {
			self::$mixins = array();
		}
		if ($mixin instanceof \Craft\Interfaces\Mixin) {
			//do nothing.
		} else if (is_callable($mixin) && $name != NULL) {
			$mixin = new \Craft\dummyMixin($name, $mixin);
		} else {
			$mixin = new $mixin;
		}
		$mixin->setContainer($this);
		self::$mixins[] = $mixin;
	}

	public function __call($m, $a) {
		if ($this->driver instanceof \Craft\Interfaces\Driver) {
			if (is_callable($this->driver->$m)) {
				call_user_func_array(array($this->driver, $m), $a);
			} else {
				foreach (self::$mixins as $mixin) {
					if (is_callable($mixin->$m)) {
						call_user_func_array(array($mixin, $m), $a);
						break;
					}
				}
			}
			return $this;
		}
	}

	public function __get($n) {
		if ($n == "id" && isset($this::$options['idField'])) {
			return $this->{$this::$options['idField']};
		}
		if (is_array($this->_data) && isset($this->_data[$n])) {
			return $this->_data[$n];
		}
		if ($this->driver instanceof \Craft\Interfaces\Driver) {
			return $this->driver->$n;
		}
	}

}



abstract class Collection extends Object implements \ArrayAccess, \Iterator, \Countable {
	private $_pointer = 0;
	public $_data = NULL;
	public static $associations = array();


 public function count ( ) {
    if ( !$this->_data || !$this::$driver->isClean() ) {
      $this->_data = $this::$driver->query();
    }
    return count( $this->_data );
  }
  
  public function offsetExists ( $index ) {
    if ( !$this->_data || !$this::$driver->isClean() ) {
      $this->_data = $this::$driver->query();
    }
    return isset( $this->_data[ $index ] );
  }
  
  public function offsetGet ( $index ) {
    if ( !$this->_data || !$this::$driver->isClean() ) {
      $this->_data = $this::$driver->query();
    }
    if (!($this->_data[ $index ] instanceof \Craft\Object)) {
      $oClass = \Craft\Collection::$associations[ get_class( $this ) ];
      $this->_data[ $index ] = new $oClass($this->_data[ $index ]);
    }
    return $this->_data[ $index ];
  }
  
  public function offsetSet ( $index, $newValue ) {
    if ( !$this->_data || !$this::$driver->isClean() ) {
      $this->_data = $this::$driver->query();
    }
    $this->_data[ $index ] = $newValue;
  }
  
  public function offsetUnset ( $index ) {
    if ( !$this->_data || !$this::$driver->isClean() ) {
      $this->_data = $this::$driver->query();
    }
    unset( $this->_data[ $index ] );
  }
  
  public function key ( ) {
    return $this->_pointer;
  }
  
  public function current ( ) {
    if ( !$this->_data || !$this::$driver->isClean() ) {
      $this->_data = $this::$driver->query();
    }
    if (!($this->_data[ $this->_pointer ] instanceof \Craft\Object)) {
      $soClass = \Craft\Collection::$associations[ get_class( $this ) ];
      $this->_data[ $this->_pointer ] = new $soClass($this->_data[ $this->_pointer ]);
    }
    return $this->_data[ $this->_pointer ];
  }
  
  public function next ( ) {
    if ( !$this->_data || !$this::$driver->isClean() ) {
      $this->_data = $this::$driver->query();
    }
    $this->_pointer++;
  }
  
  public function rewind ( ) {
    $this->_pointer = 0;
  }
  
  public function seek ( $position ) {
    $this->_pointer = $position;
  }
  
  public function valid ( ) {
    if ( !$this->_data || !$this::$driver->isClean() ) {
      $this->_data = $this::$driver->query();
    }
    return isset( $this->_data[ $this->_pointer ] );
  }

  
  public static function associate ( $factory, $class ) {
    self::$associations[ $factory ] = $class;
  }
}

?>