--TEST--
json_post with most common types, as array
--SKIPIF--
<?php
extension_loaded("json_post") or die("skip need json_post support\n");
?>
--POST_RAW--
Content-type: text/json

{
	"null": null,
	"bool": true,
	"int": 123,
	"float": 123.123,
	"string": "Hello World",
	"array": [1,2,3],
	"object": {
		"array": [1,2,3],
		"unicode": "\u00D6sterreich"
	}
}
--FILE--
<?php
var_dump($_POST, json_last_error());
?>
Done
--EXPECTF--
array(7) {
  ["null"]=>
  NULL
  ["bool"]=>
  bool(true)
  ["int"]=>
  int(123)
  ["float"]=>
  float(123.123)
  ["string"]=>
  string(11) "Hello World"
  ["array"]=>
  array(3) {
    [0]=>
    int(1)
    [1]=>
    int(2)
    [2]=>
    int(3)
  }
  ["object"]=>
  array(2) {
    ["array"]=>
    array(3) {
      [0]=>
      int(1)
      [1]=>
      int(2)
      [2]=>
      int(3)
    }
    ["unicode"]=>
    string(11) "Österreich"
  }
}
int(0)
Done