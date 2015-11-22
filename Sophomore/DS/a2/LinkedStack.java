public class LinkedStack<T> implements StackInterface<T>
{
	private Node topNode; // references the first node in the chain
	
	public LinkedStack()
	{
		topNode = null;
	} // end default constructor
	
	// 6.03
	public void push(T newEntry)
	{
		Node newNode = new Node(newEntry, topNode);
		topNode = newNode;
	} // end push

	// 6.04
	public T peek()
	{
		T top = null;
		
		if (topNode != null)
			top = topNode.getData();
		
		return top;
	} // end peek

	// 6.05
	public T pop()
	{
	   T top = peek();
	  
	   if (topNode != null)
	      topNode = topNode.getNextNode(); 

	   return top;
	} // end pop

/*
// does not call peek (Question 1, Chapter 6)
	public T pop()
	{
		T top = null;
		
		if (topNode != null)
		{
			top = topNode.getData();   
			topNode = topNode.getNextNode();
		} // end if
		
		return top;
	} // end pop
*/		

	// 6.06
	public boolean isEmpty()
	{
		return topNode == null;
	} // end isEmpty
	
	// 6.06
	public void clear()
	{
		topNode = null;	
	} // end clear

	private class Node
	{
      private T    data; // entry in stack
      private Node next; // link to next node

      private Node(T dataPortion)
      {
         data = dataPortion;
         next = null;	
      } // end constructor

      private Node(T dataPortion, Node linkPortion)
      {
         data = dataPortion;
         next = linkPortion;	
      } // end constructor

      private T getData()
      {
         return data;
      } // end getData

      private void setData(T newData)
      {
         data = newData;
      } // end setData

      private Node getNextNode()
      {
         return next;
      } // end getNextNode

      private void setNextNode(Node nextNode)
      {
         next = nextNode;
      } // end setNextNode
	} // end Node
} // end LinkedStack