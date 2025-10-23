/**
 * @file Projeto1.c
 * @author Gonçalo Sousa
 * // Manipulação das antenas e nefastos através de listas ligadas
 * @version 0.1
 * @date 2025-03-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include "Projeto1.h"
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#define MAX_VERTICES 1000


/**
 * @brief Esta Função lê um ficheiro de texto e armazena os dados numa matriz e adiciona as antenas a uma lista ligada.
 * 
 * @param filename 
 * @param matriz 
 * @param linhas 
 * @param colunas 
 * @return Antena* 
 */
// Função para ler o ficheiro e adicionar a matriz
Antena *ler_ficheiro(char *filename, char ***matriz, int *linhas, int *colunas)
{
    FILE *file = fopen(filename, "r"); // Abre o ficheiro para leitura
    if (file == NULL)                  // Verifica se o ficheiro foi aberto corretamente
    {
        printf("Erro ao abrir o ficheiro!\n");
        exit(1);
    }

    *linhas = *colunas = 0;                 // Inicializa linhas e colunas a 0
    char temp[1024];                        // Buffer temporário para ler linhas do ficheiro lmitado a 1024 caracteres
    while (fgets(temp, sizeof(temp), file)) // Lê linha a linha do ficheiro
    {
        if (*colunas == 0) // percorre a primeira linha caracter a caracter para obter o numero de colunas, se já existir colunas não faz nada 
        {
            char *ptr = temp;    // Conta o número de colunas
            while (*ptr != '\0') // Enquanto não chegar ao fim da string
            {
                if (*ptr != ' ' && *ptr != '\n') // Se não for espaço ou nova linha adicona 1 coluna, apenas conta os caracteres
                    (*colunas)++;
                ptr++;
            }
        }
        (*linhas)++;
    }
    rewind(file); // Volta ao início do ficheiro para ler novamente e guardar os dados na matriz

    *matriz = (char **)malloc((*linhas) * sizeof(char *)); // Aloca memória para as linhas da matriz
    for (int i = 0; i < *linhas; i++)                      // Percorre as linhas
    {
        (*matriz)[i] = (char *)malloc((*colunas) * sizeof(char)); // Aloca memória para as colunas da matriz
    }

    Antena *antena = NULL;            // Inicializa a lista de antenas
    for (int i = 0; i < *linhas; i++) // Percorre as linhas da matriz
    {
        for (int j = 0; j < *colunas; j++) // Percorre as colunas da matriz
        {
            fscanf(file, " %c", &((*matriz)[i][j]));                                       // Lê o caracter da matriz
            if ((*matriz)[i][j] != '.')                                                    // Se o caracter não for um ponto, adiciona à lista de antenas
                adicionar_antena(&antena, matriz, linhas, colunas, (*matriz)[i][j], i, j); // Adiciona a antena à lista
        }
    }

    fclose(file);  // Fecha o ficheiro
    return antena; // Retorna a lista de antenas
}


/**
 * @brief Cria uma nova antena e retorna o ponteiro para ela.
 * 
 * @param freq 
 * @param x 
 * @param y 
 * @return Antena* 
 */
