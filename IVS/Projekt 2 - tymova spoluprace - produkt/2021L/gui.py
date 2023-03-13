"""
author:
    Jiří Hrabovský (xhrabo17@stud.fit.vutbr.cz)
file:
    gui.py
brief:
    file containing main function and graphical interface
"""
from PyQt5 import QtCore, QtWidgets
from styles import Palette
from mat_lib import MathLib as MLib
from help_text import HelpText
from re import split, search

ALT_COL_CNT = len(Palette.colors[0][0]) - 1


class HelpDialog(QtWidgets.QDialog):
    """Manages help dialog opening"""
    def __init__(self, language, parent=None):
        super(HelpDialog, self).__init__(parent)
        help_text = HelpText()
        self.setWindowTitle("Help")
        if language == "CZ":
            self.text = help_text.help_cz
        elif language == "EN":
            self.text = help_text.help_en
        self.label = QtWidgets.QLabel(self.text)
        self.layout = QtWidgets.QVBoxLayout()
        self.layout.addWidget(self.label)
        self.setLayout(self.layout)


class UiMainWindow(object):
    """Manages GUI of main window"""
    def __init__(self):
        """initializes variables that are to be used by main window"""
        self.showing_result = False
        self.expect_binary_operator = False
        self.floating_point = False
        self.showing_error = False
        self.index = 0
        self.central_widget = 0
        self.vertical_layout = 0
        self.history_layout = 0
        self.history = 0
        self.history1 = 0
        self.history2 = 0
        self.history3 = 0
        self.display = 0
        self.button_rows = []
        self.buttons = []
        self.help_dialog = 0
        self.trailing_menu = 0
        self.help = 0
        self.help_EN = ''
        self.help_CZ = ''
        self.menu = 0
        self.setup_ui(self)

    def next_color(self):
        """Changes to next set of colors"""
        if self.index == ALT_COL_CNT:
            self.index = 0
        else:
            self.index += 1
        Palette.change(self.index, self)

    def prev_color(self):
        """Changes to previous set of colors"""
        if self.index == 0:
            self.index = ALT_COL_CNT
        else:
            self.index -= 1
        Palette.change(self.index, self)

    def clear_display(self):
        """Clears display and resets state indicators"""
        self.display.setText("")
        self.showing_result = False
        self.expect_binary_operator = False
        self.floating_point = False
        self.showing_error = False

    def auto_correct(self):
        """Closes open parentheses by appending close parentheses to the end of expression"""
        open_bracket = 0
        for char in self.display.text():
            if char == '(':
                open_bracket += 1
            if char == ')':
                open_bracket -= 1
        fix = ''.join(')' for _ in range(open_bracket))
        self.display.setText(self.display.text() + fix)

    def open_help_EN(self):
        """Opens English help dialog"""
        self.help_dialog = HelpDialog("EN")
        self.help_dialog.setModal(True)
        self.help_dialog.exec()

    def open_help_CZ(self):
        """Opens Czech help dialog"""
        self.help_dialog = HelpDialog("CZ")
        self.help_dialog.setModal(True)
        self.help_dialog.exec()

    def setup_ui(self, main_window):
        """
        Setups gui elements and connects buttons to functions

        Args:
            main_window:   main window of application
        """
        main_window.setWindowTitle("Calculator")
        main_window.resize(300, 470)
        main_window.setMinimumSize(QtCore.QSize(300, 470))
        main_window.setMaximumSize(QtCore.QSize(300000, 470))
        main_window.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.central_widget = QtWidgets.QWidget(main_window)
        main_window.setCentralWidget(self.central_widget)

        self.vertical_layout = QtWidgets.QVBoxLayout(self.central_widget)
        self.vertical_layout.setContentsMargins(11, 11, 11, 11)
        self.vertical_layout.setSpacing(4)

        # help dialog
        self.trailing_menu = main_window.menuBar()
        self.menu = QtWidgets.QMenu()
        self.menu = self.trailing_menu.addMenu("&Options")

        self.help_EN = QtWidgets.QAction("Help(EN)", self.trailing_menu)
        self.help_EN.triggered.connect(self.open_help_EN)
        self.menu.addAction(self.help_EN)

        self.help_CZ = QtWidgets.QAction("Help(CZ)", self.trailing_menu)
        self.help_CZ.triggered.connect(self.open_help_CZ)
        self.menu.addAction(self.help_CZ)

        # history
        self.history = QtWidgets.QGroupBox(self.central_widget)
        self.history.setTitle("History:")
        self.history.setMaximumSize(QtCore.QSize(2000000, 120))
        self.vertical_layout.addWidget(self.history)

        self.history_layout = QtWidgets.QVBoxLayout(self.history)
        self.history_layout.setSpacing(2)

        self.history1 = QtWidgets.QLineEdit(self.central_widget)
        self.history1.setText("")
        self.history1.setReadOnly(True)
        self.history1.setAlignment(QtCore.Qt.AlignmentFlag.AlignRight | QtCore.Qt.AlignmentFlag.AlignTrailing |
                                   QtCore.Qt.AlignmentFlag.AlignVCenter)
        self.history_layout.addWidget(self.history1)

        self.history2 = QtWidgets.QLineEdit(self.central_widget)
        self.history2.setText("")
        self.history2.setReadOnly(True)
        self.history2.setAlignment(QtCore.Qt.AlignmentFlag.AlignRight | QtCore.Qt.AlignmentFlag.AlignTrailing |
                                   QtCore.Qt.AlignmentFlag.AlignVCenter)
        self.history_layout.addWidget(self.history2)

        self.history3 = QtWidgets.QLineEdit(self.central_widget)
        self.history3.setText("")
        self.history3.setReadOnly(True)
        self.history3.setAlignment(QtCore.Qt.AlignmentFlag.AlignRight | QtCore.Qt.AlignmentFlag.AlignTrailing |
                                   QtCore.Qt.AlignmentFlag.AlignVCenter)
        self.history_layout.addWidget(self.history3)

        # display
        self.display = QtWidgets.QLineEdit(self.central_widget)
        self.display.setText("")
        self.display.setReadOnly(True)
        self.display.setAlignment(QtCore.Qt.AlignmentFlag.AlignRight | QtCore.Qt.AlignmentFlag.AlignTrailing |
                                  QtCore.Qt.AlignmentFlag.AlignVCenter)
        self.display.setMinimumSize(QtCore.QSize(0, 40))
        self.vertical_layout.addWidget(self.display)

        # buttons:
        for i in range(0, 5):
            self.button_rows.append(QtWidgets.QHBoxLayout())
            for j in range(0, 5):
                self.buttons.append(QtWidgets.QPushButton(self.central_widget))
                self.buttons[i * 5 + j].setText("" + str(i * 5 + j))
                self.button_rows[i].addWidget(self.buttons[i * 5 + j])
            self.vertical_layout.addLayout(self.button_rows[i])

        self.buttons[0].setText("√")
        self.buttons[0].clicked.connect(self.sqrt)

        self.buttons[1].setText("ln")
        self.buttons[1].clicked.connect(self.ln)

        self.buttons[2].setText("!")
        self.buttons[2].clicked.connect(self.factorial)

        self.buttons[3].setText("(")
        self.buttons[3].clicked.connect(self.bracket_l)

        self.buttons[4].setText(")")
        self.buttons[4].clicked.connect(self.bracket_r)

        self.buttons[5].setText("7")
        self.buttons[5].clicked.connect(self.num7)

        self.buttons[6].setText("8")
        self.buttons[6].clicked.connect(self.num8)

        self.buttons[7].setText("9")
        self.buttons[7].clicked.connect(self.num9)

        self.buttons[8].setText("DEL")
        self.buttons[8].clicked.connect(self.delete)

        self.buttons[9].setText("C")
        self.buttons[9].clicked.connect(self.clear)

        self.buttons[10].setText("4")
        self.buttons[10].clicked.connect(self.num4)

        self.buttons[11].setText("5")
        self.buttons[11].clicked.connect(self.num5)

        self.buttons[12].setText("6")
        self.buttons[12].clicked.connect(self.num6)

        self.buttons[13].setText("+")
        self.buttons[13].clicked.connect(self.add)

        self.buttons[14].setText("*")
        self.buttons[14].clicked.connect(self.mul)

        self.buttons[15].setText("1")
        self.buttons[15].clicked.connect(self.num1)

        self.buttons[16].setText("2")
        self.buttons[16].clicked.connect(self.num2)

        self.buttons[17].setText("3")
        self.buttons[17].clicked.connect(self.num3)

        self.buttons[18].setText("-")
        self.buttons[18].clicked.connect(self.sub)

        self.buttons[19].setText("/")
        self.buttons[19].clicked.connect(self.div)

        self.buttons[20].setText("0")
        self.buttons[20].clicked.connect(self.num0)

        self.buttons[21].setText(".")
        self.buttons[21].clicked.connect(self.point)

        self.buttons[22].setText("=")
        self.buttons[22].clicked.connect(self.solve)

        self.buttons[23].setText("^")
        self.buttons[23].clicked.connect(self.pow)

        self.buttons[24].setText("%")
        self.buttons[24].clicked.connect(self.mod)

        Palette.change(self.index, self)   # default color set

        QtCore.QMetaObject.connectSlotsByName(main_window)

    def insert_binary_operator(self, operator):
        """
        Adds binary operator to display if it is possible

        Args:
            operator    binary operator that will be added to display
        """
        if self.showing_error:
            self.clear_display()
        if self.expect_binary_operator:
            self.display.setText(self.display.text() + operator)
            self.expect_binary_operator = False
            self.floating_point = False
            self.showing_result = False

    def insert_unary_operator(self, operator):
        """
        Adds unary operator to display if it is possible

        Args:
            operator    unary operator that will be added to display
        """
        if self.showing_error:
            self.clear_display()
        if self.showing_result:
            self.clear_display()
        if self.expect_binary_operator:
            return
        self.display.setText(self.display.text() + operator)

    def num(self, num):
        """
        Adds number to display if it is possible

        Args:
            num    number that will be added to display
        """
        if self.showing_error:
            self.clear_display()
        if self.showing_result:
            self.clear_display()
        self.display.setText(self.display.text() + num)
        self.expect_binary_operator = True

    # functions connected to their corresponding buttons:
    def sqrt(self):
        """Writes Square root on display"""
        self.insert_unary_operator("√")

    def ln(self):
        """Writes natural logarithm on display"""
        self.insert_unary_operator("ln(")

    def factorial(self):
        """Writes factorial '!' on display"""
        if self.showing_error:
            self.clear_display()
        if self.expect_binary_operator:
            self.display.setText(self.display.text() + "!")
            self.showing_result = False

    def bracket_l(self):
        """Writes left bracket '(' on display"""
        if self.showing_error:
            self.clear_display()
        if self.showing_result:
            self.clear_display()
        if self.expect_binary_operator:
            return
        self.display.setText(self.display.text() + "(")

    def bracket_r(self):
        """Writes right bracket ')' on display"""
        if self.showing_error:
            self.clear_display()
        open_bracket = 0
        for char in self.display.text():
            if char == '(':
                open_bracket += 1
            if char == ')':
                open_bracket -= 1
        if open_bracket < 1:
            return
        if self.expect_binary_operator:
            self.display.setText(self.display.text() + ")")

    def delete(self):
        """deletes last character from display"""
        if self.showing_error:
            self.clear_display()
        if len(self.display.text()) != 0:
            if self.display.text()[-1]:
                self.expect_binary_operator = False
            self.display.setText(self.display.text()[:-1])
            if len(self.display.text()) != 0:
                c = self.display.text()[-1]
                if c.isdigit() or c == '.' or c == '!':
                    self.expect_binary_operator = True

    def clear(self):
        """deletes all characters from display"""
        self.clear_display()

    def add(self):
        """Writes plus '+' sign to display"""
        if self.expect_binary_operator:
            self.insert_binary_operator("+")
        else:
            self.insert_unary_operator("+")

    def mul(self):
        """Writes multiply '*' sign to display"""
        self.insert_binary_operator("*")

    def sub(self):
        """Writes minus '-' sign to display"""
        if self.expect_binary_operator:
            self.insert_binary_operator("-")
        else:
            self.insert_unary_operator("-")

    def div(self):
        """Writes division '/' sign to display"""
        self.insert_binary_operator("/")

    def point(self):
        """Writes decimal point '.' to display"""
        if self.showing_error:
            self.clear_display()
        if self.floating_point:
            return
        if self.expect_binary_operator:
            self.display.setText(self.display.text() + ".")
            self.floating_point = True
            self.showing_result = False

    def solve(self):
        """Evaluates expression presented on display, and writes result on the same display"""
        if len(self.display.text()) == 0:
            return
        if self.showing_error:
            self.clear_display()
            return
        if self.showing_result:
            return
        try:
            self.auto_correct()
            result = MainWindow.eval(self.display.text())
            MainWindow.update_display(result)
            self.showing_result = True
        except Exception as e:
            error = type(e).__name__
            self.display.setText(error)
            self.showing_error = True

    def pow(self):
        """Writes power '^' sign to display"""
        self.insert_binary_operator("^")

    def mod(self):
        """Writes modulo '%' sign to display"""
        self.insert_binary_operator("%")

    def num0(self):
        """Writes '0' sign to display"""
        self.num("0")

    def num1(self):
        """Writes '1' sign to display"""
        self.num("1")

    def num2(self):
        """Writes '2' sign to display"""
        self.num("2")

    def num3(self):
        """Writes '3' sign to display"""
        self.num("3")

    def num4(self):
        """Writes '4' sign to display"""
        self.num("4")

    def num5(self):
        """Writes '5' sign to display"""
        self.num("5")

    def num6(self):
        """Writes '6' sign to display"""
        self.num("6")

    def num7(self):
        """Writes '7' sign to display"""
        self.num("7")

    def num8(self):
        """Writes '8' sign to display"""
        self.num("8")

    def num9(self):
        """Writes '9' sign to display"""
        self.num("9")


