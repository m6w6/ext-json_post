# PECL :: json_post [![Build Status](https://travis-ci.org/m6w6/ext-json_post.svg?branch=master)](https://travis-ci.org/m6w6/ext-sjon_post)

JSON POST handler 

## About

This tiny extension provides a PHP content type handler for JSON to PHP's
form data parser. If the `Content-Type` of an incoming request is `text/json`
or `application/json`, the JSON contents of the request body will by parsed
into `$_POST`.

This extension does not provide any constants, functions or classes.

### INI Entries

* `json_post.flags = 1`  
  Takes any combination of JSON_ flags which will be passed to `json_decode()`.  
  The default is `JSON_OBJECT_AS_ARRAY`.
