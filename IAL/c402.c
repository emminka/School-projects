//emmka krompascikova
/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS
**                                     Implementace: Petr Pøikryl, prosinec 1994
**                                           Úpravy: Petr Pøikryl, listopad 1997
**                                                     Petr Pøikryl, kvìten 1998
**			  	                        Pøevod do jazyka C: Martin Tuèek, srpen 2005
**                                         Úpravy: Bohuslav Køena, listopad 2009
**                                                 Karel Masaøík, øíjen 2013
**                                                 Radek Hranický 2014-2018
**
** S využitím dynamického pøidìlování pamìti, implementujte NEREKURZIVNÌ
** následující operace nad binárním vyhledávacím stromem (pøedpona BT znamená
** Binary Tree a je u identifikátorù uvedena kvùli možné kolizi s ostatními
** pøíklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vložení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní prùchod typu pre-order
**     BTInorder ....... nerekurzivní prùchod typu in-order
**     BTPostorder ..... nerekurzivní prùchod typu post-order
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech funkcí, které využívají nìkterý z prùchodù stromem, implementujte
** pomocnou funkci pro nalezení nejlevìjšího uzlu v podstromu.
**
** Pøesné definice typù naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na nìj je typu tBTNodePtr. Jeden uzel obsahuje položku int Cont,
** která souèasnì slouží jako užiteèný obsah i jako vyhledávací klíè
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Pøíklad slouží zejména k procvièení nerekurzivních zápisù algoritmù
** nad stromy. Než zaènete tento pøíklad øešit, prostudujte si dùkladnì
** principy pøevodu rekurzivních algoritmù na nerekurzivní. Programování
** je pøedevším inženýrská disciplína, kde opìtné objevování Ameriky nemá
** místo. Pokud se Vám zdá, že by nìco šlo zapsat optimálnìji, promyslete
** si všechny detaily Vašeho øešení. Povšimnìte si typického umístìní akcí
** pro rùzné typy prùchodù. Zamyslete se nad modifikací øešených algoritmù
** napøíklad pro výpoèet poètu uzlù stromu, poètu listù stromu, výšky stromu
** nebo pro vytvoøení zrcadlového obrazu stromu (pouze popøehazování ukazatelù
** bez vytváøení nových uzlù a rušení starých).
**
** Pøi prùchodech stromem použijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodušení práce máte pøedem pøipraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte
** Pozor! Je tøeba správnì rozlišovat, kdy použít dereferenèní operátor *
** (typicky pøi modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (napø. pøi vyhledávání). V tomto pøíkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocná funkce, kterou budete volat pøi prùchodech stromem pro zpracování
** uzlu urèeného ukazatelem Ptr. Tuto funkci neupravujte.
**/

	if (Ptr==NULL)
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  else
    printf("Výpis hodnoty daného uzlu> %d\n",Ptr->Cont);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;
}

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{
                 /* Pøi implementaci v poli mùže dojít k pøeteèení zásobníku. */
  if (S->top==MAXSTACK)
    printf("Chyba: Došlo k pøeteèení zásobníku s ukazateli!\n");
  else {
		S->top++;
		S->a[S->top]=ptr;
	}
}

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a souèasnì vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem zpùsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podteèení zásobníku s ukazateli!\n");
		return(NULL);
	}
	else {
		return (S->a[S->top--]);
	}
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zásobníku.
**/

	S->top = 0;
}

