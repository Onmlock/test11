#include "hmap.h"
#include "Structs.h"
#include "Graph.h"


/*---------HELPER FUNCTIONS---------*/

void set_dependencies(gList* lst, HMAP* filemap, char* depString, int index)
{
  int i;
  char c;
  int maxlen = strlen(depString);
  char depFile[maxlen];
  int count = 0;

  for (i = 0; i < maxlen; i++)
    depFile[i] = '\0';

  for (i = 1; i < maxlen; i++) {
    if (depString[i-1] == 32) {
      do {
        c = depString[i++];
        if (c == 32)
          break;
        depFile[count++] = c;
      } while (c != '\0');

      depFile[count+1] = '\0';

      gList_add_to_list(lst, index, hmap_get(filemap, depFile));
      count = 0;
      i--;
    }
  }
}

void touch_file(gList* lst, HMAP* filemap, char* command, int *clock)
{
  if (hmap_contains(filemap, command) == 0 || hmap_get(filemap, command) >= lst->targetStart) {
    fprintf(stderr, "%s is not a basic file.", command);
  }
  else {
    int index = hmap_get(filemap, command);
    printf("Updating %s from %i to %i.\n", lst->fileNameArray[index], lst->gArr[index]->timestamp, ++*clock);
    lst->gArr[index]->timestamp = *clock;
  }
}

void timestamp_get(gList* lst, HMAP* filemap, char* command)
{
  if (hmap_contains(filemap, command) == 0) {
    fprintf(stderr, "Please enter a valid file name.\n");
  }
  else {
    int index = hmap_get(filemap, command);
    printf("The timestamp on %s is %i.\n", lst->fileNameArray[index], lst->gArr[index]->timestamp);
  }
}

int max_int(int a, int b)
{
  if (a > b)
    return a;
  else
    return b;
}

int make_func_r(gList* lst, int index, int *clock)
{
  gNode* current;
  gNode* temp;

  current = lst->gArr[index];
  temp = lst->gArr[index]->next;

  int maxstamp = current->timestamp;
  int childstamp = 0;

  while (temp != NULL) {
    childstamp = max_int(make_func_r(lst, temp->index, clock), childstamp);
    temp = temp->next;
  }

  if (childstamp > maxstamp)
    current->timestamp = ++*clock;

  return max_int(maxstamp, childstamp);
}

void make_func(gList* lst, HMAP* filemap, char* command, int *clock)
{
  if (hmap_contains(filemap, command) == 0) {
    fprintf(stderr, "Please enter a valid file name.\n");
  }
  else {
    int index = hmap_get(filemap, command);
    printf("Making %s...\n", lst->fileNameArray[index]);

    int newstamp = make_func_r(lst, index, clock);
    if (newstamp > lst->gArr[index]->timestamp)
      lst->gArr[index]->timestamp = ++*clock;

    printf("%s is up to date.\n", lst->fileNameArray[index]);
  }
}
/*---------END HELPER FUNCTIONS---------*/

