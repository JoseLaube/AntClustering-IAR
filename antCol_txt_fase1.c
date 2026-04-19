#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct formiga{
    int posiX;
    int posiY;
    int status;
    int idMorta; // 0 não esta carregando 1 carregando formiga
} Formiga;

typedef struct morta{
    int posiX;
    int posiY;
    int status; 
    int grupo;
    float x_grupo;
    float y_grupo;
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

void movimento(Formiga formigas[], int i, int tamForm){
    /*Os movimentos
    a formiga pode se mover para 4 direções 
    0 = cima - x - 1; y
    1 = esquerda - x; y - 1
    2 = direita - x; y + 1
    3 = baixo - x + 1; y
    */

    // VERSÃO NOVA DA DIREÇÃO:
    int novoX = formigas[i].posiX;
    int novoY = formigas[i].posiY;

    int direcao = rand() % 4;

    switch(direcao){
        case 0: novoX--; break;
        case 1: novoY--; break;
        case 2: novoY++; break;
        case 3: novoX++; break;
    }

    if(novoX < 0){
        novoX = tamForm - 1;
    }

    if (novoX >= tamForm) {
        novoX = 0;
    }

    if(novoY < 0){
        novoY = tamForm - 1;
    }

    if (novoY >= tamForm) {
        novoY = 0;
    }

    formigas[i].posiX = novoX;
    formigas[i].posiY = novoY;

}

int visao(Formiga formigas[], int i, int tamForm, int formigueiro[tamForm][tamForm], int raio){
    int cont = 0;
    int x = formigas[i].posiX;
    int y = formigas[i].posiY;

    for(int dx = -raio; dx <= raio; dx++){
        for(int dy = -raio; dy <= raio; dy++){

            if(dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx < 0) nx = tamForm - 1;
            if (nx >= tamForm) nx = 0;

            if (ny < 0) ny = tamForm - 1;
            if (ny >= tamForm) ny = 0;

            cont += formigueiro[nx][ny];

        }
    }
    float result = 0;
    if(formigas[i].status == 1){
        result = cont/(pow((2 * raio + 1), 2) - 1);
    }
    else if(formigas[i].status == 0){
        result = 1.0 - cont/(pow((2 * raio + 1), 2) - 1);
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

void dandoVida(Formiga formigas[], int i, int tamForm, int formigueiro[tamForm][tamForm], Morta colonia[], int quantMortas, int raio){
    
    int decisao = 0;
    int j = 0;
    if(formigas[i].status == 1){
        if(formigueiro[formigas[i].posiX][formigas[i].posiY] == 1){
            movimento(formigas, i, tamForm);
        }
        else{
            decisao = visao(formigas, i, tamForm, formigueiro, raio);
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
            decisao = visao(formigas, i, tamForm, formigueiro, raio);
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

void salvarMapaTxt(int tam, int formigueiro[tam][tam], Formiga formigas[], int quantForm, int iteracao) {
    FILE *f = fopen("visualizacao.txt", "w");
    if (f == NULL) return;

    char visual[tam][tam];

    // 1. Preenche a matriz visual com Itens ou Espaço Vazio
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            visual[i][j] = (formigueiro[i][j] == 1) ? 'o' : ' '; 
        }
    }

    // 2. Sobrepõe as formigas
    for (int i = 0; i < quantForm; i++) {
        int fx = formigas[i].posiX;
        int fy = formigas[i].posiY;
        if (formigas[i].status == 1) visual[fx][fy] = 'X'; // Carregando
        else visual[fx][fy] = 'A'; // Livre
    }

    // 3. Grava no arquivo com bordas para facilitar ver o limite
    fprintf(f, "Iteracao: %d | Formigas: A (livre) X (carregando) | Itens: o\n", iteracao);
    for(int j=0; j<tam+2; j++) fprintf(f, "-"); fprintf(f, "\n");
    for (int i = 0; i < tam; i++) {
        fprintf(f, "|");
        for (int j = 0; j < tam; j++) {
            fprintf(f, "%c ", visual[i][j]);
        }
        fprintf(f, "|\n");
    }
    for(int j=0; j<tam+2; j++) fprintf(f, "-"); fprintf(f, "\n");

    fclose(f);
}


int main(){
    int tamForm = 64;
    int quantForm = 100;
    int quantMortas = 101;
    int raio = 1;
    int num_iteracoes = 250000;
    int frequenciaSnapshot = 500; // taxa de atualizacao do vizualizacao.txt

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

    printf("\nIniciando simulacao Fase 1\n");
    
    clock_t t_inicio = clock();
    int check_point = num_iteracoes / 10;

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
    printf("Progresso: ");
            

    for(int i = 0; i < num_iteracoes; i++){
        for(int j = 0; j < quantForm; j++){
            dandoVida(formigas, j, tamForm, formigueiro, corpos, quantMortas, raio);
        }

        // Print de progressão de 10 em 10%
        if ((i + 1) % check_point == 0) {
            int porcentagem = (int)((i + 1) * 100.0 / num_iteracoes);
            
            printf("%d%% ", porcentagem);
            fflush(stdout);
        }

        if(i % frequenciaSnapshot == 0){
            salvarMapaTxt(tamForm, formigueiro, formigas, quantForm, i);
        }
    }

    printf("\n\n");
    clock_t t_fim = clock();
    double tempo_total = (double)(t_fim - t_inicio) / CLOCKS_PER_SEC;

    // print final do formigueiro
    salvarMapaTxt(tamForm, formigueiro, formigas, quantForm, num_iteracoes);

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

    printf("\n========================================\n");
    printf("       RELATORIO FINAL - FASE 1         \n");
    printf("========================================\n");
    printf("Iteracoes: %d | Formigas Vivas: %d\n", num_iteracoes, quantForm);
    printf("Grid: %d x %d | Mortas: %d\n", tamForm, tamForm, quantMortas);
    printf("Tempo Total: %.4f segundos\n", tempo_total);
    printf("Desempenho: %.8f s/iteracao\n", tempo_total / num_iteracoes);
    printf("========================================\n\n");


    return 0;
}