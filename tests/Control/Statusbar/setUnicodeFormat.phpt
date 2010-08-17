--TEST--
Win\Gui\Control\Statusbar->setUnicodeFormat() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->setUnicodeFormat(false));
var_dump($statusbar->setUnicodeFormat(true));
var_dump($statusbar->setUnicodeFormat(true));

/* Wrong number args - none */
try {
    $statusbar->setUnicodeFormat();
    trigger_error('Win\Gui\Control\Statusbar->setUnicodeFormat requires at least one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - too many */
try {
    $statusbar->setUnicodeFormat(1, 1);
    trigger_error('Win\Gui\Control\Statusbar->setUnicodeFormat requires no more than one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 1 */
try {
    $statusbar->setUnicodeFormat(array());
    trigger_error('Win\Gui\Control\Statusbar->setUnicodeFormat requires a boolean argument');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
bool(false)
bool(false)
bool(true)
Win\Gui\Control\Statusbar::setUnicodeFormat() expects exactly 1 parameter, 0 given
Win\Gui\Control\Statusbar::setUnicodeFormat() expects exactly 1 parameter, 2 given
Win\Gui\Control\Statusbar::setUnicodeFormat() expects parameter 1 to be boolean, array given