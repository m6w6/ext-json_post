--TEST--
json_post with malformed JSON [default] (https://github.com/m6w6/ext-json_post/issues/3)
--POST_RAW--
Content-Type: application/json

{"a
--FILE_EXTERNAL--
error.inc
--EXPECTHEADERS--
--EXPECT--
DONE
