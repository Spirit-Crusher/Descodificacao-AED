/*#####################################################################
#                  Projecto AED - Padrão secreto p.1                  #
#                                                                     #
# Nome: proj.c                                                        #
#                                                                     #
# Descrição: Ficheiro que contém funções principais associadas à      #
# leitura dos dados e cálculo das possibilidades para cada codifi-    #
# cação.                                                              #
#                                                                     #
# Autores:  Rodrigo Caldeira nº106963                                 #
#           Guilherme Garcia nº105917                                 #
#           Mariana Domingos nº105988                                 #
#####################################################################*/

#include "proj.h"


int main(int argc, char* argv[]) 
{
  int linhas, colunas;
  FILE* input;
	FILE* output;
  int* v_linhas;
  int* v_colunas;
  int* PL;
  int* PC;
  char* sPointer;

	
  if(argc != 2)
  {
    printf("Erro: Argumentos inválidos.\n");  /*O comando de execução do programa deve ser seguido pelo nome do ficheiro a analisar.*/
    return 0;
  }

  input = fopen(argv[1], "r");                  /*Abertura do ficheiro fornecido de modo a ser possível ler conteúdo.*/
  if(input == NULL) 
  {
    printf("Erro: Abertura do ficheiro de entrada sem sucesso.\n");     /*Se fopen() retornar NULL houve um problema relacionado com a abertura do ficheiro.*/
    return 0;
  }

  /*Operações responsáveis por manipular string de entrada de modo a obter a string de saída respectiva. (Desta maneira o ficheiro de saída terá o nome correto).*/
  sPointer = strchr(argv[1], '.');    /*Procurar '.' no nome do ficheiro e colocar endereço num apontador.*/
  *sPointer = '\0';                   /*Colocar nessa posição o caracter '\0' de forma a "cortar" a string.*/
  strcat(argv[1], ".sol1");           /*Adicionar extensão correta.*/

  output = fopen(argv[1], "w");   /*Abertura ou criação (caso inexistente) do ficheiro onde será escrita solução.*/
  if(output == NULL) 
  {
    printf("Erro: Abertura do ficheiro de saída mal-sucedida.\n");  /*De forma semelhante ao ficheiro de entrada, caso não seja possível abrir ficheiro de saída, deve ser apresentada mensagem de erro.*/
    return 0;
  }

  fscanf(input, "%d %d", &linhas, &colunas);       /*Leitura da primeira linha do ficheiro de modo a obter dimensão da matriz em questão.*/

 /*Agora que temos dimensões podemos alocar a memória necessária para guardar todas as variáveis.*/
  v_linhas = (int*) malloc(linhas * sizeof(int));  	 /*Vector que guarda dados referentes às linhas.*/
  v_colunas = (int*) malloc(colunas * sizeof(int));  /*Igual ao anterior mas para as colunas.*/
  PL = (int*) malloc(linhas * sizeof(int));          /*Vector que guarda possibilidades de colocar quadrados em cada linha*/
  PC = (int*) malloc(colunas * sizeof(int));         /*Igual ao anterior mas para as colunas.*/

  ler_ficheiro(input, v_linhas, v_colunas, linhas, colunas);
  calculo_possibildades(output, PL, PC, linhas, colunas, v_linhas, v_colunas);
  
  fclose(input);  /*Fechar ficheiro de entrada após execução do programa.*/
  fclose(output);   /*Fechar ficheiro após todas as operações de escrita terem terminado.*/

/*Libertação da memória dinâmica antes de terminar o programa.*/
  free(PC);
  free(PL);
  free(v_linhas);
  free(v_colunas);

  printf("Sucesso.\n");  /*Mensagem de sucesso*/

  return 0;
}

/*Função responsável por ler os dados a partir do ficheiro de entrada recebido através da linha de comando.*/
void ler_ficheiro(FILE* input, int v_linhas[], int v_colunas[], int linhas, int colunas)
{
	int i;
	for(i = 0; i < linhas; i++) fscanf(input, "%d", &v_linhas[i]);     /*Leitura de dados referentes a linhas e colocação da informação lida na posição correta do vector correspondente.*/
	for(i = 0; i < colunas; i++) fscanf(input, "%d ", &v_colunas[i]);  /*Igual ao anterior mas para as colunas.*/
}

/*Função responsável por calcular possibilidades para cada linha e coluna.*/
void calculo_possibildades(FILE* output, int PL[], int PC[], int linhas, int colunas, int v_linhas[], int v_colunas[])
{
  int i;

  /*Loop responsável por escrever no ficheiro de saída as possibilidades para cada coluna.*/
  for(i = colunas-1; i >= 0; i--)
  {
	  if(v_colunas[i] == 0) fprintf(output, "C %d 0\n", i+1);   /*Caso o nº de quadrados seja 0, não faz sentido calcular possibilidades, sendo apresentado o valor 0.*/
	  else
	  {
      PC[i] = linhas - v_colunas[i] + 1;                      /*Cálculo das possibilidades com base na fórmula desenvolvida.*/
      fprintf(output, "C %d %d\n", i+1, PC[i]);               /*Escrita do resultado no ficheiro de saída.*/
	  }
  }

  /*Loop exatamente igual ao anterior mas adaptado às linhas.*/
  for(i=linhas-1; i >= 0; i--)
  {
    if(v_linhas[i] == 0) fprintf(output, "L %d 0\n",i+1);
    else
    {
      PL[i] = colunas - v_linhas[i] + 1;
      fprintf(output, "L %d %d\n",i+1, PL[i]);
    }
  }
}
