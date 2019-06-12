/*
         --*********************************************************--
         --**      copyright (C)   1994                           **--
         --**                                                     **--
         --**      Permission to use, copy and distribute this    **--
         --**      software for educational purposes without      **--
         --**      fee is hereby granted provided that the        **--
         --**      copyright notice and this permission notice    **--
         --**      appear on all copies. Permission to modify     **--
         --**      the software is granted, but not the right     **--
         --**      to distribute the modified code. All           **--
         --**      modifications are to be distributed as         **--
         --**      changes to released versions by AFIT. Use      **--
		 --**      of program should be explicity referenced.     **--
         --**											          **--
         --*********************************************************--
TITLE: SCP/SPP program

DESCRIPTION: This program solves the set-covering problem and the
             set-partitioning problem using the algorithm in 
	     Christofides.
*/
#include "stdafx.h"
#include <stdlib.h>
#include <limits.h>
#include <iostream.h>
#include <fstream.h>
#include "list.cpp"
#include "set.cpp"
#include "opair.cpp"
#include "tableau.cpp"

typedef Set<int> IntSet;
typedef OPair<IntSet,int> Weighted_set;
typedef Set<Weighted_set> Set_of_weighted_sets;
typedef List<Weighted_set> List_of_weighted_sets;

//********************************************************************
/***************************
  cost_of

  This simple function calculates the cost of the partial solution B by
  adding together all the weights of the sets in B.
***************************/

int cost_of(const Set_of_weighted_sets &B)
{
  int i, n = B.cardinality();
  int cost = 0;

  for(i=1;i<=n;i++)
    cost += B(i).rhs();

  return cost;
}

//********************************************************************
/***************************
  cover_of

  This simple function calculates the cover of a partial solution B by
  unioning together all the sets in B.
***************************/

IntSet cover_of(const Set_of_weighted_sets &B)
{
  int i, n = B.cardinality();
  IntSet E;

  for(i=1;i<=n;i++)
    E = E + B(i).lhs(); 

  return E;
}
//********************************************************************
/*****************************
  next_set

  This function calculates the next set to be considered for inclusion
  in the partial solution.  Because this function may have to backtrack
  through a previously visited block of the tableau, the function is
  allowed to modify B, E, and z.  Additionally this function keeps
  (modifies) the current_position_in_block array and the previous block
  counter.  Finally, this function sets the done flag when the tableau
  has been exhausted.
*****************************/

Weighted_set next_set(Tableau              &T,
		      Set_of_weighted_sets &B,
		      IntSet               &E,
		      int                  &z,
		      OPair<int,int>       current_position_in_block[],
		      int                  &previous_block,
		      int                  &done)
{
  List<int> block_ind = T.Block_indicies(); //amounts to R put in the order
											//of blocks in the tableau

  int i;

  for(i=1;
      ((i<=block_ind.length()) && E.ismember(block_ind(i)));
      i++); //increment i until it equals the
			//index of the next block
			//to be included in the search


  //check to see if E really covers R---
  //should happen only when all of the must include sets from
  //reduction_2_2 actually cover R
  if (i==(block_ind.length() + 1))
    {
      i = 1;	//bogus
      done = 1; //finished
    }
 
  if (!(done))
    current_position_in_block[i].lhs()++; //move index of next block to next set

  while ((!(done)) &&
	 (current_position_in_block[i].lhs() > T.num_sets_in_block(i)))
    {
      //all sets in this block have been searched
      if (i!=1)
        {
	  //must "backtrack"--goto previous state

	  current_position_in_block[i].lhs() = 0;
	  current_position_in_block[i].rhs() = 0;

	  Weighted_set W(T.Get_Set(previous_block, 
			       current_position_in_block[previous_block].lhs()),
                         T.Get_Weight(previous_block,
			      current_position_in_block[previous_block].lhs()));
 

	  B.remove(W);
	  E = cover_of(B);
	  z = cost_of(B);

          previous_block = current_position_in_block[previous_block].rhs();

          for(i=1;(E.ismember(block_ind(i)));i++); //increment i until equals
	        				   //index of the next block
	        				   //to be included in search
          current_position_in_block[i].lhs()++; //move index of next block
						//to next set
	}
      else
	{
	  done = 1; //if all sets in first block have been searched, end search
        }

    }//end while

  if (done)
    {//bogus info since done
      Weighted_set P(E,0);
      return P;
    }


  current_position_in_block[i].rhs() = previous_block;
  previous_block = i;
  Weighted_set P(T.Get_Set(i,current_position_in_block[i].lhs()),
                 T.Get_Weight(i,current_position_in_block[i].lhs()));

  return P;
      
}

