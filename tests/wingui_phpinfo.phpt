--TEST--
phpinfo specific to wingui
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
ob_start();
phpinfo(INFO_MODULES);
$data = ob_get_clean();
$data = explode("\n\n", $data);
foreach($data as $key => $info) {
    if ($info === 'wingui') {
        break;
    }
}
$data = $data[$key + 1];
var_dump($data);
?>
--EXPECTF--
string(%d) "Win32 Gui => enabled
Version => %s