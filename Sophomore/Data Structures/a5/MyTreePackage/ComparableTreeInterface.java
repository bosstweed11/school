// CS 0445 Spring 2012
// Assignment 5
// Interface for the additional operations in the ComparableBinaryTree
package MyTreePackage;

public interface ComparableTreeInterface<T extends Comparable<? super T>> 
       extends TreeInterface<T>
{
	public T getMax();	// If the tree is not empty, return the maximum
						// value in the tree; otherwise return null

	public T getMin();	// If the tree is not empty, return the minimum
						// value in the tree; otherwise return null

	public boolean isBST();	// Return true if the the tree meets the
							// recursive definition of a BST; else
							// return false 
}