// Função para criar uma nova antena
Antena *criar_antena(char freq, int x, int y)
{
    Antena *nova_antena = (Antena *)malloc(sizeof(Antena)); // Aloca memória para a nova antena
    if (!nova_antena)                                       // Verifica se a alocação foi bem sucedida
    {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    nova_antena->freq = freq; // Atribui a frequência à nova antena
    nova_antena->x = x;       // Atribui a coordenada x à nova antena
    nova_antena->y = y;       // Atribui a coordenada y à nova antena 
    nova_antena->next = NULL; // Inicializa o ponteiro para a próxima antena como NULL ficando como a última antena da lista
    return nova_antena;       // Retorna a nova antena
}


/**
 * @brief Adiciona uma nova antena à lista de antenas e atualiza a matriz.
 * 
 * @param antena 
 * @param matriz 
 * @param linhas 
 * @param colunas 
 * @param freq 
 * @param x 
 * @param y 
 */
// Função para adiconar uma nova antena à lista de antenas 
void adicionar_antena(Antena **antena, char ***matriz, int *linhas, int *colunas, char freq, int x, int y)
{
    (*matriz)[x][y] = freq;                        // Adiciona a antena à matriz
    Antena *new_antena = criar_antena(freq, x, y); // Cria uma nova antena

    if (*antena == NULL || (x < (*antena)->x) || (x == (*antena)->x && y < (*antena)->y)) // Se a lista estiver vazia ou a antena for menor que a primeira antena passa a primeira antena
    {
        new_antena->next = *antena; // A nova antena aponta para a primeira antena
        *antena = new_antena;       // A nova antena é a primeira antena da lista
        return;
    }

    Antena *temp = *antena;
    while (temp->next != NULL && (temp->next->x < x || (temp->next->x == x && temp->next->y < y))) // Enquanto a proxima antena for menor que a nova
    {
        temp = temp->next; // Avança para a próxima antena
    }

    new_antena->next = temp->next; // A nova antena aponta para a próxima antena
    temp->next = new_antena;       // A próxima antena do temp passa a ser a nova antena
    // Atualiza a matriz com a nova antena
}


/**
 * @brief Remove antena da matriz e da lista de antenas.
 * 
 * @param antena 
 * @param x 
 * @param y 
 * @param matriz 
 */
// Função para remover uma antena da lista de antenas
void remover_antena(Antena **antena, int x, int y, char ***matriz)
{
    (*matriz)[x][y] = '.'; // Remove a antena da matriz
    Antena *temp = *antena, *prev = NULL;
    while (temp != NULL && (temp->x != x || temp->y != y)) // Percorre a lista de antenas
    {
        prev = temp;       // Guarda o ponteiro para a antena anterior
        temp = temp->next; // Avança para a próxima antena
    }
    if (temp == NULL)            // Antena não encontrada
        return;                  // Retorna sem fazer nada
    if (prev == NULL)            // Se a antena a remover for a primeira da lista
        *antena = temp->next;    // A 2a antena passa a ser a primeira
    else                         // Se a antena a remover não for a primeira da lista
        prev->next = temp->next; // A antena anterior passa a apontar para a próxima antena
    free(temp);                  // Liberta a memória da antena removida
}


/**
 * @brief Aplica o efeito nefasto às antenas da lista e guarda as coordenadas na lista de nefastos.
 * 
 * @param antena 
 * @param linhas 
 * @param colunas 
 * @param lista 
 */
// Função para aplicar o efeito nefasto e armazenar as coordenadas na lista de nefastos
void aplicar_nefasto(Antena *antena, int linhas, int colunas, Nefasto **lista)
{
    free_lista2(*lista); // Liberta a memória da lista de nefastos para evitar duplicação de nefastos
    *lista = NULL;       // Inicializa a lista de nefastos como NULL
    Antena *a1, *a2;    // Ponteiros para percorrer a lista de antenas

    for (a1 = antena; a1 != NULL; a1 = a1->next) // Percorre a lista de antenas
    {
        for (a2 = a1->next; a2 != NULL; a2 = a2->next) // Percorre a lista de antenas a partir da próxima antena
        {
            if (a1->freq == a2->freq) // Se as antenas forem do mesmo tipo
            {
                // Calcula a nova posição nefasta
                // A nova posição é calculada subtraindo a diferença entre as coordenadas de A1 e A2 ajuda a saber o espaçamento entre as antenas
                // Isso cria uma linha entre as duas antenas, marcando as células entre elas
                // Originando 2 posições nefastas.
                {
                    int dx = a2->x - a1->x;  // Diferença entre as coordenadas x
                    int dy = a2->y - a1->y;  // Diferença entre as coordenadas y
                    int new_x1 = a1->x - dx; // Nova coordenada x da antena A1 (posição nefasta)
                    int new_y1 = a1->y - dy; // " " "  "y da antena A1 
                    int new_x2 = a2->x + dx; // " " "  "x da antena A2
                    int new_y2 = a2->y + dy; // " " "  "y da antena A2

                    
                    if (new_x1 >= 0 && new_x1 < linhas && new_y1 >= 0 && new_y1 < colunas && !existe_nefasto(*lista, new_x1, new_y1))
                    { // Verifica se a posição não é uma antena existente e se está dentro da matriz

                        Nefasto *novo = (Nefasto *)malloc(sizeof(Nefasto)); 
                        novo->x = new_x1; //atribui a nova coordenada x
                        novo->y = new_y1; //atribui a nova coordenada y
                        novo->next = *lista; // A nova posição aponta para a lista de nefastos
                        *lista = novo; // A lista de nefastos passa a ser a nova posição
                    }

                    if (new_x2 >= 0 && new_x2 < linhas && new_y2 >= 0 && new_y2 < colunas && !existe_nefasto(*lista, new_x2, new_y2))
                    { // Verifica se a posição não é uma antena existente e se está dentro da matriz

                        Nefasto *novo = (Nefasto *)malloc(sizeof(Nefasto));
                        novo->x = new_x2;
                        novo->y = new_y2;
                        novo->next = *lista;
                        *lista = novo;
                    }
                }
            }
        }
    }
}


/**
 * @brief Verifica se existe um nefasto na lista de nefastos.
 * 
 * @param lista 
 * @param x 
 * @param y 
 * @return int 
 */
// Função para verificar se existe um nefasto na lista de nefastos
int existe_nefasto(Nefasto *lista, int x, int y)
{
    Nefasto *temp = lista; // Cria um ponteiro temporário para percorrer a lista de nefastos
    while (temp != NULL){ // Enquanto houver nefastos na lista
        if (temp->x == x && temp->y == y)
        {             // Se a coordenada do nefasto for igual à coordenada a verificar
            return 1; // Retorna 1 se o nefasto já existe
        }
        temp = temp->next; // Avança para o próximo nefasto
    }
    return 0; // Retorna 0 se o nefasto não existe
}


/**
 * @brief Imprime a matriz principal.
 * 
 * @param matriz 
 * @param linhas 
 * @param colunas 
 */
// Função para imprimir a matriz original
void print_matriz(char **matriz, int linhas, int colunas)
{
    for (int i = 0; i < linhas; i++) // Percorre as linhas da matriz
    {
        for (int j = 0; j < colunas; j++) // Percorre as colunas
        {
            printf("%c ", matriz[i][j]); // Imprime o caracter na matriz
        }
        printf("\n");
    }
}


/**
 * @brief Imprime a Matriz com os nefastos.
 * 
 * @param matriz 
 * @param linhas 
 * @param colunas 
 * @param lista 
 */
// Função para imprimir a matriz com os nefastos
void print_matriznefasto(char **matriz, int linhas, int colunas, Nefasto *lista)
{
    char matriznefasto[linhas][colunas]; // Cria uma matriz temporária

    // Copia a matriz original para a matriz temporária
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            matriznefasto[i][j] = matriz[i][j];
        }
    }

    // Percorre a lista e adiciona '#' nas posições dos nefastos
    Nefasto *temp = lista;
    while (temp != NULL)
    {
        if (temp->x >= 0 && temp->x < linhas && temp->y >= 0 && temp->y < colunas)
        {
            matriznefasto[temp->x][temp->y] = '#';
        }
        temp = temp->next;
    }

    // Imprime a matriz com os nefastos
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            printf("%c ", matriznefasto[i][j]);
        }
        printf("\n");
    }
}


