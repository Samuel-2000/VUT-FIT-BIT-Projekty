import collections


def log_and_count(counts, key=None):
    def get_func(function):
        def inner(*args, **kwargs):
            nonlocal key, counts
            if key is None:
                key = function.__name__
            counts[key] += 1
            print(f"called {function.__name__} with {args} and {kwargs}")
            return function(*args, **kwargs)
        return inner
    return get_func


my_counter = collections.Counter()


@log_and_count(key='basic functions', counts=my_counter)
def f1(a, b=2):
    return a ** b


@log_and_count(key='basic functions', counts=my_counter)
def f2(a, b=3):
    return a ** 2 + b


@log_and_count(counts=my_counter)
def f3(a, b=5):
    return a ** 3 - b


if __name__ == '__main__':
    f1(2)           # >>> called f1 with (2,) and {}
    f2(2, b=4)      # >>> called f2 with (2,) and {'b': 4}
    f1(a=2, b=4)    # >>> called f1 with () and {'a': 2, 'b': 4}
    f2(4)           # >>> called f2 with (4,) and {}
    f2(5)           # >>> called f2 with (5,) and {}
    f3(5)           # >>> called f3 with (5,) and {}
    f3(5, 4)        # >>> called f3 with (5, 4) and {}
    print(my_counter)  # >>> Counter({'basic functions': 5, 'f3': 2})
