--TEST--
Win\Gui\Windowing->deactivate() method
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
var_dump($window->isActive());
var_dump($window->deactivate());
var_dump($window->isActive());

$window = new Window(['noactivate' => true]);
var_dump($window->isActive());
var_dump($window->activate());
var_dump($window->isActive());
var_dump($window->deactivate());
var_dump($window->isActive()); /* This is a win32 api bug - hurray - should be false but is not */

/* Wrong number args - 1 */
try {
    $window->deactivate(1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
= DONE =
--EXPECT--
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(true)
bool(true)
bool(true)
Win\Gui\Window::deactivate() expects exactly 0 parameters, 1 given
= DONE =