#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct character_node {
    char val;
    struct character_node *next;
} cNode;

typedef struct character_list {
    cNode *head;
    cNode *tail;
    int n;
} cList;

typedef struct adjlist_node {
    int node_id;
    int timestamp;
    struct adjlist_node *next;
} adjNode;

typedef struct adjlist_list {
    adjNode* adArr;
    int n;
} adjList;

extern cList * cList_create();

extern void cList_add(cList * lst, char c);

extern void cList_reset_count(cList * lst);

extern void cList_reset_count(cList * lst);

extern void cList_remove(cList * lst);

extern char * cList_to_string(cList * lst);
