
/****************************
  Howard Gleason
  CSCE 200

  These two classes work together to implement a set
  of any type of elements.  The set, however, must be
  of all one type; not a strict mathematical interpretation
  of an actual set.

  As long as the data type to be elements in the set has
  the '==', '>>', and '<<' operators defined, this template will work.

  The sets in the istream must be in a readable form for
  the type of set declared.  For example:

    Set<char> will recognize {abc}, { a b c }, {a bc }, ...
	      as all the same set.

    Set<int>  will recognize {1 2 3} and { 1 2 3 } 
	      as the same but {123} is completely different
****************************/
template<class TYPE> class Set;
template <class TYPE>
class Elmt
{
  friend class Set<TYPE>;
  friend ostream &operator<<(ostream &os, const Set<TYPE> &s);
  friend istream &operator>>(istream &is, Set<TYPE> &s);

  friend Set<TYPE> &operator+(const Set<TYPE> &s1, const Set<TYPE> &s2);//union
  friend Set<TYPE> &operator-(const Set<TYPE> &s1, const Set<TYPE> &s2);//sub
  friend Set<TYPE> &operator&(const Set<TYPE> &s1, const Set<TYPE> &s2);//inter

  friend void crossprod(ostream &os, const Set<TYPE> &s1,
				     const Set<TYPE> &s2);//x-pdt
  friend int operator==(const Set<TYPE> &s1, const Set<TYPE> &s2);//equality
  friend int operator!=(const Set<TYPE> &s1, const Set<TYPE> &s2);//in-equality
  friend int operator>(const Set<TYPE> &s1, const Set<TYPE> &s2);//prop superset
  friend int operator<(const Set<TYPE> &s1, const Set<TYPE> &s2);//proper subset
  friend int operator>=(const Set<TYPE> &s1, const Set<TYPE> &s2);//superset
  friend int operator<=(const Set<TYPE> &s1, const Set<TYPE> &s2);//subset


  public:

  private:
	  Elmt(const TYPE &e, Elmt<TYPE> *e_ptr);
	  ~Elmt();
	  TYPE elmt;
	  Elmt<TYPE> *next;

}; //end class

template<class TYPE>
Elmt<TYPE>::Elmt(const TYPE &e, Elmt<TYPE> *e_ptr)
{
  elmt = e;
  next = e_ptr;
}


template<class TYPE>
Elmt<TYPE>::~Elmt()
{
  next = NULL;
}


template <class TYPE>
class Set
{
  friend ostream &operator<<(ostream &os, const Set<TYPE> &s);
  friend istream &operator>>(istream &is, Set<TYPE> &s);

  friend Set<TYPE> &operator+(const Set<TYPE> &s1, const Set<TYPE> &s2);//union
  friend Set<TYPE> &operator-(const Set<TYPE> &s1, const Set<TYPE> &s2);//sub
  friend Set<TYPE> &operator&(const Set<TYPE> &s1, const Set<TYPE> &s2);//inter

  friend void crossprod(ostream &os, const Set<TYPE> &s1,
				     const Set<TYPE> &s2);//x-pdt

  friend int operator==(const Set<TYPE> &s1, const Set<TYPE> &s2);//equality
  friend int operator!=(const Set<TYPE> &s1, const Set<TYPE> &s2);//in-equality
  friend int operator>(const Set<TYPE> &s1, const Set<TYPE> &s2);//prop superset
  friend int operator<(const Set<TYPE> &s1, const Set<TYPE> &s2);//proper subset
  friend int operator>=(const Set<TYPE> &s1, const Set<TYPE> &s2);//superset
  friend int operator<=(const Set<TYPE> &s1, const Set<TYPE> &s2);//subset
  
  public:
	  Set();
	  ~Set();
	  Set(const Set<TYPE> &s);

	  Set<TYPE> &operator=(const Set<TYPE> &s); //assignment

	  TYPE operator()(const int &n) const; //direct reference to element n

	  int ismember(const TYPE &elmt) const;//1 if elmt member of set
	  int cardinality() const;//returns the number of elements in a set
	  void insert(const TYPE &elmt);
	  int remove(const TYPE &elmt);//return 1 if successful
	  void remove();//removes ALL elements of the set

