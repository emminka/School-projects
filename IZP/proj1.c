/* IZP projekt c.1
   Emma Krompascikova
   Vyhladavanie pomocou cisel v telefonnom zozname */

#include <stdio.h>
#include <string.h> //strlen strstr strcpy
#include <ctype.h> //tolower

#define MAX_DLZKA_RIADKU 101

int zhoda(char postupnost[], char meno[], char cislo[]) 
{
    if (strstr(cislo, postupnost) != NULL) return 1; // hladam zadane cislo v tel. cisle

    char p,c;
    char konvertovane[MAX_DLZKA_RIADKU];
    int dlzka_mena = strlen(meno);

    //zmena pismenok na cislice
    for (int i = 0; i < dlzka_mena; i++) // pre kazde pismenko v mene
    {
        p = meno[i];
        p = tolower(p); //zmena velkeho pismenka na male
        
        switch (p) 
        {
          case 'a':
          case 'b':
          case 'c':
            c='2';
            break;
          case 'd':
          case 'e':
          case 'f':
            c='3';
            break;
          case 'g':
          case 'h':
          case 'i':
            c='4';
            break;
          case 'j':
          case 'k':
          case 'l':
            c='5';
            break;
          case 'm':      
          case 'n':
          case 'o':
            c='6';
            break;
          case 'p':
          case 'q':
          case 'r':
          case 's':
            c='7';
            break;
          case 't':
          case 'u':
          case 'v':
            c='8';
            break;
          case 'w':
          case 'x':
          case 'y':
          case 'z':
            c='9';
            break;
          case '+':
            c='0';
            break;
        default: //ak ani jedno nesedi
            c=' ';
            break;                
        } 

        konvertovane[i] = c;
    }

    if (strstr(konvertovane, postupnost) != NULL) return 1; // hladam prekonvertovane meno v zadanom cisle
     
    return 0;
}

    

int main(int argc, char* argv[]) // argc je pocet argumentov, argv je ich zoznam
{   
    char postupnost[MAX_DLZKA_RIADKU]; // cislice, ktore uzivatel zadal
    char meno[MAX_DLZKA_RIADKU];
    char cislo[MAX_DLZKA_RIADKU];
    int vypisalo_sa_nieco = 0; // naslo aspon jeden kontakt
    
    if (argc != 1)  // uzivatel nieco zadal
    {
        strcpy(postupnost, argv[1]); //priradime do postupnosti
    }
    
    if (argc > 2)
    {
       fprintf(stderr, "Zadanych privela argumentov.\n");
       return -1;  // chybovy stav
    }

    int zhodne;   
    while (fgets(meno, MAX_DLZKA_RIADKU, stdin) != NULL) //nacitanie po koniec vstupu
    {    
        fgets(cislo, MAX_DLZKA_RIADKU, stdin);
        zhodne = zhoda(postupnost, meno, cislo); 
        
        if (zhodne == 1 || argc == 1)
        {          
            vypisalo_sa_nieco = 1;
            strcpy(meno, strtok(meno, "\n")); //odstrani z mena \n
            printf("%s, %s",meno,cislo);   
        }    
    }

    if (vypisalo_sa_nieco == 0) //ak sa nic nenaslo
    {    
        printf("Not found");
    } 

    printf("\n");
    return 0;
}   