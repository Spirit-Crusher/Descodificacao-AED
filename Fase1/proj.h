/*#####################################################################
#                  Projecto AED - Padrão secreto p.1                  #
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

void ler_ficheiro(FILE* ficheiro, int v_linhas[], int v_colunas[], int linhas, int colunas);
void calculo_possibildades(FILE* output, int PL[], int PC[], int linhas, int colunas, int v_linhas[], int v_colunas[]);
