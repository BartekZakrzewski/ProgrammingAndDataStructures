#include "global_variables.h"
#include "primlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SQR_SIZE 24
#define PIECE_SIZE 4
#define WIDTH 10
#define HEIGHT 30
#define DELAY 100
#define STEP 1

#define EMPTY 0
#define ERR_SIDE -2
#define ERR_DOWN -1

typedef struct {
  int x;
  int y;
} Point;

typedef struct {
  Point blocks[PIECE_SIZE][PIECE_SIZE];
  Point top_left;
  int is_moving;
  int rotation;
  int kind;
  int dx;
  int color[PIECE_SIZE][PIECE_SIZE];
} Piece;

void initGame();
void handleInput(Piece *piece_to_play, const Point *board_top_left,
                 Piece *next_piece);
int handleGeneratePiece(Piece *piece_to_play, Piece *next_piece);
int verifyMove(Piece *p);

void drawBoard(const Point *board_top_left);
void drawMovingPiece(Piece *piece_to_play, const Point *board_top_left);
void drawNextPiece(Piece *next_piece, const Point *board_top_left);

void movePiece(Piece *p);
void addPieceToBoard(Piece *p);
void skipDown(Piece *p, const Point *board_top_left, Piece *next_piece);

void checkRow();
void checkLose(Piece *p);

Piece generateRandomPiece();
void rotatePiece(Piece *p);
void centerRotation(Point p[PIECE_SIZE][PIECE_SIZE], Point *rotation_point,
                    Point *old_rotation_point);

int getColorForPiece(int c);
void write_score_to_string(char *_score) {
  sprintf(_score, "Score: %d", score);
}

int is_playing = 1;
int game[HEIGHT][WIDTH];

int main(int argc, char *argv[]) {
  if (gfx_init())
    exit(3);
  initGame();
  const Point board_top_left = {gfx_screenWidth() / 2 - (WIDTH * SQR_SIZE) / 2,
                                gfx_screenHeight() / 2 -
                                    (HEIGHT * SQR_SIZE) / 2};

  Piece piece_to_play = generateRandomPiece();
  Piece next_piece = generateRandomPiece();

  while (is_playing) {
    drawBoard(&board_top_left);
    drawMovingPiece(&piece_to_play, &board_top_left);
    drawNextPiece(&next_piece, &board_top_left);

    if (handleGeneratePiece(&piece_to_play, &next_piece))
      continue;

    handleInput(&piece_to_play, &board_top_left, &next_piece);

    movePiece(&piece_to_play);

    gfx_updateScreen();
    SDL_Delay(DELAY);
  }

  while (gfx_pollkey() != SDLK_q) {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

    gfx_textout(gfx_screenWidth() / 2, gfx_screenHeight() / 2 - 40,
                "Game Over!", WHITE);
    char *_score = (char[20]){0};
    write_score_to_string(_score);
    gfx_textout(gfx_screenWidth() / 2, gfx_screenHeight() / 2, _score, WHITE);
    gfx_textout(gfx_screenWidth() / 2, gfx_screenHeight() / 2 + 40,
                "Press [q] to quit", WHITE);

    gfx_updateScreen();
    SDL_Delay(DELAY);
  }

  return 0;
}

void initGame() {
  srand(time(NULL));
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      game[y][x] = EMPTY;
    }
  }
}

void handleInput(Piece *piece_to_play, const Point *board_top_left,
                 Piece *next_piece) {
  int key_pressed = gfx_pollkey();
  if (key_pressed == SDLK_LEFT) {
    piece_to_play->dx = -STEP;
  } else if (key_pressed == SDLK_RIGHT) {
    piece_to_play->dx = STEP;
  } else {
    piece_to_play->dx = 0;
  }
  if (key_pressed == SDLK_DOWN)
    skipDown(piece_to_play, board_top_left, next_piece);
  if (key_pressed == SDLK_SPACE) {
    piece_to_play->rotation = (piece_to_play->rotation + 1) % 4;
    rotatePiece(piece_to_play);
  }
  if (key_pressed == SDLK_RETURN || key_pressed == SDLK_ESCAPE)
    is_playing = 0;
}

int handleGeneratePiece(Piece *piece_to_play, Piece *next_piece) {
  if (!piece_to_play->is_moving) {
    checkRow();
    SDL_Delay(2 * DELAY);
    *piece_to_play = *next_piece;
    *next_piece = generateRandomPiece();
    checkLose(piece_to_play);
    movePiece(piece_to_play);
    return 1;
  }
  return 0;
}

