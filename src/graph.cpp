#include <SDL.h>
#include "DBSCAN/DBSCAN.hpp"
#include <cmath>
#include <iostream>

// window size
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// boundaries
const double X_MIN = -2 * M_PI;
const double X_MAX = 2 * M_PI;
const double Y_MIN = -1.5;
const double Y_MAX = 1.5;

double scaleX = WINDOW_WIDTH / (X_MAX - X_MIN);
double scaleY = WINDOW_HEIGHT / (Y_MAX - Y_MIN);

int graphToScreenX(double x) {
    return static_cast<int>((x - X_MIN) * scaleX);
}

int graphToScreenY(double y) {
    return static_cast<int>(WINDOW_HEIGHT - (y - Y_MIN) * scaleY);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Ошибка инициализации SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "График aboba",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Ошибка создания окна: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Ошибка создания рендерера: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, graphToScreenX(X_MIN), graphToScreenY(0), graphToScreenX(X_MAX), graphToScreenY(0)); // x
        SDL_RenderDrawLine(renderer, graphToScreenX(0), graphToScreenY(Y_MIN), graphToScreenX(0), graphToScreenY(Y_MAX)); // y

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        double step = 0.01;
        for (double x = X_MIN; x < X_MAX; x += step) {
            double y1 = sin(x);
            double y2 = sin(x + step);

            SDL_RenderDrawLine(
                renderer,
                graphToScreenX(x),
                graphToScreenY(y1),
                graphToScreenX(x + step),
                graphToScreenY(y2)
            );
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
