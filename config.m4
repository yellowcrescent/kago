dnl config.m4 for kago

PHP_ARG_ENABLE(kago, whether to enable Kago support,
[  --enable-kago          Enable Kago support])

if test "$PHP_MYEXT" != "no"; then
    PHP_NEW_EXTENSION(kago, kago.c, $ext_shared)
fi

dnl use colorgcc, if available, so we can see errors/warnings in the output easier
AC_MSG_CHECKING([whether colorgcc is available])
if test $(which colorgcc) != ""; then
	CC=colorgcc
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi

CFLAGS="-std=c99"
