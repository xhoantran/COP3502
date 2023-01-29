#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 20

int main()
{
  // Declare original string variable
  char s[MAX_LENGTH];
  scanf("%s", s);
  char last = s[strlen(s) - 1];

  // Declare number of available strings variable
  int n;
  scanf("%d", &n);

  // Allocate memory for n strings
  char **arr = (char **)malloc(n * sizeof(char *));
  // Read n strings from stdin and store them in arr
  for (int i = 0; i < n; i++)
  {
    arr[i] = (char *)malloc(MAX_LENGTH * sizeof(char));
    scanf("%s", arr[i]);
  }

  // Declare counter arr works as a dictionary
  // to store first letter of each string
  int count['z' - 'a' + 1] = {0};

  // Declare variables to store the number of strings
  // that contain the last letter and their index
  int *valid = (int *)malloc(n * sizeof(int));
  int numValid = 0;

  // Count the valid strings and store their index in valid int array
  for (int x = 0; x < n; x++)
  {
    // Get the first letter of the strings
    count[arr[x][0] - 'a']++;
    // If the first letter is the last letter of the original string
    if (arr[x][0] == last)
      valid[numValid++] = x;
  }

  // If there are no valid strings
  if (numValid == 0)
  {
    printf("?");
    return 0;
  }

  // Loop through the valid strings
  for (int i = 0; i < numValid; i++)
  {
    // Get the last and first letter of the strings
    char lastChar = arr[valid[i]][strlen(arr[valid[i]]) - 1];
    char firstChar = arr[valid[i]][0];
    // If there is no other string that contains the last letter of
    // valid string or the last letter of valid string is its first
    // letter, print the valid string and exclamation mark
    if (!count[lastChar - 'a'] || lastChar == firstChar && count[lastChar - 'a'] == 1)
    {
      printf("%s!", arr[valid[i]]);
      return 0;
    }
  }

  printf("%s", arr[valid[0]]);

  return 0;
}
