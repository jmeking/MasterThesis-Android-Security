
<?php
$date = date("Y-m-d H:i:s");        // 2001-03-10 17:16:18 (the MySQL DATETIME format)
echo $date, "\r\n";
echo "<br>";
echo "End Server receiving encrypted JSON";
echo "<br>";
echo "via HTTPS(SSL)";
echo "<br>";

$ctext = file_get_contents('php://input');
//$ctext = "ZGioFzoApFk9CfV9XFQhxQ==";
//$ctext = "BvndAJh0d3OEPhX0udz/xw==";
//$ctext = "loJRbUB3/2mdNhMmrQgrnBYV/UlNwyS+C837H2TgOuQ=";

$key = "abcdefghijklmnop";
//$key = "abcdefghijklmnopabcdefghijklmnop";

$res = null;

function decrypt_data($data, $iv, $key) {
	
    //$cypher = mcrypt_module_open('rijndael-256', '', 'ecb', '');
    $cypher = mcrypt_module_open('rijndael-128', '', 'ecb', '');

    if(is_null($iv)) {
        $iv = mcrypt_create_iv(mcrypt_enc_get_iv_size($cypher), MCRYPT_RAND);
    }

    // initialize encryption handle
    if (mcrypt_generic_init($cypher, $key, $iv) != -1) {
            // decrypt
            $decrypted = mdecrypt_generic($cypher, $data);

            // clean up
            mcrypt_generic_deinit($cypher);
            mcrypt_module_close($cypher);

            return $decrypted;

    }
    
    return false;
}
if($ctext != NULL){
	$time = microtime(true); 
	$res = decrypt_data(base64_decode($ctext), null, $key);
	echo (microtime(true) - $time) . ' elapsed';

}
else{
	echo "No data received \n";
}


echo $ctext;
echo "\n";
echo $res;

if($ctext != NULL){
	$res = str_replace("*", "", $res);
	$resjson = json_decode($res, true);

	include ("connect.php");

	@mysql_select_db("$DBName") or die("Unable to select
	database $DBName");

	$date     = mysql_real_escape_string($date);
	$ctext     = mysql_real_escape_string($ctext);
	$res = mysql_real_escape_string($res);

	$query    = "INSERT INTO $table (DATE, CTEXT, CLEAR, TEMP) 
	             VALUES('$date', '$ctext', '$res', '$resjson[temp]')";
	mysql_query($query) or trigger_error(mysql_error()." in ".$query);
	mysql_close();
}

?>