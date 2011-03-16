<?php
\Craft\enablePlugin('rdiodriver'); //oauth driver will be changed to rdio driver
/**
 * This will be removed and added to the rdio driver.
 */
\Config\Routes::add(1000, '/oauth\/callback/', function($matches) {
		include_once('oauthtest.inc');
		return true;
	}, 'Oauth Callback');

\Craft\Register('oauthtest', array(
																	 'driver' => array(
																										 'class' => 'rdio',
																										 'config' => '\Config\Rdio'
																										 )
																	 ));


?>