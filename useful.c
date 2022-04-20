// Union two sets together
Set *set_union(Set *setA, Set *setB)
{
  Set *result = init();
  for (size_t i = 0; i < setA->length; i++)
  {
    insert(result, setA->items[i]);
  }

  for (size_t i = 0; i < setB->length; i++)
  {
    insert(result, setB->items[i]);
  }

  return result;
}

// Intersection of two sets
Set *set_inter(Set *setA, Set *setB)
{
  Set *result = init();
  for (size_t i = 0; i < setA->length; i++)
  {
    for (size_t j = 0; j < setB->length; j++)
    {
      if (setA->items[i] == setB->items[j])
        insert(result, setA->items[i]);
    }
  }

  return result;
}

// Difference of two sets
Set *set_diff(Set *setA, Set *setB)
{
  Set *result = init();
  for (size_t i = 0; i < setA->length; i++)
  {
    bool found = false;
    for (size_t j = 0; j < setB->length; j++)
    {
      if (setA->items[i] == setB->items[j])
        found = true;
    }

    if (!found)
      insert(result, setA->items[i]);
  }

  // Delete temp??
  return result;
}

// Check if subset
bool is_subset(Set *setA, Set *setB)
{
  for (size_t i = 0; i < setA->length; i++)
  {
    bool found = false;
    for (size_t j = 0; j < setB->length; j++)
    {
      if (setA->items[i] == setB->items[j])
        found = true;
    }

    if (!found)
      return false;
  }

  return true;
}

// Check if sets are equal
bool set_equal(Set *setA, Set *setB)
{
  if (setA->length != setB->length)
    return false;

  return is_subset(setA, setB);
}