// Arup Guha
// 3/25/04, with code added to do delete on 3/30/04
// COP 3502 Lecture: Some Binary Tree Functions
// Edited on 7/6/2020 to add a recursive delete function

#include <stdio.h>
#include <stdlib.h>

#define QUIT 8

struct tree_node
{
  int data;
  struct tree_node *left;
  struct tree_node *right;
};

// Creates a single node.
struct tree_node *create_node(int val);

// Traversals.
void preorder(struct tree_node *current_ptr);
void inorder(struct tree_node *current_ptr);

// Search and insert.
int find(struct tree_node *current_ptr, int val);
struct tree_node *insert(struct tree_node *root,
                         struct tree_node *element);

// Structural functions to aid delete.
int isLeaf(struct tree_node *node);
int hasOnlyLeftChild(struct tree_node *node);
int hasOnlyRightChild(struct tree_node *node);
struct tree_node *findNode(struct tree_node *current_ptr, int value);

// Other functions to aid delete.
struct tree_node *parent(struct tree_node *root, struct tree_node *node);
struct tree_node *minVal(struct tree_node *root);
struct tree_node *maxVal(struct tree_node *root);

// Delete functions.
struct tree_node *delete (struct tree_node *root, int value);
struct tree_node *delRec(struct tree_node *root, int delval);

// Random questions from past foundation exams.
struct tree_node *q6(struct tree_node *root, int x);
void what(struct tree_node *root, int val);
int add(struct tree_node *current_ptr);

// Functions to find the kth smallest item in the tree.
int numnodes(struct tree_node *root);
int findKthSmallest(struct tree_node *root, int k);

// Menu of user choices.
int menu();

int main()
{

  struct tree_node *my_root = NULL, *temp_node;
  int done = 0, ans = 1, val;

  // Get first user choice.
  ans = menu();

  // Run the menu driven program.
  while (ans != QUIT)
  {

    // Insert.
    if (ans == 1)
    {

      // Get value to insert.
      printf("What value would you like to insert?");
      scanf("%d", &val);
      temp_node = create_node(val); // Create the node.

      // Insert the value.
      my_root = insert(my_root, temp_node);
    }

    // Delete.
    if (ans == 2)
    {

      // Get value to delete.
      printf("What value would you like to delete?\n");
      scanf("%d", &val);

      // Didn't find it.
      if (!find(my_root, val))
        printf("Sorry that value isn't in the tree to delete.\n");

      // Do the delete!
      else
        my_root = delRec(my_root, val);
    }

    // Search.
    if (ans == 3)
    {

      // Get search value.
      printf("What value would you like to search for?\n");
      scanf("%d", &val);

      // Display search result.
      if (find(my_root, val))
        printf(" Found %d in the tree.\n", val);
      else
        printf(" Did not find %d in the tree.\n", val);
    }

    // Sum of the nodes in the tree.
    if (ans == 4)
      printf("The sum of the nodes in your tree is %d.\n", add(my_root));

    // Inorder traversal.
    if (ans == 5)
    {

      // Print the resulting tree.
      printf("Here is an inorder traversal of your tree: ");
      inorder(my_root);
      printf("\n");
    }

    // Preorder traversal.
    if (ans == 6)
    {
      printf("Here is an preorder traversal of your tree: ");
      preorder(my_root);
      printf("\n");
    }

    // Get the kth smallest.
    if (ans == 7)
    {

      // Ask user for rank.
      int rank;
      printf("Which ranked item would you like to find?\n");
      scanf("%d", &rank);

      // Assumes a valid one was entered ans answers it.
      printf("The item is %d\n", findKthSmallest(my_root, rank));
    }

    // See if there's another choice.
    ans = menu();
  }

  return 0;
}

// Prints a preorder traversal of the tree rooted at current_ptr.
void preorder(struct tree_node *current_ptr)
{

  // Only traverse the node if it's not null.
  if (current_ptr != NULL)
  {
    printf("%d ", current_ptr->data);
    preorder(current_ptr->left);  // Go Left.
    preorder(current_ptr->right); // Go Right.
  }
}

// Prints a inorder traversal of the tree rooted at current_ptr.
void inorder(struct tree_node *current_ptr)
{

  // Only traverse the node if it's not null.
  if (current_ptr != NULL)
  {
    inorder(current_ptr->left);       // Go Left.
    printf("%d ", current_ptr->data); // Print the root.
    inorder(current_ptr->right);      // Go Right.
  }
}

// Inserts a the node pointed to by element in the tree rooted by
// root and returns a pointer to the resulting tree.
struct tree_node *insert(struct tree_node *root,
                         struct tree_node *element)
{

  // Inserting into an empty tree.
  if (root == NULL)
    return element;

  // element should be inserted to the right.
  if (element->data > root->data)
    root->right = insert(root->right, element);

