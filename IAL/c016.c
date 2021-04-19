//emmka krompascikova
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Pøikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masaøík, øíjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvoøete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitnì øetìzenými synonymy. Tabulka je implementována polem
** lineárních seznamù synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku pøed prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištìní pøítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... pøeètení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typù naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelù na položky, jež obsahují složky
** klíèe 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Pøi implementaci funkcí
** uvažujte maximální rozmìr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimnìte si
** zpùsobu pøedávání parametrù a zamyslete se nad tím, zda je možné parametry
** pøedávat jiným zpùsobem (hodnotou/odkazem) a v pøípadì, že jsou obì
** možnosti funkènì pøípustné, jaké jsou výhody èi nevýhody toho èi onoho
** zpùsobu.
**
** V pøíkladech jsou použity položky, kde klíèem je øetìzec, ke kterému
** je pøidán obsah - reálné èíslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíè a pøidìlit
** mu index v rozmezí 0..HTSize-1.  V ideálním pøípadì by mìlo dojít
** k rovnomìrnému rozptýlení tìchto klíèù po celé tabulce.  V rámci
** pokusù se mùžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode(tKey key)
{
	int retval = 1;
	int keylen = strlen(key);
	for (int i = 0; i < keylen; i++)
		retval += key[i];
	return (retval % HTSIZE);
}

/*
** Inicializace tabulky s explicitnì zøetìzenými synonymy.  Tato procedura
** se volá pouze pøed prvním použitím tabulky.
*/

void htInit(tHTable *ptrht)
{

	for (int i = 0; i < HTSIZE; i++)
	{
		(*ptrht)[i] = NULL;
	}
}

/* TRP s explicitnì zøetìzenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíèe key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem *htSearch(tHTable *ptrht, tKey key)
{

	int hash = hashCode(key); //vieme na ktorom mieste v tabulke budeme
	tHTItem *obsahuje = (*ptrht)[hash];
	while (obsahuje != NULL)
	{
		if (key == obsahuje->key)
		{
			return obsahuje;
		}
		else
		{
			obsahuje = obsahuje->ptrnext;
		}
	}
	return NULL;
}

/*
** TRP s explicitnì zøetìzenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíèem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemùže být prvek se stejným
** klíèem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíè se již v tabulce nachází, aktualizujte jeho datovou èást.
**
** Využijte døíve vytvoøenou funkci htSearch.  Pøi vkládání nového
** prvku do seznamu synonym použijte co nejefektivnìjší zpùsob,
** tedy proveïte.vložení prvku na zaèátek seznamu.
**/

void htInsert(tHTable *ptrht, tKey key, tData data)
{
	int hash = hashCode(key);
	tHTItem *nova, *hladana;
	hladana = htSearch(ptrht, key);
	if (hladana == NULL) //nie je este v tabulke
	{
		nova = malloc(sizeof(tHTItem));
		nova->key = key;				//dame novy kluc
		nova->data = data;				//dame nove data
		nova->ptrnext = (*ptrht)[hash]; //nastavime novy ukazatel
		(*ptrht)[hash] = nova;			//nastavim položku èo bude porva
	}
	else
	{
		hladana->data = data; //aktualizujem data ked uz tam je
	}
}

/*
** TRP s explicitnì zøetìzenými synonymy.
** Tato funkce zjišuje hodnotu datové èásti položky zadané klíèem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkèní hodnota NULL
**
** Využijte døíve vytvoøenou funkci HTSearch.
*/

tData *htRead(tHTable *ptrht, tKey key)
{
	tHTItem *hladana;
	hladana = htSearch(ptrht, key);
	if (hladana != NULL) //nasli sme ju
	{
		return &hladana->data;
	}
	else
	{
		return NULL;
	}
}

/*
** TRP s explicitnì zøetìzenými synonymy.
** Tato procedura vyjme položku s klíèem key z tabulky
** ptrht.  Uvolnìnou položku korektnì zrušte.  Pokud položka s uvedeným
** klíèem neexistuje, dìlejte, jako kdyby se nic nestalo (tj. nedìlejte
** nic).
**
** V tomto pøípadì NEVYUŽÍVEJTE døíve vytvoøenou funkci HTSearch.
*/

void htDelete(tHTable *ptrht, tKey key)
{
	int hash = hashCode(key);
	tHTItem *aktualny = (*ptrht)[hash];
	tHTItem *otec = NULL;

	while (aktualny != NULL) {
		if (aktualny->key == key) {
			if (otec == NULL) {
				(*ptrht)[hash] = aktualny->ptrnext;
			}
			else {
				otec->ptrnext = aktualny->ptrnext;
			}	
			free(aktualny);
			break;
		}
		else {
			otec = aktualny;
			aktualny = aktualny->ptrnext;
		}
	}
}

/* TRP s explicitnì zøetìzenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektnì uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do poèáteèního stavu.
*/

void htClearAll(tHTable *ptrht)
{
	tHTItem *aktualny, *syn;

	for (int i = 0; i < HTSIZE; i++)
	{
		aktualny = (*ptrht)[i];

		while (aktualny != NULL) //prechadzame celu
		{
			// syn = (*ptrht)[i];
			syn = aktualny->ptrnext;
			free(aktualny);
			aktualny = syn;
		}

		(*ptrht)[i] = NULL;
	}
}
///super