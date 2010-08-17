--TEST--
Win\Gui\Control\Statusbar->getText() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->getText());
$statusbar->setText("my string");
var_dump($statusbar->getText());

$statusbar->setParts(array(15, 23));
$statusbar->setText("second string", 1);
var_dump($statusbar->getText(1));

$statusbar->setSimple(true);
$statusbar->setText("simple text");
var_dump($statusbar->getText());

/* Wrong number args - too many */
try {
    $statusbar->getText(1, 1);
    trigger_error('Win\Gui\Control\Statusbar->getText requires at most 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $statusbar->getText(array());
    trigger_error('Win\Gui\Control\Statusbar->getText requires an integer argument for arg 1');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
string(0) ""
string(9) "my string"
string(13) "second string"
string(11) "simple text"
Win\Gui\Control\Statusbar::getText() expects at most 1 parameter, 2 given
Win\Gui\Control\Statusbar::getText() expects parameter 1 to be long, array given