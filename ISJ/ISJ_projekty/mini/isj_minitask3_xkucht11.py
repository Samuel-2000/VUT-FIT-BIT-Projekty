def complex_func(x,y):
    return 2 ** x ** y
print(complex_func(2,3))
def first(x):
    return 2 ** x - 1
def second(x):
    return 3 ** x - 2
def third(x):
    return 5 ** x - 3
# minitask 3

funlist = [first, second, third, complex_func]  # list of functions first, second, third, complex_func
a = 2
b = 3
print(funlist[-1](a, b))  # print the result of the last function in the list with parameters a and b
for func in funlist[1:3]:  # from the second to the third
    print(func(a))  # print the result of the function with parameter a
