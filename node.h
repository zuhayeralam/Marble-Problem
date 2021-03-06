/*
 * Specification for the Node ADT
 * Author Julian Dermoudy
 * Version 10/4/18
*/

#include <stdbool.h>

struct node_int;
typedef struct node_int *node;

void init_node(node *v, void *o);
void set_data(node v, void *o);
void set_next(node v, node n);
void *get_data(node v);
node get_next(node v);