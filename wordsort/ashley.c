// Ashley Barnaba
// COP3502
// Program 5
// 04/07/22

#include <string.h>

#include <stdio.h>

#include <stdlib.h>

struct info
{
  char word[21];
  int freq;
  int height;
} info;

struct tree_node
{

  struct info *data;
  struct tree_node *left;
  struct tree_node *right;

} tree_node;

// Part 1
//  Creates a single node.
struct tree_node *create_node(char newword[21]);

// Traversal
void inorder(struct tree_node *current_ptr);

// Search and insert.
struct tree_node *insert(struct tree_node *root,
                         struct tree_node *element);
void findNode(struct tree_node *current_ptr, char findword[21]);

// Part 2
// int getCount(struct tree_node * current_ptr);
// struct info * Fill_Array(int index, struct info * array, struct tree_node * root);
// void MergeSort(int array[], int start, int end);
// void Merge(int array[], int start, int middle, int end) ;
// int Is_Sorted(int array[], int length);
void freeTree(struct tree_node *ptr);

int main(void)
{

  struct tree_node *my_root = NULL, *temp_node;
  char word[21];
  long long n; // number of lines for each action
  int a;       // action
  int i = 0;

  scanf("%lld", &n);

  while (i < n)
  {

    scanf("%d", &a);

    // Insert.
    if (a == 1)
    {

      scanf("%s", word);

      // Creat a new node to be able to add it to the tree
      temp_node = create_node(word);

      // Insert the wordue.
      my_root = insert(my_root, temp_node);
    }

    if (a == 2)
    {

      scanf("%s", word);

      // search function
      findNode(my_root, word);
      printf("\n");
    }
    // Inorder traversal.
    // if (a == 5) {

    // inorder(my_root);
    // printf("\n");

    // }
    i++;
  }

  // int count = 0;
  // count = getCount(my_root);
  // printf("%d", count);

  // int index;
  // struct info array[count];
  // Fill_Array(index, array, my_root);

  // printf("error\n");

  // for (index = 0; index < count; index++) {

  // array[index] = (info malloc(sizeof(struct info);
  // Fill_Array(index, &array[index], my_root);

  // printf("%s %d\n", array[index].word, array[index].freq);

  // }

  // MergeSort(array, 0, count);
  // free(array);
  freeTree(my_root);

  return 0;
}

/*
//Used to check binary search tree
// Prints a inorder traversal of the tree rooted at current_ptr.
void inorder(struct tree_node * current_ptr) {

        // Only traverse the node if it's not null.
        if (current_ptr != NULL) {

                inorder(current_ptr -> left); // Go Left.
                printf("%s ", (current_ptr -> data -> word)); // Print the root.
                inorder(current_ptr -> right); // Go Right.

        }
}
*/

// Find where the new node will be inserted in the binary tree
struct tree_node *insert(struct tree_node *root,
                         struct tree_node *element)
{

  // Inserting into an empty tree.
  if (root == NULL)
    return element;

  // Regular case.
  else
  {

    // element should be inserted to the right.
    if (strcmp((element->data->word), (root->data->word)) >= 1)
    {

      element->data->height++;

      // There is a right subtree to insert the node.
      root->right = (insert(root->right, element));
    }

    // element should be inserted to the left.
    if (strcmp((element->data->word), (root->data->word)) <= -1)
    {

      element->data->height++;

      root->left = (insert(root->left, element));
    }

    if (strcmp((element->data->word), (root->data->word)) == 0)
    {

      root->data->freq++;
      return root;
    }
    // Return the root pointer of the updated tree.
    return root;
  }
  return root;
}

// Returns a pointer to a single node storing word.
struct tree_node *create_node(char newword[21])
{

  // Allocate space for the node, set the fields.
  struct tree_node *temp;
  temp = (struct tree_node *)malloc(sizeof(struct tree_node));
  temp->data = (struct info *)malloc(sizeof(info));
  strcpy((temp->data->word), newword);
  temp->data->height = 0;
  temp->data->freq = 1;
  temp->left = NULL;
  temp->right = NULL;

