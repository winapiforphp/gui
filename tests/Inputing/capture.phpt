--TEST--
Win\Gui\Windowing->capture() method
--SKIPIF--
<?php
include __DIR__ . '/../../skipif.inc';
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\System\InvalidArgumentException;

$window = new Window([]);
var_dump($window->hasCapture());
var_dump($window->capture());
var_dump($window->hasCapture());

/* Wrong number args - 1 */
try {
    $window->capture(1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
= DONE =
--EXPECT--
bool(false)
bool(false)
bool(true)
Win\Gui\Window::capture() expects exactly 0 parameters, 1 given
= DONE =