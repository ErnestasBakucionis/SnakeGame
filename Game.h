#pragma once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "Snake.h"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_isRunning;
    Snake m_snake;
    SDL_Point m_food;

    void initialize();
    void processInput();
    void update();
    void render();
    void cleanup();
    void spawnFood();
};

#endif
