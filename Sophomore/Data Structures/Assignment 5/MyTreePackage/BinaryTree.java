// CS 0445 Spring 2012
// Modified BinaryTree class.  I have implemented one of the required methods
// for you -- see (far) below.  You must complete the implementations for the 
// remaining methods.

package MyTreePackage;
import java.util.*;
import java.io.*;
import java.util.Iterator;
import java.util.NoSuchElementException;
import StackAndQueuePackage.*;

/**
 * A class that implements the ADT binary tree.
 * 
 * @author Frank M. Carrano
 * @version 2.0
 */
public class BinaryTree<T> implements BinaryTreeInterface<T>, java.io.Serializable
{
  private BinaryNodeInterface<T> root;
  private StackInterface<BinaryNodeInterface<T>> nodeStack;
  
  
  public BinaryTree()
  {
    root = null;
  } // end default constructor
  
  public BinaryTree(T rootData)
  {
    root = new BinaryNode<T>(rootData);
  } // end constructor
  
  public BinaryTree(T rootData, BinaryTree<T> leftTree, 
                                BinaryTree<T> rightTree)
  {
    privateSetTree(rootData, leftTree, rightTree);
  } // end constructor
  
  public void setTree(T rootData)
  {
    root = new BinaryNode<T>(rootData);
  } // end setTree
  
  public void setTree(T rootData, BinaryTreeInterface<T> leftTree,
                                  BinaryTreeInterface<T> rightTree)
  {
    privateSetTree(rootData, (BinaryTree<T>)leftTree, 
                             (BinaryTree<T>)rightTree);
  } // end setTree

	// 26.08
	private void privateSetTree(T rootData, BinaryTree<T> leftTree, 
	                                        BinaryTree<T> rightTree)
	{
	  root = new BinaryNode<T>(rootData);
	  
	  if ((leftTree != null) && !leftTree.isEmpty())
	    root.setLeftChild(leftTree.root);
	    
	  if ((rightTree != null) && !rightTree.isEmpty())
	  {
	    if (rightTree != leftTree)
	      root.setRightChild(rightTree.root);
	    else
	      root.setRightChild(rightTree.root.copy());
	  } // end if
	  
	  if ((leftTree != null) && (leftTree != this))
	    leftTree.clear(); 
	    
	  if ((rightTree != null) && (rightTree != this))
	    rightTree.clear();
	} // end privateSetTree


	private BinaryNode<T> copyNodes() // not essential
	{
		return (BinaryNode<T>)root.copy();
		
	} // end copyNodes

	// 26.09
	public T getRootData()
	{
	  T rootData = null;
	  
	  if (root != null)
	    rootData = root.getData();
	    
	  return rootData;
	} // end getRootData

	// 26.09
	public boolean isEmpty()
	{
	  return root == null;
	} // end isEmpty

	// 26.09
	public void clear()
	{
	  root = null;
	} // end clear

	// 26.09
	public void setRootData(T rootData)
	{
	  root.setData(rootData);
	} // end setRootData

	// 26.09
	// I changed this from protected to public
	public void setRootNode(BinaryNodeInterface<T> rootNode)
	{
	  root = rootNode;
	} // end setRootNode

	// 26.09
	protected BinaryNodeInterface<T> getRootNode()
	{
	  return root;
	} // end getRootNode

	// 26.10
	public int getHeight()
	{
	  return root.getHeight();
	} // end getHeight

	// 26.10
	public int getNumberOfNodes()
	{
	  return root.getNumberOfNodes();
	} // end getNumberOfNodes

	// 26.12
	public void inorderTraverse() 
	{
	  inorderTraverse(root);
	} // end inorderTraverse

	private void inorderTraverse(BinaryNodeInterface<T> node) 
	{
	  if (node != null)
	  {
	    inorderTraverse(node.getLeftChild());
	    System.out.println(node.getData());
	    inorderTraverse(node.getRightChild());
	  } // end if
	} // end inorderTraverse

	public Iterator<T> getPreorderIterator()
	{
		return new PreorderIterator();	
	} // end getPreorderIterator

	// 26.13
	public Iterator<T> getInorderIterator()
	{
		return new InorderIterator();	
	} // end getInorderIterator
	
	public Iterator<T> getPostorderIterator()
	{
		return new PostorderIterator();	
	} // end getPostorderIterator

