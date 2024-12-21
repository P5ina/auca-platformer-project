#ifndef GRAPHICS_OLD_H
#define GRAPHICS_OLD_H

#include <globals.h>
#include <utilities.h>
#include <level/tiles/air.h>

// inline void draw_pause_menu() {
//     draw_text(game_paused);
// }
//
// inline void create_victory_menu_background() {
//     for (auto &ball : victory_balls) {
//         ball.x  = rand_up_to(screen_size.x);
//         ball.y  = rand_up_to(screen_size.y);
//         ball.dx = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
//         ball.dx *= screen_scale;
//         if (abs(ball.dx) < 0E-1) ball.dx = 1.0f;
//         ball.dy = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
//         ball.dy *= screen_scale;
//         if (abs(ball.dy) < 0E-1) ball.dy = 1.0f;
//         ball.radius = rand_from_to(VICTORY_BALL_MIN_RADIUS, VICTORY_BALL_MAX_RADIUS);
//         ball.radius *= screen_scale;
//     }
//
//     /* Clear both the front buffer and the back buffer to avoid ghosting of the game graphics. */
//     ClearBackground(BLACK);
//     EndDrawing();
//     BeginDrawing();
//     ClearBackground(BLACK);
//     EndDrawing();
//     BeginDrawing();
// }
//
// inline void animate_victory_menu_background() {
//     for (auto &ball : victory_balls) {
//         ball.x += ball.dx;
//         if (ball.x - ball.radius < 0 ||
//             ball.x + ball.radius >= screen_size.x) {
//             ball.dx = -ball.dx;
//         }
//         ball.y += ball.dy;
//         if (ball.y - ball.radius < 0 ||
//             ball.y + ball.radius >= screen_size.y) {
//             ball.dy = -ball.dy;
//         }
//     }
// }
//
// inline void draw_victory_menu_background() {
//     for (auto &ball : victory_balls) {
//         DrawCircleV({ ball.x, ball.y }, ball.radius, VICTORY_BALL_COLOR);
//     }
// }
//
// inline void draw_victory_menu() {
//     DrawRectangle(
//         0, 0,
//         static_cast<int>(screen_size.x), static_cast<int>(screen_size.y),
//         { 0, 0, 0, VICTORY_BALL_TRAIL_TRANSPARENCY }
//     );
//
//     animate_victory_menu_background();
//     draw_victory_menu_background();
//
//     draw_text(victory_title);
//     draw_text(victory_subtitle);
// }

#endif // GRAPHICS_OLD_H
