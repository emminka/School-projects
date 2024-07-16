/*
PRL projekt cislo 2
Hra Life v MPI
Emma Krompascikova
xkromp00
LS 2023/2024

riesenie pre implementaciu typu wrap-around
funguje aj pre obdlznikove pole
fixny pocet procesov - 4
kazdy rank pracuje s jednym kvadrantom

napr pre pole 6x6 je rozdelanie pre ranky nasledovne
000111
000111
000111
222333
222333
222333

rank = 0 pracuje s castou oznacenou nulami, rank = 1 pracuje s castou otznacenou jednotkami atd.

napriklad pre pole 4x8 je to teda
00001111
00001111
22223333
22223333

a pre pole 4x4 je to
0011
0011
2233
2233

ocakava sa zadany parny pocet riadkov a stlpcov

riesenie je vypisovane ako ID: <cast mrizky>
pokial je napriklad riesenie
0011
1010
1010
0011
tak na stdout bude vypisane
0: 00
0: 10
1: 11
1: 10
2: 10
2: 00
3: 10
3: 11

bohuzial sa ranky mozu vypisat v zlom poradi, potom si to musite 
sami v hlave ''preskladat'' na spravne miesto

program funguje pre zadany pocet krokov

zobrazene riesenie je vysledok po zadanom pocte krokov*/


#include <stdio.h>
#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

int najdi_riesenie(int **pole,int bod, int i, int j, int rank, int pocet_riadkov, int pocet_stlpcov){ //i je riadok, j je stlec
    if (bod == 0){ //bunka je mrtva
        int count_zivy = 0;
            if(pole[(i-1+pocet_riadkov)%pocet_riadkov][(j-1+pocet_stlpcov)%pocet_stlpcov] == 1){ //NW
                count_zivy++;
            }
            if (pole[(i+pocet_riadkov)%pocet_riadkov][(j-1+pocet_stlpcov)%pocet_stlpcov] == 1){ //N
                count_zivy++;
            }
            if (pole[(i+1+pocet_riadkov)%pocet_riadkov][(j-1+pocet_stlpcov)%pocet_stlpcov] == 1){ //NE
                count_zivy++;
            }
            if (pole[(i-1+pocet_riadkov)%pocet_riadkov][(j+pocet_stlpcov)%pocet_stlpcov] == 1){ //W
                count_zivy++;
            }
            if (pole[(i+1+pocet_riadkov)%pocet_riadkov][(j+pocet_stlpcov)%pocet_stlpcov] == 1){ //E
                count_zivy++;
            }
            if (pole[(i-1+pocet_riadkov)%pocet_riadkov][(j+1+pocet_stlpcov)%pocet_stlpcov] == 1){ //SW
                count_zivy++;
            }
            if (pole[(i+pocet_riadkov)%pocet_riadkov][(j+1+pocet_stlpcov)%pocet_stlpcov] == 1){ //S
                count_zivy++;
            }
            if (pole[(i+1+pocet_riadkov)%pocet_riadkov][(j+1+pocet_stlpcov)%pocet_stlpcov] == 1){ //SE
                count_zivy++;
            }
            
            if (count_zivy == 3){
               return 1; //ozije
            }
            else{
                return 0;//ostava mrtva
            }
    }
    else{ //bunka je ziva
        int count_zivy = 0;
        if(pole[(i-1+pocet_riadkov)%pocet_riadkov][(j-1+pocet_stlpcov)%pocet_stlpcov] == 1){ //NW
            count_zivy++;
        }
        if (pole[(i+pocet_riadkov)%pocet_riadkov][(j-1+pocet_stlpcov)%pocet_stlpcov] == 1){ //N
            count_zivy++;
        }
        if (pole[(i+1+pocet_riadkov)%pocet_riadkov][(j-1+pocet_stlpcov)%pocet_stlpcov] == 1){ //NE
            count_zivy++;
        }
        if (pole[(i-1+pocet_riadkov)%pocet_riadkov][(j+pocet_stlpcov)%pocet_stlpcov] == 1){ //W
            count_zivy++;
        }
        if (pole[(i+1+pocet_riadkov)%pocet_riadkov][(j+pocet_stlpcov)%pocet_stlpcov] == 1){ //E
            count_zivy++;
        }
        if (pole[(i-1+pocet_riadkov)%pocet_riadkov][(j+1+pocet_stlpcov)%pocet_stlpcov] == 1){ //SW
            count_zivy++;
        }
        if (pole[(i+pocet_riadkov)%pocet_riadkov][(j+1+pocet_stlpcov)%pocet_stlpcov] == 1){ //S
            count_zivy++;
        }
        if (pole[(i+1+pocet_riadkov)%pocet_riadkov][(j+1+pocet_stlpcov)%pocet_stlpcov] == 1){ //SE
            count_zivy++;
        }

        if (count_zivy < 2){
            return 0;
        }else if(count_zivy == 2 || count_zivy ==3){
            return 1;
        }
        else if(count_zivy > 3){
            return 0;
        } 
        
    }
    
    //nikdy sa nestane ale warningy ma prinutili
    return -1;
}


