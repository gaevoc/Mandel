#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <getopt.h>
#include <wchar.h>
#include <locale.h>
#include "mandel.h"

#include <SDL2/SDL.h>


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
double *r_ruler, *i_ruler;
int * n_map, n_map_min, n_map_max;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGBColor;

// Color palette for the Mandelbrot set
/* RGBColor mandelbrotPalette[] = {
    {66, 30, 15}, {25, 7, 26}, {9, 1, 47}, {4, 4, 73}, {0, 7, 100},
    {12, 44, 138}, {24, 82, 177}, {57, 125, 209}, {134, 181, 229},
    {211, 236, 248}, {241, 233, 191}, {248, 201, 95}, {255, 170, 0},
    {204, 128, 0}, {153, 87, 0}, {106, 52, 3}
}; */

RGBColor mandelbrotLightCoolPalette[] = {
    {190, 234, 236}, {166, 215, 226}, {141, 195, 215}, {114, 175, 204}, {83, 155, 193},
    {56, 135, 182}, {38, 114, 171}, {33, 93, 160}, {37, 71, 149}, {45, 48, 137},
    {58, 25, 125}, {75, 4, 112}, {94, 0, 97}, {112, 0, 82}, {129, 0, 67},
    {144, 0, 53}
};

RGBColor mandelbrotCoolPalette[] = {
    {48, 25, 52}, {58, 42, 81}, {64, 63, 112}, {55, 87, 138}, {39, 116, 157},
    {37, 146, 158}, {55, 176, 148}, {96, 202, 134}, {142, 224, 118}, {194, 237, 108},
    {245, 245, 121}, {254, 217, 118}, {253, 173, 100}, {250, 121, 89}, {236, 65, 80},
    {211, 8, 80}
};

RGBColor mandelbrotPalette1[] = {
    {48, 25, 52}, {58, 42, 81}, {64, 63, 112}, {55, 87, 138}, {39, 116, 157},
    {37, 146, 158}, {55, 176, 148}, {96, 202, 134}, {142, 224, 118}, {194, 237, 108},
    {245, 245, 121}, {254, 217, 118}, {253, 173, 100}, {250, 121, 89}, {236, 65, 80},
    {211, 8, 80}
};

RGBColor mandelbrotPalette[] = {
    {66, 30, 15}, {25, 7, 26}, {9, 1, 47}, {4, 4, 73}, {0, 7, 100},
    {12, 44, 138}, {24, 82, 177}, {57, 125, 209}, {134, 181, 229}, {211, 236, 248},
    {241, 233, 191}, {248, 201, 95}, {255, 170, 0}, {204, 128, 0}, {153, 87, 0},
    {106, 52, 3}, {254, 67, 101}, {252, 157, 154}, {249, 205, 173}, {200, 200, 169},
    {131, 175, 155}, {74, 131, 157}, {47, 92, 142}, {33, 42, 116}, {20, 12, 80},
    {255, 119, 119}, {255, 204, 170}, {255, 255, 178}, {170, 255, 195}, {137, 236, 204},
    {137, 221, 255}, {176, 196, 222}, {255, 175, 189}, {238, 173, 255}, {152, 150, 243},
    {139, 160, 170}, {171, 154, 205}, {193, 167, 215}, {191, 181, 217}, {208, 195, 255},
    {222, 204, 255}, {235, 221, 255}, {255, 204, 255}, {255, 214, 229}, {255, 239, 217},
    {255, 248, 225}, {255, 255, 240}, {255, 255, 255}
};



// Function to interpolate between two colors
RGBColor interpolateColors(RGBColor color1, RGBColor color2, double t) {
    RGBColor result;
    result.r = (uint8_t)(color1.r + t * (color2.r - color1.r));
    result.g = (uint8_t)(color1.g + t * (color2.g - color1.g));
    result.b = (uint8_t)(color1.b + t * (color2.b - color1.b));
    return result;
}

