#!/usr/bin/env python3


def balanced_paren(parenstr):
	parenstack = []
	for c in parenstr:
		if c == "{" or c == "(" or c == "<" or c == "[":
		  parenstack.append(c)
		if c == "}":
		  if parenstack.pop() != "{":
			  return False
		if c == ")":
		  if parenstack.pop() != "(":
			  return False
		if c == ">":
		  if parenstack.pop() != "<":
			  return False
		if c == "]":
		  if parenstack.pop() != "[":
			  return False
	
	if not parenstack:
	  return True
	return False

if balanced_paren("12<4<[a]b>>5"):
  print("true")
else:
  print("false")
	
if balanced_paren("123"):
  print("true")
else:
  print("false")
	
if balanced_paren("'[{}]"):
  print("true")
else:
  print("false")

if balanced_paren("{1<2(>3)}"):
  print("true")
else:
  print("false")