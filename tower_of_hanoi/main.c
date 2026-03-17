#include "primlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// CONSTANTS
#define PEG_HEIGHT 400
#define PADDING 10
#define FLOOR 3 * PADDING
#define FLOOR_HEIGHT 20
#define DELAY 10

// PARAMETERS
#define N_PEGS 2
#define N_DISCS 60
#define DISK_STEP 30

// KEYS
const int KEYS[10] = {SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
                      SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_0};
const int N_KEYS = 10;

// GLOBAL VARIABLES
int moves = 0;

// STRUCTS
typedef struct Disk {
  int x;
  int y;
  int width;
} Disk;

typedef struct {
  Disk disks[N_DISCS];
  int cx;
  int cy;
  int count;
} Peg;

// HELPER FUNCTIONS
int get_max_disk_width() { return gfx_screenWidth() / N_PEGS - PADDING; }
int get_min_disk_width() { return get_max_disk_width() / N_DISCS; }
int get_disk_height() { return (PEG_HEIGHT - PADDING) / N_DISCS; }
int get_peg_width() { return get_min_disk_width() - PADDING; }
void write_moves_to_string(char *_moves) {
  sprintf(_moves, "Moves: %d", moves);
}

// INIT FUNCTIONS
void initPegs(Peg *peg);
void initDisks(Peg *peg);

// VIEW FUNCTIONS
void drawStaticElements(Peg *pegs, int from_peg, int to_peg);
void drawDisks(Peg *pegs, int from_peg, int to_peg);
void drawStandardScreen(Peg *pegs, int target_disk);
void animateMove(Peg *pegs, int *from_peg, int *to_peg);
void drawWinScreen();

// MODEL FUNCTIONS
void handle_from_peg(int *from_peg);
void handle_to_peg(Peg *peg, int *from_peg, int *to_peg, int *target_disk);

// CONTROL FUNCTIONS
int handle_move(Peg *pegs, int *from_peg, int *to_peg);
int check_win(Peg *pegs);

int main(int argc, char *argv[]) {
  if (gfx_init()) {
    exit(3);
  }

  Peg pegs[N_PEGS];
  int target_disk = -1;
  int is_quitting = 0;
  int is_winning = 0;

  initPegs(pegs);
  initDisks(pegs);

  while (!is_winning && !(is_quitting = gfx_isKeyDown(SDLK_q))) {
    drawStandardScreen(pegs, target_disk);
    int from_peg = -1;
    int to_peg = -1;

    handle_from_peg(&from_peg);
    handle_to_peg(pegs, &from_peg, &to_peg, &target_disk);

    gfx_updateScreen();
    if (!!(is_winning = check_win(pegs))) {
      drawStandardScreen(pegs, target_disk);
      gfx_updateScreen();
      SDL_Delay(500);
      break;
    }
    SDL_Delay(DELAY);
  }

  if (is_quitting)
    return 0;

  while (gfx_pollkey() != 'q') {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

    gfx_textout(gfx_screenWidth() / 2 - 5 * PADDING,
                gfx_screenHeight() / 2 - 4 * PADDING, "You win!", WHITE);
    char *_moves = (char[20]){0};
    write_moves_to_string(_moves);
    gfx_textout(gfx_screenWidth() / 2 - 5 * PADDING,
                gfx_screenHeight() / 2 - 2 * PADDING, _moves, WHITE);
    gfx_textout(gfx_screenWidth() / 2 - 8 * PADDING,
                gfx_screenHeight() / 2 + 2 * PADDING, "Press [q] to quit",
                WHITE);

    gfx_updateScreen();
    SDL_Delay(DELAY);
  }

  return 0;
}

void initPegs(Peg *peg) {
  for (int p = 0; p < N_PEGS; p++) {
    (peg + p)->cx = p * (get_max_disk_width() + PADDING) + PADDING / 2 +
                    get_max_disk_width() / 2;
    (peg + p)->cy = gfx_screenHeight() - FLOOR;
    for (int d = 0; d < N_DISCS; d++) {
      (peg + p)->disks[d].width = -1;
      (peg + p)->count = 0;
    }
  }
}

