// Hoan Tran
// 04/11/2022
// Solution to COP 3502 Program #5: Hairsalon Priority Queue

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NAME_SIZE 20
#define HEAP_INIT_CAP 1
#define NUM_STYLIST 10
#define MAX_TIME_ARRIVE 1000000000
#define MAX_CUSTOMER 100000

typedef struct customer
{
  int time_arrive;
  char name[NAME_SIZE + 1];
  char preferred_stylist[NAME_SIZE + 1];
  int loyalty_point;
  int time_haircut;
  int time_end;
} customer;

typedef struct stylistHeap
{
  customer **heaparray;
  int size;
  int capacity;
  char name[NAME_SIZE + 1];
  int time_end;
} heap;

void init(heap *heap_ptr);
void freeHeap(heap *heap_ptr);

void percolateUp(heap *heap_ptr, int index);
void percolateDown(heap *heap_ptr, int index);

int insert(heap *heap_ptr, customer *item);
int findMinSize(heap heap_arr[], int numStylist);
int findAndInsert(heap heap_arr[], int numStylist, customer *item);
customer *deleteMin(heap *heap_ptr);
customer *deleteFront(heap *heap_ptr);

int compare(customer *c1, customer *c2, heap *heap_ptr);
void swap(customer **ptrA, customer **ptrB);
int max(int a, int b);
customer *createCustomer(int time_arrive, char *name, char *preferred_stylist, int loyalty_point, int time_haircut);
void processCustomers(heap heap_arr[], int numStylist, int current_time);

void printHeap(heap *h);

int main()
{

  // Declare number of customers and number of stylists
  int num_customer, num_stylist;

  // Read in the number of customers and stylists
  scanf("%d %d", &num_customer, &num_stylist);

  // Statically allocate the array of stylist heap structures
  heap salon_lines[NUM_STYLIST];

  // Read in the names of the stylists
  for (int i = 0; i < num_stylist; i++)
  {
    init(&salon_lines[i]);
    scanf("%s", salon_lines[i].name);
  }

  // Declare the variables to store customer information
  int time_arrive, loyalty_point, time_haircut;
  char name[NAME_SIZE + 1], preferred_stylist[NAME_SIZE + 1];

  // Read in the customer information
  for (int i = 0; i < num_customer; i++)
  {

    // Read in the customer information
    scanf("%d %s %s %d %d", &time_arrive, name, preferred_stylist, &loyalty_point, &time_haircut);

    // Create a customer struct
    customer *cust = createCustomer(time_arrive, name, preferred_stylist, loyalty_point, time_haircut);

    // Variable to store the status of insertion
    int insert_result = 0;

    // If the customer has a preferred stylist
    if (strcmp(preferred_stylist, "NONE") != 0)
    {
      // Find the preferred stylist and insert the customer
      insert_result = findAndInsert(salon_lines, num_stylist, cust);
    }

    // If insertion failed or the customer does not have a preferred stylist
    if (insert_result == 0)
    {
      // Insert the customer into the heap with the minimum size
      insert(&salon_lines[findMinSize(salon_lines, num_stylist)], cust);
    }

    processCustomers(salon_lines, num_stylist, time_arrive);

    printf("----------------------------------------------------\n");
    printf("After registering customer %s arriving at %d\n", name, time_arrive);
    for (int j = 0; j < num_stylist; j++)
    {
      printf("%s: ", salon_lines[j].name);
      printHeap(&salon_lines[j]);
    }
    printf("----------------------------------------------------\n");
  }

  processCustomers(salon_lines, num_stylist, MAX_TIME_ARRIVE);

  return 0;
}

/*
 * Initialize the heap.
 * @param heap_ptr The heap to initialize.
 */
void init(heap *heap_ptr)
{
  // Initialize the heap
  heap_ptr->size = 0;
  heap_ptr->capacity = HEAP_INIT_CAP;
  heap_ptr->time_end = 0;
  heap_ptr->heaparray = malloc(sizeof(customer *) * (HEAP_INIT_CAP + 1));
  if (heap_ptr->heaparray != NULL)
  {
    heap_ptr->heaparray[0] = NULL;
  }
}

// Frees the memory dynamically allocated in the heap pointed to by heap_ptr.
// void freeHeap(heap* heap_ptr) {
//     free(heap_ptr->heaparray);
// }

void percolateUp(heap *heap_ptr, int index)
{

  // At root, can't go up any more.
  if (index == 1)
    return;

  int pIndex = index / 2;

  // Node and parent are out of order, swap and recurse.
  if (compare(heap_ptr->heaparray[index], heap_ptr->heaparray[pIndex], heap_ptr) < 0)
  {
    swap(&heap_ptr->heaparray[index], &heap_ptr->heaparray[pIndex]);
    percolateUp(heap_ptr, pIndex);
  }
}

