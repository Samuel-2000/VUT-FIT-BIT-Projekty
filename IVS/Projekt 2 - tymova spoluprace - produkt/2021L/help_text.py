"""
author:
    Jiří Hrabovský (xhrabo17@stud.fit.vutbr.cz)
file:
    help_text.py
brief:
    file containing text for help dialog
"""


class HelpText:
    """Class for initializing text used in help dialog"""
    def __init__(self):
        self.help_en = """
The calculator can be operated using the mouse or keyboard.
All functions of the numeric part of the keyboard can be used, including operations.
The input is displayed on the screen above the function keys and the result appears on the screen after evaluation.
The operation entered and the result will also appear in the History section.

To clear the screen, press the "C" button.
To delete the last character shown on the display, press "DEL".
To calculate the Nth root of x, enter "x^(1/n)".
To evaluate the operations, press "=".
The calculator supports decimal numbers, which can be entered with the "." character (both "." and "," can be used on the keyboard).
To enter an X from the list "! ( ) + - * / ^ %" via the English keyboard, use SHIFT+X.
The square root and logarithm can only be entered using the keys.

Basic mathematical operations:
Addition\t\t+
Subtraction\t-
Multiplication\t*
Division\t\t/
Modulo\t\t% (remainder after division)

Advanced mathematical operations:
Factorial\t\t!
Logarithm\tln (natural)
Nth power\txⁿ
Nth root\t\tⁿ√ (only the square root via the button)

Priority of operations:
1) Priority as in algebra
2) Parentheses ()

You can minimize the application by pressing the "-" sign in the upper right corner of the application window.

To close the application, press the "×" sign in the upper right corner of the application window.

The color themes of the calculator can be changed by using the "N" (next) and "B" (back) keys. There are a total of 21 themes to choose from."""

        self.help_cz = """
Kalkulačku je možné ovládat pomocí myši nebo klávesnici.
Lze použít všechny funkce numerické části klávesnice včetně operací.
Vstup se zobrazí na obrazovce nad funkčními klávesy a po vyhodnocení se na ní objeví výsledek.
Zadaná operace i s výsledkem se rovněž objeví v sekci History.

Pro vyčištění obrazovky stiskněte tlačítko "C".
Pro smazání posledního znaku zobrazeného na displeji stiskněte "DEL".
Pro výpočet N-tej odmocniny z x zadejte "x^(1/n)".
Pro vyhodnocení operací stiskněte "=".
Kalkulačka podporuje desetinná čísla, které lze zadat znakem "." (na klávesnici lze zadat "." i ",").
Pro zadání znaků X ze seznamu "! ( ) + - * / ^ %" přes anglickou klávesnici použijte SHIFT+X.
Druhou odmocninu a logaritmus lze zadat pouze pomocí tlačítek.

Základní matematické operace:
Sčítání\t\t+
Odčítání\t\t-
Násobení\t*
Dělení\t\t/
Modulo\t\t% (zbytek po dělení)

Pokročilé matematické operace:
Faktoriál\t\t!
Logaritmus\tln (přirozený)
N-tá mocnina\txⁿ
N-tá odmocnina\tⁿ√ (tlačítkem pouze druhá)

Priorita operací:
1) Priorita jako v algebře
2) Závorky ()

Minimalizovat aplikaci lze stisknutím znaku "-" v pravém horním rohu okna aplikace.

Vypnout aplikaci lze stisknutím znaku "×" v pravém horním rohu okna aplikace.

Barevné motívy kalkulačky se dají měnit použitím kláves "N" (dopředu) a "B" (nazpět). Na výběr je spolu 21 motivů."""
