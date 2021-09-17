<?php
if ($_POST)
	var_dump(compact("_POST"));
if (PHP_VERSION_ID >= 70000 && ($json_last_error = json_last_error()))
	var_dump(compact("json_last_error"));
if (($JSON_POST_ERROR = JSON_POST_ERROR) !== (PHP_VERSION_ID < 70000 ? 4 : 3))
	var_dump(compact("JSON_POST_ERROR"));
if (($http_response_code = http_response_code()) != ini_get("json_post.onerror.response") && ini_get("json_post.onerror.response"))
	var_dump(compact("http_response_code"));
?>
DONE
