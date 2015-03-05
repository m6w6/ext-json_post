PHP_ARG_ENABLE(json-post, whether to enable json-post support,
[  --enable-json-post      Enable json-post support])

if test "$PHP_JSON_POST" != "no"; then
  PHP_NEW_EXTENSION(json_post, php_json_post.c, $ext_shared)
  PHP_ADD_EXTENSION_DEP(json_post, json, true)
fi
