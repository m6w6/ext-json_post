--TEST--
json_post with empty object
--SKIPIF--
<?php
extension_loaded("json_post") or die("skip need json_post support\n");
?>
--POST_RAW--
Content-type: text/json

{
}
--FILE--
<?php
var_dump($_POST, json_last_error());
?>
Done
--EXPECTF--
array(0) {
}
int(0)
Done
