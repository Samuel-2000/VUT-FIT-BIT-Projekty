def first_with_given_key(iterable, key=lambda x: x):
    iterator = iter(iterable)
    found = []
    for item in iterator:
        key_val = key(item)
        if key_val not in found:
            found.append(key_val)
            yield item