class MainWindow(QtWidgets.QMainWindow, UiMainWindow):
    """Manages and extends functionality of main window"""
    def __init__(self, parent=None):
        """calls its parent class initialization, and initializes regular expressions for binary operators"""
        QtWidgets.QMainWindow.__init__(self, parent=parent)
        self.binary_operators = {
            "+": r'(?<=([0-9])|!|\.)\+(?=[0-9]+|([\+\-ln√]+[0-9]+))',
            "-": r'(?<=([0-9])|!|\.)-(?=[0-9]+|([\+\-ln√]+[0-9]+))',
            "*": r'(?<=([0-9])|!|\.)\*(?=[0-9]+|([\+\-ln√]+[0-9]+))',
            "/": r'(?<=([0-9])|!|\.)/(?=[0-9]+|([\+\-ln√]+[0-9]+))',
            "%": r'(?<=([0-9])|!|\.)%(?=[0-9]+|([\+\-ln√]+[0-9]+))',
            "^": r'(?<=([0-9])|!|\.)\^(?=[0-9]+|([\+\-ln√]+[0-9]+))',
        }

    def keyPressEvent(self, e):
        """Sets functions to its keybindings"""
        pressed = e.key()

        if pressed == QtCore.Qt.Key.Key_N:
            self.next_color()
        elif pressed == QtCore.Qt.Key.Key_B:
            self.prev_color()
        elif pressed == QtCore.Qt.Key.Key_Backspace:
            self.delete()
        elif pressed == QtCore.Qt.Key.Key_Delete:
            self.clear()
        elif pressed == QtCore.Qt.Key.Key_Enter:
            self.solve()
        elif pressed == QtCore.Qt.Key.Key_Return:
            self.solve()
        elif pressed == QtCore.Qt.Key.Key_Equal:
            self.solve()
        elif pressed == QtCore.Qt.Key.Key_ParenLeft:
            self.bracket_l()
        elif pressed == QtCore.Qt.Key.Key_ParenRight:
            self.bracket_r()
        elif pressed == QtCore.Qt.Key.Key_Plus:
            self.add()
        elif pressed == QtCore.Qt.Key.Key_Minus:
            self.sub()
        elif pressed == QtCore.Qt.Key.Key_Period:
            self.point()
        elif pressed == QtCore.Qt.Key.Key_Comma:
            self.point()
        elif pressed == QtCore.Qt.Key.Key_Percent:
            self.mod()
        elif pressed == QtCore.Qt.Key.Key_Asterisk:
            self.mul()
        elif pressed == QtCore.Qt.Key.Key_Slash:
            self.div()
        elif pressed == QtCore.Qt.Key.Key_AsciiCircum:
            self.pow()
        elif pressed == QtCore.Qt.Key.Key_Exclam:
            self.factorial()
        elif pressed == QtCore.Qt.Key.Key_0:
            self.num0()
        elif pressed == QtCore.Qt.Key.Key_1:
            self.num1()
        elif pressed == QtCore.Qt.Key.Key_2:
            self.num2()
        elif pressed == QtCore.Qt.Key.Key_3:
            self.num3()
        elif pressed == QtCore.Qt.Key.Key_4:
            self.num4()
        elif pressed == QtCore.Qt.Key.Key_5:
            self.num5()
        elif pressed == QtCore.Qt.Key.Key_6:
            self.num6()
        elif pressed == QtCore.Qt.Key.Key_7:
            self.num7()
        elif pressed == QtCore.Qt.Key.Key_8:
            self.num8()
        elif pressed == QtCore.Qt.Key.Key_9:
            self.num9()

    @classmethod
    def convert_to_exp(cls, expr):
        """
        Converts expresion to exponencial form

        Args:
            expr:   expresion to be converted

        Returns:
            Expresion in exponencial form

        Examples:
            >>> cls.convert_to_exp("1000000000000000000")
            1.0e+18
        """
        maxDec = 16
        exponent = len(expr) - 1
        expr = expr[:maxDec + 1]
        for i in range(maxDec, 1, -1):
            if expr[i] != '0':
                break
            expr = expr[:i]
        return f"{expr[0]}.{expr[1:]}e+{exponent}"

    @classmethod
    def convert_from_exp(cls, expr):
        """
        Replaces expresion in exponencial form with number

        Args:
            expr:   expr containing exponencial form

        Returns:
            Expresion without exponencial form

        Examples:
            >>> cls.convert_from_exp("1.2e+5")
            120000
        """
        start, end = search(r"[0-9]\.?[0-9]*e\+[0-9]*", expr).span()
        exp_form = expr[start:end]
        base, exponent = exp_form.split("e+")
        base = base.replace('.', '')
        exponent = int(exponent) - len(base) + 1
        base = base + ''.join('0' for _ in range(exponent))
        return f"{expr[:start]}{base}{expr[end:]}"

    def update_display(self, result: str):
        """
        Updates display and history with new result

        Args:
            result:   new result to be displayed
        """
        if len(result) * 12 > self.size().width():
            result = MainWindow.convert_to_exp(result)
        self.history1.setText(self.history2.text())
        self.history2.setText(self.history3.text())
        self.history3.setText(self.display.text() + " = " + result)
        self.display.setText(result)

    def remove_brackets(self, expr):
        """
        Replaces the first bracket from left by its value

        Args:
            expr:   expresion containing brackets

        Returns:
            Value of expresion

        Examples:
            >>> self.remove_brackets("5+(30/15)")
            5+2
        """
        first_index = 0
        second_index = 0
        open_bracket = -1
        for i, c in enumerate(expr):
            if c == "(":
                if open_bracket == -1:
                    first_index = i
                open_bracket += 1
            if c == ")":
                if open_bracket == 0:
                    second_index = i
                    break
                open_bracket -= 1

        return expr[0:first_index] + self.eval(expr[first_index + 1:second_index]) + expr[second_index + 1:]

    def eval(self, expr: str):
        """
        Evaluates expresion

        Args:
            expr:   expresion to be evaluated

        Returns:
            Value of expresion

        Examples:
            >>> self.eval("2+5")
            7
        """
        if 'e' in expr:
            expr = MainWindow.convert_from_exp(expr)
        # remove brackets form expr
        while "(" in expr:
            expr = self.remove_brackets(expr)
        # remove binary operators from expr
        for operator, regex in self.binary_operators.items():
            if search(regex, expr) is not None:
                operands = split(regex, expr, maxsplit=1)
                return MLib.solve(f"{self.eval(operands[0])}{operator}{self.eval(operands[3])}")
        # remove unary operators from expr
        if expr[-1] == "!":
            if expr[0] == '-':
                raise ValueError
            operand = self.eval(expr[0:len(expr) - 1])
            if "." in operand:
                operand = operand.rstrip("0")
                if operand[-1] == ".":
                    operand = operand[:-1]
            return MLib.solve(f"{operand}{expr[-1]}")
        if expr[0] == "+":
            new_expr = self.eval(expr[1:])
            if new_expr[0] == "-":
                return new_expr[1:]
            return new_expr
        if expr[0] == "-":
            new_expr = self.eval(expr[1:])
            if new_expr[0] == "-":
                return new_expr[1:]
            return "-" + new_expr
        if expr[0:2] == "ln":
            return MLib.solve(f"{expr[0:2]}{self.eval(expr[2:])}")
        if expr[0] == "√":
            return MLib.solve(f"{expr[0]}{self.eval(expr[1:])}")
        # checking if result is number
        for i, char in enumerate(expr):
            if i == 0 and char == '-':
                continue
            if char == '.' or char.isdigit():
                continue
            raise SyntaxError
        # return result
        return expr


if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)
    MainWindow = MainWindow()
    MainWindow.show()
    sys.exit(app.exec_())
