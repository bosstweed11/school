// CS 0445 Spring 2012 Assignment 5 Test Program
// This program should run as is with your program files.
// See the file A5Out.txt for the output -- your output should match
// that shown in A5Out.txt exactly.
import java.util.*;
import MyTreePackage.*;
public class Assig5
{
	public static void main(String [] args)
	{
		BinaryNode<Integer> root = init1();
		ComparableBinaryTree<Integer> T1 = new ComparableBinaryTree<Integer>();
		// Note that the setRootNode() method is originally defined to be
		// protected, and would not be accessible outside the MyTreePackage.
		// However, for the purposes of this demo, the setRootNode() method
		// should be made public.
		T1.setRootNode(root);
		testTree(T1);
		
		root = init2();
		ComparableBinaryTree<Integer> T2 = new ComparableBinaryTree<Integer>();
		T2.setRootNode(root);
		testTree(T2);

		T2.saveInorder("BST.dat");

		BinarySearchTree<Integer> T3 = new BinarySearchTree<Integer>();
		T3.buildInorder("BST.dat");
		testTree(T3);

		root = init3();
		ComparableBinaryTree<Integer> T4 = new ComparableBinaryTree<Integer>();
		T4.setRootNode(root);
		testTree(T4);

		BinarySearchTree<Integer> T5 = new BinarySearchTree<Integer>();
		for (int i = 1; i <= 15; i++)
		{
			T5.add(new Integer(i));
		}
		testTree(T5);
		T5.saveInorder("BST2.dat");
		T5.buildInorder("BST2.dat");
		testTree(T5);

		BinarySearchTree<String> T6 = new BinarySearchTree<String>();
		initBST(T6);
		testTree(T6);
		T6.saveInorder("BST3.dat");
		T6.setRootNode(null);
		T6.buildInorder("BST3.dat");
		testTree(T6);
	}

	// This method tests the methods you have written. Note that, due
	// to inheritance, this will work with a ComparableBinaryTree or
	// a BinarySearchTree (from your methods -- not that Author's)
	public static <T extends Comparable<? super T>> void testTree(ComparableBinaryTree<T> tree)
	{
		int height = tree.getHeight();
		System.out.println("Height: " + height);
		int numNodes = tree.getNumberOfNodes();
		System.out.println("Nodes: " + numNodes);
		boolean test;
		try 
		{
			T extreme = tree.getMax();
			System.out.println("Max is: " + extreme);
		}
		catch (RuntimeException e)
		{
			System.out.println(e);
		}
		try
		{
			T extreme = tree.getMin();
			System.out.println("Min is: " + extreme);
		}
		catch (RuntimeException e)
		{
			System.out.println(e);
		}

		test = tree.isFull();
		if (test)
			System.out.println("Tree is FULL");
		else
			System.out.println("Tree is NOT FULL");
		test = tree.isBST();
		if (test)
			System.out.println("Tree is a BST");
		else
			System.out.println("Tree is NOT A BST");
		for (int i = 0; i < 3; i++)
		{
			boolean bal = tree.isBalanced(i);
			if (bal)
				System.out.println("Tree is " + i + " balanced");
			else
				System.out.println("Tree is NOT " + i + " balanced");
		}
		Iterator<T> I = tree.getInorderIterator();
		System.out.println("Inorder the data is:");
		IterateOn(I);
		System.out.println();
	}

	public static <T> void IterateOn(Iterator<T> I)
	{
		while (I.hasNext())
		{
			T curr = I.next();
			System.out.print(curr + " ");
		}
		System.out.println();
	}

	// This binary tree is not full and not a BST
	public static BinaryNode<Integer> init1()
	{
		BinaryNode<Integer> temp1 = new BinaryNode<Integer>(new Integer(60));
		BinaryNode<Integer> temp2 = new BinaryNode<Integer>(new Integer(30));
		BinaryNode<Integer> temp3 = new BinaryNode<Integer>(new Integer(80), temp1, temp2);
		temp1 = new BinaryNode<Integer>(new Integer(20));
		temp2 = new BinaryNode<Integer>(new Integer(15), temp1, temp3);
		temp3 = temp2;
		temp2 = new BinaryNode<Integer>(new Integer(50));
		temp1 = new BinaryNode<Integer>(new Integer(40), null, temp2);
		temp2 = new BinaryNode<Integer>(new Integer(75));
		BinaryNode<Integer> temp4 = new BinaryNode<Integer>(new Integer(65), temp1, temp2);
		temp1 = new BinaryNode<Integer>(new Integer(90), temp4, temp3);
		return temp1;
	}

	// This tree will be a binary search tree (BST), but not full. 
	public static BinaryNode<Integer> init2()
	{
		BinaryNode<Integer> temp1 = new BinaryNode<Integer>(new Integer(17));
		BinaryNode<Integer> temp2 = new BinaryNode<Integer>(new Integer(20), temp1, null);
		temp1 = new BinaryNode<Integer>(new Integer(10));
		BinaryNode<Integer> temp3 = new BinaryNode<Integer>(new Integer(15), temp1, temp2);
		temp2 = new BinaryNode<Integer>(new Integer(30));
		temp1 = new BinaryNode<Integer>(new Integer(25), temp3, temp2);
		temp3 = temp1;

		temp1 = new BinaryNode<Integer>(new Integer(55));
		temp2 = new BinaryNode<Integer>(new Integer(70));
		BinaryNode<Integer> temp4 = new BinaryNode<Integer>(new Integer(60), temp1, temp2);
		temp1 = new BinaryNode<Integer>(new Integer(80));
		temp2 = new BinaryNode<Integer>(new Integer(85), temp1, null);
		temp1 = new BinaryNode<Integer>(new Integer(75), temp4, temp2);

		temp4 = new BinaryNode<Integer>(new Integer(50), temp3, temp1);
		return temp4;
	}

	// This tree is full but not a BST
	public static BinaryNode<Integer> init3()
	{
		BinaryNode<Integer> temp1 = new BinaryNode<Integer>(new Integer(5));
		BinaryNode<Integer> temp2 = new BinaryNode<Integer>(new Integer(15));
		BinaryNode<Integer> temp3 = new BinaryNode<Integer>(new Integer(10), temp1, temp2);

		temp1 = new BinaryNode<Integer>(new Integer(40));
		temp2 = new BinaryNode<Integer>(new Integer(80));
		BinaryNode<Integer> temp4 = new BinaryNode<Integer>(new Integer(70), temp1, temp2);

		temp3 = new BinaryNode<Integer>(new Integer(50), temp3, temp4);
		return temp3;
	}

	public static void initBST(BinarySearchTree<String> T)
	{
		T.add(new String("Outrageous"));
		T.add(new String("Zany"));
		T.add(new String("Bogus"));
		T.add(new String("Wacky"));
		T.add(new String("Weasel"));
		T.add(new String("Esoteric"));
		T.add(new String("Zippy"));
		T.add(new String("Uncertainty"));
		T.add(new String("Melancholy"));
	}

}