void percolateDown(heap *heap_ptr, int index)
{

  // Leaf node.
  if (2 * index > heap_ptr->size)
    return;

  // You only have a left child.
  if (2 * index == heap_ptr->size)
  {

    // Last swap.
    if (compare(heap_ptr->heaparray[2 * index], heap_ptr->heaparray[index], heap_ptr) < 0)
      swap(&heap_ptr->heaparray[2 * index], &heap_ptr->heaparray[index]);
    return;
  }

  // Figure out whether or not the left or right child is better.
  int swapIndex = compare(heap_ptr->heaparray[2 * index], heap_ptr->heaparray[2 * index + 1], heap_ptr) < 0 ? 2 * index : 2 * index + 1;

  // Node and parent are out of order, swap and recurse.
  if (compare(heap_ptr->heaparray[swapIndex], heap_ptr->heaparray[index], heap_ptr) < 0)
  {
    swap(&heap_ptr->heaparray[swapIndex], &heap_ptr->heaparray[index]);
    percolateDown(heap_ptr, swapIndex);
  }
}

/*
 * Inserts the item into the heap.
 * @param heap_ptr The heap to insert into.
 * @param item The item to insert.
 * @return 1 if the item was inserted, 0 if the heap is fail to insert.
 */
int insert(heap *heap_ptr, customer *item)
{

  // If the front of the heap is NULL, then we can just insert the item.
  if (heap_ptr->heaparray[0] == NULL)
  {
    heap_ptr->heaparray[0] = item;
    heap_ptr->time_end = max(heap_ptr->time_end, item->time_arrive) + item->time_haircut;
    return 1;
  }

  int *temp;
  int *throwaway;
  int i;

  // Our array is full, we need to allocate some new space!
  if (heap_ptr->size == heap_ptr->capacity)
  {

    // Allocate new space for an array.
    heap_ptr->heaparray = (customer **)realloc(heap_ptr->heaparray, sizeof(customer *) * (2 * heap_ptr->capacity + 1));

    // Realloc failed so we quit.
    if (heap_ptr->heaparray == NULL)
      return 0;

    // Double the capacity.
    heap_ptr->capacity *= 2;
  }

  // Adjust all the necessary components of heap_ptr, and then move the inserted
  // item into its appropriate location.
  heap_ptr->size++;
  heap_ptr->heaparray[heap_ptr->size] = item;
  percolateUp(heap_ptr, heap_ptr->size);

  return 1;
}

/*
 * Finds the heap has minimum size in the heap array.
 * If there are multiple heaps with the same minimum size,
 * the first one is returned.
 *
 * @param heap_arr The array of heaps.
 * @param numStylist The number of stylists.
 * @return The heap index with the minimum size.
 */
int findMinSize(heap heap_arr[], int numStylist)
{
  // Init the min size to the largest possible size.
  int minIndex, minSize = MAX_CUSTOMER;

  // Loop through the heap array.
  for (int i = 0; i < numStylist; i++)
  {

    // If the first stylist is free, return it.
    if (heap_arr[i].heaparray[0] == NULL)
      return i;

    // If the current heap's size is smaller than the min size,
    if (heap_arr[i].size < minSize)
    {

      // Set the min size to the current heap's size.
      minSize = heap_arr[i].size;

      // Set the minIndex to the current heap's index.
      minIndex = i;
    }
  }

  return minIndex;
}

/*
 * Finds the stylist with the name given and inserts the customer into the
 * heap
 * @param heap_ptr The array of stylist heaps
 * @param numStylist The number of stylists
 * @param item The customer to be inserted
 * @return 1 if the customer was inserted, 0 otherwise
 */
int findAndInsert(heap heap_arr[], int numStylist, customer *item)
{

  // Find the appropriate stylist to insert into.
  for (int i = 0; i < numStylist; i++)
  {

    // If the stylist name matches.
    if (strcmp(heap_arr[i].name, item->preferred_stylist) == 0)
    {
      // Insert the customer into the heap.
      insert(&heap_arr[i], item);

      // Return 1 to indicate success.
      return 1;
    }
  }

  // Return 0 to indicate failure.
  return 0;
}

/*
 * Deletes the customer with the highest
 * priority from the heap pointed to by heap_ptr
 * @param heap_ptr The heap to delete from.
 * @return The minimum item in the heap.
 */
customer *deleteMin(heap *heap_ptr)
{

  // This is the item to delete.
  customer *retval = heap_ptr->heaparray[1];

  // Copy last item into first slot.
  heap_ptr->heaparray[1] = heap_ptr->heaparray[heap_ptr->size];

  // Now our heap has one fewer item.
  heap_ptr->size--;

  // Now, we just have to percolate this item down.
  percolateDown(heap_ptr, 1);

  // This is the pointer I need to return.
  return retval;
}

