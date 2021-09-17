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
	STD_PHP_INI_ENTRY("json_post.onerror.response", "0", PHP_INI_PERDIR, OnUpdateLong, onerror.response, zend_json_post_globals, json_post_globals)
	STD_PHP_INI_ENTRY("json_post.onerror.exit", "0", PHP_INI_PERDIR, OnUpdateBool, onerror.exit, zend_json_post_globals, json_post_globals)
	STD_PHP_INI_ENTRY("json_post.onerror.warning", "0", PHP_INI_PERDIR, OnUpdateBool, onerror.warning, zend_json_post_globals, json_post_globals)
PHP_INI_END()

static void php_json_post_init_globals(zend_json_post_globals *json_post_globals)
{
	memset(json_post_globals, 0, sizeof(*json_post_globals));
#if PHP_VERSION_ID >= 50400
	json_post_globals->flags = PHP_JSON_OBJECT_AS_ARRAY;
#else
	json_post_globals->flags = 1;
#endif
}

#ifndef TSRMLS_CC
#	define TSRMLS_D
#	define TSRMLS_C
#	define TSRMLS_CC
#endif

PHP_MINFO_FUNCTION(json_post)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "json_post support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

#if PHP_VERSION_ID < 70000
#	undef JSON_G
#	ifdef ZTS
#		define JSON_G(v) TSRMG(*JSON_POST_G(json_module)->globals_id_ptr, zend_json_globals *, v)
#	else
#		define JSON_G(v) ((zend_json_globals *) JSON_POST_G(json_module)->globals_ptr)->v
#	endif
#endif

static SAPI_POST_HANDLER_FUNC(php_json_post_handler)
{
	int module_number = 0;

#if PHP_VERSION_ID >= 70000
	zend_string *json = NULL;

	if (SG(request_info).request_body) {
		/* FG(stream_wrappers) not initialized yet, so we cannot use php://input */
		php_stream_rewind(SG(request_info).request_body);
		json = php_stream_copy_to_mem(SG(request_info).request_body, PHP_STREAM_COPY_ALL, 0);
	}

	if (json) {
		if (json->len) {
			zval tmp;
			long flags = JSON_POST_G(flags);

#ifdef PHP_JSON_THROW_ON_ERROR
			/* there's no execute data, so we must ensure json_decode() is not throwing */
			flags &= ~PHP_JSON_THROW_ON_ERROR;
#endif

			ZVAL_NULL(&tmp);
			php_json_decode_ex(&tmp, json->val, json->len, flags, PG(max_input_nesting_level));

			switch (Z_TYPE(tmp)) {
			case IS_OBJECT:
			case IS_ARRAY:
				if (zend_hash_num_elements(HASH_OF(&tmp))) {
					zval_dtor(arg);
					ZVAL_COPY_VALUE(&PG(http_globals)[TRACK_VARS_POST], &tmp);
				} else {
					/* PHP-7.4 optimizes empty array */
					zval_ptr_dtor(&tmp);
				}
				break;
			default:
				break;
			}
		}
		zend_string_release(json);
	}

#else

	zval *zarg = arg;
	char *json_str = NULL;
	size_t json_len = 0;

#	if PHP_VERSION_ID >= 50600
	if (!SG(request_info).request_body) {
		return;
	}

	/* FG(stream_wrappers) not initialized yet, so we cannot use php://input */
	php_stream_rewind(SG(request_info).request_body);
	json_len = php_stream_copy_to_mem(SG(request_info).request_body, &json_str, PHP_STREAM_COPY_ALL, 0);
#	else
	json_str = SG(request_info).raw_post_data;
	json_len = SG(request_info).raw_post_data_length;
#	endif

	if (json_len) {
		zval zjson;

		INIT_ZVAL(zjson);

#	if PHP_VERSION_ID >= 50400
		php_json_decode_ex(&zjson, json_str, json_len, JSON_POST_G(flags), PG(max_input_nesting_level) TSRMLS_CC);
		if (Z_TYPE(zjson) != IS_NULL) {
			zval_dtor(zarg);
			ZVAL_COPY_VALUE(zarg, (&zjson));
#	else
		php_json_decode(&zjson, json_str, json_len, (zend_bool)(JSON_POST_G(flags)&1), PG(max_input_nesting_level) TSRMLS_CC);
		if (Z_TYPE(zjson) != IS_NULL) {
			zval_dtor(zarg);
			zarg->value = zjson.value;
			Z_TYPE_P(zarg) = Z_TYPE(zjson);
#	endif
		}
	}
#	if PHP_VERSION_ID >= 50600
	if (json_str) {
		efree(json_str);
	}
#	endif
#endif

	REGISTER_LONG_CONSTANT("JSON_POST_ERROR", JSON_G(error_code), CONST_CS);

	if (JSON_G(error_code)) {
		if (JSON_POST_G(onerror.response)) {
			sapi_header_op(SAPI_HEADER_SET_STATUS, (void *) (zend_long) JSON_POST_G(onerror.response) TSRMLS_CC);
		}
		if (JSON_POST_G(onerror.warning)) {
			zend_error(E_WARNING, "json_post: json_decode failed with error code: %d", JSON_G(error_code));
		}
		if (JSON_POST_G(onerror.exit)) {
			sapi_send_headers(TSRMLS_C);
			zend_bailout();
		}
#if PHP_VERSION_ID >= 70000 && PHP_VERSION_ID < 80000
		/* ext/json in PHP-7 fails to reset error_code in RINIT */
		JSON_G(error_code) = 0;
#endif
	}
}

PHP_MINIT_FUNCTION(json_post)
{
	sapi_post_entry json_post_entries[] = {
		{ "text/json", sizeof("text/json")-1, sapi_read_standard_form_data, php_json_post_handler },
		{ "application/json", sizeof("application/json")-1, sapi_read_standard_form_data, php_json_post_handler },
		{ NULL, 0, NULL, NULL }
	};

	sapi_register_post_entries(json_post_entries TSRMLS_CC);

	ZEND_INIT_MODULE_GLOBALS(json_post, php_json_post_init_globals, NULL);

#if PHP_VERSION_ID < 70000
	zend_hash_find(&module_registry, ZEND_STRS("json"), (void **) &JSON_POST_G(json_module));
#endif

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