/**
 * @brief Verifica se existe antenas na lista de antenas.
 * 
 * @param antena 
 * @param x 
 * @param y 
 * @return int 
 */
// Função para verificar se existe uma antena na lista de antenas
int antena_existe(Antena *antena, int x, int y)
{
    while (antena != NULL) // Enquanto houver antenas na lista
    {
        if (antena->x == x && antena->y == y) // Se a coordenada da antena for igual à coordenada da antena a verificar
        {
            // Imprime que a antena já existe
            return (1); // Retorna sem fazer nada
        }
        antena = antena->next; // Avança para a próxima antena até encontrar uma possivel coordenada igual
    }
}


/**
 * @brief Imprime a lista de antenas.
 * 
 * @param antena 
 */
// Função para imprimir a lista de antenas
void print_lista1(Antena *antena)
{
    while (antena != NULL) // Enquanto houver antenas na lista
    {
        printf("Antena: %c na posição (%d, %d)\n", antena->freq, antena->x, antena->y); // Imprime a lista de antenas
        antena = antena->next;
    }
}


/**
 * @brief Imprime a lista de nefastos.
 * 
 * @param lista 
 */
// Função para imprimir a lista de nefastos
void print_lista2(Nefasto *lista)
{
    while (lista != NULL)
    {                                                                // Enquanto houver nefastos na lista
        printf("Nefasto na posição (%d, %d)\n", lista->x, lista->y); // Imprime o nefasto
        lista = lista->next;                                         // Avança para o próximo nefasto
    }
}


/**
 * @brief Liberta a memória da lista de antenas.
 * 
 * @param antena 
 */
// Função para libertar a memória da lista de antenas
void free_lista1(Antena *antena)
{
    Antena *temp; // Cria um ponteiro temporário para percorrer a lista de antenas
    while (antena != NULL) // Enquanto houver antenas na lista
    {
        temp = antena; // Guarda o ponteiro atual
        antena = antena->next; // Avança para a próxima antena
        free(temp); // Liberta a memória da antena atual de forma a não perder o ponteiro
    }
}


/**
 * @brief Liberta a memoria da lista de nefastos.
 * 
 * @param lista 
 */
// Função para libertar a memória da lista de nefastos
void free_lista2(Nefasto *lista)
{
    // Limpa a lista toda
    Nefasto *temp;
    while (lista != NULL) // Enquanto houver nefastos na lista
    {
        temp = lista; // Guarda o ponteiro para o nefasto atual
        lista = lista->next; // Avança para o próximo nefasto
        free(temp); 
    }
}


/**
 * @brief Liberta a memória da matriz original.
 * 
 * @param matriz 
 * @param linhas 
 */
