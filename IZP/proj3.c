#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct 
{
    int rows;
    int cols;
    unsigned char *cells;
} Map;

Map nacitaj(char* name){ //funkcia na nacitanie mapy
    FILE* file = fopen(name, "r");
    Map mapa;
    char nacitane;
    char *r,*c;
    int row_number = 0;
    int col_number = 0;

    fscanf(file, "%d %d", r, c);
    int rows = strtol(r,NULL,10);
    int cols = strtol(c,NULL,10);
    mapa.rows = rows;
    mapa.cols = cols;
    mapa.cells = malloc(sizeof(char) * rows * cols);
    while  ((nacitane=getc(file)) != EOF){
        if (nacitane == ' '){
            continue;
        }
        else if (nacitane == '\n'){
            row_number++;
            col_number=0;
            continue;
        }
        mapa.cells[row_number * cols + col_number]= nacitane;
    }
    return mapa;
}

void vypisat_mapu (Map* mapa, int rows, int cols,int cells){
    mapa->rows = rows;
    mapa->cols = cols;
    mapa->cells = cells;
    cells = rows*cols;
    printf("stlpce %d\n", rows);
    printf("riadky %d\n",cols);
    int i ;
    for(i=0 ; i < cells; i++){
        printf("cells %d\n", cells);
    }
}

bool isborder(Map *mapa, int r, int c, int border){
    int mask = 0;
    if(border == 0){
        mask = 1;
    }
    else if (border == 1){
        mask = 2;
    } 
    else if (border == 2){
        mask = 4;
    }
    if (mapa->cells[r*mapa->cols+c]&mask == 0){
        return 1;
        }
        else
        {
            return 0;
        }
        
}

void inic ( Map * mapa, strcmp(argv[4]){
    //strtok
    //strtol
    malloc(rows*cols*sizeof(char));
    File * f = fopen(nazov);
    mapa->rows = 0;
    mapa->cols = 0;
    fgets (f,100);
}

/*int start_border(Map *map, int r, int c, int leftright){

}*/

int main (int argc, char* argv[])
{
    int start_row;
    int start_col;
    //podmienky argumentov
    if (argc == 4 || argc > 5){
        fprintf(stderr,"zly pocet argumentov\n");
    }

    if (argc == 1){
        fprintf(stderr,"Chybaju argumenty\n");
        return -1;
    }

    if(argc == 2){
        if (strcmp(argv[1],"--help")==0){
            printf
            ("Pristupne argmenty su:\n"
            "--help - vypise pomoc\n"
            "--test <file> - skontroluje platnosť bludiska\n"
            "--rpath <R> <C> <file> - najde cestu pomocou pravidla pravej ruky\n"
            "--lpath <R> <C> <file> - najde cestu pomocou pravidla lavej ruky\n");
        }
        else {
            fprintf(stderr,"zle zadany druhy argument\n");
            return -1;
        }
    }

    if(argc == 3){
        if (strcmp(argv[1],"--test")==0){
            FILE* file = fopen(argv[2], "r");
            // dorobit
        }
        else {
            fprintf(stderr,"zle zadany druhy argument z troch\n");
            return -1;
        }
    }

    if (argc == 5){
        if (strcmp(argv[1],"--rpath")==0){
            start_row = strtol(argv[2],NULL,10); //string to long, 10tkova sustava
            start_col = strtol(argv[3],NULL,10);
            FILE* file = fopen(argv[4], "r");
            nacitaj(file);
            
        } 
        else if (strcmp(argv[1],"--lpath")==0){
            start_row = strtol(argv[2],NULL,10);
            start_col = strtol(argv[3],NULL,10);
            FILE* file = fopen(argv[4], "r");
        } 
        else {
            fprintf(stderr, "Zle argumenty\n");
            return -1;
        }
    }
    return 0;
}