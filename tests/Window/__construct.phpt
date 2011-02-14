--TEST--
Win\Gui\Window->__construct() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\ArgumentException;

use Win\System\Unicode;
use Win\System\CodePage;

// window with all defaults
$window = new Window;
var_dump($window->type == Window::TOPLEVEL);

// Window with the three different types
$toplevel = new Window(array('type' => Window::TOPLEVEL));
var_dump($toplevel->type == Window::TOPLEVEL);

$popup = new Window(array('type' => Window::POPUP));
var_dump($toplevel->type == Window::TOPLEVEL);

$child = new Window(array('type' => Window::CHILD,
                          'text' => "A window",
                          'parent' => $toplevel));
var_dump($toplevel->type == Window::TOPLEVEL);

// text in a window
$window = new Window(array('type' => Window::TOPLEVEL,
                           'text' => "My text"));
var_dump($window->text == "My text");

?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)