/*#####################################################################
#                  Projecto AED - Padrão secreto p.2                  #
#                                                                     #
# Nome: proj.c                                                        #
#                                                                     #
# Descrição: Ficheiro que contém funções associadas à      			      #
# leitura e descodificação do padrão fornecido                        #
#                                                                     #
# Autores:  Rodrigo Caldeira nº106963                                 #
#           Guilherme Garcia nº105917                                 #
#           Mariana Domingos nº105988                                 #
#####################################################################*/

#include "proj.h"

int main(int argc, char *argv[]) {
  
  int i, validity = ERROR, rows, columns, tier = 0;
  bool fromStack = false;
  FILE *input_file;
  FILE *output_file;
  int *v_rows;
  int *v_columns;
  int *squareCounter;
  PIECE* pieceStack = NULL;
  PIECE* currentPiece = NULL;
  int* PR;
  int* PC;
  char* sPointer;
  char* solution;

  if (argc != 2) 
  {
    printf("Erro: Argumentos inválidos.\n"); /*O comando de execução do programa deve ser seguido pelo nome do ficheiro a analisar.*/
    return 0;
  }

  input_file = fopen(argv[1], "r"); /*Abertura do ficheiro fornecido de modo a ser possível ler conteúdo.*/
  if (input_file == NULL)
  {
    printf("Erro: Abertura do ficheiro de entrada sem sucesso.\n"); /*Se fopen() retornar NULL houve um problema relacionado com a abertura do ficheiro.*/
    return 0;
  }

/*Operações responsáveis por manipular string de entrada de modo a obter a string de saída respectiva. (Desta maneira o ficheiro de saída terá o nome correto).*/
  sPointer = strchr(argv[1], '.'); /*Procurar '.' no nome do ficheiro e colocar endereço num apontador.*/
  *sPointer = '\0';       /*Colocar nessa posição o caracter '\0' de forma a "cortar" a string.*/
  strcat(argv[1], ".sol2"); /*Adicionar extensão correta.*/

  output_file = fopen(argv[1], "w"); /*Abertura ou criação (caso inexistente) do ficheiro onde será escrita solução.*/
  if (output_file == NULL) 
  {
    printf("Erro: Abertura do ficheiro de saída mal-sucedida.\n"); /*De forma semelhante ao ficheiro de entrada, caso não seja possível abrir ficheiro de saída, deve ser apresentada mensagem de erro.*/
    return 0;
  }

  fscanf(input_file, "%d %d", &rows, &columns); /*Leitura da primeira linha do ficheiro de modo a obter dimensão da matriz em questão.*/
  
  /*Agora que temos dimensões podemos alocar a memória necessária para guardar todas as variáveis.*/
  v_rows = (int*) malloc(rows * sizeof(int)); /*Vector que guarda dados referentes às linhas.*/
  v_columns = (int*) malloc(columns * sizeof(int)); /*Igual ao anterior mas para as colunas.*/
  PR = (int *) malloc(rows * sizeof(int)); /*Vector que guarda possibilidades de colocar quadrados em cada linha*/
  PC = (int *) malloc(columns * sizeof(int)); /*Igual ao anterior mas para as colunas.*/

  read_file(input_file, v_rows, v_columns, rows, columns);		/*Leitura dos valores das linhas e colunas*/
  possibility_calculator(PR, PC, rows, columns, v_rows, v_columns);		/*Calcular as possibilidades de colocação das peças*/

	/*Setup do square counter*/
	squareCounter = (int*) malloc(columns * sizeof(int));		
    for (i = 0; i < columns; i++)
    {
     	squareCounter[i] = v_columns[i];		/*Copiar vetor colunas para o square counter*/
    }

/*Ciclo principal*/
  while (0 <= tier && tier < rows)
  {
    if(fromStack == false) currentPiece = pieceCreator(tier, v_rows);		/*Se ñ tivermos a usar peça do stack, criamos nova peça*/
	  
    if (currentPiece->size == 0 && fromStack == false)		/*Se a peça tem tamanho 0, podemos logo adicionar ao stack visto que a sua posição ñ interessa*/
    {
      add2Stack(&currentPiece, &pieceStack);	/*Adicionar ao stack*/
      currentPiece = NULL;
      tier++;		/*Passar à próxima linha*/
      continue;
    }
    else if (currentPiece->size == 0 && fromStack == true)	/*Se viemos do stack e a peça tem tamanho 0, temos de voltar atrás mais uma vez visto que o erro não poderá ter sido nesta peça*/
    {
      free(currentPiece);    /*Voltar atrás implica libertar memória da peça atual*/
      if(pieceStack != NULL)    /*Se houver peça anterior no stack...*/
      {
      currentPiece = pieceStack;    /*Vamos buscar nova peça ao stack (pop)...*/
      pieceStack = pieceStack->previous_piece;    /*...e atualizamos o stack*/
      }
      tier--;    /*Ao ir buscar peça anterior, passámos para linha anterior*/
      continue;    /*Recomeçamos o loop depois das alterações*/
    }
	  
    /*Loop responsável por testar várias possibilidades de colocar a peça*/
    while(currentPiece->cC < PR[tier])   /*Testamos posições de forma sequencial até termos esgotado as possibilidades*/ 
    {
      validity = evaluatePosition(currentPiece, squareCounter);    /*Guardamos numa variável o resultado da avaliação da possibilidade escolhida*/
      if (validity == SUCCESS)    /*Se for válido...*/
      {
        updateState(SUB, squareCounter, currentPiece);    /*atualizamos número de quadrados que ainda podem ser pintados nas respetivas colunas*/
        add2Stack(&currentPiece, &pieceStack);    /*Adicionamos peça ao stack*/
        currentPiece = NULL;    
        fromStack = false;    /*Não fomos buscar nada ao stack*/
        tier++;    /*Podemos passar à próxima linha*/
        break;    /*Saimos do loop*/
      }
      else currentPiece->cC++;    /*Se ñ for válida passamos à próxima*/
    }


    if (validity == ERROR)    /*Se após percorrer todas as possibilidades ñ conseguirmos encontrar uma válida, teremos que voltar atrás */
    {
      free(currentPiece);    /*Libertamos peça atual*/
      if(pieceStack != NULL)    /*Se existe alguma peça no stack vamos reavaliar essa...*/
      {
      currentPiece = pieceStack;    /*Pegar na peça*/
      pieceStack = pieceStack->previous_piece;    /*Atualizar o stack*/
      }
      updateState(ADD, squareCounter, currentPiece);    /*Ao reavaliar peça anterior temos de desfazer alterações provocadas pela mesma*/
      fromStack = true;    /*Peça veio do stack*/
      currentPiece->cC++;    /*Incrementamos posição para avaliar hipóteses novas*/
      tier--;    /*Ao voltar atrás passámos para a linha anterior*/
    }
  }

  /*Após conclusão do loop vamos ver em qual das situações nos encontramos...*/
  if(tier >= rows)    /*Se percorremos todas as linhas de forma válida, quer dizer que encontrámos uma solução*/ 
  {
    solution = (char*) malloc(rows * columns * sizeof(char));    /*Solução vai ser uma matriz de tamanho linhasXcolunas*/
    solutionWriter(pieceStack, solution, output_file, rows, columns);    /*Vamos escreve-la no ficheiro de saída*/
  }
  else fprintf(output_file, "0");    /*Caso contrário, ñ existe solução e apresentamos o valor 0*/



  fclose(input_file);  /*Fechar ficheiro de entrada após execução do programa.*/
  fclose(output_file); /*Fechar ficheiro após todas as operações de escrita terem terminado.*/

  /*Libertação da memória dinâmica antes de terminar o programa.*/
  stackRelease(pieceStack);
  free(solution);
  free(PC);
  free(PR);
  free(squareCounter);
  free(v_rows);
  free(v_columns);

  return 0;
}