  return temp; // Return a pointer to the created node.
}

// Search for a node already in the tree
void findNode(struct tree_node *current_ptr, char findword[21])
{

  // Oh no! This child isn’t in the treehouse!
  if (current_ptr == NULL)
  {
  }

  // FOUND IT! Score!!!!!
  if (strcmp(findword, (current_ptr->data->word)) == 0)
  {

    printf("%d %d", (current_ptr->data->freq), (current_ptr->data->height));
  }

  // Regular case.
  else
  {

    // element should be inserted to the right.
    if (strcmp(findword, (current_ptr->data->word)) >= 1)
    {

      if (current_ptr->right != NULL)
      {

        findNode(current_ptr->right, findword);
      }
      else

        // Oh no! We can't find the child you are looking for
        printf("-1 -1");
    }

    // element should be inserted to the left.
    if (strcmp(findword, (current_ptr->data->word)) <= -1)
    {

      if (current_ptr->left != NULL)
      {

        findNode(current_ptr->left, findword);
      }
      else
        // Oh no! We can't find the child you are looking for
        printf("-1 -1");
    }
  }
}

/*
int getCount(struct tree_node * current_ptr) {

        // Only traverse the node if it's not null.
        if (current_ptr == NULL) {

                return 0;
        }

        return 1 + getCount(current_ptr -> left) + getCount(current_ptr -> right);

}

struct info * Fill_Array(int index, struct info * array, struct tree_node * root) {

        if (root == NULL)
                return array;

        array[index] = * root -> data;

        if (root -> left != NULL) {

                return Fill_Array(index + 1, array, root -> left);

        }

        if (root -> right != NULL) {

                return Fill_Array(index + 1, array, root -> right);
        }

        return array;
}


int Is_Sorted(int array[], int length) {

    int i;

    // Return false if any adjacent pair is out of order.
    for (i=0; i<length; i++)
        if (values[i] > values[i+1])
            return 0;

    return 1;
}

void MergeSort(int array[], int start, int end) {

    int mid;

    // Check if our sorting range is more than one element.
    if (start < end) {

        mid = (start+end)/2;

        // Sort the first half of the values.
        MergeSort(array, start, mid);

        // Sort the last half of the values.
        MergeSort(array, mid+1, end);

        // Put it all together.
        Merge(array, start, mid+1, end);
    }
}

void Merge(int array[], int start, int middle, int end) {

    //printf("merge %d, %d, %d\n", start, middle, end);

    int *temp, i, length, count1, count2, mc;

    // Allocate the proper amount of space for our auxiliary array.
    length = end - start + 1;
    temp = (int*)calloc(length, sizeof(int));

    // These will be our indexes into our two sorted lists.
    count1 = start;
    count2 = middle;

    // Keeps track of our index into our auxiliary array.
    mc = 0;

    // Here we copy values into our auxiliary array, so long as there are
    // numbers from both lists to copy.
    while ((count1 < middle) || (count2 <= end)) {


        if (count2 > end || (count1 < middle && array[count1] < array[count2])) {
            temp[mc] = array[count1];
            count1++;
            mc++;
        }

        // We copy the next value from list two.
        else {
            temp[mc] = array[count2];
            count2++;
            mc++;
        }
    }

    // Copy back all of our values into the original array.
    for (i=start; i<=end; i++)
        array[i] = temp[i - start];

    // Don't need this space any more!
    free(temp);
}
*/

// Free the binary tree aka burn it down starting with the children!
void freeTree(struct tree_node *ptr)
{
  if (ptr != NULL)
  {
    freeTree(ptr->left->data);
    freeTree(ptr->left);
    freeTree(ptr->right->data);
    freeTree(ptr->right);
    free(ptr);
  }
}