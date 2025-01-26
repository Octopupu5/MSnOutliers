#include "RegressionModel.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int SQUARE_SIZE = 50;
const float CIRCLE_RADIUS = 200.0f;
const float SPEED = 0.01f;

int main() {
    std::cout << "Добро пожаловать в aboba!" << std::endl;

    RegressionModel model;
    model.loadData("data/dataset.csv");
    model.train();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Ошибка инициализации SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Пример рисования на SDL2",
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

    float angle = 0.0f;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        int centerX = WINDOW_WIDTH / 2;
        int centerY = WINDOW_HEIGHT / 2;

        int squareX = centerX + static_cast<int>(CIRCLE_RADIUS * cos(angle)) - SQUARE_SIZE / 2;
        int squareY = centerY + static_cast<int>(CIRCLE_RADIUS * sin(angle)) - SQUARE_SIZE / 2;

        angle += SPEED;
        if (angle > 2 * M_PI) {
            angle -= 2 * M_PI;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect square = {squareX, squareY, SQUARE_SIZE, SQUARE_SIZE};
        SDL_RenderFillRect(renderer, &square);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}