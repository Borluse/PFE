<?php
	echo "Welcome : ";
	$file = $HTTP_POST_FILES['xml']['name'];
	
	$dir = "cgi-bin/";
	$path = $dir.$file;
	
	if(copy($HTTP_POST_FILES['xml']['tmp_name'], $path)){ //IF IT HAS BEEN COPIED... 

        //GET FILE NAME 
        $theFileName = $HTTP_POST_FILES['xml']['name']; 

        //GET FILE SIZE 
        $theFileSize = $HTTP_POST_FILES['xml']['size']; 

        if ($theFileSize>999999){ //IF GREATER THAN 999KB, DISPLAY AS MB 
            $theDiv = $theFileSize / 1000000; 
            $theFileSize = round($theDiv, 1)." MB"; //round($WhatToRound, $DecimalPlaces) 
        } else { //OTHERWISE DISPLAY AS KB 
            $theDiv = $theFileSize / 1000; 
            $theFileSize = round($theDiv, 1)." KB"; //round($WhatToRound, $DecimalPlaces) 
        }
	}
	echo "recu : ".$theFileSize;
//	$cmd = "cgi-bin/SiftMatch temp.xml";
	passthru ($cmd);
	
?>