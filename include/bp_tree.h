#ifndef BPTREE_H
#define BPTREE_H

typedef struct node Node;

typedef struct bptree {
  Node *root;
  void (*insert)(struct bptree *, char *, int);
  Node *(*search)(char *);
  void (*update_root)(struct bptree *, int);
} BPTree;

/* ========== Node ========== */

// Constructor
Node *Node__create(int is_leaf);
// File manipulation
Node *Node__read(int rrn);
void Node__append(Node *node);
void Node__write(Node *node, int rrn);
// Utilities
void debug();
void Node__list_range(Node *node, char *key);
char *Node__get_lower_key(BPTree *tree);
void Node__print(Node *node);
void Node__update_parent(Node *node);
// Split
void Node__split_leaf(Node *dest, Node *ori);
char *Node__split_internal(Node *dest, Node *ori);
// Search
Node *Node__search(char *key);
int Node__search_key(char *key);
// Insertion
void Node__insert(BPTree *tree, char *key, int data_rrn);
void Node__insert_at_leaf(Node *leaf, char *key, int data_rrn);
void Node__insert_in_parent(BPTree *tree, Node *old_node, Node *new_node,
                            char *promoted_key);

/* ========== B+ Tree ========== */

// Constructor
BPTree *BPTree__init();
// Destructor
void BPTree__free(BPTree *tree);
// Root manipulation
Node *BPTree__read_root();
void BPTree__update_root(BPTree *tree, int root_rrn);

#endif // !BPTREE_H
