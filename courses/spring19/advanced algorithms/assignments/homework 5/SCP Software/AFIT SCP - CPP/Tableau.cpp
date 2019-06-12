#include "block.cpp"

class Tableau : public List<OPair<Block,int> >
{
  public:
	  Tableau();
	  Tableau(const Set<OPair<Set<int>,int> > &S);	//makes tableau
													//from set of
			//**must use this method for			//weighted sets
            //  construction of tableau				//organized as
													//ordered pairs 

	  Tableau(const Set<OPair<Set<int>,int> > &S, int spp);	//makes SPP tableau
															//from set of
	    //**must use this method for						//weighted sets
        //  construction of tableau							//organized as
															//ordered pairs 


	  Set<int> Get_Set( const int &b, const int &i); //retrieve set at block
							 //b, position i
	  int Get_Weight( const int &b, const int &i ); //retrieve weight of set
						        //at block b, position i

      List<int> Block_indicies();	//returns list of block indicies
									//e.g. if the tableau
									//currently contains 3 blocks
									//and the first block is for
									//the element '2' the second
									//block is for the element '4'
									//and the third block is for
									//the element '5', then this
									//function will return [ 2 4 5 ].
									//This supports a non-consequtively
									//ordered set to cover (R)

	  int num_blocks() const;		//return number of blocks currently in tableau
	  int num_sets() const;			//return number of sets currently in tableau
	  int num_sets_in_block(const int &b);	//return number of sets in 
											//block b

  private:
	  int number_of_sets;
	  void Add_Set( const Set<int> &s, const int &weight );	//add s to all
	   														//appropriate
															//blocks in
															//tableau

   	  void Add_Set_SPP( const Set<int> &s, const int &weight );	//add s to all
	   															//appropriate
																//blocks in
																//tableau

	  void Optimize();	//Optimize arrangement of blocks in tableau
	  int exist_block_for(const int &e);	//returns 1 if a block that
											//a set containing e should
											//be put in exists, 0 otherwise
	  int block_index_for(const int &e);	//returns index of block that
											//a set containing e should
											//be put in--even if the
											//block does not currently exist

};

ostream &operator<<(ostream &os, const List<OPair<Block,int> > &t)
{
  Element<OPair<Block,int> > *ptr = t.list;

  os<<"\n*[\n";

  for(;ptr != NULL; ptr = ptr->next )
    os<<(ptr->element).lhs()<<"\n";

  os<<"]*\n";

  return os;
}

Tableau::Tableau()
{
  number_of_sets = 0;
}

Tableau::Tableau(const Set<OPair<Set<int>,int> > &S)  //makes tableau
	                                              //from set of
						      //weighted sets
						      //organized as
						      //ordered pairs 
{
  number_of_sets = 0;

  int i, n = S.cardinality();

  for(i=1;i<=n;i++)
    Add_Set(S(i).lhs(), S(i).rhs());

  //could optimize here

  /************** 
  Not needed--by eliminating this section it takes less time to
  construct tableau, and less storage space to keep track of set
  identities

  //ensure sets in blk K only contain the Kth element and below
  List<int> B_I = Block_indicies();

  for(i=2;i<=num_elements;i++)               //each block (after 1)
    for(int j=1;j<=num_sets_in_block(i);j++) //each set in block
      for(int k=1;k<i;k++)                   //each element before this
        (*this)(i).lhs()(j).lhs().remove(B_I(k));
  ***************/

}

Tableau::Tableau(const Set<OPair<Set<int>,int> > &S, int spp)	//makes SPP tableau
																//from set of
																//weighted sets
																//organized as
																//ordered pairs 
{
  number_of_sets = 0;

  int i, n = S.cardinality();

  for(i=1;i<=n;i++)
    Add_Set_SPP(S(i).lhs(), S(i).rhs());

  //could optimize here

  /************** 
  Not needed--by eliminating this section it takes less time to
  construct tableau, and less storage space to keep track of set
  identities

  //ensure sets in blk K only contain the Kth element and below
  List<int> B_I = Block_indicies();

  for(i=2;i<=num_elements;i++)               //each block (after 1)
    for(int j=1;j<=num_sets_in_block(i);j++) //each set in block
      for(int k=1;k<i;k++)                   //each element before this
        (*this)(i).lhs()(j).lhs().remove(B_I(k));
  ***************/

}



