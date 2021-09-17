--TEST--
json_post with malformed JSON [warning] (https://github.com/m6w6/ext-json_post/issues/3)
--EXTENSIONS--
json_post
--INI--
json_post.onerror.warning = on
--POST_RAW--
Content-Type: application/json

{"a
--FILE_EXTERNAL--
error.inc
--EXPECTHEADERS--
--EXPECTF--
Warning: json_post: json_decode failed with error code: 3 in %s on line %s
DONE
