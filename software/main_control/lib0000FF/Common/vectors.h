#ifndef VECTORS_H
#define VECTORS_H

#include "stdint.h"
#include  <math.h>

typedef struct _Vector2f
{
  float x;
  float y;
} Vector2f;

typedef struct _Vector3f {
  float x;
  float y;
  float z;
} Vector3f;

typedef struct _Vector4f {
  float q0;
  float q1;
  float q2;
  float q3;
} Vector4f;

float norm2(Vector2f* v);
float norm3(Vector3f* v);
float norm4(Vector4f* v);

#endif
