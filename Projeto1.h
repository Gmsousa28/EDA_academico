/**
 * @file Projeto1.h
 * @author Gonçalo Sousa
 * @brief Ficheiro de declaração de funções e estruturas de dados para o projeto.
 * @version 0.1
 * @date 2025-03-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef PROJETO1_H
#define PROJETO1_H

/**
 * @brief Estrutura de dados para armazenar as antenas. 
 * 
 */
typedef struct Antena
{
    char freq;           // Tipo de antena (A, B, 0, 1)
    int x, y;            // Coordenadas na matriz
    struct Antena *next; // Aponta para o próximo nó da lista
} Antena;

/**
 * @brief Estrutura de dados para armazenar os nefastos.
 * 
 */
typedef struct Nefasto
{
    int x, y; // Coordenadas do nefasto
    struct Nefasto *next; // Aponta para o próximo nó da lista
} Nefasto;
 






/** * @brief Estrutura de dados para armazenar os adjacentes de cada vértice no grafo.
 * 
 */
typedef struct Adjacente {
    int verticeadj;
    struct Adjacente *next;
} Adjacente;


/**
 * @brief Estrutura de dados para armazenar os vértices do grafo, frequencia, posição (x,y).
 * 
 */
typedef struct Grafo
{
    int vertice;
    char freq;
    int x, y;
    struct Grafo *next; 
    Adjacente *listaadj; // Lista de adjacentes
} Grafo;


/**
 * @brief Estrutura de dados para armazenar as frequências e suas antenas através de interligar esta LL com a LL Grafo obtendo assim as outras informações.
 * 
 */
typedef struct Frequencia
{
    char freq;
    Grafo *listagrafo;
    struct Frequencia *next;
} Frequencia;










//Declarações de funções
Antena *ler_ficheiro(char *filename, char ***matriz, int *linhas, int *colunas);
void adicionar_antena(Antena **antena, char ***matriz, int *linhas, int *colunas, char freq, int x, int y);
void remover_antena(Antena **antena, int x, int y, char ***matriz);
void aplicar_nefasto(Antena *antena, int linhas, int colunas, Nefasto **lista);
void print_matriz(char **matriz, int linhas, int colunas);
void print_matriznefasto(char **matriz, int linhas, int colunas, Nefasto *lista);
void print_lista1(Antena *antena);
void print_lista2(Nefasto *lista);


int antena_existe(Antena *antena, int x, int y);
int existe_nefasto(Nefasto *lista, int x, int y);


void free_lista1(Antena *antena);
void free_lista2(Nefasto *lista);
void free_matriz(char **matriz, int linhas);
void free_matriznefasto(char **matriz, int linhas, int colunas);








// Funções para a parte 2 do projeto


void separar_por_frequencia(Antena *listaAntena, Frequencia **listafreq);
void criarLL_adjacentes(Frequencia *listafreq);
void free_lista_freq(Frequencia *listafreq);
void free_lista_adj(Adjacente *listaadj);
void print_lista_freq(Frequencia *listafreq);
void print_lista_adj(Frequencia *listafreq);



void iniciar_profundidade(Frequencia *listasfreq, int vertice_inicial, int *visitado, int num_vertices);
void procurar_profundidade(Frequencia *listasfreq, int vertice, int *visitado, int *caminho, int *tam_max_caminho);
void busca_em_largura(Frequencia *listasfreq, int inicio);


int vertice_existe(Frequencia *listasfreq, int vertice);
int mesma_frequencia(Frequencia *listasfreq, int v1, int v2);
int frequencia_existe(Frequencia *listasfreq, char freq);



Frequencia* obter_frequencia(Frequencia *listasfreq, char freq);
Grafo* obter_vertice(Frequencia *listasfreq, int vertice);
void calcular_caminhos(Grafo *atual, int destino, int visitado[], int caminho[], int pos, Frequencia *listasfreq);
void print_caminhos(Frequencia *listasfreq, int origem, int destino);


void ver_interseçoes(Frequencia *lista, char tipoA, char tipoB);
int cruzar_segmentos(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, float *ix, float *iy);


#endif