//********************************************************************
/***************************
  reduction_2_2

  This function implements the second half of reduction 2 as discussed in
  Christofides.  This function removes from F any set that is the only
  set to contain some element of R.  This set must appear in the partial
  solution, and all of these sets are returned in the partial solution.
  The element is not removed from R (contrary to Christofides) because
  the partial solution already contains this element, and if it was
  removed, the comparision of E==R in set_cover would fail.  The element
  could be removed from R, and the partial solution, and it would then
  have to be removed from the Original_Order_List so that proper matching
  could take place at the conclusion of the program.  This implementation
  seem less cumbersome.
  this is no longer called because it cause unwanted complications
***************************/

void reduction_2_2 ( IntSet &R,
		     Set_of_weighted_sets &F,
		     Set_of_weighted_sets &Partial_solution )
{
  int count;
  Weighted_set ws_tmp;

  for(int i=1;i<=R.cardinality();i++) //for every element in R
    {
      count = 0;

      for(int j=1;                        //for every set in F
  	  ((j<=F.cardinality()) && (count < 2));
	  j++) 
	    if (F(j).lhs().ismember(R(i))) //if Ri is member of Fj
	      {
		ws_tmp = F(j);
		count++;
              }

      if (count == 1)
        Partial_solution.insert(ws_tmp);

    }//end for

  //R is not changed because the partial solution already contains
  //the elements of R that are already covered.  This would not
  //reduce the problem size since constructing the tableau is a 
  //function of F alone, not R and F.

  F = F - Partial_solution; 

}

//********************************************************************
/***************************
  reduction_2_1

  This function performs the first half of reduction 2 as mentioned in
  Christofides.  This reduction removes from R and all sets in F, any
  element that appears in every set--since this element would be covered
  regardless of the solution.
  the element is not added back to R so this functin is no longer called
***************************/

void reduction_2_1 ( IntSet &R,
		     Set_of_weighted_sets &F,
		     List_of_weighted_sets &Original_Order_List )
{
  IntSet All_Intersection = F(1).lhs();

  for(int i=2;i<=F.cardinality();i++)  //for each set in F
    All_Intersection = All_Intersection & F(i).lhs();

  //All_Intersection now contains all elements in every set in F

  R = R - All_Intersection;//remove from R

  Set_of_weighted_sets New_F;
  for(int j=1;j<=F.cardinality();j++)
    {
      //keeps track of set identities
      Original_Order_List(j).lhs() = Original_Order_List(j).lhs() -
				     All_Intersection;

      //because set elements cannot be individually changed
      //a new set must be constructed to remove elements from F
      IntSet       is_tmp = F(j).lhs() - All_Intersection;
      Weighted_set ws_tmp(is_tmp,F(j).rhs());
      New_F.insert(ws_tmp);
    }
  F = New_F;

}

//********************************************************************
/***************************
  Set_Partition

  This is the heart of Christofides set partitioning algorithm.  This
  function is not called if there is no chance for a possible solution.
  Once called this function initializes the variables it will be using.
  Next it performs the reduction operations discussed in Christofides.
  Since some of these operations modify R, F, B, and Original_Order_List,
  they must come after the initializations, and befor construction of the
  tableau.  The tableau is constructed with a single call to the tableau
  object which knows how to construct itself from a set of weighted sets, F.  
  Next, the current_position_in_block[] array is set up which facilitates
  backtracking through the tableau with the use of the previous_position
  counter.  The first set is selected, and the main loop is entered until
  the next_set function sets the done flag and the loop terminates.  The
  loop takes the next candidate, checks to see if the no-overcovering
  requirement is met, and if it is does a cost test to see if it is
  feasible to be added to the current partial solution.  It should be
  noted that since this is the only test performed, this implementation
  is basically an exhaustive depth-first search of the tableau and
  pruning is not done nearly as often as would be possible with the
  dominance tests Christofides mentions.  These tests could easially be
  included in the if-statment just inside the while loop. Finally when
  the loop is done B_hat and z_hat contain the best solutions to the
  problem and are returned to the main program.
***************************/

