CC=gcc
CFLAGS=-O
DEPS = 
OBJ = mandel.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mandel: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
