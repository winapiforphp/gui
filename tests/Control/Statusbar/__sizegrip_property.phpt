--TEST--
Win\Gui\Control\Statusbar->sizegrip property
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

// default is false
$statusbar = new Statusbar(new Window);
var_dump($statusbar->sizegrip);

// set it to something in the constructor
$statusbar = new Statusbar(new Window, array('sizegrip' => true));
var_dump($statusbar->sizegrip);

$statusbar->sizegrip = false;
var_dump($statusbar->sizegrip);

// set to value via properties
class test extends Statusbar {
    public $sizegrip = true;
}
$statusbar = new test(new Window);
var_dump($statusbar->sizegrip);

// isset check
if (!isset($statusbar->sizegrip)) {
    trigger_error('Statusbar property sizegrip should be set');
}

// unset check
try {
    unset($statusbar->sizegrip);
    trigger_error('Statusbar property sizegrip should not be unsetable');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

// var_dump the properties
ob_start();
var_dump($frame);
$string = ob_get_clean();
if (strstr($string, '["sizegrip"]') === 0) {
    trigger_error('Statusbar property sizegrip should be in var_dump');
}
?>
--EXPECT--
bool(false)
bool(true)
bool(false)
bool(true)
Internal properties cannot be unset