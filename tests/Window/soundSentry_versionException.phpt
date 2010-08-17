--TEST--
Win\Gui\Window::soundSentry() version exception
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
if(Win\Gui::isVista()) die('skip - Win\Gui\Window::soundSentry available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\VersionException;

// calling this should give a version exception
try {
    Window::soundSentry();
    trigger_error('soundSentry will not run on less than Vista');
} catch (VersionException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
Win\Gui\Window::soundSentry() does not work on Windows versions before Vista