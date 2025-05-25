#include "vectors.h"

float norm2(Vector2f* v) { return sqrtf(v->x * v->x + v->y * v->y); }
float norm3(Vector3f* v) {
  return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}
float norm4(Vector4f* v) {
  return sqrtf(v->q0 * v->q0 + v->q1 * v->q1 + v->q2 * v->q2 + v->q3 * v->q3);
}