  private:
	  int num_elmts;
	  Elmt<TYPE> *set;

}; //end class 


template<class TYPE>
Set<TYPE>::Set()
{
  num_elmts = 0;
  set = NULL;
}

template<class TYPE>
Set<TYPE>::Set(const Set<TYPE> &s)
{
  Elmt<TYPE> *pt = s.set;  // set is the set to be copied

  num_elmts = 0;
  set = NULL;

  while ( pt != NULL )
    { // add all elements to this
      insert(pt->elmt);
      pt = pt->next;
    }//end while

}

template<class TYPE>
Set<TYPE>::~Set()
{
  remove();
}

template<class TYPE>
void Set<TYPE>::remove()
{
  Elmt<TYPE> *tmp, *ptr;

  ptr = set;
  while (ptr != NULL)
  {
    tmp = ptr;
    ptr = ptr->next;
    delete tmp;
  }//end while

  set = NULL;
  num_elmts = 0;
}


template<class TYPE>
Set<TYPE> &Set<TYPE>::operator=(const Set<TYPE> &s) //assignment
{
  if (this != &s)				// check for: a = a;
    {        
      Elmt<TYPE> *pt = s.set;	// set is righthand side (rhs) of "="

      this->remove();			// remove all old members of lhs

      while ( pt != NULL )
        { // add all elements to this
          this->insert(pt->elmt);
          pt = pt->next;
        }//end while

    }//end then

  return *this;
}


template<class TYPE>
TYPE Set<TYPE>::operator()(const int &n) const
{
  int i;
  Elmt<TYPE> *ptr = this->set;

  for (i=1;i<n;i++)
    ptr = ptr->next;

  return ptr->elmt;
}

template<class TYPE>
void Set<TYPE>::insert(const TYPE &elmt)
{
  Elmt<TYPE> *new_elmt = new Elmt<TYPE>(elmt,set);//make new elmt ptg to set

  if (!ismember(elmt)) //add only unique elements
    {
      set = new_elmt; //make set now point to new elmt
      num_elmts++;
    }
}

template<class TYPE>
int Set<TYPE>::remove(const TYPE &key) //return 1 if successful
{
  Elmt<TYPE> *nxt, *prev=NULL, *curr=set;

  if (cardinality()==0)
    return 0;
 
  //set has at least one elmt
  ///////////////////////////
  nxt = curr->next;
  while ((nxt != NULL) && (curr->elmt != key))
    {
      prev = curr;
      curr = nxt;
      nxt = nxt->next;
    }//end while

  if (curr->elmt == key) // found desired element
    {
      if (curr == set) //at first elmt of set
	set = nxt;
      else if (nxt == NULL) //at last elmt of set
	prev->next = NULL;
      else //somewhere in middle of set
	prev->next = nxt;

      delete curr; 
      num_elmts--;
      return 1;
    }//then
  else
    return 0;
  
}

template<class TYPE>
Set<TYPE> &operator+(const Set<TYPE> &s1, const Set<TYPE> &s2) //union
{
  Set<TYPE> *u = new Set<TYPE>;
  Elmt<TYPE> *ptr = s2.set;

  *u = s1; 
  while (ptr != NULL)
    {
      u->insert(ptr->elmt);
      ptr = ptr->next;
    }//end while

  return *u;
}

template<class TYPE>
Set<TYPE> &operator-(const Set<TYPE> &s1, const Set<TYPE> &s2) //sub
{
  Set<TYPE> *sub = new Set<TYPE>;
  Elmt<TYPE> *ptr = s2.set;

  *sub = s1;
  while (ptr != NULL)
    {
      sub->remove(ptr->elmt);
      ptr = ptr->next;
    }//end while

  return *sub;
}


template<class TYPE>
Set<TYPE> &operator&(const Set<TYPE> &s1, const Set<TYPE> &s2) //inter
{
  Set<TYPE> *inter = new Set<TYPE>;
  Elmt<TYPE> *ptr = s2.set;

  while (ptr != NULL)
    {
      if (s1.ismember(ptr->elmt))
        inter->insert(ptr->elmt);
      ptr = ptr->next;
    }//end while

  return *inter;

}