	public Iterator<T> getLevelOrderIterator()
	{
		return new LevelOrderIterator();	
	} // end getLevelOrderIterator

/*
	// 26.14 ITERATIVE
	public void inorderTraverse()
	{
	  StackInterface<BinaryNodeInterface<T>> nodeStack = 
	                           new LinkedStack<BinaryNodeInterface<T>>();
	  BinaryNodeInterface<T> currentNode = root;
	  
	  while (!nodeStack.isEmpty() || (currentNode != null))
	  {
	    // find leftmost node with no left child
	    while (currentNode != null)
	    {
	      nodeStack.push(currentNode);
	      currentNode = currentNode.getLeftChild();
	    } // end while
	    
	    // visit leftmost node, then traverse its right subtree
	    if (!nodeStack.isEmpty())
	    {
	      BinaryNodeInterface<T> nextNode = nodeStack.pop();
	      assert nextNode != null; // since nodeStack was not empty
	                               // before the pop
	      System.out.println(nextNode.getData());
	      currentNode = nextNode.getRightChild();
	    } // end if
	  } // end while
	} // end inorderTraverse

	public void inorderTraverse() // Q5
	{
		StackInterface<BinaryNode<T>> nodeStack = new LinkedStack<BinaryNode<T>>();
		BinaryNode<T> currentNode = (BinaryNode<T>)root;//(BinaryNode<T>)
		
		while (!nodeStack.isEmpty() || (currentNode != null))
		{
			while (currentNode != null)
			{
				nodeStack.push(currentNode);
				currentNode = (BinaryNode<T>)currentNode.getLeftChild();	//(BinaryNode<T>)
			} // end while

			if (!nodeStack.isEmpty())
			{
				BinaryNode<T> nextNode = nodeStack.pop();				
				assert nextNode != null; // since stack was not empty before pop
				System.out.print(nextNode.getData() + " ");
				currentNode = (BinaryNode<T>)nextNode.getRightChild();	//(BinaryNode<T>)
			}
		}
		System.out.println();
	} // end inorderTraverse

*/
	private class PreorderIterator implements Iterator<T>
	{
		private StackInterface<BinaryNodeInterface<T>> nodeStack;
		
		public PreorderIterator()
		{
			nodeStack = new LinkedStack<BinaryNodeInterface<T>>();
			if (root != null)
				nodeStack.push(root);
		} // end default constructor
		
		public boolean hasNext() 
		{
			return !nodeStack.isEmpty();
		} // end hasNext
		
		public T next()
		{
			BinaryNodeInterface<T> nextNode;
			
			if (hasNext())
			{
				nextNode = nodeStack.pop();
				BinaryNodeInterface<T> leftChild = nextNode.getLeftChild();
				BinaryNodeInterface<T> rightChild = nextNode.getRightChild();
				
				// push into stack in reverse order of recursive calls
				if (rightChild != null)
					nodeStack.push(rightChild);
					
				if (leftChild != null)
					nodeStack.push(leftChild);
			}
			else
			{
				throw new NoSuchElementException();
			}
		
			return nextNode.getData();
		} // end next
	
		public void remove()
		{
			throw new UnsupportedOperationException();
		} // end remove
	} // end PreorderIterator

	// 26.15
	private class InorderIterator implements Iterator<T>
	{
	  private StackInterface<BinaryNodeInterface<T>> nodeStack;
	  private BinaryNodeInterface<T> currentNode;
	  
	  public InorderIterator()
	  {
	    nodeStack = new LinkedStack<BinaryNodeInterface<T>>();
	    currentNode = root;
	  } // end default constructor
	  
	  public boolean hasNext() 
	  {
	    return !nodeStack.isEmpty() || (currentNode != null);
	  } // end hasNext
	  
	  public T next()
	  {
	    BinaryNodeInterface<T> nextNode = null;
	    
	    // find leftmost node with no left child
	    while (currentNode != null)
	    {
	      nodeStack.push(currentNode);
	      currentNode = currentNode.getLeftChild();
	    } // end while
	    
	    // get leftmost node, then move to its right subtree
	    if (!nodeStack.isEmpty())
	    {
	      nextNode = nodeStack.pop();
	      assert nextNode != null; // since nodeStack was not empty
	                               // before the pop
	      currentNode = nextNode.getRightChild();
	    }
	    else
	      throw new NoSuchElementException();
	      
	    return nextNode.getData(); 
	  } // end next
	  
	  public void remove()
	  {
	    throw new UnsupportedOperationException();
	  } // end remove
	} // end InorderIterator

	private class PostorderIterator implements Iterator<T>
	{
		private StackInterface<BinaryNodeInterface<T>> nodeStack;
		private BinaryNodeInterface<T> currentNode;
		
		public PostorderIterator()
		{
			nodeStack = new LinkedStack<BinaryNodeInterface<T>>();
			currentNode = root;
		} // end default constructor
		
		public boolean hasNext() 
		{
			return !nodeStack.isEmpty() || (currentNode != null);
		} // end hasNext
		
		public T next()
		{
			boolean foundNext = false;
			BinaryNodeInterface<T> leftChild, rightChild, nextNode = null;
			
			// find leftmost leaf
			while (currentNode != null)
			{
				nodeStack.push(currentNode);
				leftChild = currentNode.getLeftChild();
				if (leftChild == null)	
					currentNode = currentNode.getRightChild();
				else
					currentNode = leftChild;
			} // end while
			
			// stack is not empty either because we just pushed a node, or
			// it wasn't empty to begin with since hasNext() is true.
			// But Iterator specifies an exception for next() in case
			// hasNext() is false.
			
			if (!nodeStack.isEmpty())
			{
				nextNode = nodeStack.pop();
				// nextNode != null since stack was not empty before pop

				BinaryNodeInterface<T> parent = nodeStack.peek();
				
				if (parent != null && nextNode == parent.getLeftChild())
					currentNode = parent.getRightChild();	
				else
					currentNode = null;
			}
			else
			{
				throw new NoSuchElementException();
			} // end if
			
			return nextNode.getData();
		} // end next

