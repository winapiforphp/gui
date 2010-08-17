--TEST--
Win\Gui\Inputing Interface Reflection
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
ReflectionClass::export('Win\Gui\Inputing');
?>
--EXPECT--
Interface [ <internal:wingui> interface Win\Gui\Inputing ] {

  - Constants [0] {
  }

  - Static properties [0] {
  }

  - Static methods [0] {
  }

  - Properties [0] {
  }

  - Methods [15] {
    Method [ <internal:wingui> public method enable ] {
    }

    Method [ <internal:wingui> public method disable ] {
    }

    Method [ <internal:wingui> public method isEnabled ] {
    }

    Method [ <internal:wingui> public method activate ] {
    }

    Method [ <internal:wingui> public method deactivate ] {
    }

    Method [ <internal:wingui> public method isActive ] {
    }

    Method [ <internal:wingui> public method focus ] {
    }

    Method [ <internal:wingui> public method unfocus ] {
    }

    Method [ <internal:wingui> public method hasFocus ] {
    }

    Method [ <internal:wingui> public method capture ] {
    }

    Method [ <internal:wingui> public method release ] {
    }

    Method [ <internal:wingui> public method hasCapture ] {
    }

    Method [ <internal:wingui> public method trackMouse ] {

      - Parameters [2] {
        Parameter #0 [ <required> $type ]
        Parameter #1 [ <required> $time ]
      }
    }

    Method [ <internal:wingui> public method queryMouse ] {

      - Parameters [2] {
        Parameter #0 [ <required> $type ]
        Parameter #1 [ <required> $time ]
      }
    }

    Method [ <internal:wingui> public method dragDetect ] {

      - Parameters [2] {
        Parameter #0 [ <required> $x ]
        Parameter #1 [ <required> $y ]
      }
    }
  }
}