void zisti_oblast(int **pole, int **pole_po_iteracii,int rank, int pocet_riadkov, int pocet_stlpcov){
    int konkretny_bod;
    for (int i = 0; i<pocet_riadkov; i++){
        for (int j = 0; j <pocet_stlpcov; j++){ // prvy kvadrant
            if(rank == 0 && i<(pocet_riadkov/2) && (j<pocet_stlpcov/2)){
                konkretny_bod = pole[i][j];
                pole_po_iteracii[i][j] = najdi_riesenie(pole,konkretny_bod,i,j,rank, pocet_riadkov,pocet_stlpcov);
            }
            if(rank == 1 && i<(pocet_riadkov/2) && (j>=pocet_stlpcov/2)){ // druhy kvadrant
                konkretny_bod = pole[i][j];
                pole_po_iteracii[i][j] = najdi_riesenie(pole,konkretny_bod,i,j,rank, pocet_riadkov,pocet_stlpcov);
            }
            if(rank == 2 && i>=(pocet_riadkov/2) && (j<pocet_stlpcov/2)){ // treti kvadrant
                konkretny_bod = pole[i][j];
                pole_po_iteracii[i][j] = najdi_riesenie(pole,konkretny_bod,i,j,rank, pocet_riadkov,pocet_stlpcov);
            }
            if(rank == 3 &&i>=(pocet_riadkov/2) && (j>=pocet_stlpcov/2)){ // stvrty kvadrant
                konkretny_bod = pole[i][j];
                pole_po_iteracii[i][j] = najdi_riesenie(pole,konkretny_bod,i,j,rank, pocet_riadkov,pocet_stlpcov);
            }
        }
    }   
}


