--TEST--
Win\Gui\Windowing reflection output
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
Reflection::export(new ReflectionClass('Win\Gui\Windowing'));
?>
--EXPECTF--
Interface [ <internal:wingui> interface Win\Gui\Windowing ] {

  - Constants [0] {
  }

  - Static properties [0] {
  }

  - Static methods [0] {
  }

  - Properties [0] {
  }

  - Methods [1] {
    Method [ <internal:wingui> abstract public method adjustSize ] {

      - Parameters [2] {
        Parameter #0 [ <required> $height ]
        Parameter #1 [ <required> $width ]
      }
    }
  }
}