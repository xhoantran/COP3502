#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double **initMemo(int size);
void freeMemo(double **mem, int size);
void minTotalDistance(int *perm, int *used, int k, int numCoordinate, int *x, int *y, double *min, double **mem);
void runMinTotalDistance();

int main()
{

  // Get number of test cases.
  int testCase;
  scanf("%d", &testCase);

  // Loop through each case.
  for (int i = 0; i < testCase; i++)

    // Find the minimum total distance for each test case.
    runMinTotalDistance();

  return 0;
}

// Set up valriable and start the brute force.
void runMinTotalDistance()
{
  // Get number of rope(n).
  int numRope;
  scanf("%d", &numRope);

  // Get number of coordinate(2n).
  int numCoordinate = numRope * 2;

  // Used array to keep track of which coordinate is used.
  int *used = calloc(numCoordinate, sizeof(int));
  // Permutation array to keep track of the permutation.
  int *perm = calloc(numCoordinate, sizeof(int));
  // Dynamically allocate memory for x and y array
  // to store coordinate each point.
  int *x = malloc(numCoordinate * sizeof(int));
  int *y = malloc(numCoordinate * sizeof(int));

  // Loop though all coordinates.
  for (int i = 0; i < numCoordinate; i++)
  {
    // Get x and y.
    scanf("%d%d", &x[i], &y[i]);
  }

  // Initialize the min.
  double min = 0;

  // Dynamically allocate memory for the 2d array.
  double **mem = calloc(numCoordinate, sizeof(double *));

  for (int i = 0; i < numCoordinate; i++)
    mem[i] = calloc(numCoordinate, sizeof(double));

  // Start brute force.
  minTotalDistance(perm, used, 0, numCoordinate, x, y, &min, mem);

  // Print the minimum total distance.
  printf("%.3f\n", min);

  // Free dynamically allocated memory.
  free(used);
  free(perm);
  free(x);
  free(y);
  for (int i = 0; i < numCoordinate; i++)
    free(mem[i]);
  free(mem);
}

// Recursive function to find the minimum total distance.
void minTotalDistance(int *perm, int *used, int k, int numCoordinate, int *x, int *y, double *min, double **mem)
{

  // If all coordinates are used, calculate the total distance.
  if (numCoordinate == k)
  {

    // Initialize the total distance.
    double total = 0;

    // Loop through all coordinates.
    for (int i = 0; i < numCoordinate; i += 2)
    {

      // Get the index of pair of coordinates.
      int p1 = perm[i];
      int p2 = perm[i + 1];

      // Variable to store the distance between 2 coordinatge.
      double res;

      // If the distance is already calculated, use the value.
      if (mem[p1][p2] > 10e-3)
      {
        // Get the value from the memo.
        res = mem[p1][p2];
      }
      else
      {
        // If the distance is not calculated, calculate it.
        double dX = x[p1] - x[p2];
        double dY = y[p1] - y[p2];
        res = sqrt(dX * dX + dY * dY);

        // Store the value in the memo.
        mem[p1][p2] = res;
        mem[p2][p1] = res;
      }

      // Add the distance to the total distance.
      total += res;
    }

    // If the total distance is less than the minimum
    // or the minimum is not set.
    if (total < *min || *min < 10e-3)

      // Update the minimum.
      *min = total;
  }
  else
  {

    // Loop through all coordinates.
    for (int i = 0; i < numCoordinate; i++)
    {

      // If the coordinate is not used, skip it.
      if (used[i])
        continue;

      // Set the coordinate to used.
      used[i] = 1;

      // Add the coordinate to the permutation.
      perm[k] = i;

      // Recursive call again.
      minTotalDistance(perm, used, k + 1, numCoordinate, x, y, min, mem);

      // After recursive call done, set the coordinate to unused.
      used[i] = 0;

      /***
       * This is used to skip the redundant permutation.
       * The even index has n-k-1 choice(s) to choose to pair with.
       * Therefore, there is no need to change even index again.
       ***/
      if (k % 2 == 0)
        break;
    }
  }
}
