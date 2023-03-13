poduloha16([], KLIC, []):- !.
poduloha16(SEZNAM, [], SEZNAM) :- !.
poduloha16([X|Xs], [Y|Ys], [Z|Zs]) :-
    	Z is X + Y,
	!,
	poduloha16(Xs, Ys, Zs).

uloha16(SEZNAM, KLIC, SEZNAMO) :-
    	poduloha16(SEZNAM, KLIC, T4),
    	T4 = SEZNAMO.
