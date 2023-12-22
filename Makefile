CC=gcc
CFLAGS=-O
DEPS = 
OBJ = mandel.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mandel: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

gnuplot: mandel
	./mandel -r 1920 -i 1080 > ./mandel.out
	gnuplot -p -e "set view map; set dgrid3d; plot './mandel.out' using 1:2:5 with image; pause mouse close"