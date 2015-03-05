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


#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/json/php_json.h"
#include "php_json_post.h"
#include "SAPI.h"

ZEND_DECLARE_MODULE_GLOBALS(json_post);

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("json_post.flags", "1", PHP_INI_PERDIR, OnUpdateLong, flags, zend_json_post_globals, json_post_globals)
PHP_INI_END()

static void php_json_post_init_globals(zend_json_post_globals *json_post_globals)
{
#if PHP_VERSION_ID >= 50400
	json_post_globals->flags = PHP_JSON_OBJECT_AS_ARRAY;
#else
	json_post_globals->flags = 1;
#endif
}

PHP_MINFO_FUNCTION(json_post)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "json_post support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

static SAPI_POST_HANDLER_FUNC(php_json_post_handler)
{
	zval *zarg = arg;
	char *json_str = NULL;
	size_t json_len = 0;

#if PHP_VERSION_ID >= 50600
	if (SG(request_info).request_body) {
		/* FG(stream_wrappers) not initialized yet, so we cannot use php://input */
		php_stream_rewind(SG(request_info).request_body);
		json_len = php_stream_copy_to_mem(SG(request_info).request_body, &json_str, PHP_STREAM_COPY_ALL, 0);
	}
#else
	json_str = SG(request_info).raw_post_data;
	json_len = SG(request_info).raw_post_data_length;
#endif

	if (json_len) {
		zval zjson;

		INIT_ZVAL(zjson);
#if PHP_VERSION_ID >= 50400
		php_json_decode_ex(&zjson, json_str, json_len, JSON_POST_G(flags), PG(max_input_nesting_level) TSRMLS_CC);
		if (Z_TYPE(zjson) != IS_NULL) {
			zval_dtor(zarg);
			ZVAL_COPY_VALUE(zarg, (&zjson));
#else
		php_json_decode(&zjson, json_str, json_len, (zend_bool)(JSON_POST_G(flags)&1), PG(max_input_nesting_level) TSRMLS_CC);
		if (Z_TYPE(zjson) != IS_NULL) {
			zval_dtor(zarg);
			zarg->value = zjson.value;
			Z_TYPE_P(zarg) = Z_TYPE(zjson);
#endif
		}
	}
#if PHP_VERSION_ID >= 50600
	if (json_str) {
		efree(json_str);
	}
#endif
}

PHP_MINIT_FUNCTION(json_post)
{
	sapi_post_entry entry = {NULL, 0, NULL, NULL};

	entry.post_reader = sapi_read_standard_form_data;
	entry.post_handler = php_json_post_handler;

	entry.content_type = "text/json";
	entry.content_type_len = sizeof("text/json")-1;
	sapi_register_post_entry(&entry TSRMLS_CC);

	ZEND_INIT_MODULE_GLOBALS(json_post, php_json_post_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(json_post)
{
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}

zend_function_entry json_post_functions[] = {
	{0}
};

static zend_module_dep json_post_module_deps[] = {
	ZEND_MOD_REQUIRED("json")
	{NULL, NULL, NULL, 0}
};

zend_module_entry json_post_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	json_post_module_deps,
	"json_post",
	json_post_functions,
	PHP_MINIT(json_post),
	PHP_MSHUTDOWN(json_post),
	NULL,
	NULL,
	PHP_MINFO(json_post),
	PHP_JSON_POST_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_JSON_POST
ZEND_GET_MODULE(json_post)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