void SPushB (tStackB *S,bool val) {
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
                 /* Pøi implementaci v poli mùže dojít k pøeteèení zásobníku. */
	if (S->top==MAXSTACK)
		printf("Chyba: Došlo k pøeteèení zásobníku pro boolean!\n");
	else {
		S->top++;
		S->a[S->top]=val;
	}
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstraní prvek z vrcholu zásobníku a souèasnì vrátí jeho hodnotu.
**/
                            /* Operace nad prázdným zásobníkem zpùsobí chybu. */
	if (S->top==0) {
		printf("Chyba: Došlo k podteèení zásobníku pro boolean!\n");
		return(NULL);
	}
	else {
		return(S->a[S->top--]);
	}
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat.
*/

void BTInit (tBTNodePtr *RootPtr)	{
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze pøed prvním použitím binárního
** stromu, protože neuvolòuje uzly neprázdného stromu (a ani to dìlat nemùže,
** protože pøed inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zrušení binárního stromu slouží procedura BTDisposeTree.
**
** Všimnìte si, že zde se poprvé v hlavièce objevuje typ ukazatel na ukazatel,
** proto je tøeba pøi práci s RootPtr použít dereferenèní operátor *.
**/
	*RootPtr = NULL;	
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vloží do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytváøený strom jako binární vyhledávací strom,
** kde uzly s hodnotou menší než má otec leží v levém podstromu a uzly vìtší
** leží vpravo. Pokud vkládaný uzel již existuje, neprovádí se nic (daná hodnota
** se ve stromu mùže vyskytnout nejvýše jednou). Pokud se vytváøí nový uzel,
** vzniká vždy jako list stromu. Funkci implementujte nerekurzivnì.
**/
	tBTNodePtr novyUzol = *RootPtr, rodic = NULL;
	int obsahRodica;
	
	while (novyUzol != NULL) {
		obsahRodica = novyUzol->Cont;

		if (Content < obsahRodica) {
			rodic = novyUzol;
			novyUzol = novyUzol->LPtr;
		}
		else if (Content > obsahRodica) {
			rodic = novyUzol;
			novyUzol = novyUzol->RPtr;
		}
		else {
			// taku hodnotu uz mame v strome
			return;
		}
	}

	novyUzol = malloc(sizeof(struct tBTNode));
	novyUzol->Cont = Content;
	novyUzol->LPtr = NULL;
	novyUzol->RPtr = NULL;

	if (rodic != NULL) {
		if (Content < obsahRodica) {
			rodic->LPtr = novyUzol;
		}
		else {
			rodic->RPtr = novyUzol;
		}
	}
	else {
		*RootPtr = novyUzol;
	}
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*   -----------------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìjší uzel.
**
** Pøi prùchodu Preorder navštívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na nì is uložíme do zásobníku.
**/
	while (ptr != NULL) {
		BTWorkOut(ptr);
		SPushP(Stack, ptr->RPtr);
		ptr = ptr->LPtr;
	}
}

void BTPreorder (tBTNodePtr RootPtr)	{
/*   ----------
** Prùchod stromem typu preorder implementovaný nerekurzivnì s využitím funkce
** Leftmost_Preorder a zásobníku ukazatelù. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut().
**/
	if (RootPtr != NULL) {
		tStackP *zasobnik = malloc(sizeof(tStackP));
		SInitP(zasobnik);

		SPushP(zasobnik, RootPtr);

		while (!SEmptyP(zasobnik)) {
			tBTNodePtr uzol = STopPopP(zasobnik);

			Leftmost_Preorder(uzol, zasobnik);
		}

		free(zasobnik);
	}
}


/*                                  INORDER                                   */

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìjší uzel.
**
** Pøi prùchodu Inorder ukládáme ukazatele na všechny navštívené uzly do
** zásobníku.
**/
	while (ptr != NULL) {
		SPushP(Stack, ptr);
		ptr = ptr->LPtr;
	}
}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** Prùchod stromem typu inorder implementovaný nerekurzivnì s využitím funkce
** Leftmost_Inorder a zásobníku ukazatelù. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut().
**/
	if (RootPtr != NULL) {
		tStackP *zasobnik = malloc(sizeof(tStackP));
		SInitP(zasobnik);

		Leftmost_Inorder(RootPtr, zasobnik);

		while (!SEmptyP(zasobnik)) {
			tBTNodePtr uzol = STopPopP(zasobnik);

			BTWorkOut(uzol);

			Leftmost_Inorder(uzol->RPtr, zasobnik);
		}

		free(zasobnik);
	}
}

/*                                 POSTORDER                                  */

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìjší uzel.
**
** Pøi prùchodu Postorder ukládáme ukazatele na navštívené uzly do zásobníku
** a souèasnì do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** navštíven poprvé a že se tedy ještì nemá zpracovávat.
**/
	while (ptr != NULL) {
		SPushB(StackB, true);

		SPushP(StackP, ptr);

		ptr = ptr->LPtr;
	}	

}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Prùchod stromem typu postorder implementovaný nerekurzivnì s využitím funkce
** Leftmost_Postorder, zásobníku ukazatelù a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut().
**/
	if (RootPtr != NULL) {
		tStackB *zasobnikB = malloc(sizeof(tStackB)); // booleany
		tStackP *zasobnikP = malloc(sizeof(tStackP)); // pointery na uzly

		SInitB(zasobnikB);
		SInitP(zasobnikP);

		Leftmost_Postorder(RootPtr, zasobnikP, zasobnikB);

		while(!SEmptyP(zasobnikP)) {
			tBTNodePtr aktualny = STopPopP(zasobnikP);

			if (STopPopB(zasobnikB) == true) { // este sme uzol nepresli
				SPushP(zasobnikP, aktualny);
				SPushB(zasobnikB, false);

				Leftmost_Postorder(aktualny->RPtr, zasobnikP, zasobnikB);
			}
			else { // uz prechadzame uzol druhykrat
				BTWorkOut(aktualny);
			}
		}
		
		free(zasobnikB);
		free(zasobnikP);
	}
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** Zruší všechny uzly stromu a korektnì uvolní jimi zabranou pamì.
**
** Funkci implementujte nerekurzivnì s využitím zásobníku ukazatelù.
**/

	if (*RootPtr != NULL) {
		tStackP *zasobnik = malloc(sizeof(tStackP));
		SInitP(zasobnik);

		SPushP(zasobnik, *RootPtr);

		tBTNodePtr lavySyn, aktualny;

		while (!SEmptyP(zasobnik)) {
			aktualny = STopPopP(zasobnik);

			while (aktualny != NULL) {
				lavySyn = aktualny->LPtr;

				if (aktualny->RPtr != NULL) {
					SPushP(zasobnik, aktualny->RPtr);
				}
				
				free(aktualny);
				aktualny = lavySyn;
			}
		}

		*RootPtr = NULL;

		free(zasobnik);
	}
}

/* konec c402.c */ //super