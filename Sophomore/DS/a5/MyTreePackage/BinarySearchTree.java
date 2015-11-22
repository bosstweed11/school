// CS 0445 Spring 2012
// Modified BinarySearchTree class.  This class now extends
// ComparableBinaryTree rather than BinaryTree.
// Add your methods to the class so that it works as specified
// in the assignment.  Note: This class will work without any modifications,
// since it is not defining any new methods -- rather it is overriding already
// existing methods to be more efficient.  However, you will lose credit if
// you do not override the methods to take advantage of the fact that this is
// a BST, not a general ComparableBinaryTree.

package MyTreePackage;
import java.util.Iterator;
/**
 * A class that implements the ADT binary search tree by extending BinaryTree.
 * 
 * @author Frank M. Carrano
 * @version 2.0
 */
public class BinarySearchTree<T extends Comparable<? super T>>
             extends ComparableBinaryTree<T> 
             implements SearchTreeInterface<T>, java.io.Serializable
{
  public BinarySearchTree()
  {
    super();
  } // end default constructor
  
  public BinarySearchTree(T rootEntry)
  {
    super();
    setRootNode(new BinaryNode<T>(rootEntry));
  } // end constructor
  
  public void setTree(T rootData) // disable setTree (see Segment 27.6)
  {
    throw new UnsupportedOperationException();
  } // end setTree
  
  public void setTree(T rootData, BinaryTreeInterface<T> leftTree, 
                                  BinaryTreeInterface<T> rightTree)
  {
    throw new UnsupportedOperationException();
  } // end setTree
  
	public T getEntry(T entry) 
	{
		T result = null;
		boolean found = false;
		
		BinaryNodeInterface<T> currentNode = getRootNode();
		while (!found && (currentNode != null) )
		{
			T currentEntry = currentNode.getData();
			
			if (entry.equals(currentEntry))
			{
				result = currentEntry;
				found = true;
			}
			else if (entry.compareTo(currentEntry) < 0)
				currentNode = currentNode.getLeftChild(); 
			else
				currentNode = currentNode.getRightChild(); 
		} // end while
		
		return result;
	} // end getEntry
	
	// 27.10
	public boolean contains(T entry)
	{
		return getEntry(entry) != null;
	} // end contains
	
	public T add(T newEntry)
	{
	  T result = null;
	  
	  if (isEmpty())
	    setRootNode(new BinaryNode<T>(newEntry));
	  else
	    result = addEntry(newEntry);
	    
	  return result;
	} // end add

	// 27.18
	private T addEntry(T newEntry)
	{
	  BinaryNodeInterface<T> currentNode = getRootNode();
	  assert currentNode != null;
	  T result = null;
	  boolean found = false;
	  
	  while (!found)
	  {
	    T currentEntry = currentNode.getData();
	    int comparison = newEntry.compareTo(currentEntry);
	    
	    if (comparison == 0)
	    { // newEntry matches currentEntry;
	      // return and replace currentEntry
	      found = true;
	      result = currentEntry;
	      currentNode.setData(newEntry);
	    }
	    else if (comparison < 0)
	    {
	      if (currentNode.hasLeftChild())
	        currentNode = currentNode.getLeftChild();
	      else
	      {
	        found = true;
	        currentNode.setLeftChild(new BinaryNode<T>(newEntry));
	      } // end if
	    }
	    else
	    {
	      assert comparison > 0;
	      
	      if (currentNode.hasRightChild())
	        currentNode = currentNode.getRightChild();
	      else
	      {
	        found = true;
	        currentNode.setRightChild(new BinaryNode<T>(newEntry));
	      } // end if
	    } // end if
	  } // end while
	  
	  return result; 
	} // end addEntry

	// 27.37
	public T remove(T entry)
	{
	  T result = null;
	  
	  // locate node (and its parent) that contains a match for entry
	  NodePair pair = findNode(entry);
	  BinaryNodeInterface<T> currentNode = pair.getFirst();
	  BinaryNodeInterface<T> parentNode = pair.getSecond();
	  
	  if (currentNode != null) // entry is found
	  {
	    result = currentNode.getData(); // get entry to be removed
	    
	    // Case 1: currentNode has two children
	    if (currentNode.hasLeftChild() && currentNode.hasRightChild())
	    {
	      // replace entry in currentNode with the entry in another node
	      // that has at most one child; that node can be deleted
	      
	      // get node to remove (contains inorder predecessor; has at 
	      // most one child) and its parent
	      pair = getNodeToRemove(currentNode);
	      BinaryNodeInterface<T> nodeToRemove = pair.getFirst();
	      parentNode = pair.getSecond();
	      
	      // copy entry from nodeToRemove to currentNode
	      currentNode.setData(nodeToRemove.getData());
	      
	      currentNode = nodeToRemove;
	      // Assertion: currentNode is the node to be removed; it has at 
	      //            most one child
	      // Assertion: Case 1 has been transformed to Case 2
	    } // end if
	    
	    // Case 2: currentNode has at most one child; delete it
	    removeNode(currentNode, parentNode);
	  } // end if
	  
	  return result;
	} // end remove

// Other public methods in SearchTreeInterface are inherited from BinaryTree.

	// locate node that contains a match for entry
	private NodePair findNode(T entry)
	{
	  NodePair result = new NodePair();
	  boolean found = false;
	  
	  BinaryNodeInterface<T> currentNode = getRootNode();
	  BinaryNodeInterface<T> parentNode = null;
	  
	  while (!found && (currentNode != null) )
	  {
	    T currentEntry = currentNode.getData();
	    int comparison = entry.compareTo(currentEntry);
	    
	    if (comparison < 0)
	    {
	      parentNode = currentNode;
	      currentNode = currentNode.getLeftChild(); 
	    }
	    else if (comparison > 0)
	    {
	      parentNode = currentNode;
	      currentNode = currentNode.getRightChild(); 
	    }
	    else
	      found = true;
	  } // end while
	  
	  if (found)
	  	result = new NodePair(currentNode, parentNode);
	  	// found entry is currentNode.getData()
	  	
	  return result;
	} // end findNode

	/** Task: Gets the node that contains the inorder predecessor of currentNode.
	 *  Precondition: currentNode has two children */
	// 27.39
	private NodePair getNodeToRemove(BinaryNodeInterface<T> currentNode)
	{
	  // find node with largest entry in left subtree by
	  // moving as far right in the subtree as possible
	  BinaryNodeInterface<T> leftSubtreeRoot = currentNode.getLeftChild();
	  BinaryNodeInterface<T> rightChild = leftSubtreeRoot;
	  BinaryNodeInterface<T> priorNode = currentNode;
	  
	  while (rightChild.hasRightChild())
	  {
	    priorNode = rightChild;
	    rightChild = rightChild.getRightChild();
	  } // end while
	  
	  // rightChild contains the inorder predecessor and is the node to 
	  // remove; priorNode is its parent
	  
	  return new NodePair(rightChild, priorNode);
	} // end getNodeToRemove

	/** Task: Removes a node having at most one child. */
	// 27.40
	private void removeNode(BinaryNodeInterface<T> nodeToRemove, BinaryNodeInterface<T> parentNode)
	{		
		BinaryNodeInterface<T> childNode;
		
		if (nodeToRemove.hasLeftChild())
		  childNode = nodeToRemove.getLeftChild();
		else 
		  childNode = nodeToRemove.getRightChild();

		// Assertion: if nodeToRemove is a leaf, childNode is null  
		assert (nodeToRemove.isLeaf() && childNode == null) || !nodeToRemove.isLeaf();

		// if nodeToRemove is the root
		if (nodeToRemove == getRootNode())
		  setRootNode(childNode);
		  
		// else nodeToRemove is not the root;
		// link the parent of nodeToRemove to childNode, 
		// thereby deleting nodeToRemove
		else if (parentNode.getLeftChild() == nodeToRemove)
		  parentNode.setLeftChild(childNode);
		else
		  parentNode.setRightChild(childNode);
	} // end removeNode

	private class NodePair
	{
		private BinaryNodeInterface<T> first, second;
		
		public NodePair()
		{
			first = null;
			second = null;
		} // end default constructor

		public NodePair(BinaryNodeInterface<T> firstNode, BinaryNodeInterface<T> secondNode)
		{
			first = firstNode;
			second = secondNode;
		} // end constructor
		
		public BinaryNodeInterface<T> getFirst()
		{
			return first;
		} // end getFirst
		
		public BinaryNodeInterface<T> getSecond()
		{
			return second;
		} // end getSecond
	} // end NodePair

	// **************************************************
	// Override the specified methods below
	// **************************************************
	
	//Written by Andrew Beers 4/17
	// while there is a right child keep going to it until you get to the end, then return data (property of BST)
	public T getMax()
	{
		currentNode = getRootNode();
		while (currentNode.hasRightChild())
	    {
	      
	      currentNode = currentNode.getRightChild();
	    } // end while
	    return currentNode.getData();
	    
		
	
	}
	
	// while there is a left child keep going to it until you get to the end, then return data (property of BST)
	public T getMin()
	{
		currentNode = getRootNode();
		while (currentNode.hasLeftChild())
	    {
	      
	      currentNode = currentNode.getLeftChild();
	    } // end while
	    return currentNode.getData();
		
	
	}
	
	// always return true since we are in the BST class
	public boolean isBST()
	{
		
		return true;
	}

} // end BinarySearchTree