		public void remove()
		{
			throw new UnsupportedOperationException();
		} // end remove
	} // end PostorderIterator
	
	private class LevelOrderIterator implements Iterator<T>
	{
		private QueueInterface<BinaryNodeInterface<T>> nodeQueue;
		
		public LevelOrderIterator()
		{
			nodeQueue = new LinkedQueue<BinaryNodeInterface<T>>();
			if (root != null)
				nodeQueue.enqueue(root);
		} // end default constructor
		
		public boolean hasNext() 
		{
			return !nodeQueue.isEmpty();
		} // end hasNext
		
		public T next()
		{
			BinaryNodeInterface<T> nextNode;
			
			if (hasNext())
			{
				nextNode = nodeQueue.dequeue();
				BinaryNodeInterface<T> leftChild = nextNode.getLeftChild();
				BinaryNodeInterface<T> rightChild = nextNode.getRightChild();
				
				// add to queue in order of recursive calls
				if (leftChild != null)
					nodeQueue.enqueue(leftChild);

				if (rightChild != null)
					nodeQueue.enqueue(rightChild);
			}
			else
			{
				throw new NoSuchElementException();
			}
		
			return nextNode.getData();
		} // end next
	
		public void remove()
		{
			throw new UnsupportedOperationException();
		} // end remove
	} // end LevelOrderIterator


	// *******************************
	// Assignment 5 Methods Start Here
	// *******************************
	
//Written by Andrew Beers 4/17
	public void saveInorder(String fileName)
	{
		try
		{
			ObjectOutputStream OS = new ObjectOutputStream(
				new FileOutputStream(fileName));
			int n = getNumberOfNodes();
			OS.writeInt(n);
			RecWriteTree(OS, (BinaryNode<T>)getRootNode());
			OS.close();
		}
		catch (IOException e)
		{
			System.out.println("Writing problem");
		}
	}

	public void RecWriteTree(ObjectOutputStream OS, BinaryNode<T> node)
	{
		if (node != null)
		{
			try
			{
				RecWriteTree(OS, (BinaryNode<T>) node.getLeftChild());
				OS.writeObject(node.getData());
				RecWriteTree(OS, (BinaryNode<T>) node.getRightChild());
			}
			catch (IOException e)
			{
				System.out.println("Rec Writing Problem" + e);
			}
		}
	}
	
	//is full method, call binary node method for it.
	public boolean isFull()
	{
		BinaryNode<T> root = (BinaryNode<T>)getRootNode();
		return root.isFull();
		
	}
	//is balanced method, call binary node method for it.
	public boolean isBalanced(int k)
	{
		BinaryNode<T> root = (BinaryNode<T>)getRootNode();
		return root.isBalanced(k);
	
	}
	// Build in order method, recursively builds a 1 balanced binary search tree
	public void buildInorder(String fileName) 
	{
		
		BinaryTree<T> L,R;
		T data=null;
		int size;
		try
		{
			ObjectInputStream IS = new ObjectInputStream(
				new FileInputStream(fileName));
			//read in size
			size = IS.readInt();
			
			//recursive build method
			L=RecBuild(IS,size/2);
			
			try
			{
				data =(T) IS.readObject();
				
			}
			catch(Exception e)
			{
				System.out.println("Writing problem");
			}
			
			//recursively build right subtree 
			R=RecBuild(IS,size/2);
			privateSetTree(data,L,R);
			IS.close();
		}
		catch (IOException e)
		{
			System.out.println("Writing problem");
		}
	
	}
	
	//recursive building method
	private BinaryTree<T> RecBuild(ObjectInputStream IS, int n) 
	{
			BinaryTree<T> L,R,M;
			T data=null;
			
			//if size one, then the node is a leaf node, so just return the leaf
			if(n==0)
			{
				
				return null;
				
			}
			else if(n<=1)
			{
				try
				{
					data =(T) IS.readObject();
					
				}
				catch(Exception e)
				{
					System.out.println("Writing problem");
				}
				M = new BinaryTree<T>(data);
				return M;
			}
			//else it is larger, so continue to cut down the size to the left, 
			else
			{
				//the left subtree always has n/2 nodes of the root nodes descendants
				L = RecBuild(IS,n/2);
				try
				{
					data =(T) IS.readObject();
					
				}
				catch(Exception e)
				{
					System.out.println("Writing problem");
				}
				
				//the right subtree always has (n-1)/2 nodes of the root nodes descendants
				R = RecBuild(IS,(n-1)/2);
				//set root as middle data
				M = new BinaryTree<T>(data);
				//return tree as root and left and right subtrees
				M.setTree(data,L,R);
				return M;
			}
			
	}
} // end BinaryTree