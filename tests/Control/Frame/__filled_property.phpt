--TEST--
Win\Gui\Control\Frame->filled property
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
var_dump($frame->filled);

// set it to false
$frame = new Frame($window, false);
var_dump($frame->filled);

// set it to true
$frame = new Frame($window, true);
var_dump($frame->filled);

// set it to false with blackframe
$frame = new Frame($window, false, Frame::FRAME_COLOR);
var_dump($frame->filled);

// set it to true with blackrect
$frame = new Frame($window, true, Frame::FRAME_COLOR);
var_dump($frame->filled);

// set it to false with whiteframe
$frame = new Frame($window, false, Frame::WINDOW_COLOR);
var_dump($frame->filled);

// set it to true with whiterect
$frame = new Frame($window, true, Frame::WINDOW_COLOR);
var_dump($frame->filled);

// set it to false with grayframe
$frame = new Frame($window, false, Frame::BACKGROUND_COLOR);
var_dump($frame->filled);

// set it to true with grayrect
$frame = new Frame($window, true, Frame::BACKGROUND_COLOR);
var_dump($frame->filled);

// no matter what, it's always false with etched anything
$frame = new Frame($window, true, Frame::ETCHED);
var_dump($frame->filled);

// no matter what, it's always false with etched anything
$frame = new Frame($window, true, Frame::ETCHED_HORZ);
var_dump($frame->filled);

// no matter what, it's always false with etched anything
$frame = new Frame($window, true, Frame::ETCHED_VERT);
var_dump($frame->filled);
echo PHP_EOL;

// set to false via properties
class BlackFrame extends Frame {
    public $filled = false;
    public $color = Frame::FRAME_COLOR;
}
$frame = new BlackFrame($window);
var_dump($frame->filled);

// set to true via properties
class BlackRect extends Frame {
    public $filled = true;
    public $color = Frame::FRAME_COLOR;
}
$frame = new BlackRect($window);
var_dump($frame->filled);

// set to false via properties
class WhiteFrame extends Frame {
    public $filled = false;
    public $color = Frame::WINDOW_COLOR;
}
$frame = new WhiteFrame($window);
var_dump($frame->filled);

// set to true via properties
class WhiteRect extends Frame {
    public $filled = true;
    public $color = Frame::WINDOW_COLOR;
}
$frame = new WhiteRect($window);
var_dump($frame->filled);

// set to false via properties
class GrayFrame extends Frame {
    public $filled = false;
    public $color = Frame::BACKGROUND_COLOR;
}
$frame = new GrayFrame($window);
var_dump($frame->filled);

// set to true via properties
class GrayRect extends Frame {
    public $filled = true;
    public $color = Frame::BACKGROUND_COLOR;
}
$frame = new GrayRect($window);
var_dump($frame->filled);

// always false for etched
class Etched extends Frame {
    public $filled = true;
    public $color = Frame::ETCHED;
}
$frame = new Etched($window);
var_dump($frame->filled);

// always false for etched
class EtchedHorz extends Frame {
    public $filled = true;
    public $color = Frame::ETCHED_HORZ;
}
$frame = new EtchedHorz($window);
var_dump($frame->filled);

// always false for etched
class EtchedVert extends Frame {
    public $filled = true;
    public $color = Frame::ETCHED_VERT;
}
$frame = new EtchedVert($window);
var_dump($frame->filled);
echo PHP_EOL;

// default is false
$frame = new Frame($window);
var_dump($frame->filled);

// change to true
$frame->filled = true;
var_dump($frame->filled);

// set it to false with blackframe
$frame = new Frame($window, false, Frame::FRAME_COLOR);
var_dump($frame->filled);

$frame->filled = true;
var_dump($frame->filled);

// set it to false with whiteframe
$frame = new Frame($window, false, Frame::WINDOW_COLOR);
var_dump($frame->filled);

$frame->filled = true;
var_dump($frame->filled);

// set it to false with grayframe
$frame = new Frame($window, false, Frame::BACKGROUND_COLOR);
var_dump($frame->filled);

$frame->filled = true;
var_dump($frame->filled);

// always false with etched
$frame = new Frame($window, false, Frame::ETCHED);
var_dump($frame->filled);

$frame->filled = true;
var_dump($frame->filled);
echo PHP_EOL;

// isset check
if (!isset($frame->filled)) {
    trigger_error('Frame property filled should be set');
}

// unset check
try {
    unset($frame->filled);
    trigger_error('Frame property filled should not be unsetable');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

// var_dump the properties
ob_start();
var_dump($frame);
$string = ob_get_clean();
if (strstr($string, '["filled"]') === 0) {
    trigger_error('Frame property filled should be in var_dump');
}
?>
--EXPECTF--
bool(false)
bool(false)
bool(true)
bool(false)
bool(true)
bool(false)
bool(true)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)

bool(false)
bool(true)
bool(false)
bool(true)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)

bool(false)
bool(true)
bool(false)
bool(true)
bool(false)
bool(true)
bool(false)
bool(true)
bool(false)
bool(false)

Internal properties cannot be unset