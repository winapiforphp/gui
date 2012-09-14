--TEST--
Win\Gui\Windowing->release() method
--SKIPIF--
<?php
include __DIR__ . '/../../skipif.inc';
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\System\InvalidArgumentException;

$window = new Window([]);
$window->capture();
var_dump($window->hasCapture());
var_dump($window->release());
var_dump($window->hasCapture());

/* Wrong number args - 1 */
try {
    $window->release(1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
= DONE =
--EXPECT--
bool(true)
bool(true)
bool(false)
Win\Gui\Window::release() expects exactly 0 parameters, 1 given
= DONE =