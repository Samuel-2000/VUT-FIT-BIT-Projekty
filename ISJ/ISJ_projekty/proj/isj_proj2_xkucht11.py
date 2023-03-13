#!/usr/bin/env python3

  # ukol za 2 body
def she_says_he_says(she_says):                     # 'ma rymu'
    phonetic_she_says = she_says.replace('y', 'i')  # Replaces y/i
    compact = phonetic_she_says.replace(' ', '')    # removes spaces
    he_says = compact[::-1]                         # returns reversed
    return he_says                                  # 'umiram'

  # ukol za 3 body
def solfege(title_hymn):    # 'Hymn of St. John: Ut queant laxis re sonare fibris mi ra gestorum fa muli tuorum sol ve polluti la bii reatum Sancte Iohannes'
    _, _, hymn = title_hymn.rpartition(": ")  # the input string partitioned to the title (if given) and the actual hymn
    hymn_list = hymn.split(' ')     # the hymn as a list of strings separated by ' '
    skip2 = hymn_list[:-3:3]        # skipping always two strings, and ending 3 strings from the end
    skip2_str = ', '.join(skip2)    # the skip2 list as a string, ', ' as a separator
    return skip2_str        # 'Ut, re, mi, fa, sol, la'

if __name__ == "__main__":
    # import doctest
    # doctest.testmod()
    pass
