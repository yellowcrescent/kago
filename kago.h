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


// Module infos //
#define KAGO_NAME "Kago"
#define KAGO_VERSION "0.1.0-wip"
#define KAGO_AUTHOR "Jacob Hipps"
#define KAGO_URL "https://ycnrg.org/"
#define KAGO_COPYRIGHT "Copyright (c) 2016 J. Hipps/Neo-Retro Group, Inc."
#define KAGO_COPYRIGHT_SHORT "Copyright (c) 2016"

// Module globals //
ZEND_BEGIN_MODULE_GLOBALS(kago)
	zend_bool enabled;
	char* log_path;
ZEND_END_MODULE_GLOBALS(kago)

#ifdef ZTS
	#define KAGO_G(v) TSRMG(kago_globals_id, zend_kago_globals*, v)
#else
	#define KAGO_G(v) (kago_globals.v)
#endif

// Module framework function declarations //
PHP_MINIT_FUNCTION(kago);
PHP_MSHUTDOWN_FUNCTION(kago);
PHP_MINFO_FUNCTION(kago);

// Exported functions //
PHP_FUNCTION(kago_version);


extern zend_module_entry kago_module_entry;
#define phpext_kago_ptr &kago_module_entry

#endif
