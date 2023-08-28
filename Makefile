CC= g++
CFLAGS= -Wall
LIBS= -lfreeglut -lopengl32
SRC= src/*.cpp

Pong: $(SRC)
	$(CC) $(CFLAGS) -o Pong $(SRC) $(LIBS)