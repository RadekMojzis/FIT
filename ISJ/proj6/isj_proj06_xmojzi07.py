#!/usr/bin/env python3

def first_nonrepeating(string):
	"""
	  returns first non-repeating character in a string
		so for "Hey you, Henery, i didnt see you there"
		returns 's'
		for "helloh"
		returns 'e'
	"""
	# set of already repeeted characters
	repeated = set()
	# a fifo
	nonrepeated = list()
	# go through the string, if it is first repeat, put it in repeated and remove it from the nonrepeated
	for c in string:
		if c not in repeated:
			if c in nonrepeated:
				repeated.add(c)
				nonrepeated.remove(c)
			else:
				nonrepeated.append(c)
	if not nonrepeated:
		return None
	return nonrepeated[0]

def div(a, b):
	"""division"""
	return a/b
def add(a, b):
	"""adding"""
	return a+b
def sub(a, b):
	"""subtraction"""
	return a-b
def mul(a, b):
	"""multiplication"""
	return a*b

def tstr(op):
	"""changes the functon name into string..."""
	if(op == add):
		return "+"
	if(op == mul):
		return "*"
	if(op == sub):
		return "-"
	if(op == div):
		return "/"
	raise Exception('this only works for add, mul, sub and div :/')

def combine4(numbers, result):
	"""
		solves the arithmetic puzzle "operator search" 
		returns list of all possible solutions
		note:
			doesnt include all possible permutations of a solution
			solution is not necesarily contained only once
			
			returns only ((a + b) + c) + d
								or (a + b) + (c + d)
			since any expression with any priority of operators can
			be expressed by these two "templates"
	"""
	
	if len(numbers) != 4:
		raise Exception('the list of numbers must have length 4!')
		
	operators = (add, mul, sub, div)
	solution = set()
	# try to put every operator in every positon and all numbers in all positions and check for solution
	for op1 in operators:
		for op2 in operators:
			for op3 in operators:
				for i, a in enumerate(numbers):
					for j, b in enumerate(numbers):
						if j != i:
							for k, c in enumerate(numbers):
								if k != i and k != j:
									for l, d in enumerate(numbers):
										if l != i and l != j and l != k:
											try:
												if(op3(op2(op1(a, b), c), d) == result):
													op1str = tstr(op1)
													op2str = tstr(op2)
													op3str = tstr(op3)
													solution.add(str('(({0}{1}{2}){3}{4}){5}{6}'.format(a, op1str, b, op2str, c, op3str, d)))
											except Exception:
												pass
											try:	
												if(op3(op1(a, b), op2(c, d)) == result):
													op1str = tstr(op1)
													op2str = tstr(op2)
													op3str = tstr(op3)
													solution.add(str('({0}{1}{2}){3}({4}{5}{6})'.format(a, op1str, b, op3str, c, op2str, d)))
											except Exception:
												pass
	return list(solution);

	