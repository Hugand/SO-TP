CC=gcc

all: client jogos arbitro

jogos: utils.o
	@${CC} jogos_files/g_1.c -o g_1.o
	@${CC} jogos_files/g_2.c utils.o -o g_2.o
	@echo Ficheiros dos jogos Compilados!

client:
	@${CC} client_files/client.c -o client.o
	@echo Ficheiros dos Clientes Compilados!

arbitro:
	@${CC} arbitro_files/arbitro.c arbitro_files/client_handlers.c -o arbitro.o
	@echo Ficheiros do arbitro Compilados!

utils.o: utils.c
	@${CC} utils.c -c

clean:
	@rm *.o
	@rm pipe_*
	@echo Ficheiros limpos! 
