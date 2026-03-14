#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define N_PEGS 5
#define N_DISCS 5

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

const int PEG_HEIGHT = 400;
const int PADDING = 10;
const int FLOOR_HEIGHT = 20;
const int DISK_STEP = 1;

void initPegs(Peg *peg, int peg_width, int max_disk_width);
void initDisks(Peg *peg, int min_disk_width, int max_disk_width, int disk_height, int peg_width);

void drawStaticElements(Peg *pegs, int peg_width, int max_disk_width);
void drawDisks(Peg *pegs, int disk_height, int moving_index);

int handle_move(Peg *pegs, int *key_queue, int disk_height);
void check_win(Peg *pegs);

int main(int argc, char* argv[]) {
    if (gfx_init()) {
        exit(3);
    }

    const int MAX_DISK_WIDTH = (gfx_screenWidth() / N_PEGS) - PADDING;
    const int MIN_DISK_WIDTH = MAX_DISK_WIDTH / N_DISCS;
    const int DISK_HEIGHT = (PEG_HEIGHT - PADDING) / N_DISCS;
    const int PEG_WIDTH = MIN_DISK_WIDTH - PADDING;

    Peg pegs[N_PEGS];
    int key_queue[2];
    int key_index = 0;

    int is_moving = 0;
    int x_dir = 0;
    int y_dir = 0;

    initPegs(pegs, PEG_WIDTH, MAX_DISK_WIDTH);
    initDisks(pegs, MIN_DISK_WIDTH, MAX_DISK_WIDTH, DISK_HEIGHT, PEG_WIDTH);

    while (1) {
        if (key_index > 1) {
            is_moving = handle_move(pegs, key_queue, DISK_HEIGHT);
            key_index = 0;
            x_dir = -1;

            continue;
        }

        if (is_moving) {
            if (x_dir < 0 && (pegs + *key_queue)->disks[(pegs + *key_queue)->count].y > gfx_screenHeight() - PEG_HEIGHT - PADDING*2) {
                // printf("Moving up\n");
                (pegs + *key_queue)->disks[(pegs + *key_queue)->count].y -= DISK_STEP;
            }
            if (x_dir < 0 && (pegs + *key_queue)->disks[(pegs + *key_queue)->count].y <= gfx_screenHeight() - PEG_HEIGHT - PADDING*2) {
                x_dir = 0;
                y_dir = *key_queue < *(key_queue + 1) ? 1 : -1;
            }
            if (y_dir != 0 && (pegs + *key_queue)->disks[(pegs + *key_queue)->count].x != (pegs + *(key_queue + 1))->disks[(pegs + *(key_queue + 1))->count - 1].x) {
                // printf("Moving sideways\n");
                (pegs + *key_queue)->disks[(pegs + *key_queue)->count].x += y_dir * DISK_STEP;
                // printf("%d %d\n", (pegs + *key_queue)->disks[(pegs + *key_queue)->count].x, (pegs + *(key_queue + 1))->disks[(pegs + *(key_queue + 1))->count - 1].x);
            }
            if (y_dir != 0 && (pegs + *key_queue)->disks[(pegs + *key_queue)->count].x == (pegs + *(key_queue + 1))->disks[(pegs + *(key_queue + 1))->count - 1].x) {
                y_dir = 0; x_dir = 1;
            }
            if (x_dir > 0 && (pegs + *key_queue)->disks[(pegs + *key_queue)->count].y < (pegs + *(key_queue + 1))->disks[(pegs + *(key_queue + 1))->count - 1].y) {
                // printf("Moving down\n");
                (pegs + *key_queue)->disks[(pegs + *key_queue)->count].y += DISK_STEP;
            }
            if (x_dir > 0 && (pegs + *key_queue)->disks[(pegs + *key_queue)->count].y >= (pegs + *(key_queue + 1))->disks[(pegs + *(key_queue + 1))->count - 1].y) {
                x_dir = 0;
                is_moving = 0;
            }
        }

        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

        drawStaticElements(pegs, PEG_WIDTH, MAX_DISK_WIDTH);
        drawDisks(pegs, DISK_HEIGHT, is_moving ? *key_queue : -1);

        if (!is_moving) {
            int key_pressed = gfx_pollkey();
            if (key_pressed >= 48 && key_pressed <= 57) {
                *(key_queue + key_index) =  key_pressed - 48;
                key_index++;
                printf("%d\n", key_pressed);
            }
        }

        gfx_updateScreen();
        SDL_Delay(10);
    }

    return 0;
}

