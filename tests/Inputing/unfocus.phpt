--TEST--
Win\Gui\Windowing->unfocus() method
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
var_dump($window->unfocus());
var_dump($window->hasFocus());

/* Wrong number args - 1 */
try {
    $window->unfocus(1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
= DONE =
--EXPECT--
bool(true)
bool(true)
bool(false)
Win\Gui\Window::unfocus() expects exactly 0 parameters, 1 given
= DONE =