// Função para libertar a memória da matriz
void free_matriz(char **matriz, int linhas)
{
    for (int i = 0; i < linhas; i++)
    {
        free(matriz[i]);
    }
    free(matriz);
}

/**
 * @brief Liberta a memória da matriz nefasto.
 * 
 * @param matriz 
 * @param linhas 
 * @param colunas 
 */
// Função para libertar a memória da matriz nefasto 
void free_matriznefasto(char **matriz, int linhas, int colunas)
{
    for (int i = 0; i < linhas; i++)
    {                    // Percorre as linhas da matriz
        free(matriz[i]); // Liberta a memória da linha
    }
    free(matriz); // Liberta a memória da matriz
}










/*



                                        PARTE 2 PROJETO





"*/






int contadorID = 1;
/** @brief Função para separar as antenas por frequência e criar a lista de frequências.
 * 
 * @param listaAntena 
 * @param listafreq 
 */
// Função que separa as antenas por frequência, organizando-as em listas dentro da estrutura Frequencia
void separar_por_frequencia(Antena *listaAntena, Frequencia **listafreq) {
    // Percorre toda a lista de antenas fornecida
    while (listaAntena != NULL) {
        // Cria uma nova estrutura Grafo (representando uma antena no grafo)
        Grafo *novaAntena = (Grafo *)malloc(sizeof(Grafo)); // aloca memória para a nova antena
        if (!novaAntena) {
            printf("Erro de alocação de memória para nova antena.\n");
            return; // Encerra a função se não conseguir alocar memória
        }

        // Inicializa os dados da nova antena
        novaAntena->vertice = contadorID++;     // Atribui um ID único à antena
        novaAntena->freq = listaAntena->freq;   // Copia a frequência da antena original
        novaAntena->x = listaAntena->x;         // Copia a coordenada X
        novaAntena->y = listaAntena->y;         // Copia a coordenada Y
        novaAntena->listaadj = NULL;            // Inicializa a lista de adjacência como vazia
        novaAntena->next = NULL;                // Ainda não está ligada a outras antenas

        // Procura a frequência correspondente na lista de frequências
        Frequencia *f = *listafreq;  // Ponteiro para percorrer a lista de frequências
        Frequencia *anterior = NULL; // Ponteiro para manter referência do elemento anterior
        while (f != NULL && f->freq != novaAntena->freq) {
            anterior = f;
            f = f->next; // Avança na lista 
        }

        // Se a frequência ainda não existe na lista, cria uma nova entrada
        if (f == NULL) {
            f = (Frequencia *)malloc(sizeof(Frequencia)); // Aloca memória para a nova frequência
            if (!f) { //
                printf("Erro ao alocar memória para nova frequência.\n");
                free(novaAntena); // Liberta a antena alocada anteriormente
                return;
            }
            f->freq = novaAntena->freq;   // Define a nova frequência
            f->listagrafo = NULL;         // Lista de grafos (antenas) inicialmente vazia
            f->next = NULL;               // Ainda não ligada a outras frequências

            // Insere a nova frequência na lista de frequências
            if (anterior == NULL) {
                *listafreq = f;           // Primeira frequência da lista
            } else {
                anterior->next = f;       // Adiciona ao final da lista existente
            }
        }

        // Insere a nova antena no início da lista de grafos (antenas) da frequência correspondente
        novaAntena->next = f->listagrafo; // Liga a nova antena à lista atual
        f->listagrafo = novaAntena;       // Atualiza o início da lista com a nova antena

        // Avança para a próxima antena da lista originals
        listaAntena = listaAntena->next;
    }
}



/**
 * @brief Verifica se um vértice existe na lista de frequências.
 * 
 * @param listafreq 
 * @param vertice 
 * @return int 
 */
// Função para criar a lista de adjacentes para cada antena
void criarLL_adjacentes(Frequencia *listafreq) {
    for (Frequencia *f = listafreq; f != NULL; f = f->next) { // Percorre a lista de frequências
        for (Grafo *a = f->listagrafo; a != NULL; a = a->next) { // Percorre a lista de antenas (grafos) dentro da frequência

            int primeiro = 1; // Flag para verificar se é o primeiro adjacente (para evitar mostrar o ele proprio)
            for (Grafo *b = f->listagrafo; b != NULL; b = b->next) { // Percorre novamente a lista de antenas (grafos) dentro da frequência
                if (a->vertice != b->vertice) { // Se não for o mesmo vértice, mas for a mesma frequência então é adjacente
                    // Criar adjacente
                    Adjacente *novo = (Adjacente *)malloc(sizeof(Adjacente));
                    if (!novo) {
                        printf("Erro de memória ao criar adjacente.\n");
                        continue;
                    }
                    // Atribui os valores ao novo adjacente
                    novo->verticeadj = b->vertice;
                    novo->next = NULL;

                    // Adiciona ao final da lista de adjacentes de 'x'
                    if (a->listaadj == NULL || primeiro == 1) {
                        primeiro = 0; // Desativa a flag após o primeiro adjacente
                        a->listaadj = novo;
                    } else { // Se já houver adjacentes, percorre até o final da lista
                        Adjacente *temp = a->listaadj;
                        while (temp->next != NULL) {
                            temp = temp->next;
                        }
                        temp->next = novo;
                    }
                }
            }
        }
    }
}


