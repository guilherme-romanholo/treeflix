#include "../include/buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 192

// Atributos do buffer
struct buffer_atr {
  char content[MAX_SIZE];
  int pos;
  int sizes[6];
  int total_size;
};

// Criação do buffer
Buffer *Buffer__create() {
  Buffer *buff = malloc(sizeof(Buffer));
  buff->atr = malloc(sizeof(struct buffer_atr));

  strcpy(buff->atr->content, "");
  buff->atr->pos = 0;
  buff->atr->total_size = 0;
  buff->append = Buffer__append;
  buff->get_content = Buffer__content;
  buff->get_total = Buffer__total;

  return buff;
}

// Libera o buffer na memória
void Buffer__destroy(Buffer *self) {
  free(self->atr);
  free(self);
}

// Adiciona os valores ao buffer
void Buffer__append(Buffer *self, char *value) {
  int len = strlen(value);

  if (self->atr->pos < 6 && self->atr->total_size + len <= MAX_SIZE) {
    strcat(self->atr->content, value);
    self->atr->total_size += len;
    if (strcmp(value, "@")) {
      Buffer__append(self, "@");
      self->atr->sizes[self->atr->pos] = len;
      self->atr->pos++;
    }
  } else {
    printf("Buffer exception: O conteúdo ultrapassa 192 bytes!\n");
    exit(1);
  }

  // Deixar para completar só quando for escrever no arquivo
  if (self->atr->pos == 6)
    for (int i = self->atr->total_size; i < MAX_SIZE; i++)
      self->atr->content[i] = '#';
}

// Getter para o conteudo
char *Buffer__content(Buffer *self) { return self->atr->content; }

// Getter do tamanho total
int Buffer__total(Buffer *self) { return self->atr->total_size; }
