// Hoan Tran
// 04/08/2022
// Solution to COP 3502 Program #4: Word Sort and Search

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define WORD_LENGTH 20

typedef struct nodeData
{
  char word[WORD_LENGTH + 1];
  int freq, depth;
} nodeData;
typedef struct node
{
  nodeData *data;
  struct node *left, *right;
} node;

// Node functions
struct node *newNode(char *word, int depthIn);
struct node *insertNode(struct node *root, char *wordIn, int depthIn);
struct node *findNode(struct node *root, char *word);
int numberNode(struct node *root);
void toInorderArr(struct node *root, struct nodeData **nodeDataArr, int *arrIndex);

// Sort functions
void mergeSortNodeData(struct nodeData **nodeDataArr, int start, int end);
void mergeNodeData(struct nodeData **nodeDataArr, int start, int middle, int end);

// Heper functions
void swapNodeData(struct nodeData *a, struct nodeData *b);
int isSortedNodeData(struct nodeData **nodeDataArr, int low, int high);
void printNodeData(struct nodeData **nodeDataArr, int arrSize);
void freeBST(struct node *root);

int main()
{

  // Declare number of words to insert and number of queries
  int n;
  scanf("%d", &n);

  // Initialize root node
  struct node *root = NULL;

  // Start inserting words and querying
  for (int i = 0; i < n; i++)
  {
    // Declare the action to be performed
    int action;
    scanf("%d", &action);

    // Declare the word to be inserted or queried
    char word[WORD_LENGTH + 1];
    scanf("%s", word);

    if (action == 1)
    {
      // Insert the word or update the existing word frequency
      root = insertNode(root, word, 0);
    }
    else
    {
      // Query the word
      struct node *node = findNode(root, word);

      // If the node is not found
      if (node == NULL)
      {

        // Print not found
        printf("-1 -1\n");
      }
      else
      {
        // Here is the node data
        printf("%d %d\n", node->data->freq, node->data->depth);
      }
    }
  }

  // Init the node data array, its size and capacity
  int arrIdex = 0, arrCap = numberNode(root);
  struct nodeData **nodeDataArr = malloc(sizeof(struct nodeData *) * arrCap);

  // Convert the tree to an array
  toInorderArr(root, nodeDataArr, &arrIdex);

  // Sort the array
  mergeSortNodeData(nodeDataArr, 0, arrIdex - 1);

  // Print the array
  printNodeData(nodeDataArr, arrIdex);

  // Free the array
  free(nodeDataArr);

  // Free the tree
  freeBST(root);

  return 0;
}

/*
 * Creates a new node with the given word and depth
 * @param wordIn the word to be inserted
 * @param depthIn the depth of the node
 * @return the new node
 */
struct node *newNode(char *wordIn, int depthIn)
{

  // Allocate memory for the node data
  struct nodeData *newNodeData = (struct nodeData *)malloc(sizeof(struct nodeData));

  // Copy the word
  strcpy(newNodeData->word, wordIn);

  // Set the frequency to 1
  newNodeData->freq = 1;

  // Set the depth of the node
  newNodeData->depth = depthIn;

  // Allocate memory for the node
  struct node *newNode = (struct node *)malloc(sizeof(struct node));

  // Set the node data
  newNode->data = newNodeData;

  // Set the left and right children to NULL
  newNode->left = newNode->right = NULL;

  return newNode;
}

/*
 * Inserts a node with the given word and depth
 * @param root the root node of the tree
 * @param wordIn the word to be inserted
 * @param depthIn the depth of the node
 * @return the new root node
 */
struct node *insertNode(struct node *root, char *wordIn, int depthIn)
{

  // Return new root if the root is NULL
  if (root == NULL)
    return newNode(wordIn, depthIn);

  // If the word is less than the root word, insert to the left
  if (strcmp(root->data->word, wordIn) > 0)
  {
    root->left = insertNode(root->left, wordIn, depthIn + 1);
  }
  // If the word is greater than the root word, insert to the right
  else if (strcmp(root->data->word, wordIn) < 0)
  {
    root->right = insertNode(root->right, wordIn, depthIn + 1);
  }
  // If the word is equal to the root word, update the frequency
  else
  {
    root->data->freq++;
  }

  // Return the current root
  return root;
}

/*
 * Finds the node with the given word
 * @param root the root node of the tree
 * @param word the word to be queried
 * @return the node with the given word
 */
struct node *findNode(struct node *root, char *word)
{

  // Return NULL if the root is NULL, or if the word is found
  if (root == NULL || strcmp(root->data->word, word) == 0)
    return root;

  // If the word is less than the root word, search the left
  if (strcmp(root->data->word, word) > 0)
    return findNode(root->left, word);

  // If the word is greater than the root word, search the right
  return findNode(root->right, word);
}

/*
 * Counts the number of nodes in the tree
 * @param root the root node of the tree
 * @return the number of nodes in the tree
 */
