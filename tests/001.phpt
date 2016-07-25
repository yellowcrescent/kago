--TEST--
Kago loaded as zend_extension
--INI--
html_errors=0
display_startup_errors=1
kago.enabled=1
--FILE--
<?php
$kv = kago_version();
echo "OK - $kv";
?>
--EXPECTF--
OK - %d.%d.%d%S