// Function to get the color from the gradient based on iteration count
RGBColor getColorFromGradient(int iterationCount, int maxIterations) {
    if (iterationCount == max_iter) {
        return (RGBColor){0, 0, 0}; // Black for points inside the set
    }

    double t = (double)iterationCount / maxIterations;
    int index1 = (int)(t * (sizeof(mandelbrotPalette) / sizeof(RGBColor)));
    int index2 = index1 + 1;

    // Prevent index out of bounds
    if (index2 >= sizeof(mandelbrotPalette) / sizeof(RGBColor)) {
        return mandelbrotPalette[sizeof(mandelbrotPalette) / sizeof(RGBColor) - 1];
    }

    RGBColor color1 = mandelbrotPalette[index1];
    RGBColor color2 = mandelbrotPalette[index2];

    // Interpolate between colors
    double t_interpolate = fmod(t * (sizeof(mandelbrotPalette) / sizeof(RGBColor)), 1.0);
    return interpolateColors(color1, color2, t_interpolate);
}


void mandel_init(int argc, char *argv[]) {
    int opt;

    is_a_tty = isatty(1);

    while ((opt = getopt(argc, argv, "x:d:r:R:i:I:")) != -1) {
        switch (opt) {
            
            case 'x':
                // printf("Option -x (max_iter): %s\n", optarg);
                max_iter = atoi(optarg);
                break;
            case 'd':
                // printf("Option -r (r_div): %s\n", optarg);
                r_div = atoi(optarg);
                break;
            // case 'i':
            //     // printf("Option -i (i_div): %s\n", optarg);
            //     i_div = atoi(optarg);
            //     break;
            case '?':
                // printf("Opzione sconosciuta: %c\n", optopt);
                break;
            case 'r':
                // printf("Option -r (r_div): %s\n", optarg);
                r_min = atof(optarg);
                break;
            case 'R':
                // printf("Option -r (r_div): %s\n", optarg);
                r_max = atof(optarg);
                break;
            case 'i':
                // printf("Option -r (r_div): %s\n", optarg);
                i_min = atof(optarg);
                break;
            case 'I':
                // printf("Option -r (r_div): %s\n", optarg);
                i_max = atof(optarg);
                break;
        }
    }
    
    r_delta = (r_max - r_min)/(r_div - 1);
    // i_delta = (i_max - i_min) / (i_div - 1);
    i_delta = r_delta;
    i_div = (int)((i_max - i_min) / i_delta + 0.5) + 1;

    r_ruler = malloc(sizeof(double) * r_div);
    i_ruler = malloc(sizeof(double) * i_div);

    int n;
    for (n=0; n<=r_div; n++) {
        r_ruler[n] = r_min + (n * r_delta);
        // printf("%2.14e\n", r_ruler[n]);
    }
    for (n=0; n<=i_div; n++) {    
        i_ruler[n] = i_min + (n * i_delta);
    }

    n_map = malloc(sizeof(int) * (r_div * i_div));
    if (n_map == NULL) {
        printf("Error allocating n_map\n");
        exit (1);
    }
}

int mandel_point(double cr, double ci) {
  double zr, zi, mod_z2, zrn, zin;
  int n;
  
  n = 0;
  zr = 0.0; zi = 0.0;
            
  while (n < max_iter) {
        zrn = zr * zr - zi * zi + cr;
        zin = 2 * zr * zi + ci;

    if (zrn * zrn + zin * zin > 4.0) return ++n;
     
        zr = zrn;
        zi = zin;
        n++;
    }
    return n;
}

void plot_point(double cr, double ci, int r, int i, int n)
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

    if (r == r_div - 1) printf("\n");
} // Some examples of box chars \u2591', '\u2592', '\u2593', '\u2588'

void list_point (double cr, double ci, int r, int i, int n) {
    printf("%1.14e %1.14e %d %d %d\n", cr, ci, r, i, n);
}

void plot_set() {
    int i, r;
    for (i=0; i<i_div; i++) {
            for (r=0; r<r_div; r++) {
                    plot_point(r_ruler[r], i_ruler[i], r, i, n_map[i * r_div + r]);
            }
        }
}

