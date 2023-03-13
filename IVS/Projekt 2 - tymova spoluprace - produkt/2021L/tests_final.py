"""
author:
    Dominik Augustín (xaugus14@stud.fit.vutbr.cz)
file:
    tests_final.py
brief:
    Various tests for testing the Calculator I/O.
"""

# NOTE Requires connection to a display. Will not work in WSL terminal

import unittest as ut
from PyQt5 import QtWidgets
import gui

STARTED = "\033[1;33m[STARTD]\033[0m"
PASSED = "\033[1;32m[PASSED]\033[0m"

app = QtWidgets.QApplication(["gui.py"])
tester = gui.MainWindow()

big_positive_int = "1267650600228229401496703205376"  # 2^100
big_positive_float = "999999999999999.9999999999"
small_positive_int = "1"
small_positive_float = "0.0000000001"
zero_int = "0"
zero_float = "000000000000000.0000000000"
smallNegativeFloat = "-0.0000000001"
small_negative_int = "-1"
big_negative_float = "-999999999999999.9999999999"
big_negative_int = "-1267650600228229401496703205376"  # - 2^100


def test_operation(num_in, out, operation, self):
    """Tests functions"""
    input = ""
    input_brackets = ""
    if (operation == "√"):
        input = f"√{num_in}"
        input_brackets = f"√({num_in})"
    elif (operation == "ln"):
        input = f"ln{num_in}"
        input_brackets = f"ln({num_in})"
    elif (operation == "!"):
        input = f"{num_in}!"
        input_brackets = f"({num_in})!"
    print(f"input {input}")
    tester.display.setText(input)
    tester.update_display(tester.eval(tester.display.text()))
    self.assertAlmostEqual(float(out), float(tester.display.text()), delta=0.0001)
    tester.display.setText(input_brackets)
    tester.update_display(tester.eval(tester.display.text()))
    self.assertAlmostEqual(float(out), float(tester.display.text()), delta=0.0001)
    print("\tOK")


def test_operation_two(num_in, num_in2, out, operation, self):
    """Tests binary operators"""
    input = ""
    input_brackets = ""
    if (operation == "+"):
        input = f"{num_in}+{num_in2}"
        input_brackets = f"({num_in})+({num_in2})"
    elif (operation == "-"):
        input = f"{num_in}-{num_in2}"
        input_brackets = f"({num_in})-({num_in2})"
    elif (operation == "*"):
        input = f"{num_in}*{num_in2}"
        input_brackets = f"({num_in})*({num_in2})"
    elif (operation == "/"):
        input = f"{num_in}/{num_in2}"
        input_brackets = f"({num_in})/({num_in2})"
    elif (operation == "^"):
        input = f"{num_in}^{num_in2}"
        input_brackets = f"({num_in})^({num_in2})"
    elif (operation == "%"):
        input = f"{num_in}%{num_in2}"
        input_brackets = f"({num_in})%({num_in2})"
    print(f"input {input}")
    tester.display.setText(input)
    tester.update_display(tester.eval(tester.display.text()))
    self.assertAlmostEqual(float(out), float(tester.display.text()), delta=0.0001)
    tester.display.setText(input_brackets)
    tester.update_display(tester.eval(tester.display.text()))
    self.assertAlmostEqual(float(out), float(tester.display.text()), delta=0.0001)
    print("\tOK")