int numberNode(struct node *root)
{

  // Return 0 if the root is NULL
  if (root == NULL)
    return 0;

  // Count itself and its children
  return 1 + numberNode(root->left) + numberNode(root->right);
}

/*
 * Converts the tree to an inorder array
 * Guarantees that the array is sorted alphabetically
 * @param root the root node of the tree
 * @param nodeDataArr the array to store the nodes
 * @param arrSize the size of the array
 * @param arrCap the capacity of the array
 */
void toInorderArr(struct node *root, struct nodeData **nodeDataArr, int *arrIndex)
{

  // If the root is not NULL
  if (root != NULL)
  {

    // Recursively call the function on the left child
    toInorderArr(root->left, nodeDataArr, arrIndex);

    // Copy the node data pointer to the array
    nodeDataArr[(*arrIndex)++] = root->data;

    // Recursively call the function on the right child
    toInorderArr(root->right, nodeDataArr, arrIndex);
  }
}

/*
 * Prints the node data array
 * @param nodeDataArr the node data array
 * @param size the size of the node data array
 */
void printNodeData(struct nodeData **nodeDataArr, int size)
{

  // Loop through the array
  for (int i = 0; i < size; i++)
  {

    // Print the word and frequency
    printf("%s %d\n", nodeDataArr[i]->word, nodeDataArr[i]->freq);
  }
}

/*
 * Sorts the given array of node data in descending order of frequency
 * The alphabetic order will be maintained if the frequencies are equal
 * @param nodeDataArr the array of node data
 * @param start the start index of the array
 * @param end the end index of the array
 */
void mergeSortNodeData(struct nodeData **nodeDataArr, int start, int end)
{

  int mid;

  // Check if our sorting range is more than one element.
  if (start < end)
  {

    // Find the midpoint of the array
    mid = (start + end) / 2;

    // Sort the first half of the values.
    mergeSortNodeData(nodeDataArr, start, mid);

    // Sort the last half of the values.
    mergeSortNodeData(nodeDataArr, mid + 1, end);

    // Put it all together.
    mergeNodeData(nodeDataArr, start, mid + 1, end);
  }
}

/*
 * Merges the two sorted node data arrays
 * @param nodeDataArr the array of node data
 * @param start the start index of the first array
 * @param mid the end index of the first array
 * @param end the end index of the second array
 */
void mergeNodeData(struct nodeData **nodeDataArr, int start, int middle, int end)
{

  int i, length, count1, count2, mc;
  struct nodeData **tempArr;

  // Allocate the proper amount of space for our auxiliary array.
  length = end - start + 1;
  tempArr = (struct nodeData **)malloc(length * sizeof(struct nodeData *));

  // These will be our indexes into our two sorted lists.
  count1 = start;
  count2 = middle;

  // Keeps track of our index into our auxiliary array.
  mc = 0;

  // Here we copy values into our auxiliary array, so long as there are
  // numbers from both lists to copy.
  while ((count1 < middle) || (count2 <= end))
  {

    // Next value to copy comes from list one - make sure list
    // one isn't exhausted yet. Also make sure we don't access index
    // count2 if we aren't supposed to.
    if (count2 > end || (count1 < middle && nodeDataArr[count1]->freq >= nodeDataArr[count2]->freq))
    {
      tempArr[mc] = nodeDataArr[count1];
      count1++;
      mc++;
    }

    // We copy the next value from list two.
    else
    {
      tempArr[mc] = nodeDataArr[count2];
      count2++;
      mc++;
    }
  }

  // Copy back all of our values into the original array.
  for (i = start; i <= end; i++)
    nodeDataArr[i] = tempArr[i - start];

  // Don't need this space any more!
  free(tempArr);
}

/*
 * Swaps the two given node data
 * @param a the first node data
 * @param b the second node data
 */
void swapNodeData(struct nodeData *a, struct nodeData *b)
{
  struct nodeData temp = *a;
  *a = *b;
  *b = temp;
}

/*
 * Check if the array is sorted
 * @param nodeDataArr the array of node data
 * @param low the low index of the array
 * @param high the high index of the array
 */
int isSortedNodeData(struct nodeData **nodeDataArr, int low, int high)
{

  // Return false if any adjacent pair is out of order.
  for (int i = low; i < high - 1; i++)

    // If the next value is less than the current value, return false.
    if (nodeDataArr[i]->freq < nodeDataArr[i + 1]->freq ||
        // or if the next value is equal to the current value, but the
        // next value's word is alphabetically less than the current value's word
        (nodeDataArr[i]->freq == nodeDataArr[i + 1]->freq && strcmp(nodeDataArr[i]->word, nodeDataArr[i + 1]->word) > 0))
      return 0;

  // If we made it here, the array is sorted.
  return 1;
}

/*
 * Free BST node
 * @param root the root node of the tree
 */
void freeBST(struct node *root)
{

  // If the root is not NULL
  if (root != NULL)
  {

    // Free the children
    freeBST(root->left);
    freeBST(root->right);

    // Free data
    free(root->data);

    // Free the node
    free(root);
  }
}
