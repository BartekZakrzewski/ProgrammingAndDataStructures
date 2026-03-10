#include "../primlib/primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define N_VERTICES 3
#define RADIUS_STEP 2
#define MIN_RADIUS 20
#define MAX_RADIUS 200
#define ROTATION 0.02

typedef struct {
  int x;
  int y;
} Point;

const double rad = (2.0 * M_PI) / N_VERTICES;

void calculate_point_rotation(Point *p, double *rotation_offset, Point *center, int *radius);
void draw_polygon(Point *p);

int main(int argc, char* argv[]) {
  if (gfx_init()) {
    exit(3);
  }

  Point points[N_VERTICES];
  double rotation_offset = 0.0;
  int radius = MIN_RADIUS + RADIUS_STEP;
  int is_growing = 1;

  Point center; center.x = gfx_screenWidth() / 2; center.y = gfx_screenHeight() / 2;

  while (1) {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

    calculate_point_rotation(points, &rotation_offset, &center, &radius);
    rotation_offset = rotation_offset >= 2.0 * M_PI ? 0 : rotation_offset + ROTATION;
    is_growing = radius >= MAX_RADIUS || radius <= MIN_RADIUS ? -1 * is_growing : is_growing;
    radius += is_growing * RADIUS_STEP;

    draw_polygon(points);

    if (gfx_pollkey() == 3) {
      break;
    }

    gfx_updateScreen();
    SDL_Delay(10);
  }

  return 0;
}

void calculate_point_rotation(Point *p, double *rotation_offset, Point *center, int *radius) {
  for (int i = 0; i < N_VERTICES; i++) {
    double angle = i * rad + *rotation_offset;
    (p + i)->x = center->x + *radius * cos(angle);
    (p + i)->y = center->y + *radius * sin(angle);
  }
}

void draw_polygon(Point *p) {
  for (int i = 0; i < N_VERTICES; i++) {
    gfx_line((p + i)->x, (p + i)->y, (p + (i+1)%N_VERTICES)->x, (p + (i+1)%N_VERTICES)->y, YELLOW);
  }
}

