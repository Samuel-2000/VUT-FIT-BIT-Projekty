from py_lib.error import print_err_and_exit
import sys


class ArgsChecker:
    def __init__(self):
        super().__init__()
        self.XML_source = ''
        self.Input_source = ''
        self.dbg = 0

    def check(self):
        self.dbg += 1
        length = len(sys.argv)
        if length not in [2, 3]:  # skontroluje počet argumentov.
            print_err_and_exit('Zadaný zlý počet argumentov.', 10)

        for argument in sys.argv[1:]:  # skontrolujte typy argumentov.
            self.dbg += 1
            if argument == '--help' or argument == '-h':
                print(
                    "Program načíta XML reprezentáciu programu a tento program s využitím vstupu podľa parametrov príkazového riadka")
                print(
                    "interpretuje a generuje výstup. Vstupná XML reprezentácia je napr. generovaná skriptom parse.php ")
                print("(ale nie nutne) zo zdrojového kódu v IPPcode22.")
                exit(0)
            
            elif argument.startswith('--source='):  # vstupný súbor s XML reprezentáciou zdrojového kódu
                self.XML_source = argument[9:]
            elif argument.startswith('--input='):  # súbor so vstupmi pre samotnú interpretáciu zadaného zdrojového kódu
                self.Input_source = argument[8:]
            else:
                print_err_and_exit(f'Bol zadaný chybný argument: {argument}', 10)

    def get_xml_path(self):
        self.dbg += 1
        if self.XML_source == '':
            return 'sys.stdin'
        else:
            return self.XML_source

    def get_input_path(self):
        self.dbg += 1
        return self.Input_source
