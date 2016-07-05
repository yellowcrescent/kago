/****************************************************************************
 * vim: set ts=4 sw=4 expandtab syntax=c
 *
 * Kago audit extension
 * kago.h: Main header
 *
 * Copyright (c) 2016 J. Hipps / Neo-Retro Group, Inc.
 * <https://ycnrg.org/>
 *
 * @author      J. Hipps <jacob@ycnrg.org>
 * @license     MIT
 * @repo        https://git.ycnrg.org/projects/YKG/repos/kago
 *
 ****************************************************************************/

#ifndef KAGO_H
#define KAGO_H

// PHP includes //

#include "php.h"
#include "php_version.h"
#include "SAPI.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#ifdef ZTS
    #include "TSRM.h"
#endif

#include "zend.h"
#include "zend_extensions.h"
#include "zend_closures.h"
#include "zend_hash.h"

// Module infos //

#define KAGO_NAME "Kago"
#define KAGO_VERSION "0.1.2-wip"
#define KAGO_AUTHOR "Jacob Hipps"
#define KAGO_URL "https://ycnrg.org/"
#define KAGO_COPYRIGHT "Copyright (c) 2016 J. Hipps/Neo-Retro Group, Inc."
#define KAGO_COPYRIGHT_SHORT "Copyright (c) 2016"

// Macros & Defines //

#define KAGO_MAXPATH	2048

#if PHP_VERSION_ID >= 70000
	#define KAGO_CALLED_FUNCTION execute_data->func->common.function_name->val
#else
	#define KAGO_CALLED_FUNCTION EG(current_execute_data)->function_state.function->common.function_name
#endif

// Typedefs & Structs //

typedef struct _kago_overfuncs {
	char *funcname;
	void *fptr;
} kago_overfuncs;

// Module globals //

ZEND_BEGIN_MODULE_GLOBALS(kago)
	zend_bool enabled;
	zend_bool restrict_php;
	char* log_path;
ZEND_END_MODULE_GLOBALS(kago)

#ifdef ZTS
	#define KAGO_G(v) TSRMG(kago_globals_id, zend_kago_globals*, v)
#else
	#define KAGO_G(v) (kago_globals.v)
#endif

// Utility functions //

int replace_function(char *fname, void *fptr TSRMLS_DC);
int restore_function(char *fname, void *fptr TSRMLS_DC);
int kago_fovr_add(char *funcname, void *fptr);
void kago_fovr_free();
void* kago_fovr_get(char *funcname);
void kago_parse_sglobals(char *vname, char *vkey TSRMLS_DC);

// Module framework function declarations //

PHP_MINIT_FUNCTION(kago);
PHP_MSHUTDOWN_FUNCTION(kago);
PHP_MINFO_FUNCTION(kago);
PHP_RINIT_FUNCTION(kago);
PHP_RSHUTDOWN_FUNCTION(kago);

// Exported functions //

PHP_FUNCTION(kago_version);
PHP_FUNCTION(kago_show_func);
PHP_FUNCTION(kago_zend_precall_hook);
PHP_FUNCTION(kago_fopen_precall_hook);

// Zend functions //

ZEND_DLEXPORT int kago_zend_startup(zend_extension *extension);
ZEND_DLEXPORT int kago_zend_shutdown(zend_extension *extension);
ZEND_DLEXPORT void kago_zend_activate();
ZEND_DLEXPORT void kago_zend_deactivate();
ZEND_DLEXPORT void kago_fcall_begin_handler(zend_op_array *op_array);
ZEND_DLEXPORT void kago_fcall_end_handler(zend_op_array *op_array);

// Logging functions //

int log_init(logfile TSRMLS_DC);
void log_close();
void log_write(char *fmt, ...);


// Module entry point //

extern zend_module_entry kago_module_entry;
#define phpext_kago_ptr &kago_module_entry

// Pragma //

#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

#endif
