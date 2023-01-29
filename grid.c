// Arup Guha
// 6/26/07
// Example of how to implement a queue with an array.
// 2/14/2022 - used queue to solve grid problem on Kattis
// Grid uses a BFS on a grid.

#include <stdio.h>
#include <stdlib.h>

#define EMPTY -1
#define INIT_SIZE 10
#define MAX_SIZE 501

// Ways we can move.
#define NUMDIR 4
const int DX[4] = {-1, 0, 0, 1};
const int DY[4] = {0, -1, 1, 0};

// Stores our queue.
struct queue
{
  int *elements;
  int front;
  int numElements;
  int queueSize;
};

void init(struct queue *qPtr);
int enqueue(struct queue *qPtr, int val);
int dequeue(struct queue *qPtr);
int empty(struct queue *qPtr);
int front(struct queue *qPtr);

int bfs(int **grid, int row, int col);
int inbounds(int x, int y, int row, int col);
void freeArray(int **array, int numR);

int main()
{

  // Read in size of grid.
  int row, col;
  scanf("%d%d", &row, &col);

  // Allocates space.
  int **grid = malloc(sizeof(int *) * row);
  for (int i = 0; i < row; i++)
    grid[i] = malloc(sizeof(int) * col);

  // Read in grid.
  for (int i = 0; i < row; i++)
  {

    // Get string.
    char str[MAX_SIZE];
    scanf("%s", str);

    for (int j = 0; j < col; j++)
      grid[i][j] = str[j] - '0';
  }

  // Print and free memory.
  printf("%d\n", bfs(grid, row, col));
  freeArray(grid, row);
  return 0;
}

int bfs(int **grid, int row, int col)
{

  // Set up queue for BFS.
  struct queue myQueue;
  init(&myQueue);

  // Allocate space for distance array and fill with -1.
  int **dist = malloc(sizeof(int *) * row);
  for (int i = 0; i < row; i++)
  {
    dist[i] = malloc(sizeof(int) * col);
    for (int j = 0; j < col; j++)
      dist[i][j] = -1;
  }

  // Mark that I have been to the beginning with distance 0.
  enqueue(&myQueue, 0);
  dist[0][0] = 0;

  // Go until queue is empty.
  while (!empty(&myQueue))
  {

    // Dequeue current item.
    int cur = dequeue(&myQueue);

    // Extract out x,y location.
    int curX = cur / col;
    int curY = cur % col;

    printf("For cur %d\n: %d %d", curX, curY);

    // Made it to end, return!
    if (curX == row - 1 && curY == col - 1)
    {

      // Store return value.
      int retval = dist[curX][curY];

      // Free memory for distance array.
      freeArray(dist, row);

      // Gets rid of this dynamically allocated memory.
      while (!empty(&myQueue))
        dequeue(&myQueue);

      // Now we can return.
      return retval;
    }

    // Try jumping in all four directions.
    for (int i = 0; i < NUMDIR; i++)
    {

      // Where I can go to.
      int newX = curX + DX[i] * grid[curX][curY];
      int newY = curY + DY[i] * grid[curX][curY];

      // Can't go to this square.
      if (!inbounds(newX, newY, row, col))
        continue;

      // We've been here before.
      if (dist[newX][newY] != -1)
        continue;

      // Add to queue, store distance.
      int newC = newX * col + newY;
      printf("For cur %d\n: %d %d", newC, newX, newY);
      dist[newX][newY] = dist[curX][curY] + 1;
      enqueue(&myQueue, newC);
    }
  } // end while

  // Never made it - free memory, return -1.
  freeArray(dist, row);
  return -1;
}

// Returns true iff (x,y) is inbounds on an row by col grid.
int inbounds(int x, int y, int row, int col)
{
  return x >= 0 && x < row && y >= 0 && y < col;
}

// Frees the dynamically allocated memory pointed to by array.
void freeArray(int **array, int numR)
{
  for (int i = 0; i < numR; i++)
    free(array[i]);
  free(array);
}

// Pre-condition: qPtr points to a valid struct queue.
// Post-condition: The struct that qPtr points to will be set up to represent an
//                 empty queue.
void init(struct queue *qPtr)
{

  // The front index is 0, as is the number of elements.
  qPtr->elements = (int *)malloc(sizeof(int) * INIT_SIZE);
  qPtr->front = 0;
  qPtr->numElements = 0;
  qPtr->queueSize = INIT_SIZE;
}

// Pre-condition: qPtr points to a valid struct queue and val is the value to
//                enqueue into the queue pointed to by qPtr.
// Post-condition: If the operation is successful, 1 will be returned, otherwise
//                 no change will be made to the queue and 0 will be returned.

// Note: Right now, I don't know how to detect that the realloc failed, so 0
//       does not get returned.

int enqueue(struct queue *qPtr, int val)
{

  int i;

  // Regular case where our queue isn't full.
  if (qPtr->numElements != qPtr->queueSize)
  {

    // Enqueue the current element. Note the use of mod for the wraparound
    // case. Edit the number of elements.
    qPtr->elements[(qPtr->front + qPtr->numElements) % qPtr->queueSize] = val;
    (qPtr->numElements)++;

    // Signifies a successful operation.
    return 1;
  }

  // Case where the queue is full, we must find more space before we enqueue.
  else
  {

    // Allocate more space!
    qPtr->elements = realloc(qPtr->elements, (qPtr->queueSize) * sizeof(int) * 2);

    // Copy all of the items that are stored "before" front and copy them
    // into their new correct locations.
    for (i = 0; i <= qPtr->front - 1; i++)
      qPtr->elements[i + qPtr->queueSize] = qPtr->elements[i];

    // Enqueue the new item, now that there is space. We are guaranteed that
    // no wraparound is necessary here.
    qPtr->elements[i + qPtr->queueSize] = val;

    // More bookkeeping: The size of the queue as doubled and the number of
    // elements has increased by one.
    (qPtr->queueSize) *= 2;
    (qPtr->numElements)++;

    return 1;
  }
}

// Pre-condition: qPtr points to a valid struct queue.
// Post-condition: If the queue pointed to by qPtr is non-empty, then the value
//                 at the front of the queue is deleted from the queue and
//                 returned. Otherwise, -1 is returned to signify that the queue
//                 was already empty when the dequeue was attempted.
int dequeue(struct queue *qPtr)
{

  int retval;

  // Empty case.
  if (qPtr->numElements == 0)
    return EMPTY;

  // Store the value that should be returned.
  retval = qPtr->elements[qPtr->front];

  // Adjust the index to the front of the queue accordingly.
  qPtr->front = (qPtr->front + 1) % qPtr->queueSize;

  // We have one fewer element in the queue.
  (qPtr->numElements)--;

  // Return the dequeued element.
  return retval;
}

// Pre-condition: qPtr points to a valid struct queue.
// Post-condition: returns true iff the queue pointed to by pPtr is empty.
int empty(struct queue *qPtr)
{
  return qPtr->numElements == 0;
}

// Pre-condition: qPtr points to a valid struct queue.
// Post-condition: if the queue pointed to by qPtr is non-empty, the value
//                 stored at the front of the queue is returned. Otherwise,
//                 -1 is returned to signify that this queue is empty.
int front(struct queue *qPtr)
{
  if (qPtr->numElements != 0)
    return qPtr->elements[qPtr->front];
  else
    return EMPTY;
}