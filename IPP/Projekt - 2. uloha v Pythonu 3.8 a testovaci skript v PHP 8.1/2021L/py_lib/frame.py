from py_lib.error import print_err_and_exit
import py_lib.inst as i

class FrameCls:
    def __init__(self):
        super().__init__()
        self.globalFrame = {}
        self.tmpFrame = {}
        self.tmpFrameDefined = False
        self.frameStack = []
        self.dbg = 0

    def get_frame(self, frame: str) -> dict:
        self.dbg += 1
        if frame == 'GF':
            return self.globalFrame
        elif frame == 'LF':
            return self.frameStack[-1] if len(self.frameStack) > 0 else None
        elif frame == 'TF':
            return self.tmpFrame if self.tmpFrameDefined else None
        else:
            return None

    def create_frame(self):
        self.dbg += 1
        self.tmpFrame = {}
        self.tmpFrameDefined = True

    def push_frame(self):
        self.dbg += 1
        if self.tmpFrameDefined:
            self.frameStack.append(self.tmpFrame)
            self.tmpFrameDefined = False
        else:
            print_err_and_exit('Pokus o pushframe bez definovaného rámca.', 55)

    def pop_frame(self):
        self.dbg += 1
        if len(self.frameStack):  # neprazdny stack
            self.tmpFrame = self.frameStack.pop()
            self.tmpFrameDefined = True
        else:
            print_err_and_exit('Pokus o popframe nad prázdnym zásobníkom rámcov.', 55)

    def set_var(self, argument, type_val, data_val):
        self.dbg += 1
        frame, name = i.InstCls.split_var(argument)
        frameObj = self.get_frame(frame)
        if frameObj is None:
            print_err_and_exit('Pokus o čítanie premennj z nedefinovaného rámca.', 55)
        if name not in frameObj:
            print_err_and_exit('Pokús o zápis do neexistujúcej premennej.', 54)
        self.dbg += 1
        frameObj[name]['type'] = type_val
        frameObj[name]['data'] = data_val

    def def_var(self, argument):
        self.dbg += 1
        frame, name = i.InstCls.split_var(argument)
        frameObj = self.get_frame(frame)
        if frameObj is None:
            print_err_and_exit(f'Pokus o vytvorenie premennej {argument["data"]} na nedefinovanom rámci.', 55)
        else:
            self.dbg += 1
            if name in frameObj:
                print_err_and_exit('Pokús o vytvorenie už existujúcej premennej.', 52)
            else:
                frameObj[name] = {'data': None, 'type': None}
