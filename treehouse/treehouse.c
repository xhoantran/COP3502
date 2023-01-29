#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct coordinate
{
  int x;
  int y;
} coordinate;

coordinate *readCoordinate(int numRope);
double minimumSumDistance(coordinate *listCoordinate, int numCoordinate, int *perm, int *used, int k, double *min);
double calculateDistance(coordinate *listCoordinate, int x, int y);
double calculateTotalDistance(coordinate *listCoordinate, int numCoordinate, int *perm);

int main()
{

  int testCase;
  scanf("%d", &testCase);

  for (int i = 0; i < testCase; i++)
  {

    clock_t t1, t2;
    t1 = clock();

    int numRope;
    scanf("%d", &numRope);

    coordinate *listCoordinate = readCoordinate(numRope);

    int *used = calloc(numRope * 2, sizeof(int));
    int *perm = calloc(numRope * 2, sizeof(int));

    double min = 0;

    minimumSumDistance(listCoordinate, numRope * 2, perm, used, 0, &min);

    printf("%.3f\n", min);

    t2 = clock();
    printf("For n=%d, time to run: %.6fs\n", numRope, (double)(t2 - t1) / CLOCKS_PER_SEC);
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

double minimumSumDistance(coordinate *listCoordinate, int numCoordinate, int *perm, int *used, int k, double *min)
{

  if (numCoordinate == k)
  {
    double result = calculateTotalDistance(listCoordinate, numCoordinate, perm);
    if (result < *min || *min < 10e-3)
      *min = result;
  }
  else
  {

    for (int i = 0; i < numCoordinate; i++)
    {
      if (used[i])
        continue;
      used[i] = 1;
      perm[k] = i;
      minimumSumDistance(listCoordinate, numCoordinate, perm, used, k + 1, min);
      used[i] = 0;
      if (k % 2 == 0)
        break;
    }
  }
}

double calculateTotalDistance(coordinate *listCoordinate, int numCoordinate, int *perm)
{

  double total = 0;

  for (int i = 0; i < numCoordinate; i += 2)
  {
    total += calculateDistance(listCoordinate, perm[i], perm[i + 1]);
  }

  return total;
}

double calculateDistance(coordinate *listCoordinate, int x, int y)
{

  double dX = listCoordinate[x].x - listCoordinate[y].x;
  double dY = listCoordinate[x].y - listCoordinate[y].y;

  double distance = sqrt(dX * dX + dY * dY);

  return distance;
}
