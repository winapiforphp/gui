--TEST--
Win\Gui\Window\Animate Basics
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
// we have the class
var_dump(class_exists('Win\Gui\Window\Animate'));
// no parents
$parents = class_parents('Win\Gui\Window\Animate');
var_dump(empty($parents));
// implements nothing
$implements = class_implements('Win\Gui\Window\Animate');
var_dump(empty($implements));

// has no methods
// has no properties

// has the following constants
$constants =  array('SLIDE',
                    'BLEND',
                    'BOX',
                    'HOR_POSITIVE',
                    'HOR_NEGATIVE',
                    'VER_POSITIVE',
                    'VER_NEGATIVE');
$error = false;
foreach($constants as $name) {
    if (!defined('Win\Gui\Window\Animate::' . $name)) {
        $error = true;
        echo 'Missing Constant: Win\Gui\Window\Animate::' . $name . PHP_EOL;
    }
}
if (!$error) {
    echo "No missing constants, checked " . count($constants) . '!' . PHP_EOL;
}

// cannot instantiate
new Win\Gui\Window\Animate;
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
No missing constants, checked 7!

Fatal error: Cannot instantiate abstract class Win\Gui\Window\Animate in %s on line %d