int verifyMove(Piece *p) {
  int move_down = p->is_moving;
  int move_side = 1;
  for (int y = 0; y < PIECE_SIZE; y++) {
    for (int x = 0; x < PIECE_SIZE; x++) {
      if (p->blocks[y][x].x < 0 && p->blocks[y][x].y < 0) {
        continue;
      }
      if (p->blocks[y][x].x < 0 || p->blocks[y][x].y < 0 ||
          p->blocks[y][x].x >= WIDTH || p->blocks[y][x].y >= HEIGHT) {
        return 0;
      }
      if (p->blocks[y][x].x + p->dx < 0 || p->blocks[y][x].x + p->dx >= WIDTH) {
        move_side = 0;
        p->dx = 0;
      } else if (game[p->blocks[y][x].y][p->blocks[y][x].x + p->dx] != EMPTY) {
        move_side = 0;
        p->dx = 0;
      }
      if (p->blocks[y][x].y < 0 ||
          p->blocks[y][x].y + (move_down ? 1 : 0) >= HEIGHT) {
        p->is_moving = 0;
      } else if (game[p->blocks[y][x].y + (move_down ? 1 : 0)]
                     [p->blocks[y][x].x] != EMPTY) {
        p->is_moving = 0;
      }
    }
  }
  return p->is_moving == move_down && move_side;
}

void drawBoard(const Point *board_top_left) {
  gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

  gfx_rect(board_top_left->x, board_top_left->y,
           board_top_left->x + WIDTH * SQR_SIZE,
           board_top_left->y + HEIGHT * SQR_SIZE, WHITE);

  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      gfx_filledRect(
          board_top_left->x + x * SQR_SIZE, board_top_left->y + y * SQR_SIZE,
          board_top_left->x + (x + 1) * SQR_SIZE,
          board_top_left->y + (y + 1) * SQR_SIZE, getColorForPiece(game[y][x]));
      gfx_rect(board_top_left->x + x * SQR_SIZE,
               board_top_left->y + y * SQR_SIZE,
               board_top_left->x + (x + 1) * SQR_SIZE,
               board_top_left->y + (y + 1) * SQR_SIZE, WHITE);
    }
  }
}

void drawMovingPiece(Piece *piece_to_play, const Point *board_top_left) {
  for (int y = 0; y < PIECE_SIZE; y++) {
    for (int x = 0; x < PIECE_SIZE; x++) {
      if (piece_to_play->blocks[y][x].x >= 0 &&
          piece_to_play->blocks[y][x].x < WIDTH &&
          piece_to_play->blocks[y][x].y >= 0 &&
          piece_to_play->blocks[y][x].y < HEIGHT) {
        gfx_filledRect(
            board_top_left->x + piece_to_play->blocks[y][x].x * SQR_SIZE,
            board_top_left->y + piece_to_play->blocks[y][x].y * SQR_SIZE,
            board_top_left->x + (piece_to_play->blocks[y][x].x + 1) * SQR_SIZE,
            board_top_left->y + (piece_to_play->blocks[y][x].y + 1) * SQR_SIZE,
            getColorForPiece(piece_to_play->color[y][x]));
      }
    }
  }
}

void drawNextPiece(Piece *next_piece, const Point *board_top_left) {
  Point left = {2 * SQR_SIZE, board_top_left->y};

  char *_score = (char[20]){0};
  write_score_to_string(_score);
  gfx_textout(left.x, left.y, _score, WHITE);

  gfx_textout(left.x, left.y + SQR_SIZE, "Next Piece:", WHITE);

  gfx_rect(left.x, left.y + 2 * SQR_SIZE, left.x + 8 * SQR_SIZE,
           left.y + 9 * SQR_SIZE, WHITE);

  for (int y = 0; y < PIECE_SIZE; y++) {
    for (int x = 0; x < PIECE_SIZE; x++) {
      if (next_piece->blocks[y][x].x < 0 || next_piece->blocks[y][x].y < 0) {
        continue;
      }
      gfx_filledRect(left.x + (next_piece->blocks[y][x].x - 1) * SQR_SIZE,
                     left.y + (next_piece->blocks[y][x].y + 3) * SQR_SIZE,
                     left.x + (next_piece->blocks[y][x].x) * SQR_SIZE,
                     left.y + (next_piece->blocks[y][x].y + 4) * SQR_SIZE,
                     MAGENTA);
    }
  }
}

void movePiece(Piece *p) {
  verifyMove(p);

  if (!p->is_moving) {
    addPieceToBoard(p);
    return;
  }

  p->is_moving += 1;
  for (int y = 0; y < PIECE_SIZE; y++) {
    for (int x = 0; x < PIECE_SIZE; x++) {
      if (p->blocks[y][x].x < 0 || p->blocks[y][x].y < 0) {
        continue;
      }
      p->blocks[y][x].x += p->dx;
      p->blocks[y][x].y += !(p->is_moving % falling_speed);
    }
  }
  p->top_left.x += p->dx;
  p->top_left.y += !(p->is_moving % falling_speed);
}

void addPieceToBoard(Piece *p) {
  for (int y = 0; y < PIECE_SIZE; y++) {
    for (int x = 0; x < PIECE_SIZE; x++) {
      if (p->blocks[y][x].x >= 0 && p->blocks[y][x].x < WIDTH &&
          p->blocks[y][x].y >= 0 && p->blocks[y][x].y < HEIGHT) {
        game[p->blocks[y][x].y][p->blocks[y][x].x] = MAGENTA;
      }
    }
  }
}

