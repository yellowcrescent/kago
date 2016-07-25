dnl config.m4 for kago

PHP_ARG_ENABLE(kago, whether to enable Kago support,
[  --enable-kago          Enable Kago support])

PHP_ARG_ENABLE(debug, whether to enable debugging symbols,
[  --enable-debug          Enable debug support], no, no)

dnl check if we should enable Kago
if test "$PHP_KAGO" != "no"; then
    PHP_NEW_EXTENSION(kago, kago.c logger.c ruleparser.c, $ext_shared)
fi

dnl check if we should enable debugging
if test "$PHP_DEBUG" != "no"; then
	AC_DEFINE(USE_DEBUG, 1, [Include debugging support in Kago])
	CFLAGS="-ggdb3 -O0"
fi

dnl check for libyaml header
AC_MSG_CHECKING([whether libyaml headers are available])
for i in /usr/include /usr/local/include /opt/include; do
	if test -f $i/yaml.h; then
		YAML_INCLUDE=$i
	fi
done

if test -z "$YAML_INCLUDE"; then
	AC_MSG_RESULT([no])
	AC_MSG_ERROR([libyaml headers not installed])
else
	AC_MSG_RESULT([yes])
fi

dnl check for libyaml library
AC_MSG_CHECKING([whether libyaml library is available])
for i in /usr/lib /usr/local/lib /lib /lib64 /usr/lib/x86* /lib/x86* /usr/local/lib/x86*; do
	if test -f $i/libyaml.so; then
		YAML_LIBDIR=$i
	fi
done

if test -z "$YAML_LIBDIR"; then
	AC_MSG_RESULT([no])
	AC_MSG_ERROR([libyaml library could not be located])
else
	AC_MSG_RESULT([yes])
fi

dnl set up YAML header and linker includes
PHP_ADD_LIBRARY_WITH_PATH(yaml, $YAML_LIBDIR, YAML_SHARED_LIBADD)
PHP_ADD_INCLUDE($YAML_INCLUDE)

dnl use colorgcc, if available, so we can see errors/warnings in the output easier
AC_MSG_CHECKING([whether colorgcc is available])
if test $(which colorgcc) != ""; then
	CC="colorgcc"
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

CFLAGS="$CFLAGS -std=c99"
