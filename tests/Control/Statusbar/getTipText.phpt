--TEST--
Win\Gui\Control\Statusbar->getTipText() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

$statusbar->setTipText("my string");
var_dump($statusbar->getTipText());

var_dump($statusbar->setParts(array(1, 2)));

$statusbar->setTipText("my new string", 0);
var_dump($statusbar->getTipText(0));

$statusbar->setTipText("my first string", 1);
var_dump($statusbar->getTipText(1));

$statusbar->setSimple(true);
$statusbar->setTipText("my string");
var_dump($statusbar->getTipText());

/* Wrong number args - too many */
try {
    $statusbar->getTipText(1, 1);
    trigger_error('Win\Gui\Control\Statusbar->getTipText requires at most one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $statusbar->getTipText(array());
    trigger_error('Win\Gui\Control\Statusbar->getTipText requires an int argument for arg 1');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
string(9) "my string"
bool(true)
string(13) "my new string"
string(15) "my first string"
string(9) "my string"
Win\Gui\Control\Statusbar::getTipText() expects at most 1 parameter, 2 given
Win\Gui\Control\Statusbar::getTipText() expects parameter 1 to be long, array given