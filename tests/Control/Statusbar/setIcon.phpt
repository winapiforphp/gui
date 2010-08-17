--TEST--
Win\Gui\Control\Statusbar->setIcon() method
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
var_dump($statusbar->setIcon($icon));
var_dump($statusbar->getIcon() === $icon);

// change the icon
var_dump($statusbar->setIcon($icon2));
var_dump($statusbar->getIcon() === $icon);
var_dump($statusbar->getIcon() === $icon2);

// set the icon to empty
var_dump($statusbar->setIcon(null));
var_dump($statusbar->getIcon() === null);

// set parts
$statusbar->setParts(array(15, 23));
var_dump($statusbar->setIcon($icon3, 1));
var_dump($statusbar->getIcon(1) === $icon3);

// simple
$statusbar->setSimple(true);
var_dump($statusbar->setIcon($icon));
var_dump($statusbar->getIcon() === $icon);

/* Wrong number args - none */
try {
    $statusbar->setIcon();
    trigger_error('Win\Gui\Control\Statusbar->setIcon requires at least 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - too many */
try {
    $statusbar->setIcon(null, 1, 1);
    trigger_error('Win\Gui\Control\Statusbar->setIcon requires at most 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 1 */
try {
    $statusbar->setIcon(1, 1);
    trigger_error('Win\Gui\Control\Statusbar->setIcon requires arg 1 to be instance of Win\Gui\Resource\Icon');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 2 */
try {
    $statusbar->setIcon($icon, array());
    trigger_error('Win\Gui\Control\Statusbar->setIcon requires arg 2 to be integer');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(false)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Win\Gui\Control\Statusbar::setIcon() expects at least 1 parameter, 0 given
Win\Gui\Control\Statusbar::setIcon() expects at most 2 parameters, 3 given
Win\Gui\Control\Statusbar::setIcon() expects parameter 1 to be Win\Gui\Resource\Icon, integer given
Win\Gui\Control\Statusbar::setIcon() expects parameter 2 to be long, array given