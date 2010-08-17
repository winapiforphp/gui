--TEST--
Win\Gui\Control\Statusbar->tooltips property
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
var_dump($statusbar->tooltips);

// set it to something in the constructor
$statusbar = new Statusbar(new Window, array('tooltips' => true));
var_dump($statusbar->tooltips);

$statusbar->tooltips = false;
var_dump($statusbar->tooltips);

// set to value via properties
class test extends Statusbar {
    public $tooltips = true;
}
$statusbar = new test(new Window);
var_dump($statusbar->tooltips);

// isset check
if (!isset($statusbar->tooltips)) {
    trigger_error('Statusbar property tooltips should be set');
}

// unset check
try {
    unset($statusbar->tooltips);
    trigger_error('Statusbar property tooltips should not be unsetable');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

// var_dump the properties
ob_start();
var_dump($frame);
$string = ob_get_clean();
if (strstr($string, '["tooltips"]') === 0) {
    trigger_error('Statusbar property tooltips should be in var_dump');
}
?>
--EXPECT--
bool(false)
bool(true)
bool(false)
bool(true)
Internal properties cannot be unset