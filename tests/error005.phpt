--TEST--
json_post with malformed JSON (https://github.com/m6w6/ext-json_post/issues/3)
--SKIPIF--
<?php
extension_loaded("json_post") or die("skip need json_post support\n");
if (PHP_VERSION_ID < 70000) doe("skip need PHP-7.0+\n");
?>
--INI--
json_post.error_response = 444
json_post.flags = 1
--POST_RAW--
Content-Type: application/json

{
	"greeting": "Hello World
}
--FILE--
<?php
http_response_code(400);
var_dump($_POST);
var_dump(http_response_code());
?>
Done
--EXPECTHEADERS--
Status: 400 Bad Request
X-JSON-Error-Code: 3
--EXPECTF--
array(0) {
}
int(400)
Done
