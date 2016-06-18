
Kago
==================

# Installation

	git clone https://git.ycnrg.org/scm/ykg/kago.git
	cd kago
	phpize
	./configure
	make
	sudo make install

Then add `zend_extension=kago.so` to your `php.ini` file, a file included in `conf.d` (if enabled for your PHP build).