def test_errors(num_in, num_in2, expected_error, operation, self):
    """Tests exceptions"""
    input = ""
    input_brackets = ""
    if (operation == "√"):
        input = f"√{num_in}"
        input_brackets = f"√({num_in})"
    elif (operation == "ln"):
        input = f"ln{num_in}"
        input_brackets = f"ln({num_in})"
    elif (operation == "!"):
        input = f"{num_in}!"
        input_brackets = f"({num_in})!"
    elif (operation == "+"):
        input = f"{num_in}+{num_in2}"
        input_brackets = f"({num_in})+({num_in2})"
    elif (operation == "-"):
        input = f"{num_in}-{num_in2}"
        input_brackets = f"({num_in})-({num_in2})"
    elif (operation == "*"):
        input = f"{num_in}*{num_in2}"
        input_brackets = f"({num_in})*({num_in2})"
    elif (operation == "/"):
        input = f"{num_in}/{num_in2}"
        input_brackets = f"({num_in})/({num_in2})"
    elif (operation == "^"):
        input = f"{num_in}^{num_in2}"
        input_brackets = f"({num_in})^({num_in2})"
    elif (operation == "%"):
        input = f"{num_in}%{num_in2}"
        input_brackets = f"({num_in})%({num_in2})"
    print(f"input {input}")
    tester.display.setText(input)
    self.assertRaises(expected_error, tester.eval, str(tester.display.text()))
    tester.display.setText(input_brackets)
    self.assertRaises(expected_error, tester.eval, str(tester.display.text()))
    print("\tOK")


