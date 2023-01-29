#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Using coordinate struct.
typedef struct coordinate
{
  int x;
  int y;
} coordinate;

coordinate *readCoordinate(int numRope);
void minimumSumDistance(int *perm, int *used, int k, int numCoordinate);
double calculateTotalDistance(int *perm, int numCoordinate);
double calculateDistance(int x1, int y1, int x2, int y2);

double min;
coordinate *listCoordinate;

int main()
{

  // Get number of test cases.
  int testCase;
  scanf("%d", &testCase);

  // Loop through each case.
  for (int i = 0; i < testCase; i++)
  {

    clock_t t1, t2;
    t1 = clock();

    // Get number of rope(n).
    int numRope;
    scanf("%d", &numRope);

    // Read all the coordinate.
    listCoordinate = readCoordinate(numRope);

    int *used = calloc(numRope * 2, sizeof(int));
    int *perm = calloc(numRope * 2, sizeof(int));
    min = 0;

    minimumSumDistance(perm, used, 0, numRope * 2);

    printf("%.3f\n", min);

    t2 = clock();
    printf("For n=%d, time to run: %.6fs\n", numRope, (double)(t2 - t1) / CLOCKS_PER_SEC);

    free(listCoordinate);
    free(used);
    free(perm);
  }

  return 0;
}

coordinate *readCoordinate(int numRope)
{

  coordinate *listCoordinate = malloc(sizeof(coordinate) * 2 * numRope);

  for (int i = 0; i < numRope * 2; i++)
  {
    scanf("%d%d", &listCoordinate[i].x, &listCoordinate[i].y);
  }

  return listCoordinate;
}

void minimumSumDistance(int *perm, int *used, int k, int numCoordinate)
{

  if (numCoordinate == k)
  {
    double result = calculateTotalDistance(perm, numCoordinate);
    if (result < min || min < 10e-3)
      min = result;
  }
  else
  {

    for (int i = 0; i < numCoordinate; i++)
    {
      if (used[i])
        continue;
      used[i] = 1;
      perm[k] = i;
      minimumSumDistance(perm, used, k + 1, numCoordinate);
      used[i] = 0;
      if (k % 2 == 0)
        break;
    }
  }
}

double calculateTotalDistance(int *perm, int numCoordinate)
{

  double total = 0;

  for (int i = 0; i < numCoordinate; i += 2)
  {
    int p1 = perm[i];
    int p2 = perm[i + 1];
    total += calculateDistance(listCoordinate[p1].x, listCoordinate[p1].y, listCoordinate[p2].x, listCoordinate[p2].y);
  }

  return total;
}

double calculateDistance(int x1, int y1, int x2, int y2)
{
  double dX = x1 - x2;
  double dY = y1 - y2;

  return sqrt(dX * dX + dY * dY);
}
