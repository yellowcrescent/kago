<?php

$dummy_file = "~dummy_file";

printf("Kago version: %s\n", kago_version());

printf("Writing data to test file: %s\n", $dummy_file);

file_put_contents($dummy_file, "This is test data");

$f = fopen($dummy_file, "r");
var_dump($f);
if($f) fclose($f);

printf("Removing dummy file\n");
unlink($dummy_file);
