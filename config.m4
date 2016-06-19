dnl config.m4 for kago

PHP_ARG_ENABLE(kago, whether to enable Kago support,
[  --enable-kago          Enable Kago support])

PHP_ARG_ENABLE(debug, whether to enable debugging symbols,
[  --enable-debug          Enable debug support], no, no)

dnl check if we should enable Kago
if test "$PHP_KAGO" != "no"; then
    PHP_NEW_EXTENSION(kago, kago.c, $ext_shared)
fi

dnl check if we should enable debugging
if test "$PHP_DEBUG" != "no"; then
	AC_DEFINE(USE_DEBUG, 1, [Include debugging support in Kago])
	CFLAGS="-ggdb3 -O0"
fi

dnl use colorgcc, if available, so we can see errors/warnings in the output easier
AC_MSG_CHECKING([whether colorgcc is available])
if test $(which colorgcc) != ""; then
	CC="colorgcc"
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

CFLAGS="$CFLAGS -std=c99"
