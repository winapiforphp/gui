--TEST--
Win\Gui\Window::addFilterMessage() version exception
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
if(Win\Gui::isVista()) die('skip - Win\Gui\Window::addFilterMessage available');
?>
--FILE--
<?php
use Win\Gui\Container;
use Win\Gui\VersionException;

// calling this should give a version exception
try {
    Window::addFilterMessage(1);
    trigger_error('addFilterMessage will not run on less than Vista');
} catch (VersionException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
Win\Gui\Window::addFilterMessage() does not work on Windows versions before Vista