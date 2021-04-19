//emmka krompascikova
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvoøil: Petr Pøikryl, listopad 1994
**                                         Úpravy: Andrea Nìmcová, prosinec 1995
**                                                      Petr Pøikryl, duben 1996
**                                                   Petr Pøikryl, listopad 1997
**                                  Pøevod do jazyka C: Martin Tuèek, øíjen 2005
**                                         Úpravy: Bohuslav Køena, listopad 2009
**                                                 Karel Masaøík, øíjen 2013
**                                                 Radek Hranickı 2014-2018
**
** Implementujte rekurzivním zpùsobem operace nad binárním vyhledávacím
** stromem (BVS; v angliètinì BST - Binary Search Tree).
**
** Klíèem uzlu stromu je jeden znak (obecnì jím mùe bıt cokoliv, podle
** èeho se vyhledává). Uiteènım (vyhledávanım) obsahem je zde integer.
** Uzly s menším klíèem leí vlevo, uzly s vìtším klíèem leí ve stromu
** vpravo. Vyuijte dynamického pøidìlování pamìti.
** Rekurzivním zpùsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíèem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadanım klíèem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován koøenovım ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíè (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levı a pravı podstrom (LPtr a RPtr). Pøesnou definici typù
** naleznete v souboru c401.h.
**
** Pozor! Je tøeba správnì rozlišovat, kdy pouít dereferenèní operátor *
** (typicky pøi modifikaci) a kdy budeme pracovat pouze se samotnım ukazatelem
** (napø. pøi vyhledávání). V tomto pøíkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, pouijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit(tBSTNodePtr *RootPtr)
{
	/*   -------
** Funkce provede poèáteèní inicializaci stromu pøed jeho prvním pouitím.
**
** Ovìøit, zda byl ji strom pøedanı pøes RootPtr inicializován, nelze,
** protoe pøed první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor vyuívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to pøed vlastní prací s BVS. Provedení
** inicializace nad neprázdnım stromem by toti mohlo vést ke ztrátì pøístupu
** k dynamicky alokované pamìti (tzv. "memory leak").
**
** Všimnìte si, e se v hlavièce objevuje typ ukazatel na ukazatel.
** Proto je tøeba pøi pøiøazení pøes RootPtr pouít dereferenèní operátor *.
** Ten bude pouit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/

	*RootPtr = NULL;
}

int BSTSearch(tBSTNodePtr RootPtr, char K, int *Content)
{
	/*  ---------
** Funkce vyhledá uzel v BVS s klíèem K.
**
** Pokud je takovı nalezen, vrací funkce hodnotu TRUE a v promìnné Content se
** vrací obsah pøíslušného uzlu.´Pokud pøíslušnı uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah promìnné Content není definován (nic do ní proto
** nepøiøazujte).
**
** Pøi vyhledávání v binárním stromu bychom typicky pouili cyklus ukonèenı
** testem dosaení listu nebo nalezení uzlu s klíèem K. V tomto pøípadì ale
** problém øešte rekurzivním volání této funkce, pøièem nedeklarujte ádnou
** pomocnou funkci.
**/
	if (RootPtr != NULL)
	{
		if (RootPtr->Key == K)
		{
			*Content = RootPtr->BSTNodeCont;
			return TRUE;
		}
		else if (RootPtr->Key != K)
		{
			if (RootPtr->Key < K)
			{
				return(BSTSearch(RootPtr->RPtr,K, Content));

			}
			else if (RootPtr->Key > K)
			{
				return(BSTSearch(RootPtr->LPtr,K, Content));

			}
			
		}
	}return FALSE;
}

void BSTInsert(tBSTNodePtr *RootPtr, char K, int Content)
{
	/*   ---------
** Vloí do stromu RootPtr hodnotu Content s klíèem K.
**
** Pokud ji uzel se zadanım klíèem ve stromu existuje, bude obsah uzlu
** s klíèem K nahrazen novou hodnotou. Pokud bude do stromu vloen novı
** uzel, bude vloen vdy jako list stromu.
**
** Funkci implementujte rekurzivnì. Nedeklarujte ádnou pomocnou funkci.
**
** Rekurzivní implementace je ménì efektivní, protoe se pøi kadém
** rekurzivním zanoøení ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto pøípadì byla efektivnìjší jak z hlediska
** rychlosti, tak z hlediska pamìovıch nárokù. Zde jde ale o školní
** pøíklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/
	if (*RootPtr != NULL) //existuje
	{
		if (K < (*RootPtr)->Key)
		{
			BSTInsert(&(*RootPtr)->LPtr, K, Content);
		}
		else
		{
			if (K > (*RootPtr)->Key)
			{
				BSTInsert(&(*RootPtr)->RPtr, K, Content);
			}
			else
			{
				(*RootPtr)->BSTNodeCont = Content; //if (K == (*RootPtr)->Key)
			}
		}
	}
	else // (*RootPtr == NULL)
	{
		*RootPtr = malloc(sizeof(struct tBSTNode));
		(*RootPtr)->BSTNodeCont = Content;
		(*RootPtr)->Key = K;
		(*RootPtr)->LPtr = NULL;
		(*RootPtr)->RPtr = NULL;
	}
}

void ReplaceByRightmost(tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr)
{
	/*   ------------------
** Pomocná funkce pro vyhledání, pøesun a uvolnìní nejpravìjšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude pøesunuta hodnota
** nejpravìjšího uzlu v podstromu, kterı je urèen ukazatelem RootPtr.
** Pøedpokládá se, e hodnota ukazatele RootPtr nebude NULL (zajistìte to
** testováním pøed volání této funkce). Tuto funkci implementujte rekurzivnì.
**
** Tato pomocná funkce bude pouita dále. Ne ji zaènete implementovat,
** pøeètìte si komentáø k funkci BSTDelete().
**/
	if ((*RootPtr)->RPtr == NULL)
	{
		PtrReplaced->Key = (*RootPtr)->Key;
		PtrReplaced->BSTNodeCont = (*RootPtr)->BSTNodeCont;

		tBSTNodePtr na_vymazanie = *RootPtr;

		*RootPtr = (*RootPtr)->LPtr;

		free(na_vymazanie);
	}
	else
	{
		ReplaceByRightmost(PtrReplaced, &(*RootPtr)->RPtr); //rekurzivne volam
	}
}

void BSTDelete(tBSTNodePtr *RootPtr, char K)
{
	/*   ---------
** Zruší uzel stromu, kterı obsahuje klíè K.
**
** Pokud uzel se zadanım klíèem neexistuje, nedìlá funkce nic.
** Pokud má rušenı uzel jen jeden podstrom, pak jej zdìdí otec rušeného uzlu.
** Pokud má rušenı uzel oba podstromy, pak je rušenı uzel nahrazen nejpravìjším
** uzlem levého podstromu. Pozor! Nejpravìjší uzel nemusí bıt listem.
**
** Tuto funkci implementujte rekurzivnì s vyuitím døíve deklarované
** pomocné funkce ReplaceByRightmost.
**/
	if (*RootPtr != NULL)
	{
		if ((*RootPtr)->Key > K) //rušeny je v lavom podstrome
		{
			BSTDelete(&(*RootPtr)->LPtr, K);
		}
		else
		{
			if ((*RootPtr)->Key < K) //rušeny je v pravom podstrome
			{
				BSTDelete(&(*RootPtr)->RPtr, K);
			}
			else
			{
				if ((*RootPtr)->LPtr == NULL && (*RootPtr)->RPtr == NULL) //nema syna
				{
					free(*RootPtr);
					*RootPtr = NULL;
				}
				else // (*RootPtr)->Key == K
				{
					if ((*RootPtr)->LPtr != NULL && (*RootPtr)->RPtr != NULL) //ma oba podstromy
					{
						ReplaceByRightmost((*RootPtr), &(*RootPtr)->LPtr);
					}
					else //ma iba jeden podstrom
					{
						tBSTNodePtr jedno_bobo;
						jedno_bobo = *RootPtr;
						if ((*RootPtr)->LPtr != NULL) //nema laveho synka
						{
							*RootPtr = (*RootPtr)->LPtr;
							free(jedno_bobo);
						}
						else if ((*RootPtr)->RPtr != NULL) //nema praveho synka
						{ 
							
							*RootPtr = (*RootPtr)->RPtr;
							free(jedno_bobo);
						}
					}
				}
			}
		}
	}
}

void BSTDispose(tBSTNodePtr *RootPtr)
{
	/*   ----------
** Zruší celı binární vyhledávací strom a korektnì uvolní pamì.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivnì bez deklarování pomocné
** funkce.
**/
	if (*RootPtr != NULL)
	{
		BSTDispose(&(*RootPtr)->LPtr);
		BSTDispose(&(*RootPtr)->RPtr);

		free(*RootPtr);
		*RootPtr = NULL;
	}
}

/* konec c401.c */
//super