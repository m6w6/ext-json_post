--TEST--
json_post with > 32bits integer as float on 32bits
--SKIPIF--
<?php
extension_loaded("json_post") or die("skip need json_post support\n");
if (PHP_INT_SIZE != 4) die("skip this test is for 32bits platform only");
?>
--INI--
json_post.flags=2;JSON_BIGINT_AS_STRING
--POST_RAW--
Content-type: text/json

{
	"bigint": 12345678901234
}
--FILE--
<?php
var_dump($_POST, json_last_error());
?>
Done
--EXPECTF--
object(stdClass)#%d (1) {
  ["bigint"]=>
  string(14) "12345678901234"
}
int(0)
Done