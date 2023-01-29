#include <stdio.h>

#define PERMS 0
#define DERANGEMENTS 1

#define SIZE 16

// Recursive functions.
void printPerms(int perm[], int used[], int k, int n);
void printDerangements(int perm[], int used[], int k, int n, int *step);
void print(int array[], int n);

// Wrapper functions that run the recursive functions.
void runPerms();
void runDerangements();

int main()
{

  // Run what you want...
  if (DERANGEMENTS)
    runDerangements();

  return 0;
}

// Prints array[0..n-1].
void print(int array[], int n)
{
  int i;
  for (i = 0; i < n; i++)
    printf("%d ", array[i]);
  printf("\n");
}

// Prints out all derangements of 0,1,2,3,4.
void runDerangements()
{
  int i, step = 0, used[SIZE], perm[SIZE];
  for (i = 0; i < SIZE; i++)
    used[i] = 0;
  printDerangements(perm, used, 0, SIZE, &step);
  printf("Step = %d\n", step);
}

// Prints out all derangements in perm with the first k digits fixed.
void printDerangements(int perm[], int used[], int k, int n, int *step)
{

  if (n == k)
  {
    // print(perm, n);
    *step += 1;
  }
  else
  {

    for (int i = 0; i < n; i++)
    {
      if (used[i])
        continue;
      used[i] = 1;
      perm[k] = i;
      printDerangements(perm, used, k + 1, n, step);
      used[i] = 0;
      if (k % 2 == 0)
        break;
    }
  }
}
