#include "Game.h"
#include <iostream>

Game::Game() : m_window(nullptr), m_renderer(nullptr), m_isRunning(false), m_snake(800 / 10, 600 / 10), m_food{} {}

Game::~Game() {
    cleanup();
}

bool compareSDLPoint(const SDL_Point& a, const SDL_Point& b) {
    return a.x == b.x && a.y == b.y;
}

void Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

    m_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!m_window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        cleanup();
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        cleanup();
        return;
    }

    m_isRunning = true;
}

void Game::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            m_isRunning = false;
            break;
        case SDL_KEYDOWN:
            m_snake.setDirection(event.key.keysym.sym);
            if (event.key.keysym.sym == SDLK_ESCAPE)
                m_isRunning = false;
            break;
        default:
            break;
        }
    }
}

void Game::update() {
    if (m_snake.checkCollision()) {
        std::cout << "Game Over!" << std::endl;
        m_isRunning = false;
        return;
    }

    if (compareSDLPoint(m_snake.getHead(), m_food)) {
        m_snake.grow();
        spawnFood();
    }

    m_snake.move(m_snake.getDirection());
}

void Game::render() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    m_snake.render(m_renderer);

    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
    SDL_Rect foodRect{ m_food.x * 10, m_food.y * 10, 10, 10 };
    SDL_RenderFillRect(m_renderer, &foodRect);

    SDL_RenderPresent(m_renderer);
}

void Game::cleanup() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
}

void Game::run() {
    const int TARGET_FPS = 5;
    const int FRAME_DELAY = 1000 / TARGET_FPS;
    initialize();

    Uint32 frameStart;
    int frameTime;

    while (m_isRunning) {
        frameStart = SDL_GetTicks();

        processInput();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    cleanup();
}

void Game::spawnFood() {
    int x = rand() % (800 / 10);
    int y = rand() % (600 / 10);
    m_food.x = x;
    m_food.y = y;
}