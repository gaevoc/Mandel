#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Function to initialize SDL and create a window
SDL_Window* initSDL() {
    SDL_Window* window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("SDL Points",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH,
                                  SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
    }

    return window;
}

// Function to draw points
void drawPoints(SDL_Renderer* renderer) {
    // Array of points (x, y)
    int points[][2] = {
        {50, 50},
        {100, 150},
        {200, 100},
        {300, 200},
        {400, 300}
    };

    // Array of colors (R, G, B)
    int colors[][3] = {
        {255, 0, 0},    // Red
        {0, 255, 0},    // Green
        {0, 0, 255},    // Blue
        {255, 255, 0},  // Yellow
        {0, 255, 255}   // Cyan
    };

    int numPoints = sizeof(points) / sizeof(points[0]);

    for (int i = 0; i < numPoints; ++i) {
        SDL_SetRenderDrawColor(renderer, colors[i][0], colors[i][1], colors[i][2], 255);
        SDL_RenderDrawPoint(renderer, points[i][0], points[i][1]);
    }

    SDL_RenderPresent(renderer); // Update screen
}

int main() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    window = initSDL();
    if (window == NULL) {
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Event e;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white
        SDL_RenderClear(renderer); // Clear screen

        drawPoints(renderer);

        SDL_Delay(100); // Delay to see the points (milliseconds)
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
