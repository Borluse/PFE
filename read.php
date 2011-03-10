<?php
	echo "Welcome : ";
	$receive = $_POST["caract"];
	echo "recu : ";
	echo strlen($receive);
	// $myfile = 'temp.xml';
	// $fp = fopen($myfile,'w');
	// fwrite($fp, $receive);
	// fclose($fp);
	$cmd = "cgi-bin/SiftMatch temp.xml";
	passthru ($cmd);
	
?>