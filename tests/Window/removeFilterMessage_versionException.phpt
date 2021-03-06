--TEST--
Win\Gui\Window::removeFilterMessage() version exception
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
if(Win\Gui::isVista()) die('skip - Win\Gui\Window::removeFilterMessage available');
?>
--FILE--
<?php
use Win\Gui\Container;
use Win\Gui\VersionException;

// calling this should give a version exception
try {
    Window::removeFilterMessage(1);
    trigger_error('removeFilterMessage will not run on less than Vista');
} catch (VersionException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
Win\Gui\Window::removeFilterMessage() does not work on Windows versions before Vista