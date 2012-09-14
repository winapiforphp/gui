--TEST--
Win\Gui\Windowing->isEnabled() method
--SKIPIF--
<?php
include __DIR__ . '/../../skipif.inc';
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\System\InvalidArgumentException;

$window = new Window([]);
var_dump($window->isEnabled());

$window = new Window(['disable' => true]);
var_dump($window->isEnabled());

var_dump($window->enable());
var_dump($window->isEnabled());
var_dump($window->disable());
var_dump($window->isEnabled());

/* Wrong number args - 1 */
try {
    $window->isEnabled(1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
= DONE =
--EXPECT--
bool(true)
bool(false)
bool(true)
bool(true)
bool(false)
bool(false)
Win\Gui\Window::isEnabled() expects exactly 0 parameters, 1 given
= DONE =