--TEST--
Win\Gui\Control\Statusbar::drawText() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

$statusbar->drawText($statusbar, array(), "my text");
$statusbar::drawText($statusbar, array(), "my text");

Statusbar::drawText($statusbar, array('top' => 50, 'bottom' => 50), "my text");

var_dump(Statusbar::drawText($statusbar, array(), "my text"));

/* Wrong number args - none */
try {
    Statusbar::drawText();
    trigger_error('Win\Gui\Control\Statusbar::drawText requires at least 3 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 1 */
try {
    Statusbar::drawText($statusbar);
    trigger_error('Win\Gui\Control\Statusbar::drawText requires at least 3 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    Statusbar::drawText($statusbar, array());
    trigger_error('Win\Gui\Control\Statusbar::drawText requires at least 3 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - too many */
try {
    Statusbar::drawText($statusbar, array(), "", 1, 1);
    trigger_error('Win\Gui\Control\Statusbar::drawText requires at most 4 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    Statusbar::drawText(array(), array(), "", 1);
    trigger_error('Win\Gui\Control\Statusbar::drawText requires arg 1 to implement Win\Gui\Windowing');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 2 */
try {
    Statusbar::drawText($statusbar, 1, "", 1);
    trigger_error('Win\Gui\Control\Statusbar::drawText requires arg 2 to be an array');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 3 */
try {
    Statusbar::drawText($statusbar, array(), array(), 1);
    trigger_error('Win\Gui\Control\Statusbar::drawText requires arg 3 to be a string');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 4 */
try {
    Statusbar::drawText($statusbar, array(), "", array());
    trigger_error('Win\Gui\Control\Statusbar::drawText requires arg 4 to be an integer');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
NULL
Win\Gui\Control\Statusbar::drawText() expects at least 3 parameters, 0 given
Win\Gui\Control\Statusbar::drawText() expects at least 3 parameters, 1 given
Win\Gui\Control\Statusbar::drawText() expects at least 3 parameters, 2 given
Win\Gui\Control\Statusbar::drawText() expects at most 4 parameters, 5 given
Win\Gui\Control\Statusbar::drawText() expects parameter 1 to be Win\Gui\Windowing, array given
Win\Gui\Control\Statusbar::drawText() expects parameter 2 to be array, integer given
Win\Gui\Control\Statusbar::drawText() expects parameter 3 to be string, array given
Win\Gui\Control\Statusbar::drawText() expects parameter 4 to be long, array given