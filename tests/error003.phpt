--TEST--
json_post with malformed JSON (https://github.com/m6w6/ext-json_post/issues/3)
--SKIPIF--
<?php
extension_loaded("json_post") or die("skip need json_post support\n");
?>
--INI--
json_post.error_response = 444
json_post.error_exit = true
--POST_RAW--
Content-Type: application/json

{
	"greeting": "Hello World
}
--FILE--
<?php
var_dump($_POST);
var_dump(http_response_code());
?>
Done
--EXPECTHEADERS--
Status: 444
X-JSON-Error-Code: 3
--EXPECT--
