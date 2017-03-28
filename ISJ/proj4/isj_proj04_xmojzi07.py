#!/usr/bin/env python3


def balanced_paren(parenstr):
	paren_stack = []
	check_list = {')' : '(', ']' : '[', '}' : '{', '>' : '<'}
	for c in parenstr:
		if c in check_list.values():
			paren_stack.append(c)
		if c in check_list:
			if not paren_stack:
				return False
			if check_list[c] != paren_stack.pop():
				return False

	if not paren_stack:
		return True
	return False
	
def test_balanced_paren():
	print(balanced_paren("12<4<[a]b>>5"))
	print(balanced_paren("123"))
	print(balanced_paren("'[{}]"))
	print(balanced_paren("{1<2(>3)}"))
	print(balanced_paren(">>>"))

test_balanced_paren()

