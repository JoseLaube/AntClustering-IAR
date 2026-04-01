#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct formiga{
    int posiX;
    int posiY;
    int status;
    int idMorta; //0 não ésta carregando 1 carregando formiga
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
        formigas[i].idMorta = -1;

        //vivas[x][y]++;
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
 void movimento(Formiga formigas[], int i, int tamForm){
    /*Os movimentos
    a formiga pode se mover para 8 direções 
    0 = diagonal superior esquerda - x - 1; y -1
    1 = cima - x - 1; y
    2 = diagonal superior direita - x - 1; y + 1
    3 = esquerda - x; y - 1
    4 = direita - x; y + 1
    5 = diagonal inferior esquerda - x + 1; y - 1
    6 = baixo - x + 1; y
    7 = diagonal inferior direita - x + 1; y + 1
    
    */
    int direcao = rand() % 8;

    int novoX = formigas[i].posiX;
    int novoY = formigas[i].posiY;

    switch(direcao){
        case 0: novoX--; novoY--; break;
        case 1: novoX--; break;
        case 2: novoX--; novoY++; break;
        case 3: novoY--; break;
        case 4: novoY++; break;
        case 5: novoX++; novoY--; break;
        case 6: novoX++; break;
        case 7: novoX++; novoY++; break;
    }

    if(novoX >= 0 && novoX < tamForm && novoY >= 0 && novoY < tamForm){
        formigas[i].posiX = novoX;
        formigas[i].posiY = novoY;
    }

 }

 int visao(Formiga formigas[], int i, int tamForm, int formigueiro[tamForm][tamForm]){
    int cont = 0;
    int x = formigas[i].posiX;
    int y = formigas[i].posiY;

    for(int dx = -1; dx <= 1; dx++){
        for(int dy = -1; dy <= 1; dy++){

            if(dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if(nx >= 0 && nx < tamForm && ny >= 0 && ny < tamForm){
                cont += formigueiro[nx][ny];
            }
        }
    }    
    float result = 0;
    if(formigas[i].status == 1){
        result = cont/8.0;

    }
    else if(formigas[i].status == 0){
        result = 1.0 - cont/8.0;
    }

    float r = (float)rand() / RAND_MAX;

    if(r < result){
        return 1;
    }
    else{
        return 0;
    }

 }

 void pegar(Formiga formigas[], Morta colonia[], int i, int tamForm, int j, int formigueiro[tamForm][tamForm]){
    formigas[i].status = 1;
    formigas[i].idMorta = j;
    colonia[j].status = 1;
    formigueiro[colonia[j].posiX][colonia[j].posiY] = 0;
 }

 void soltar(Formiga formigas[], Morta colonia[], int i, int tamForm, int j, int formigueiro[tamForm][tamForm]){
    formigas[i].status = 0;
    formigas[i].idMorta = -1;
    colonia[j].status = 0;
    colonia[j].posiX = formigas[i].posiX;
    colonia[j].posiY = formigas[i].posiY;
    formigueiro[colonia[j].posiX][colonia[j].posiY] = 1;
 }

 int encontrarMorta(int x, int y, Morta colonia[], int quantMortas){
    for(int i = 0; i < quantMortas; i++){
        if(colonia[i].posiX == x && colonia[i].posiY == y){
            return i;
        }
    }
    return -1;
 }

 void dandoVida(Formiga formigas[], int i, int tamForm, int formigueiro[tamForm][tamForm], Morta colonia[], int quantMortas){
    
    int decisao = 0;
    int j = 0;
    if(formigas[i].status == 1){
        if(formigueiro[formigas[i].posiX][formigas[i].posiY] == 1){
            movimento(formigas, i, tamForm);
        }
        else{
            decisao = visao(formigas, i, tamForm, formigueiro);
            if(decisao == 1){
                if(formigas[i].idMorta != -1){
                    soltar(formigas, colonia, i, tamForm, formigas[i].idMorta, formigueiro);
                }
            }
            else{
                movimento(formigas, i, tamForm);
            }
        }
    }
    else{
        if(formigueiro[formigas[i].posiX][formigas[i].posiY] == 0){
            movimento(formigas, i, tamForm);
        }
        else{
            decisao = visao(formigas, i, tamForm, formigueiro);
            if(decisao == 1){
                j = encontrarMorta(formigas[i].posiX, formigas[i].posiY, colonia, quantMortas);
                if(j != -1){
                    pegar(formigas, colonia, i, tamForm, j, formigueiro);
                }
            }
            else{
                movimento(formigas, i, tamForm);
            }
        }
    }
 }

int main(){
    int tamForm = 64;
    int quantForm = 100;
    int quantMortas = 500;
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

    for(int i = 0; i < tamForm; i++){
        for(int j = 0; j < tamForm; j++){
            if(formigueiro[i][j] == 1){
                printf("%i ", formigueiro[i][j]);
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("\n\n\n");

    for(int i = 0; i < 20000; i++){
        for(int j = 0; j < quantForm; j++){
            dandoVida(formigas, j, tamForm, formigueiro, corpos, quantMortas);
        }
    }

    for(int i = 0; i < tamForm; i++){
        for(int j = 0; j < tamForm; j++){
            if(formigueiro[i][j] == 1){
                printf("%i ", formigueiro[i][j]);
            }
            else{
                printf(" ");
            }        }
        printf("\n");
    }

    /*
    formigas[20].posiX = 63;
    formigas[20].posiY = 63;

    printf("Morta 20 = posicao (%i,%i), status = %i\n", formigas[20].posiX, formigas[20].posiY, formigas[20].status);
    for(int i = 0; i < 3000; i++){
        movimento(formigas, 20, tamForm);
        vivas[formigas[20].posiX][formigas[20].posiY] = 1;
        //printf("Morta 20 = posicao (%i,%i), status = %i\n", formigas[20].posiX, formigas[20].posiY, formigas[20].status);
    }
    int cont = 0;

    for(int i = 0; i<tamForm; i++){
        for(int j = 0; j < tamForm; j++){
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
    printf("\n%i\n", cont);*/
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