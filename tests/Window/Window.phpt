--TEST--
Win\Gui\Window Basics
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
var_dump(class_exists('Win\Gui\Window'));

$window = new Win\Gui\Window();
// verify it extends input
var_dump($window instanceof Win\Gui\Input);
//verify it implements nothing
$implements = class_implements('Win\Gui\Window');
var_dump(empty($implements));

// has the following methods
$methods = array('__construct');
$error = false;
foreach($methods as $name) {
    if (!method_exists('Win\Gui\Window', $name)) {
        $error = true;
        echo 'Missing Method: Win\Gui\Window::' . $name . '()' . PHP_EOL;
    }
}
if (!$error) {
    echo "No missing methods, checked " . count($methods) . '!' . PHP_EOL;
}

// has the following properties
$properties = array('type',
                    'x',
                    'y',
                    'width',
                    'height',
                    'text',
                    'parent');
$error = false;
foreach($properties as $name) {
    if (!property_exists('Win\Gui\Window', $name)) {
        $error = true;
        echo 'Missing Property: Win\Gui\Window->' . $name . PHP_EOL;
    }
}
if (!$error) {
    echo "No missing properties, checked " . count($properties) . '!' . PHP_EOL;
}

// has the following constants
$constants = array('TOPLEVEL', 'POPUP', 'CHILD');
$error = false;
foreach($constants as $name) {
    if (!defined('Win\Gui\Window::' . $name)) {
        $error = true;
        echo 'Missing Constant: Win\Gui\Window::' . $name . PHP_EOL;
    }
}
if (!$error) {
    echo "No missing constants, checked " . count($constants) . '!' . PHP_EOL;
}
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)