--TEST--
Win\Gui\Windowing->focus() method
--SKIPIF--
<?php
include __DIR__ . '/../../skipif.inc';
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\System\InvalidArgumentException;

$window = new Window([]);
var_dump($window->hasFocus());
var_dump($window->focus());
var_dump($window->hasFocus());

/* Wrong number args - 1 */
try {
    $window->focus(1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
= DONE =
--EXPECT--
bool(false)
bool(true)
bool(true)
Win\Gui\Window::focus() expects exactly 0 parameters, 1 given
= DONE =