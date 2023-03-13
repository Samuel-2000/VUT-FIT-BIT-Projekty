from py_lib.frame import FrameCls
from py_lib.error import *


class InstCls:
    def __init__(self, type, arg1=None, arg2=None, arg3=None):
        super().__init__()
        self.type = type
        self.argsCount = 0
        self.dbg = 0
        if arg1 is not None:
            self.arg1 = {'type': arg1.attrib['type'], 'data': (arg1.text if arg1.text is not None else '')}
            self.argsCount += 1
            self.dbg += 1
        if arg2 is not None:
            self.arg2 = {'type': arg2.attrib['type'], 'data': (arg2.text if arg2.text is not None else '')}
            self.argsCount += 1
            self.dbg += 1
        if arg3 is not None:
            self.arg3 = {'type': arg3.attrib['type'], 'data': (arg3.text if arg3.text is not None else '')}
            self.argsCount += 1
            self.dbg += 1

    @staticmethod
    def split_var(variable):
        return variable['data'].split('@', 1)

    def get_arg(self, argument: dict, frame_class_obj: FrameCls): 
        self.dbg += 1
        if argument['type'] in ['int', 'bool', 'string', 'type', 'label', 'nil']:
            return argument['type'], argument['data']
        else:
            self.dbg += 1
            frame, data = self.split_var(argument)
            frame_obj = frame_class_obj.get_frame(frame)
            if frame_obj is None:
                print_err_and_exit('Pokús o čítanie premennej z neexistujúceho ramca.', 55)
            if data not in frame_obj:
                print_err_and_exit('Pokus o čítanie nedefinovanej premennej v existujúcom ramci.', 54)
            else:
                self.dbg += 1
                if (frame_obj[data]['type'] is None or frame_obj[data]['data'] is None):
                    print_err_and_exit('Pokus o čítanie nedefinovanej premennej', 56)
                return (frame_obj[data]['type'], frame_obj[data]['data'])

    def get_type(self, argument: dict, frame_obj: FrameCls) -> str:
        self.dbg += 1
        if argument['type'] in ['int', 'bool', 'string', 'type', 'label', 'nil']:
            return argument['type']
        else:
            self.dbg += 1
            frame, data = self.split_var(argument)
            frame_obj = frame_obj.get_frame(frame)
            if frame_obj is None:
                print_err_and_exit('Pokus o čítanie premennej z nedefinovaného ramca.', 55)
            if data not in frame_obj:
                print_err_and_exit('Pokus o čítanie nedefinovanej premennej v existujúcom ramci.', 54)
            else:
                self.dbg += 1
                return frame_obj[data]['type']