class TestMathLibFinal(ut.TestCase):
    def test_sqrt(self):
        """Tests square root"""
        print(f"\n\n\t\tsquare root {STARTED}")
        test_arr_in = [big_positive_int, big_positive_float, small_positive_int, small_positive_float, zero_int,
                       zero_float, "1024", "10000", "50", "2022", "0.5"]
        test_arr_out = ["1125899906842624.0", "31622776.6016837933", "1.0", "0.00001", "0.0", "0.0", "32.0", "100.0",
                        "7.0710678119", "44.9666543118", "0.7071067812"]
        for num_in, num_out in zip(test_arr_in, test_arr_out):
            test_operation(num_in, num_out, "√", self)

        test_arr = [smallNegativeFloat, small_negative_int, big_negative_float, big_negative_int]
        for num in test_arr:
            test_errors(num, 0, ValueError, "√", self)
        print(f"\n\t\t{PASSED}\n")

    def test_ln(self):
        """Tests natural logarithm function"""
        print(f"\n\n\t\tnatural logarithm {STARTED}")
        test_arr_in = [big_positive_int, big_positive_float, small_positive_int, small_positive_float, "1024", "10000",
                       "50", "2022", "0.5"]
        test_arr_out = ["69.314718056", "34.5387763949", "0.0", "-23.0258509299", "6.9314718056", "9.2103403720",
                        "3.9120230054", "7.6118423996", "-0.6931471806"]
        for num_in, num_out in zip(test_arr_in, test_arr_out):
            print(num_in)
            test_operation(num_in, num_out, "ln", self)

        test_arr = [zero_int, zero_float, smallNegativeFloat, small_negative_int, big_negative_float, big_negative_int]
        for num in test_arr:
            test_errors(num, 0, ValueError, "ln", self)
        print(f"\n\t\t{PASSED}\n")

    def test_factorial(self):
        """Tests factorial '!' function"""
        print(f"\n\n\t\tfactorial {STARTED}")
        test_arr_in = [small_positive_int, zero_int, "1024", "100", "50", "20", "432", "23"]
        test_arr_out = ["1", "1", "5.418528796058857e+2639", "9.332621544394415e+157", "3.0414093201713376e+64",
                        "2432902008176640000", "4.272460196051823e+952", "25852016738884976640000"]
        for num_in, num_out in zip(test_arr_in, test_arr_out):
            test_operation(num_in, num_out, "!", self)

        test_arr = [smallNegativeFloat, big_negative_float, big_negative_int, small_positive_float, big_positive_float,
                    big_positive_int, "1025", "1026", small_negative_int]
        for num in test_arr:
            test_errors(num, 0, ValueError or SyntaxError, "!", self)
        print(f"\n\t\t{PASSED}\n")

    def test_add(self):
        """Tests addition"""
        print(f"\n\n\t\tadd {STARTED}")
        test_arr_in_first = [big_positive_int, big_positive_float, small_positive_float, smallNegativeFloat,
                             small_negative_int, "23.4", "0.1", "5.8", "0", small_negative_int]
        test_arr_in_second = [big_positive_int, big_negative_int, small_positive_int, small_positive_float,
                              small_positive_float, "2022", "0.1", "0", "0.0", small_negative_int]
        test_arr_out = ["2.535301200456459e+30", "-1.2676506002282284e+30", "1.0000000001", "0.0", "-0.9999999999",
                        "2045.4", "0.2", "5.8", "0.0", "-2.0"]
        for num_in_frst, num_in_scnd, num_out in zip(test_arr_in_first, test_arr_in_second, test_arr_out):
            test_operation_two(num_in_frst, num_in_scnd, num_out, "+", self)
        print(f"\n\t\t{PASSED}\n")

    def test_multiply(self):
        """Tests multiplication"""
        print(f"\n\n\t\tmultiply {STARTED}")
        test_arr_in_first = [big_positive_int, big_positive_float, small_positive_float, smallNegativeFloat,
                             small_negative_int, "23.4", "0.1", "5.8", "0", "-5"]
        test_arr_in_second = [big_positive_int, big_negative_int, small_positive_int, small_positive_float,
                              small_positive_float, "2022", "0.1", "0", "0.0", "-4"]
        test_arr_out = ["1.6069380442589903e+60", "-1.2676506002282294e+45", small_positive_float,
                        "0.00000000000000000001", smallNegativeFloat, "47314.8", "0.01", "0.0", "0.0", "20.0"]
        for num_in_frst, num_in_scnd, num_out in zip(test_arr_in_first, test_arr_in_second, test_arr_out):
            test_operation_two(num_in_frst, num_in_scnd, num_out, "*", self)
        print(f"\n\t\t{PASSED}\n")

    def test_subtract(self):
        """Tests subtraction"""
        print(f"\n\n\t\tsubtract {STARTED}")
        test_arr_in_first = [big_positive_int, big_positive_float, small_positive_float, smallNegativeFloat,
                             small_negative_int, "23.4", "0.1", "5.8", "0", "-5"]
        test_arr_in_second = [big_positive_int, big_negative_int, small_positive_int, small_positive_float,
                              small_positive_float, "2022", "0.1", "0", "0.0", "-4"]
        test_arr_out = ["0.0", "1.2676506002282305e+30", "-0.9999999999", "-0.0000000002", "-0.9999999999", "-1998.6",
                        "0.0", "5.8", "0.0", "-1.0"]
        for num_in_frst, num_in_scnd, num_out in zip(test_arr_in_first, test_arr_in_second, test_arr_out):
            test_operation_two(num_in_frst, num_in_scnd, num_out, "-", self)
        print(f"\n\t\t{PASSED}\n")

    def test_division(self):
        """Tests division"""
        print(f"\n\n\t\tdivision {STARTED}")
        test_arr_in_first = [big_positive_int, big_positive_float, small_positive_float, smallNegativeFloat,
                             small_negative_int, "23.4", "0.1", "0", "8", "-5"]
        test_arr_in_second = [big_positive_int, big_negative_int, small_positive_int, small_positive_float,
                              small_positive_float, "2022", "0.1", small_negative_int, "3", "-4"]
        test_arr_out = ["1.0", "-7.9e-16", small_positive_float, "-1", "-10000000000", "0.0115727003", "1.0", "0.0",
                        "2.666666666", "1.25"]
        for num_in_frst, num_in_scnd, num_out in zip(test_arr_in_first, test_arr_in_second, test_arr_out):
            test_operation_two(num_in_frst, num_in_scnd, num_out, "/", self)

        test_arr = [big_positive_int, big_positive_float, small_positive_int, small_positive_float, zero_int,
                    zero_float, smallNegativeFloat, small_negative_int, big_negative_float, big_negative_int]
        for num in test_arr:
            test_errors(num, "0", ZeroDivisionError, "/", self)
        print(f"\n\t\t{PASSED}\n")

    def test_power(self):
        """Tests exponentiation"""
        print(f"\n\n\t\tpower {STARTED}")
        test_arr_in_first = [big_positive_float, small_positive_float, smallNegativeFloat, "0.1", small_negative_int,
                             big_negative_float, big_negative_int, zero_float, "4", "2"]
        test_arr_in_second = [big_negative_int, small_positive_int, small_positive_int, "0.1", small_negative_int,
                              zero_int, zero_int, small_positive_float, small_negative_int, "1024"]
        test_arr_out = ["0.0", small_positive_float, smallNegativeFloat, "0.7943282347", "-1.0", "1.0", "1.0", "0.0",
                        "0.25", "1.797693134862316e+308"]
        for num_in_frst, num_in_scnd, num_out in zip(test_arr_in_first, test_arr_in_second, test_arr_out):
            test_operation_two(num_in_frst, num_in_scnd, num_out, "^", self)

        test_arr = [big_positive_int, big_positive_float, "1025", "1026"]
        for num in test_arr:
            test_errors("2", num, ValueError, "^", self)
        test_errors(small_negative_int, "5.2", ValueError, "^", self)
        test_errors(zero_int, small_negative_int, ZeroDivisionError, "^", self)
        test_errors(zero_float, small_negative_int, ZeroDivisionError, "^", self)
        print(f"\n\t\t{PASSED}\n")

    def test_modulo(self):
        """Tests modulo '%'"""
        print(f"\n\n\t\tmodulo {STARTED}")
        test_arr_in_first = [big_positive_int, big_positive_float, small_positive_float, smallNegativeFloat,
                             small_negative_int, "23.4", "0.1", "0", "13"]
        test_arr_in_second = [big_positive_int, big_negative_int, small_positive_int, small_positive_float,
                              small_positive_float, "2022", "0.1", small_negative_int, "5"]
        test_arr_out = ["0.0", "-1.2676506002282284e+30", small_positive_float, "0.0", "0.0", "23.4", "0.0", "0.0",
                        "3.0"]
        for num_in_frst, num_in_scnd, num_out in zip(test_arr_in_first, test_arr_in_second, test_arr_out):
            test_operation_two(num_in_frst, num_in_scnd, num_out, "%", self)

        test_arr = [big_positive_int, big_positive_float, small_positive_int, small_positive_float, zero_int,
                    zero_float, smallNegativeFloat, small_negative_int, big_negative_float, big_negative_int]
        for num in test_arr:
            test_errors(num, "0", ZeroDivisionError, "%", self)
        print(f"\n\t\t{PASSED}\n")

    def test_error_inputs(self):  # invalid brackets
        """Tests invalid inputs"""
        print(f"\n\n\t\terror inputs {STARTED}")
        test_arr = ["5+", "2.1*", "10-", "23/", "30^", "134.2%"]
        for expr in test_arr:
            print(f"{expr}")
            self.assertRaises(SyntaxError, tester.eval, f"{expr}")
            self.assertRaises(SyntaxError, tester.eval, f"({expr})")
            print(f"\tOK")
        self.assertRaises(IndexError, tester.eval, "8(")

        print(f"\n\t\t{PASSED}\n")

    def test_advanced(self):  # combinations of operators
        """Advanced tests"""
        print(f"\n\n\t\tadvanced {STARTED}")
        test_arr_in = ["5--3", "12.2*3-5.3", "8^2*2", "(√16)!", "ln(1)-2*4.8", "13%5*(9-2)"]
        test_arr_out = ["8", "31.3", "128", "24", "-9.6", "21"]
        for num_in, num_out in zip(test_arr_in, test_arr_out):
            print(f"{num_in}")
            self.assertAlmostEqual(float(tester.eval(f"{num_in}")), float(f"{num_out}"))
            self.assertAlmostEqual(float(tester.eval(f"({num_in})")), float(f"{num_out}"))
            print(f"\tOK")

        self.assertAlmostEqual(float(tester.eval("5*ln(4)")), float("6.9314718056"), delta=0.00001)

        print(f"\n\t\t{PASSED}\n")


if __name__ == '__main__':
    ut.main()