/**
 * @brief Função para imprimir a lista de adjacentes de cada antena.
 * 
 * @param listafreq 
 */
// Função para imprimir a lista de adjacentes de cada antena
void print_lista_adj(Frequencia *listafreq) {
    if (listafreq == NULL) {
        printf("Nenhuma frequência encontrada.\n");
        return;
    } 
    while (listafreq != NULL) {
        printf("\nFrequência %c:\n", listafreq->freq);

        // Percorre a lista de antenas (grafos) dentro da frequência
        Grafo *antena = listafreq->listagrafo;
        while (antena != NULL) {
            printf("Vertice %d (%d,%d) -> ", antena->vertice, antena->x, antena->y);

            // Imprime os adjacentes do vértice
            Adjacente *adj = antena->listaadj;
            if (adj == NULL) {
                printf("Nenhum adjacente");
            } else {
                while (adj != NULL) {
                    printf("%d ", adj->verticeadj);
                    adj = adj->next;
                }
            }

            printf("\n");
            antena = antena->next;
        }

        listafreq = listafreq->next;
        printf("\n"); // linha extra entre frequências para melhor visualização
    }
}


/**
 * @brief Função para imprimir a lista de frequências e suas antenas.
 * 
 * @param listafreq 
 */
// Função para dar printf da lista de frequências e suas antenas
void print_lista_freq(Frequencia *listafreq) {
    if (listafreq == NULL) {
        printf("Nenhuma frequência encontrada.\n");
        return;
    }
    // Percorre a lista de frequências
    while (listafreq != NULL) {
        printf("\nFrequência: %c\n", listafreq->freq);

        // Percorre a lista de antenas (grafos) dentro da frequência
        Grafo *antena = listafreq->listagrafo;
        while (antena != NULL) {
            printf("ID: %d | Coordenadas: (%d, %d)\n", antena->vertice, antena->x, antena->y);
            antena = antena->next;
        }

        listafreq = listafreq->next;
    }
    free_lista_freq(listafreq); // Liberta a memória da lista de frequências
}


/**
 * @brief Inicia a busca em profundidade a partir de um vértice inicial.
 * 
 * @param listasfreq 
 * @param vertice_inicial 
 * @param visitado 
 * @param num_vertices 
 */
// Função para iniciar a busca em profundidade a partir de um vértice inicial
void iniciar_profundidade(Frequencia *listasfreq, int vertice_inicial, int *visitado, int num_vertices) {
    printf("\nCom o vértice %d em procura em profundidade tenho estas opções:\n", vertice_inicial);

    // Alocar memória para o caminho
    int *caminho = malloc(num_vertices * sizeof(int)); 
    int tam_max_caminho = 0; // Tamanho máximo do caminho

    // Iniciar a busca em profundidade a partir do vértice inicial
    procurar_profundidade(listasfreq, vertice_inicial, visitado, caminho, &tam_max_caminho); //Chama a função de busca em profundidade

    // Liberar memória do caminho após o uso
    free(caminho);
}


/**
 * @brief Função para procurar em profundidade a partir de um vértice.
 * 
 * @param listasfreq 
 * @param vertice 
 * @param visitado 
 * @param caminho 
 * @param tam_max_caminho 
 */
// Função para realizar busca em profundidade a partir de um vértice
void procurar_profundidade(Frequencia *listasfreq, int vertice, int *visitado, int *caminho, int *tam_max_caminho) {
    // Marcar o vértice como visitado 
    visitado[vertice] = 1;

    // Adicionar o vértice ao caminho atual
    caminho[*tam_max_caminho] = vertice; // Adiciona o vértice atual ao caminho
    (*tam_max_caminho)++; // Incrementa o tamanho do caminho

    // Exibir o caminho atual
    printf("--> Caminho: ");
    for (int i = 0; i < *tam_max_caminho; i++) { // Imprime o caminho atual
        printf("%d ", caminho[i]);
    }
    printf("\n");

    // Procurar o vértice na estrutura do grafo
    Frequencia *f = listasfreq;
    while (f != NULL) {
        Grafo *g = f->listagrafo;

        // Percorrer a lista de grafos até encontrar o vértice atual
        while (g != NULL && g->vertice != vertice) {
            g = g->next;
        }

        if (g != NULL) {
            // Se encontrou, percorre os adjacentes do vértice
            Adjacente *adj = g->listaadj;
            while (adj != NULL) {
                // Se o vértice adjacente ainda não foi visitado, visita recursivamente
                if (!visitado[adj->verticeadj]) {
                    procurar_profundidade(listasfreq, adj->verticeadj, visitado, caminho, tam_max_caminho);
                }
                // Avança para o próximo adjacente
                adj = adj->next;
            }
        }

        // Passa para o próximo elemento da lista de frequências
        f = f->next;
    }

    // Ao voltar da recursão, desfaz o passo atual:
    (*tam_max_caminho)--;   // Remove o vértice atual do caminho
    visitado[vertice] = 0;  // Marca o vértice como não visitado para permitir outras buscas a partir dele
}


