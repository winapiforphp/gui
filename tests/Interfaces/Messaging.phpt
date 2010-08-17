--TEST--
Win\Gui\Messaging Interface Reflection
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
ReflectionClass::export('Win\Gui\Messaging');
?>
--EXPECT--
Interface [ <internal:wingui> interface Win\Gui\Messaging ] {

  - Constants [0] {
  }

  - Static properties [0] {
  }

  - Static methods [0] {
  }

  - Properties [0] {
  }

  - Methods [10] {
    Method [ <internal:wingui> abstract public method connect ] {

      - Parameters [2] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $callback ]
      }
    }

    Method [ <internal:wingui> abstract public method connectFull ] {

      - Parameters [2] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $callback ]
      }
    }

    Method [ <internal:wingui> abstract public method connectSimple ] {

      - Parameters [2] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $callback ]
      }
    }

    Method [ <internal:wingui> abstract public method disconnect ] {

      - Parameters [2] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $id ]
      }
    }

    Method [ <internal:wingui> abstract public method getMessage ] {

      - Parameters [2] {
        Parameter #0 [ <optional> $filtermin ]
        Parameter #1 [ <optional> $filtermax ]
      }
    }

    Method [ <internal:wingui> abstract public method peek ] {

      - Parameters [3] {
        Parameter #0 [ <optional> $filtermin ]
        Parameter #1 [ <optional> $filtermax ]
        Parameter #2 [ <optional> $remove ]
      }
    }

    Method [ <internal:wingui> abstract public method post ] {

      - Parameters [1] {
        Parameter #0 [ <required> $msg ]
      }
    }

    Method [ <internal:wingui> abstract public method send ] {

      - Parameters [1] {
        Parameter #0 [ <required> $msg ]
      }
    }

    Method [ <internal:wingui> abstract public method sendCallback ] {

      - Parameters [3] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $callback ]
        Parameter #2 [ <required> $data ]
      }
    }

    Method [ <internal:wingui> abstract public method sendTimeout ] {

      - Parameters [3] {
        Parameter #0 [ <required> $msg ]
        Parameter #1 [ <required> $flags ]
        Parameter #2 [ <required> $timeout ]
      }
    }
  }
}