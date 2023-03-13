"""
author:
    Samuel Kuchta (xkucht11@stud.fit.vutbr.cz)
file:
    profiling.py
brief:
    Tests correctness of computation of MathLib
"""
import unittest as ut
from mat_lib import MathLib as Ml


class TestMathLib(ut.TestCase):
    """
    Does unit tests on our MathLib.

    Args:
        ut.TestCase:   class that our tests inherit from.
    """
    def test_add(self):
        """tests adding functionality"""
        self.assertEqual(Ml.add(2, 3), 5)
        self.assertEqual(Ml.add(0, 13), 13)
        self.assertEqual(Ml.add(-6, 7), 1)
        self.assertEqual(Ml.add(7.2, 1.4), 8.6)

    def test_sub(self):
        """tests subtracting functionality"""
        self.assertEqual(Ml.sub(4, 4), 0)
        self.assertEqual(Ml.sub(3, 4), -1)
        self.assertEqual(Ml.sub(6, -2), 8)
        self.assertAlmostEqual(Ml.sub(8.2, 3.3), 4.9)

    def test_mul(self):
        """tests multiplying functionality"""
        self.assertEqual(Ml.mul(3, 3), 9)
        self.assertEqual(Ml.mul(-5, 4), -20)
        self.assertEqual(Ml.mul(2, 0), 0)
        self.assertEqual(Ml.mul(2.7, 8), 21.6)

    def test_mod(self):
        """tests modulo '%' functionality"""
        self.assertEqual(Ml.mod(12, 4), 0)
        self.assertEqual(Ml.mod(8, 3), 2)
        self.assertEqual(Ml.mod(7.2, 2.5), 2.2)
        with self.assertRaises(ZeroDivisionError):
            Ml.mod(1, 0)

    def test_div(self):
        """tests division functionality"""
        self.assertEqual(Ml.div(12, 4), 3)
        self.assertAlmostEqual(Ml.div(8, 3), 2.666666666)
        self.assertEqual(Ml.div(7.2, 2.5), 2.88)
        with self.assertRaises(ZeroDivisionError):
            Ml.div(1, 0)

    def test_factorial(self):
        """tests factorial functionality"""
        self.assertEqual(Ml.factorial(5), 120)
        self.assertEqual(Ml.factorial(0), 1)
        self.assertEqual(Ml.factorial(1), 1)
        self.assertEqual(Ml.factorial(12), 479001600)
        with self.assertRaises(ValueError):
            Ml.factorial(-3)
            Ml.factorial(6.2)
            Ml.factorial(1025)

    def test_pow(self):
        """tests functionality of exponentiation"""
        self.assertEqual(Ml.pow(5, 0), 1)
        self.assertEqual(Ml.pow(5, 1), 5)
        self.assertEqual(Ml.pow(1, 3), 1)
        self.assertEqual(Ml.pow(-5, 4), 625)
        self.assertEqual(Ml.pow(4.2, 3), 74.088)
        with self.assertRaises(ValueError):
            Ml.pow(4, 3.2)  # why not?
            Ml.pow(4, -2)  # why not?
            Ml.pow(3, 1025)

    def test_root(self):
        """tests functionality of second root"""
        self.assertEqual(Ml.root(9), 3)
        self.assertEqual(Ml.root(7.2), 2.683281573)
        with self.assertRaises(ValueError):
            Ml.root(-4)
            Ml.root(-10)

    def test_ln(self):
        """tests functionality of natural logarithm"""
        self.assertAlmostEqual(Ml.ln(2.718281828), 1.0)
        self.assertAlmostEqual(Ml.ln(8), 2.0794415523)
        self.assertAlmostEqual(Ml.ln(137), 4.9199810448)
        with self.assertRaises(ValueError):
            Ml.ln(0)
            Ml.ln(-2)

    def test_solve(self):
        """tests functionality of solving binary expressions"""
        self.assertEqual(Ml.solve('7+3'), '10')
        self.assertEqual(Ml.solve('5-8'), '-3')
        self.assertEqual(Ml.solve('4*5'), '20')
        self.assertEqual(Ml.solve('9/3'), '3.0')
        self.assertEqual(Ml.solve('4%3'), '1')
        self.assertEqual(Ml.solve('5!'), '120')
        self.assertEqual(Ml.solve('2^3'), '8')
        self.assertEqual(Ml.solve('√9'), '3.0')
        self.assertEqual(Ml.solve('ln1'), '0.0')
        self.assertEqual(Ml.solve('4^-1'), '0.25')
        with self.assertRaises(ValueError):
            Ml.solve('-2!')
            Ml.solve('-4√(40')
            Ml.solve('ln(-4')
        with self.assertRaises(SyntaxError):
            Ml.solve('5ln(4')
            Ml.solve('4!2')
        with self.assertRaises(ZeroDivisionError):
            Ml.solve('5/0')
            Ml.solve('5%0')


if __name__ == '__main__':
    ut.main()
