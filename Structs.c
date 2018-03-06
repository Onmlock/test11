#include "Structs.h"

cList * cList_create()
{
  cList * nlist = malloc(sizeof(cList));
  nlist->head = malloc(sizeof(cNode));
  nlist->tail = malloc(sizeof(cNode));

  nlist->head = NULL;
  nlist->tail = NULL;
  nlist->n = 0;

  return nlist;
}

void cList_add(cList * lst, char c)
{
  if (lst->head == NULL) {
    cNode* nnode = malloc(sizeof(cNode));

    nnode->next = lst->tail;

    lst->head = nnode;
    lst->tail = nnode;

  }
  if (lst->tail->next == NULL) {
    cNode* nnode = malloc(sizeof(cNode));
    nnode->next = NULL;

    lst->tail->next = nnode;
  }


  lst->tail->val = c;
  lst->tail = lst->tail->next;


  lst->n++;
}

void cList_reset_count(cList * lst)
{
  lst->tail = lst->head;
  lst->n = 0;
}

cNode * list_remove_r(cNode * c)
{
  if (c == NULL)
    return c;

  list_remove_r(c->next);

  free(c);

}

void cList_remove(cList * lst)
{
  list_remove_r(lst->head->next);
  lst->n = 0;
}

char * cList_to_string(cList * lst)
{
  cNode* temp = lst->head;

  char* s = malloc(lst->n * sizeof(char));
  int i = 0;

  for(i = 0; i < lst->n; i++) {
    s[i] = temp->val;
    temp = temp->next;
  }

  s[lst->n] = '\0';
  return s;
}
