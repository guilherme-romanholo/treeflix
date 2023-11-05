#include "../include/buffer.h"
#define MAX_SIZE 192

struct buffer {
  char content[MAX_SIZE];
  int sizes[6];
  int total_size;
};
