#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Node structure for stack
typedef struct node_t
{
  int key;
  struct node_t *next;
} Node;

// Stack structure (linked list)
typedef struct
{
  Node *head;
  int len;
} Stack;

// Argument structure for threads
typedef struct
{
  Node *node;
  Stack *stack;
} arg_t;

Node *init_node(int key);
Stack *init_stack(void);
void push(void *arguments);
Node *pop(void *arguments);
void print_stack(Stack *stack);

pthread_mutex_t lock;

// init_node()
// @params: (int) key - key of new node
// @return: (Node *) - pointer to newly created node
Node *init_node(int key)
{
  Node *result = malloc(sizeof(Node));

  result->key = key;
  result->next = NULL;

  return result;
}

// init_stack()
// @params: (void)
// @return: (Stack *) - pointer to newly created stack
Stack *init_stack(void)
{
  Stack *result = malloc(sizeof(Stack));

  result->head = NULL;
  result->len = 0;

  return result;
}

// push()
// @params: (void *) arguments - a pointer to a struct containing the arguments
// @return: (void)
void push(void *arguments)
{
  pthread_mutex_lock(&lock);

  arg_t *args = (arg_t *)arguments;
  Node *new_node = args->node;
  Stack *stack = args->stack;

  if (stack == NULL)
    return;

  stack->len++;
  new_node->next = stack->head;
  stack->head = new_node;

  pthread_mutex_unlock(&lock);

  return;
}

// pop()
// @params: (void *) arguments - a pointer to a struct containing the arguments
// @return: (Node *) - pointer to popped node
Node *pop(void *arguments)
{
  pthread_mutex_lock(&lock);

  arg_t *args = (arg_t *)arguments;
  Stack *stack = args->stack;

  if (stack == NULL)
  {
    return NULL;
  }

  Node *free_node = stack->head;

  stack->len--;
  if (stack->head)
  {
    stack->head = stack->head->next;
  }

  pthread_mutex_unlock(&lock);

  return free_node;
}

// print_stack()
// @params: (Stack *) stack - a pointer to a struct containing the arguments
// @return: (void)
void print_stack(Stack *stack)
{
  Node *curr = stack->head;

  while (curr != NULL)
  {
    if (curr->next != NULL)
      printf("%d ", curr->key);
    else
      printf("%d\n", curr->key);
    curr = curr->next;
  }

  return;
}

// main()
int main()
{
  if (pthread_mutex_init(&lock, NULL))
  {
    printf("Mutext failed to init\n");
    return 1;
  }

  pthread_t thread_id[100];
  arg_t *args[100];
  Stack *my_stack = init_stack();

  for (int i = 0; i < 100; i++)
  {
    args[i] = malloc(sizeof(arg_t));

    Node *new_node = init_node(i);

    args[i]->node = new_node;
    args[i]->stack = my_stack;

    pthread_create(&thread_id[i], NULL, (void *)push, args[i]);
  }

  for (int i = 0; i < 100; i++)
  {
    pthread_join(thread_id[i], NULL);
  }

  pthread_mutex_destroy(&lock);

  print_stack(my_stack);

  return 0;
}
