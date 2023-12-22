CC=gcc
CFLAGS=-O
DEPS = 
OBJ = mandel.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm *.o mandel mandel.out

mandel: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

gnuplot: mandel
	./mandel -r 1800 -i 1200 -x 500 > ./mandel.out
	gnuplot -p -e "set terminal wxt 0;\
	set view map;\
	set palette defined ( 0 'white', 0.07 'red', 0.3 'blue', 1 'black' );\
	plot [-2:1][-1:1]'./mandel.out' using 1:2:5 with image;\
	pause mouse close"