void skipDown(Piece *p, const Point *board_top_left, Piece *next_piece) {
  falling_speed = 1;
  while (p->is_moving) {
    drawBoard(board_top_left);
    drawMovingPiece(p, board_top_left);
    drawNextPiece(next_piece, board_top_left);
    movePiece(p);
    p->dx = 0;
  }
  falling_speed = 4;
}

void checkRow() {
  int rows_cleared = 0;
  for (int y = 0; y < HEIGHT; y++) {
    int is_full = 1;
    for (int x = 0; x < WIDTH; x++) {
      if (game[y][x] == EMPTY) {
        is_full = 0;
        break;
      }
    }
    if (is_full) {
      rows_cleared++;
      for (int yy = y; yy > 0; yy--) {
        for (int x = 0; x < WIDTH; x++) {
          game[yy][x] = game[yy - 1][x];
        }
      }
      for (int x = 0; x < WIDTH; x++) {
        game[0][x] = EMPTY;
      }
    }
  }
  score += rows_cleared * rows_cleared * WIDTH;
}

void checkLose(Piece *p) {
  for (int y = 0; y < PIECE_SIZE; y++) {
    for (int x = 0; x < PIECE_SIZE; x++) {
      if (p->blocks[y][x].x >= 0 && p->blocks[y][x].y >= 0 &&
          game[p->blocks[y][x].y][p->blocks[y][x].x] != EMPTY) {
        is_playing = 0;
        return;
      }
    }
  }
}

Piece generateRandomPiece() {
  Piece p;
  int kind = rand() % 7;
  int rotation = rand() % 4;
  Point top_left = {WIDTH / 2 - PIECE_SIZE / 2 + 1, 1};
  for (int y = 0; y < PIECE_SIZE; y++) {
    for (int x = 0; x < PIECE_SIZE; x++) {
      if (pieces[kind][rotation][y][x]) {
        p.blocks[y][x] = (Point){top_left.x + x, top_left.y + y};
        p.color[y][x] = pieces[kind][rotation][y][x];
      } else {
        p.blocks[y][x] = (Point){-1, -1};
        p.color[y][x] = EMPTY;
      }
    }
  }
  p.top_left = top_left;
  p.is_moving = 1;
  p.dx = 0;
  p.rotation = rotation;
  p.kind = kind;
  return p;
}

void rotatePiece(Piece *p) {
  Piece new_blocks;
  new_blocks.is_moving = 0;
  new_blocks.dx = 0;
  Point rotation_point = {-1, -1};
  for (int y = 0; y < PIECE_SIZE; y++) {
    for (int x = 0; x < PIECE_SIZE; x++) {
      if (pieces[p->kind][p->rotation][y][x]) {
        new_blocks.blocks[y][x] = (Point){p->top_left.x + x, p->top_left.y + y};
        if (pieces[p->kind][p->rotation][y][x] == 2) {
          rotation_point = new_blocks.blocks[y][x];
        }
      } else {
        new_blocks.blocks[y][x] = (Point){-1, -1};
      }
    }
  }
  Point old_rotation_point = {-1, -1};
  for (int y = 0; y < PIECE_SIZE; y++) {
    for (int x = 0; x < PIECE_SIZE; x++) {
      if (p->color[y][x] == 2) {
        old_rotation_point = p->blocks[y][x];
      }
    }
  }
  centerRotation(new_blocks.blocks, &rotation_point, &old_rotation_point);
  if (verifyMove(&new_blocks)) {
    for (int y = 0; y < PIECE_SIZE; y++) {
      for (int x = 0; x < PIECE_SIZE; x++) {
        p->blocks[y][x] = new_blocks.blocks[y][x];
        p->color[y][x] = pieces[p->kind][p->rotation][y][x];
      }
    }
    return;
  }
  p->rotation--;
}

void centerRotation(Point p[PIECE_SIZE][PIECE_SIZE], Point *rotation_point,
                    Point *old_rotation_point) {
  int dx = rotation_point->x < old_rotation_point->x ? 1 : -1;
  int dy = rotation_point->y < old_rotation_point->y ? 1 : -1;
  while (rotation_point->x != old_rotation_point->x ||
         rotation_point->y != old_rotation_point->y) {
    if (old_rotation_point->x == rotation_point->x)
      dx = 0;
    if (old_rotation_point->y == rotation_point->y)
      dy = 0;
    for (int y = 0; y < PIECE_SIZE; y++) {
      for (int x = 0; x < PIECE_SIZE; x++) {
        if (p[y][x].x < 0 || p[y][x].y < 0 || p[y][x].x >= WIDTH ||
            p[y][x].y >= HEIGHT) {
          continue;
        }
        if (p[y][x].y + dy < 0 || p[y][x].y + dy >= HEIGHT) {
          continue;
        } else if (game[p[y][x].y + dy][p[y][x].x] != EMPTY) {
          continue;
        }
        p[y][x].x += dx;
        p[y][x].y += dy;
      }
    }
    rotation_point->x += dx;
    rotation_point->y += dy;
  }
}

int getColorForPiece(int c) {
  switch (c) {
  case EMPTY:
    return BLACK;
  case 1:
    return RED;
  case 2:
    return GREEN;
  default:
    return WHITE;
  }
}
