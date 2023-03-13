#!/usr/bin/env python3
# Samuel Kuchta (xkucht11)
# ukol za 2 body
def first_odd_or_even(numbers):
    even_cnt = 0
    odd_cnt = 0

    first_even = 0
    first_odd = 0

    bool_even = False
    bool_odd = False

    for num in numbers:
        if num % 2 == 0:
            even_cnt = even_cnt + 1
            if bool_even is False:
                first_even = num
                bool_even = True
        else:
            odd_cnt = odd_cnt + 1
            if bool_odd is False:
                first_odd = num
                bool_odd = True


    if even_cnt == odd_cnt or even_cnt == 0 or odd_cnt == 0:
        return 0
    elif even_cnt > odd_cnt:
        return first_odd
    else:
        return first_even


# ukol za 3 body
def to_pilot_alpha(word):
    pilot_alpha = ['Alfa', 'Bravo', 'Charlie', 'Delta', 'Echo', 'Foxtrot',
                   'Golf', 'Hotel', 'India', 'Juliett', 'Kilo', 'Lima', 'Mike',
                   'November', 'Oscar', 'Papa', 'Quebec', 'Romeo', 'Sierra', 'Tango',
                   'Uniform', 'Victor', 'Whiskey', 'Xray', 'Yankee', 'Zulu']

    pilot_alpha_list = []

    for letter in word:
        for code in pilot_alpha:
            if letter.upper() == code[0]:
                pilot_alpha_list.append(code)
                break

    return pilot_alpha_list


if __name__ == "__main__":
    import doctest

    doctest.testmod()
