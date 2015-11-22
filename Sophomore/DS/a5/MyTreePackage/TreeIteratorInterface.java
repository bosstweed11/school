// 25.17
package MyTreePackage;
import java.util.Iterator;
/**
 * An interface for iterators that perform traversals of a tree.
 * 
 * @author Frank M. Carrano
 * @version 2.0
 */
public interface TreeIteratorInterface<T>
{
  public Iterator<T> getPreorderIterator();
  public Iterator<T> getPostorderIterator();
  public Iterator<T> getInorderIterator();
  public Iterator<T> getLevelOrderIterator();
} // end TreeIteratorInterface