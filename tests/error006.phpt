--TEST--
json_post with malformed JSON [json_throw] (https://github.com/m6w6/ext-json_post/issues/3)
--EXTENSIONS--
json_post
--INI--
json_post.flags = 4194305
--POST_RAW--
Content-Type: application/json

{"a
--FILE_EXTERNAL--
error.inc
--EXPECTHEADERS--
--EXPECT--
DONE
