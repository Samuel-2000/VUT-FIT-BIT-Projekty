from py_lib.error import print_err_and_exit
from py_lib.inst import InstCls
from py_lib.inst_list import InstListCls

import xml.etree.ElementTree as elmTree
import sys


class XmlParser:
    def __init__(self, xml_path):
        super().__init__()
        self.xml_path = xml_path
        self.dbg = 0

    def check(self):
        self.check_tree_integrity()
        self.check_root()
        self.check_instructions()

    def check_tree_integrity(self):
        self.dbg += 1
        try:
            if self.xml_path == 'sys.stdin':
                myTree = elmTree.parse(sys.stdin)
            else:
                myTree = elmTree.parse(self.xml_path)
        except FileNotFoundError:
            print_err_and_exit(f'Vyskytla sa chyba pri otváraní súboru {self.xml_path}', 11)
        except Exception:
            print_err_and_exit('XML format nie je v poradku.', 31)
        self.dbg += 1
        try:
            self.root = myTree.getroot()
        except:
            print_err_and_exit('Nebolo možné získať koreň XML súboru.', 31)

    def check_root(self):
        self.dbg += 1
        if self.root.tag != 'program':
            print_err_and_exit('XML kořen nemá názov program.', 32)
        for atribute in self.root.attrib:
            self.dbg += 1
            if atribute not in ['language', 'name', 'description']:
                print_err_and_exit('Nepovolený atribút pri koreňovom elemente XML.', 32)
        if 'language' not in self.root.attrib:
            print_err_and_exit('Chybí atribút language pri korenovom elemente XML.', 32)
        if self.root.attrib['language'].lower() != 'ippcode22':
            print_err_and_exit('Atribút language pri korenovom elemente musí byť ippcode22.', 32)
        self.dbg += 1

    def check_instructions(self):  # spojit s import instruction
        self.dbg += 1
        order_tmp = 0
        for instruction in self.root:
            arg_dict = {}
            if instruction.tag != 'instruction':
                print_err_and_exit(f'Nespravny nazov elementu instruction. názov "{instruction.tag}" je nespravny', 32)
            if 'order' not in instruction.attrib:
                print_err_and_exit('pri elemente inštrukcie chýba atribút order alebo opcode.', 32)
            if 'opcode' not in instruction.attrib:
                print_err_and_exit('pri elemente inštrukcie chýba atribút order alebo opcode.', 32)
            try:
                instr_number = int(instruction.attrib['order'])
            except ValueError:
                print_err_and_exit('Nebolo možné prečítať hodnotu typu int parametru order pri argumente inštrukcie.', 32)
            if instr_number <= order_tmp:
                print_err_and_exit('Ocislovanie inštrukcií musí byť kladné a rýdzo rastúce.', 32)
            order_tmp = instr_number
            arg_counter = 0
            shuffled = False
            for argument in instruction:
                arg_counter += 1
                if argument.tag[0:3] != 'arg' or argument.tag[3] not in ['1', '2', '3']:
                    print_err_and_exit(f'argument musí mať názov arg1-3. Názov "{argument.tag}" je nesprávný', 32)
                if argument.tag[3] != str(arg_counter):
                   shuffled = True
                if 'type' not in argument.attrib:
                    print_err_and_exit('Chýbajúci atribút type pri argumente inštrukcie.', 32)
                if argument.attrib['type'] not in ['string', 'int', 'bool', 'label', 'type', 'nil', 'var']:
                    print_err_and_exit('Chybný udaj atribútu type pri argumente inštrukcie.', 32)

            if shuffled is True:  # ak su argumenty poprehadzovane, tak ich prehodi na spravne miesto
                for argument in instruction:
                    arg_dict[argument.tag[3]] = (arg_counter, argument.attrib['type'], argument.text)

                correctly_ordered = []
                try:
                    for key in range(1, len(arg_dict)+1):
                        correctly_ordered.append(arg_dict[str(key)])
                except KeyError:
                    print_err_and_exit(f'Inštrukcia {instruction.attrib["opcode"]} má nesprávne zadané argumenty.', 32)

                for index, argument in enumerate(instruction):
                    argument.tag = argument.tag[0:3] + str(index+1)
                    argument.attrib['type'] = correctly_ordered[index][1]
                    argument.text = correctly_ordered[index][2]

    def import_instructions(self, inst_list: InstListCls):
        self.dbg += 1
        for instruction in self.root:
            self.dbg += 1
            inst_name = instruction.attrib['opcode'].upper()
            if inst_name in ['CREATEFRAME', 'PUSHFRAME', 'POPFRAME', 'BREAK', 'RETURN']:
                self.check_num_of_arg(instruction, 0)
                i = InstCls(inst_name)
                self.dbg += 1
            elif inst_name in ['DPRINT', 'DEFVAR', 'CALL', 'PUSHS', 'POPS', 'LABEL', 'JUMP', 'WRITE', 'EXIT']:
                self.check_num_of_arg(instruction, 1)
                i = InstCls(inst_name, instruction[0])
                self.dbg += 1
            elif inst_name in ['MOVE', 'INT2CHAR', 'READ', 'STRLEN', 'TYPE', 'NOT']:
                self.check_num_of_arg(instruction, 2)
                i = InstCls(inst_name, instruction[0], instruction[1])
                self.dbg += 1
            elif inst_name in ['ADD', 'SUB', 'MUL', 'IDIV', 'LT', 'GT', 'EQ', 'AND', 'OR', 'JUMPIFEQ', 'JUMPIFNEQ',
                               'STRI2INT', 'CONCAT', 'GETCHAR', 'SETCHAR']:
                self.check_num_of_arg(instruction, 3)
                i = InstCls(inst_name, instruction[0], instruction[1], instruction[2])
                self.dbg += 1
            else:
                print_err_and_exit(f'CHYBA: inštrukcia {inst_name} (order={instruction.attrib["order"]}) je neplatná', 32)

            inst_list.insert_inst(i)

    @staticmethod
    def check_num_of_arg(instruction: InstCls, number: int):
        if (len(list(instruction))) != number:
            print_err_and_exit(f'Inštrukcia {instruction.attrib["opcode"]} musí mať počet argumentov práve {number}.', 32)
