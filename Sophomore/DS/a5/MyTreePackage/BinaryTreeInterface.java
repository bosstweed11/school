// CS 0445 Spring 2012
// See the methods for Assignment 5 below.
package MyTreePackage;
/**
 * An interface for the ADT binary tree.
 * 
 * @author Frank M. Carrano
 * @version 2.0
 */
public interface BinaryTreeInterface<T> extends TreeInterface<T>, 
                                            TreeIteratorInterface<T>
{
  /** Task: Sets an existing binary tree to a new one-node binary tree.
   *  @param rootData   an object that is the data in the new treeÕs root
   */
  public void setTree(T rootData);
  
  /** Task: Sets an existing binary tree to a new binary tree.
   *  @param rootData   an object that is the data in the new treeÕs root
   *  @param leftTree   the left subtree of the new tree
   *  @param rightTree  the right subtree of the new tree */
  public void setTree(T rootData, BinaryTreeInterface<T> leftTree,
                                  BinaryTreeInterface<T> rightTree);

	// ************************************************
	// Methods added for CS 0445 Assignment 5 are below
	// ************************************************
	public boolean isFull();			// See BinaryNodeInterface
	public boolean isBalanced(int k);	// See BinaryNodeInterface
	public void saveInorder(String fileName); // Save the data in the BT
		// to file "fileName" using an inorder traversal.  Format
		// the file in the following way: It first contains an int 
		// representing the number of nodes in the tree, followed 
		// by the actual objects from the tree (inorder).  Use the 
		// writeObject() method to write the objects.
	public void buildInorder(String filename); // Build a balanced BT
		// from the file "fileName".  Assume the first line of the
		// file has an integer, N, indicating the number of values
		// to follow.  The remaining N lines of the file contain N
		// values.  The order of the values in the file should be
		// preserved by the tree (i.e. an inorder traversal should
		// show the data in the order stored in the file). Also
		// if N = 2^K-1 for some K, the tree MUST be a FULL tree.
		// This method MUST build the tree recursively (hint: have
		// it call a recursive method).  Use the readObject()
		// method to read the objects (so the file can contain any
		// type of Serializable objects).

} // end BinaryTreeInterface