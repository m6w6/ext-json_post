--TEST--
json_post with malformed JSON [response] (https://github.com/m6w6/ext-json_post/issues/3)
--EXTENSIONS--
json_post
--INI--
json_post.onerror.response = 400
--POST_RAW--
Content-Type: application/json

{"a
--FILE_EXTERNAL--
error.inc
--EXPECTHEADERS--
Status: 400 Bad Request
--EXPECT--
DONE
