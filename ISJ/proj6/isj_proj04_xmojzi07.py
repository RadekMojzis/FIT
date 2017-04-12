#!/usr/bin/env python3

def first_nonrepeating(string):
	repeated = set()
	nonrepeated = list()
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
	res = a/b
	if not is_integer(res):assert False 
	return res  

operators = (int.__add__, int.__mul__, int.__sub__, div)

numbers = [6, 6, 5, 2]

def tstr(op):
	if(op == int.__add__):
		return "+"
	if(op == int.__mul__):
		return "*"
	if(op == int.__sub__):
		return "-"
	if(op == div):
		return "/"
	
result = set()

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
										op1str = tstr(op1)
										op2str = tstr(op2)
										op3str = tstr(op3)
										try:
											if(op3(op2(op1(a, b), c), d) == 36):
												result.add(str('(({0}{1}{2}){3}{4}){5}{6}'.format(a, op1str, b, op2str, c, op3str, d)))
										except Exception:
											pass
										try:	
											if(op3(op1(a, b), op2(c, d)) == 36):
												result.add(str('({0}{1}{2}){3}({4}{5}{6})'.format(a, op1str, b, op3str, c, op2str, d)))
										except Exception:
											pass
											
for res in result:
	print(res)