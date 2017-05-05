#!/usr/bin/env python3

# ISJ, projekt 7.
# Radek Mojzis
# xmojzi07
#
# 1. dekorator
import math


class TooManyCallsError(Exception):
	"""
		custom exeption
	"""
	pass
	


#	that means basically this:
#	this is a function that returns a function that calls a function
#	outer function is for arguments
#	inner is the decorator that we want
#	and inner inner is the wrapper of the function we want to decorate

def limit_calls(max_calls = 2, error_message_tail =  'called too often'):
	"""
		decorator that restricts calling of functions to a certain number of calls
		
		ARGS:
			max_calls - Maximum amount of times a function can be called
		RETURN:
			error_message_tail - The error message we want to display
	"""
	def decorator(function):							# an actual decorator
		specific_error_message = "function \"" + function.__name__+"\" - " + error_message_tail
		
		def wrapper(*args,**kwargs):				# wrapper
			wrapper.calls +=1									# increment...
			if(wrapper.calls > max_calls):		# Check
				raise TooManyCallsError(specific_error_message)
			return function(*args,**kwargs)		# execute the decorated function
		wrapper.calls = 0										# adds a calls variable to the wrapper
		
		return wrapper
	return decorator
# test
"""
@limit_calls(1, 'that is too much')
def pyth(a,b):
    c = math.sqrt(a**2 + b ** 2)
    return c

print(pyth(3,4))
print(pyth(6,8))
"""


def ordered_merge(*args, selector = []):
	"""
			A generator function 
			ARGS:
				*args - variable number of iterable	objects.
				selector - specifies from which iterable object should the result be picked from
			YELD
				a member from args specified by selector
	"""
	if len(selector) == 0:
		return									# big fat nothing

	current_indexes = [0 for i in range(len(args))] # array that holds current indexes in each object

	for i in selector:
		yield args[i][current_indexes[i]]
		current_indexes[i] += 1

print(list(ordered_merge('abcde', [1, 2, 3], (3.0, 3.14, 3.141), range(11, 44, 11))))




class Log:
	"""
		A log class that creates a log file and works well with exeptions
		
		INITIALIZATION:
			self - object
			self.logs - log enteries
			self.out - output file
		FUNCTIONS:
		 __init__(self, file) - opens a file
		 __enter__(self)  - for with - as
		 __exit__(self, exc_type, exc_value, traceback) -prints the logs Executes upon exiting from with - as block, even after exeption
		 logging(self, log) - creates a new log
			 
	"""
	
	def __init__(self, file):
		"""
			not sure why i shuold put comments everywhere but the tests demand it :[
			ARGS:
				self - what to init
				file - input file...
		"""
		self.out = open(file, "w")
		self.logs = []					# a list of logs (strings)
	#for with-as
	def __enter__(self):
		"""
			really? they want me to comment even this :(	
		"""
		return self
	#for with-as, upon ending the block, writes into the file... 
	def __exit__(self, exc_type, exc_value, traceback):		#executes even in case of an exeption
		"""
		append log to logs...
		ARGS:
			self - this object
			exc_type - ...
			exc_value - ...
			traceback - in case of exeption
		"""
		# ok, this part is extremely complicated, prints beggin, new line, a bunch of logs, end, new line
		# and then.....
		self.out.write("Begin\n")
		for log in self.logs:
			self.out.write(log+"\n")
		self.out.write("End\n")
		#you will not belive this part....
		self.out.close()
		#closes the file...

	def logging(self, log):	#create a log entry
		"""
		append log to logs...
		ARGS:
			log - log to create
			self - this object...
		"""
		self.logs.append(log)
# test
"""
with Log('mylog.txt') as logfile:
    logfile.logging('Test1')
    logfile.logging('Test2')
    a = 1/0
    logfile.logging('Test3')
"""	
	