  // element should be inserted to the left.
  else
    root->left = insert(root->left, element);

  // Return the root pointer of the updated tree.
  return root;
}

// Returns a pointer to a single node storing val.
struct tree_node *create_node(int val)
{

  // Allocate space for the node, set the fields.
  struct tree_node *temp;
  temp = (struct tree_node *)malloc(sizeof(struct tree_node));
  temp->data = val;
  temp->left = NULL;
  temp->right = NULL;

  return temp; // Return a pointer to the created node.
}

// Returns 1 iff the BST pointed to by current_ptr stores val.
int find(struct tree_node *current_ptr, int val)
{

  // Get this out of the way.
  if (current_ptr == NULL)
    return 0;

  // Found the value at the root.
  if (current_ptr->data == val)
    return 1;

  // Search to the left.
  else if (val < current_ptr->data)
    return find(current_ptr->left, val);

  // Or...search to the right.
  else
    return find(current_ptr->right, val);
}

// Foundation Exam Question!
void what(struct tree_node *root, int val)
{
  if (root != NULL)
  {
    if (root->data > val)
      printf("%d ", root->data + val);
    if (root->data % val > 5)
      what(root->left, val + 3);
    else
      what(root->right, val + 4);
  }
}

// Returns the sum of the nodes in the tree pointed to by current_ptr.
int add(struct tree_node *current_ptr)
{

  // Tree exists, add node to left and right sums.
  if (current_ptr != NULL)
    return current_ptr->data + add(current_ptr->left) +
           add(current_ptr->right);

  // NULL tree.
  else
    return 0;
}

// Returns the parent of the node pointed to by node in the tree rooted at
// root. If the node is the root of the tree, or the node doesn't exist in
// the tree, null will be returned.
struct tree_node *parent(struct tree_node *root, struct tree_node *node)
{

  // Take care of NULL cases.
  if (root == NULL || root == node)
    return NULL;

  // The root is the direct parent of node.
  if (root->left == node || root->right == node)
    return root;

  // Look for node's parent in the left side of the tree.
  if (node->data < root->data)
    return parent(root->left, node);

  // Look for node's parent in the right side of the tree.
  else if (node->data > root->data)
    return parent(root->right, node);

  return NULL; // Catch any other extraneous cases.
}

// Returns a pointer to the node storing the minimum value in the tree
// with the root, root. Will not work if called with an empty tree.
struct tree_node *minVal(struct tree_node *root)
{

  // Root stores the minimal value.
  if (root->left == NULL)
    return root;

  // The left subtree of the root stores the minimal value.
  else
    return minVal(root->left);
}

// Returns a pointer to the node storing the maximum value in the tree
// with the root, root. Will not work if called with an empty tree.
struct tree_node *maxVal(struct tree_node *root)
{

  // Root stores the maximal value.
  if (root->right == NULL)
    return root;

  // The right subtree of the root stores the maximal value.
  else
    return maxVal(root->right);
}

// Returns 1 if node is a leaf node, 0 otherwise.
int isLeaf(struct tree_node *node)
{
  return (node->left == NULL && node->right == NULL);
}

// Returns 1 iff node has a left child and no right child.
int hasOnlyLeftChild(struct tree_node *node)
{
  return (node->left != NULL && node->right == NULL);
}

// Returns 1 iff node has a right child and no left child.
int hasOnlyRightChild(struct tree_node *node)
{
  return (node->left == NULL && node->right != NULL);
}

// Returns a pointer to a node that stores value in it in the subtree
// pointed to by current_ptr. NULL is returned if no such node is found.
struct tree_node *findNode(struct tree_node *current_ptr, int value)
{

  // Check if there are nodes in the tree.
  if (current_ptr != NULL)
  {

    // Found the value at the root.
    if (current_ptr->data == value)
      return current_ptr;

    // Search to the left.
    if (value < current_ptr->data)
      return findNode(current_ptr->left, value);

    // Or...search to the right.
    else
      return findNode(current_ptr->right, value);
  }

  // Null case.
  else
    return NULL;
}

// Will delete the node storing value in the tree rooted at root. The
// value must be present in the tree in order for this function to work.
// The function returns a pointer to the root of the resulting tree.
struct tree_node *delete (struct tree_node *root, int value)
{

  struct tree_node *delnode, *new_del_node, *save_node;
  struct tree_node *par;
  int save_val;

  delnode = findNode(root, value); // Get a pointer to the node to delete.

  par = parent(root, delnode); // Get the parent of this node.

  // Take care of the case where the node to delete is a leaf node.
  if (isLeaf(delnode))
  {

    // Deleting the only node in the tree.
    if (par == NULL)
    {
      free(root); // free the memory for the node.
      return NULL;
    }

    // Deletes the node if it's a left child.
    if (value < par->data)
    {
      free(par->left); // Free the memory for the node.
      par->left = NULL;
    }

    // Deletes the node if it's a right child.
    else
    {
      free(par->right); // Free the memory for the node.
      par->right = NULL;
    }

    return root; // Return the root of the new tree.
  }

