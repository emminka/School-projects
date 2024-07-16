/*
FLP projekt cislo 2
Prolog - Rubikova kostka
Emma Krompascikova
xkromp00
LS 2023/2024*/

main :-
    prompt(_, ''),
    %precitanie vstupu po troch riadkoch
    read_lines2(S1, 3),
    read_lines2(P2, 3),
    read_lines2(S6, 3),

    transpose(P2, [S2,S3,S4,S5]),

    %startovacie pole zo vstupu
    Start = [S1,S2,S3,S4,S5,S6],

    %vysledny stav pola, ktory chceme dosiahnut
    Goal = [[['5','5','5'],['5','5','5'],['5','5','5']],
    [['1','1','1'],['1','1','1'],['1','1','1']],
    [['2','2','2'],['2','2','2'],['2','2','2']],
    [['3','3','3'],['3','3','3'],['3','3','3']],
    [['4','4','4'],['4','4','4'],['4','4','4']],
    [['6','6','6'],['6','6','6'],['6','6','6']]],
     
    ids_start(Goal, Start, Path),
    print_path(Path),
    write_pretty(Goal).

%ids_start(+Start, +Goal, -Path)
%startuje algoritmus IDS s pociatocnym limitom 0
ids_start(Start, Goal, Path) :-
    iterate_dls(Start, Goal, Path, 0).

%iterate_dls(+Start, +Goal, -Path, +Limit)
%vykonava jednu iteraciu IDS (DLS pre konkretny limit)
iterate_dls(Start, Goal, Path, Limit) :-
    dls_start(Start, Goal, Path, Limit),
    !.  

iterate_dls(Start, Goal, Path, Limit) :-
    NewLimit is Limit + 1,
    iterate_dls(Start, Goal, Path, NewLimit).

%dls_start(+Start, +Goal, -Path, +Limit)
%startuje algoritmus DLS
dls_start(Start, Goal, Path, Limit) :-
    dls([(Start, [])], Goal, Path, Limit).

%dls(+Stack, +Node, -Path, +Limit)
%ked najde ciel, vrat path
dls([(Node, Path) | _], Node, Path, _).
%najdeme mnozinu vsewtkych stavov do ktorych sa dokazeme dostat jednym pohybom
dls([(Node, Path) | Stack], Goal, FinalPath, Limit) :-
    findall((NextNode, [NextNode | Path]),
            ((
                movementU(Node, NextNode);
                movementU(NextNode, Node);
                movementD(Node, NextNode);
                movementD(NextNode, Node);
                movementR(Node, NextNode);
                movementR(NextNode, Node);
                movementL(Node, NextNode);
                movementL(NextNode, Node);
                movementF(Node, NextNode);
                movementF(NextNode, Node);
                movementB(Node, NextNode);
                movementB(NextNode, Node);
                movementM(Node, NextNode);
                movementM(NextNode, Node);
                movementE(Node, NextNode);
                movementE(NextNode, Node);
                movementS(Node, NextNode);
                movementS(NextNode, Node)
            ),
             \+ member(NextNode, Path), %vylucujeme cyklicke stavy (nemozeme sa dostat do stavu kde uz sme boli)
                length(Path, PathLength),
                PathLength < Limit %DLS
             ),
            NewNodes),
    append(NewNodes, Stack, UpdatedStack), %nove uzly idu na zaciatok zasobnika
    dls(UpdatedStack, Goal, FinalPath, Limit).

%print_path(+Path).
%pre vypisanie finalnych tahov k ziadanemu vysledku
print_path([]).
print_path([H|T]) :- write_pretty(H), nl, print_path(T).

%write_pretty(+A).
%pre pekne vypisanie na vystup v pozadovanom tvare
write_pretty([]).
write_pretty([S1,P2,P3,P4,P5,S6]) :-
        writeshort(S1),
        writelong(P2,P3,P4,P5),
        writeshort(S6).

%writeshort(+A)
%pre vypisanie prvych troch a poslednych 3 riadkov urciteho stavu
writeshort([]).
writeshort(S):- 
        S = [[A1, A2, A3],
        [B1, B2, B3],
        [C1, C2, C3]],
        write(A1),write(A2),write(A3),nl,
        write(B1),write(B2),write(B3),nl,
        write(C1),write(C2),write(C3),nl.

