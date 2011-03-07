<?php
namespace SimpleTemplate;

/**
 * anApp HTML Template class
 */


class Template extends \Craft\Object implements \Craft\Interfaces\Template {
  protected $_vars = array();
  /**
   * Template file to load
   * @var string $file File to load
   */
  public $_file;

  public function __construct($file = NULL) {
    if ($file) {
      $this->_file = $file;
    }
  }
  
  public function __get($name) {
    if (isset($this->_vars[$name])) {
      return $this->_vars[$name];
    }
  }

  public function __set($name, $value) {
    $this->set($name, $value);
  }
  /**
   * Assign a variable to the template. Is not set within the
   * template until {@link template::parse()} is called.
   * @deprecated Use $template->var = $val instead.
   * @param string $name name to assign the variable within the template.
   * @param mixed $value variable to pass into the template. If this
   * is another template object, it will be parsed immediately.
   */
  public function set($name, $value)
  {
    if ( is_object( $value ) && ( ( $value instanceof \Craft\Interfaces\Template ) || ( $value->template instanceof \Craft\Interfaces\Template ) ) ) {
      //someone passed us a template
      $this->_vars[$name] = $value;
    } else if ( is_object( $value ) ) {
      throw new SimpleTemplateInvalidValueException('Unable to set '.$name.': object is not a template.');
    } else {
      $this->_vars[$name] = $value;
    }
  }
  
  /**
   * Assign an associative array.
   */
  public function setVars(array $vars) {
    foreach ($vars as $key => $val) {
      $this->set($key, $val);
    }
  }
  
  /**
   * Assign a variable to an array within the template. Is not set
   * within the template until {@link template::parse()} is called.
   * @param string $name name of the array to add the variable to.
   * @param mixed $value variable to pass into the template. If this
   * is another template object, it will be parsed immediately.
   */
  public function push($name, $value)
  {
    if (isset($this->vars[$name]) && !is_array($this->vars[$name]))
    {
      # Convert to an array...
      $temp = array();
      $temp[] = $this->vars[$name];
      $this->vars[$name] = $temp;
    }
    if (is_object($value) && get_class($value) == 'template')
    {
      //someone passed us a template
      $this->vars[$name][] = $value->parse();
    }
    else
    {
      $this->vars[$name][] = $value;
    }
  }
  /**
   * Parse the template and return the results as a string.
   * @param string $file [optional] the name of the template
   * file. Yes, you can change template files midstream.
   * @return string HTML
   */
  public function parse($file = NULL)
  {
    if (!$file || $file === NULL) {
       $file = $this->_file;
    }
    if (is_array($this->_vars)){
      extract($this->_vars);
    }
    $contents = ""; // return empty string if things go sideways.
    if ($file != '') {
      ob_start();
      include($file);
      $contents = ob_get_contents();
      ob_end_clean();
    }
    return $contents;
  }
  /**
   * Same as parse but goes ahead and outputs the HTML
   * @param string $file [optional] the name of the template
   * file. Yes, you can change template files midstream.
   */
  public function display($file = null)
  {
    $output = $this->parse($file);
    print $output;
  }

  public function clear($file = null)
  {
    $this->_vars = array();
  }

  public function __toString() {
    return $this->parse();
  }

}

class SimpleTemplateException extends \Craft\CraftPluginException implements \Craft\Exception {};
class SimpleTemplateFileException extends SimpleTemplateException {};
class SimpleTemplateInvalidValueExcecption extends \InvalidArgumentException implements \Craft\Exception {};


?>