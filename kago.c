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
#include "ext/standard/info.h"

#ifdef ZTS
    #include "TSRM.h"
#endif

#include "zend.h"
#include "zend_extensions.h"
#include "zend_closures.h"
#include "zend_hash.h"

#include "kago.h"

int func_overrides_len = 0;
kago_overfuncs **func_overrides = NULL;

int kago_zend_init = 0;
int kago_active = 0;

ZEND_DECLARE_MODULE_GLOBALS(kago)

/**
 * Function Exports table
 */

zend_function_entry kago_functions[] = {
    PHP_FE(kago_version, NULL)
    PHP_FE(kago_show_func, NULL)
    PHP_FE_END
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
    PHP_RINIT(kago), /* Request init */
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
    // register globals & ini settings
    ZEND_INIT_MODULE_GLOBALS(kago, php_kago_init_globals, NULL);
    REGISTER_INI_ENTRIES();

    // ensure we're loaded via 'zend_extension=' and not 'extension='
    if(!kago_zend_init) {
        zend_error(E_WARNING, "Kago must be loaded as a Zend Extension! Use 'zend_extension=kago.so', NOT 'extension=kago.so'");
    }

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

    // first table
    php_info_print_table_start();
    php_info_print_table_row(2, "Version", KAGO_VERSION);
    php_info_print_table_row(2, "Kago support", (KAGO_G(enabled) ? "Enabled" : "Disabled"));
    php_info_print_table_row(2, "Protection", (kago_active ? "Active" : "Inactive"));

    if(!kago_zend_init) {
        php_info_print_table_header(1, "Kago must be loaded as a Zend Extension! Use 'zend_extension=kago.so', NOT 'extension=kago.so'");
    }

    php_info_print_table_end();
    DISPLAY_INI_ENTRIES();

}

PHP_RINIT_FUNCTION(kago) {

    // setup function overrides
    replace_function("fopen", zif_kago_fopen_precall_hook);
    replace_function("file_put_contents", zif_kago_fopen_precall_hook);

}

/**
 * Exported Functions
 */

PHP_FUNCTION(kago_version) {
    RETURN_STRING(KAGO_VERSION, 1);
}

PHP_FUNCTION(kago_show_func) {
    char *fname, *fname2;
    int fname_len, fname2_len;
    char *param_fname;
    zend_function *fe;

    // parse params
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &fname, &fname_len) == FAILURE) {
        RETURN_FALSE;
    }

    param_fname = estrdup(fname);

    #pragma GCC diagnostic ignored "-Wformat"
    #pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

    // find existing function in function_table
    if(zend_hash_find(EG(function_table), param_fname, fname_len+1, (void*)&fe) == FAILURE) {
        efree(param_fname);
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s() not found", fname);
        RETURN_FALSE;
    }

    zend_printf("** got function %s\n", fname);
    zend_printf("fe->type: %hu\n", fe->type);
    zend_printf("fe->common.function_name: %s\n", fe->common.function_name);
    zend_printf("fe->common.scope: %08x\n", fe->common.scope);
    zend_printf("fe->common.fn_flags: %08x\n", fe->common.fn_flags);
    zend_printf("fe->common.prototype: %08x\n", fe->common.prototype);
    zend_printf("fe->common.num_args: %d\n", fe->common.num_args);
    zend_printf("fe->common.required_num_args: %d\n", fe->common.required_num_args);
    zend_printf("fe->common.arg_info @@ 0x%08x\n", fe->common.arg_info);
    zend_printf("fe->op_array: %08x\n", fe->op_array);

    if(fe->type == ZEND_INTERNAL_FUNCTION) {
        zend_internal_function *zif = (void*)fe;
        zend_printf("zif->handler: %08x\n", zif->handler);
    }

    zend_printf("zif_kago_hook_func @@ 0x%08x\n", zif_kago_zend_precall_hook);

    efree(param_fname);

    RETURN_TRUE;
}

PHP_FUNCTION(kago_fopen_precall_hook) {
    char *tfunc;
    void (*fptr)(INTERNAL_FUNCTION_PARAMETERS);
    char *realpath;

    char *filename, *mode;
    int filename_len, mode_len;
    zend_bool use_include_path = 0;
    zval *zcontext = NULL;
    php_stream *stream;
    php_stream_context *context = NULL;
    zval *data;
    long flags = 0;

    zend_printf("*** inside kago_fopen_precall_hook()\n");

    char *tfuncname = estrdup(KAGO_CALLED_FUNCTION);

    if((fptr = kago_fovr_get(tfuncname)) == NULL) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "no pointer for %s() found!", tfuncname);
        efree(tfuncname);
        RETURN_FALSE;
    }

    // scoop up them params
    if(!strcmp("fopen", tfuncname)) {
        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ps|br", &filename, &filename_len, &mode, &mode_len, &use_include_path, &zcontext) != FAILURE) {
            if((realpath = emalloc(KAGO_MAXPATH)) == NULL) {
                zend_printf("failed to allocate memory\n");
                return;
            }
            VCWD_REALPATH(filename, realpath);
            zend_printf("filename=[%s] realpath=[%s] mode=[%s] use_include_path=[%s]\n", filename, (realpath ? realpath : "??"), mode, (use_include_path ? "yes" : "no"));
            efree(realpath);
        }
    } else if(!strcmp("file_put_contents", tfuncname)) {
        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "pz/|lr!", &filename, &filename_len, &data, &flags, &zcontext) != FAILURE) {
            if((realpath = emalloc(KAGO_MAXPATH)) == NULL) {
                zend_printf("failed to allocate memory\n");
                return;
            }
            VCWD_REALPATH(filename, realpath);
            zend_printf("filename=[%s] realpath=[%s] data_length=[%d] flags=[0x%08x]\n", filename, (realpath ? realpath : "??"), Z_STRLEN_P(data), flags);
            efree(realpath);
        }
    }

    zend_printf("*** calling original function: %s() @@ 0x%08x\n", tfuncname, fptr);

    fptr(INTERNAL_FUNCTION_PARAM_PASSTHRU);

    zend_printf("*** leaving function %s()\n", tfuncname);

    efree(tfuncname);
    return;
}

