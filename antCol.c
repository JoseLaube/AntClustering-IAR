#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct formiga{
    int posiX;
    int posiY;
    int status; //0 não ésta carregando 1 carregando formiga
} Formiga;

typedef struct morta{
    int posiX;
    int posiY;
    int status; //0 no chão, 1 sendo carregada;
} Morta;

void inic_formigas(Formiga formigas[], int quantForm, int tamForm, int vivas[tamForm][tamForm]){
    for(int i =0; i < quantForm; i++){
        int x, y;
        
        x = rand() % tamForm;
        y = rand() % tamForm;

        formigas[i].posiX = x;
        formigas[i].posiY = y;
        formigas[i].status = 0;

        vivas[x][y]++;
    }
}

void inic_mortas(Morta mortas[], int quantMortas, int tamForm, int formigueiro[tamForm][tamForm]){
    for(int i = 0; i < quantMortas; i++){
        int x, y;

        do{
            x = rand() % tamForm;
            y = rand() % tamForm;
        } while (formigueiro[x][y] == 1);
        
        mortas[i].posiX = x;
        mortas[i].posiY = y;
        mortas[i].status = 0;

        formigueiro[x][y] = 1;
    } 
}


int main(){
    int tamForm = 64;
    int quantForm = 100;
    int quantMortas = 200;
    int visao = 1;
    int formigueiro[tamForm][tamForm];
    int vivas[tamForm][tamForm];
    srand(time(NULL));

    Formiga formigas[quantForm];
    Morta corpos[quantMortas];

    for(int i =0; i< tamForm; i++){
        for(int j = 0; j < tamForm; j++){
            formigueiro[i][j] = 0;
            vivas[i][j] = 0;
        }
    }

    inic_formigas(formigas, quantForm, tamForm, vivas);

    inic_mortas(corpos, quantMortas, tamForm, formigueiro);

    /*

    ESTRUTURA PARA TESTE DE INICIALIZAÇÃO

    for(int i = 0; i < quantMortas; i++){
        printf("Morta %i = posicao (%i,%i), status = %i\n", i, corpos[i].posiX, corpos[i].posiY, corpos[i].status);
    }

    int cont = 0;
    for(int i = 0; i < tamForm; i++){
        for (int j = 0; j < tamForm; j++){
            if(vivas[i][j] > 0){
                printf("%i ", vivas[i][j]);
                cont += vivas[i][j];
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n\nQuantidade = %i", cont);
    */
    return 0;
}