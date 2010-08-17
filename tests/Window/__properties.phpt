--TEST--
Win\Gui\Window properties
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Exception;

$properties = array('type', 'x', 'y', 'width', 'height', 'text', 'parent');

// writing properties
$window = new Window();

// type
$window->type = Window::POPUP;
var_dump($window->type == Window::POPUP);
$window->type = Window::TOPLEVEL;
var_dump($window->type == Window::TOPLEVEL);

// no parent window, go boom
try {
    $window->type = Window::CHILD;
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

// has a parent window, no go boom
$child = new Window(Window::POPUP, "Child", $window);
var_dump($child->type == Window::POPUP);
$child->type = Window::CHILD;
var_dump($child->type == Window::CHILD);

// if you make it a toplevel, then parent is erased
$child = new Window(Window::POPUP, "Child", $window);
var_dump($child->type == Window::POPUP);
var_dump(is_null($child->parent));
$child->type = Window::TOPLEVEL;
var_dump($child->type == Window::TOPLEVEL);
var_dump(is_null($child->parent));

// writing/reading X
var_dump($window->x);
$window->x = 55;
var_dump($window->x);

// writing/reading Y
var_dump($window->y);
$window->y = 55;
var_dump($window->y);

// writing/reading width
var_dump($window->width);
$window->width = 200;
var_dump($window->width);

// writing/reading height
var_dump($window->height);
$window->height = 55;
var_dump($window->height);

// writing non special properties
$window->foo = 'foo'; // create
$window->foo = 'bar'; // write
var_dump($window->foo); // read

// internal properties are isset
foreach($properties as $name) {
    if (!isset($window->$name)) {
        trigger_error('Window property ' . $name . ' should be set');
    }
}

// unset normal property
unset($window->foo);

// cannot unset internal properties
foreach($properties as $name) {
    try {
        unset($window->$name);
        trigger_error('Window property ' . $name . ' should not be unsetable');
    } catch (Exception $e) {}
}

// var_dump the properties
var_dump($window);
die;
?>
--EXPECTF--
string(3) "bar"
NULL
int(%d)
int(%d)
int(%d)
int(%d)
object(Win\Gui\Message)#%d (5) {
  ["window"]=>
  NULL
  ["message"]=>
  int(%d)
  ["time"]=>
  int(%d)
  ["x"]=>
  int(%d)
  ["y"]=>
  int(%d)
}