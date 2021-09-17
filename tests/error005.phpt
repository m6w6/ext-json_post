--TEST--
json_post with malformed JSON [response override] (https://github.com/m6w6/ext-json_post/issues/3)
--INI--
json_post.onerror.response = 444
--POST_RAW--
Content-Type: application/json

{"a
--FILE--
<?php
if (JSON_POST_ERROR)
	http_response_code(400);
include "./error.inc";
?>
--EXPECTHEADERS--
Status: 400 Bad Request
--EXPECT--
array(1) {
  ["http_response_code"]=>
  int(400)
}
DONE
