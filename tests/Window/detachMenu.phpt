--TEST--
Win\Gui\Window->detachMenu() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Menu;

// create a window
$window = new Window();

// create a new menu
$menu = new Menu();

var_dump($window->detachMenu());
$window->attachMenu($menu);
var_dump($window->detachMenu());

var_dump($window->menu === NULL);

/* Wrong number args - 1 */
try {
    $window->attachMenu();
    trigger_error('attachMenu requires 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    $window->attachMenu($menu, 1);
    trigger_error('attachMenu requires 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $window->attachMenu(1);
    trigger_error('attachMenu requires arg 1 to be instanceof Win\Gui\Menu');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
Win\Gui\Window::attachMenu() expects exactly 1 parameter, 0 given
Win\Gui\Window::attachMenu() expects exactly 1 parameter, 2 given
Argument 1 passed to Win\Gui\Window::attachMenu() must be an instance of Win\Gui\Menu, integer given