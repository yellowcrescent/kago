--TEST--
Test kago_fopen_precall_hook() with fopen
--CREDITS--
Jacob Hipps jacob@ycnrg.org
--INI--
html_errors=0
kago.enabled=1
kago.restrict_php=0
kago.log_path=./kago.test.log
--FILE--
<?php
	$tstr = "0123456789abcdef";

	$tf = "testfile-kago_fopen_precall_hook"
	$f = fopen($tf,"wb");
	fwrite($f, $tstr);
	fclose($f);

	$f = fopen($tf,"r");
	$rstr = fread($f, 64);
	var_dump($rstr);
	fclose($f);

	unlink($tf);

?>
--EXPECTF--
string(16) "0123456789abcdef"