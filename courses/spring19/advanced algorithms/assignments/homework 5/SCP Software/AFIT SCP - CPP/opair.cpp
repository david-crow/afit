
template<class TYPE1, class TYPE2>
class OPair
{
  friend ostream &operator<<(ostream &os, const OPair<TYPE1,TYPE2> &p);//output
  friend istream &operator>>(istream &is, OPair<TYPE1,TYPE2> &p);//input

  friend int operator==(const OPair<TYPE1,TYPE2> &p1,
			const OPair<TYPE1,TYPE2> &p2);//equal
  friend int operator!=(const OPair<TYPE1,TYPE2> &p1,
			const OPair<TYPE1,TYPE2> &p2);//in-eql

  public:
	 OPair(const TYPE1 &l, const TYPE2 &r);
	 OPair();
	 OPair(const OPair<TYPE1,TYPE2> &p);
	 ~OPair();

         OPair<TYPE1,TYPE2> &operator=(const OPair<TYPE1,TYPE2> &p);//assignment

	 TYPE1 &lhs();//index for left member
	 TYPE2 &rhs();//index for right member

  private:
	  TYPE1 left;
	  TYPE2 right;

}; //end class
 

template<class TYPE1, class TYPE2>
ostream &operator<<(ostream &os, const OPair<TYPE1,TYPE2> &p) //output
{
  os<<"("<<p.left<<","<<p.right<<")";

  return os;
}

template<class TYPE1, class TYPE2>
istream &operator>>(istream &is, OPair<TYPE1,TYPE2> &p) //input
{
  char ch;

  //  (   left     ,    right   )
  is>>ch>>p.left>>ch>>p.right>>ch;

  return is;
}


  
template<class TYPE1, class TYPE2>
int operator==(const OPair<TYPE1,TYPE2> &p1,
	       const OPair<TYPE1,TYPE2> &p2) //equality
{
  if ((p1.left==p2.left) && (p1.right==p2.right)) 
    return 1;
  else
    return 0;
}

template<class TYPE1, class TYPE2>
int operator!=(const OPair<TYPE1,TYPE2> &p1,
	       const OPair<TYPE1,TYPE2> &p2) //in-equality
{
  return (!(p1==p2));
}



template<class TYPE1, class TYPE2>
OPair<TYPE1,TYPE2>::OPair(const TYPE1 &l, const TYPE2 &r) 
{
  left  = l;
  right = r;
}

template<class TYPE1, class TYPE2>
OPair<TYPE1,TYPE2>::OPair(const OPair<TYPE1,TYPE2> &p)
{
  left  = p.left;
  right = p.right;
}

template<class TYPE1, class TYPE2>
OPair<TYPE1,TYPE2>::OPair()
{
  ;
}

template<class TYPE1, class TYPE2>
OPair<TYPE1,TYPE2>::~OPair() 
{
  ;
}


template<class TYPE1, class TYPE2>
OPair<TYPE1,TYPE2> &OPair<TYPE1,TYPE2>::operator=(const OPair<TYPE1,TYPE2> &p)
//assignment
{
  if (this != &p)
    {
      left  = p.left;
      right = p.right;
    }//end then
  
  return *this;
}

template<class TYPE1, class TYPE2>
TYPE1 &OPair<TYPE1,TYPE2>::lhs() //index for left member
{
  return left;
}

template<class TYPE1, class TYPE2>
TYPE2 &OPair<TYPE1,TYPE2>::rhs() //index for right member
{
  return right;
}

#include "stdafx.h"