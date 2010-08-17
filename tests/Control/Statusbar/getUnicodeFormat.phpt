--TEST--
Win\Gui\Control\Statusbar->getUnicodeFormat() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->getUnicodeFormat());
$statusbar->setUnicodeFormat(true);
var_dump($statusbar->getUnicodeFormat());

/* Wrong number args - any */
try {
    $statusbar->getUnicodeFormat(1);
    trigger_error('Win\Gui\Control\Statusbar->getUnicodeFormat requires no args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
bool(false)
bool(true)
Win\Gui\Control\Statusbar::getUnicodeFormat() expects exactly 0 parameters, 1 given