//Written by Andrew Beers 4/17
package MyTreePackage;

import java.util.*;
import java.io.*;
import java.util.Iterator;
import java.util.NoSuchElementException;
import StackAndQueuePackage.*;
public class ComparableBinaryTree<T extends Comparable<? super T>> extends BinaryTree<T> implements ComparableTreeInterface<T>, java.io.Serializable
{
	
	protected BinaryNodeInterface<T> currentNode=getRootNode();
	// constructor
	public ComparableBinaryTree()
  	{
    	super();
  	} // end default constructor
  	
  	//constructor
	public ComparableBinaryTree(T rootEntry)
  	{
    	super();
    	setRootNode(new BinaryNode<T>(rootEntry));
  	} // end constructor
  	
  	// use a post order iterator, to get the max
	public T getMax()
	{
		currentNode = getRootNode();
		Iterator<T> P = getPostorderIterator();
		T max,data,end=currentNode.getData();
		data = currentNode.getData();
		
		// set max as root node data, if any node is larger, replace max, since post iterator returns root last, when root comes back up, we are done
		max = data;
		do
		{
			data = P.next();
			
			if(data.compareTo(max) > 0)
			{
				
				max = data;
			}
		}while(data.compareTo(end) != 0);
	
		
		return max;
	}
	// use a post order iterator, to get the min
	public T getMin()
	{
		currentNode=getRootNode();
		Iterator<T> P = getPostorderIterator();
		T min,data,end=currentNode.getData();
		data = currentNode.getData();
		
		// set min as root node data, if any node is smaller, replace max, since post iterator returns root last, when root comes back up, we are done
		min = data;
		do
		{
			data = P.next();
			
			if(data.compareTo(min) < 0)
				min = data;
				
		}while(data.compareTo(end) != 0);
		
		return min;
		
	}
	
	
	//use inorder iterator for bst
	public boolean isBST()
	{
		ArrayList<T> A = new ArrayList<T>(25);
		Iterator<T> P = getInorderIterator();
		T data,end = currentNode.getData();
		int k = currentNode.getNumberOfNodes();
		boolean bst = true;
		
		
		//load all tree values into an array
		for(int i=0;i<k;i++)
		{
			A.add(P.next());
		}
		
		//if the array is not sorted, then return false(property of BST)
		for(int i=0;i<k-1;i++)
		{
			if(A.get(i).compareTo(A.get(i+1))>0)
			{
				bst = false;
			}
			
		}
		
		return bst;
	}
		
}



