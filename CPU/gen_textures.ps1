$OUTPUT_FILE = ".\2_gen_textures.ino"

$global_code = "";
$function_code = "";


$current_tex_name = "";
$current_tex_data = @();
$current_tex_w = 0;
$current_tex_h = 0;

foreach($line in Get-Content .\textures.txt) {
    
	$trimmed = $line.Trim();
	
	if ($current_tex_name -eq "" -and $trimmed -ne ""){
		$current_tex_name = $trimmed
	}else{
		
		if ($trimmed -eq ""){
			
			$global_code+="GTexture* tex_$current_tex_name;`n"
			
			$function_code+="tex_$current_tex_name = gpu_createTex($current_tex_w, $current_tex_h);`n"
			for ($i = 0; $i -lt $current_tex_data.Count; $i++){
				$function_code+="tex_$current_tex_name->Data[$i] = $current_tex_data[$i]"
			}
			
			
			$current_tex_name = "";
			$current_tex_data = @();
			$current_tex_w = 0;
			$current_tex_h = 0;
			continue;
		}
		
		$current_tex_h++;
		foreach ($char in $trimmed){
			if ($char -eq '.')
			{
				$current_tex_w++;
				$current_tex_data+= "false";
			}
			elseif ($char -eq 'x' -or $char -eq 'X')
			{
				$current_tex_w++;
				$current_tex_data+= "true";
			}
			
		}
		
		
	}
	
	
}

echo $global_code;
echo "void create_textures(){"
echo $function_code
echo "}";