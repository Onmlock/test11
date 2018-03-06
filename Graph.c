#include "Graph.h"

gNode * gNode_create_with_index(int index)
{
  gNode* nnode = malloc(sizeof(gNode));

  nnode->index = index;
  nnode->visited = 0;
  nnode->next = NULL;
  nnode->timestamp = 0;

  return nnode;
}

gList * gList_create(int numOfFiles, int targetCutOff)
{
  gList* nlist = malloc(sizeof(gList));

  nlist->fileNameArray = malloc(numOfFiles * sizeof(char*));
  nlist->listSize = numOfFiles;
  nlist->targetStart = targetCutOff;
  nlist->gArr = malloc(nlist->listSize * sizeof(gNode*));

  int i;
  for (i = 0; i < numOfFiles; i++) {
      nlist->gArr[i] = gNode_create_with_index(i);
  }

  return nlist;
}

void gList_add_to_list (gList* lst, int parentIndex, int selfIndex)
{
  //printf("%i, %i\n", parentIndex, selfIndex);
  gNode* temp = lst->gArr[parentIndex];

  while (temp->next != NULL)
    temp = temp->next;

  temp->next = gNode_create_with_index(selfIndex);
}

void print_gNode(gList* lst, int index)
{
  gNode* temp = lst->gArr[index]->next;
  while (temp != NULL) {
    printf("%s -> ",lst->fileNameArray[temp->index]);
    temp = temp->next;
  }
  printf("N");
}

void print_gList(gList* lst)
{
  int i;

  for (i = 0; i < lst->listSize; i++) {
    printf("%s: ", lst->fileNameArray[i]);
    print_gNode(lst, i);
    printf("\n");
  }
}

void reset_visited(gList* lst)
{
  int i;

  for (i = 0; i < lst->listSize; i++)
    lst->gArr[i]->visited = 0;
}

int dfs_r(gList* lst, int index)
{
  if (lst->gArr[index]->visited == 1)
    return 1;
  else if (lst->gArr[index]->visited == 2)
    return 0;
  else
    lst->gArr[index]->visited = 1;

  gNode* temp = lst->gArr[index]->next;
  while (temp != NULL) {
    if (dfs_r(lst, temp->index) == 1) {
      return 1;
    }
    temp = temp->next;
  }
  lst->gArr[index]->visited = 2;
  return 0;

}

int contains_cycle(gList* lst)
{
  int i;
  gNode* temp;
  //first check if there are loops within a file's own dependencies
  for (i = 0; i < lst->listSize; i++) {
    temp = lst->gArr[i]->next;
    while (temp != NULL) {
      if (temp->index == i) {
        fprintf(stderr, "A file cannot be dependent on itself, but %s is.\n", lst->fileNameArray[i]);
        return 1;
      }
      temp = temp->next;
    }
  }

  for (i = 0; i < lst->listSize; i++) {
    if (dfs_r(lst, i) == 1) {
      fprintf(stderr, "A cycle was found in %s.\n", lst->fileNameArray[i]);
      return 1;
    }
    reset_visited(lst);
  }

  return 0;
}