void Tableau::Add_Set( const Set<int> &s, const int &weight ) //add s to all
                                                              //appropriate 
							      //blocks in 
							      //tableau
{
  int i, b, n = s.cardinality();

  for(i=1;i<=n;i++)
    {
      b = block_index_for(s(i));
      if (exist_block_for(s(i)))
	{
          (*this)(b).lhs().Add_Set(s,weight);
        }
      else
	{
	  Block B;
	  B.Add_Set(s,weight);
	  OPair<Block,int> P(B,s(i));
	  insert(P,b);
	}
    }//end for

  if (n)   //if not an empty set
    number_of_sets++;
}

void Tableau::Add_Set_SPP( const Set<int> &s, const int &weight ) //add s to all
	                                                              //appropriate 
																  //blocks in 
																  //tableau
{
  int i, b, n = s.cardinality();


	// For SPP each set goes into the block corresponding to its first
	// element.  The first element will be the smallest
	int min = s(1);
	for(i=2;i<=n;i++){
		if(s(i) < min)
			min = s(i);
	} // for
	b = block_index_for(min);
	if (exist_block_for(min)){
		(*this)(b).lhs().Add_Set(s,weight);
	} else {
		Block B;
		B.Add_Set(s,weight);
		OPair<Block,int> P(B,min);
		insert(P,b);
	} // if

	if (n)   //if not an empty set
		number_of_sets++;
}



Set<int> Tableau::Get_Set( const int &b, const int &i ) //retrieve set at block
							//b position i
{
  return (*this)(b).lhs().Get_Set(i);
}

int Tableau::Get_Weight( const int &b, const int &i ) //retrieve weight of set
						      //at block b position i
{
  return (*this)(b).lhs().Get_Weight(i);
}

List<int> Tableau::Block_indicies()         //returns list of block indicies
					    //e.g. if the tableau
					    //currently contains 3 blocks
					    //and the first block is for
					    //the element '2' the second
					    //block is for the element '4'
					    //and the third block is for
					    //the element '5', then this
					    //function will return [ 2 4 5 ].
					    //This supports a non-consequtively
					    //ordered set to cover (R)
{
  List<int> L;
  int i; 

  for(i=1;i<=num_elements;i++)
    L.append((*this)(i).rhs());

  return L;
  
}

int Tableau::num_sets() const //return number of sets currently in tableau
{
  return number_of_sets;
}


int Tableau::exist_block_for(const int &e)  //returns 1 if a block that
	                                    //a set containing e should
					    //be put in exists, 0 otherwise
{
  int i;

  for(i=1;i<=num_elements;i++)
    if ((*this)(i).rhs() == e)
      return 1;

  return 0;
}

int Tableau::block_index_for(const int &e)  //returns index of block that
					    //a set containing e should
					    //be put in--even if the
					    //block does not currently exist
{
  int i=1;

  if (exist_block_for(e))
    for(i=1;((*this)(i).rhs() != e);i++);
  else if (num_elements > 0)
    {
      for(i=num_elements;((i > 0) && ((*this)(i).rhs() > e));i--);
      i++;
    }

  return (i);
}

void Tableau::Optimize() //Optimize arrangement of blocks in tableau
{
  int i,j;

  //bubble sort of the blocks based on size of each block
  for (i=1;i<=num_elements;i++)
    for (j=1;j<num_elements;j++)
       if ((*this)(j).lhs().num_sets() > (*this)(j+1).lhs().num_sets()) 
         {
	   OPair<Block,int> T((*this)(j));

	   (*this)(j) = (*this)(j+1);
	   (*this)(j+1) = T;
	 }
}

int Tableau::num_blocks() const  //return number of blocks currently in tableau
{
  return num_elements; 
}

int Tableau::num_sets_in_block(const int &b)  //return number of sets in 
					       //block b
{
  return (*this)(b).lhs().num_sets();
}

#include "stdafx.h"




















