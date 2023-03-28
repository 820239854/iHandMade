// recursive functions

unsigned recursive_fn(unsigned a)
{
	unsigned result = 1;
	if (a > 1)
	{   result = recursive_fn(a-1) + recursive_fn(a-2);   }
	return result;
}