void list_set() {
    int i, r;
    for (i=0; i<i_div; i++) {
            for (r=0; r<r_div; r++) {
                    list_point(r_ruler[r], i_ruler[i], r, i, n_map[i * r_div + r]);
            }
        }
}

void mandel_set()
{
    int n, i, r;
    double cr, ci;

    n_map_min = max_iter;
    n_map_max = 0;

    for (i = 0; i < i_div; i++)
    {
        ci = i_ruler[i];
        for (r = 0; r < r_div; r++)
        {
            // cr = r_min + (r * r_delta);
            // ci = i_min + (i * i_delta);
            cr = r_ruler[r];
            n = mandel_point(cr, ci);

            n_map[i * r_div + r] = n;

            if (n > n_map_max && n != max_iter) n_map_max = n;
            if (n < n_map_min) n_map_min = n;
        }
    }
}


// Function to initialize SDL and create a window
SDL_Window* initSDL() {
    SDL_Window* window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("SDL Points",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  r_div,
                                  i_div,
                                  SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
    }

    return window;
}

// Function to draw points - deprecated
void drawPoints(SDL_Renderer* renderer) {
    int i, r, n, c;
    for (i=0; i<i_div; i++) {
            for (r=0; r<r_div; r++) {
                
                n = n_map[i * r_div + r];
                // n = (int)(sqrt(n) + 0.5);
                RGBColor color = getColorFromGradient(n, n_map_max);
                
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                SDL_RenderDrawPoint(renderer, r, i);
                // plot_point(r_ruler[r], i_ruler[i], r, i, n_map[i * r_div + r]);
            }
        }

    SDL_RenderPresent(renderer); // Update screen
}

/// @brief Draws Mandelbrot set points on the surface
/// @param surface 
void drawPointsOnSurface(SDL_Surface * surface) {
    int i, r, n, c;
    // Draw points on the surface
    SDL_LockSurface(surface); // Lock surface to manipulate pixels directly

    for (i=0; i<i_div; i++) {
        for (r=0; r<r_div; r++) {
            n = n_map[i * r_div + r];
            RGBColor color = getColorFromGradient(n, n_map_max);
            Uint32 *pixel = (Uint32 *)((Uint8 *)surface->pixels + i * surface->pitch + r * sizeof(Uint32));
            *pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b); 
        }
    }
    SDL_UnlockSurface(surface);
}


int main(int argc, char *argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture *texture;

    setlocale(LC_ALL, "en_US.UTF-8");

    mandel_init(argc, argv);

    // Header information on stderr
    fprintf (stderr, "%f %f %f %f %f %f\n", r_min, r_max, i_min, i_max, r_delta, i_delta);
    mandel_set();
    fprintf (stderr, "r_div %d i_div %d max_iter %d n_map_max %d n_map_min %d\n", r_div, i_div, max_iter, n_map_max, n_map_min);

    fprintf(stderr, "Initializing SDL\n");

    window = initSDL();
    if (window == NULL) {
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a surface and a color bitmap in memory
    SDL_Surface *surface = SDL_CreateRGBSurface(0, r_div, i_div, 32,
                                                0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) {
        SDL_Log("Failed to create surface: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    fprintf(stderr, "Plotting SDL\n");

    drawPointsOnSurface(surface);

    SDL_Event e;
    int quit = 0;
    int should_redraw = 1;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        if (should_redraw) {
            // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white
            // SDL_RenderClear(renderer); // Clear screen

            // drawPoints(renderer);
            // Create a texture from the surface
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!texture) {
                SDL_Log("Failed to create texture: %s", SDL_GetError());
                SDL_FreeSurface(surface);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 1;
            }

            // Clear the renderer
            // SDL_RenderClear(renderer);

            // Render the texture
            SDL_RenderCopy(renderer, texture, NULL, NULL);

            // Update the screen
            SDL_RenderPresent(renderer);
            should_redraw = 0;
        }
        
        SDL_Delay(100); // Delay to see the points (milliseconds)
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

