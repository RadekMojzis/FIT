#!/usr/bin/env python3
# Author: Radek Mojzis. xmojzi07, FIT VUT
# 5. April 2017
# --------------------------------------------------------
class Polynomial(list):
	"""
    class "polynomial"
    INITIALIZATION:
      self - object
      self.coef - list of coefficients
    FUNCTIONS:
       __init__(self, *argc, **kwords) - initialisation either by a list, by arguments or by keywords
       __str__(self) - Print the polynomial
       __add__(self, other) - Adding two polynomials, return new polynomial
       __pow__(self, exp) - Exponentiation of polynomial
       __eq__(self, other) - Checks if two polynomials are equal to each other
       derivate(self) - Returns the derivative of the polynomial
       at_value(self, *x) - "Solves" the polynomial for given x or returns difference between at_value(x1) and at_value(x2)
   """
#-----------------------------------------[Init]-----------------------------------------
	def __init__(self, *argc, **kwords):
		if not len(argc) and not len(kwords):
			self.coef = []
			return
		if kwords:
			# fills the list from index 0 to highest index given in keys, if a key is not defined, 0 is put in the list
			self.coef = [0 if not "x"+str(index) in kwords else kwords["x"+str(index)] for index in range(0, int(max(list(kwords))[1:])+1)]
			return
		if type(argc[0]) == type(list()):
			self.coef = argc[0]
		else:
			self.coef = list(argc)
		return

	def __str__(self):
		"""
			Conversion to strings
			ARGS:
				self - polynomial to print...
			RETURN:
				string representing the polynomial
		"""
		index = 0
		# creates list of strings from coeficients and leaves out x at 0 power and 0 multiples of any power of x
		out = [str(number) + "x^" + str(index) if index != 0 else str(number) for index, number in enumerate(self.coef) if number != 0]
		# for empty polynomial result is 0
		if len(out) == 0:
			return "0"
		# chops off excesive "1"
		for index, part in enumerate(out):
			if "x" in part:
				if(part.startswith("1x")):
					out[index] = out[index][1:]
				if(part.startswith("-1x")):
					out[index] = "-" + out[index][2:]
			if(part.endswith("^1")):
				out[index] = out[index][:-2]
		#add correct signs
		for index, part in enumerate(out):
			if not part:
				continue
			if part[0] == "-":
				out[index] = "- " + part[1:] + (" " if index != 0 else "")
			else:
				if index != len(out)-1:
					out[index] = "+ " + part + (" " if index != 0 else "")
				else:
					out[index] = part + (" " if index != 0 else "")
			
		return "".join(reversed(out))
			
			
	def __add__(self, other):
		"""
			Add two polynomials
			ARGS:
					self - first polynomial
					other - second polynomial
			RETURN:
					result - sum of the 2 polynomials as a new object
    """
		# iterate through the bigger list, element of the new polynom is the sum of either 0 and a member of a polynom or 2 members
		#											if index exists use it, if it doesnt, use 0							same thing here...																							just iterate long enough...
		return Polynomial([(self.coef[index] if index < len(self.coef) else 0) + (other.coef[index] if index < len(other.coef) else 0) for index in range(max([len(self.coef), len(other.coef)]))])
		
	def __pow__(self, exp):
		"""
			Rising a polynomial to the power of exp
			ARGS:
					self - this polynomial
					exp - exponent
			RETURN:
					result - self^exp
    """
		#classic sloppy recursive implementation of power algorithm i implemented the __mul__ so that i could use it here...
		if exp == 0:
			return Polynomial(x0 = 1)
		if exp == 1:
			return self
		if exp == 2:
			return self * self
		if exp > 2:
			return self * self**(exp-1)
		
	def __mul__(self, other):
		"""
			Multiply two polynomials
			ARGS:
					self - this polynomial
					other - other polynomial
			RETURN:
					Polynomial(result) - result of multiplication of 2 polynoms
		"""
		result = Polynomial()
		for power_a, a in enumerate(self.coef):
			for power_b, b in enumerate(other.coef):
				init_list = []
				# just creates a list of 0 and in the end there is the a*b coeficient
				init_list = [0 for i in range(power_a + power_b)]
				init_list.append(a*b)
				result = result + Polynomial(init_list) 
		return result
		
	def derivative(self):
		"""
			derivative of a polynom
			ARGS:
					self - polynomial to make derivative of
			RETURN:
					result - the requiered derivative
		"""
		result = Polynomial()
		for power, mul in enumerate(self.coef):
			init_list = []
			if power > 0:
				init_list = [0 for i in range(power-1)]
				init_list.append(power*mul)
				result = result + Polynomial(init_list)
		return result
#-----------------------------------------[AT_VALUE]-----------------------------------------
	def at_value(self, *x):
		"""
			Getting the value of polynom(x) by substituting in the x... or difference between polynom(x1) and polynom(x2)
			
			ARGS:
					self - polynomial
					*x - X for which to "solve"
			RETURN:
					result - value
		"""
		assert len(x) <= 2
		result = 0
		if len(x) == 1:
			for power, mul in enumerate(self.coef):
				result += x[0]**power * mul
			return result
		if len(x) == 2:
			return self.at_value(x[1]) - self.at_value(x[0])
		
		
	def __eq__(self, other):
		"""
			Determinate if two polynomials are equal
			ARGS:
					self - this polynom
					other - other polynom
			RETURN:
					result - True when equal, false otherwise
		"""
		return (str(self) == str(other))

def test():
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
		test()
		