%writelong(+A)
%pre spravne vypisanie strednej "dlhej" casti
writelong([]).
writelong(P2,P3,P4,P5) :-
        P2 = [[A1, A2, A3],
              [B1, B2, B3],
              [C1, C2, C3]],
        P3 = [[A4, A5, A6],
              [B4, B5, B6],
              [C4, C5, C6]],
        P4 = [[A7, A8, A9],
              [B7, B8, B9],
              [C7, C8, C9]],
        P5 = [[A10, A11, A12],
              [B10, B11, B12],
              [C10, C11, C12]],
        write(A1),write(A2),write(A3),write(' '),write(A4),write(A5),write(A6),write(' '),
        write(A7),write(A8),write(A9),write(' '),write(A10),write(A11),write(A12),nl,
        write(B1),write(B2),write(B3),write(' '),write(B4),write(B5),write(B6),write(' '),
        write(B7),write(B8),write(B9),write(' '),write(B10),write(B11),write(B12),nl,
        write(C1),write(C2),write(C3),write(' '),write(C4),write(C5),write(C6),write(' '),
        write(C7),write(C8),write(C9),write(' '),write(C10),write(C11),write(C12),nl.

%transpose(+A, -B)
%odstranenie medzier a pattern matching pre jednotlive steny
transpose([
    [A1, A2, A3, _, A4, A5, A6, _, A7, A8, A9, _, A10, A11, A12],
    [B1, B2, B3, _, B4, B5, B6, _, B7, B8, B9, _, B10, B11, B12],
    [C1, C2, C3, _, C4, C5, C6, _, C7, C8, C9, _, C10, C11, C12]
], [
    [
        [A1, A2, A3],
        [B1, B2, B3],
        [C1, C2, C3]
    ],
    [
        [A4, A5, A6],
        [B4, B5, B6],
        [C4, C5, C6]
    ],
    [
        [A7, A8, A9],
        [B7, B8, B9],
        [C7, C8, C9]
    ],
    [
        [A10, A11, A12],
        [B10, B11, B12],
        [C10, C11, C12]
    ]
]).

%movementU(?A,?B)
%rotacia hornej steny
%strana 6 sa nikdy nezmeni
%pre spatny pohyb len zamenim argumenty
movementU([S1,S2,S3,S4,S5,S6],[T1,T2,T3,T4,T5,S6]) :-
    rotate_clockwise(S1,T1),
    S2=    [[A1, A2, A3],
            [B1, B2, B3],
            [C1, C2, C3]],
    S3=    [[D1, D2, D3],
            [E1, E2, E3],
            [F1, F2, F3]],
    S4=    [[G1, G2, G3],
            [H1, H2, H3],
            [I1, I2, I3]],
    S5=    [[J1, J2, J3],
            [K1, K2, K3],
            [L1, L2, L3]],
    T2=    [[D1, D2, D3],
            [B1, B2, B3],
            [C1, C2, C3]],
    T3=    [[G1, G2, G3],
            [E1, E2, E3],
            [F1, F2, F3]],
    T4=    [[J1, J2, J3],
            [H1, H2, H3],
            [I1, I2, I3]],
    T5=    [[A1, A2, A3],
            [K1, K2, K3],
            [L1, L2, L3]].

%movementD(?A,?B)
%rotacia spodnej steny
%strana 1 sa nikdy nezmeni
%pre spatny pohyb len zamenim argumenty
movementD([S1,S2,S3,S4,S5,S6],[S1,T2,T3,T4,T5,T6]) :-
    rotate_clockwise(S6,T6),
    S2=    [[A1, A2, A3],
            [B1, B2, B3],
            [C1, C2, C3]],
    S3=    [[D1, D2, D3],
            [E1, E2, E3],
            [F1, F2, F3]],
    S4=    [[G1, G2, G3],
            [H1, H2, H3],
            [I1, I2, I3]],
    S5=    [[J1, J2, J3],
            [K1, K2, K3],
            [L1, L2, L3]],
    T2=    [[A1, A2, A3],
            [B1, B2, B3],
            [L1, L2, L3]],
    T3=    [[D1, D2, D3],
            [E1, E2, E3],
            [C1, C2, C3]],
    T4=    [[G1, G2, G3],
            [H1, H2, H3],
            [F1, F2, F3]],
    T5=    [[J1, J2, J3],
            [K1, K2, K3],
            [I1, I2, I3]].

