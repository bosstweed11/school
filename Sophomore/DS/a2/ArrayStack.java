public class ArrayStack<T> implements StackInterface<T>
{
	private T[] stack;    // array of stack entries
	private int topIndex; // index of top entry
	private static final int DEFAULT_INITIAL_CAPACITY = 50;
	
	public ArrayStack()
	{
      this(DEFAULT_INITIAL_CAPACITY);
 	} // end default constructor
	
	public ArrayStack(int initialCapacity)
	{
      // the cast is safe because the new array contains null entries
      @SuppressWarnings("unchecked")
      T[] tempStack = (T[])new Object[initialCapacity];
      stack = tempStack;
		topIndex = -1;
	} // end constructor

   // 6.9
	public void push(T newEntry)
	{
		ensureCapacity();
		topIndex++;
		stack[topIndex] = newEntry;
	} // end push

   // Doubles the size of the array stack if it is full
	private void ensureCapacity() 
	{
 	   if (topIndex == stack.length - 1) // if array is full,
         // double size of array
         stack = Arrays.copyOf(stack, 2 * stack.length);
	} // end ensureCapacity

   // 6.10
	public T peek()
	{
		T top = null;
		
		if (!isEmpty())
			top = stack[topIndex];
		
		return top;
	} // end peek

   // 6.11
	public T pop()
	{
		T top = null;
		
		if (!isEmpty())
		{
			top = stack[topIndex];
			stack[topIndex] = null;
			topIndex--; 
		} // end if
		
		return top;
	} // end pop

   // 6.12
   public boolean isEmpty()
	{
		return topIndex < 0;
	} // end isEmpty
	
	public void clear()
	{
		for (; topIndex > -1; topIndex--)
			stack[topIndex] = null;
   //	Assertion: topIndex is -1	
	} // end clear
} // end ArrayStack