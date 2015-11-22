// CS 0445 Spring 2012
// See my additions to the author's interface below.
package MyTreePackage;
/**
 * An interface for a node in a binary tree.
 * 
 * @author Frank M. Carrano
 * @version 2.0
 */
interface BinaryNodeInterface<T>
{
  /** Task: Retrieves the data portion of the node.
   *  @return the object in the data portion of the node */
  public T getData();
  
  /** Task: Sets the data portion of the node.
   *  @param newData  the data object */
  public void setData(T newData);
  
  /** Task: Retrieves the left child of the node.
   *  @return the node that is this nodeÕs left child */
  public BinaryNodeInterface<T> getLeftChild();
  
  /** Task: Retrieves the right child of the node.
   *  @return the node that is this nodeÕs right child */
  public BinaryNodeInterface<T> getRightChild();
  
  /** Task: Sets the nodeÕs left child to a given node.
   *  @param leftChild  a node that will be the left child */
  public void setLeftChild(BinaryNodeInterface<T> leftChild);
  
  /** Task: Sets the nodeÕs right child to a given node.
   *  @param rightChild  a node that will be the right child */
  public void setRightChild(BinaryNodeInterface<T> rightChild);
  
  /** Task: Detects whether the node has a left child.
   *  @return true if the node has a left child */
  public boolean hasLeftChild();

  /** Task: Detects whether the node has a right child.
   *  @return true if the node has a right child */
  public boolean hasRightChild();
  
  /** Task: Detects whether the node is a leaf.
   *  @return true if the node is a leaf */
  public boolean isLeaf();
  
  /** Task: Counts the nodes in the subtree rooted at this node.
   *  @return the number of nodes in the subtree rooted at this node */
  public int getNumberOfNodes();
  
	/** Task: Computes the height of the subtree rooted at this node.
	   *  @return the height of the subtree rooted at this node */
	public int getHeight();

  /** Task: Copies the subtree rooted at this node.
   *  @return the root of a copy of the subtree rooted at this node */
  public BinaryNodeInterface<T> copy();

  // **********************************************
  // Methods added for CS445 Assignment 5 are below
  // **********************************************
  public boolean isFull();	// If the tree is a full tree, return true
							// Otherwise, return false.  See notes for
							// definition of full.
  public boolean isBalanced(int k);	// Return true if 1) the difference
		// in height between the left and right subtrees is at most k,
		// and 2) the left and right subtrees are both recursively
		// k-balanced; return false otherwise

} // end BinaryNodeInterface

