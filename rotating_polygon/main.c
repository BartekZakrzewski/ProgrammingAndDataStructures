#include "primlib.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define N_VERTICES 8
#define RADIUS_STEP 2
#define MIN_RADIUS 20
#define MAX_RADIUS 200
#define ROTATION 0.02 * M_PI

typedef struct {
  int x;
  int y;
} Point;

const double ANGLE_BETWEEN_POINTS = (2.0 * M_PI) / N_VERTICES;

void calculate_points_position(Point *point, double *rotation_offset,
                               Point *center, int *radius);
void draw_polygon(Point *point);

int main(int argc, char *argv[]) {
  if (gfx_init()) {
    exit(3);
  }

  Point points[N_VERTICES];
  double rotation_offset = 0.0;
  int radius = MIN_RADIUS + RADIUS_STEP;
  int grow_direction = 1;

  Point center;
  center.x = gfx_screenWidth() / 2;
  center.y = gfx_screenHeight() / 2;

  while (1) {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

    calculate_points_position(points, &rotation_offset, &center, &radius);
    rotation_offset = fmod(rotation_offset + ROTATION, 2.0 * M_PI);
    grow_direction = radius >= MAX_RADIUS || radius <= MIN_RADIUS
                         ? -1 * grow_direction
                         : grow_direction;
    radius += grow_direction * RADIUS_STEP;

    draw_polygon(points);

    if (gfx_pollkey() == 'q') {
      break;
    }

    gfx_updateScreen();
    SDL_Delay(10);
  }

  return 0;
}

void calculate_points_position(Point *point, double *rotation_offset,
                               Point *center, int *radius) {
  for (int vertex = 0; vertex < N_VERTICES; vertex++) {
    double angle = vertex * ANGLE_BETWEEN_POINTS + *rotation_offset;
    (point + vertex)->x = center->x + *radius * cos(angle);
    (point + vertex)->y = center->y + *radius * sin(angle);
  }
}

void draw_polygon(Point *point) {
  for (int vertex = 0; vertex < N_VERTICES; vertex++) {
    gfx_line((point + vertex)->x, (point + vertex)->y,
             (point + (vertex + 1) % N_VERTICES)->x,
             (point + (vertex + 1) % N_VERTICES)->y, YELLOW);
  }
}
