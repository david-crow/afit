
class Block : public List<OPair<Set<int>,int> >
{
  friend ostream &operator<<(ostream &os, const List<OPair<Set<int>,int> > &b);
  public:
	  void Add_Set( const Set<int> &s, const int &weight );//add s to block
	   						      //sorted by weight
	  Set<int> Get_Set( const int &i );//retrieve set at position i
	  int Get_Weight( const int &i );//retrieve weight of set at position i

	  int num_sets(); //return number of sets currently in block

};

int operator==(const List<OPair<Set<int>,int> > &b1,
	       const List<OPair<Set<int>,int> > &b2)

{
  Element<OPair<Set<int>,int> > *ptr1, *ptr2;

  if (b1.num_elements != b2.num_elements) //quick check
    return 0;
  
  //both lists must be same size

  for (ptr1 = b1.list , ptr2 = b2.list;
       ptr1 != NULL;
       ptr1 = ptr1->next, ptr2 = ptr2->next )
  
      //every element must be in each list in order

      if (!(ptr1->element == ptr2->element))
        return 0;

  return 1;
}

ostream &operator<<(ostream &os, const List<OPair<Set<int>,int> > &b)
{
  //int i, l = b.length(); 
  Element<OPair<Set<int>,int> > *ptr = b.list;

  os<<"\n[\n";

  for(;ptr != NULL; ptr = ptr->next )
    os<<ptr->element<<"\n";

  os<<"]\n";

  return os;
}

void Block::Add_Set( const Set<int> &s, const int &weight ) //add s to block
	  					            //sorted by weight
{
  int i;

  for(i=1;((i<=num_elements)&&((*this)(i).rhs() < weight));i++);
  OPair<Set<int>,int> P(s,weight);
  insert(P,i);
}


Set<int> Block::Get_Set( const int &i ) //retrieve set at position i
{
  return (*this)(i).lhs();
}

int Block::Get_Weight( const int &i ) //retrieve weight of set at position i
{
  return (*this)(i).rhs();
}


int Block::num_sets() //return number of sets currently in block
{
  return num_elements;
}


#include "stdafx.h"