--TEST--
Win\Gui\MenuItem Basics
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
var_dump(class_exists('Win\Gui\MenuItem'));

// no parents
$parents = class_parents('Win\Gui\MenuItem');
var_dump(empty($parents));

//verify it implements nothing
$implements = class_implements('Win\Gui\MenuItem');
var_dump(empty($implements));

// has the following methods
$expected_methods = array('__construct',
                          'getCheckmarkSize');
$methods = get_class_methods('Win\Gui\MenuItem');

$error = false;
foreach($expected_methods as $name) {
    if (!method_exists('Win\Gui\MenuItem', $name) || !in_array($name, $expected_methods)) {
        $error = true;
        echo 'Missing Method: Win\Gui\MenuItem::' . $name . '()' . PHP_EOL;
    } else {
        unset($methods[array_search($name, $expected_methods)]);
    }
}
if (!$error) {
    echo 'No missing methods, checked ' . count($expected_methods) . '!' . PHP_EOL;
}
$error = false;
foreach($methods as $name) {
    $error = true;
    echo 'Extra Method: Win\Gui\MenuItem::' . $name . '()' . PHP_EOL;
}
if (!$error) {
    echo 'No extra methods found' . PHP_EOL;
}


// has the following properties
$expected_properties = array('id',
                    'text',
                    'enabled',
                    'hilite',
                    'checked',
                    'default',
                    'radio',
                    'image');
$properties = array_keys(get_class_vars('Win\Gui\MenuItem'));
$error = false;
foreach($expected_properties as $name) {
    if (!property_exists('Win\Gui\MenuItem', $name) || !in_array($name, $properties)) {
        $error = true;
        echo 'Missing Property: Win\Gui\MenuItem->' . $name . PHP_EOL;
    } else {
        unset($properties[array_search($name, $expected_properties)]);
    }
}
if (!$error) {
    echo "No missing properties, checked " . count($expected_properties) . '!' . PHP_EOL;
}
$error = false;
foreach($properties as $name) {
    $error = true;
    echo 'Extra Property: Win\Gui\MenuItem->' . $name . PHP_EOL;
}
if (!$error) {
    echo 'No extra properties found' . PHP_EOL;
}

// has the following constants
$expected_constants = array();
// FAIL - I don't want to use reflection, php needs get_class_constants
$reflect = new ReflectionClass('Win\Gui\MenuItem');
$constants = $reflect->getConstants();
unset($reflect);
$error = false;
foreach($expected_constants as $name) {
    if (!defined('Win\Gui\MenuItem::' . $name) || !in_array($name, $constants)) {
        $error = true;
        echo 'Missing Constant: Win\Gui\MenuItem::' . $name . PHP_EOL;
    } else {
        unset($constants[array_search($name, $expected_constants)]);
    }
}
if (!$error) {
    echo "No missing constants, checked " . count($expected_constants) . '!' . PHP_EOL;
}

$error = false;
foreach($constants as $name) {
    $error = true;
    echo 'Extra Constant: Win\Gui\MenuItem::' . $name . PHP_EOL;
}
if (!$error) {
    echo 'No extra constants found' . PHP_EOL;
}
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
No missing methods, checked %d!
No extra methods found
No missing properties, checked %d!
No extra properties found
No missing constants, checked %d!
No extra constants found