void initDisks(Peg *peg) {
  int disk;
  int disk_width = get_max_disk_width();
  for (disk = 0; disk < N_DISCS; disk++) {
    peg->disks[disk].x = peg->cx;
    peg->disks[disk].y = peg->cy - disk * get_disk_height();
    peg->disks[disk].width = disk_width;
    disk_width -= get_min_disk_width();
  }
  peg->count = disk;
}

void drawStaticElements(Peg *pegs, int from_peg, int to_peg) {
  for (int peg = 0; peg < N_PEGS; peg++) {
    gfx_filledRect(
        (pegs + peg)->cx - get_peg_width() / 2, (pegs + peg)->cy - PEG_HEIGHT,
        (pegs + peg)->cx + get_peg_width() / 2, (pegs + peg)->cy, RED);
    char *text =
        peg < 9 ? (char[2]){(char)(peg + '1'), '\0'} : (char[2]){'0', '\0'};
    gfx_textout((pegs + peg)->cx - PADDING / 2, PADDING / 2, text,
                peg == from_peg ? GREEN
                : peg == to_peg ? RED
                                : WHITE);
  }
  gfx_filledRect(0, gfx_screenHeight() - FLOOR, gfx_screenWidth() - 1,
                 gfx_screenHeight() - 1, YELLOW);
  gfx_textout(gfx_screenWidth() / 2 - 6 * PADDING, 5 * PADDING,
              "Press [q] to quit", WHITE);
  char *_moves = (char[20]){0};
  write_moves_to_string(_moves);
  gfx_textout(gfx_screenWidth() / 2 - 3 * PADDING,
              gfx_screenHeight() - FLOOR - PEG_HEIGHT - 3 * PADDING, _moves,
              WHITE);
}

void drawDisks(Peg *pegs, int from_peg, int to_peg) {
  for (int peg = 0; peg < N_PEGS; peg++) {
    for (int disk = 0; disk < (pegs + peg)->count - (to_peg == peg ? 1 : 0);
         disk++) {
      if ((pegs + peg)->count > 0) {
        int disk_x = (pegs + peg)->disks[disk].x;
        int disk_width = (pegs + peg)->disks[disk].width;
        int disk_y = (pegs + peg)->disks[disk].y;
        gfx_filledRect(disk_x - (disk_width / 2), disk_y - get_disk_height(),
                       disk_x + (disk_width / 2), disk_y, BLUE);
        gfx_rect(disk_x - (disk_width / 2), disk_y - get_disk_height(),
                 disk_x + (disk_width / 2), disk_y, WHITE);
      }
    }
  }
}

void drawStandardScreen(Peg *pegs, int target_disk) {
  gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
  drawStaticElements(pegs, target_disk, -1);
  drawDisks(pegs, target_disk, -1);
}