PHP_FUNCTION(kago_zend_precall_hook) {
    zval **params;
    int params_len;
    char *tfunc;
    void (*fptr)(INTERNAL_FUNCTION_PARAMETERS);

    zend_printf("*** inside kago_zend_precall_hook()\n");

    char *tfuncname = estrdup(KAGO_CALLED_FUNCTION);

    if((fptr = kago_fovr_get(tfuncname)) == NULL) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "no pointer for %s() found!", tfuncname);
        efree(tfuncname);
        RETURN_FALSE;
    }

    zend_printf("*** callee sent %d params\n", ZEND_NUM_ARGS());
    zend_printf("*** calling original function: %s() @@ 0x%08x\n", tfuncname, fptr);

    fptr(INTERNAL_FUNCTION_PARAM_PASSTHRU);

    zend_printf("*** leaving function %s()\n", tfuncname);

    efree(tfuncname);
    return;
}

int replace_function(char *fname, void *fptr TSRMLS_DC) {
    zend_internal_function *orig_func = NULL;

    //zend_printf("hooking function '%s'\n", fname);

    // find existing function in function_table
    if(zend_hash_find(EG(function_table), fname, strlen(fname)+1, (void*)&orig_func) == FAILURE) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s() not found", fname);
        //efree(param_fname);
        return FAILURE;
    }

    // might need to use zend_hash_str_find_ptr() with PHP7
    // orig_func = zend_hash_str_find_ptr(EG(function_table), param_fname, strlen(fname) - 1);

    if(orig_func->type != ZEND_INTERNAL_FUNCTION) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s() is not a supported candidate for replacement", fname);
        return FAILURE;
    }

    // point handler to new function, return orig_func, and add old handler to overrides table
    kago_fovr_add(fname, orig_func->handler);
    orig_func->handler = fptr;

    return SUCCESS;
}

int kago_fovr_add(char *funcname, void *fptr) {
    func_overrides_len++;

    if((func_overrides = erealloc(func_overrides, sizeof(kago_overfuncs*) * func_overrides_len)) == NULL) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "erealloc() failed to reallocate memory");
        return FAILURE;
    }

    if((func_overrides[func_overrides_len - 1] = emalloc(sizeof(kago_overfuncs))) == NULL) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "malloc() failed to allocate memory for new entry");
        return FAILURE;
    }

    func_overrides[func_overrides_len - 1]->funcname = estrdup(funcname);
    func_overrides[func_overrides_len - 1]->fptr = fptr;

    return SUCCESS;
}

void kago_fovr_free() {
    for(int i = 0; i < func_overrides_len; i++) {
        if(func_overrides[i]->funcname) efree(func_overrides[i]->funcname);
        efree(func_overrides[i]);
    }
    efree(func_overrides);
}

void* kago_fovr_get(char *funcname) {
    for(int i = 0; i < func_overrides_len; i++) {
        if(!strcmp(funcname, func_overrides[i]->funcname)) {
            return func_overrides[i]->fptr;
        }
    }
    return NULL;
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
    if(func_overrides) {
        kago_fovr_free();
    }
}

ZEND_DLEXPORT void kago_zend_activate() {
    /* nothing here */
}

ZEND_DLEXPORT void kago_zend_deactivate() {
    /* nothing here */
}

ZEND_DLEXPORT void kago_fcall_begin_handler(zend_op_array *op_array) {
    /* nothing here */
}

ZEND_DLEXPORT void kago_fcall_end_handler(zend_op_array *op_array) {
    /* nothing here */
}

/**
 * Zend extension init & properties
 */

#ifndef ZEND_EXT_API
#define ZEND_EXT_API ZEND_DLEXPORT
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

    kago_zend_activate, /* activate_func_t activate */
    kago_zend_deactivate, /* deactivate_func_t deactivate */
    NULL, /* message_handler_func_t message_handler */
    NULL, /* op_array_handler_func_t op_array_handler */
    NULL, /* statement_handler_func_t statement_handler */
    NULL, /* fcall_begin_handler_func_t fcall_begin_handler */
    NULL, /* fcall_end_handler_func_t fcall_end_handler */
    NULL, /* op_array_ctor_func_t op_array_ctor */
    NULL, /* op_array_dtor_func_t op_array_dtor */
    STANDARD_ZEND_EXTENSION_PROPERTIES
};
