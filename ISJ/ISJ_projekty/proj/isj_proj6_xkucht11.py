#!/usr/bin/env python3

"""
author:
    Samuel Kuchta (xkucht11@stud.fit.vutbr.cz)
file:
    mat_lib.py
brief:
    Class for Polynomial operations.
"""


class Polynomial:
    """
    Class representing polynomial values
    """

    def __init__(self, *args, **kwargs):
        """
        Method for initialization of Polynomial instance. stores multiple values in list representing coefficients.
        Args:
            *args:   either list or arguments holding values.
            **kwargs: keyword arguments holding values
        Returns:
            object holding list of elements representing polynomial values.
        """
        self.coefficients = list()
        for i in args:
            if type(i) is list:
                self.coefficients = i  # coefficients as list
        if not self.coefficients:
            if args:
                self.coefficients = list(args)  # coefficients as arguments
            else:
                for order, val in kwargs.items():
                    for i in range(0, 1 + int(order.split("x")[1]) - len(self.coefficients)):  # fill with zeroes
                        self.coefficients.append(0)
                    self.coefficients[int(order.split("x")[1])] = val  # right value at right index
        for i in range(len(self.coefficients) - 1, 0, -1):  # get rid of unwanted zeroes
            if self.coefficients[i] == 0:
                del self.coefficients[i]
            else:  # no more zeroes
                break

    def __str__(self):
        """
        Magic method for informal Polynomial printing (print(Polynomial()) does not represent the actual structure)
        Returns:
            Informal string representation of Polynomial instance
        """
        poly_str = ""
        if len(self.coefficients) == 1:
            poly_str = poly_str + str(self.coefficients[0])
            return poly_str  # only absolute value
        for i in reversed(range(len(self.coefficients))):
            val = self.coefficients[i]
            if val == 0:  # zero coefficient
                continue  # skip

            if poly_str:  # not first character, therefore need to add sign
                if val > 0:
                    poly_str += " + "
                else:
                    poly_str += " - "

            val = abs(val)
            if i == 0:  # last coefficient
                poly_str += f"{val}"
                return poly_str

            if i == 1:  # penultimate koeficient
                if val == 1:
                    poly_str += "x"
                else:
                    poly_str += f"{val}x"

            else:  # order of 2 and more
                if val == 1:
                    poly_str += f"x^{i}"
                else:
                    poly_str += f"{val}x^{i}"

        return poly_str

    def __eq__(self, other):
        """
        Magic method for comparing 2 Polynomial instances.
        Args:
            other:   second Polynomial instance for comparison
        Returns:
            True if same, False otherwise
        """
        if len(self.coefficients) != len(other.coefficients): # different lengths, they cannot be the same
            return False # this condition is necessary because the zip function ignores the rest of the list

        for s, o in zip(self.coefficients, other.coefficients):
            if s != o: # if a single term is different, the polynomials are different
                return False

        return True

    def __add__(self, other):
        """
        Magic method for adding 2 Polynomial instances.
        Args:
            other:   second Polynomial instance for addition
        Returns:
            Polynomial instance of added values
        """
        tmp = self.coefficients[:]  # a temporary list without a reference to be passed as a result
        for index, (tmp_coefficients, o_coefficients) in enumerate(zip(self.coefficients, other.coefficients)):
            tmp[index] = tmp_coefficients + o_coefficients  # iteration through coefficients of the same power
        if len(self.coefficients) < len(other.coefficients):  # first argument is shorter, result must be adjusted
            for i in range(len(self.coefficients), len(other.coefficients)):
                tmp.append(other.coefficients[i])
        return Polynomial(tmp)

    def derivative(self):
        """
        Method for computing derivative of Polynomial instance.
        Returns:
            derived polynomial instance
        """
        if len(self.coefficients) == 1: # the derivative of the constant is 0
            return 0

        tmp = self.coefficients[:]  # a temporary list without a reference to be passed as a result
        for order in range(len(tmp)):
            tmp[order] = tmp[order] * order
        del tmp[0]  # deletes the last character, therefore it moves the indexes, and also reduces the power by 1
        return Polynomial(tmp)

    def at_value(self, x1, x2="undefined"):
        """
        Method for computing value of x of polynomial instance using Horner's method.
        Args:
            x1:   mandatory x-value to compute
            x2:   optional x-value to subtract from x1
        Returns:
            derived polynomial instance
        """
        res = self.coefficients[-1]
        for i in reversed(range(len(self.coefficients) - 1)):
            res = (res * x1) + self.coefficients[i]  # use of Horner's scheme to eliminate multiple exponentiation

        if x2 == "undefined":
            return res

        res2 = self.coefficients[-1]
        for i in reversed(range(len(self.coefficients) - 1)):
            res2 = (res2 * x2) + self.coefficients[i]
        return res2 - res

    def __mul__(self, other):
        """
        Magic method for multiplying 2 Polynomial instances
        Args:
            other:   second Polynomial instance for multiplying
        Returns:
            Polynomial instance of multiplied values
        """
        tmp = [0] * (len(self.coefficients) + len(other.coefficients) + 1)
        for i in range(len(self.coefficients)):
            for j in range(len(other.coefficients)):
                tmp[i + j] += self.coefficients[i] * other.coefficients[j]
        return Polynomial(tmp)

    def __pow__(self, n):
        """
        Magic method for exponentiating Polynomial instance to the power of n
        Args:
            n: power
        Returns:
            exponentiated Polynomial instance
        """
        if n == 0:  # the zero power of the polynomial is always 1
            if len(self.coefficients) == 1 and self.coefficients[0] == 0:
                # Výraz 0^0 není definován
                raise ValueError("Undefined: 0^0")
            else:
                return 1

        if n == 1:  # the first power of the polynomial is the original polynomial
            return Polynomial(self.coefficients)

        if n < 0:
            raise ValueError("Negative power is not allowed.")

        if n > 1:
            tmp = self
            for i in range(1, n):
                tmp *= self
            return Polynomial(tmp)


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