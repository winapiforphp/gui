--TEST--
Win\Gui\Control\Statusbar::SB_SETTEXT message
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

// check magic callback
class magic extends Statusbar {
    public function onSbSettext() {
        var_dump(func_get_args());
    }
}

$statusbar = new magic(new Window);
$statusbar->setParts(array(15, 25));
$statusbar->send(Statusbar::SB_SETTEXT, 'My Text', 1, Statusbar::POPOUT);

// settext callbacks
function settext_args_callback($part, $type, $string) {
    var_dump($string === 'My Text');
    var_dump($part === 1);
    var_dump($type === Statusbar::POPOUT);
}

function settext_return_value_callback(&$return_value) {
    $return_value = 1;
}

function settext_simple_callback() {
    $total_args = func_num_args();
    var_dump($total_args === 0);
}

function settext_post_callback($part, $type, $string) {
    echo "we did a post of $string to part $part\n";
}

$statusbar = new Statusbar(new Window);
$statusbar->setParts(array(15, 25));

// check standard connect
$id = $statusbar->connect(Statusbar::SB_SETTEXT, 'settext_args_callback');
$statusbar->send(Statusbar::SB_SETTEXT, 'My Text', 1, Statusbar::POPOUT);
$statusbar->disconnect(Statusbar::SB_SETTEXT, $id);

// check return value and lresult manipulation
$id = $statusbar->connectFull(Statusbar::SB_SETTEXT, 'settext_return_value_callback');
var_dump($statusbar->send(Statusbar::SB_SETTEXT, 'text') === true);
$statusbar->disconnect(Statusbar::SB_SETTEXT, $id);

// check simple connection
$id = $statusbar->connectSimple(Statusbar::SB_SETTEXT, 'settext_simple_callback');
$statusbar->send(Statusbar::SB_SETTEXT, 'text');
$statusbar->disconnect(Statusbar::SB_SETTEXT, $id);

/* Wrong number args - none */
try {
    $statusbar->send();
    trigger_error('Win\Gui\Control\Statusbar::SB_SETTEXT requires at least 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 1 */
try {
    $statusbar->send(Statusbar::SB_SETTEXT);
    trigger_error('Win\Gui\Control\Statusbar::SB_SETTEXT requires at least 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - too many */
try {
    $statusbar->send(Statusbar::SB_SETTEXT, 'string', 1, 0, 1);
    trigger_error('Win\Gui\Control\Statusbar::SB_SETTEXT requires at most 4 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $statusbar->send(array());
    trigger_error('Win\Gui\Control\Statusbar::SB_SETTEXT requires arg 1 to be long');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 2 */
try {
    $statusbar->send(Statusbar::SB_SETTEXT, array());
    trigger_error('Win\Gui\Control\Statusbar::SB_SETTEXT requires arg 2 to be string');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 3 */
try {
    $statusbar->send(Statusbar::SB_SETTEXT, 'string', array());
    trigger_error('Win\Gui\Control\Statusbar::SB_SETTEXT requires arg 3 to be long');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 4 */
try {
    $statusbar->send(Statusbar::SB_SETTEXT, 'string', 1, array());
    trigger_error('Win\Gui\Control\Statusbar::SB_SETTEXT requires arg 4 to be long');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
array(4) {
  [0]=>
  NULL
  [1]=>
  int(1)
  [2]=>
  int(512)
  [3]=>
  string(7) "My Text"
}
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Win\Gui\Messaging::send() expects at least 1 parameter, 0 given
Win\Gui\Messaging::send() expects at least 2 parameters, 1 given
Win\Gui\Messaging::send() expects at most 4 parameters, 5 given
Win\Gui\Messaging::send() expects parameter 1 to be long, array given
Win\Gui\Messaging::send() expects parameter 2 to be string, array given
Win\Gui\Messaging::send() expects parameter 3 to be long, array given
Win\Gui\Messaging::send() expects parameter 4 to be long, array given