--TEST--
Win\Gui\Windowing->isActive() method
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
$window->deactivate();
var_dump($window->isActive());

$window = new Window(['noactivate' => true]);
$window->show(false, true);
var_dump($window->isActive());
$window->activate();
var_dump($window->isActive());

/* Wrong number args - 1 */
try {
    $window->isActive(1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
= DONE =
--EXPECT--
bool(true)
bool(false)
bool(false)
bool(true)
Win\Gui\Window::isActive() expects exactly 0 parameters, 1 given
= DONE =