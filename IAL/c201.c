/* Emma Krompascikova
IAL Domaca uloha c.1
oktober 2020 */
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      DisposeList ... zrušení všech prvků seznamu,
**      InitList ...... inicializace seznamu před prvním použitím,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int errflg;
int solved;

void Error()
{
    /*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE; /* globální proměnná -- příznak chyby */
}

void InitList(tList *L)
{
    /*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->Act = NULL;
    L->First = NULL;

    //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DisposeList(tList *L)
{
    /*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
    if (L->First == NULL) //zistujem ci nie je list prazdny
        return;
    tElemPtr prvok;          //pomocny prvok kam budem davat prvky
    while (L->First != NULL) //cyklicky rusim postupne prvky
    {
        prvok = L->First; //nastavim pomocny prvok na prvy
        L->First = prvok->ptr;
        free(prvok);
    }
    L->Act = NULL; //uvedenie zoznamu do povodneho stavu
    L->First = NULL;

    // solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

void InsertFirst(tList *L, int val)
{
    /*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
    tElemPtr prvok;
    prvok = malloc(sizeof(struct tElem));
    if (prvok == NULL)
    {
        Error();
        return;
    }
    prvok->data = val;     //nahram obsah val-u do pomocneho prvku
    prvok->ptr = L->First; //ukazatel na stary prvy prvok
    L->First = prvok;      //prvy prvok bude novym prvkom

    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

void First(tList *L)
{
    /*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
    L->Act = L->First;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

void CopyFirst(tList *L, int *val)
{
    /*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
    if (L->First == NULL)
    {
        Error();
        return;
    }
    *val = L->First->data;

    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

void DeleteFirst(tList *L)
{
    /*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
    tElemPtr prvok;
    if (L->First == NULL) //ked je prazdny tak sa nic nedeje
    {
        return;
    }
    if (L->First == L->Act)
    {
        L->Act = NULL;
    }
    prvok = L->First; //podobne ako pri disposelist, ale robim len prvy prvok
    L->First = prvok->ptr;
    free(prvok);

    // solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

void PostDelete(tList *L)
{
    /* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
    if (L->Act == NULL || L->Act->ptr == NULL) //nie je aktivny zoznam ani posledny prvok
    {
        return;
    }
    tElemPtr prvok;
    prvok = L->Act->ptr;
    L->Act->ptr = prvok->ptr;
    free(prvok);

    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

void PostInsert(tList *L, int val)
{
    /*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
    if (L->Act == NULL)
    {
        return;
    }
    tElemPtr prvok;
    prvok = malloc(sizeof(struct tElem));
    if (prvok == NULL)
    {
        Error();
        return;
    }
    prvok->data = val;        //nahram obsah val-u do pomocneho prvku
    prvok->ptr = L->Act->ptr; //ukazatel na aktivny prvok
    L->Act->ptr = prvok;      //prvy prvok bude za aktivnym prvkom

    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

void Copy(tList *L, int *val)
{
    /*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/

    if (L->Act == NULL)
    {
        Error();
        return;
    }

    *val = L->Act->data;
    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

void Actualize(tList *L, int val)
{
    /*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
    if (L->Act == NULL)
    {
        return;
    }
    L->Act->data = val;

    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

void Succ(tList *L)
{
    /*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/

    if (L->First == NULL || L->Act == NULL)
    { //ked je prazdny alebo neaktivny tak sa nic nedeje
        return;
    }
    //iba pre aktivne
    L->Act = L->Act->ptr; // presunie sa aktivny na dalsi prvok

    // solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

int Active(tList *L)
{
    /*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/

    //upravit do 1 riadku
    return (L->Act != NULL);

    //solved = FALSE; /* V případě řešení, smažte tento řádek! */
}

/* Konec c201.c */
