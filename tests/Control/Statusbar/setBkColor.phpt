--TEST--
Win\Gui\Control\Statusbar->setBkColor() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->setBkColor());

var_dump($statusbar->setBkColor(255));
var_dump($statusbar->setBkColor(255, 255));
var_dump($statusbar->setBkColor(255, 255, 255));
var_dump($statusbar->setBkColor('CCC'));
var_dump($statusbar->setBkColor('#FFF'));
var_dump($statusbar->setBkColor('FFFFFF'));
var_dump($statusbar->setBkColor('#CC0000'));
var_dump($statusbar->setBkColor(469, 569, 815));
var_dump($statusbar->setBkColor(-469, -8, -34));

/* Wrong number args - too many */
try {
    $statusbar->setBkColor(1, 1, 1, 1);
    trigger_error('Win\Gui\Control\Statusbar->setBkColor requires at most 3 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $statusbar->setBkColor(array());
    trigger_error('Win\Gui\Control\Statusbar->setBkColor an int or properly formatted string argument');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $statusbar->setBkColor('foobar baz');
    trigger_error('Win\Gui\Control\Statusbar->setBkColor requires string to be hex parseable');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 2 */
try {
    $statusbar->setBkColor(1, array());
    trigger_error('Win\Gui\Control\Statusbar->setBkColor an int for arg 2');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 3 */
try {
    $statusbar->setBkColor(1, 1, array());
    trigger_error('Win\Gui\Control\Statusbar->setBkColor an int for arg 3');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
NULL
NULL
int(255)
int(65535)
int(16777215)
int(16777215)
int(16777215)
int(16777215)
int(204)
int(16777215)
Win\Gui\Control\Statusbar::setBkColor() expects at most 3 parameters, 4 given
Win\Gui\Control\Statusbar::setBkColor() expects parameter 1 to be long or parseable hex string
Win\Gui\Control\Statusbar::setBkColor() expects parameter 1 string in RGB or RRGGBB format, with our without a # prefix
Win\Gui\Control\Statusbar::setBkColor() expects parameter 2 to be long, array given
Win\Gui\Control\Statusbar::setBkColor() expects parameter 3 to be long, array given