/*
PRL projekt cislo 1
Emma Krompascikova
xkromp00
2023/2024*/

#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(){
 
    MPI_Init(NULL, NULL);

    int world_size; //pocet procesorov ktore algoritmus vykonavaju
    MPI_Comm_size(MPI_COMM_WORLD, &world_size );

    int rank; //cislo procesora (jeho index)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if(rank == 0 ){ //vykona iba nulty procesor

          FILE * pNumbers;
        pNumbers = fopen("./numbers","r");

        if (pNumbers == NULL){
            fprintf(stderr,"Failed to open a file.\n");
            return -1;
        }
        fseek(pNumbers, 0L, SEEK_END);
        int velkostSuboru = ftell(pNumbers); //kolko cisel je zadanych

        MPI_Bcast( &velkostSuboru, 1, MPI_INT,0, MPI_COMM_WORLD);

        fseek(pNumbers, 0L, SEEK_SET); //vraciam sa kurzorom na zaciatok suboru
        unsigned char* vstupnePole = (unsigned char*)malloc(velkostSuboru);
        fread(vstupnePole,1,velkostSuboru, pNumbers); //otvori subor a zisti jeho dlzku
        
        int parity = 1; //ked je 0 ide hore, ked je 1 ide dole
        //frona hore ma tag  0
        //fronta dole ma tag 1
        int vstup;


        MPI_Request send_request[2];

        for (int i = 0; i < velkostSuboru; i++){
            parity++;
            if ( i == velkostSuboru - 1){
                printf("%u\n",vstupnePole[i]);
                vstup = vstupnePole[i];
                if (parity % 2 == 0){
                    MPI_Isend(&vstup,1,MPI_INT,1,0,MPI_COMM_WORLD, &send_request[0]);
                }
                else{
                    MPI_Isend(&vstup,1,MPI_INT,1,1,MPI_COMM_WORLD, &send_request[1]);
                }
                
            }
            else{
                int f = vstupnePole[i];
                printf("%u ",vstupnePole[i]);
                vstup = vstupnePole[i];
                if (parity % 2 == 0){
                   MPI_Isend(&vstup,1,MPI_INT,1,0,MPI_COMM_WORLD, &send_request[0]);
                }
                else{
                    MPI_Isend(&vstup,1,MPI_INT,1,1,MPI_COMM_WORLD, &send_request[1]);
                }
            }
        }


    }
    else{
        int velkostSuboru = 0;
        MPI_Bcast( &velkostSuboru, 1, MPI_INT,0, MPI_COMM_WORLD);
        int dlzka_fronty = pow(2,rank);
        MPI_Request send_request[2];
        MPI_Request recv_request[2];
        unsigned char fronta0[dlzka_fronty];
        unsigned char fronta1[dlzka_fronty];
        int pocet_prvkov_vo_fronte0 = 0;
        int pocet_prvkov_vo_fronte1 = 0;
        int prichozi0;
        int prichozi1;
        int prislo_cislo0;
        int prislo_cislo1;
        int posledne_poslane = 0;

        MPI_Irecv(&prichozi0,1,MPI_INT, (rank-1), 0, MPI_COMM_WORLD, &recv_request[0]);
        MPI_Irecv(&prichozi1,1,MPI_INT, (rank-1), 1, MPI_COMM_WORLD, &recv_request[1]);

        bool prvePriate, druhePriate;
        int pocetPriatychCisel = 0;
        int pocetodoslanych = 0;
        
        int poslednafronta = 0 ;
        int prveCisNaOdoslanie;
        int druheCisNaOdoslanie;

        int tagNaPoslanie1 = 0;



        while(true){
            MPI_Test(&recv_request[0], &prislo_cislo0, MPI_STATUS_IGNORE);
            if (prislo_cislo0 && pocet_prvkov_vo_fronte0!=dlzka_fronty) {
                fronta0[pocet_prvkov_vo_fronte0] = prichozi0;
                pocet_prvkov_vo_fronte0++;
                pocetPriatychCisel++;
                MPI_Irecv(&prichozi0,1,MPI_INT, (rank-1), 0, MPI_COMM_WORLD, &recv_request[0]);
            }

            MPI_Test(&recv_request[1], &prislo_cislo1, MPI_STATUS_IGNORE);
            if (prislo_cislo1 && pocet_prvkov_vo_fronte0!=dlzka_fronty) {

                fronta1[pocet_prvkov_vo_fronte1] = prichozi1;
                pocet_prvkov_vo_fronte1++;
                pocetPriatychCisel++;
                MPI_Irecv(&prichozi1,1,MPI_INT, (rank-1), 1, MPI_COMM_WORLD, &recv_request[1]);
            }

            if  ((pocet_prvkov_vo_fronte0 >= (dlzka_fronty/2) && pocet_prvkov_vo_fronte1 > 0) || (pocet_prvkov_vo_fronte0 > 0 && pocet_prvkov_vo_fronte1 >= (dlzka_fronty/2)) || pocetPriatychCisel == velkostSuboru){
                if (rank == world_size - 1 && pocetPriatychCisel == velkostSuboru && (pocet_prvkov_vo_fronte0 == 0 || pocet_prvkov_vo_fronte1 == 0)) {
                    if (pocet_prvkov_vo_fronte1 == 0) {
                        printf("%d\n", fronta0[0]);

                        for (int i = 0; i<(dlzka_fronty-1); i++){
                            fronta0[i] = fronta0[i+1];
                        }

                        pocet_prvkov_vo_fronte0--;
                    }
                    else {
                        printf("%d\n", fronta1[0]);

                        for (int i = 0; i<(dlzka_fronty-1); i++){
                            fronta1[i] = fronta1[i+1];
                        }

                        pocet_prvkov_vo_fronte1--;
                    }

                    pocetodoslanych++;
                }
                else {
                    if (fronta0[0] < fronta1[0]){
                        prveCisNaOdoslanie = fronta0[0];
                        druheCisNaOdoslanie = fronta1[0];
                        
                        if (posledne_poslane < prveCisNaOdoslanie){
                            tagNaPoslanie1 = tagNaPoslanie1;
                        }
                        else{
                            tagNaPoslanie1 = !tagNaPoslanie1;
                        }
                        
                        if (rank == (world_size-1)){
                            printf("%d\n", prveCisNaOdoslanie);
                            printf("%d\n", druheCisNaOdoslanie);

                        }
                        else {
                            MPI_Isend(&prveCisNaOdoslanie,1,MPI_INT,(rank+1),tagNaPoslanie1,MPI_COMM_WORLD, &send_request[0]);
                            MPI_Isend(&druheCisNaOdoslanie,1,MPI_INT,(rank+1),tagNaPoslanie1,MPI_COMM_WORLD, &send_request[1]);
                            posledne_poslane = druheCisNaOdoslanie;
                        }
                        pocetodoslanych++;
                        pocetodoslanych++;
                    }
                    else {
                        prveCisNaOdoslanie = fronta1[0];
                        druheCisNaOdoslanie = fronta0[0];

                        if (posledne_poslane < prveCisNaOdoslanie){
                            tagNaPoslanie1 = tagNaPoslanie1;
                        }
                        else{
                            tagNaPoslanie1 = !tagNaPoslanie1;
                        }

                        if (rank == (world_size-1)){
                            printf("%d\n", prveCisNaOdoslanie);
                            printf("%d\n", druheCisNaOdoslanie);

                        }
                        else {
                            MPI_Isend(&prveCisNaOdoslanie,1,MPI_INT,(rank+1),tagNaPoslanie1,MPI_COMM_WORLD, &send_request[0]);
                            MPI_Isend(&druheCisNaOdoslanie,1,MPI_INT,(rank+1),tagNaPoslanie1,MPI_COMM_WORLD, &send_request[1]);
                            posledne_poslane = druheCisNaOdoslanie;
                        }
                        pocetodoslanych++;
                        pocetodoslanych++;
                    }

                    for (int i = 0; i<(dlzka_fronty-1); i++){
                        fronta0[i] = fronta0[i+1];
                        fronta1[i] = fronta1[i+1]; 
                    }

                    pocet_prvkov_vo_fronte0--;
                    pocet_prvkov_vo_fronte1--;
                }
                
            }
            
            if (pocetodoslanych == velkostSuboru ) {
                break;
            }
        }

    }

    MPI_Finalize();
    return 0;
}