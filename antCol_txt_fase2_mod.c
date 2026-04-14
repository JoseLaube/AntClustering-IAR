#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

float alpha = 11.8029;
float k1 = 0.3;
float k2 = 0.6;

typedef struct formiga{
    int posiX;
    int posiY;
    int status;
    int idMorta; //0 não ésta carregando 1 carregando formiga
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

        //vivas[x][y]++;
    }
}

void inic_mortas_txt(Morta mortas[], int quantMortas, int tamForm, int formigueiro[tamForm][tamForm]) {
    FILE *f = fopen("dados.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }

    float x_real, y_real;
    int grupo;

    for (int i = 0; i < quantMortas; i++) {

        // lê os dados do arquivo
        if (fscanf(f, "%f %f %d", &x_real, &y_real, &grupo) != 3) {
            printf("Erro ao ler linha %d\n", i);
            break;
        }

        // salva os dados "semânticos"
        mortas[i].x_grupo = x_real;
        mortas[i].y_grupo = y_real;
        mortas[i].grupo = grupo;
        mortas[i].status = 0;

        // posição aleatória no grid (igual ao inic_mortas)
        int x, y;
        do {
            x = rand() % tamForm;
            y = rand() % tamForm;
        }  while (formigueiro[x][y] != -1);
        
        // while (formigueiro[x][y] != 0);


        mortas[i].posiX = x;
        mortas[i].posiY = y;

        // formigueiro[x][y] = mortas[i].grupo;
        formigueiro[x][y] = i;
    
    }

    fclose(f);
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

    // VERSÃO ANTIGA DA DIREÇÃO:
    
    /*
    int direcao = rand() % 8;
    switch(direcao){
        case 0: novoX--; novoY--; break; // diagonal superior esquerda - x - 1; y -1
        case 1: novoX--; break;
        case 2: novoX--; novoY++; break; // diagonal superior direita - x - 1; y + 1
        case 3: novoY--; break;
        case 4: novoY++; break;
        case 5: novoX++; novoY--; break; // diagonal inferior esquerda - x + 1; y - 1
        case 6: novoX++; break;
        case 7: novoX++; novoY++; break; // diagonal inferior direita - x + 1; y + 1
    }

    if(novoX >= 0 && novoX < tamForm && novoY >= 0 && novoY < tamForm){
        formigas[i].posiX = novoX;
        formigas[i].posiY = novoY;
    }
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

int encontrarMorta(int x, int y, Morta colonia[], int quantMortas){
    for(int i = 0; i < quantMortas; i++){
        if(colonia[i].posiX == x && colonia[i].posiY == y){
            return i;
        }
    }
    return -1;
}

float dist_euclidiana(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int visao(Formiga formigas[], int i, int tamForm, int formigueiro[tamForm][tamForm], int raio, Morta colonia[], int quantMortas){
    int x = formigas[i].posiX;
    int y = formigas[i].posiY;
    
    int id_referencia;
    if (formigas[i].status == 1) {
        id_referencia = formigas[i].idMorta; // Formiga carregando o dado
    } else {
        id_referencia = formigueiro[x][y];   // Formiga vazia olhando pro dado no chão
    }

    float soma_similaridade = 0.0;
    int num_celulas_vizinhanca = (pow((2 * raio + 1), 2) - 1); // s^2

    for(int dx = -raio; dx <= raio; dx++){
        for(int dy = -raio; dy <= raio; dy++){
            if(dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx < 0) nx = tamForm - 1;
            if (nx >= tamForm) nx = 0;
            if (ny < 0) ny = tamForm - 1;
            if (ny >= tamForm) ny = 0;

            // int id_vizinho = encontrarMorta(nx, ny, colonia, quantMortas);
            int id_vizinho = formigueiro[nx][ny];

            if(id_vizinho != -1){
                
                float d = dist_euclidiana(
                    colonia[id_referencia].x_grupo, colonia[id_referencia].y_grupo,
                    colonia[id_vizinho].x_grupo, colonia[id_vizinho].y_grupo
                );
                
                // (1 - d(i,j)/alpha)
                float similaridade = 1.0 - (d / alpha);
                if (similaridade > 0) {
                    soma_similaridade += similaridade;
                }
            }
        }
    }

    float f = (1.0 / num_celulas_vizinhanca) * soma_similaridade;
    if(f < 0) f = 0;
    if(f > 1) f = 1;

    float probabilidade = 0.0;

    if (formigas[i].status == 0) {
        // Pp(xi) = (k1 / (k1 + f(xi)))^2
        probabilidade = pow(k1 / (k1 + f), 2);
    } else {
        // Pd(xi) = (f(xi) / (k2 + f(xi)))^2
        probabilidade = pow(f / (k2 + f), 2);
    }

    // Role a roleta (entre 0 e 1)
    float r = (float)rand() / RAND_MAX;
    
    if (r <= probabilidade) {
        return 1; // Ação aceita (vai pegar ou soltar)
    } else {
        return 0; // Ação rejeitada
    }
}

void pegar(Formiga formigas[], Morta colonia[], int i, int tamForm, int j, int formigueiro[tamForm][tamForm]){
    formigas[i].status = 1;
    formigas[i].idMorta = j;
    colonia[j].status = 1;
    //formigueiro[colonia[j].posiX][colonia[j].posiY] = 0;
    formigueiro[colonia[j].posiX][colonia[j].posiY] = -1;
}

void soltar(Formiga formigas[], Morta colonia[], int i, int tamForm, int j, int formigueiro[tamForm][tamForm]){
    formigas[i].status = 0;
    formigas[i].idMorta = -1;
    colonia[j].status = 0;
    colonia[j].posiX = formigas[i].posiX;
    colonia[j].posiY = formigas[i].posiY;
    //formigueiro[colonia[j].posiX][colonia[j].posiY] = colonia[j].grupo;
    formigueiro[colonia[j].posiX][colonia[j].posiY] = j;
}

void dandoVida(Formiga formigas[], int i, int tamForm, int formigueiro[tamForm][tamForm], Morta colonia[], int quantMortas, int raio){
    
    int decisao = 0;
    int j = 0;
    if(formigas[i].status == 1){
        //if(formigueiro[formigas[i].posiX][formigas[i].posiY] != 0){
        if(formigueiro[formigas[i].posiX][formigas[i].posiY] != -1){
            movimento(formigas, i, tamForm);
        }
        else{
            decisao = visao(formigas, i, tamForm, formigueiro, raio, colonia, quantMortas);
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
        //if(formigueiro[formigas[i].posiX][formigas[i].posiY] == 0){
        if(formigueiro[formigas[i].posiX][formigas[i].posiY] == -1){
            movimento(formigas, i, tamForm);
        }
        else{
            decisao = visao(formigas, i, tamForm, formigueiro, raio, colonia, quantMortas);
            if(decisao == 1){
                // j = encontrarMorta(formigas[i].posiX, formigas[i].posiY, colonia, quantMortas);
                
                j = formigueiro[formigas[i].posiX][formigas[i].posiY];
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

void salvarMapaTxt(int tam, int formigueiro[tam][tam], Formiga formigas[], int quantForm, int iteracao, Morta colonia[]) {
    FILE *f = fopen("visualizacao.txt", "w");
    if (f == NULL) return;

    char visual[tam][tam];

    // 1. Preenche a matriz visual com Itens ou Espaço Vazio
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (formigueiro[i][j] != -1) {
                visual[i][j] = colonia[formigueiro[i][j]].grupo + '0';
            } else {
                visual[i][j] = ' ';
            }
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
    int quantMortas = 400;
    int raio = 1;
    int num_iteracoes = 2000000;
    int frequenciaSnapshot = 500; // iteracoes para atualizar o txt

    int formigueiro[tamForm][tamForm];
    int vivas[tamForm][tamForm];
    srand(time(NULL));
    Formiga formigas[quantForm];
    Morta corpos[quantMortas];

    for(int i =0; i< tamForm; i++){
        for(int j = 0; j < tamForm; j++){
            // formigueiro[i][j] = 0;
            formigueiro[i][j] = -1;
            vivas[i][j] = 0;
        }
    }

    inic_formigas(formigas, quantForm, tamForm, vivas);

    inic_mortas_txt(corpos, quantMortas, tamForm, formigueiro);

    printf("\nDisposição de elementos inicialmente: \n\n");
    for(int i = 0; i < tamForm; i++){
        for(int j = 0; j < tamForm; j++){
            // if(formigueiro[i][j] != 0){
            if(formigueiro[i][j] != -1){
                // printf("%i ", formigueiro[i][j]);
                printf("%d ", corpos[formigueiro[i][j]].grupo);
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("\n\n\n");

    for(int i = 0; i < num_iteracoes; i++){
        for(int j = 0; j < quantForm; j++){
            dandoVida(formigas, j, tamForm, formigueiro, corpos, quantMortas, raio);
        }

        // if(i % frequenciaSnapshot == 0){
        //     salvarMapaTxt(tamForm, formigueiro, formigas, quantForm, i);
        // }
    }

    // print final do formigueiro
    salvarMapaTxt(tamForm, formigueiro, formigas, quantForm, num_iteracoes, corpos);

    printf("\nDisposição Final: \n\n");
    for(int i = 0; i < tamForm; i++){
        for(int j = 0; j < tamForm; j++){
            if(formigueiro[i][j] > 0){
                // printf("%i ", formigueiro[i][j]);
                if(corpos[formigueiro[i][j]].grupo == 0){
                    printf(" ");
                } else {
                    printf("%i ", corpos[formigueiro[i][j]].grupo);
                }
            }
            else{
                printf(" ");
            }        }
        printf("\n");
    }

    printf("\n");

    return 0;
}