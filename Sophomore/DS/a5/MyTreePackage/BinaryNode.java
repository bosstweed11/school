// CS 0445 Spring 2012
// BinaryNode class for Assignment 5.  Add the methods specified in the
// assignment sheet so that this class works correctly.
package MyTreePackage;
/**
 * A class that represents nodes in a binary tree.
 * 
 * @author Frank M. Carrano
 * @version 2.0
 */
public class BinaryNode<T> implements BinaryNodeInterface<T>, 
                               java.io.Serializable
{
	private T data;
	private BinaryNode<T> left;
	private BinaryNode<T> right;
  
	public BinaryNode()
	{
		this(null); // call next constructor
	} // end default constructor
  
	public BinaryNode(T dataPortion)
	{
		this(dataPortion, null, null); // call next constructor
	} // end constructor

	public BinaryNode(T dataPortion, BinaryNode<T> leftChild,
                                   BinaryNode<T> rightChild)
	{
		data = dataPortion;
		left = leftChild;
		right = rightChild;
	} // end constructor
  
	public T getData()
	{
		return data;
	} // end getData
  
	public void setData(T newData)
	{
		data = newData;
	} // end setData
  
	public BinaryNodeInterface<T> getLeftChild()
	{
		return left;
	} // end getLeftChild
 
	public BinaryNodeInterface<T> getRightChild()
	{
		return right;
	} // end getRightChild

	public void setLeftChild(BinaryNodeInterface<T> leftChild)
	{
		left = (BinaryNode<T>)leftChild;
	} // end setLeftChild
  
	public void setRightChild(BinaryNodeInterface<T> rightChild)
	{
		right = (BinaryNode<T>)rightChild;
	} // end setRightChild	
	
	public boolean hasLeftChild()
	{
		return left != null;
	} // end hasLeftChild
  
	public boolean hasRightChild()
	{
		return right != null;
	} // end hasRightChild
	
	public boolean isLeaf()
	{
		return (left == null) && (right == null);
	} // end isLeaf
  
  	// 26.06
	public BinaryNodeInterface<T> copy()
	{
		BinaryNode<T> newRoot = new BinaryNode<T>(data);
	  
		if (left != null)
			newRoot.left = (BinaryNode<T>)left.copy();
	    if (right != null)
			newRoot.right = (BinaryNode<T>)right.copy();
	    
		return newRoot;	
	} // end copy

	// 26.11
	public int getHeight()
	{
		return getHeight(this); // call private getHeight
	} // end getHeight

	// 26.11
	private int getHeight(BinaryNode<T> node)
	{
		int height = 0;
	  
		if (node != null)
			height = 1 + Math.max(getHeight(node.left), getHeight(node.right));
	                          
		return height;
	} // end getHeight

	// 26.11
	public int getNumberOfNodes()
	{
		int leftNumber = 0;
		int rightNumber = 0;
	  
		if (left != null)
			leftNumber = left.getNumberOfNodes();
	    
		if (right != null)
			rightNumber = right.getNumberOfNodes();
	    
		return 1 + leftNumber + rightNumber;
	} // end getNumberOfNodes

	// **********************************
	// Add your methods below
	// **********************************
//Written by Andrew Beers 4/17
	
	//if the tree is 0 balanced, it is full, since we already have a 0 balanced method, we call it here
	public boolean isFull()
	{
	
		return isBalanced(0);
	
	}
	
	//returns if the tree has any subtrees of difference in height of at most k
	public boolean isBalanced(int k)
	{
		boolean l = true, r= true,bal=false;
		int le,ri,ans;
		le=getHeight(left);
		ri =getHeight(right);
		
		ans = le- ri;
		
		//test if the heights have a difference of at most k
		if(k >= Math.abs(ans))
		{
			//if so recursively call is balanced for all subtrees
			if(left!=null)
				l =left.isBalanced(k);
			if(right!=null)
				r =right.isBalanced(k);
				
			if(l&&r)
				bal = true;
			else 
				bal = false;
				
		}
		else
			//else it is not k balance so return false
			bal = false;
			
		return bal;
		
	}

} // end BinaryNode