#!/usr/bin/env python3

import sys
import getopt
import re
import bisect

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
			if len(opts) > 1:
				sys.stderr.write("Bad args!")
				close_IO()
				exit(1)
		if option == "--br":
			brflag = True
		elif option in "--input":
			input_file = open(value, 'r')
		elif option == "--output":
			try:
				output_file = open(value, 'w')
			except Exception as e:
				sys.stderr.write(str(e))
				close_IO()
				exit(3)
		elif option == "--format":
			fmt_file = open(value, 'r')
		else:
			sys.stderr.write("Unrecognized option!")
			assert False

	if fmt_file == "":
		sys.stderr.write("No format file given!")
		close_IO()
		exit(2)
		
#----------------------------------------------------------------------------------------------#		

def normalise_regex(reg):
	reg = re.sub("\.", "", reg)
	
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
	
	reg = re.sub("%\.", ".", reg)
	reg = re.sub("%\|", "\|", reg)
	reg = re.sub("%\!", "\!", reg)
	reg = re.sub("%\*", "\*", reg)
	reg = re.sub("%\+", "\+", reg)
	reg = re.sub("%\(", "\(", reg)
	reg = re.sub("%\)", "\)", reg)
	reg = re.sub("%%", "%", reg)
		
	return reg

# ------------------------------------------------------------------------ #
def normalise_format(fmt):
	fmt = re.sub("bold", "b", fmt)
	fmt = re.sub("italic", "i", fmt)
	fmt = re.sub("underline", "u", fmt)
	fmt = re.sub("teletype", "tt", fmt)
	fmt = re.sub("size:[1-7]", "font size=", fmt)
	fmt = re.sub("color:[0-6a-fA-F]{6}", "font color=#", fmt)
	
	if re.search("font size=", fmt) and re.search("font color=#", fmt):
		color = re.search("(?<=font color=#)[0-6a-fA-F]{6}", fmt).group(0)
		fmt = re.sub("font color=#[0-6A-F]{6}", "", fmt)
		fmt = re.sub("font size=", "font color=#" + color + " size=" ,fmt)
	
	fmt = re.split(",", fmt)
	fmt = filter(None, [member.strip() for member in fmt])
	fmt = [member for member in fmt]
	return fmt

# ------------------------------------------------------------------------ #

def parse_fmt_file(fmt_file):
	parsed_file = [re.split("\t+", line, 1) for line in fmt_file]
			
	try:
		parsed_fmt = [[normalise_regex(reg), normalise_format(fmt)] for reg, fmt in parsed_file]
	except Exception:
		sys.stderr.write("Bad syntax")
		close_IO()
		exit(4)
	for line in parsed_fmt:
		for fmt in line[1]:
			if re.search("color:", fmt):
				sys.stderr.write("Bad color")
				close_IO()
				exit(4)
			if re.search("size:", fmt):
				sys.stderr.write("Bad size")
				close_IO()
				exit(4)
			if fmt != "b" and fmt != "i" and fmt != "u" and fmt != "tt" and not re.search("font", fmt):
				sys.stderr.write("Unrecognised option: " + fmt)
				close_IO()
				exit(4)
				
	
	
	
	return parsed_fmt
	
# ------------------------------------------------------------------------ #
	
def format_text(fmt, file_in):
	global current_html_tag
	line_separator = "\n"
	if brflag:
		line_separator = "<br />\n"

	file_str = line_separator.join(line.strip() for line in file_in.readlines())
	list_of_tag_pairs = []
	for what, how in fmt:
		if what != "":
			if re.search(what, file_str):
				for how_exactly in how:
					for cosi in re.finditer(what, file_str):
						list_of_tag_pairs.append((list(cosi.span()), how_exactly))
	
	list_of_tags = []

	order = []
	for item in list_of_tag_pairs:
		tag = [item[0][0], "<" + item[1] + ">"]
		tag2 = [item[0][1], "</" + re.split(" ", item[1])[0] + ">"]
		
		tag_index = tag[0]
		tag2_index = tag2[0]
		
		index = bisect.bisect_right(order, tag_index)
		order.insert(index, tag_index)
		list_of_tags.insert(index, tag)
		
		index = bisect.bisect_left(order, tag2_index)
		order.insert(index, tag2_index)
		list_of_tags.insert(index, tag2)
		
	shift = 0
	for tag in list_of_tags:
		file_str = file_str[:tag[0] + shift] + tag[1] + file_str[tag[0] + shift:]
		shift += len(tag[1])
	
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
except getopt.GetoptError as e:
	sys.stderr.write(str(e))
	close_IO()
	exit(1)
except FileNotFoundError as e:
	sys.stderr.write(str(e))
	close_IO()
	exit(2)
except Exception as e:
	sys.stderr.write(str(e))
	close_IO()
	exit(100)

try:
	formated_text = format_text(parse_fmt_file(fmt_file), input_file)
except Exception as e:
	sys.stderr.write(str(e))
	close_IO()
	exit(100)

if(brflag):
	formated_text += "<br />\n"
output_file.write(formated_text )
close_IO()