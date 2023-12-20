#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
// #include <math.h>
#include <getopt.h>
#include <wchar.h>
#include <locale.h>

extern char *optarg;
extern int optind, opterr, optopt;

float r_min = -2.0;
float r_max = 1.0;
float i_min = -1.0;
float i_max = 1.0;
int max_iter = 300;
int r_div = 100;
int i_div = 35;

float r_delta, i_delta;
short int is_a_tty = 1;



void init(int argc, char *argv[]) {
    int opt;

    is_a_tty = isatty(1);

    while ((opt = getopt(argc, argv, "x:r:i:")) != -1) {
        switch (opt) {
            
            case 'x':
                // printf("Option -x (max_iter): %s\n", optarg);
                max_iter = atoi(optarg);
                break;
            case 'r':
                // printf("Option -r (r_div): %s\n", optarg);
                r_div = atoi(optarg);
                break;
            case 'i':
                // printf("Option -i (i_div): %s\n", optarg);
                i_div = atoi(optarg);
                break;
            case '?':
                // printf("Opzione sconosciuta: %c\n", optopt);
                break;
        }
    }
    
    r_delta = (r_max - r_min)/(r_div - 1);
    i_delta = (i_max - i_min) / (i_div - 1);
}

int mandel(double cr, double ci) {
  double zr, zi, mod_z2, zrn, zin;
  int n;
  
  n = 0;
    zr = 0.0; zi = 0.0;
            
  while (n <= max_iter) {
        zrn = zr * zr - zi * zi + cr;
        zin = 2 * zr * zi + ci;

    if (zrn * zrn + zin * zin > 4.0) return ++n;
     
        zr = zrn;
        zi = zin;
        n++;
    }
    return n;
}

void plotme(double cr, double ci, int r, int i, int n)
{
    if (n >= max_iter)
    {
        // printf("*");
        printf(" ");
    }
    else if (n >= 250)
    {
        wchar_t box = 0x2591;
        printf("%lc", box);
    }
    else if (n >= 200)
    {
        wchar_t box = 0x2592;
        printf("%lc", box);
    }
    else if (n >= 100)
    {
        wchar_t box = 0x2593;
        printf("%lc", box);
    }
    else if (n >= 50)
    {
        wchar_t box = 0x2588;
        printf("%lc", box);
    }
    else
    {
        wchar_t box = '*';
        printf("%lc", box);
    }

    if (r == r_div) printf("\n");
} // ' ', '\u2591', '\u2592', '\u2593', '\u2588', '\u2593', '\u2592', '\u2591'

void listme (double cr, double ci, int r, int i, int n) {
    printf("%1.14e %1.14e %d %d %d\n", cr, ci, r, i, n);
}

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "en_US.UTF-8");

    init(argc, argv);
    if (is_a_tty == 1) {
        printf ("%f %f %f %f %f %f\n", r_min, r_max, i_min, i_max, r_delta, i_delta);
        printf ("r_div %d i_div %d max_iter %d \n", r_div, i_div, max_iter);
    }
    
    int n, i, r;
    double cr, ci;

    for (i=0; i<=i_div; i++) {
        for (r=0; r<=r_div; r++) {
            cr = r_min + (r * r_delta);
            ci = i_min + (i * i_delta);

            n = mandel(cr, ci);

            if (is_a_tty == 1) {
                plotme(cr, ci, r, i, n);
            } else {
                listme (cr, ci, r, i, n);
            }
            
        }
    }
    return 0;
}