void initPegs(Peg *peg, int peg_width, int max_disk_width) {
    for (int p = 0; p < N_PEGS; p++) {
        (peg + p)->cx = p * (max_disk_width + PADDING) + PADDING / 2 + max_disk_width / 2;
        (peg + p)->cy = gfx_screenHeight() - PADDING;
    }
}

void initDisks(Peg *peg, int min_disk_width, int max_disk_width, int disk_height, int peg_width) {
    int disk;
    int disk_width = max_disk_width;
    for (disk = 0; disk < N_DISCS; disk++) {
        peg->disks[disk].x = peg->cx;
        peg->disks[disk].y = peg->cy - disk * disk_height;
        peg->disks[disk].width = disk_width;
        disk_width -= min_disk_width;
    }
    peg->count = disk;
    for (int p = 1; p < N_PEGS; p++) {
        (peg + p)->count = 0;
    }
}

void drawStaticElements(Peg *pegs, int peg_width, int max_disk_width) {
    for (int peg = 0; peg < N_PEGS; peg++) {
        gfx_filledRect((pegs + peg)->cx - peg_width / 2, (pegs + peg)->cy - PEG_HEIGHT, (pegs + peg)->cx + peg_width / 2, (pegs + peg)->cy, RED);
    }
    gfx_filledRect(0, gfx_screenHeight() - PADDING, gfx_screenWidth() - 1, gfx_screenHeight() - 1, YELLOW);
}

void drawDisks(Peg *pegs, int disk_height, int moving_index) {
    for (int peg = 0; peg < N_PEGS; peg++) {
        for (int disk = 0; disk < (pegs + peg)->count + (moving_index == peg ? 1 : 0); disk++) {
            if ((pegs + peg)->count > 0) {
                int disk_x = (pegs + peg)->disks[disk].x;
                int disk_width = (pegs + peg)->disks[disk].width;
                int disk_y = (pegs + peg)->disks[disk].y;
                gfx_filledRect(disk_x - (disk_width / 2), disk_y - disk_height, disk_x + (disk_width / 2), disk_y, BLUE);
                gfx_rect(disk_x - (disk_width / 2), disk_y - disk_height, disk_x + (disk_width / 2), disk_y, WHITE);
                // gfx_filledRect((pegs + peg)->disks[disk].x, (pegs + peg)->disks[disk].y, (pegs + peg)->disks[disk].width, (pegs + peg)->disks[disk].y + disk_height, BLUE);
            }
        }
    }
}

int handle_move(Peg *pegs, int *key_queue, int disk_height) {
    if ((pegs + *key_queue)->count <= 0) {
        return 0;
    } 
    if ((pegs + *(key_queue + 1))->count > 0 && (pegs + *(key_queue + 1))->disks[(pegs + *(key_queue + 1))->count - 1].width < (pegs + *key_queue)->disks[(pegs + *key_queue)->count - 1].width) {
        return 0;
    }
    if (*key_queue == *(key_queue + 1)) {
        return 0;
    }

    printf("Handling move: %d %d\n", *key_queue, *(key_queue + 1));

    (pegs + *(key_queue + 1))->disks[(pegs + *(key_queue + 1))->count] = (pegs + *key_queue)->disks[(pegs + *key_queue)->count - 1];
    (pegs + *key_queue)->count--;
    (pegs + *(key_queue + 1))->count++;
    (pegs + *(key_queue + 1))->disks[(pegs + *(key_queue + 1))->count - 1].x = (pegs + *(key_queue + 1))->cx;
    (pegs + *(key_queue + 1))->disks[(pegs + *(key_queue + 1))->count - 1].y = (pegs + *(key_queue + 1))->cy - ((pegs + *(key_queue + 1))->count - 1) * disk_height;
    return 1;
    printf("New count: from: %d to: %d\n", (pegs + *key_queue)->count, (pegs + *(key_queue + 1))->count);
}

void check_win(Peg *pegs) {
    for (int peg = 1; peg < N_PEGS; peg++) {
        if ((pegs + peg)->count == N_DISCS) {
            printf("You won!\n");
            return;
        }
    }
}