template<class TYPE>
int operator>(const Set<TYPE> &s1, const Set<TYPE> &s2) //proper superset
{
  Elmt<TYPE> *ptr1;
  TYPE e1;

  if (s1.cardinality() == s2.cardinality())
    return 0;

  for (ptr1 = s2.set;
       ptr1 != NULL;
       ptr1 = ptr1->next )
    {
      e1 = ptr1->elmt;
  
      //every element must be in super set

      if (!(s1.ismember(e1)))
        return 0;

    }//end for

  return 1;

}


template<class TYPE>
int operator<(const Set<TYPE> &s1, const Set<TYPE> &s2) //proper subset
{
  return (s2 > s1);
}

template<class TYPE>
int operator>=(const Set<TYPE> &s1, const Set<TYPE> &s2) //superset
{
  Elmt<TYPE> *ptr1;
  TYPE e1;

  for (ptr1 = s2.set;
       ptr1 != NULL;
       ptr1 = ptr1->next )
    {
      e1 = ptr1->elmt;
  
      //every element must be in super set

      if (!(s1.ismember(e1)))
        return 0;

    }//end for

  return 1;
}

template<class TYPE>
int operator<=(const Set<TYPE> &s1, const Set<TYPE> &s2) //subset
{
  return (s2 >= s1);
}

template<class TYPE>
void crossprod(ostream &os, const Set<TYPE> &s1, 
			    const Set<TYPE> &s2) //x-pdt
{
  Elmt<TYPE> *ptr1, *ptr2;
  TYPE e1, e2;

  os << "[ ";
  for (ptr1=s1.set;ptr1 != NULL;ptr1 = ptr1->next)
    for (ptr2=s2.set;ptr2 != NULL;ptr2 = ptr2->next)
      {
	e1 = ptr1->elmt;
	e2 = ptr2->elmt;
	os<<e1<<e2<<" ";
      }//end for

  os<<"]";

}


template<class TYPE>
int operator==(const Set<TYPE> &s1, const Set<TYPE> &s2) //equality
{
  Elmt<TYPE> *ptr1, *ptr2;
  TYPE e1, e2;

  if (s1.cardinality() != s2.cardinality()) //quick check
    return 0;
  
  //both sets must be same size

  for (ptr1 = s1.set , ptr2 = s2.set;ptr1 != NULL;ptr1 = ptr1->next, 
						  ptr2 = ptr2->next )
    {
      e1 = ptr1->elmt;
      e2 = ptr2->elmt;
  
      //every element must be in each set

      if (!(s1.ismember(e2) && s2.ismember(e1)))
        return 0;

    }//end for

  return 1;
}

template<class TYPE>
int operator!=(const Set<TYPE> &s1, const Set<TYPE> &s2) //in-equality
{
  return (!(s1==s2));
}

template<class TYPE>
int Set<TYPE>::ismember(const TYPE &elmt) const 
{
  Elmt<TYPE> *ptr = set;

  while (ptr != NULL)
    {
      if (ptr->elmt == elmt)
	return 1;

      ptr = ptr->next;
    }//end while

  //is not a member
  return 0;
}

template<class TYPE>
int Set<TYPE>::cardinality() const //returns the number of elements in a set
{
  return num_elmts;
}

template<class TYPE>
ostream &operator<<(ostream &os, const Set<TYPE> &s)
{
  Elmt<TYPE> *ptr = s.set;

  os<<"{ ";
  for ( ;ptr != NULL;ptr = ptr->next)
    os<<ptr->elmt<<" ";
  os<<"}";

  return os;

}


template<class TYPE>
istream &operator>>(istream &is, Set<TYPE> &s)
{
  char ch;
  TYPE e;

  is>>ch;  //read '{'
  if (ch != '{')
    {
      cerr<<"\nNot in proper set format"<<endl;
      return is;
    }

  s.remove(); //clean out receiveing set
  is>>ch;
  while (ch != '}')
    {
      is.putback(ch);
      is>>e;
      s.insert(e);
      is>>ch;
    }//end while

  //have just read in '}'
  return is;
}

#include "stdafx.h"