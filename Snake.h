#pragma once
#ifndef SNAKE_H
#define SNAKE_H

#include <SDL.h>
#include <vector>

class Snake {
public:
    Snake(int gridWidth, int gridHeight);
    void move(SDL_Keycode direction);
    void grow();
    bool checkCollision() const;
    void render(SDL_Renderer* renderer) const;
    void setDirection(SDL_Keycode direction);
    SDL_Keycode getDirection() const;
    SDL_Point getHead() const;

private:
    std::vector<SDL_Point> m_body;
    SDL_Point m_head;
    int m_speed;
    int m_gridWidth;
    int m_gridHeight;
    SDL_Keycode m_direction;
};

#endif