%movementR(?A,?B)
%rotacia pravej steny
%strana 5 sa nikdy nezmeni
%pre spatny pohyb len zamenim argumenty
movementR([S1,S2,S3,S4,S5,S6],[T1,T2,T3,T4,S5,T6]) :-
    rotate_clockwise(S3,T3),
    S1=    [[A1, A2, A3],
            [B1, B2, B3],
            [C1, C2, C3]],
    S2=    [[D1, D2, D3],
            [E1, E2, E3],
            [F1, F2, F3]],
    S4=    [[G1, G2, G3],
            [H1, H2, H3],
            [I1, I2, I3]],
    S6=    [[J1, J2, J3],
            [K1, K2, K3],
            [L1, L2, L3]],
    T1=    [[A1, A2, D3],
            [B1, B2, E3],
            [C1, C2, F3]],
    T2=    [[D1, D2, J3],
            [E1, E2, K3],
            [F1, F2, L3]],
    T4=    [[C3, G2, G3],
            [B3, H2, H3],
            [A3, I2, I3]],
    T6=    [[J1, J2, I1],
            [K1, K2, H1],
            [L1, L2, G1]].

%movementL(?A,?B)
%rotacia lavej steny
%strana 3 sa nikdy nezmeni
%pre spatny pohyb len zamenim argumenty
movementL([S1,S2,S3,S4,S5,S6],[T1,T2,S3,T4,T5,T6]) :-
    rotate_clockwise(S5,T5),
    S1=    [[A1, A2, A3],
            [B1, B2, B3],
            [C1, C2, C3]],
    S2=    [[D1, D2, D3],
            [E1, E2, E3],
            [F1, F2, F3]],
    S4=    [[G1, G2, G3],
            [H1, H2, H3],
            [I1, I2, I3]],
    S6=    [[J1, J2, J3],
            [K1, K2, K3],
            [L1, L2, L3]],
    T1=    [[I3, A2, A3],
            [H3, B2, B3],
            [G3, C2, C3]],
    T2=    [[A1, D2, D3],
            [B1, E2, E3],
            [C1, F2, F3]],
    T4=    [[G1, G2, L1],
            [H1, H2, K1],
            [I1, I2, J1]],
    T6=    [[D1, J2, J3],
            [E1, K2, K3],
            [F1, L2, L3]].

%movementF(?A,?B)
%rotacia prednej steny
%strana 4 sa nikdy nezmeni
%pre spatny pohyb len zamenim argumenty
movementF([S1,S2,S3,S4,S5,S6],[T1,T2,T3,S4,T5,T6]) :-
    rotate_clockwise(S2,T2),
    S1=    [[A1, A2, A3],
            [B1, B2, B3],
            [C1, C2, C3]],
    S3=    [[D1, D2, D3],
            [E1, E2, E3],
            [F1, F2, F3]],
    S5=    [[G1, G2, G3],
            [H1, H2, H3],
            [I1, I2, I3]],
    S6=    [[J1, J2, J3],
            [K1, K2, K3],
            [L1, L2, L3]],
    T1=    [[A1, A2, A3], 
            [B1, B2, B3],
            [I3, H3, G3]],
    T3=    [[C1, D2, D3], 
            [C2, E2, E3],
            [C3, F2, F3]],
    T5=    [[G1, G2, J1],
            [H1, H2, J2],
            [I1, I2, J3]], 
    T6=    [[F1, E1, D1],
            [K1, K2, K3],
            [L1, L2, L3]].

%movementB(?A,?B)  
%rotacia zadnej steny
%strana 2 sa nikdy nezmeni
%pre spatny pohyb len zamenim argumenty
movementB([S1,S2,S3,S4,S5,S6],[T1,S2,T3,T4,T5,T6]) :-
    rotate_clockwise(S4,T4),
    S1=    [[A1, A2, A3],
            [B1, B2, B3],
            [C1, C2, C3]],
    S3=    [[D1, D2, D3],
            [E1, E2, E3],
            [F1, F2, F3]],
    S5=    [[G1, G2, G3],
            [H1, H2, H3],
            [I1, I2, I3]],
    S6=    [[J1, J2, J3],
            [K1, K2, K3],
            [L1, L2, L3]],
    T1=    [[D3, E3, F3],
            [B1, B2, B3],
            [C1, C2, C3]],
    T3=    [[D1, D2, L3],
            [E1, E2, L2],
            [F1, F2, L1]],
    T5=    [[A3, G2, G3],
            [A2, H2, H3],
            [A1, I2, I3]],
    T6=    [[J1, J2, J3],
            [K1, K2, K3],
            [G1, H1, I1]].