/*
 * Deletes the current customer from the heap.
 * @param heap_ptr The heap to delete from.
 * @return The item at index 0 of the heap.
 */
customer *deleteFront(heap *heap_ptr)
{

  // This is the item to delete.
  customer *retval = heap_ptr->heaparray[0];

  // Update the time_end of customer.
  retval->time_end = heap_ptr->time_end;

  if (heap_ptr->size > 0)
  {
    // Delete the first item in heap.
    heap_ptr->heaparray[0] = deleteMin(heap_ptr);

    // Update the time_end.
    heap_ptr->time_end = max(heap_ptr->time_end, heap_ptr->heaparray[0]->time_arrive) + heap_ptr->heaparray[0]->time_haircut;
  }
  else
  {
    heap_ptr->heaparray[0] = NULL;
  }

  // Return the deleted item.
  return retval;
}

/*
 * Compares two customers based on their priority.
 * @param c1 The first customer.
 * @param c2 The second customer.
 * @return value < 0 if c1 has a higher priority than c2
 * and > 0 if c2 has a higher priority than c1.
 */
int compare(customer *c1, customer *c2, heap *heap_ptr)
{

  // The customer comes at and before the time stylist
  // doing the hair cut is done have a higher priority.
  if ((c1->time_arrive <= heap_ptr->time_end) ^ ((c1->time_arrive <= heap_ptr->time_end)))
  {
    // If c1 has this stylist as preferred stylist
    if (c1->time_arrive <= heap_ptr->time_end)
      return -1;
    return 1;
  }

  // If they have different loyalty points
  if (c1->loyalty_point != c2->loyalty_point)
    // Return the difference.
    return c2->loyalty_point - c1->loyalty_point;

  // If one of them has this stylist as preferred stylist
  // and the other doesn't.
  if ((strcmp(c1->preferred_stylist, heap_ptr->name) == 0) ^ (strcmp(c2->preferred_stylist, heap_ptr->name) == 0))
  {

    // If c1 has this stylist as preferred stylist
    if (strcmp(c1->preferred_stylist, heap_ptr->name) == 0)
      return -1;
    return 1;
  }

  // Compare the names.
  return strcmp(c1->name, c2->name);
}

/*
 * Swaps the contents of two pointers.
 * @param a The first pointer to swap.
 * @param b The second pointer to swap.
 */
void swap(customer **ptrA, customer **ptrB)
{
  customer *tmp = *ptrA;
  *ptrA = *ptrB;
  *ptrB = tmp;
}

/*
 * Return the maximum of two integers.
 * @param a The first integer.
 * @param b The second integer.
 * @return The maximum of a and b.
 */
int max(int a, int b)
{
  return a > b ? a : b;
}

/*
 * Creates a customer with the given parameters.
 *
 * @param time_arrive The time the customer arrives.
 * @param name The name of the customer.
 * @param loyalty_point The number of loyalty_point the customer has.
 * @param time_haircut The time the customer will be given a haircut.
 *
 * @return A pointer to the customer created.
 */
customer *createCustomer(int time_arrive, char *name, char *preferred_stylist, int loyalty_point, int time_haircut)
{
  customer *cPtr = (customer *)malloc(sizeof(customer));
  cPtr->time_arrive = time_arrive;
  strcpy(cPtr->name, name);
  strcpy(cPtr->preferred_stylist, preferred_stylist);
  cPtr->loyalty_point = loyalty_point;
  cPtr->time_haircut = time_haircut;
  return cPtr;
}

/*
 * Processes the customers in the queue based on the current time.
 *
 * @param heap_arr The array of stylist heaps.
 * @param current_time The current time.
 *
 */
void processCustomers(heap heap_arr[], int numStylist, int current_time)
{
  // Loop through the queue.
  for (int i = 0; i < numStylist; i++)
  {

    // If there are customers in the queue and the stylist is free.
    while (heap_arr[i].heaparray[0] != NULL && heap_arr[i].time_end <= current_time)
    {

      // Delete the customer from the queue.
      customer *cus = deleteFront(&heap_arr[i]);

      // Print out the customer's information.
      printf("%s %d %d %s\n", cus->name, cus->time_end, cus->loyalty_point + cus->time_haircut / 10, heap_arr[i].name);
    }
  }
}

void printHeap(heap *h)
{
  int i;

  for (i = 0; i <= h->size; i++)
    if (h->heaparray[i] != NULL)
      printf("%s ", h->heaparray[i]->name);
  printf("\n");
}
