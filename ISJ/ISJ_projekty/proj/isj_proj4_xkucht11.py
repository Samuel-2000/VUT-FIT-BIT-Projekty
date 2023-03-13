#!/usr/bin/env python3

from itertools import permutations

def all_permutations_substrings(a_str): # Generates all permutations of all substrings of the input string
    return set(''.join(item) for length in range(len(a_str)+1) for item in permutations(a_str, length))

# max 2 points
def match_permutations_substrings(string, words): # ('okna', ['a', 'z', 'v', 'o', 'k', 'ok', 'ano', 'no', 'hlava', 'oko', 'noky', 'nok', 'on', 'ona', 'ony'])
    perms = all_permutations_substrings(string) # permutations as a set
    return {item for item in perms if item in words} # {'ona', 'a', 'ok', 'o', 'nok', 'no', 'ano', 'on', 'k'}

# max 1 point
def uniq_srt(it): # uniq_srt([3, 3, 5, 3, 4, 2, 4])
    return sorted(list(set(it))) # [2, 3, 4, 5]

# max 2 points
def uniq_orig_order(it): # uniq_orig_order([3, 3, 5, 3, 4, 2, 4])
    lst = []
    for i in it:
        if i not in lst:
            lst.append(i)
    return lst # [3, 5, 4, 2]

if __name__ == "__main__":
    import doctest
    doctest.testmod()
