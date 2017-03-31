#!/usr/bin/env python3
import sys
import getopt
import re

brflag = False
help = False
input_file = sys.stdin
output_file = sys.stdout
fmt_file = ""
current_html_tag = ""

def get_options():
	global fmt_file
	global input_file
	global output_file
	global brflag
	global help
	
	opts, args = getopt.getopt(sys.argv[1:], "", ["help", "input=", "output=", "format=", "br"])
	
	for option, value in opts:
		if option == "--help":
			help = True
		elif option in "--input":
			input_file = open(value, 'r')
		elif option == "--output":
			output_file = open(value, 'w')
		elif option == "--format":
			fmt_file = open(value, 'r')
		else:
			print("Unrecognized option :/")
			assert False
	
	if fmt_file == "":
		print("No format file given!")
		assert False
#----------------------------------------------------------------------------------------------#		

def normalise_regex(reg):
	reg = re.sub(r"\\", "\\\\", reg)
	reg = re.sub("\\ufeff", "", reg)
	reg = re.sub("\[", "\\\[", reg)
	reg = re.sub("\]", "\\\]", reg)
	reg = re.sub("\{", "\\\{", reg)
	reg = re.sub("\}", "\\\}", reg)
	
	reg = re.sub("\^", "\\\^", reg)
	reg = re.sub("\?", "\\\?", reg)
	
	if re.search("(?<!%)(?<=!)%[salLwWstn]", reg):
		reg = re.sub("(?<!%)!%[salLwWstn]", "[^" + re.search("(?<!%)(?<=!)%[salLwWstn]", reg).group(0) + "]", reg)
	
	if re.search("(?<!%)(?<=!).", reg):
		reg = re.sub("(?<!%)!.", "[^" + re.search("(?<!%)(?<=!).", reg).group(0) + "]", reg)
	
	reg = re.sub("%s", "\s", reg)
	reg = re.sub("%a", ".", reg)
	reg = re.sub("%d", "[0-9]", reg)
	reg = re.sub("%l", "[a-z]", reg)
	reg = re.sub("%L", "[A-Z]", reg)
	reg = re.sub("%w", "[a-zA-Z]", reg)
	reg = re.sub("%W", "[a-zA-Z0-9]", reg)
	reg = re.sub("%s", "\s", reg)
	reg = re.sub("%t", "\t", reg)
	reg = re.sub("%n", "\n", reg)
	
	reg = re.sub("%\.", "\.", reg)
	reg = re.sub("%\|", "\|", reg)
	reg = re.sub("%\!", "\!", reg)
	reg = re.sub("%\*", "\*", reg)
	reg = re.sub("%\+", "\+", reg)
	reg = re.sub("%\(", "\(", reg)
	reg = re.sub("%\)", "\)", reg)
	reg = re.sub("%%", "%", reg)
	
	reg = re.sub("\.", "", reg)
	reg = re.sub("\"", "\\\"", reg)
	return reg

# ------------------------------------------------------------------------ #
def normalise_format(fmt):
	fmt = re.sub("bold", "b", fmt)
	fmt = re.sub("italic", "i", fmt)
	fmt = re.sub("underline", "u", fmt)
	fmt = re.sub("teletype", "tt", fmt)
	fmt = re.sub("size:", "font size=", fmt)
	fmt = re.sub("color:", "font color=#", fmt)
	fmt = re.sub("\\n", "", fmt)
	
	fmt = re.split(",", fmt)
	return fmt

# ------------------------------------------------------------------------ #

def parse_fmt_file(fmt_file):
	parsed_file = [re.split("\t+", line) for line in fmt_file]
	parsed_fmt = [[normalise_regex(reg), normalise_format(fmt)] for reg, fmt in parsed_file]
	return parsed_fmt
	
# ------------------------------------------------------------------------ #

def replace_whatever_what_matched(what):
	return "<" + current_html_tag + ">" + what.group(0) + "<"+ re.split(" ", current_html_tag.strip())[0] +">"
	
def format_text(fmt, file_in):
	global current_html_tag
	line_separator = "\n"
	if brflag:
		line_separator = "<br />\n"
	file_str = line_separator.join(line.strip() for line in file_in.readlines())
	for what, how in fmt:
		if re.search(what, file_str):
			for how_exactly in how:
				current_html_tag = how_exactly
				file_str = re.sub(what, replace_whatever_what_matched , file_str)
	return file_str
	
	#+ ">" + what + "</" +  + ">"
	

def close_IO():
	try:
		fmt_file.close()
	except Exception:
	  pass
	try:
		input_file.close()
	except Exception:
	  pass
	try:
		output_file.close()
	except Exception:
	  pass

try:
	get_options()
except AssertionError:
	close_IO()
	exit(1)
except FileNotFoundError:
	print("Error, file not found!")
	close_IO()
	exit(1)
except Exception as e:
	print (e)
	close_IO()
	exit(1)

formated_text = format_text(parse_fmt_file(fmt_file), input_file)
output_file.write(formated_text + "<br />\n")
close_IO()
