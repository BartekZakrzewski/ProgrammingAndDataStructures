#include "primlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DELAY 10

#define RECW 400
#define RECH 200
#define CR 40

int main(int argc, char *argv[]) {
    if (gfx_init()) {
        exit(3);
    }
    const int W = gfx_screenWidth();
    const int H = gfx_screenHeight();
    const int sx = W / 2;
    const int sy = H / 2;

    const int tlx = sx - RECW / 2, tly = sy - RECH / 2;
    const int trx = sx + RECW / 2, try = sy - RECH / 2;
    const int blx = sx - RECW / 2, bly = sy + RECH / 2;
    const int brx = sx + RECW / 2, bry = sy + RECH / 2;

    int cx = sx + RECW / 2 - 20, cy = sy - RECH / 2 - CR;
    int dx = 1, dy = 0;
    double sangle = 0, eangle = 0;
    int dangle = 1;

    double SPEED = 2.0;
    double ANGLE_SPEED = (SPEED / CR);

    while (1) {
        if (sangle > eangle) {
            if (dangle == 1) {
                cx = trx + CR * cos(sangle);
                cy = try - CR * sin(sangle);
            } else if (dangle == 2) {
                cx = brx + CR * cos(sangle);
                cy = bry - CR * sin(sangle);
            } else if (dangle == 3) {
                cx = blx + CR * cos(sangle);
                cy = bly - CR * sin(sangle);
            } else if (dangle == 4) {
                cx = tlx + CR * cos(sangle);
                cy = tly - CR * sin(sangle);
            }
            sangle -= ANGLE_SPEED;
            if (sangle <= eangle)
                sangle = eangle;
        } else if (cx < trx && dx == 1 && dy == 0) {
            cx += SPEED;
            if (cx >= trx) {
                dx = 0;
                dy = 1;
                sangle = M_PI / 2.0;
                eangle = 0.0;
                dangle = 1;
            }
        } else if (cy < bry && dx == 0 && dy == 1) {
            cy += SPEED;
            if (cy >= bry) {
                dx = -1;
                dy = 0;
                sangle = 0.0;
                eangle = -M_PI / 2.0;
                dangle = 2;
            }
        } else if (cx > blx && dx == -1 && dy == 0) {
            cx -= SPEED;
            if (cx <= blx) {
                dx = 0;
                dy = -1;
                sangle = (3.0 * M_PI) / 2.0;
                eangle = M_PI;
                dangle = 3;
            }
        } else if (cy > tly && dx == 0 && dy == -1) {
            cy -= SPEED;
            if (cy <= tly) {
                dx = 1;
                dy = 0;
                sangle = M_PI;
                eangle = M_PI / 2.0;
                dangle = 4;
            }
        }

        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
                       BLACK);

        gfx_rect(tlx, tly, brx, bry, WHITE);
        gfx_circle(cx, cy, CR, RED);

        int key = gfx_pollkey();
        if (key == SDLK_ESCAPE) {
            break;
        }

        if (key == 'r') {
            cx = sx;
            cy = sy - RECH / 2 - CR;
            dx = 1;
            dy = 0;
            sangle = 0;
            eangle = 0;
            dangle = 0;
        }

        if (gfx_isKeyDown(SDLK_SPACE)) {
            SPEED = 0;
            ANGLE_SPEED = 0;
        } else {
            SPEED = 2.0;
            ANGLE_SPEED = (SPEED / CR);
        }

        gfx_updateScreen();
        SDL_Delay(DELAY);
    }

    return 0;
}