/*Função responsável por ler os dados a partir do ficheiro de entrada recebido através da linha de comando.*/
void read_file (FILE *input_file, int v_rows[], int v_columns[], int rows, int columns) 
{
  int i;
  for (i = 0; i < rows; i++) fscanf(input_file, "%d", &v_rows[i]); /*Leitura de dados referentes a linhas e columnsocação da informação lida na posição correta do vector correspondente.*/
  for (i = 0; i < columns; i++) fscanf(input_file, "%d ", &v_columns[i]); /*Igual ao anterior mas para as columnsunas.*/
}

/*Função responsável por apresentar no ficheiro de saída a solução*/
void solutionWriter(PIECE* pieceStack, char* solutionMatrix, FILE* output_file, int rows, int columns)
{
  int i;
  for (i = 0; i < rows*columns; i++)    /*Inicializar matriz a branco*/
  { 
    solutionMatrix[i] = '-'; 
  }
  
  while (pieceStack != NULL)    /*Enquanto existem peças no stack...*/
  {
    for(i = 0; i < pieceStack->size; i++)    /*...para cada peça "pintar" quadrados correspondentes*/
    {
      solutionMatrix[pieceStack->cR * columns + ((pieceStack->cC + i))] = '#';
    }
    pieceStack = pieceStack->previous_piece;
  }

  for (i = 0; i < rows*columns; i++)    /*Apresentar no ficheiro de saída a matriz já preenchida*/
  {
    if(i%columns == 0 && i!=0) fprintf(output_file, "\n");    /*Caso tenhamos chegado ao fim de uma linha, fazemos new line no ficheiro. Se resto da divisão inteira da posição atual (i) por nº de colunas for 0, sabemos que chegamos ao fim de uma linha (excepto na posição 0)*/    
    fprintf(output_file, "%c",solutionMatrix[i]);    /*Imprimir quadrado selecionado*/
  }
}