void animateMove(Peg *pegs, int *from_peg, int *to_peg) {
  Disk moving_disk = (pegs + *from_peg)->disks[(pegs + *from_peg)->count];

  int disk_width = moving_disk.width;

  int disk_start_x = moving_disk.x;
  int disk_start_y = moving_disk.y;

  int disk_top_y = gfx_screenHeight() - PEG_HEIGHT - 2 * PADDING;

  int disk_end_x = (pegs + *to_peg)->cx;
  int disk_end_y =
      (pegs + *to_peg)->cy - ((pegs + *to_peg)->count - 1) * get_disk_height();

  for (int move_up = disk_start_y; move_up > disk_top_y; move_up -= DISK_STEP) {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

    drawStaticElements(pegs, *from_peg, *to_peg);
    drawDisks(pegs, *from_peg, *to_peg);

    gfx_filledRect(disk_start_x - (disk_width / 2), move_up - get_disk_height(),
                   disk_start_x + (disk_width / 2), move_up, BLUE);
    gfx_rect(disk_start_x - (disk_width / 2), move_up - get_disk_height(),
             disk_start_x + (disk_width / 2), move_up, WHITE);

    gfx_updateScreen();
    SDL_Delay(DELAY);
  }

  for (int move_horizontal = disk_start_x;
       abs(disk_end_x - disk_start_x) > abs(move_horizontal - disk_start_x);
       move_horizontal += DISK_STEP * (disk_end_x > disk_start_x ? 1 : -1)) {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

    drawStaticElements(pegs, *from_peg, *to_peg);
    drawDisks(pegs, *from_peg, *to_peg);

    gfx_filledRect(move_horizontal - (disk_width / 2),
                   disk_top_y - get_disk_height(),
                   move_horizontal + (disk_width / 2), disk_top_y, BLUE);
    gfx_rect(move_horizontal - (disk_width / 2), disk_top_y - get_disk_height(),
             move_horizontal + (disk_width / 2), disk_top_y, WHITE);

    gfx_updateScreen();
    SDL_Delay(DELAY);
  }

  for (int move_down = disk_top_y; move_down < disk_end_y;
       move_down += DISK_STEP) {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

    drawStaticElements(pegs, *from_peg, *to_peg);
    drawDisks(pegs, *from_peg, *to_peg);

    gfx_filledRect(disk_end_x - (disk_width / 2), move_down - get_disk_height(),
                   disk_end_x + (disk_width / 2), move_down, BLUE);
    gfx_rect(disk_end_x - (disk_width / 2), move_down - get_disk_height(),
             disk_end_x + (disk_width / 2), move_down, WHITE);

    if (move_down + DISK_STEP < disk_end_y) {
      gfx_updateScreen();
      SDL_Delay(DELAY);
    }
  }
}

void handle_from_peg(int *from_peg) {
  for (int key = 0; key < N_KEYS; key++) {
    if (gfx_isKeyDown(KEYS[key])) {
      *from_peg = key;
    }
  }
}

void handle_to_peg(Peg *peg, int *from_peg, int *to_peg, int *target_disk) {
  if (*from_peg != -1 && *target_disk == -1) {
    *target_disk = *from_peg;
  } else if (*from_peg != -1 && *target_disk != -1) {
    *to_peg = *from_peg;
    if (handle_move(peg, target_disk, to_peg)) {
      animateMove(peg, target_disk, to_peg);
      moves++;
      *target_disk = -1;
    }
  }
}

int handle_move(Peg *pegs, int *from_peg, int *to_peg) {
  if (*from_peg >= N_PEGS || *to_peg >= N_PEGS || *from_peg < 0 ||
      *to_peg < 0) {
    *from_peg = -1;
    return 0;
  }
  if ((pegs + *from_peg)->count <= 0) {
    *from_peg = -1;
    return 0;
  }
  if ((pegs + *to_peg)->count > 0 &&
      (pegs + *to_peg)->disks[(pegs + *to_peg)->count - 1].width <
          (pegs + *from_peg)->disks[(pegs + *from_peg)->count - 1].width) {
    *from_peg = -1;
    return 0;
  }
  if (*from_peg == *to_peg) {
    return 0;
  }

  (pegs + *to_peg)->disks[(pegs + *to_peg)->count] =
      (pegs + *from_peg)->disks[(pegs + *from_peg)->count - 1];
  (pegs + *from_peg)->count--;
  (pegs + *to_peg)->count++;
  (pegs + *to_peg)->disks[(pegs + *to_peg)->count - 1].x = (pegs + *to_peg)->cx;
  (pegs + *to_peg)->disks[(pegs + *to_peg)->count - 1].y =
      (pegs + *to_peg)->cy - ((pegs + *to_peg)->count - 1) * get_disk_height();
  return 1;
}

int check_win(Peg *pegs) {
  for (int peg = 1; peg < N_PEGS; peg++) {
    if ((pegs + peg)->count == N_DISCS)
      return 1;
  }
  return 0;
}
