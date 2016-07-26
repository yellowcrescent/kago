#!/bin/bash
NO_INTERACTION=1 TEST_PHP_EXECUTABLE=/usr/bin/php /usr/bin/php -n run-tests.php $@ | \
	tee results.raw | \
	egrep '(PASS|FAIL)' | \
	perl -pe 's/^.*\r(PASS|FAIL) (.+)$/\1|\2/' | \
	tee results.lst

