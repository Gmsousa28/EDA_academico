/**
 * @file Projetomain1.c
 * @author Gonçalo Sousa
 * @brief Menu principal do programa, onde o utilizador pode escolher o que pretende fazer.
 * @version 0.1
 * @date 2025-03-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "Projeto1.h"


/**
 * @brief Todo um menu usando switch case com várias opções para o utilizador de forma a relizar o pretendido.
 * 
 * @return int 
 */
int main()
{

    setlocale(LC_ALL, "pt_PT.UTF-8"); 

//definição de variáveis
    int opcao, linhas, colunas, x, y, v; //Variáveis para armazenar as linhas, colunas e coordenadas da matriz
    int jaSeparado = 0; //Variável para verificar se a lista de antenas por frequência já foi criada
    char filename[100], freq;
    int visitado[100] = {0}; // Array para armazenar os vértices visitados
    int caminho [100]; // Array para armazenar o caminho
    int tam_max_caminho = 0; // Número de vértices no caminho
    
    char **matriz = NULL; //Matriz que armazena os dados do ficheiro
    Antena *antena = NULL; //Lista que armazena as antenas
    Nefasto *lista = NULL;  //Lista que armazena os nefastos
    Grafo *listagrafo = NULL; //Lista que armazena o grafo das antenas
    Frequencia *listasfreq = NULL; //Lista que armazena as antenas por frequência
    Adjacente *listaadj = NULL; //Lista que armazena os adjacentes das antenas



/** 
 * @brief Função principal do programa funciona como um menu com 5 opções, desde adicionar e remover antenas até imprimir a matriz com o efeito nefasto.
 */

//Menu incial do programa
    do
    {
        printf("\n############--Menu--###############\n");
        printf("1.  Inserir o ficheiro do tipo txt\n");
        printf("2.  Adicionar antena\n");
        printf("3.  Remover antena\n");
        printf("4.  Ver a matriz com efeito nefasto e listar antenas\n");
        printf("5.  Ver antenas atuais\n\n");
        printf("\n#--------------Parte 2--------------#\n");
        printf("6.  Grafos\n");
        printf("7.  Procura em profundidade e largura\n");
        printf("8.  Caminhos entre 2 vertices\n");
        printf("9.  Interseção de frequências\n");
        printf("0.  Sair\n");
        printf("###################################\n");
        printf("Escolha uma opção: ");


    
        if (scanf("%d", &opcao) != 1 || opcao < 0 || opcao > 9) //se a opção não for um número ou for menor que 0 ou maior que 6 dá erro
        {
            printf("Entrada inválida.\n");
            while (getchar() != '\n')
                ; // Limpar o buffer de entrada
            continue;
        }
        while (getchar() != '\n')
            ; // Limpar o buffer

        switch (opcao)
        {
        case 1:
            printf("Digite o nome do ficheiro de entrada: ");
            scanf("%s", filename);
            antena = ler_ficheiro(filename, &matriz, &linhas, &colunas); //Lê o ficheiro, e armazena a matriz e as antenas
            if (matriz == NULL) //se a matriz for nula(não existir) dá erro
            {
                printf("Erro ao ler o ficheiro!\n");
            }
            else
            {
                printf("Ficheiro lido com sucesso!\n");
            }
            break;

        case 2:
            if (matriz == NULL)//Verifica se a matriz foi carregada
            {
                printf("A matriz ainda não foi carregada. Carrega o ficheiro primeiro.\n");
                break;
            }
            printf("Lista de antenas atuais:\n");
            print_lista1(antena); 
            printf("Digite a frequência da antena (A, B, 0, 1): "); //Pede a frequência da antena
            if (scanf(" %c", &freq) != 1 || (freq != 'A' && freq != 'B' && freq != '0' && freq != '1')) // Verifica a validade da frequencia
            {
                printf("Entrada inválida.\n");
                while (getchar() != '\n'); // Limpar o buffer de entrada
                continue;
            }
            printf("Digite a posição X (linha) entre 0 e %d: ", linhas - 1); //Pede a posição X da antena na matriz
            scanf("%d", &x);
            if (x < 0 || x >= linhas) //Verifica se a posição X é válida
            {
                printf("Posição inválida. Tenta novamente.\n");
                continue;
            }
            printf("Digite a posição Y (coluna) entre 0 e %d: ", colunas - 1); //Pede a posição Y da antena na matriz
            scanf("%d", &y);
            if (y < 0 || y >= colunas) //Verifica se a posição Y é válida
            {
                printf("Posição inválida. Tenta novamente.\n");
                continue;
            }
            if (antena_existe(antena, x, y)) //Verifica se a antena já existe
            {
                printf("Já existe uma antena nessa posição. Tenta novamente.\n");
                continue;
            }
            adicionar_antena(&antena, &matriz, &linhas, &colunas, freq, x, y); //Adiciona a antena à matriz e à lista
            printf("Antena adicionada com sucesso!\n"); 
            printf("Matriz atualizada:\n");
            print_matriz(matriz, linhas, colunas);
            print_lista1(antena); 
            break;

        case 3:
            if (matriz == NULL) //Verifica se a matriz foi carregada
            {
                printf("A matriz ainda não foi carregada. Carrega o ficheiro primeiro.\n");
                break;
            }
            printf("Lista de antenas atuais:\n");
            print_lista1(antena); 
            printf("Digite as coordenadas da antena a remover (x y): "); //Pede as coordenadas da antena a remover
            scanf("%d %d", &x, &y);
            if (x < 0 || x >= linhas || y < 0 || y >= colunas) //Verifica se as coordenadas são válidas
            {
                printf("Coordenadas inválidas. Tenta novamente.\n");
                continue;
            }
            remover_antena(&antena, x, y, &matriz); //Remove a antena da matriz e da lista
            printf("Antena removida com sucesso!\n");
            printf("Matriz atualizada e lista atualizada:\n");
            print_matriz(matriz, linhas, colunas); 
            print_lista1(antena); 
            break;

        case 4:
            if (matriz == NULL)
            {
                printf("A matriz ainda não foi carregada. Carrega o ficheiro primeiro.\n");
                break;
            }
            aplicar_nefasto(antena, linhas, colunas, &lista); //Aplica o efeito nefasto
            printf("\nMatriz com o efeito nefasto aplicado:\n");
            print_matriznefasto(matriz, linhas, colunas, lista); //Imprime a matriz com o efeito nefasto
            printf("\nLista de nefastos:\n"); //Imprime a lista de antenas
            print_lista2(lista);
            printf("\n\n");
            break;

        case 5: 
            if (matriz == NULL) //Verifica se a matriz foi carregada
            {
                printf("A matriz ainda não foi carregada. Carrega o ficheiro primeiro.\n");
                break;
            }
            printf("Lista de antenas atuais:\n");
            print_matriz(matriz, linhas, colunas); //Imprime a matriz
            print_lista1(antena); //Imprime a lista de antenas
            break;

        case 6:
            if (matriz == NULL) //Verifica se a matriz foi carregada
            {
                printf("A matriz ainda não foi carregada. Carrega o ficheiro primeiro.\n");
                break;
            }
            
            if (!jaSeparado) {
                separar_por_frequencia(antena, &listasfreq);
                jaSeparado = 1;
            }
            printf("\n////////////--Grafo--////////////\n");
            printf("1. Ver Grafo\n");
            printf("2. Ver lista de adjacentes\n");
            printf("Escolha uma opção: ");
            if (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 2) //Verifica a validade da opção
            {
                printf("Entrada inválida.\n");
                while (getchar() != '\n')
                    ; // Limpar o buffer de entrada
                continue;
            }
            switch (opcao) //Verifica a opção escolhida
            {
            case 1: // VER GRAFO
                printf("\nLista de antenas por frequência:\n");
                print_lista_freq(listasfreq); //Imprime a lista de antenas por frequência
                break;
            case 2: // VER LISTA DE ADJACENTES
                printf("\nLista de adjacentes:\n");
                criarLL_adjacentes(listasfreq); //Cria a lista de adjacentes
                print_lista_adj(listasfreq); //Imprime a lista de adjacentes
                break;
            
            default:
                break;
            }
            break;

        case 7:
            if (matriz == NULL) //Verifica se a matriz foi carregada
            {
                printf("A matriz ainda não foi carregada. Carrega o ficheiro primeiro.\n");
                break;
            }
            if (!jaSeparado) {
                free_lista_adj(listaadj); //Liberta a lista de adjacentes
                separar_por_frequencia(antena, &listasfreq);
                criarLL_adjacentes(listasfreq); //Cria a lista de adjacentes
                jaSeparado = 1;
            }
            printf("Escolha o tipo de procura:\n");
            printf("1. Profundidade\n");
            printf("2. Largura\n");
            printf("Escolha uma opção: ");
            if (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 2) //Verifica a validade da opção
            {
                printf("Entrada inválida.\n");
                while (getchar() != '\n')
                    ; // Limpar o buffer de entrada
                continue;
            }
            switch (opcao) //Verifica a opção escolhida
            {
            case 1: // Busca em profundidade
                printf("Escolha o número de vertice: ");
                scanf("%d", &v);
                if (!vertice_existe(listasfreq, v)) //Verifica se o vertice existe
                {
                    printf("Vertice inválido. Tenta novamente.\n");
                    continue;
                }
                if(visitado[v] == 1) //Verifica se o vertice já foi visitado
                {
                    printf("Vertice já visitado. Tenta novamente.\n");
                    continue;}
                    iniciar_profundidade(listasfreq, v, visitado, tam_max_caminho); //Inicia a busca em profundidade
                break;
            case 2: // Busca em largura
                printf("Escolha o número de vertice: ");
                scanf("%d", &v);
                if (!vertice_existe(listasfreq, v)) //Verifica se o vertice existe
                {
                    printf("Vertice inválido. Tenta novamente.\n");
                    continue;
                }
                busca_em_largura(listasfreq, v); //Inicia a busca em largura
                break;
            default:
                break;
            }
            break;
        case 8:
            if (matriz == NULL) //Verifica se a matriz foi carregada
            {
                printf("A matriz ainda não foi carregada. Carrega o ficheiro primeiro.\n");
                break;
            }
            if (!jaSeparado) {
                free_lista_adj(listaadj); //Liberta a lista de adjacentes
                separar_por_frequencia(antena, &listasfreq);
                criarLL_adjacentes(listasfreq); //Cria a lista de adjacentes
                jaSeparado = 1;
            }
            printf("Escolha o número de vertice inicial: ");
            scanf("%d", &x);
            if(!vertice_existe(listasfreq, x)) //Verifica se o vertice existe
            {
                printf("Vertice inválido. Tenta novamente.\n");
                continue;
            }
            printf("Escolha o número de vertice final: ");
            scanf("%d", &y);
            if(!vertice_existe(listasfreq, y)) //Verifica se o vertice existe
            {
                printf("Vertice inválido. Tenta novamente.\n");
                continue;
            }
            if (!mesma_frequencia(listasfreq, x, y)) //Verifica se os vertices pertencem à mesma frequência
            {
                printf("Os vértices %d e %d não pertencem à mesma frequência.\n", x, y);
                continue;
            }
            obter_vertice(listasfreq, x); //Obtem o vertice inicial
            obter_vertice(listasfreq, y); //Obtem o vertice final   
            print_caminhos(listasfreq, x, y); //Aplica a busca de caminhos entre os dois vertices

            break;
        case 9:
            if (matriz == NULL) //Verifica se a matriz foi carregada
            {
                printf("A matriz ainda não foi carregada. Carrega o ficheiro primeiro.\n");
                break;
            }
            if (!jaSeparado) {
                free_lista_adj(listaadj); //Liberta a lista de adjacentes
                separar_por_frequencia(antena, &listasfreq);
                criarLL_adjacentes(listasfreq); //Cria a lista de adjacentes
                jaSeparado = 1;
            }
            printf("Matriz disponivel:\n");
            print_matriz(matriz, linhas, colunas);
            printf("\nEscolha a primeira frequencia: ");
            scanf(" %c", &x);
            if(!frequencia_existe(listasfreq, x)) //Verifica se a frequencia existe
            {
                printf("Frequência inválida. Tenta novamente.\n");
                continue;
            }
            printf("Escolha a segunda frequencia: ");
            scanf(" %c", &y);
            if(!frequencia_existe(listasfreq, y)) //Verifica se a frequencia existe
            {
                printf("Frequência inválida. Tenta novamente.\n");
                continue;
            }
            ver_interseçoes(listasfreq, x, y); //Verifica as interseções entre as duas frequencias

            break;
        
        case 0:
            printf("\nA encerrar o programa. Obrigado!\n");
            break;

        default:
            printf("Opção inválida. Tenta novamente.\n");
            break;
        }
    }while (opcao != 0);
    
    if (matriz != NULL)
        free_matriz(matriz, linhas);
    if (antena != NULL)
        free_lista1(antena);
    if (lista != NULL)
        free_lista2(lista); 
    
    return 0;
}
