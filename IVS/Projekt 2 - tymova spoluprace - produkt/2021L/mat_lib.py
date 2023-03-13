"""
author:
    Samuel Kuchta (xkucht11@stud.fit.vutbr.cz)
file:
    mat_lib.py
brief:
    Library/Class for basic mathematical functions
"""
MAX_INPUT = 1024


def str_to_num(string: str):
    """
    Converts string value to either integer or string.

    Args:
        string:   A string argument.

    Returns:
        Either int or float holding the result.

    Examples:
        >>> str_to_num("2")
        2
        >>> str_to_num("5.5")
        5.5
    """
    if '.' in string:
        return float(string)
    else:
        return int(string)


class MathLib:
    """Class containing static methods for mathematical operations"""
    @staticmethod
    def add(first, second):
        """adds 2 arguments and returns the result"""
        return first + second

    @staticmethod
    def sub(first, second):
        """subtracts 2 arguments and returns the result"""
        return first - second

    @staticmethod
    def mul(first, second):
        """multiplies 2 arguments and returns the result"""
        return first * second

    @staticmethod
    def div(first, second):
        """divides 2 arguments and returns the result"""
        if second == 0:
            raise ZeroDivisionError()
        else:
            return first / second

    @staticmethod
    def mod(first, second):
        """performs modulo operation by dividing first argument by second, and returning the remainder"""
        if second == 0:
            raise ZeroDivisionError()
        else:
            return first % second

    @staticmethod
    def factorial(ceiling):
        """computes factorial '!' of ceiling argument and returns the result"""
        product = 1
        if type(ceiling) != int or ceiling < 0 or ceiling > MAX_INPUT:
            raise ValueError()
        elif ceiling <= 1:
            return product
        else:
            for multiplier in range(2, ceiling + 1):  # range doesn't count with the last element, so it has to be +1.
                product *= multiplier
            return product

    @staticmethod
    def pow(base, exp):
        """exponentiates base argument by the exp argument and returns the result"""
        if (type(exp) != int and base < 0) or exp > MAX_INPUT:  # can't have decimal power of negative number.
            raise ValueError()
        if exp == 0 or base == 1:
            return 1
        else:
            return round(base ** exp, 10)

    @staticmethod
    def root(base):
        """returns the second root of the base argument"""
        return MathLib.pow(base, 0.5)

    @staticmethod
    def ln(arg):
        """computes natural logarithm of arg and returns the  result"""
        if arg <= 0:
            raise ValueError()
        n = 100000000.0
        return round(n * ((arg ** (1 / n)) - 1), 10)

    @staticmethod
    def solve(expression: str):  # splits the string into expression and operands, and call the appropriate function.
        """
          # splits the expression into operator and operands, and call the appropriate function.

        Args:
            expression:   A string argument.

        Returns:
            String holding result of expression.

        Examples:
            >>> str_to_num("2")
            2
            >>> str_to_num("5.5")
            5.5
        """
        if '+' in expression:
            args = expression.split('+')
            first = str_to_num(args[0])
            second = str_to_num(args[1])
            return str(MathLib.add(first, second))
        elif '*' in expression:
            args = expression.split('*')
            first = str_to_num(args[0])
            second = str_to_num(args[1])
            return str(MathLib.mul(first, second))
        elif '/' in expression:
            args = expression.split('/')
            first = float(args[0])
            second = str_to_num(args[1])
            return str(MathLib.div(first, second))
        elif '%' in expression:
            args = expression.split('%')
            first = str_to_num(args[0])
            second = str_to_num(args[1])
            return str(MathLib.mod(first, second))
        elif '^' in expression:
            args = expression.split('^')
            base = str_to_num(args[0])
            exp = str_to_num(args[1])
            return str(MathLib.pow(base, exp))
        elif '!' in expression:
            args = expression.split('!')
            if args[1] != "":  # second operand mustn't exist
                raise SyntaxError()
            ceiling = str_to_num(args[0])
            return str(MathLib.factorial(ceiling))
        elif '√' in expression:
            args = expression.split('√')
            if args[0] != "":  # second operand mustn't exist
                raise SyntaxError()
            base = float(args[1])
            return str(MathLib.root(base))
        elif 'ln' in expression:
            args = expression.split('ln')
            if args[0] != "":  # second operand mustn't exist
                raise SyntaxError()
            arg = str_to_num(args[1])
            return str(MathLib.ln(arg))
        elif '-' in expression:  # has to be checked last, so there isn't problem with negative numbers.
            neg_first = False
            if expression[0] == '-':
                neg_first = True
                expression = expression[1:]
            if '--' in expression:  # num1 - negative num2 = num1 + num2
                args = expression.split('--')
                first = str_to_num(args[0])
                if neg_first is True:
                    first = -first
                second = str_to_num(args[1])
                return str(MathLib.add(first, second))
            else:
                args = expression.split('-')
                first = str_to_num(args[0])
                if neg_first is True:
                    first = -first
                second = str_to_num(args[1])
                return str(MathLib.sub(first, second))
        else:
            print("this should never happen, gui presented me with the wrong expression.\n")
            raise SyntaxError()