%movementM(?A,?B) 
%rotacia vertikalny rez
%strany 3 a 5 sa nikdy nezmenia
%pre spatny pohyb len zamenim argumenty
movementM([S1,S2,S3,S4,S5,S6],[T1,T2,S3,T4,S5,T6]) :-
    S1=    [[A1, A2, A3],
            [B1, B2, B3],
            [C1, C2, C3]],
    S2=    [[D1, D2, D3],
            [E1, E2, E3],
            [F1, F2, F3]],
    S4=    [[G1, G2, G3],
            [H1, H2, H3],
            [I1, I2, I3]],
    S6=    [[J1, J2, J3],
            [K1, K2, K3],
            [L1, L2, L3]],
    T1=    [[A1, I2, A3], 
            [B1, H2, B3],
            [C1, G2, C3]],
    T2=    [[D1, A2, D3],
            [E1, B2, E3],
            [F1, C2, F3]],
    T4=    [[G1, L2, G3],
            [H1, K2, H3],
            [I1, J2, I3]],
    T6=    [[J1, D2, J3],
            [K1, E2, K3],
            [L1, F2, L3]].

%movementE(?A,?B)
%rotacia horizontalny rez
%strany 1 a 6 sa nikdy nezmenia
%pre spatny pohyb len zamenim argumenty
movementE([S1,S2,S3,S4,S5,S6],[S1,T2,T3,T4,T5,S6]) :-
    S2=    [[A1, A2, A3],
            [B1, B2, B3],
            [C1, C2, C3]],
    S3=    [[D1, D2, D3],
            [E1, E2, E3],
            [F1, F2, F3]],
    S4=    [[G1, G2, G3],
            [H1, H2, H3],
            [I1, I2, I3]],
    S5=    [[J1, J2, J3],
            [K1, K2, K3],
            [L1, L2, L3]],
    T2=    [[A1, A2, A3], 
            [K1, K2, K3],
            [C1, C2, C3]],
    T3=    [[D1, D2, D3],
            [B1, B2, B3],
            [F1, F2, F3]],
    T4=    [[G1, G2, G3],
            [E1, E2, E3],
            [I1, I2, I3]],
    T5=    [[J1, J2, J3],
            [H1, H2, H3],
            [L1, L2, L3]].

%movementS(?A,?B)
%rotacia top rez
%strany 2 a 4 sa nikdy nezmenia
%pre spatny pohyb len zamenim argumenty
movementS([S1,S2,S3,S4,S5,S6],[T1,S2,T3,S4,T5,T6]) :-
    S1=    [[A1, A2, A3],
            [B1, B2, B3],
            [C1, C2, C3]],
    S3=    [[D1, D2, D3],
            [E1, E2, E3],
            [F1, F2, F3]],
    S5=    [[G1, G2, G3],
            [H1, H2, H3],
            [I1, I2, I3]],
    S6=    [[J1, J2, J3],
            [K1, K2, K3],
            [L1, L2, L3]],
    T1=    [[A1, A2, A3],
            [I2, H2, G2],
            [C1, C2, C3]],
    T3=    [[D1, B1, D3],
            [E1, B2, E3],
            [F1, B3, F3]],
    T5=    [[G1, K1, G3],
            [H1, K2, H3],
            [I1, K3, I3]],
    T6=    [[J1, J2, J3],
            [F2, E2, D2],
            [L1, L2, L3]].

%rotate_clockwise(?A,?B)
%otocenie jednej strany v smere hodinovych ruciciek
%ked chcem rotovat v protismere, tak len zamenim S1 a Z
rotate_clockwise(S1, Z) :-
    S1 =   [[A1, A2, A3],
            [B1, B2, B3],
            [C1, C2, C3]],
    Z =    [[C1, B1, A1],
            [C2, B2, A2],
            [C3, B3, A3]].

%nacita zadany pocet riadkov
%zo suboru input2.pl 
read_lines2([],0).
read_lines2(Ls,N) :-
	N > 0,
	read_line(L,_),
	N1 is N-1,
	read_lines2(LLs, N1),
	Ls = [L|LLs].

%zo suboru input2.pl 
read_line(L,C) :- 
	get_char(C),
	(isEOFEOL(C), L = [], !;
		read_line(LL,_),% atom_codes(C,[Cd]),
		[C|LL] = L).

%testuje znak na EOF nebo LF
%zo suboru input2.pl 
isEOFEOL(C) :-
	C == end_of_file;
	(char_code(C,Code), Code==10).
