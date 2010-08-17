--TEST--
Win\Gui\Container::getFromPhysicalPoint() version exception
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
if(Win\Gui::isVista()) die('skip - Win\Gui\Window::getFromPhysicalPoint available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\VersionException;

// calling this should give a version exception
try {
    Window::getFromPhysicalPoint(1, 1);
    trigger_error('getFromPhysicalPoint will not run on less than vista');
} catch (VersionException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
Win\Gui\Window::getFromPhysicalPoint() does not work on Windows versions before Vista