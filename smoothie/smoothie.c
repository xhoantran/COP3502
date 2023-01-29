// Hoan Tran
// 01/18/2022
// Solution to COP 3502 Program #1: Smoothie

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LEN 20

typedef struct item
{
  int itemID;
  int numParts;
} item;

typedef struct recipe
{
  int numItems;
  item *itemList;
  int totalParts;
} recipe;

// Declare function prototype
char **readIngredients(int numIngredients);
recipe *readRecipe(int numItems);
recipe **readAllRecipes(int numRecipes);
double *calculateOrder(int numSmoothies, recipe **recipeList, int numIngredients);
void printOrder(char **ingredientNames, double *orderList, int numIngredients);
void freeIngredients(char **ingredientList, int numIngredients);
void freeRecipes(recipe **allRecipes, int numRecipes);

int main()
{
  // Decalre variables for # of ingredients, # of recipes, and # of stores
  int numIngre, numRecipe, numStore;

  // Read in # of ingredients
  scanf("%d", &numIngre);

  // Declare dynamic array of ingredients
  // and read in ingredients using readIngredients()
  char **ingredientNames = readIngredients(numIngre);

  // Read in # of recipes
  scanf("%d", &numRecipe);

  // Declare dynamic array of recipes
  // and read in recipes using readRecipe()
  recipe **smoothieList = readAllRecipes(numRecipe);

  // Read in # of stores
  scanf("%d", &numStore);

  // Loop though each store
  for (int i = 0; i < numStore; i++)
  {

    // Print store name
    printf("Store #%d:\n", i + 1);

    // Declare # of smoothies
    int numSmoothies;

    // Read in # of smoothies for this store
    scanf("%d", &numSmoothies);

    // Declare dynamic array to store amounts of each ingredient
    // and use calculateOrder() to get the values
    double *amtOfEachItem = calculateOrder(numSmoothies, smoothieList, numIngre);

    // Print out the order
    printOrder(ingredientNames, amtOfEachItem, numIngre);

    // Free the amounts of ingredients dynamic array
    // after printing out
    free(amtOfEachItem);
  }

  // Free the ingredient dynamic array
  freeIngredients(ingredientNames, numIngre);

  return 0;
}

// Pre-condition: 0 < numIngredients <= 100000
// Post-condition: Reads in numIngredients number of strings
// from standard input, allocates an array of
// strings to store the input, and sizes each
// individual string dynamically to be the
// proper size (string length plus 1), and
// returns a pointer to the array.
char **readIngredients(int numIngredients)
{

  // Dynamically allocate a array of char pointers
  char **ingredientNames = malloc(sizeof(char *) * numIngredients);

  // Start looping through each ingredient
  for (int i = 0; i < numIngredients; i++)
  {

    // Declare a string to store the input
    char ingredientIn[STR_LEN + 1];

    // Read in the ingredient
    scanf("%s", ingredientIn);

    // Dynamically allocate a string to store the input
    // with enough space
    ingredientNames[i] = malloc((strlen(ingredientIn) + 1) * sizeof(char));

    // Copy the input into the string
    strcpy(ingredientNames[i], ingredientIn);
  }

  // Return the array of char pointers
  return ingredientNames;
}

// Pre-condition: 0 < numItems <= 100
// Post-condition: Reads in numItems number of items
// from standard input for a smoothie recipe,
// Dynamically allocates space for a single
// recipe, dynamically allocates an array of
// item of the proper size, updates the
// numItems field of the struct, fills the
// array of items appropriately based on the
// input and returns a pointer to the struct
// dynamically allocated.
recipe *readRecipe(int numItems)
{

  // Dynamically allocate a recipe struct
  recipe *r = malloc(sizeof(recipe));

  // Store the number of items in the struct
  r->numItems = numItems;

  // Initalize the total number of parts to 0
  r->totalParts = 0;

  // Dynamically allocate an array of items
  // using the number of items
  r->itemList = malloc(sizeof(item) * numItems);

  // Start looping through each item
  for (int i = 0; i < numItems; i++)
  {

    // Read in the item ID and number of parts
    // for each item
    scanf("%d%d", &r->itemList[i].itemID, &r->itemList[i].numParts);

    // Increment the total number of parts
    r->totalParts += r->itemList[i].numParts;
  }

  // Return the recipe struct
  return r;
};

