// hlavickovy soubor pro praci s bludistem
// ak. rok 2019/20
//author Emma Krompascikova
//login - xkromp00
//date 22.12.2019

#ifndef PROJ3_H
#define PROJ3_H

typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

/*! bitove vahy jednotlivych hranic */
enum borders { BLEFT=0x1, BRIGHT=0x2, BTOP=0x4, BBOTTOM=0x4 };

/*!
*uvolni mapu z pamate
*@param[in] map ukazatel na mapu, ktora sa bude uvolnovat
*/
void free_map(Map *map);

/*!
* nacitanie mapy
* @param[in] filename nazov suboru
* @param[out] map ukazatel na mapu ktora sa nacita
* @post mapa sa nacita
* @return 0 ked je vsetko ok
*/
int load_map(const char *filename, Map *map);


/*!
* zistuje, ci je hranica
* @pre musi byt validna mapa
* @param[in] map ukazatel na mapu
* @param[in] r cislo riadka
* @param[in] c cislo stlpca
* @param[in] border cislo hranic
* @return true ak je hranica
*/
bool isborder(Map *map, int r, int c, int border);


/*!
*zistuje ci ma policko spodnu hranicu
* @param[in] r cislo riadka
* @param[in] c cislo stlpca
* @return true ak ma spodnu hranicu
*/
bool hasbottom(int r, int c);

/*!
*vracia zaciatocnu hranicu
* @pre musi byt validna mapa
* @param[in] map ukazatel na mapu
* @param[in] r cislo riadka
* @param[in] c cislo stlpca
* @param[in] leftright vybera ci ide podla pravej alebo lavej ruky
* @return cislo prvej hranice od ktorej pojdem do bludiska
*/
int start_border(Map *map, int r, int c, int leftright);


/*!
* skontroluje ci je mapa v poriadku
* @pre musi byt validna mapa
* @param[in] map ukazatel na mapu
* @return 0 ak je mapa v pohode
*/
int check_map(Map *map);

/*!
* zavola load_map a check_map a hend ich aj skontroluje
* @param[in] filename nazov suboru
* @return 0 aj jej vsetko v poriadku
*/
int load_and_check_map(const char *filename, Map *map);

/*!
* zistujeme ci sme mimo mapy
* @param[in] map ukazatel na mapu
* @param[in] r cislo riadka
* @param[in] c cislo stlpca
* @return true ak je vonku mapy
*/
bool is_out(Map *map, int r, int c);

/*! najde cestu a vypise
* @param[in] map ukazatel na mapu
* @param[in] r cislo riadka
* @param[in] c cislo stlpca
* @param[in] leftright vybera ci ide podla pravej alebo lavej ruky
*/
void print_path(Map *map, int r, int c, int leftright);
#endif