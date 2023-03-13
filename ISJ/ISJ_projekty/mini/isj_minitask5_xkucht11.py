# Funkce dostane 3 parametry a _vrací_ (pomocí return) výsledný kvíz.
# Pokud je standardně zadán kvíz i na vstupu (jako 3. argument),
# tak se k existujícímu kvízu jen doplní otázka se zadaným indexem ze zadaného seznamu a výsledek se vrátí.
# Pokud není 3 parametr zadán, vrátí se seznam obsahující jen jednu otázku, danou prvními dvěma parametry.

def add_question(num, pool=None, quiz=None):


    if quiz is None:
        if pool is not None:
            return pool[num]
    else:
        if pool is not None:
            quizret = [('Which of the following keywords marks the beginning of the function block?',
                        ['func',
                         'define',
                         'def',
                         'func',
                         ])]
            quizret.append[pool[num]]
            return quizret
        else:
            return [('Which of the following keywords marks the beginning of the function block?',
                        ['func',
                         'define',
                         'def',
                         'func',
                         ])]

pool = [('If return statement is not used inside the function, the function will return:',
  ['0',
   'None object',
   'an arbitrary integer',
   'Error! Functions in Python must have a return statement.'
  ]),
 ('Which of the following function calls can be used to invoke function definition:\n def test(a, b, c, d):?',
  ['test(1, 2, 3, 4)',
   'test(a = 1, 2, 3, 4)',
   'test(a = 1, b = 2, c = 3, 4)',
   'test(a = 1, b = 2, c = 3, d = 4)',
   'test(1, 2, 3, d = 4)])'])
]



default_qentry = ('How do you feel today?', ['sad','happy','angry'])

funcqpool = [('If return statement is not used inside the function, the function will return:',
          ['0',
           'None object',
           'an arbitrary integer',
           'Error! Functions in Python must have a return statement.'
          ]),
         ('Which of the following function calls can be used to invoke function definition:\n def test(a, b, c, d):?',
          ['test(1, 2, 3, 4)',
           'test(a = 1, 2, 3, 4)',
           'test(a = 1, b = 2, c = 3, 4)',
           'test(a = 1, b = 2, c = 3, d = 4)',
           'test(1, 2, 3, d = 4)])'])
        ]
funcquiz1 = [('Which of the following keywords marks the beginning of the function block?',
         ['func',
          'define',
          'def',
          'func',
         ])]



#print(add_question(0, funcqpool, funcquiz1))
print(funcquiz1)
print(add_question(0, funcqpool))