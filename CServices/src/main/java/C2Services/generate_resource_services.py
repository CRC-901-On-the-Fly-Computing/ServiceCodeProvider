#!/usr/bin/python3

import argparse

def generate_strings(service, resource):
	file_content = """package C2Services;\n
import C2Data.C2Image;
import C2Data.C2Params;
import C2Data.C2Resource;\n
import java.util.Map;
import java.util.Set;\n
public class C2Service_""" + service.lower() + "_" + resource.upper() + """ extends C2Service_""" + service.lower() + """{
\tprivate C2Resource mResource;
\tpublic C2Service_""" + service.lower() + "_" + resource.upper() + """() {
\t\tsuper();\n
\t\tmResource = new C2Resource(\"""" + resource.lower() + """\");
\t}\n
\tpublic C2Image processImage(C2Image sourceImage) {
\t\treturn super.processImage(mResource, sourceImage);
\t}
}
"""
	
	file_name = "C2Service_" + service.lower() + "_" + resource.upper() + ".java"
	
	return (file_name, file_content)

def store_class_file(data):
	f = open(data[0], "w")
	f.write(data[1])
	f.close()

def generate_conf(service, resource):
	file_content = """    
*****classconf content*****

"C2Services.C2Service_""" + service.lower() + "_" + resource.upper() + """ ": {
        "methods": {
          "$construct" :{},
          "setOptions" :{
            "params": [
              "C2Data.C2Params"]
            },
            "processImage": {
                "params": ["C2Data.C2Image"],
                "returntype": "C2Data.C2Image"
            }
        }
    }

*****executer-conf content******
C2Services.C2Service_""" + service.lower() + "_" + resource.upper()


	file_name = "classconf_C2Service_" + service.lower() + "_" + resource.upper() + ".txt"
	
	return (file_name, file_content)

def store_conf_file(data):
	f1 = open(data[0], "w")
	f1.write(data[1])
	f1.close()

def main():
	parser = argparse.ArgumentParser(description='Generate resource-pinned services.')
	parser.add_argument("-c", action='store_true', default=False, help="Generate CPU-pinned class.")
	parser.add_argument("-s", action='store_true', default=False, help="Generate SCPU-pinned class.")
	parser.add_argument("-g", action='store_true', default=False, help="Generate GPU-pinned class.")
	parser.add_argument("-f", action='store_true', default=False, help="Generate FPGA-pinned class.")
	parser.add_argument("-j", action='store_true', default=False, help="Generate JAVA-only service class.")
	parser.add_argument("--service", type=str, nargs=1, default="")
	args = parser.parse_args()

	if args.c == False and args.s == False and args.g == False and args.f == False and args.j == False:
		print("No target resource defined!")
		exit(1)
	
	if args.service == "":
		print("Service name not defined!")
		exit(1)

	print args.c
	
	if args.c == True:
		store_class_file(generate_strings(args.service[0], "cpu"))
		store_conf_file(generate_conf(args.service[0], "cpu"))
	if args.s == True:
		store_class_file(generate_strings(args.service[0], "scpu"))
		store_conf_file(generate_conf(args.service[0], "scpu"))
	if args.g == True:
		store_class_file(generate_strings(args.service[0], "gpu"))
		store_conf_file(generate_conf(args.service[0], "gpu"))
	if args.f == True:
		store_class_file(generate_strings(args.service[0], "fpga"))
		store_conf_file(generate_conf(args.service[0], "fpga"))
	if args.j == True:
		store_class_file(generate_strings(args.service[0], "java"))
		store_conf_file(generate_conf(args.service[0], "java"))

main()
