#!/bin/bash
REPORT_EXIT_STATUS=1 NO_INTERACTION=1 TEST_PHP_EXECUTABLE=/usr/bin/php /usr/bin/php -n run-tests.php $@
