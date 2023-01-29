// Hoan Tran
// 03/21/2022
// Solution to COP 3502 Program #4: Scholarly Reader (Sorting)

#include <stdio.h>
#include <stdlib.h>

void quicksort(int* numbers, int low, int high);
int partition(int* vals, int low, int high);
void swap(int *a, int *b);
int is_sorted(int values[], int low, int high);

int main(void) {

  // Get the number of book and max pages
  int numBooks;
  long long maxPages;
  scanf("%d %lld", &numBooks, &maxPages);

  // Dynamic allocate number of pages array
  int* numPageArray = (int *)malloc(numBooks*sizeof(int));

  // Read the array
  for(int i=0; i<numBooks; i++) scanf("%d", &numPageArray[i]);

  // Sort the array
  quicksort(numPageArray, 0, numBooks-1);

  // Number of books can be read start at -1
  // This number will be increment as we read a book
  // First book: maxBooks = 0
  int maxBooks = -1;
  
  // While page limit is not reached
  while (maxPages >= 0) {

    // Count the book just read
    maxBooks++;

    // Decrement the page limit using the new book in sorted array
    maxPages -= numPageArray[maxBooks];
  }

  // Print the number of books
  printf("%d\n", maxBooks);

  // Free the memory
  free(numPageArray);

  return 0;
}

int is_sorted(int values[], int low, int high) {

    // Return false if any adjacent pair is out of order.
    for (int i=low; i<high-1; i++)
        if (values[i] > values[i+1])
            return 0;

    return 1;
}

void quicksort(int* numbers, int low, int high) {

    // Hey, it's sorted, so we're done!
    if (is_sorted(numbers, low, high)) return;

    // Three steps: partition, sort left side, sort right side!
    int split = partition(numbers, low, high);
    quicksort(numbers, low, split-1);
    quicksort(numbers, split+1, high);
}

int partition(int* vals, int low, int high) {

  int temp;
  int i, lowpos;

  // Pick a random partition element and swap it into index low.
  i = low + rand()%(high-low+1);
  swap(&vals[low], &vals[i]);

	// Store the index of the partition element.
	lowpos = low;

	// Update our low pointer.
	low++;

	// Run the partition so long as the low and high counters don't cross.
	while (low <= high) {

		// Move the low pointer until we find a value too large for this side.
		while (low <= high && vals[low] <= vals[lowpos]) low++;

		// Move the high pointer until we find a value too small for this side.
		while (high >= low && vals[high] > vals[lowpos]) high--;

		// Now that we've identified two values on the wrong side, swap them.
		if (low < high)
      swap(&vals[low], &vals[high]);
	}

	// Swap the partition element into it's correct location.
	swap(&vals[lowpos], &vals[high]);

	return high; // Return the index of the partition element.
}

// Swaps the values pointed to by a and b.
void swap(int *a, int *b) {
     int temp = *a;
     *a = *b;
     *b = temp;
}
