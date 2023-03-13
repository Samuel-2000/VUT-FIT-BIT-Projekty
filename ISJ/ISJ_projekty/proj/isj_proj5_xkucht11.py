def gen_quiz(test_qpool, indexing=0, *args, altcodes = 'ABCDEF', quiz=None):

    if quiz is None:
        quiz = []

    temp_quiz = []

    for dvojice in test_qpool:  # ('Question1', ['Answer1', 'Answer2', 'Answer3', 'Answer4'])

        question, answers = dvojice  # question = 'Question1', answer = ['Answer1', 'Answer2', 'Answer3', 'Answer4']

        answer_list = []

        for code, answer in zip(altcodes, answers):  # ('10', '20', '30'), ['Answer1', 'Answer2', 'Answer3', 'Answer4']
            tupl = (code, answer)  # ('10', 'Answer1')
            retazec = ': '.join(tupl)  # retazec = '10: Answer1'                                  # retazec = ':'.join(f'{code}, {answer}')
            answer_list.append(retazec)  # ['10: Answer1']

        # answer_list = ['10: Answer1', '20: Answer2', '30: Answer3']

        tupl2 = (question, answer_list) # ('Question1', ['10: Answer1', '20: Answer2', '30: Answer3'], ...)

        temp_quiz.extend(tupl2)

    final_quiz = []

    if indexing != 0:
        final_quiz.append(temp_quiz[indexing])

    final_quiz.append(temp_quiz[0])


    """
    list_len = len(temp_quiz)
    if indexing > 0:
        start = indexing - 1
        for i in range(start, list_len, indexing):
            final_quiz.append(temp_quiz[i])

    elif indexing < 0:
        list_len += indexing
        for i in range(list_len, -1, indexing):
            final_quiz.append(temp_quiz[i]) # 3 a 1
    """
    if len(quiz) == 0:
        return final_quiz

    else:
        quiz.append(final_quiz)
        return quiz



""""""
test_qpool6 = [('Question1', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question2', ['Answer1', 'Answer2', 'Answer3']), ('Question3', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question4', ['Answer1', 'Answer2'])]
existing_quiz6 = [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']), ('ExistingQuestion2', ['1: Answer1', '2: Answer2'])]
not_used = gen_quiz(test_qpool6, quiz = existing_quiz6)
    # [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']), ('ExistingQuestion2', ['1: Answer1', '2: Answer2'])]
print(f'{not_used}')

"""
test_qpool2 = [('Question1', ['Answer1', 'Answer2', 'Answer3', 'Answer4']),
               ('Question2', ['Answer1', 'Answer2', 'Answer3']),
                ('Question3', ['Answer1', 'Answer2', 'Answer3', 'Answer4']),
               ('Question4', ['Answer1', 'Answer2'])]

existing_quiz2 = [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']),
                       ('ExistingQuestion2', ['1: Answer1', '2: Answer2'])]

not_used = gen_quiz(test_qpool2, -2, 0, altcodes=('1', '2', '3'), quiz=existing_quiz2)

print(f'{not_used}')
"""
"""
    >>> test_qpool1 = [('Question1', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question2', ['Answer1', 'Answer2', 'Answer3']), ('Question3', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question4', ['Answer1', 'Answer2'])]
    >>> existing_quiz1 = [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']), ('ExistingQuestion2', ['1: Answer1', '2: Answer2'])]
    >>> gen_quiz(test_qpool1, -2, 0, altcodes = ('10', '20', '30'), quiz = existing_quiz1)
    [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']), ('ExistingQuestion2', ['1: Answer1', '2: Answer2']), ('Question3', ['10: Answer1', '20: Answer2', '30: Answer3']), ('Question1', ['10: Answer1', '20: Answer2', '30: Answer3'])]

    >>> test_qpool2 = [('Question1', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question2', ['Answer1', 'Answer2', 'Answer3']), ('Question3', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question4', ['Answer1', 'Answer2'])]
    >>> existing_quiz2 = [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']), ('ExistingQuestion2', ['1: Answer1', '2: Answer2'])]
    >>> not_used = gen_quiz(test_qpool2, -2, 0, altcodes = ('1', '2', '3'), quiz = existing_quiz2)
    >>> existing_quiz2
    [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']), ('ExistingQuestion2', ['1: Answer1', '2: Answer2']), ('Question3', ['1: Answer1', '2: Answer2', '3: Answer3']), ('Question1', ['1: Answer1', '2: Answer2', '3: Answer3'])]

    >>> test_qpool3 = [('Question1', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question2', ['Answer1', 'Answer2', 'Answer3']), ('Question3', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question4', ['Answer1', 'Answer2'])]
    >>> existing_quiz3 = [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']), ('ExistingQuestion2', ['1: Answer1', '2: Answer2'])]
    >>> not_used1 = gen_quiz(test_qpool3, 0, 2, altcodes = ('i', 'ii', 'iii'), quiz = existing_quiz3)
    >>> not_used2 = gen_quiz(test_qpool3, 1, altcodes = ('i', 'ii', 'iii'), quiz = existing_quiz3)
    >>> existing_quiz3
    [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']), ('ExistingQuestion2', ['1: Answer1', '2: Answer2']), ('Question1', ['i: Answer1', 'ii: Answer2', 'iii: Answer3']), ('Question3', ['i: Answer1', 'ii: Answer2', 'iii: Answer3']), ('Question2', ['i: Answer1', 'ii: Answer2', 'iii: Answer3'])]

    >>> test_qpool4 = [('Question1', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question2', ['Answer1', 'Answer2', 'Answer3']), ('Question3', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question4', ['Answer1', 'Answer2'])]
    >>> gen_quiz(test_qpool4, 0, 1, -1)
    [('Question1', ['A: Answer1', 'B: Answer2', 'C: Answer3', 'D: Answer4']), ('Question2', ['A: Answer1', 'B: Answer2', 'C: Answer3']), ('Question4', ['A: Answer1', 'B: Answer2'])]

    >>> test_qpool5 = [('Question1', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question2', ['Answer1', 'Answer2', 'Answer3']), ('Question3', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question4', ['Answer1', 'Answer2'])]
    >>> not_used = gen_quiz(test_qpool5, 0, altcodes = '123456')
    >>> gen_quiz(test_qpool5, 0, altcodes = '123456')
    [('Question1', ['1: Answer1', '2: Answer2', '3: Answer3', '4: Answer4'])]

    >>> test_qpool6 = [('Question1', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question2', ['Answer1', 'Answer2', 'Answer3']), ('Question3', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question4', ['Answer1', 'Answer2'])]
    >>> existing_quiz6 = [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']), ('ExistingQuestion2', ['1: Answer1', '2: Answer2'])]
    >>> gen_quiz(test_qpool6, quiz = existing_quiz6)
    [('ExistingQuestion1', ['1: Answer1', '2: Answer2', '3: Answer3']), ('ExistingQuestion2', ['1: Answer1', '2: Answer2'])]

    >>> test_qpool7 = [('Question1', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question2', ['Answer1', 'Answer2', 'Answer3']), ('Question3', ['Answer1', 'Answer2', 'Answer3', 'Answer4']), ('Question4', ['Answer1', 'Answer2'])]
    >>> gen_quiz(test_qpool7, 0, 4, 2, altcodes = ['101','201']) 
    Ignoring index 4 - list index out of range
    [('Question1', ['101: Answer1', '201: Answer2']), ('Question3', ['101: Answer1', '201: Answer2'])]
    """