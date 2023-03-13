# minitask 8
# f is a general iterator, for example a file
f = open('students.txt', encoding="utf-8")
it = iter(f)  # makes iterator from iterable
next(it)  # skips header
for line in it:
    print(line.rstrip())