void Set_Partition( IntSet &R,
	        Set_of_weighted_sets &F,
		List_of_weighted_sets &Original_Order_List,
	        int &z_hat,
	        Set_of_weighted_sets &B_hat)
{
  Set_of_weighted_sets B; //partial solution
  int                  z; //current cost of B
  IntSet               E; //elements covered by B
  IntSet			   Intersection; // Intersection of two sets (used in step 3)
  Weighted_set         S; //current set

  z = 0;
  z_hat = INT_MAX;//infinity
  B_hat.remove(); //B_hat = empty set

  int done = 0; //boolean
  int previous_block = 0;//aid to backtracking

  //not called because of complications but program still runs just slower
  //reduction_2_1(R,F,Original_Order_List);
  //reduction_2_2(R,F,B);
  //reduction_3(R,F,Original_Order_List);
  //reduction_4(R,F,Original_Order_List);

  int SPP = 1;
  Tableau T(F/*,SPP*/); //sets up SPP tableau
  //set up a SCP tableau makes SPP work for more cases
  //-------------STEP 1 COMPLETE-------------

  OPair<int,int> current_position_in_block[1000/*T.num_blocks() + 1*/];
  //lhs = position in block; rhs = previous block to this one

  for(int i = 1;i<=(T.num_blocks() + 1);i++)
    current_position_in_block[i].lhs() = current_position_in_block[i].rhs() = 0;

  //ensures that any mandatory included sets are accounted for
  B_hat = B;
  E = cover_of(B);
  z = cost_of(B);

  //---STEP 2 & 3---
  S = next_set(T,B,E,z,current_position_in_block,previous_block,done);

  while (!(done)) 
    {
      //---STEP 3---
	  // This is where SPP differs from SCP (besides the different Tableau setup)
	  IntSet B_set = S.lhs();
	  Intersection = cover_of(B) & S.lhs();      
	  if ((Intersection.cardinality() == 0) && (S.rhs() + z) < z_hat)
	//current set cost + cost so far < best_cost
	{
	  B.insert(S);
	  E = cover_of(B);
	  z = cost_of(B);

	  if (E == R) //---STEP 5---
	    {
	      B_hat = B;
	      z_hat = z;

	      //cout<<"\nB_hat = "<<B_hat;
	      //cout<<"\nz_hat = "<<z_hat<<endl;

	      //backtrack---STEP 4---
	      B.remove(S);
	      E = cover_of(B);
	      z = cost_of(B);
	      previous_block = current_position_in_block[previous_block].rhs();

	    }//end then

	}//end then

      else
	//backtrack---STEP 4---
	previous_block = current_position_in_block[previous_block].rhs();

      //---STEP 2 & 3---
      S = next_set(T,B,E,z,current_position_in_block,previous_block,done);

    }//end while
  
  z_hat = cost_of(B_hat); //needed in the case of mandatory inclusion
			  //sets from reductions actually form solution
}

//********************************************************************
/***************************
  match_original_sets

  This function takes a list (which by definition is ordered) that was
  made during input, and possible altered during the reductions of the
  Set_Cover.  This list has, in order, the sets that the user input for
  F.  This function, then, takes the solution B_hat, and finds each
  member in the Original_Order_List.  The index of the list, 1 for
  element/set one, 2 for set two,..., and puts it into a set which is
  returned.  This returned set is then reported to the user and informs
  them that sets 2, 5, 10, etc.  are in the solution.  Generally this
  information is more helpful that the sets themselves, which may have
  been altered in the reductions (and of which many may exist).
***************************/

IntSet match_original_sets(const Set_of_weighted_sets &B_hat,
			         List_of_weighted_sets &Original_Order_List)
{
  IntSet S;
  int i, j, n = B_hat.cardinality();

  for(i=1;i<=n;i++)
    {
      for(j=1;B_hat(i) != Original_Order_List(j);j++);
      S.insert(j);
    }

  return S;

}