int main(int argc, char** argv)
{
  int i;
  FILE* makefilename = fopen(argv[1], "r");

  if (makefilename == NULL) {
    fprintf(stderr, "Could not open file %s.\n", argv[1]);
    return 0;
  }

  char c;
  int arrsize = 0;
  int targetCutOff = -1;
  int maxlen = 0; //longest file name

  //loop to find how many files are given
  do {
    c = fgetc(makefilename);
    if (c == '\n')
      arrsize++;
    if (c == 58 && targetCutOff == -1)
      targetCutOff = arrsize;
  } while (c != EOF);
  rewind(makefilename);

  gList* lst = gList_create(arrsize, targetCutOff);
  HMAP* filemap = hmap_create(arrsize, 1.0);

  char** dependencies[arrsize - targetCutOff];

  cList* tempstring = cList_create(); //a linked list of chars to represent strings of unknown sizes

  arrsize = 0; //we just needed arrsize to hold the total amount of files, now it will be the counter for fileArray
  int stoploop = 1;

  //begin parsing file and building lists/graph
  do {
    c = fgetc(makefilename);
    if (c == EOF)
      stoploop = 0;
    else {
      if (c != 32 && c != 10) { //   \n = 10     space = 32       : = 58
        cList_add(tempstring, c);
      }
      else if (c == 32){
        c = fgetc(makefilename);
        if (c == 58) {
          lst->fileNameArray[arrsize] = cList_to_string(tempstring);

          hmap_set(filemap, lst->fileNameArray[arrsize], arrsize);
          cList_reset_count(tempstring);

          do {
            c = fgetc(makefilename);
            if (c == 10)
              break;
            cList_add(tempstring, c);
          } while (c != 10 && c != EOF);

          dependencies[arrsize - targetCutOff] = cList_to_string(tempstring);
          arrsize++;
        }
      }
    }
    if (tempstring->head->val == 32 || tempstring->head->val == 10)
      cList_reset_count(tempstring);
    else if (c == 32 || c == 10 || c == -1) {
      lst->fileNameArray[arrsize] = cList_to_string(tempstring);
      cList_reset_count(tempstring);

      hmap_set(filemap, lst->fileNameArray[arrsize], arrsize);
      arrsize++;
    }
  } while (stoploop);

  cList_remove(tempstring);
  free(tempstring);
  fclose(makefilename);

  //im using stoploop to count through the name array to make sure there are no duplicate names
  for (i = 0; i < arrsize; i++) {
    if (strlen(lst->fileNameArray[i]) > maxlen) //might as well set maxlen while we're at it
      maxlen = strlen(lst->fileNameArray[i]);
    for (stoploop = 0; stoploop < arrsize; stoploop++) {
      if (i == stoploop)
        continue;
      if (strcmp(lst->fileNameArray[i], lst->fileNameArray[stoploop]) == 0) {
        fprintf("Two files cannot have the same name, %s is a duplicate.", lst->fileNameArray[i]);
        return 0;
      }
    }
  }


  // print statements to make sure everything was loaded and hashed correctly
  /*
  printf("\nFull list of names:\n");
  for (i = 0; i < arrsize; i++)
    printf("%s\n", lst->fileNameArray[i]);
  /*
  printf("\nList of dependencies:\n");
  for (i = 0; i < arrsize - targetCutOff; i++)
    printf("%s\n", dependencies[i]);

  printf("\nHmap values: %i\n", arrsize);
  for (i = 0; i < arrsize; i++)
    printf("%s->%i\n", lst->fileNameArray[i], hmap_get(filemap, lst->fileNameArray[i]));
  */

  for (i = 0; i < (arrsize - targetCutOff); i++)
    set_dependencies(lst, filemap, dependencies[i], targetCutOff + i);

  printf("\nList:\n");
  print_gList(lst);

  if (contains_cycle(lst) == 1)
    return 0;



  //now it's time to start the time.

  int clock = 0;
  stoploop = 1;
  char command[maxlen + 15];

  while (stoploop) {
    printf("\nPlease enter a command:\n\t");

    scanf("%s", &command);

    if (strcmp(command, "time") == 0)
      printf("The current time of the clock is %i.\n", clock);

    else if (strcmp(command, "touch") == 0) {
      scanf("%s", &command);
      touch_file(lst, filemap, command, &clock);
    }

    else if (strcmp(command, "timestamp") == 0) {
      scanf("%s", &command);
      timestamp_get(lst, filemap, command);
    }

    else if (strcmp(command, "timestamps") == 0) {
      printf("Current timestamps of list:\n");
      for (i = 0; i < arrsize; i++)
        printf("%s: %i\n", lst->fileNameArray[i], lst->gArr[i]->timestamp);
    }

    else if (strcmp(command, "make") == 0) {
      scanf("%s", &command);
      make_func(lst, filemap, command, &clock);
    }

    else if (strcmp(command, "quit") == 0) {
      printf("Quitting program.\n");
      stoploop = 0;
    }

    else
      printf("Command not recognized.\n");

  }

}