// Pre-condition: 0 < numRecipes <= 100000
// Post-condition: Dynamically allocates an array of pointers to
// recipes of size numRecipes, reads numRecipes
// number of recipes from standard input, creates
// structs to store each recipe and has the
// pointers point to each struct, in the order
// the information was read in. (Should call
// readRecipe in a loop.)
recipe **readAllRecipes(int numRecipes)
{

  // Dynamically allocate an array of pointers to recipes
  recipe **r = malloc(sizeof(recipe *) * numRecipes);

  // Declare variable to store the number of items for each recipe
  int numItems;

  // Start looping through each recipe
  for (int i = 0; i < numRecipes; i++)
  {

    // Read in the number of items for this recipe
    scanf("%d", &numItems);

    // Read in the recipe using readRecipe()
    // and store the pointer to the recipe in the array
    r[i] = readRecipe(numItems);
  }

  // Return the array of pointers to recipes
  return r;
};

// Pre-condition: 0 < numSmoothies <= 100000, recipeList is
// pointing to the list of all smoothie recipes and
// numIngredients equals the number of total ingredients.
// Post-condition: Reads in information from standard input
// about numSmoothies number of smoothie orders and dynamically
// allocates an array of doubles of size numIngredients such
// that index i stores the # of pounds of ingredient i
// needed to fulfill all smoothie orders and returns a pointer
// to the array.
double *calculateOrder(int numSmoothies, recipe **recipeList, int numIngredients)
{

  // Dynamically allocate an array of doubles
  // and initialize each element to 0
  double *orderList = calloc(sizeof(double), numIngredients);

  // Declare variables to store the recipe index,
  // ingredient index, and number of parts
  int recipeId, ingredientId, ingredientPart;

  // Variable to store the weight of each smoothie
  double smoothieWeight;

  // Start looping through each smoothie
  for (int i = 0; i < numSmoothies; i++)
  {

    // Read in the recipe index and weight
    scanf("%d %lf", &recipeId, &smoothieWeight);

    // Loop through each item in the recipe
    for (int j = 0; j < recipeList[recipeId]->numItems; j++)
    {

      // Get the ingredient index by using the recipe array
      ingredientId = recipeList[recipeId]->itemList[j].itemID;

      // Get the number of parts by using the recipe array
      ingredientPart = recipeList[recipeId]->itemList[j].numParts;

      // Update the order list by adding the number of parts
      // multiplied by the weight of the smoothie
      orderList[ingredientId] += ingredientPart / (double)recipeList[recipeId]->totalParts * smoothieWeight;
    }
  }

  // Return the order list pointer
  return orderList;
};

// Pre-conditions: ingredientNames store the names of each
// ingredient and orderList stores the amount
// to order for each ingredient, and both arrays
// are of size numIngredients.
// Post-condition: Prints out a list, in ingredient order, of each
// ingredient, a space and the amount of that
// ingredient to order rounded to 6 decimal
// places. One ingredient per line.
void printOrder(char **ingredientNames, double *orderList, int numIngredients)
{
  // Start looping through each ingredient
  for (int i = 0; i < numIngredients; i++)
  {
    // Print the ingredient name and the amount to order
    // if the amount to order is not 0
    if (orderList[i] != 0)
      printf("%s %.6f\n", ingredientNames[i], orderList[i]);
  }
};

// Pre-conditions: ingredientList is an array of char* of size
// numIngredients with each char* dynamically allocated.
// Post-condition: all the memory pointed to by ingredientList is
// freed.
void freeIngredients(char **ingredientList, int numIngredients)
{

  // Start looping through each string in the array
  for (int i = 0; i < numIngredients; i++)
    // Free the char array
    free(ingredientList[i]);

  // Free the array
  free(ingredientList);
};

// Pre-conditions: allRecipes is an array of recipe* of size
// numRecipes with each recipe* dynamically allocated
// to point to a single recipe.
// Post-condition: all the memory pointed to by allRecipes is
// freed.
void freeRecipes(recipe **allRecipes, int numRecipes)
{
  // Start looping through each recipe in the array
  for (int i = 0; i < numRecipes; i++)
  {

    // Free the item list
    free(allRecipes[i]->itemList);

    // Free the recipe
    free(allRecipes[i]);
  }

  // Free the array
  free(allRecipes);
};