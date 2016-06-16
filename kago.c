/****************************************************************************
 * vim: set ts=4 sw=4 expandtab syntax=c
 *
 * Kago audit extension
 * kago.c: PHP and Zend entry points
 *
 * Copyright (c) 2016 J. Hipps / Neo-Retro Group, Inc.
 * <https://ycnrg.org/>
 *
 * @author      J. Hipps <jacob@ycnrg.org>
 * @license     MIT
 * @repo        https://git.ycnrg.org/projects/YKG/repos/kago
 *
 ****************************************************************************/

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "php.h"
#include "php_version.h"
#include "SAPI.h"
#include "php_ini.h"

#ifdef ZTS
    #include "TSRM.h"
#endif

#include "zend.h"
#include "zend_extensions.h"
#include "zend_closures.h"

#include "kago.h"


int kago_zend_init = 0;

ZEND_DECLARE_MODULE_GLOBALS(kago)

/**
 * Function Exports table
 */

zend_function_entry kago_functions[] = {
    PHP_FE(kago_version, NULL)
    { NULL, NULL, NULL }
};


/**
 * Zend module properties
 */

zend_module_entry kago_module_entry = {
    STANDARD_MODULE_HEADER,
    "kago",
    kago_functions, /* Function entries */
    PHP_MINIT(kago), /* Module init */
    PHP_MSHUTDOWN(kago), /* Module shutdown */
    NULL, /* Request init */
    NULL, /* Request shutdown */
    PHP_MINFO(kago), /* Module information */
    "0.1.0", /* Replace with version number for your extension */
    NO_MODULE_GLOBALS,
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_KAGO
    ZEND_GET_MODULE(kago)
#endif

/**
 * php.ini config & globals init
 */
PHP_INI_BEGIN()
    STD_PHP_INI_BOOLEAN("kago.enabled", "1", PHP_INI_ALL, OnUpdateBool, enabled, zend_kago_globals, kago_globals)
    STD_PHP_INI_BOOLEAN("kago.log_path", "/var/log/kago.log", PHP_INI_ALL, OnUpdateString, log_path, zend_kago_globals, kago_globals)
PHP_INI_END()

static void php_kago_init_globals(zend_kago_globals* kg TSRMLS_DC) {
    kg->enabled = 1;
    kg->log_path = strdup("/var/log/kago.log");
}

/**
 * Module init function
 */

PHP_MINIT_FUNCTION(kago) {
    ZEND_INIT_MODULE_GLOBALS(kago, php_kago_init_globals, NULL);
    REGISTER_INI_ENTRIES();
    return SUCCESS;
}

/**
 * Module shutdown function
 */

PHP_MSHUTDOWN_FUNCTION(kago) {
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}


/**
 * phpinfo callback
 */

PHP_MINFO_FUNCTION(kago) {
    php_info_print_table_start();
    php_info_print_table_header(2, "Version", KAGO_VERSION);
    php_info_print_table_header(2, "Kago enabled (kago.enabled)", (INI_BOOL("kago.enabled") ? "enabled" : "disabled"));
    php_info_print_table_header(2, "Log path (kago.log_path)", INI_STR("kago.log_path"));
    php_info_print_table_end();
    DISPLAY_INI_ENTRIES();
}

/**
 * Exported Functions
 */

PHP_FUNCTION(kago_version) {
    RETURN_STRING(KAGO_VERSION, 1);
}

/**
 * Zend init & shutdown handlers
 */

ZEND_DLEXPORT int kago_zend_startup(zend_extension *extension) {

    kago_zend_init = 1;

    // return copy of our "regular" module object
    // which allows us to be a hybrid PHP module + Zend extension
    return zend_startup_module(&kago_module_entry);
}

ZEND_DLEXPORT int kago_zend_shutdown(zend_extension *extension) {
}


/**
 * Zend extension init & properties
 */

#ifndef ZEND_EXT_API
#define ZEND_EXT_API
#endif
ZEND_EXTENSION();

ZEND_DLEXPORT zend_extension zend_extension_entry = {
    KAGO_NAME, /* name */
    KAGO_VERSION, /* version */
    KAGO_AUTHOR, /* author */
    KAGO_URL, /* url */
    KAGO_COPYRIGHT_SHORT, /* copyright */

    kago_zend_startup, /* startup_func_t startup */
    kago_zend_shutdown, /* shutdown_func_t shutdown */

    NULL, /* activate_func_t activate */
    NULL, /* deactivate_func_t deactivate */
    NULL, /* message_handler_func_t message_handler */
    NULL, /* op_array_handler_func_t op_array_handler */
    NULL, /* statement_handler_func_t statement_handler */
    NULL, /* fcall_begin_handler_func_t fcall_begin_handler */
    NULL, /* fcall_end_handler_func_t fcall_end_handler */
    NULL, /* op_array_ctor_func_t op_array_ctor */
    NULL, /* op_array_dtor_func_t op_array_dtor */
    STANDARD_ZEND_EXTENSION_PROPERTIES
};
