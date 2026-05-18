#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <random>

const int width = 800;
const int height = 600;
const int cellcount = 100;


struct color {
    int r, g, b, a;
};

struct color8 {
    Uint8 r, g, b, a;
};

struct pos {
    int x, y;
};

struct cell {
    pos position;
    color8 col;
};


int random_num(int min, int max) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);

    return dist(rng);
}


void draw_rect(SDL_Renderer* renderer, int x, int y, int w, int h, color rgba) {
    SDL_FRect rect = {
                    static_cast<float>(x),
                    static_cast<float>(y),
                    static_cast<float>(w),
                    static_cast<float>(h),
    };

    SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
    SDL_RenderFillRect(renderer, &rect);
}


void draw_pixel(SDL_Renderer* renderer, int x, int y, int size, color rgba) {
    draw_rect(renderer, x, y, size, size, rgba);
}


pos random_point(int w, int h) {
    return pos({.x=random_num(0, w), .y=random_num(0, h)});
}


color8 get_closest_color(cell cells[cellcount], int x, int y) {
    color8 mincolor;
    float mindist = 999999.0f;

    for (int i = 0; i < cellcount; i++) {
        float cx = cells[i].position.x;
        float cy = cells[i].position.y;

        float dx = x - cx;
        float dy = y - cy;

        float dist = dx * dx + dy * dy;

        if (dist < mindist) {
            mindist = dist;
            mincolor = cells[i].col;
        }
    }
    return mincolor;
}


SDL_Texture* create_voronoi(SDL_Renderer* renderer, cell cells[cellcount]) {
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height);
    void* pixels;
    int pitch;
    SDL_LockTexture(texture, nullptr, &pixels, &pitch);
    Uint32* pixelData = static_cast<Uint32*>(pixels);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            color8 col = get_closest_color(cells, x, y);
            pixelData[y * (pitch / 4) + x] = (col.r << 24) | (col.g << 16) | (col.b << 8) | 0xFF;
        }
    }
    SDL_UnlockTexture(texture);
    return texture;
}


int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }


    SDL_Window* window = SDL_CreateWindow("Empire sim", width, height, 0);

    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    cell cells[cellcount];

    for (int i = 0; i < cellcount; i++) {
        cells[i].position = random_point(width, height);
        cells[i].col = color8({
                    .r=(Uint8)random_num(0, 255),
                    .g=(Uint8)random_num(0, 255),
                    .b=(Uint8)random_num(0, 255),
                    .a=255
                });
    }

    // Draw Noise to texture
    SDL_Texture* texture = create_voronoi(renderer, cells);
    SDL_UnlockTexture(texture);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Clear
        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, texture, nullptr, nullptr);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
