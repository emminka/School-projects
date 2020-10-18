/*  IZP projekt c.2
    Iteracne vypocty
    Emma Krompascikova*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define I0 1e-12
#define UT 0.0258563
#define MAX_POCET_ITER 300 // max pocet iteracii proti zacykleniu

double vypocet(double U0, double R, double Up) {  // funkcia na vypocet podla vzorca
    return I0 * (exp(Up/UT)- 1) - (U0 - Up) / R; // exp je e^x
}

double diode(double U0, double R, double EPS) {
    double lava = 0;
    double prava = U0;
    double stred = (lava + prava) / 2; // artimeticky priemer
    double hodnota = vypocet(U0, R, stred);
    int i = 0; // pocet iteracii, ktore prebehnu
    while (fabs(hodnota) > EPS) {    // metoda bisekcie; zistujem, ci som v pozadovanej presnosti
        if (i++ > MAX_POCET_ITER){
            break;
        }
        if (vypocet(U0, R, lava) * vypocet(U0, R, stred) < 0) { // vyber praveho alebo laveho intervalu
            prava = stred;
        }
        else {
            lava = stred;
        }
        stred = (lava + prava) / 2;
        hodnota = vypocet(U0, R, stred); // Shockleyho rovnica
    }

    return stred; 
}

int main(int argc, char* argv[]){
    if (argc != 4) {
        fprintf(stderr, "Zadany nespravny pocet argumentov.\n");
        return -1;
    }

    double U0 = strtod(argv[1], NULL); // string to double
    double R = strtod(argv[2], NULL);
    double EPS = strtod(argv[3], NULL);  
    if ( U0 <= 0|| R <= 0|| EPS <= 0) {
        fprintf(stderr, "error: invalid arguments\n");
        return -1;
    }

    double Up = diode(U0,R,EPS);
    double Ip = I0 * (exp(Up/UT)- 1);   //vzorec na Ip
    printf("Up=%g V\n",Up);
    printf("Ip=%g A\n",Ip);
    
    return 0;
}