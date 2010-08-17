--TEST--
Win\Gui\Window::soundSentry() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
if(!Win\Gui::isVista()) die('skip - Win\Gui\Window::soundSentry not available');
?>
--FILE--
<?php
use Win\Gui\Window;

// add a message
var_dump(Window::soundSentry());

/* Wrong number args - 1 */
try {
    Window::soundSentry(1);
    trigger_error('soundSentry requires 0 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(false)
Win\Gui\Window::soundSentry() expects exactly 0 parameters, 1 given