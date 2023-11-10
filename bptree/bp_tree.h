#ifndef BPTREE_H
#define BPTREE_H

typedef struct node Node;

typedef struct bptree {
  Node *(*get_root)();
  void (*update_root)(int);
  void (*insert)(char *, int);
} BPTree;

Node *Node__create(int is_leaf);
void Node__append(Node *node);
void Node__rewrite(Node *node, int rrn);
Node *Node__read(int rrn);
void Node__print(Node *node);
void Node__insert(char *key, int data_rrn);
BPTree *BPTree__init();
void debug();

#endif // !BPTREE_H
