--TEST--
Win\Gui\Control\Statusbar->getIcon() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;
use Win\Gui\Resource\Icon;

$statusbar = new Statusbar(new Window);

$icon = Icon::load(Icon::OIC_SHIELD);
$icon2 = Icon::load(Icon::OIC_SHIELD);
$icon3 = Icon::load(Icon::OIC_SHIELD);

// set the icon
var_dump($statusbar->getIcon());
$statusbar->setIcon($icon);
var_dump($statusbar->getIcon());

// set the icon to empty
$statusbar->setIcon(null);
var_dump($statusbar->getIcon());

// set parts
$statusbar->setParts(array(15, 23));
$statusbar->setIcon($icon3, 1);
var_dump($statusbar->getIcon(1));

// simple
$statusbar->setSimple(true);
$statusbar->setIcon($icon);
var_dump($statusbar->getIcon());

/* Wrong number args - too many */
try {
    $statusbar->getIcon(1, 1);
    trigger_error('Win\Gui\Control\Statusbar->getIcon requires at most 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 1 */
try {
    $statusbar->getIcon(array());
    trigger_error('Win\Gui\Control\Statusbar->getIcon requires arg 1 to be integer');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
NULL
object(Win\Gui\Resource\Icon)#%d (0) {
}
NULL
object(Win\Gui\Resource\Icon)#%d (0) {
}
object(Win\Gui\Resource\Icon)#%d (0) {
}
Win\Gui\Control\Statusbar::getIcon() expects at most 1 parameter, 2 given
Win\Gui\Control\Statusbar::getIcon() expects parameter 1 to be long, array given