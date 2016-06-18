<?php

function test_func() {
	print "This is a test func!\n";
}

print "Kago version: " . kago_version() . "\n";
kago_show_func("test_func");
kago_show_func("fopen");
kago_show_func("kago_version");

$f = fopen("kago.php", "r");
var_dump($f);
$fout = fread($f, 64);
print "got data from file:\n$fout\n\n";
if($f) fclose($f);