/**
 * @brief Inicia a busca em largura a partir de um vértice inicial.
 * 
 * @param listaFreq 
 * @param inicio 
 */
// Função para buscar em largura a partir de um vértice inicial
void busca_em_largura(Frequencia *listaFreq, int inicio) {
    int visitado[MAX_VERTICES] = {0}; // Marca os vértices visitados
    int fila[MAX_VERTICES]; // Fila para armazenar os vértices a serem visitados
    int frente = 0, tras = 0; // Índices para a fila

    
    fila[tras++] = inicio; // Adiciona o vértice inicial à fila
    visitado[inicio] = 1; // Marca o vértice inicial como visitado

    printf("Busca em largura a partir do vértice %d:\n", inicio); 

    while (frente < tras) { // Enquanto houver vértices na fila
        int atual = fila[frente++]; // Remove o primeiro vértice da fila

        // Ciclo for para percorrer as frequências
        for (Frequencia *f = listaFreq; f != NULL; f = f->next) { // Percorre a lista de frequências
            for (Grafo *g = f->listagrafo; g != NULL; g = g->next) { // Percorre a lista de antenas (grafos) dentro da frequência
                if (g->vertice == atual) { // Se o vértice atual for encontrado
                    
                    if (atual != inicio) { // Só imprime se não for o vértice de início
                        printf("Vértice %d (%d, %d)\n", g->vertice, g->x, g->y); // Imprime o vértice atual e suas coordenadas
                    }
                    for (Adjacente *adj = g->listaadj; adj != NULL; adj = adj->next) { // Percorre a lista de adjacentes do vértice atual
                        int v_adj = adj->verticeadj; // Obtém o vértice adjacente
                        if (!visitado[v_adj]) { // Se o vértice adjacente não foi visitado
                            fila[tras++] = v_adj; // Adiciona o vértice adjacente à fila
                            visitado[v_adj] = 1; // Marca o vértice adjacente como visitado
                        }
                    }
                    break;
                }
            }
        }
    }
}


/**
 * @brief Obtém o vértice correspondente a um ID na lista de frequências.
 * 
 * @param listasfreq 
 * @param vertice 
 * @return Grafo* 
 */
// Função para obter o vértice correspondente a um ID
Grafo* obter_vertice(Frequencia *listasfreq, int vertice) {
    while (listasfreq != NULL) {
        Grafo *g = listasfreq->listagrafo;
        while (g != NULL) {
            if (g->vertice == vertice)
                return g;
            g = g->next;
        }
        listasfreq = listasfreq->next;
    }
    return NULL;
} 


/**
 * @brief Calcula todos os caminhos entre dois vértices no grafo.
 * 
 * @param atual 
 * @param destino 
 * @param visitado 
 * @param caminho 
 * @param pos 
 * @param listasfreq 
 */
// Função para calcular todos os caminhos entre dois vértices
void calcular_caminhos(Grafo *atual, int destino, int visitado[], int caminho[], int pos, Frequencia *listasfreq) {
    visitado[atual->vertice] = true; // marca o vértice atual como visitado
    caminho[pos++] = atual->vertice; // adiciona o vértice atual ao caminho

    if (atual->vertice == destino) { // se o vértice atual é o destino
        for (int i = 0; i < pos; i++) { // imprime o caminho encontrado
            printf("%d", caminho[i]); // imprime o vértice
            if (i < pos - 1) printf(" -> "); // imprime a seta entre os vértices
        }
        printf("\n");
    } else { 
        Adjacente *adj = atual->listaadj; // obtém a lista de adjacentes do vértice atual
        while (adj != NULL) { 
            if (!visitado[adj->verticeadj]) { // se o vértice adjacente não foi visitado
                Grafo *prox = obter_vertice(listasfreq, adj->verticeadj); // obtém o vértice adjacente correspondente
                if (prox) // se o vértice adjacente existe
                    // chama recursivamente a função para continuar o caminho
                   calcular_caminhos(prox, destino, visitado, caminho, pos, listasfreq);
            }
            adj = adj->next;
        }
    }

    visitado[atual->vertice] = false; // desmarca o vértice atual como visitado para permitir outros caminhos
}