//********************************************************************
/***************************
  Set_Cover

  This is the heart of Christofides set covering algorithm.  This
  function is not called if there is no chance for a possible solution.
  Once called this function initializes the variables it will be using.
  Next it performs the reduction operations discussed in Christofides.
  Since some of these operations modify R, F, B, and Original_Order_List,
  they must come after the initializations, and befor construction of the
  tableau.  The tableau is constructed with a single call to the tableau
  object which knows how to construct itself from a set of weighted sets, F.  
  Next, the current_position_in_block[] array is set up which facilitates
  backtracking through the tableau with the use of the previous_position
  counter.  The first set is selected, and the main loop is entered until
  the next_set function sets the done flag and the loop terminates.  The
  loop takes the next candidate and does a cost test to see if it is
  feasible to be added to the current partial solution.  It should be
  noted that since this is the only test performed, this implementation
  is basically an exhaustive depth-first search of the tableau and
  pruning is not done nearly as often as would be possible with the
  dominance tests Christofides mentions.  These tests could easially be
  included in the if-statment just inside the while loop. Finally when
  the loop is done B_hat and z_hat contain the best solutions to the
  problem and are returned to the main program.
***************************/

void Set_Cover( IntSet &R,
	        Set_of_weighted_sets &F,
		List_of_weighted_sets &Original_Order_List,
	        int &z_hat,
	        Set_of_weighted_sets &B_hat)
{
  Set_of_weighted_sets B; //partial solution
  int                  z; //current cost of B
  IntSet               E; //elements covered by B
  Weighted_set         S; //current set

  z = 0;
  z_hat = INT_MAX;//infinity
  B_hat.remove(); //B_hat = empty set

  int done = 0; //boolean
  int previous_block = 0;//aid to backtracking

  //not called because of complications but program still runs just slower
  //reduction_2_1(R,F,Original_Order_List);
  //reduction_2_2(R,F,B);
  //reduction_3(R,F,Original_Order_List);
  //reduction_4(R,F,Original_Order_List);

  Tableau T(F); //sets up tableau
  //-------------STEP 1 COMPLETE-------------
  
  OPair<int,int> current_position_in_block[100/*T.num_blocks() + 1*/];
  //lhs = position in block; rhs = previous block to this one

  for(int i = 1;i<=(T.num_blocks() + 1);i++)
    current_position_in_block[i].lhs() = current_position_in_block[i].rhs() = 0;

  //ensures that any mandatory included sets are accounted for
  B_hat = B;
  E = cover_of(B);
  z = cost_of(B);

  //---STEP 2 & 3---
  S = next_set(T,B,E,z,current_position_in_block,previous_block,done);

  while (!(done)) 
    {
      //---STEP 3---
      if ((S.rhs() + z) < z_hat)
	//current set cost + cost so far < best_cost
	{
	  B.insert(S);
	  E = cover_of(B);
	  z = cost_of(B);

	  if (E == R) //---STEP 5---
	    {
	      B_hat = B;
	      z_hat = z;

	      //cout<<"\nB_hat = "<<B_hat;
	      //cout<<"\nz_hat = "<<z_hat<<endl;

	      //backtrack---STEP 4---
	      B.remove(S);
	      E = cover_of(B);
	      z = cost_of(B);
	      previous_block = current_position_in_block[previous_block].rhs();

	    }//end then

	}//end then

      else
	//backtrack---STEP 4---
	previous_block = current_position_in_block[previous_block].rhs();

      //---STEP 2 & 3---
      S = next_set(T,B,E,z,current_position_in_block,previous_block,done);

    }//end while
  
  z_hat = cost_of(B_hat); //needed in the case of mandatory inclusion
			  //sets from reductions actually form solution
}

//********************************************************************
//get_streams
void get_streams(const int &argc, char *argv[],
                 istream **in, ostream **out)
{
  ifstream *fin = new ifstream;
  ofstream *fout = new ofstream;

  *in = &cin;
  *out = &cout;

  //establish location of io streams
  //////////////////////////////////
  switch (argc)
  {
    case 3: fout->open(argv[2]);
	    *out = fout;
    case 2: fin->open(argv[1]);
	    *in = fin;
	    break;
    default: *in = &cin;
	     *out = &cout;
	     break;
  }
  
}

