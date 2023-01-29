#include <stdio.h>
#include <stdlib.h>

/*
WHAT U NEED TO DO:
PASS INTEGERS INTO FUNCTION THAT DELETES AND SKIPS
PLUG IN REMAINING VALUES INTO AN ARRAY
*/

// Struct used to form a linked list of integers.
struct ll
{
  int data;
  int flag;
  struct ll *next;
};

struct group
{
  int groupNum;
  struct ll *peopleList;
  struct group *next;
} group;

void print(struct ll *front, int numG);
void freelist(struct ll *front);
struct ll *insert_front(struct ll *front, int num);
struct ll *insert_inorder(struct ll *front, int num);
struct ll *skipping(struct ll *inorderlist, int p, int s, int t, int groupNum, int G);

int main()
{

  struct group *listOfArtists = NULL;
  struct group *tmp = NULL;
  int p, t, G, n, s, data;
  struct ll *randlist = NULL;
  struct ll *inorderlist = NULL;

  // Collecting input from the user.
  printf("give the number of input cases:\n");
  scanf("%d", &n);

  printf("now gimme the number of groups:\n");
  for (int i = 0; i < n; i++)
  {
    scanf("%d", &G);

    printf("gimme the 3 nums;\n");
    for (int j = 0; j < G; j++)
    {
      scanf("%d %d %d", &p, &s, &t);
      for (int i = 0; i < p; i++)
      {
        inorderlist = insert_inorder(inorderlist, i + 1);
      }

      skipping(inorderlist, p, s, t, j + 1, G);

      print(inorderlist, j);
      free(inorderlist);
      // freelist(inorderlist);
    }
  }

  return 0;
}

// Pre-condition: front points to the head of a circular linked list structure.
// Post-condition: each value in the list will be printed, in order.
void print(struct ll *front, int numG)
{

  struct ll *iter = front;

  // If the list is empty just print a blank line.
  if (front == NULL)
  {
    printf("\n");
    return;
  }

  printf("Group %d:\n", numG + 1);
  // Now, iterate through each element, printing.
  do
  {
    printf("%d ", iter->data);
    iter = iter->next;

    // Here we check if the next element is back to the front, if so, we stop!
  } while (iter != front);

  // Go to the next line.
  printf("\n");
}

// Pre-condition: front points to the head of a circular linked list structure.
// Post-condition: a new node storing num will be inserted to the front of
//                 this list and a pointer to this node will be returned.
struct ll *insert_front(struct ll *front, int num)
{

  struct ll *temp, *iter;

  // Create temp node and store the value.
  temp = (struct ll *)malloc(sizeof(struct ll));
  temp->data = num;

  // We are adding to an empty list.
  if (front == NULL)
  {

    // In a circular list, this means link to yourself...
    temp->next = temp;
  }

  // Here we add to an existing list.
  else
  {

    // Link our new node to the first element in the old list.
    temp->next = front;

    // Iterate to the last element in the old list.
    iter = front;
    while (iter->next != front)
      iter = iter->next;

    // Now, this needs to relink to our new first element, temp.
    iter->next = temp;
  }

  // Return the new front of the list.
  return temp;
}

struct ll *insert_inorder(struct ll *front, int num)
{

  struct ll *temp;
  struct ll *iter;
  struct ll *savelink;

  // Take care of the case where we insert into the front of the list.
  if (front == NULL || num < front->data)
  {
    return insert_front(front, num);
  }

  // Create the new node.
  temp = (struct ll *)malloc(sizeof(struct ll));
  temp->data = num;
  temp->next = NULL;

  // Start iter at the front of the linked list.
  iter = front;

  // Use iter to iterate to the right spot to insert temp.
  // We stop if the node after iter is pointing back to the front of the list.
  while (iter->next != front && iter->next->data < num)
    iter = iter->next;

  // Save the node to patch to.
  savelink = iter->next;

  // Insert temp.
  iter->next = temp;
  temp->next = savelink;

  // Return a pointer to the front of the list.
  return front;
}

struct ll *skipping(struct ll *front, int p, int s, int t, int groupNum, int G)
{
  struct ll *temp = front;
  struct ll *temp1 = front;
  struct ll *prevNode = front;    // reference to the previous node
  struct ll *currentNode = front; // reference to the root node
  int numofskips = 0;
  int t2 = 0;

  // RETURN AN INT = THE DELETED NODE;

  t2 += t;
  /*
    while(numofskips < ll->skips)
  {
    numofskips++;
    temp= temp->next;
    temp1 = temp1->next->next;
    delete "temp"

  }
  */
  /*
  char** array = (char**)malloc(sizeof(char*)*G);
  for(int i = 0; i<G; i++)
  {
    array[i] = (char*)malloc(sizeof(char)*t);
  }
   */

  for (int i = 0; i < t; i++)
  {
    int count = 1;
    temp = temp->next;

    if (i == s + 1)
    {
      prevNode->next = currentNode->next;
      free(currentNode);
      i = 0;
    }
    prevNode = currentNode;
    currentNode = currentNode->next;

    for (int k = 0; k < p; k++)
    {
      temp1 = temp1->next;
      count++;
    }

    if (count == t)
    {
      for (int j = 0; j < t; j++)
      {
        array[groupNum][j] = temp1->next;
        temp1 = temp1->next;
      }
    }
  }
}

void freelist(struct ll *front)
{

  // Length = 0, 1 cases.
  if (front == NULL)
    return;
  if (front == front->next)
  {
    free(front);
    return;
  }

  // Mark the front.
  struct ll *tmp = front;

  // Use tmp to delete nodes, one by one.
  tmp = tmp->next;
  while (tmp != front)
  {
    struct ll *del = tmp;
    tmp = tmp->next;
    free(del);
  }

  // Free at last!
  free(tmp);
}