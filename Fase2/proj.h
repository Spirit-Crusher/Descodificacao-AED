/*#####################################################################
#                  Projecto AED - Padrão secreto p.2                  #
#                                                                     #
# Nome: proj.h                                                        #
#                                                                     #
# Descrição: Ficheiro que contém definições e protótipos que serão    #
# necessários para a correta execução do programa.                    #
#                                                                     #
# Autores:  Rodrigo Caldeira nº106963                                 #
#           Guilherme Garcia nº105917                                 #
#           Mariana Domingos nº105988                                 #
#####################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct piece
{
int cR, cC;    /*Coordenadas da peça (linhas e colunas, respetivamente)*/
int size;    /*Tamanho da peça*/
struct piece* previous_piece;    /*Apontador para ser usado na pilha*/
} PIECE;

/*Valores criados para tornar código mais legível*/
#define ERROR -1
#define UNDEF 1
#define SUCCESS 0
#define SUB 1
#define ADD 2

/*Protótipos das funções usadas*/
void read_file(FILE* ficheiro, int v_linhas[], int v_colunas[], int linhas, int colunas);
void possibility_calculator(int PR[], int PC[], int rows, int columns, int v_rows[], int v_columns[]);
int evaluatePosition (PIECE* currentPiece, int* squareCounter);
void updateState (int mode, int* squareCounter, PIECE* currentPiece);
PIECE* removeLastPiece (PIECE* pieceStack);
void stackRelease (PIECE* pieceStack);
PIECE* pieceCreator (int tier, int* evaluated_vector);
void add2Stack (PIECE** currentPiece, PIECE** pieceStack);
void solutionWriter(PIECE* pieceStack, char* solutionMatrice, FILE* output_file, int rows, int columns);