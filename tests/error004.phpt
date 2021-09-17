--TEST--
json_post with malformed JSON [exit] (https://github.com/m6w6/ext-json_post/issues/3)
--INI--
json_post.onerror.exit = true
--POST_RAW--
Content-Type: application/json

{"a
--FILE_EXTERNAL--
error.inc
--EXPECTHEADERS--
--EXPECT--
