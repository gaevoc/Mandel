#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main() {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create a window
    window = SDL_CreateWindow("Color Bitmap Rendering", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create a surface and a color bitmap in memory
    SDL_Surface *surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                                                0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) {
        SDL_Log("Failed to create surface: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Fill the surface with a color (here: red)
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));

    // Draw points on the surface
    SDL_LockSurface(surface); // Lock surface to manipulate pixels directly

    // Drawing points (randomly placed here)
    for (int i = 0; i < 1000; ++i) {
        int x = rand() % SCREEN_WIDTH;
        int y = rand() % SCREEN_HEIGHT;
        Uint32 *pixel = (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch + x * sizeof(Uint32));
        *pixel = SDL_MapRGB(surface->format, 0, 255, 0); // Green color for points
    }

    SDL_UnlockSurface(surface); // Unlock surface after manipulation

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
    SDL_RenderClear(renderer);

    // Render the texture
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Update the screen
    SDL_RenderPresent(renderer);

    // Wait for a few seconds before quitting (you can use SDL_Delay here)
    SDL_Delay(3000);

    // Clean up resources
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
