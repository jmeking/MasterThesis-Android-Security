<?php
$today = date("Y-m-d H:i:s");        // 2001-03-10 17:16:18 (the MySQL DATETIME format)
echo $today, "\r\n";

echo "<br>";
echo "Gateway receives encrypted JSON";
echo "<br>";
echo "Forwards to end server via HTTPS(SSL)";
echo "<br>";

global $data;
	$data = null;

function forwardJson(){
	$data = file_get_contents('php://input');
	//$data = '{"temp":"test","humd":"test"}'; //Test input
	//$data = '$\'\"Q0uª¸Øn‰Ù f[§'; //Test input
	$ch = curl_init('https://192.168.2.100/webserver/webservice.php');                                                                      
	curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST"); 
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); //Disable CA verification
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE); //Disable CA verification
	curl_setopt($ch, CURLOPT_POSTFIELDS, $data);                                                                  
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);                                                                      
	curl_setopt($ch, CURLOPT_HTTPHEADER, array(                                                                          
		'Content-Type: application/json',                                                                                
		'Content-Length: ' . strlen($data))                                                                       
	);

	$output = curl_exec($ch);
	if(curl_exec($ch) === false){
	    echo 'Curl error: ' . curl_error($ch);
	}
	else{
	    echo 'Operation completed without any errors';
	}
	echo "Sensor Data Received: ";
	var_dump($data);

	$fp = fopen('client.json', 'w');
	fwrite($fp, $data);
	fclose($fp);

	}
$time = microtime(true); 	
forwardJson();
echo (microtime(true) - $time) . ' elapsed';

?>