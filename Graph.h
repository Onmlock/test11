/* These structs are for building the adjacency list.
 * gNodes are the nodes of the linked list, or the children of a file
 * gList will be the array of linked lists that acts as the actual adjacency list.
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct graph_Node {
  int index; //holds the index of the file in the graph
  int visited; //will be 0 for unvisited, 1 for visiting, 2 for visited
  struct gNode* next;
  int timestamp;
} gNode;

typedef struct adjacency_list {
  int listSize;
  int targetStart; //the line with the first colon
  char** fileNameArray; //array of file names
  gNode** gArr; //graphArray of gNode pointers
} gList;

extern gNode * gNode_create_with_index(int index);

extern gList * gList_create(int numOfFiles, int targetCutOff);

extern void gList_add_to_list (gList* lst, int parentIndex, int selfIndex);

extern void print_gNode(gList* lst, int index);

extern void print_gList(gList* lst);

extern void reset_visited(gList* lst);

extern int dfs_r(gList* lst, int index);

extern int contains_cycle(gList* lst);
