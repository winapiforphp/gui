--TEST--
Win\Gui\Windowing->hasFocus() method
--SKIPIF--
<?php
include __DIR__ . '/../../skipif.inc';
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\System\InvalidArgumentException;

$window = new Window([]);
$window->show();
var_dump($window->hasFocus());
$window->unfocus();
var_dump($window->hasFocus());
$window->focus();
var_dump($window->hasFocus());

/* Wrong number args - 1 */
try {
    $window->hasFocus(1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
= DONE =
--EXPECT--
bool(true)
bool(false)
bool(true)
Win\Gui\Window::hasFocus() expects exactly 0 parameters, 1 given
= DONE =