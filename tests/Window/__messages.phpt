--TEST--
Win\Gui\Window Messages
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;

// test connecting to the message each with and without user args
// test no args
// test with return value
// test with args
// test with args and return value

// test magic method handler - done
// test bubbling (turn off by returning true)

// test sending the msg (if applicable - some are not sendable)
// test posting the msg (if applicable - some are not postable)

class MyWindow extends Window {
    // WM_SIZE
    public function __onWmSize(&$return_value, $type, $width, $height) {
        echo 'This is the default class callback with a return of ' . $return_value . ' and a type of ' . $type . ' and a width of ' . $width . ' and a height of ' . $height . PHP_EOL;
    }
}

function simpletest() {
    echo 'This is a simple callback that gets ' . func_num_args() . ' passed in' . PHP_EOL;
}

function regulartest($type, $width, $height) {
    echo 'This is a regular connect with a type of ' . $type . ' and a width of ' . $width . ' and a height of ' . $height . PHP_EOL;
}

function fulltest(&$return_value, $type, $width, $height) {
    echo 'This is a full connect with a return of ' . $return_value . ' and a type of ' . $type . ' and a width of ' . $width . ' and a height of ' . $height . PHP_EOL;
    $return_value = 8;
}
$window = new MyWindow();
$simpleid = $window->connectSimple(Window::WM_SIZE, 'simpletest');
var_dump($simpleid);
$regularid = $window->connect(Window::WM_SIZE, 'regulartest');
var_dump($regularid);
$fullid = $window->connectFull(Window::WM_SIZE, 'fulltest');
var_dump($fullid);
$window->show();
?>
--EXPECTF--