/**
 * @brief Imprime todos os caminhos entre dois vértices.
 * 
 * @param listasfreq 
 * @param origem 
 * @param destino 
 */
// Função para mostrar caminhos entre dois vértices
void print_caminhos(Frequencia *listasfreq, int origem, int destino) {
    int visitado[100] = { false }; // Inicializa o vetor de visitados
    int caminho[100]; // Array para armazenar o caminho atual

    Grafo *start = obter_vertice(listasfreq, origem); // Obtém o vértice de origem
    if (!start) { // Verifica se o vértice de origem existe
        printf("Vértice de origem %d não encontrado.\n", origem);
        return;
    }

    printf("Do vértice %d até ao vértice %d tenho estas opções de caminhos:\n", origem, destino); // Informa os vértices de origem e destino
    calcular_caminhos(start, destino, visitado, caminho, 0, listasfreq); // Calcula e imprime todos os caminhos entre os vértices de origem e destino
}


/**
 * @brief Verifica interseções entre segmentos de reta de duas frequências.
 * 
 * @param lista 
 * @param tipoA 
 * @param tipoB 
 */
// Função para verificar interseções entre segmentos de reta de duas frequências
void ver_interseçoes(Frequencia *lista, char tipoA, char tipoB) {
    Frequencia *fa = obter_frequencia(lista, tipoA); // Obtém a frequência A
    Frequencia *fb = obter_frequencia(lista, tipoB); // Obtém a frequência B

    if (!fa || !fb) { // Verifica se as frequências existem
        printf("Erro: Frequência %c ou %c não encontrada.\n", tipoA, tipoB);
        return;
    }

    bool encontrou = false; // Flag para verificar se encontrou interseções

    for (Grafo *a = fa->listagrafo; a; a = a->next) { // Percorre os vértices da frequência A
        for (Adjacente *adjA = a->listaadj; adjA; adjA = adjA->next) { // Percorre os adjacentes do vértice A
            Grafo *a2 = obter_vertice(fa, adjA->verticeadj); // Obtém o vértice adjacente A2
            if (!a2 || a->vertice > a2->vertice) continue; // Garante que A2 é maior que A para evitar duplicação de segmentos

            for (Grafo *b = fb->listagrafo; b; b = b->next) { // Percorre os vértices da frequência B
                for (Adjacente *adjB = b->listaadj; adjB; adjB = adjB->next) { // Percorre os adjacentes do vértice B
                    Grafo *b2 = obter_vertice(fb, adjB->verticeadj); // Obtém o vértice adjacente B2
                    if (!b2 || b->vertice > b2->vertice) continue; // Garante que B2 é maior que B para evitar duplicação de segmentos

                    float ix, iy;

                    if (cruzar_segmentos(a->x, a->y, a2->x, a2->y,
                                         b->x, b->y, b2->x, b2->y, &ix, &iy)) {
                        printf("Segmento A(%d,%d)-->(%d,%d) VS B(%d,%d)-->(%d,%d) --> Interseção em (%.2f; %.2f)\n",
                            a->x, a->y, a2->x, a2->y,
                            b->x, b->y, b2->x, b2->y,
                            ix, iy);
                        encontrou = true; // marca que encontrou pelo menos uma
                    }
                }
            }
        }
    }

    if (!encontrou) {
        printf("Nenhuma interseção encontrada entre as frequências %c e %c.\n", tipoA, tipoB);
    }
}


/**
 * @brief Verifica se dois segmentos de reta se cruzam e retorna o ponto de interseção.
 * 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @param x3 
 * @param y3 
 * @param x4 
 * @param y4 
 * @param ix 
 * @param iy 
 * @return int 1 se cruzam, 0 se não cruzam
 */