  // Take care of the case where the node to be deleted only has a left
  // child.
  if (hasOnlyLeftChild(delnode))
  {

    // Deleting the root node of the tree.
    if (par == NULL)
    {
      save_node = delnode->left;
      free(delnode);    // Free the node to delete.
      return save_node; // Return the new root node of the resulting tree.
    }

    // Deletes the node if it's a left child.
    if (value < par->data)
    {
      save_node = par->left;       // Save the node to delete.
      par->left = par->left->left; // Readjust the parent pointer.
      free(save_node);             // Free the memory for the deleted node.
    }

    // Deletes the node if it's a right child.
    else
    {
      save_node = par->right;        // Save the node to delete.
      par->right = par->right->left; // Readjust the parent pointer.
      free(save_node);               // Free the memory for the deleted node.
    }

    return root; // Return the root of the tree after the deletion.
  }

  // Takes care of the case where the deleted node only has a right child.
  if (hasOnlyRightChild(delnode))
  {

    // Node to delete is the root node.
    if (par == NULL)
    {
      save_node = delnode->right;
      free(delnode);
      return save_node;
    }

    // Delete's the node if it is a left child.
    if (value < par->data)
    {
      save_node = par->left;
      par->left = par->left->right;
      free(save_node);
    }

    // Delete's the node if it is a right child.
    else
    {
      save_node = par->right;
      par->right = par->right->right;
      free(save_node);
    }

    // This is the resulting root.
    return root;
  }

  // Find the new physical node to delete.
  new_del_node = minVal(delnode->right);
  save_val = new_del_node->data;

  delete (root, save_val); // Now, delete the proper value.

  // Restore the data to the original node to be deleted.
  delnode->data = save_val;

  return root;
}

// Recursive delete. Deletes a node storing delval from the tree
// rooted at root.
struct tree_node *delRec(struct tree_node *root, int delval)
{

  // Nothing happens in this case.
  if (root == NULL)
    return NULL;

  // Base case - found value to delete.
  if (root->data == delval)
  {

    // Leaf node.
    if (isLeaf(root))
    {
      free(root);
      return NULL;
    }

    // Has only a left child.
    else if (hasOnlyLeftChild(root))
    {
      struct tree_node *ret = root->left;
      free(root);
      return ret;
    }

    // Has only a right child.
    else if (hasOnlyRightChild(root))
    {
      struct tree_node *ret = root->right;
      free(root);
      return ret;
    }

    // two child case.
    else
    {
      struct tree_node *new_del_node = minVal(root->right);
      int save_val = new_del_node->data;
      root->right = delRec(root->right, save_val);
      root->data = save_val;
      return root;
    }
  }

  // Here we must go left.
  else if (delval < root->data)
    root->left = delRec(root->left, delval);

  // And here we must go right.
  else
    root->right = delRec(root->right, delval);

  // If we get here, this is the root of the tree.
  return root;
}

// Old Foundation Exam Question
struct tree_node *q6(struct tree_node *root, int x)
{

  if (root == NULL)
    return NULL;

  if (root->data > x)
  {

    struct tree_node *tmp = q6(root->left, x);
    if (tmp == NULL)
      return root;
    else
      return tmp;
  }
  else
    return q6(root->right, x);
}

// Returns the number of nodes in the tree pointed to by root.
int numnodes(struct tree_node *root)
{

  // No nodes in null tree.
  if (root == NULL)
    return 0;

  // Add root to # nodes in left and right.
  return 1 + numnodes(root->left) + numnodes(root->right);
}

// Guaranteed that k is in between 1 and the number of nodes in the tree
// pointed to by root.
int findKthSmallest(struct tree_node *root, int k)
{

  // # nodes on left.
  int numNodesLeft = numnodes(root->left);

  // Enough nodes on left, go left.
  if (numNodesLeft >= k)
    return findKthSmallest(root->left, k);

  // Got lucky it's the root.
  else if (numNodesLeft == k - 1)
    return root->data;

  // We must go right, but our rank goes down.
  else
    return findKthSmallest(root->right, k - numNodesLeft - 1);
}

// Prints out the menu of choices for the user and returns their choice.
int menu()
{

  int ans;
  printf("Here are your choices.\n");
  printf("1. Insert an item into your tree.\n");
  printf("2. Delete an item from your tree.\n");
  printf("3. Search for an item in your tree.\n");
  printf("4. Print the sum of the nodes in your tree.\n");
  printf("5. Print out an inorder traversal of your tree.\n");
  printf("6. Print preorder.\n");
  printf("7. Find kth smallest.\n");
  printf("8. Quit.\n");
  scanf("%d", &ans);
  return ans;
}