/*Função que avalia se a posição selecionada é válida*/
int evaluatePosition (PIECE* currentPiece, int* squareCounter)
{
  int i;
  for(i = 0; i < currentPiece->size; i++)    /*Para o tamanho associado à peça verificamos se é possível colocar quadrado pintado nessa posição*/
  {
    if(squareCounter[currentPiece->cC + i] <= 0) return ERROR;  /*Se for inválido retornamos erro (já não é possível colocar quadrado pintado na posição selecionada)*/
  }
  return SUCCESS;    /*Caso contrário retornamos sucesso*/
}

/*Função que atualiza o número de quadrados que ainda é possível pintar para cada coluna*/
void updateState (int mode, int* squareCounter, PIECE* currentPiece)
{
  int i;
  switch(mode)    /*Funcionalidade depende do modo escolhido*/
  {
	case SUB:      /*Este modo é usado quando colocamos uma peça no stack e precisamos de reduzir nº que podem ser pintadas (ao colocar peça, pintámos quadrados)*/
    for(i = 0; i < currentPiece->size; i++)    /*Subtraímos cada quadrado da peça tendo em conta o seu tamanho e coordenadas*/
    {
    	squareCounter[currentPiece->cC + i]--;
    }
		break;

	case ADD:    /*Modo inverso ao anterior, é usado quando precisamos de reavliar uma peça de modo a reverter efeitos da colocação da peça no stack*/
		for(i = 0; i < currentPiece->size; i++)
    {
    	squareCounter[currentPiece->cC + i]++;
    }  
	  break;
  }
}

/*Função que adiciona ao stack nova peça*/
void add2Stack (PIECE** currentPiece, PIECE** pieceStack)
{
  (*currentPiece)->previous_piece = *pieceStack;    /*Colocamos nova peça no topo de modo a que aponte para a peça do topo anterior*/
  *pieceStack = *currentPiece;    /*Novo topo é a peça colocada*/
}

/*Função que é responsável por libertar stack no final da execução do programa*/
void stackRelease (PIECE* pieceStack) 
{
  PIECE* auxPointer;
  while (pieceStack != NULL)    /*Enquanto temos peças no stack...*/
  {    
    auxPointer = pieceStack;    
    pieceStack = pieceStack->previous_piece;    /*...stack passa a ter como topo peça anterior*/
    free(auxPointer);    /*Libertamos antigo topo do stack*/
  }
}

/*Função que cria peça nova*/
PIECE* pieceCreator (int tier, int* evaluated_vector) 
{
  PIECE* newPiece = (PIECE*) malloc(sizeof(PIECE));    /*Nova peça é alocada */
  newPiece->cR = tier;   /*Coordenada das linhas corresponde ao valor da linha que estamos a avaliar */
  newPiece->cC = 0;    /*Inicializamos coordenadas das colunas a 0*/
  newPiece->size = evaluated_vector[tier];    /*Tamanho da peça corresponde ao nº de quadrados pintáveis naquela linha*/

  return newPiece;
}


/*Função responsável por calcular possibilidades para cada linha e colunas.*/
void possibility_calculator(int PR[], int PC[], int rows, int columns, int v_rows[], int v_columns[])
{
  int i;

  /*Loop responsável por escrever no ficheiro de saída as possibilidades para cada colunas.*/
  for (i = columns - 1; i >= 0; i--) 
  {
    if (v_columns[i] == 0) PC[i] = 0; /*Caso o nº de quadrados seja 0, não faz sentido calcular possibilidades, sendo guardado o valor 0.*/
    else PC[i] = rows - v_columns[i] + 1; /*Cálculo das possibilidades com base na fórmula desenvolvida.*/
  }
  /*Loop exatamente igual ao anterior mas adaptado às linhas.*/
  for (i = rows - 1; i >= 0; i--) 
  {
    if (v_rows[i] == 0) PR[i] = 0;
    else PR[i] = columns - v_rows[i] + 1;
  }
}
