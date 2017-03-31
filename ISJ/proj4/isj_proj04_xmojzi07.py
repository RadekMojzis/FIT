#!/usr/bin/env python3

# returns true when all parentheses are correct, false otherwise
def balanced_paren(parenstr):
	paren_stack = []
	check_list = {')' : '(', ']' : '[', '}' : '{', '>' : '<'}
	for c in parenstr:
		# when you find an opening bracket, push it on the stack
		if c in check_list.values():
			paren_stack.append(c)
		if c in check_list:
			# if you find a closing bracket and stack is empty - false
			if not paren_stack:
				return False
			# check if the parentheses type is the same as the one on the stack
			if check_list[c] != paren_stack.pop():
				return False
	# only return true if the stack is empty
	if not paren_stack:
		return True
	return False

#returns a string containing the encoded message
def caesar_list(word, key = [1, 2, 3]):
	if not word.islower():
		raise ValueError('caesar_list() takes only lowercase strings!')
	
	i = 0
	newword = ""
	for char in word:
		# ord(char) - ord("a") - a nmuber of a letter [0-25], shifted by appropriet key, then modulo to make sure
		# it is still a character and finally shifted back to wherever "a" is
		character = chr((ord(char) - ord("a") + key[i % len(key)])%26 + ord("a"))
		newword += character
		i = i +1;
	return newword

# same as caesar_list only uses args instead of key, so nothing really changed
def caesar_varnumkey(word, *args):
	if len(args) == 0:
		args = [1, 2, 3]
	if not word.islower():
		raise ValueError('caesar_list() takes only lowercase strings!')
	
	i = 0
	newword = ""
	for char in word:
		character = chr((ord(char) - ord("a") + args[i % len(args)])%26 + ord("a"))
		newword += character
		i = i +1;
	return newword