//********************************************************************
/***************************
  possible_solution

  This function returns a true or false (1 or 0) value depending on
  whether or not a possible soultion exists.  This is computed by
  unioning all the sets in F and seeing if that union equals R.  If it
  does not then there is no possible set cover and this function returns
  false.
***************************/

int possible_solution(const IntSet &R, const Set_of_weighted_sets &F)
{
  int i, n = F.cardinality();
  IntSet union_F;

  for(i=1;i<=n;i++)
    union_F = union_F + F(i).lhs();

  if (R == union_F)
    return 1;

  return 0;

}

//********************************************************************
/***************************
  get_F

  This function gets the family of weighted sets F from
  either the user or the file specified at the invocation of this program.
  The input file, if used, should contain two items--R and F.
  The first item in the file should be the set R and the second should
  be the set F.  (Format of R is explained in get_R) The family of sets
  F, is just a set of weighted sets.  Weighted sets are ordered pairs
  where the first entry is a set, and the second is its weight (an
  integer).  For example, the second and final item (after R) in the
  file--excluding spaces and blank lines which can appear at any
  point--should look like this:

  { ({ 1 2 3}, 6) ({3 6 7 8}, 10) } 

  The "musts" to remember are: include the curly braces and parentheses,
  enter only integer sets and integer weights, and separate each
  element (ordered pair) by a space or blank line--no commas,
  and separate each entry in the ordered pair by a comma, not a space. 
  For better readibility, the following format may be easier to use:

  {
    ({ 1 2 3 }, 6)
    ({ 3 6 7 8}, 10)
  }

  This format shows each ordered pair on a separate line, and makes
  reading the file easier on the user.

***************************/

Set_of_weighted_sets get_F (istream &is, List_of_weighted_sets &L)
{
  Set_of_weighted_sets S;

  if (&is == &cin)
    { 
      int i, n;

      //prompt user
      cout<<"\nHow many sets are there? : ";
      is>>n;

      cout<<"\nEnter the sets as integers\n";
      cout<<"\nExample: { 1 22 44 5 678 3 }      ";
      cout<<"***include the curly braces***\n";
      cout<<"\nEnter the weights as integers\n"<<endl;

      for(i=1;i<=n;i++)
	{
	  IntSet tmp_set;
	  int    tmp_weight;

          cout<<"\nEnter set "<<i<<" --> ";
          is>>tmp_set;		  
			  
	  cout<<"Enter set "<<i<<"'s weight --> ";
	  is>>tmp_weight;

	  OPair<IntSet,int> P(tmp_set,tmp_weight);
	  S.insert(P);
	  L.append(P);

	}

    }
  else
    {
      //read from file
      cout<<"\nReading family of sets from file"<<endl;
      is>>S;

      int i, n = S.cardinality();
      for(i=1;i<=n;i++)
	L.prepend(S(i)); //puts the sets in the list in proper order

      char discard;
      is.get(discard);
      while (discard != '\n')
        is.get(discard);
    }

  return S;
}

//********************************************************************
/***************************
  get_R

  This function gets the set to cover R from either the user or the file
  specified at the invocation of this program.  The input file, if used,
  should contain two items--R and F.  The first item in the file should
  be the set R and the second should be the set F.  (Format of F is
  explained in get_F)  R is just a set of integers, in any order, and not
  necessairly consecutive in value.  For example, the first item in the
  file--excluding spaces and blank lines which can appear at any
  point--should look like this:

  { 5 7 8 3 5 9 88 6 556  7 8 99 }

  The "musts" to remember are: include the curly braces, enter only
  integers, and separate each integer with a space or new-line, no
  commas.

  The option for saving the input in this function.  Naming the save file
  is also done in this function.  Files saved in this program can be
  read by this program.  Saved files can be used later.  file name can only
  be 20 characters long.  If a longer file name is desired then the user
  can always rename it at a later time outside of the program.

***************************/

