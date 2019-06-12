#include "list.h"

template <class TYPE>
class List
{
  friend ostream &operator<<(ostream &os, const List<TYPE> &l);
  friend istream &operator>>(istream &is, List<TYPE> &l);

  friend int operator==(const List<TYPE> &l1, const List<TYPE> &l2);//equality
  friend int operator!=(const List<TYPE> &l1, const List<TYPE> &l2);//in-equality
  public:
	  List();
	  ~List();
	  List(const List<TYPE> &l);

	  List<TYPE> &operator=(const List<TYPE> &l); //assignment
	  TYPE &operator()(const int &n); //direct reference to item n

	  int inlist(const TYPE &element) const;//1 if element member of list

	  int length() const;//returns the number of elements in a list

	  int insert(const TYPE &element, const int &i = 1);//insert@ position i
	  void prepend(const TYPE &element);//insert an element at front of list
	  void append(const TYPE &element);//insert an element at end of list

	  int remove(const TYPE &element);//return 1 if successful
	  void remove();//removes ALL elements of the list

  protected:
	  int num_elements;
	  Element<TYPE> *list, *tail;

}; //end class 


template<class TYPE>
List<TYPE>::List()
{
  num_elements = 0;
  list = NULL;
  tail = NULL;
}

template<class TYPE>
List<TYPE>::List(const List<TYPE> &l)
{
  Element<TYPE> *pt = l.list;  // list is the list to be copied

  num_elements = 0;
  list = NULL;
  tail = NULL;

  while ( pt != NULL )
    { // add all elements to this
      append(pt->element);
      pt = pt->next;
    }//end while

}

template<class TYPE>
List<TYPE>::~List()
{
  remove();
}

template<class TYPE>
void List<TYPE>::remove()
{
  Element<TYPE> *tmp, *ptr;

  ptr = list;
  while (ptr != NULL)
  {
    tmp = ptr;
    ptr = ptr->next;
    delete tmp;
  }//end while

  list = NULL;
  tail = NULL;
  num_elements = 0;
}


template<class TYPE>
List<TYPE> &List<TYPE>::operator=(const List<TYPE> &l) //assignment
{
  if (this != &l)   // check for: a = a;
    {        
      Element<TYPE> *pt = l.list;  // list is righthand side (rhs) of "="

      this->remove();         // remove all old members of lhs

      while ( pt != NULL )
        { // add all elements to this
          this->append(pt->element);
          pt = pt->next;
        }//end while

    }//end then

  return *this;
}

template<class TYPE>
TYPE &List<TYPE>::operator()(const int &n) 
{
  int i;
  Element<TYPE> *ptr = this->list;

  for (i=1;i<n;i++)
    ptr = ptr->next;

  return ptr->element;
}

template<class TYPE>
int List<TYPE>::insert(const TYPE &element, const int &i)
{
  if ((i > (num_elements + 1)) || (i < 1))
    return 0;

  if (i == 1)
    {
      prepend(element);
      return 1;
    }

  if (i == (num_elements + 1))
    {
      append(element);
      return 1;
    }

  if (i == num_elements)
    {
      TYPE tmp;

      tmp = (*this)(i);
      append(tmp);
      (*this)(i) = element;

      return 1;
    }

  int j;
  Element<TYPE> *ptr = list;

  for (j=1;j<i;j++)
	ptr = ptr->next;

  Element<TYPE> *new_element = new Element<TYPE>(ptr->element,ptr->next);
  ptr->next = new_element;
  (*this)(i) = element;

  num_elements++;

  return 1;

} 

template<class TYPE>
void List<TYPE>::prepend(const TYPE &element)
{
  //make new element ptg to list
  Element<TYPE> *new_element = new Element<TYPE>(element,list);

  list = new_element;
  if (!(num_elements++))
    tail = list;

}


template<class TYPE>
void List<TYPE>::append(const TYPE &element)
{
  //make new element ptg to NULL
  Element<TYPE> *new_element = new Element<TYPE>(element,NULL);

  if (!(num_elements++))
    {
      list = new_element;
      tail = new_element;
    }
  else 
    {
      tail->next = new_element; //make list now include new element
      tail = new_element;
    }

}

template<class TYPE>
int List<TYPE>::remove(const TYPE &key) //return 1 if successful
{
  Element<TYPE> *nxt, *prev=NULL, *curr=list;

  if (length()==0)
    return 0;
 
  //list has at least one element
  ///////////////////////////
  nxt = curr->next;
  while ((nxt != NULL) && (curr->element != key))
    {
      prev = curr;
      curr = nxt;
      nxt = nxt->next;
    }//end while

  if (curr->element == key) // found desired element
    {
      if (curr == list) //at first element of list
	{
	  list = nxt;
	  if (num_elements == 1) //list has only one element
	    tail = NULL;
        }
      else if (nxt == NULL) //at last element of list
	{
	  prev->next = NULL;
	  tail = prev;
        }
      else //somewhere in middle of list
	prev->next = nxt;

      delete curr; 
      num_elements--;
      return 1;
    }//then
  else
    return 0;
  
}



template<class TYPE>
int operator==(const List<TYPE> &l1, const List<TYPE> &l2) //equality
{
  Element<TYPE> *ptr1, *ptr2;

  if (l1.length() != l2.length()) //quick check
    return 0;
  
  //both lists must be same size

  for (ptr1 = l1.list , ptr2 = l2.list;
       ptr1 != NULL;
       ptr1 = ptr1->next, ptr2 = ptr2->next )
  
      //every element must be in each list in order

      if (!(ptr1->element == ptr2->element))
        return 0;

  return 1;
}

template<class TYPE>
int operator!=(const List<TYPE> &l1, const List<TYPE> &l2) //in-equality
{
  return (!(l1==l2));
}

template<class TYPE>
int List<TYPE>::inlist(const TYPE &element) const 
{
  Element<TYPE> *ptr = list;

  while (ptr != NULL)
    {
      if (ptr->element == element)
	return 1;

      ptr = ptr->next;
    }//end while

  //is not a member
  return 0;
}

template<class TYPE>
int List<TYPE>::length() const //returns the number of elements in a list
{
  return num_elements;
}


template<class TYPE>
ostream &operator<<(ostream &os, const List<TYPE> &l)
{
  Element<TYPE> *ptr = l.list;

  os<<"[ ";
  for ( ;ptr != NULL;ptr = ptr->next)
    os<<ptr->element<<" ";
  os<<"]";

  return os;

}


template<class TYPE>
istream &operator>>(istream &is, List<TYPE> &l)
{
  char ch;
  TYPE e;

  is>>ch;  //read '['
  if (ch != '[')
    {
      cerr<<"\nNot in proper list format"<<endl;
      return is;
    }

  l.remove(); //clean out receiveing list
  is>>ch;
  while (ch != ']')
    {
      is.putback(ch);
      is>>e;
      l.insert(e);
      is>>ch;
    }//end while

  //have just read in ']'
  return is;
}

#include "stdafx.h"