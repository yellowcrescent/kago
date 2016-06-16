dnl config.m4 for kago

PHP_ARG_ENABLE(kago, whether to enable Kago support,
[  --enable-kago          Enable Kago support])

if test "$PHP_MYEXT" != "no"; then
    PHP_NEW_EXTENSION(kago, kago.c, $ext_shared)
fi

