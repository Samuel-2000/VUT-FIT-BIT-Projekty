from py_lib.inst import InstCls
from py_lib.error import *


class InstListCls:
    def __init__(self):
        super().__init__()
        self.Dict = {}
        self.Inst_counter = 0
        self.Current = 1
        self.Call_stack = []
        self.Labels = {}
        self.dbg = 0

    def insert_inst(self, inst: InstCls):
        self.dbg += 1
        self.Inst_counter += 1
        self.Dict[self.Inst_counter] = inst

        if inst.type == 'LABEL':
            self.dbg += 1
            name = inst.arg1['data']
            if name not in self.Labels:
                self.Labels[name] = self.Inst_counter
            else:
                print_err_and_exit('Pokus o redefiníciu náveštia.', 52)

    def next_inst(self) -> InstCls:
        self.dbg += 1
        if self.Current <= self.Inst_counter:
            Curr_inst = self.Dict[self.Current]
            self.Current += 1
            self.dbg += 1
            return Curr_inst
        else:
            return None

    def store_pos(self):
        self.dbg += 1
        self.Call_stack.append(self.Current)

    def restore_pos(self):
        self.dbg += 1
        if len(self.Call_stack):
            self.Current = self.Call_stack.pop()
        else:
            print_err_and_exit('Žiadna hodnota v zásobníku volania.', 56)

    def jump(self, argument: dict):
        self.dbg += 1
        name = argument['data']
        if name in self.Labels:
            self.Current = self.Labels[name]
        else:
            print_err_and_exit(f'Pokus o skok na neexistujúce náveštie "{name}".', 52)

    def check_label(self, argument: dict):
        self.dbg += 1
        name = argument['data']
        if name not in self.Labels:
            print_err_and_exit(f'Pokus o skok na neexistujúce náveštie "{name}.', 52)

    @staticmethod
    def str_fix(string: str) -> str:
        index: int = string.find('\\')
        while index != -1:
            string = string.replace(string[index:index + 4], chr(int(string[index + 1:index + 4])))
            index = string.find('\\', index + 1)
        return string

    def str_check(self):
        self.dbg += 1
        for Inst_name in self.Dict:
            self.dbg += 1
            Inst = self.Dict[Inst_name]
            if hasattr(Inst, 'arg1'):
                if Inst.arg1['type'] == 'string':
                    Inst.arg1['data'] = InstListCls.str_fix(Inst.arg1['data'])
            if hasattr(Inst, 'arg2'):
                if Inst.arg2['type'] == 'string':
                    Inst.arg2['data'] = InstListCls.str_fix(Inst.arg2['data'])
            if hasattr(Inst, 'arg3'):
                if Inst.arg3['type'] == 'string':
                    Inst.arg3['data'] = InstListCls.str_fix(Inst.arg3['data'])
