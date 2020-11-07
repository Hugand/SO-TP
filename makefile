CC=gcc

all: client jogos arbitro

jogos: utils.o
	${CC} jogos_files/g_1.c -o g_1.o
	${CC} jogos_files/g_2.c utils.o -o g_2.o

client:
	${CC} client_files/client.c -o client.o

arbitro:
	${CC} arbitro_files/arbitro.c -o arbitro.o

utils.o: utils.c
	${CC} utils.c -c

# TODO: ADD TEMP FILE CLEAN
clean:
	rm *.o

