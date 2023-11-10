#ifndef BPTREE_H
#define BPTREE_H

typedef struct node Node;
Node *Node__create(int is_leaf);
void Node__write(Node *node);
Node *Node__read(int rrn);
void Node__print(Node *node);
void debug();

#endif // !BPTREE_H