IntSet get_R (istream &is, char& save, char filename[21])
{
  IntSet S;

  if (&is == &cin)
    {
	    while ((save != 'Y') && (save != 'N'))
		{
			cout<<"Do you want to save the graph input data? (Y/N)"<<endl;
			cin>>save;
			save = toupper(save);
			if ((save!='Y')&&(save!='N'))
			{
				cout<<"Illegal input, please try again."<<endl;
			}
			else if(save == 'Y')
			{
				cout << "Enter name of file to save to:\n"
					 << "(limit 20 characters including file type, no spaces allowed)\n"
					 << "(ex: longest_allowed.file)\n";
				cin >> filename;
				cout<< "You are about to enter graph data in file:  " << filename << endl;
			}
		}
      //prompt user
      cout<<"\nEnter the set of elements to cover as integers\n";
      cout<<"\nExample: { 1 22 44 5 678 3 }      ";
      cout<<"***include the curly braces***\n"<<endl;
      cout<<"set to cover--> ";
      is>>S;
    }
  else
    {
      //read from file
      cout<<"\nReading set of elements to cover from file"<<endl;
      is>>S;

      char discard;
      is.get(discard);
      while (discard != '\n')
        is.get(discard);
    }

  return S;
}

/***************************
  main

  This is the driver for the scp program.  If it is activated by
  csh> scp in.file.name 
  the in.file.name will be read from instead of prompting the user for
  input.  This file should be in the format provided as a sample and as
  mentioned in the get_R and get_F functions.  Once the input is taken
  in, this program checks if a solution is possible, then calls the
  Set_Cover function which finds the best solution.  When that function
  returns the best solution (which may have been altered by the reduction
  functions of Set_Cover) is printed out along with the best weight.
  Since the sets seen in B_hat may have been altered, a set containing
  the original number of the sets (as input) is provided for the user.
  This set of set numbers, is usually the most helpful information for
  the user since typically they want to know exactly which sets are in
  the solution.
***************************/

void main(int argc, char *argv[])
{
  //establish proper input and output streams
  istream *is;
  ostream *os;
  get_streams(argc, argv, &is, &os);
  istream &in  = *is;
  ostream &out = *os;
  //string filename;
  char save = '?';
  bool save_graph_data = false;
  char filename[21];

  //get sets R and F
  IntSet R, RSPP;
  Set_of_weighted_sets F, FSPP;
  List_of_weighted_sets Original_Order_List;

  R = RSPP = get_R(in,save,filename);
  F = FSPP = get_F(in,Original_Order_List);

	//save in a file
  save_graph_data = (save == 'Y'); //qualifies bool 
	if (save_graph_data)
	{
		ofstream outFile(filename);
		outFile << R << endl << F << endl;
	}

  //cout<<R<<endl;
  //cout<<F<<endl;
  //cout<<Original_Order_List<<endl;

  if (!(possible_solution(R,F)))
    {
      cerr<<"\nNo Possible Solution\n"<<endl;
      exit(0);
    }

  int z_hat;
  Set_of_weighted_sets B_hat;

  //z_hat is the cost of the best
  //cover B_hat
  cout<<"+++call Set Cover+++"<<endl;
  Set_Cover(R,F,Original_Order_List,z_hat,B_hat);

  cout<<"+++MAIN+++"<<endl;
  cout<<"B_hat ="<<B_hat<<endl;
  cout<<"z_hat ="<<z_hat<<endl;

  IntSet Q;
  Q = match_original_sets(B_hat,Original_Order_List);
  cout<<"set numbers found--"<<Q<<endl;

  if (!(possible_solution(RSPP,FSPP)))
    {
      cerr<<"\nNo Possible SPP Solution\n"<<endl;
//      exit(0);
    }

  //z_hat is the cost of the best
  //cover B_hat
  cout<<"+++call Set Partition+++"<<endl;
  Set_Partition(RSPP,FSPP,Original_Order_List,z_hat,B_hat);

  if (!(possible_solution(RSPP,B_hat)))
    {
      cerr<<"\nNo Possible SPP Solution\n"<<endl;
      exit(0);
    }
  cout<<"+++MAIN+++"<<endl;

  cout<<"B_hat ="<<B_hat<<endl;
  cout<<"z_hat ="<<z_hat<<endl;

  Q = match_original_sets(B_hat,Original_Order_List);
  cout<<"set numbers found--"<<Q<<endl;
}

