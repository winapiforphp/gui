--TEST--
Win\Gui\Window::popupPosition() version exception
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
if(Win\Gui::isWin7()) die('skip - Win\Gui\Window::popupPosition available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\VersionException;

// calling this should give a version exception
try {
    Window::popupPosition(1, 1, 1, 1);
    trigger_error('popupPosition will not run on less than windows 7');
} catch (VersionException $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
Win\Gui\Window::popupPosition() does not work on Windows versions before Windows 7