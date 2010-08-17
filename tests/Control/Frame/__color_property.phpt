--TEST--
Win\Gui\Control\Frame->color property
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
var_dump($frame->color);

// set it to blackframe
$frame = new Frame($window, false, Frame::FRAME_COLOR);
var_dump($frame->color);

// set it to blackrect
$frame = new Frame($window, true, Frame::FRAME_COLOR);
var_dump($frame->color);

// set it to  whiteframe
$frame = new Frame($window, false, Frame::WINDOW_COLOR);
var_dump($frame->color);

// set it to whiterect
$frame = new Frame($window, true, Frame::WINDOW_COLOR);
var_dump($frame->color);

// set it to grayframe
$frame = new Frame($window, false, Frame::BACKGROUND_COLOR);
var_dump($frame->color);

// set it to grayrect
$frame = new Frame($window, true, Frame::BACKGROUND_COLOR);
var_dump($frame->color);

// set it to etched
$frame = new Frame($window, true, Frame::ETCHED);
var_dump($frame->color);

// set it to etched
$frame = new Frame($window, true, Frame::ETCHED_HORZ);
var_dump($frame->color);

// set it to etched
$frame = new Frame($window, true, Frame::ETCHED_VERT);
var_dump($frame->color);
echo PHP_EOL;

// set to false via properties
class BlackFrame extends Frame {
    public $filled = false;
    public $color = Frame::FRAME_COLOR;
}
$frame = new BlackFrame($window);
var_dump($frame->color);

// set to true via properties
class BlackRect extends Frame {
    public $filled = true;
    public $color = Frame::FRAME_COLOR;
}
$frame = new BlackRect($window);
var_dump($frame->color);

// set to false via properties
class WhiteFrame extends Frame {
    public $filled = false;
    public $color = Frame::WINDOW_COLOR;
}
$frame = new WhiteFrame($window);
var_dump($frame->color);

// set to true via properties
class WhiteRect extends Frame {
    public $filled = true;
    public $color = Frame::WINDOW_COLOR;
}
$frame = new WhiteRect($window);
var_dump($frame->color);

// set to false via properties
class GrayFrame extends Frame {
    public $filled = false;
    public $color = Frame::BACKGROUND_COLOR;
}
$frame = new GrayFrame($window);
var_dump($frame->color);

// set to true via properties
class GrayRect extends Frame {
    public $filled = true;
    public $color = Frame::BACKGROUND_COLOR;
}
$frame = new GrayRect($window);
var_dump($frame->color);

// set to true via properties
class Etched extends Frame {
    public $color = Frame::ETCHED;
}
$frame = new Etched($window);
var_dump($frame->color);

// set to true via properties
class EtchedHorz extends Frame {
    public $color = Frame::ETCHED_HORZ;
}
$frame = new EtchedHorz($window);
var_dump($frame->color);

// set to true via properties
class EtchedVert extends Frame {
    public $color = Frame::ETCHED_VERT;
}
$frame = new EtchedVert($window);
var_dump($frame->color);
echo PHP_EOL;

// default is black
$frame = new Frame($window);
var_dump($frame->color);

// change to white
$frame->color = Frame::WINDOW_COLOR;
var_dump($frame->color);

// change to gray
$frame->color = Frame::BACKGROUND_COLOR;
var_dump($frame->color);

// change to filled
$frame->filled = true;

// change to black
$frame->color = Frame::FRAME_COLOR;
var_dump($frame->color);

// change to white
$frame->color = Frame::WINDOW_COLOR;
var_dump($frame->color);

// change to gray
$frame->color = Frame::BACKGROUND_COLOR;
var_dump($frame->color);

// change to etched
$frame->color = Frame::ETCHED;
var_dump($frame->color);

// change to etchedhorz
$frame->color = Frame::ETCHED_HORZ;
var_dump($frame->color);

// change to etched
$frame->color = Frame::ETCHED_VERT;
var_dump($frame->color);
echo PHP_EOL;

// isset check
if (!isset($frame->color)) {
    trigger_error('Frame property color should be set');
}

// unset check
try {
    unset($frame->filled);
    trigger_error('Frame property color should not be unsetable');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

// var_dump the properties
ob_start();
var_dump($frame);
$string = ob_get_clean();
if (strstr($string, '["color"]') === 0) {
    trigger_error('Frame property color should be in var_dump');
}
?>
--EXPECTF--
int(1)
int(1)
int(1)
int(2)
int(2)
int(3)
int(3)
int(4)
int(5)
int(6)

int(1)
int(1)
int(2)
int(2)
int(3)
int(3)
int(4)
int(5)
int(6)

int(1)
int(2)
int(3)
int(1)
int(2)
int(3)
int(4)
int(5)
int(6)

Internal properties cannot be unset