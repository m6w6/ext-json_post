# PECL :: json_post

JSON POST handler

## About

This tiny extension provides a PHP content type handler for "text/json" to PHP's
form data parser. If the `Content-Type` of an incoming request is `text/json`,
the JSON contents of the request body will by parsed into `$_POST`.

This extension does not provide any constants, functions or classes.

### INI Entries

* `json_post.flags = 1`  
  Takes any combination of JSON_ flags which will be passed to `json_decode()`.  
  The default is `JSON_OBJECT_AS_ARRAY`.
