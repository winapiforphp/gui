--TEST--
Win\Gui\Window::getFromPhysicalPoint() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
if(!Win\Gui::isVista()) die('skip - Win\Gui\Window::getFromPhysicalPoint not available');
?>
--FILE--
<?php
use Win\Gui\Window;

// create a window
$window = new Window();
// show the window
$window->maximize();
// get the container at the 1, 1 point
$window2 = Window::getFromPhysicalPoint(1, 1);
var_dump($window == $window2);
unset($window);
var_dump($window2 instanceof Window);

/* Wrong number args - 1 */
try {
    Window::getFromPhysicalPoint();
    trigger_error('getFromPhysicalPoint requires 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    Window::getFromPhysicalPoint(1);
    trigger_error('getFromPhysicalPoint requires 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    Window::getFromPhysicalPoint(1, 1, 1);
    trigger_error('getFromPhysicalPoint requires exactly 3 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 1 */
try {
    Window::getFromPhysicalPoint(array(), 1);
    trigger_error('getFromPhysicalPoint requires arg 1 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    Window::getFromPhysicalPoint(1, array());
    trigger_error('getFromPhysicalPoint requires arg 2 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
bool(true)
Win\Gui\Window::getFromPhysicalPoint() expects exactly 2 parameters, 0 given
Win\Gui\Window::getFromPhysicalPoint() expects exactly 2 parameters, 1 given
Win\Gui\Window::getFromPhysicalPoint() expects exactly 2 parameters, 3 given
Win\Gui\Window::getFromPhysicalPoint() expects parameter 1 to be long, array given
Win\Gui\Window::getFromPhysicalPoint() expects parameter 2 to be long, array given