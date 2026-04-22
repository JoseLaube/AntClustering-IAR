# Ant Clustering - Agrupamento Utilizando Formigas

Este repositório contém a implementação de um sistema de agrupamento (clustering) inspirado no comportamento necrofórico de formigas reais, baseado no modelo de **Lumer-Faieta**. O projeto foi desenvolvido como parte da disciplina de Inteligência Artificial para o curso de Ciência da Computação da UDESC.

## Sobre o Projeto

O algoritmo simula o comportamento de agentes (formigas operárias) em um ambiente toroidal que organizam itens espalhados aleatoriamente em clusters coesos. O projeto foi dividido em duas fases:

1.  **Fase 1 (Agrupamento por Densidade):** Itens homogêneos são agrupados baseando-se apenas na densidade local de ocupação do grid.
2.  **Fase 2 (Clusterização Semântica):** Itens heterogêneos são agrupados utilizando similaridade baseada na **Distância Euclidiana** entre atributos multidimensionais, permitindo a separação de diferentes classes (grupos).

## Tecnologias Utilizadas

*   Linguagem C
*   Compilador GCC
*   Ambiente Linux (Ubuntu)

## Modelagem Matemática

A probabilidade de uma formiga interagir com um dado depende da função de similaridade local $f(x_i)$:

$$f(x_i) = \frac{1}{s^2} \sum_{x_j \in \text{Vizinhança}} \max\left(0, 1 - \frac{d(x_i, x_j)}{\alpha}\right)$$

As decisões de pegar ($P_p$) e soltar ($P_d$) são governadas por:

$$P_p(x_i) = \left( \frac{k_1}{k_1 + f(x_i)} \right)^2 \quad \text{e} \quad P_d(x_i) = \left( \frac{f(x_i)}{k_2 + f(x_i)} \right)^2$$

## Como Executar

Certifique-se de estar em um ambiente Linux com o GCC instalado. Devido ao uso da biblioteca `<math.h>`, a flag `-lm` é obrigatória.

### Passo 1: Clonar o repositório
```bash
git clone https://github.com/JoseLaube/AntClustering-IAR.git
cd AntClustering-IAR
```

### Passo 2: Compilar
Para a Fase 2 (versão completa com dados heterogêneos):
```bash
gcc antCol_txt_fase2.c -o antCol -lm
```

### Passo 3: Executar
```bash
./antCol
```

## Estrutura do Repositório

*   `antCol_txt_fase1.c`: Implementação inicial baseada em densidade.
*   `antCol_txt_fase2.c`: Implementação avançada com Lumer-Faieta e otimização $O(1)$.
*   `dados_4.txt` / `dados_15.txt`: Datasets com 4 e 15 grupos de dados.
*   `visualizacao.txt`: Snapshot gerado da última iteração da simulação.
*   `/imagens`: Prints dos resultados obtidos e métricas de desempenho.
*   `Relatorio_AntColony_GustavoSouza_JoseLaube.pdf`: Documentação técnica completa.

## Autores

*   **Gustavo de Souza** - [Souza04Gustavo](https://github.com/Souza04Gustavo)
*   **Jose Augusto Laube** - [JoseLaube](https://github.com/JoseLaube)