int main(int argc, char** argv){

    MPI_Init(NULL, NULL);

    int rank; //cislo procesora (jeho index)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int celkovy_pocet_krokov;
    celkovy_pocet_krokov = atoi(argv[2]);
    int pocet_riadkov;
    int pocet_stlpcov;
    int prislo_cislo;
    int aktualny_krok=0;
    
    while(aktualny_krok <celkovy_pocet_krokov){
    if (rank == 0){        
            pocet_riadkov = 1;
            pocet_stlpcov = 0;
            int tag3 = 0;
            int tag2 = 0;
            int tag1 = 0;
            int tag0r = 0; //receive
            int tag0s = 0; //send
            if (argc != 3){
                    fprintf(stderr, "Invalid number of arguments.\n");
                    return -1;
                }

                // argv[1] je nazov vstupneho suboru
                // argv[2] je pocet krokov na vykonanie

                // predpokladame, ze pocet krokov je kladne cislo
                FILE *file = fopen(argv[1], "r");

                bool stlpce_zratane = false;
                int znak;

                while ((znak = fgetc(file)) != EOF) {
                    if (znak == '\n') {
                        stlpce_zratane = true;
                        pocet_riadkov++;
                    }
                    else {
                        if (!stlpce_zratane) {
                            pocet_stlpcov++;
                        }
                    }
                }

            // pre pole zo suboru
            int **pole = (int **) malloc(pocet_riadkov * sizeof(int *));
            for (int i = 0; i < pocet_riadkov; i++) {
                pole[i] = (int *) malloc(pocet_stlpcov * sizeof(int));
            }
            fseek(file, 0L, SEEK_SET); // vraciam sa kurzorom na zaciatok suboru
            
            // pre druhe pole, do ktoreho budeme zapisovat hodnoty novej generacie
            int **pole_po_iteracii = (int **) malloc(pocet_riadkov * sizeof(int *));
            for (int i = 0; i < pocet_riadkov; i++) {
                pole_po_iteracii[i] = (int *) malloc(pocet_stlpcov * sizeof(int));
            }
            

            //pomocne pole, aby sme si neprepisali hodnoty v povodnom
            int **pole3 = (int **) malloc(pocet_riadkov * sizeof(int *));
            for (int i = 0; i < pocet_riadkov; i++) {
                pole3[i] = (int *) malloc(pocet_stlpcov * sizeof(int));
            }

            // nacitame zo suboru do pola
            for (int i = 0; i < pocet_riadkov; i++) {
                for (int j = 0; j < pocet_stlpcov; j++) {
                    znak = fgetc(file);
                    // ked sme na konci riadku, citame dalsi znak
                    if (znak == '\n') {
                        znak = fgetc(file);
                    }
                    pole[i][j] = znak - '0'; 
                }
            }


            fclose(file);



            if (aktualny_krok != 0){
                //z ranku3
                for (int i = pocet_riadkov/2; i < pocet_riadkov; i++) {
                    for (int j = pocet_stlpcov/2; j < pocet_stlpcov; j++) {
                        MPI_Recv(&prislo_cislo, 1, MPI_INT, 3, tag3, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                        pole3[i][j] = prislo_cislo;
                        tag3++;      
                    }
                }

                //z ranku2
                for (int i = pocet_riadkov/2; i < pocet_riadkov; i++) {
                    for (int j = 0; j < pocet_stlpcov/2; j++) {
                        MPI_Recv(&prislo_cislo, 1, MPI_INT, 2, tag2, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                        pole3[i][j] = prislo_cislo;
                        tag2++;      
                    }
                }

                
                //z ranku1
                for (int i = 0; i < pocet_riadkov/2; i++) {
                    for (int j = pocet_stlpcov/2; j < pocet_stlpcov; j++) {
                        MPI_Recv(&prislo_cislo, 1, MPI_INT, 1, tag1, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                        pole3[i][j] = prislo_cislo;
                        tag1++;      
                    }
                }

                //z ranku0
                for (int i = 0; i < pocet_riadkov/2; i++) {
                    for (int j = 0; j < pocet_stlpcov/2; j++){
                        MPI_Recv(&prislo_cislo, 1, MPI_INT, 0, tag0r, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                        pole3[i][j] = prislo_cislo;
                        tag0r++;      
                    }
                }

                //prepisanie pole3 do pole
                for (int i = 0; i<pocet_riadkov; i++){
                    for (int j = 0; j <pocet_stlpcov; j++){ 
                        pole[i][j] = pole3[i][j];
                }}
                
                //pre vypisanie pole z minulej generacie
                /*printf("pre %d pocet krokv\n",aktualny_krok);
                for (int i = 0; i<pocet_riadkov; i++){
                    for (int j = 0; j <pocet_stlpcov; j++){ 
                        printf("%d",pole[i][j]);
                        }
                        printf("\n");
                }*/

            }

            for (int i = 0; i<pocet_riadkov; i++){
                for (int j = 0; j <pocet_stlpcov; j++){ 
                        MPI_Send(&pole[i][j], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                        MPI_Send(&pole[i][j], 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
                        MPI_Send(&pole[i][j], 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
                    }
                }
            
            zisti_oblast(pole,pole_po_iteracii,0, pocet_riadkov, pocet_stlpcov);

            for (int i = 0; i < pocet_riadkov/2; i++) {
                    if (aktualny_krok == (celkovy_pocet_krokov-1)){
                    printf("%d:", rank);}
                for (int j = 0; j < pocet_stlpcov/2; j++) {
                    if (aktualny_krok == (celkovy_pocet_krokov-1)){
                    printf("%d", pole_po_iteracii[i][j]);}
                    MPI_Send(&pole_po_iteracii[i][j], 1, MPI_INT, 0, tag0s, MPI_COMM_WORLD);
                    tag0s++;
                }
                if (aktualny_krok == (celkovy_pocet_krokov-1)){
                printf("\n");}
            }
            
            // vycistenie pole
            for (int i = 0; i < pocet_riadkov; i++) {
                free(pole[i]);
            }
            free(pole);

            // vycistenie pole_po_iteracii
            for (int i = 0; i < pocet_riadkov; i++) {
                free(pole_po_iteracii[i]);
            }
            free(pole_po_iteracii);

            for (int i = 0; i < pocet_riadkov; i++) {
                free(pole3[i]);
            }
            free(pole3);
        
    }

        MPI_Bcast( &pocet_riadkov, 1, MPI_INT,0, MPI_COMM_WORLD);
        MPI_Bcast( &pocet_stlpcov, 1, MPI_INT,0, MPI_COMM_WORLD);
        MPI_Bcast( &celkovy_pocet_krokov, 1, MPI_INT,0, MPI_COMM_WORLD);

    if (rank==1){
        int tag1 = 0;
        MPI_Request send_request[1];
        // pre pole zo suboru
        int **pole = (int **) malloc(pocet_riadkov * sizeof(int *));
        for (int i = 0; i < pocet_riadkov; i++) {
            pole[i] = (int *) malloc(pocet_stlpcov * sizeof(int));
        }
        
        int **pole_po_iteracii = (int **) malloc(pocet_riadkov * sizeof(int *));
        for (int i = 0; i < pocet_riadkov; i++) {
            pole_po_iteracii[i] = (int *) malloc(pocet_stlpcov * sizeof(int));
        }

        
        int prislo_pole ;

        for (int i = 0; i<pocet_riadkov; i++){
            for (int j = 0; j <pocet_stlpcov; j++){
                MPI_Recv(&prislo_pole, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
                pole[i][j] = prislo_pole;
            }
        }
        zisti_oblast(pole,pole_po_iteracii,1, pocet_riadkov, pocet_stlpcov);

        for (int i = 0; i < pocet_riadkov/2; i++) {
            if (aktualny_krok == (celkovy_pocet_krokov-1)){
            printf("%d:", rank);}
            for (int j = pocet_stlpcov/2; j < pocet_stlpcov; j++) {
                if (aktualny_krok == (celkovy_pocet_krokov-1)){
                printf("%d", pole_po_iteracii[i][j]);}
                MPI_Send(&pole_po_iteracii[i][j], 1, MPI_INT, 0, tag1, MPI_COMM_WORLD);
                tag1++;
            }
            if (aktualny_krok == (celkovy_pocet_krokov-1)){
            printf("\n");}
        }

        // vycistenie pole
        for (int i = 0; i < pocet_riadkov; i++) {
            free(pole[i]);
        }
        free(pole);

        // vycistenie pole_po_iteracii
        for (int i = 0; i < pocet_riadkov; i++) {
            free(pole_po_iteracii[i]);
        }
        free(pole_po_iteracii);

        
    }
    if (rank==2){
        int tag2 = 0;
        MPI_Request send_request[1];
        // pre pole zo suboru
        int **pole = (int **) malloc(pocet_riadkov * sizeof(int *));
        for (int i = 0; i < pocet_riadkov; i++) {
            pole[i] = (int *) malloc(pocet_stlpcov * sizeof(int));
        }
        
        int **pole_po_iteracii = (int **) malloc(pocet_riadkov * sizeof(int *));
        for (int i = 0; i < pocet_riadkov; i++) {
            pole_po_iteracii[i] = (int *) malloc(pocet_stlpcov * sizeof(int));
        }

        
        int prislo_pole;

        for (int i = 0; i<pocet_riadkov; i++){
            for (int j = 0; j <pocet_stlpcov; j++){
                MPI_Recv(&prislo_pole, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
                pole[i][j] = prislo_pole;
            }
        }

        zisti_oblast(pole,pole_po_iteracii,2, pocet_riadkov, pocet_stlpcov);

        for (int i = pocet_riadkov/2; i < pocet_riadkov; i++) {
            if (aktualny_krok == (celkovy_pocet_krokov-1)){
            printf("%d:", rank);}
            for (int j = 0; j < pocet_stlpcov/2; j++) {
                if (aktualny_krok == (celkovy_pocet_krokov-1)){
                printf("%d", pole_po_iteracii[i][j]);}
                MPI_Send(&pole_po_iteracii[i][j], 1, MPI_INT, 0, tag2, MPI_COMM_WORLD);
                tag2++; 
            }
            if (aktualny_krok == (celkovy_pocet_krokov-1)){
            printf("\n");}
        }

        // vycistenie pole
        for (int i = 0; i < pocet_riadkov; i++) {
            free(pole[i]);
        }
        free(pole);

        // vycistenie pole_po_iteracii
        for (int i = 0; i < pocet_riadkov; i++) {
            free(pole_po_iteracii[i]);
        }
        free(pole_po_iteracii);

        
    }
    if (rank==3){
        int tag3=0;
        MPI_Request send_request[1];
        // pre pole zo suboru
        int **pole = (int **) malloc(pocet_riadkov * sizeof(int *));
        for (int i = 0; i < pocet_riadkov; i++) {
            pole[i] = (int *) malloc(pocet_stlpcov * sizeof(int));
        }
        
        // pre druhe pole, do ktoreho budeme zapisovat hodnoty novej generacie
        int **pole_po_iteracii = (int **) malloc(pocet_riadkov * sizeof(int *));
        for (int i = 0; i < pocet_riadkov; i++) {
            pole_po_iteracii[i] = (int *) malloc(pocet_stlpcov * sizeof(int));
        }

        
        int prislo_pole;
       
        for (int i = 0; i<pocet_riadkov; i++){
            for (int j = 0; j <pocet_stlpcov; j++){
                MPI_Recv(&prislo_pole, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
                pole[i][j] = prislo_pole;
            }
        }

        zisti_oblast(pole,pole_po_iteracii,3, pocet_riadkov, pocet_stlpcov);

        for (int i = pocet_riadkov/2; i < pocet_riadkov; i++) {
            if (aktualny_krok == (celkovy_pocet_krokov-1)){
            printf("%d:", rank);}
            for (int j = pocet_stlpcov/2; j < pocet_stlpcov; j++) {
                if (aktualny_krok == (celkovy_pocet_krokov-1)){
                printf("%d", pole_po_iteracii[i][j]);}
                MPI_Send(&pole_po_iteracii[i][j], 1, MPI_INT, 0, tag3, MPI_COMM_WORLD);
                tag3++;      
            }
            if (aktualny_krok == (celkovy_pocet_krokov-1)){
            printf("\n");}
        }

        // vycistenie pole
        for (int i = 0; i < pocet_riadkov; i++) {
            free(pole[i]);
        }
        free(pole);

        // vycistenie pole_po_iteracii
        for (int i = 0; i < pocet_riadkov; i++) {
            free(pole_po_iteracii[i]);
        }
        free(pole_po_iteracii);

        
    }
    aktualny_krok++;
    }

    MPI_Finalize();
    return 0;
}