#ifndef BUFFER_H
#define BUFFER_H

typedef struct buffer {
  struct buffer_atr *atr;
  void (*append)(struct buffer *, char *);
  char *(*get_content)(struct buffer *);
  int (*get_total)(struct buffer *);
} Buffer;

Buffer *Buffer__create();
void Buffer__destroy(Buffer *self);
void Buffer__append(Buffer *self, char *value);
char *Buffer__content(Buffer *self);
int Buffer__total(Buffer *self);

#endif // !BUFFER_H
