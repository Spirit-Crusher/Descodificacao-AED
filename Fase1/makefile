#######################################################################
#                  Projecto AED - Padrão secreto p.1                  #
#                                                                     #
# Nome: makefile                                                      #
#                                                                     #
# Descrição: Ficheiro que contém instruções para que seja efetuada		#
# correta compilação e linkagem de todos os ficheiros relacionados		#
# com o programa.																											#
#                                                                     #
# Autores:  Rodrigo Caldeira nº106963                                 #
#           Guilherme Garcia nº105917                                 #
#           Mariana Domingos nº105988                                 #
#######################################################################

CFLAGS = -Wall -ansi -pedantic
CC = gcc
EXEC = proj

all: $(EXEC)

$(EXEC):	proj.c proj.h
					$(CC) $(CFLAGS) -o $(EXEC) proj.c proj.h

clean:
					rm -f $(EXEC)
