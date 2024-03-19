#include <iostream>
#include <raylib.h>

Color Green = Color{28, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
    float x, y;
    int radius, speed_x, speed_y;

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (x + radius >= GetScreenWidth()) {
            cpu_score++;
            ResetBall();
        }
        if (x - radius <= 0) {
            player_score++;
            ResetBall();
        }
        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
        if (y <= 0) {
            y = 0;
        }
    }

public:
    float x, y, width, height;
    int speed_y;

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update() {
        if(IsKeyDown(KEY_UP)) {
            y -= speed_y;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += speed_y;
        }
        LimitMovement();
    }
    
};

class CpuPaddle : public Paddle {
public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) {
            y -= speed_y;
        }
        if (y + height / 2 <= ball_y) {
            y += speed_y;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{
    std::cout << "Starting the game..." << std::endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    const int paddle_width = 25;
    const int paddle_height = 120;

    InitWindow(screen_width, screen_height, "Pong Game");
    SetTargetFPS(60);

    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.radius = 20;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.x = screen_width - paddle_width - 10;
    player.y = screen_height / 2 - paddle_height / 2;
    player.width = paddle_width;
    player.height = paddle_height;
    player.speed_y = 6;

    cpu.x = 10;
    cpu.y = screen_height / 2 - paddle_height / 2;
    cpu.width = paddle_width;
    cpu.height = paddle_height;
    cpu.speed_y = 6;

    while(WindowShouldClose() == false) {
        BeginDrawing();

        // Update
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Check for collisions
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
        }

        // Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine (screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%d", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%d", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}