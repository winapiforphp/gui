--TEST--
Win\Gui\Control\Frame->__construct() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Frame;

$window = new Window;

$frame = new Frame($window);
$frame = new Frame($window, true);
var_dump($frame->filled);
$frame = new Frame($window, true, Frame::ETCHED);
var_dump($frame->color);
$frame = new Frame($window, true, Frame::ETCHED, array('x' => 25));
var_dump($frame->x);

/* Wrong number args - 1 */
try {
    $frame = new Frame();
    trigger_error('Frame::__construct requires at least 1 argument');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    $frame = new Frame($window, true, Frame::ETCHED, array('x' => 25), 1);
    trigger_error('Frame::__construct requires no more than 4 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    $frame = new Frame(1);
    trigger_error('Frame::__construct requires arg 1 to be instanceof Win\Gui\Window');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    $frame = new Frame($window, array());
    trigger_error('Frame::__construct requires arg 2 to be boolean');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    $frame = new Frame($window, true, array());
    trigger_error('Frame::__construct requires arg 3 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    $frame = new Frame($window, true, 1, 1);
    trigger_error('Frame::__construct requires arg 4 to be array');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
int(4)
int(25)
Win\Gui\Control\Frame::__construct() expects at least 1 parameter, 0 given
Win\Gui\Control\Frame::__construct() expects at most 4 parameters, 5 given
Win\Gui\Control\Frame::__construct() expects parameter 1 to be Win\Gui\Window, integer given
Win\Gui\Control\Frame::__construct() expects parameter 2 to be boolean, array given
Win\Gui\Control\Frame::__construct() expects parameter 3 to be long, array given
Win\Gui\Control\Frame::__construct() expects parameter 4 to be array, integer given