// Função para verificar se dois segmentos de reta se cruzam
int cruzar_segmentos(int x1, int y1, int x2, int y2,
                       int x3, int y3, int x4, int y4,
                       float *ix, float *iy) {
    int A1 = y2 - y1; // Coeficiente em y do primeiro segmento
    int B1 = x1 - x2; // Coeficiente em x do primeiro segmento    
    int C1 = A1 * x1 + B1 * y1; // Equação da reta 1: Ax + By = C

    int A2 = y4 - y3; // Coeficiente em y do segundo segmento
    int B2 = x3 - x4; // Coeficiente em x do segundo segmento
    int C2 = A2 * x3 + B2 * y3; // Equação da reta 2: Ax + By = C

    int det = A1 * B2 - A2 * B1; // Determinante para verificar se as retas são paralelas
    if (det == 0) // Se o determinante for zero, as retas são paralelas(dado em MDAL)
        return 0; // Retas paralelas

    *ix = (float)(B2 * C1 - B1 * C2) / det; // Calcula o ponto de interseção em x usando a regra de Cramer
    *iy = (float)(A1 * C2 - A2 * C1) / det; // Calcula o ponto de interseção em y 

    // Verifica se está dentro dos dois segmentos
    if (*ix >= fmin(x1, x2) && *ix <= fmax(x1, x2) &&
        *ix >= fmin(x3, x4) && *ix <= fmax(x3, x4) &&
        *iy >= fmin(y1, y2) && *iy <= fmax(y1, y2) &&
        *iy >= fmin(y3, y4) && *iy <= fmax(y3, y4)) // Verifica se o ponto de interseção está dentro dos limites dos segmentos 
        return 1;

    return 0;
}



/**
 * @brief Obtém a frequência correspondente a um caractere.
 * 
 * @param listasfreq 
 * @param freq 
 * @return Frequencia* 
 */
// Função auxiliar para obter a frequência
Frequencia* obter_frequencia(Frequencia *listasfreq, char freq) { // Objetivo: fornecer o ponteiro para a frequência correspondente ao caractere dado pelo user
    while (listasfreq != NULL) {
        if (toupper(listasfreq->freq) == toupper(freq)) { // Verifica se a frequência é igual à procurada
            // Se for, retorna o ponteiro para a frequência
            return listasfreq;
        }
        listasfreq = listasfreq->next; // Avança para a próxima frequência
    }
    return NULL;
}


/**
 * @brief Liberta a memória da lista de frequências e suas antenas.
 * 
 * @param listafreq 
 */
// Liberta a memória da lista de frequências e suas antenas
void free_lista_freq(Frequencia *listafreq) {
    while (listafreq != NULL) {
        Frequencia *temp = listafreq;
        listafreq = listafreq->next;

        // Liberta a lista de adjacentes
        for (Grafo *g = temp->listagrafo; g != NULL;) {
            Grafo *tempG = g;
            g = g->next;
            free(tempG);
        }

        free(temp);
    }
}


/** 
 * @brief Liberta a memória da lista de adjacentes.
 * 
 * @param listaadj 
 */
// Liberta a memória da lista de adjacentes
void free_lista_adj(Adjacente *listaadj) {
    while (listaadj != NULL) {
        Adjacente *temp = listaadj;
        listaadj = listaadj->next;
        free(temp);
    }
}


/** 
 * @brief Verifica se dois vértices pertencem à mesma frequência.
 * 
 * @param listasfreq 
 * @param v1 
 * @param v2 
 * @return int 
 */
// Verifica se dois vértices pertencem à mesma frequência
int mesma_frequencia(Frequencia *listasfreq, int v1, int v2) {
    while (listasfreq != NULL) {
        int encontrou_v1 = 0, encontrou_v2 = 0; // Flags pra verificar se os vértices foram encontrados
        Grafo *g = listasfreq->listagrafo; // Percorre a lista de antenas

        while (g != NULL) {
            if (g->vertice == v1) encontrou_v1 = 1;
            if (g->vertice == v2) encontrou_v2 = 1;
            g = g->next;
        }

        if (encontrou_v1 && encontrou_v2) { // Se ambos forem igual a 1, significa que ambos estão na mesma frequência
            return 1; // Ambos estão nesta frequência
        }

        listasfreq = listasfreq->next;
    }

    return 0; // Não pertencem à mesma frequência
}


/**
 * @brief Verifica se um vértice existe na lista de frequências.
 * 
 * @param listasfreq 
 * @param vertice 
 * @return int 
 */
// Verifica se um vértice existe na lista de frequências
int vertice_existe(Frequencia *listasfreq, int vertice) {
    while (listasfreq != NULL) {
        Grafo *g = listasfreq->listagrafo; // Percorre a lista de antenas
        while (g != NULL) { 
            if (g->vertice == vertice) {
                return 1; // Vértice encontrado
            }
            g = g->next;
        }
        listasfreq = listasfreq->next;
    }
    return 0; // Vértice não encontrado
}


/**
 * @brief Verifica se uma frequência já existe na lista de frequências.
 * 
 * @param listasfreq 
 * @param freq 
 * @return int 
 */
// Verifica se uma frequência já existe na lista de frequências
int frequencia_existe(Frequencia *listasfreq, char freq) {
    while (listasfreq != NULL) {
        if (listasfreq->freq == freq) {
            return 1; // Frequência encontrada
        }
        listasfreq = listasfreq->next;
    }
    return 0; // Frequência não encontrada
}