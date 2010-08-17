--TEST--
Win\Gui\Control\Frame->sunken property
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Frame;

$window = new Window;

// default is false
$frame = new Frame($window);
var_dump($frame->sunken);

// option to true
$frame = new Frame($window, false, Frame::FRAME_COLOR, array('sunken' => true));
var_dump($frame->sunken);
echo PHP_EOL;

// set to true via properties
class Sunken extends Frame {
    public $sunken = true;
}
$frame = new Sunken($window);
var_dump($frame->sunken);
echo PHP_EOL;

// default is false
$frame = new Frame($window);
var_dump($frame->sunken);

// change to true
$frame->sunken = true;
var_dump($frame->sunken);
echo PHP_EOL;

// isset check
if (!isset($frame->sunken)) {
    trigger_error('Frame property sunken should be set');
}

// unset check
try {
    unset($frame->sunken);
    trigger_error('Frame property notify sunken not be unsetable');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

// var_dump the properties
ob_start();
var_dump($frame);
$string = ob_get_clean();
if (strstr($string, '["sunken"]') === 0) {
    trigger_error('Frame property sunken should be in var_dump');
}
?>
--EXPECT--
bool(false)
bool(true)

bool(true)

bool(false)
bool(true)

Internal properties cannot be unset