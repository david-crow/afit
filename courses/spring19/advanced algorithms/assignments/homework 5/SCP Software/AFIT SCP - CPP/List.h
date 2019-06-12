#ifndef __list_element__
#define __list_element__
#include <iostream.h>
template<class TYPE> class List;

template<class TYPE> class Element
{
  friend class List<TYPE>;
  friend ostream &operator<<(ostream &os, const List<TYPE> &l);
  friend istream &operator>>(istream &is, List<TYPE> &l);

  friend int operator==(const List<TYPE> &l1, const List<TYPE> &l2);//equality
  friend int operator!=(const List<TYPE> &l1, const List<TYPE> &l2);//in-equality

  public:

  private:
	  Element(const TYPE &e, Element<TYPE> *e_ptr){
		element = e;
		next = e_ptr;
	  }
	  ~Element(){
		next = NULL;
	  }

	  TYPE element;
	  Element<TYPE> *next;

}; //end class


#endif









