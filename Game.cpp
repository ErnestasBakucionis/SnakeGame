#include "Game.h"
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 10;
bool isStartHovered = false;
bool isExitHovered = false;


Game::Game() : m_window(nullptr), m_renderer(nullptr), m_isRunning(false), m_snake(WINDOW_WIDTH / CELL_SIZE, WINDOW_HEIGHT / CELL_SIZE), m_food{}, m_inMainMenu(true), font(nullptr) {}

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

    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return;
    }

    font = TTF_OpenFont("Assets/Arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        cleanup();
        return;
    }

    m_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
            if (m_inMainMenu && event.key.keysym.sym == SDLK_RETURN) {
                startGame();
            }
            else if (!m_inMainMenu && event.key.keysym.sym == SDLK_ESCAPE) {
                m_isRunning = false;
            }
            else {
                m_snake.setDirection(event.key.keysym.sym);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (m_inMainMenu) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= WINDOW_WIDTH / 2 - 100 && mouseX <= WINDOW_WIDTH / 2 + 100 &&
                    mouseY >= WINDOW_HEIGHT / 2 - 50 && mouseY <= WINDOW_HEIGHT / 2) {
                    startGame();
                }
                else if (mouseX >= WINDOW_WIDTH / 2 - 100 && mouseX <= WINDOW_WIDTH / 2 + 100 &&
                    mouseY >= WINDOW_HEIGHT / 2 && mouseY <= WINDOW_HEIGHT / 2 + 50) {
                    m_isRunning = false;
                }
            }
            break;
        case SDL_MOUSEMOTION:
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= WINDOW_WIDTH / 2 - 100 && mouseX <= WINDOW_WIDTH / 2 + 100 &&
                mouseY >= WINDOW_HEIGHT / 2 - 50 && mouseY <= WINDOW_HEIGHT / 2) {
                isStartHovered = true;
            }
            else {
                isStartHovered = false;
            }
            if (mouseX >= WINDOW_WIDTH / 2 - 100 && mouseX <= WINDOW_WIDTH / 2 + 100 &&
                mouseY >= WINDOW_HEIGHT / 2 && mouseY <= WINDOW_HEIGHT / 2 + 50) {
                isExitHovered = true;
            }
            else {
                isExitHovered = false;
            }
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

    SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 100);
    for (int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE) {
        SDL_RenderDrawLine(m_renderer, x, 0, x, WINDOW_HEIGHT);
    }
    for (int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE) {
        SDL_RenderDrawLine(m_renderer, 0, y, WINDOW_WIDTH, y);
    }

    m_snake.render(m_renderer);

    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
    SDL_Rect foodRect{ m_food.x * CELL_SIZE, m_food.y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
    SDL_RenderFillRect(m_renderer, &foodRect);

    SDL_RenderPresent(m_renderer);
}

void Game::cleanup() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

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
    Uint32 frameStart;
    int frameTime;

    initialize();

    while (m_isRunning) {
        frameStart = SDL_GetTicks();

        processInput();
        if (m_inMainMenu) {
            showMainMenu();
        }
        else {
            update();
            render();
        }

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    cleanup();
}

void Game::spawnFood() {
    int x = rand() % (WINDOW_WIDTH / CELL_SIZE);
    int y = rand() % (WINDOW_HEIGHT / CELL_SIZE);
    m_food.x = x;
    m_food.y = y;
}

void Game::showMainMenu() {
    m_inMainMenu = true;

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    SDL_Color startColor = isStartHovered ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };
    SDL_Surface* startSurface = TTF_RenderText_Solid(font, "Start Game", startColor);
    SDL_Texture* startTexture = SDL_CreateTextureFromSurface(m_renderer, startSurface);
    SDL_Rect startRect = { WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 25, startSurface->w, startSurface->h };
    SDL_RenderCopy(m_renderer, startTexture, NULL, &startRect);
    SDL_DestroyTexture(startTexture);
    SDL_FreeSurface(startSurface);

    SDL_Color exitColor = isExitHovered ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };
    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "Exit", exitColor);
    SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(m_renderer, exitSurface);
    SDL_Rect exitRect = { WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 25, exitSurface->w, exitSurface->h };
    SDL_RenderCopy(m_renderer, exitTexture, NULL, &exitRect);
    SDL_DestroyTexture(exitTexture);
    SDL_FreeSurface(exitSurface);

    SDL_RenderPresent(m_renderer);
}

void Game::startGame() {
    m_inMainMenu = false;
}