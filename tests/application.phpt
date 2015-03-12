--TEST--
json_post with Content-Type: application/json
--SKIPIF--
<?php
extension_loaded("json_post") or die("skip need json_post support\n");
?>
--POST_RAW--
Content-Type: application/json

{
	"greeting": "Hello World"
}
--FILE--
<?php
var_dump($_POST);
?>
Done
--EXPECTF--
array(1) {
  ["greeting"]=>
  string(11) "Hello World"
}
Done
