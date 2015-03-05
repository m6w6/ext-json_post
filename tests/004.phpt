--TEST--
json_post with > 32bits integer as float on 64bits
--SKIPIF--
<?php
extension_loaded("json_post") or die("skip need json_post support\n");
if (PHP_INT_SIZE != 8) die("skip this test is for 64bits platform only");
?>
--INI--
json_post.flags=0
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
  int(12345678901234)
}
int(0)
Done