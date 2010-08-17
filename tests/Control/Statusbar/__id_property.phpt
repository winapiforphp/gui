--TEST--
Win\Gui\Control\Statusbar->id property
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
var_dump($statusbar->id);

// set it to something in the constructor
$statusbar = new Statusbar(new Window, array('id' => 8));
var_dump($statusbar->id);

$statusbar->id = 5;
var_dump($statusbar->id);

// set to value via properties
class test extends Statusbar {
    public $id = 6;
}
$statusbar = new test(new Window);
var_dump($statusbar->id);

// isset check
if (!isset($statusbar->id)) {
    trigger_error('Statusbar property id should be set');
}

// unset check
try {
    unset($statusbar->id);
    trigger_error('Statusbar property id should not be unsetable');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

// var_dump the properties
ob_start();
var_dump($frame);
$string = ob_get_clean();
if (strstr($string, '["id"]') === 0) {
    trigger_error('Statusbar property id should be in var_dump');
}
?>
--EXPECT--
int(0)
int(8)
int(5)
int(6)
Internal properties cannot be unset