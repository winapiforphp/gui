--TEST--
Win\Gui\Windowing->activate() method
--SKIPIF--
<?php
include __DIR__ . '/../../skipif.inc';
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\System\InvalidArgumentException;

$window = new Window([]);
var_dump($window->isActive());
var_dump($window->activate());
var_dump($window->isActive());

$window = new Window(['noactivate' => true]);
var_dump($window->isActive());
var_dump($window->activate());
var_dump($window->isActive());

/* Wrong number args - 1 */
try {
    $window->activate(1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
= DONE =
--EXPECT--
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(true)
Win\Gui\Window::activate() expects exactly 0 parameters, 1 given
= DONE =