#!/usr/bin/env python3
# minitask 4
# Samuel Kuchta (xkucht11)


"""
wanted = {'a':17,'b':34,'z':3}
"""
mcase = {'a':10,'b':34,'A':7,'Z':3}
names = ['Alice', 'Bob', 'John', 'JOHN', 'alice']
wanted = {k.lower(): mcase.get(k.upper(), 0) + mcase.get(k.lower(), 0) for k in mcase}
print(wanted)