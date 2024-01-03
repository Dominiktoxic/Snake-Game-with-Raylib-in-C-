#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

int cells = 25;
int cellSize = 30;

const int screenWidth = cells * cellSize;
const int screenHeight = cells * cellSize;
Color green = {0, 255, 0, 255};
Color red = {255, 0, 0, 255};
double lastUpdateTime = 0;
Color yellow = {30, 190, 50};

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }

    return false;
}

class Fruit {
    public:
        Vector2 position = getRandomPos();

        Vector2 getRandomPos() {
            float x = GetRandomValue(0, 24);
            float y = GetRandomValue(0, 24);
            return Vector2{x, y};
        }

        void Draw() {
            DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, red);
        }
};

class Snake {
    public:
        std::deque<Vector2> body = {Vector2{5, 9}, Vector2{6, 9}, Vector2{7, 9}};
        Vector2 direction = {0, 1};
        bool addSegment = false;

        void Draw() {
            for (unsigned int i = 0; i < body.size(); i++) {
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize, (float)cellSize};
                if (i == 0) {
                    DrawRectangleRounded(segment, 0.5, 6, YELLOW);
                } else {
                    DrawRectangleRounded(segment, 0.5, 6, DARKBLUE);
                }
            }
        }

        void Update() {
            if (addSegment) {
                body.push_front(Vector2Add(body[0], direction));
                addSegment = false;
            } else {
                body.pop_back();
                body.push_front(Vector2Add(body[0], direction));
            }
        }
};

class Game {
    public:
        Fruit fruit = Fruit();
        Snake snake = Snake();
        int score = 0;
        bool running = true;

        void Draw() {
            fruit.Draw();
            snake.Draw();
        }

        void Update() {
            if (running) {
                snake.Update();
                collisionsCheckFruit();
                collisionsCheckTail();
            }
        }

        void collisionsCheckFruit() {
            if (Vector2Equals(snake.body[0], fruit.position)) {
                fruit.position = fruit.getRandomPos();
                snake.addSegment = true;
                score++;
            }
        }

        void GameOver() {
            running = false;
            score = 0;
            fruit.position = fruit.getRandomPos();
            Reset();
        }

        void collisionsCheckTail() {
            std::deque<Vector2> headlessSnake = snake.body;
            headlessSnake.pop_front();

            for (unsigned int i = 0; i < headlessSnake.size(); i++) {
                if (Vector2Equals(snake.body[0], headlessSnake[i])) {
                    GameOver();
                }
            }
        }

        void Reset() {
            snake.body = {Vector2{5, 9}, Vector2{6, 9}, Vector2{7, 9}};
            running = false;
        }
};

int main () {
    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);

    Game game = Game();

    while (WindowShouldClose() == false){
        BeginDrawing();

        if (eventTriggered(0.2)) {
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.running = true;
            game.snake.direction = {0, 1};
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.running = true;
            game.snake.direction = {-1, 0};
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.running = true;
            game.snake.direction = {1, 0};
        }

        ClearBackground(green);

        DrawText(TextFormat("%i", game.score), screenWidth / 2 - 30, screenHeight / 2 - 50, 100, WHITE);

        game.fruit.Draw();
        game.snake.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}