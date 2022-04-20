#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
  int *items;
  int length;
} Set;

// Argument structure for threads
typedef struct
{
  Set *set;
  int value;
} arg_t;

Set *init();
bool is_empty(Set *set);
void insert(void *arguments);
void print_set(Set *set);
bool contains(Set *set, int item);
void delete (Set *set, int item);

pthread_mutex_t lock;

int main()
{
  if (pthread_mutex_init(&lock, NULL))
  {
    printf("Mutext failed to init\n");
    return 1;
  }

  pthread_t thread_id[100];
  arg_t *args[100];
  Set *set1 = init();

  for (int i = 0; i < 100; i++)
  {
    args[i] = malloc(sizeof(arg_t));

    args[i]->set = set1;
    args[i]->value = i;

    pthread_create(&thread_id[i], NULL, (void *)insert, args[i]);
  }

  for (int i = 0; i < 100; i++)
  {
    pthread_join(thread_id[i], NULL);
  }

  pthread_mutex_destroy(&lock);

  print_set(set1);

  return 0;
}

// Initialize new set
Set *init()
{
  Set *new_set = malloc(sizeof(Set));
  new_set->length = 0;
  new_set->items = malloc(sizeof(int));

  return new_set;
}

// Check if set is empty
bool is_empty(Set *set)
{
  return (set->length == 0);
}

// Insert element into set
void insert(void *arguments)
{
  arg_t *args = (arg_t *)arguments;
  int item = args->value;
  Set *set = args->set;

  pthread_mutex_lock(&lock);

  if (!contains(set, item))
  {
    set->items = realloc(set->items, sizeof(int) * (set->length + 1));
    set->items[set->length] = item;
    set->length++;
  }

  pthread_mutex_unlock(&lock);
}

// Print set
void print_set(Set *set)
{
  for (size_t i = 0; i < set->length; i++)
  {
    if (i == (set->length - 1))
      printf("%d\n", set->items[i]);
    else
      printf("%d, ", set->items[i]);
  }
}

// Check if set contains specified element
bool contains(Set *set, int item)
{
  for (size_t i = 0; i < set->length; i++)
  {
    if (set->items[i] == item)
      return true;
  }

  return false;
}

// Delete item from set
void delete (Set *set, int item)
{
  for (size_t i = 0; i < set->length; i++)
  {
    if (set->items[i] == item)
    {
      for (size_t j = i; j < set->length - 1; j++)
      {
        set->items[j] = set->items[j + 1];
      }

      set->items = realloc(set->items, sizeof(int) * set->length);

      set->length--;

      break;
    }
  }
}