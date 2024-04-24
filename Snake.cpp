#include "Snake.h"
#include <iostream>

Snake::Snake(int gridWidth, int gridHeight) : m_speed(1), m_gridWidth(gridWidth), m_gridHeight(gridHeight), m_direction(SDLK_UP) {
    m_head.x = gridWidth / 2;
    m_head.y = gridHeight / 2;
    m_body.push_back(m_head);
}

void Snake::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    for (const auto& segment : m_body) {
        SDL_Rect block{ segment.x * 10, segment.y * 10, 10, 10 };
        SDL_RenderFillRect(renderer, &block);
    }
}

void Snake::move(SDL_Keycode direction) {
    switch (direction) {
    case SDLK_UP:
        m_head.y -= 1;
        break;
    case SDLK_DOWN:
        m_head.y += 1;
        break;
    case SDLK_LEFT:
        m_head.x -= 1;
        break;
    case SDLK_RIGHT:
        m_head.x += 1;
        break;
    default:
        break;
    }

    m_body.insert(m_body.begin(), m_head);
    m_body.pop_back();
}

bool Snake::checkCollision() const {
    if (m_head.x < 0 || m_head.x >= m_gridWidth || m_head.y < 0 || m_head.y >= m_gridHeight) {
        return true;
    }

    for (size_t i = 1; i < m_body.size(); ++i) {
        if (m_head.x == m_body[i].x && m_head.y == m_body[i].y) {
            return true;
        }
    }

    return false;
}

void Snake::grow() {
    m_body.push_back(m_body.back());
}

void Snake::setDirection(SDL_Keycode direction) {
    m_direction = direction;
}

SDL_Keycode Snake::getDirection() const {
    return m_direction;
}

SDL_Point Snake::getHead() const {
    return m_head;
}