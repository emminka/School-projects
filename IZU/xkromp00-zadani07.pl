/* xkromp00 */

/* Prazdny zoznam má rozdiel 0 */
uloha7([], 0).

/* */
uloha7([Kladne|Zvysok], Rozdiel) :- Kladne > 0, uloha7(Zvysok, RozdielZvysku), Rozdiel is RozdielZvysku + 1.

uloha7([Zaporne|Zvysok], Rozdiel) :- Zaporne < 0, uloha7(Zvysok, RozdielZvysku), Rozdiel is RozdielZvysku - 1.

uloha7([0|Zvysok], Rozdiel) :- uloha7(Zvysok, Rozdiel).
