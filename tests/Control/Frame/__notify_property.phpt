--TEST--
Win\Gui\Control\Frame->notify property
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
var_dump($frame->notify);

// option to true
$frame = new Frame($window, false, Frame::FRAME_COLOR, array('notify' => true));
var_dump($frame->notify);
echo PHP_EOL;

// set to true via properties
class Notify extends Frame {
    public $notify = true;
}
$frame = new Notify($window);
var_dump($frame->notify);
echo PHP_EOL;

// default is false
$frame = new Frame($window);
var_dump($frame->notify);

// change to true
$frame->notify = true;
var_dump($frame->notify);
echo PHP_EOL;

// isset check
if (!isset($frame->notify)) {
    trigger_error('Frame property notify should be set');
}

// unset check
try {
    unset($frame->notify);
    trigger_error('Frame property notify should not be unsetable');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

// var_dump the properties
ob_start();
var_dump($frame);
$string = ob_get_clean();
if (strstr($string, '["notify"]') === 0) {
    trigger_error('Frame property notify should be in var_dump');
}
?>
--EXPECT--
bool(false)
bool(true)

bool(true)

bool(false)
bool(true)

Internal properties cannot be unset