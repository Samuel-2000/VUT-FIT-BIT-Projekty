# Samuel Kuchta (xkucht11)

from py_lib.error import *
from py_lib.args_checker import ArgsChecker
from py_lib.XMLparser import XmlParser
from py_lib.inst_list import InstListCls
from py_lib.frame import FrameCls

dbg = 0
argsChecker = ArgsChecker()
argsChecker.check()  # skontrolute argumenty.
dbg += 1
parser = XmlParser(argsChecker.get_xml_path())
parser.check()  # skontrolujte súbor XML.
dbg += 1
Inst_list = InstListCls()  # vytvoriť prázdny zoznam inštrukcií.
parser.import_instructions(Inst_list)  # načítá inštrukcie do zoznamu inštrukcií.
Inst_list.str_check()  # skontroluje reťazce a nahradí znak '\'.
dbg += 1
frame = FrameCls()  # vytvorí inštanciu Frame helper
dataStack = []  # vytvorí dátový zásobník

lineCounter = 0  # pre inštrukciu READ
dbg += 1
while (inst := Inst_list.next_inst()) is not None:
    Inst_name = inst.type
    dbg += 1

    # Práce s rámci, volání funkcí
    if Inst_name == 'MOVE':
        dbg += 1
        arg_type, data = inst.get_arg(inst.arg2, frame)
        frame.set_var(inst.arg1, arg_type, data)

    elif Inst_name == 'CREATEFRAME':
        dbg += 1
        frame.create_frame()

    elif Inst_name == 'PUSHFRAME':
        dbg += 1
        frame.push_frame()

    elif Inst_name == 'POPFRAME':
        dbg += 1
        frame.pop_frame()

    elif Inst_name == 'DEFVAR':
        dbg += 1
        frame.def_var(inst.arg1)

    elif Inst_name == 'CALL':
        dbg += 1
        Inst_list.store_pos()
        Inst_list.jump(inst.arg1)

    elif Inst_name == 'RETURN':
        dbg += 1
        Inst_list.restore_pos()

        # Práce s datovým zásobníkem
    elif Inst_name == 'PUSHS':
        dbg += 1
        arg_type, data = inst.get_arg(inst.arg1, frame)
        dataStack.append((arg_type, data))

    elif Inst_name == 'POPS':
        dbg += 1
        try:
            arg_type, data = dataStack.pop()
        except IndexError:
            print_err_and_exit(f'Inštrukcia {Inst_name}: prázdny dátový zásobník.', 56)
        frame.set_var(inst.arg1, arg_type, data)

        # Aritmetické, relační, booleovské a konverzní instrukce
    elif Inst_name in ['ADD', 'SUB', 'MUL', 'IDIV']:
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        type2, data2 = inst.get_arg(inst.arg3, frame)
        if type1 == type2 == 'int':
            dbg += 1
            try:
                int_a = int(data1)
                int_b = int(data2)
            except ValueError:
                print_err_and_exit(f'Inštrukcia {Inst_name}: zlá hodnota argumentu. Hodnota musí byť číslo.', 32)
            dbg += 1
            if Inst_name == 'ADD':
                frame.set_var(inst.arg1, 'int', str(int_a + int_b))
            elif Inst_name == 'SUB':
                frame.set_var(inst.arg1, 'int', str(int_a - int_b))
            elif Inst_name == 'MUL':
                frame.set_var(inst.arg1, 'int', str(int_a * int_b))
            else:
                try:
                    frame.set_var(inst.arg1, 'int', str(int_a // int_b))
                except ZeroDivisionError:
                    print_err_and_exit(f'CHYBA: Delenie nulou vo výraze {Inst_name} {inst.arg1["data"]} {int_a} {int_b}.', 57)
        else:
            print_err_and_exit(f'Zlé typy operandov: {type1}, {type2} v inštrukcii {Inst_name}. Vsetky operadne musia byť typu integer', 53)

    elif Inst_name in ['LT', 'GT', 'EQ']:
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        type2, data2 = inst.get_arg(inst.arg3, frame)
        dbg += 1
        if type1 == type2:
            dbg += 1
            if Inst_name == 'EQ':
                dbg += 1
                frame.set_var(inst.arg1, 'bool', 'true' if data1 == data2 else 'false')
            elif (Inst_name in ['GT', 'LT'] and (type1 == 'nil' and type2 == 'nil')):
                print_err_and_exit(f'V inštrukcii {Inst_name} nie je možné porovnať typy {type1} a {type2}.', 53)
            elif Inst_name == 'LT':
                dbg += 1
                if type1 == 'string':
                    frame.set_var(inst.arg1, 'bool', 'true' if data1 < data2 else 'false')
                elif type1 == 'nil':
                    frame.set_var(inst.arg1, 'bool', 'false')
                elif type1 == 'bool':
                    dbg += 1
                    frame.set_var(inst.arg1, 'bool', 'true' if data1 == 'false' and data2 == 'true' else 'false')
                else:  # int
                    frame.set_var(inst.arg1, 'bool', 'true' if int(data1) < int(data2) else 'false')
            else:
                dbg += 1
                if type1 == 'string':
                    frame.set_var(inst.arg1, 'bool', 'true' if data1 > data2 else 'false')
                elif type1 == 'nil':
                    frame.set_var(inst.arg1, 'bool', 'false')
                elif type1 == 'bool':
                    dbg += 1
                    frame.set_var(inst.arg1, 'bool', 'true' if data1 == 'true' and data2 == 'false' else 'false')
                else:
                    frame.set_var(inst.arg1, 'bool', 'true' if int(data1) > int(data2) else 'false')

        elif Inst_name == 'EQ' and (type1 == 'nil' or type2 == 'nil'):
            dbg += 1
            frame.set_var(inst.arg1, 'bool', 'false')
        else:
            print_err_and_exit(f'V inštrukcii {Inst_name} nie je možné porovnať typy {type1} a {type2}.', 53)

    elif Inst_name in ['AND', 'OR']:
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        type2, data2 = inst.get_arg(inst.arg3, frame)

        if type1 == type2 == 'bool':
            dbg += 1
            if Inst_name == 'AND':
                frame.set_var(inst.arg1, 'bool', 'true' if data1 == data2 == 'true' else 'false')
            else:  # OR
                frame.set_var(inst.arg1, 'bool', 'true' if data1 == 'true' or data2 == 'true' else 'false')
        else:
            print_err_and_exit(f'Nepodarilo sa vykonať "and" alebo "or" s neboolovskými typmi {type1} a {type2}.', 53)

    elif Inst_name == 'NOT':
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        if type1 == 'bool':
            dbg += 1
            frame.set_var(inst.arg1, 'bool', 'true' if data1 == 'false' else 'false')
        else:
            print_err_and_exit(f'Nepodarilo sa vykonať {Inst_name} s typom {type1}.', 53)

    elif Inst_name == 'STRI2INT':
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        type2, data2 = inst.get_arg(inst.arg3, frame)

        if type1 == 'string' and type2 == 'int':
            dbg += 1
            i = int(data2)
            if i >= 0 and i <= len(data1) - 1:
                ordd = ord(data1[i])
                frame.set_var(inst.arg1, 'int', ordd)
            else:
                print_err_and_exit(f'Indexácia mimo reťazca pri inštrukcii {Inst_name}.', 58)
        else:
            print_err_and_exit(f'Nepodarilo sa vykonať {Inst_name} s typmi {type1} a {type2}.', 53)

    elif Inst_name == 'INT2CHAR':
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        if type1 == 'int':
            dbg += 1
            try:
                char = chr(int(data1))
            except ValueError:
                print_err_and_exit(f'Nepodarilo sa previesť hodnotu {data1} do typu char.', 58)
            frame.set_var(inst.arg1, 'string', char)
        else:
            print_err_and_exit(f'Nepodarilo sa vykonat {Inst_name} s typom {type1}, nakoľko inštrukcia vyzaduje integer.', 53)

        #  Vstupně-výstupní a ladící instrukce
    elif Inst_name == 'READ':
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        if len(argsChecker.get_input_path()):
            dbg += 1
            try:
                with open(argsChecker.get_input_path()) as file:
                    uis = file.read().splitlines()
            except FileNotFoundError:
                print_err_and_exit('Nepodarilo sa otvoriť a prečítať vstup zo súboru.', 11)
            dbg += 1
            try:
                User_input = uis[lineCounter]
            except IndexError:
                print_err('Chýbajúci údaj v premennej.')
                frame.set_var(inst.arg1, 'nil', '')
                continue
            finally:
                lineCounter += 1
        else:
            dbg += 1
            try:
                User_input = input()
            except Exception:
                print_err_and_exit('Nepodarilo sa vziať vstup zo stdin.', 11)

        if data1 == 'int':
            dbg += 1
            try:
                number = str(int(User_input))
            except:
                print_err('Bol zadaný chybný údaj do premennej typu int.')
                frame.set_var(inst.arg1, 'nil', '')
            else:
                frame.set_var(inst.arg1, 'int', number)
        elif data1 == 'bool':
            dbg += 1
            if User_input.lower() == 'true':
                frame.set_var(inst.arg1, 'bool', 'true')
            elif User_input.lower() == 'false':
                frame.set_var(inst.arg1, 'bool', 'false')
            else:
                print_err('Bol zadaný chybný údaj do premennej typu bool.')
                frame.set_var(inst.arg1, 'bool', 'false')
        else:
            dbg += 1
            frame.set_var(inst.arg1, 'string', User_input)

    elif Inst_name in ['WRITE', 'DPRINT']:
        dbg += 1
        aType, aData = inst.get_arg(inst.arg1, frame)
        if aData == None:
            print_err_and_exit('Pokus o prístup k neinicializovanej premennej.', 56)
        else:
            dbg += 1
            if (aType == 'nil' and aData == 'nil'):
                aData = ''

            if Inst_name == 'WRITE':
                print(aData, end='')
            else:
                print_stderr(aData)

    elif Inst_name == 'BREAK':
        dbg += 1
        pass

        # Práce s řetězci
    elif Inst_name == 'CONCAT':
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        type2, data2 = inst.get_arg(inst.arg3, frame)

        if type1 == type2 == 'string':
            dbg += 1
            data1 = '' if data1 is None else data1
            data2 = '' if data2 is None else data2
            frame.set_var(inst.arg1, 'string', data1 + data2)
        else:
            print_err_and_exit('Nepodarilo sa vykonať konkatenáciu reťazca. nespravny dátový typ', 53)

    elif Inst_name == 'STRLEN':
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)

        if type1 == 'string':
            dbg += 1
            frame.set_var(inst.arg1, 'int', len(data1))
        else:
            print_err_and_exit('Nepodarilo sa zistiť dĺžku reťazca (zlý operand)', 53)

    elif Inst_name == 'GETCHAR':
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        type2, data2 = inst.get_arg(inst.arg3, frame)

        if type1 == 'string' and type2 == 'int':
            dbg += 1
            pos = int(data2)
            if pos >= 0 and pos < len(data1):
                frame.set_var(inst.arg1, 'string', data1[pos])
            else:
                print_err_and_exit(f'Indexácia mimo reťazca u inštrukcie {Inst_name}.', 58)
        else:
            print_err_and_exit(f'Nepodarilo sa vykonať operáciu {Inst_name} (zlé operandy)', 53)

    elif Inst_name == 'SETCHAR':
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        type2, data2 = inst.get_arg(inst.arg3, frame)
        dataV: str
        typeV, dataV = inst.get_arg(inst.arg1, frame)

        if type1 == 'int' and type2 == 'string' and typeV == 'string':
            dbg += 1
            pos = int(data1)
            if pos < 0 or pos >= len(dataV) or dataV == '':
                print_err_and_exit(f'Indexácia mimo reťazca pri inštrukcii {Inst_name}.', 58)
            if data2 == '':
                print_err_and_exit(f'Prázdny reťazec - chyba u inštrukcie {Inst_name}.', 58)
            else:
                data_list = list(dataV)
                data_list[pos] = data2[0]
                dataV = "".join(data_list)
                frame.set_var(inst.arg1, 'string', dataV)
        else:
            print_err_and_exit(f'Nepodarilo sa previesť {Inst_name} setchar (zlé operandy)', 53)

        # Práce s typy
    elif Inst_name == 'TYPE':
        dbg += 1
        type1 = inst.get_type(inst.arg2, frame)
        if type1 is None:
            type1 = ''
        frame.set_var(inst.arg1, 'string', type1)

        # Instrukce pro řízení toku programu
    elif Inst_name == 'LABEL':
        dbg += 1
        continue

    elif Inst_name == 'JUMP':
        dbg += 1
        Inst_list.jump(inst.arg1)

    elif Inst_name in ['JUMPIFEQ', 'JUMPIFNEQ']:
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg2, frame)
        type2, data2 = inst.get_arg(inst.arg3, frame)

        Inst_list.check_label(inst.arg1)

        if (type1 == type2 or type1 == 'nil' or type2 == 'nil'):
            dbg += 1
            if Inst_name == 'JUMPIFEQ' and data1 == data2:
                Inst_list.jump(inst.arg1)
            elif Inst_name == 'JUMPIFNEQ' and data1 != data2:
                Inst_list.jump(inst.arg1)
            else:
                pass
        else:
            print_err_and_exit(f'Argumenty inštrukcie {Inst_name} nie sú rovnakého typu.', 53)

    elif Inst_name == 'EXIT':
        dbg += 1
        type1, data1 = inst.get_arg(inst.arg1, frame)
        if type1 != 'int':
            print_err_and_exit(f'Chybný typ argumentu v inštrukcii {Inst_name}.', 53)
        else:
            dbg += 1
            ret_val = int(data1)
            if ret_val < 0 or ret_val > 49:
                print_err_and_exit(f'Chybná číselná hodnota v argumente inštrukcie {Inst_name}.', 57)
            else:
                print_err_and_exit('', ret_val)
