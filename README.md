
Kago
==================

# Installation

## PECL

	pecl install channel://pecl.ycnrg.org/Kago

## From Source

	git clone https://git.ycnrg.org/scm/ykg/kago.git
	cd kago
	phpize
	./configure
	make
	sudo make install

With either installation methods, you'll need to add `zend_extension=kago.so` to your `php.ini` file,
or create an additional file in `conf.d` (if enabled for your PHP build).
