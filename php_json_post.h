/*
    +--------------------------------------------------------------------+
    | PECL :: json_post                                                  |
    +--------------------------------------------------------------------+
    | Redistribution and use in source and binary forms, with or without |
    | modification, are permitted provided that the conditions mentioned |
    | in the accompanying LICENSE file are met.                          |
    +--------------------------------------------------------------------+
    | Copyright (c) 2015, Michael Wallner <mike@php.net>                 |
    +--------------------------------------------------------------------+
*/

#ifndef PHP_JSON_POST_H
#define PHP_JSON_POST_H

extern zend_module_entry json_post_module_entry;
#define phpext_json_post_ptr &json_post_module_entry

#define PHP_JSON_POST_VERSION "1.1.0"

#ifdef PHP_WIN32
#	define PHP_JSON_POST_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_JSON_POST_API extern __attribute__ ((visibility("default")))
#else
#	define PHP_JSON_POST_API extern
#endif

#ifdef ZTS
#	include "TSRM.h"
#endif

#if PHP_VERSION_ID < 70000
typedef long zend_long;
#endif

ZEND_BEGIN_MODULE_GLOBALS(json_post)
	zend_long flags;
	struct {
		zend_long response;
		zend_bool warning;
		zend_bool exit;
	} onerror;
ZEND_END_MODULE_GLOBALS(json_post)

ZEND_EXTERN_MODULE_GLOBALS(json_post);

#ifdef ZTS
#	define JSON_POST_G(v) TSRMG(json_post_globals_id, zend_json_post_globals *, v)
#else
#	define JSON_POST_G(v) (json_post_globals.v)
#endif

#endif	/* PHP_JSON_POST_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
