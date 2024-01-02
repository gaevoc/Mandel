CC=gcc
CFLAGS=-O `sdl2-config --cflags`
OFLAGS=`sdl2-config --libs`
DEPS = 
OBJ = mandel.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm *.o mandel mandel.out

mandel: $(OBJ)
	$(CC) -o $@ $^ $(OFLAGS)

console: mandel
	./mandel -r -2 -R 1 -i -1 -I 1 -x 500

gnuplot: mandel
	./mandel -r -2 -R 1 -i -1 -I 1 -d 1800 -x 500 > ./mandel.out
	gnuplot -p -e "set terminal wxt 0;\
	set view map;\
	set palette defined ( 0 'white', 0.07 'red', 0.3 'blue', 1 'black' );\
	set cbrange[1:500];\
	plot './mandel.out' using 1:2:5 with image;\
	pause mouse close"


	