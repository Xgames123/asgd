import sys
import os
OUTPUT_FILE = "2_gen_textures.ino"

global_code = ""
function_code =""

current_tex_name=""
current_tex_data=[]
current_tex_w=0
current_tex_h=0


if not os.getcwd().endswith("CPU"):
    print("you are in the wrong directory. Go to CPU")
    sys.exit()


def write_texture_code(name, w, h, data):
    global function_code
    global global_code
    
    global_code+=f"GTexture* tex_{name};\n"
    global_code+=f"GTexture* Tex_{name}()"+"{"+f"return tex_{name};"+"}"
    
    function_code+=f"tex_{name} = gpu_createTex({w}, {h});\n"
    
    data = current_tex_data
    
    for i in range(len(data)):
        function_code+=f"tex_{name}->Data[{i}] = {data[i]};\n"
    

def valid_texname(line):
    if line.startswith("#"):
        return False
    
    if line == "":
        return False
    
    if "." in line:
        return False
    
    non_x_char = False
    for char in line:
        if char != "x" and char != ".":
            non_x_char = True
    
    if not non_x_char:
        return False
    
    return True

with open("textures.txt") as file:
    for line in file:
        trimmed = line.strip()
        
        if current_tex_name == "":
            if valid_texname(trimmed):
                print(trimmed)
                current_tex_name = trimmed
            
        else:
            if trimmed == "":
                write_texture_code(current_tex_name, current_tex_w, current_tex_h, current_tex_data)
                current_tex_name=""
                current_tex_data=[]
                current_tex_w=0
                current_tex_h=0
                continue
            
            current_tex_h+=1
            current_tex_w = 0
            for char in trimmed:
                current_tex_w+=1
                if char == '.':
                    current_tex_data.append("false")
                else:
                    current_tex_data.append("true")
                    


if current_tex_name != "":
    write_texture_code(current_tex_name, current_tex_w, current_tex_h, current_tex_data)



code = "//This code is auto generated by gen_textures.py\n"
code += "//use textures.txt to create and edit textures\n"
code += global_code
code += "void create_textures(){"
code += function_code
code += "};"

file = open(OUTPUT_FILE, "w")
file.write(code)
file.close()

print("done")