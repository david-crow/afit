#include <iostream.h>

/********************
   Howard Gleason
   CSCE 200

   This is the implementation of the template
   class matrix.  This class will work properly
   as long as the element type has the basic
   math and comparision operators defined.

   The reason that most all of the functions associated
   with this class do not have "const" in the function
   headers is because the functions reference the matrix
   through the () operator.  Since this operator is not
   a static function, the compiler will not let it be
   used on parameters that are "const".
********************/


template<class TYPE>
class matrix
{
  friend ostream &operator<<(ostream &os, matrix<TYPE> &m);//output
  friend istream &operator>>(istream &is, matrix<TYPE> &m);//input

  friend matrix<TYPE> operator+(matrix<TYPE> &m1,matrix<TYPE> &m2);
  friend matrix<TYPE> operator*(matrix<TYPE> &m1,matrix<TYPE> &m2);

  friend int operator==(matrix<TYPE> &m1, matrix<TYPE> &m2);//equal
  friend int operator!=(matrix<TYPE> &m1, matrix<TYPE> &m2);//in-eql

  public:
	 matrix(const long &r = 0, const long &c = 0);
	 matrix(matrix<TYPE> &m);
	 ~matrix();
         void clear();

         matrix<TYPE> &operator=(matrix<TYPE> &m);//assignment
	 TYPE &operator()(const long &r, const long &c);//index

	 long rows() const;//returns number of rows in matrix
	 long cols() const;//returns number of columns in matrix

  private:
          long row, col;
	  TYPE **entry;

}; //end class
 

template<class TYPE>
ostream &operator<<(ostream &os, matrix<TYPE> &m) //output
{
  long r,c;

  os<<"\n";
  os<<m.row<<"  "<<m.col<<"\n";
  for(r=1;r<=m.rows();r++)
    {
      os<<"  ";
      for(c=1;c<=m.cols();c++)
        os<<m(r,c)<<"  ";
      os<<"\n"; 
    }

  return os;
}

template<class TYPE>
istream &operator>>(istream &is, matrix<TYPE> &m) //input
{
  long r,c;

  m.clear();
  is>>m.row>>m.col;

  m.entry = new TYPE*[m.row];

  for(r=1;r<=m.row;r++)
    {
      m.entry[r-1] = new TYPE[m.col];

      for(c=1;c<=m.col;c++)
        is>>m(r,c);
    }
  
  return is;
}


  
template<class TYPE>
matrix<TYPE> operator+(matrix<TYPE> &m1, matrix<TYPE> &m2) //add
{
  matrix<TYPE> tmp(m1);
  long r,c;

  if ((m1.rows() == m2.rows()) && (m1.cols() == m2.cols()))
    {
      for(r=1;r<=m2.rows();r++)
	for(c=1;c<=m2.cols();c++)
	  tmp(r,c) +=  m2(r,c);

      return tmp;
    }//end then

  cerr<<"\nCannot add 2 matricies of different sizes."<<endl;
  exit(0);
}

template<class TYPE>
matrix<TYPE> operator*(matrix<TYPE> &m1, matrix<TYPE> &m2) //mult
{
  matrix<TYPE> tmp(m1.rows(),m2.cols());
  long r,c,p;

  if (m1.cols() == m2.rows())
    {
      for(r=1;r<=m1.rows();r++)
	for(c=1;c<=m2.cols();c++)
	  {
	    tmp(r,c) =  0;
            for(p=1;p<=m1.col;p++)
	      tmp(r,c) += (m1(r,p) * m2(p,c));
          }
      return tmp;
    }//end then

  cerr<<"\nCannot multiply matricies of incomptable sizes."<<endl;
  exit(0);

}


template<class TYPE>
int operator==(matrix<TYPE> &m1, matrix<TYPE> &m2) //equality
{
  long r,c;

  if ((m1.rows() == m2.rows()) && (m1.cols() == m2.cols()))
    {
      for(r=1;r<=m1.rows();r++)
	for(c=1;c<=m1.cols();c++)
	  if (m1(r,c) != m2(r,c))
	    return 0;

      return 1;
    }
  else
    return 0;
}

template<class TYPE>
int operator!=(matrix<TYPE> &m1, matrix<TYPE> &m2) //in-equality
{
  return (!(m1==m2));
}



template<class TYPE>
matrix<TYPE>::matrix(const long &r, const long &c) 
{
  long i;

  row = r;
  col = c;

  entry = new TYPE*[r];
  for(i=0;i<row;i++)
    entry[i] = new TYPE[c];

}

template<class TYPE>
matrix<TYPE>::matrix(matrix<TYPE> &m)
{
  long r,c;

  row = m.row;
  col = m.col;

  entry = new TYPE*[row];
  for(r=0;r<row;r++)
    entry[r] = new TYPE[col];

  for(r=1;r<=row;r++)
    for(c=1;c<=col;c++)
      (*this)(r,c) = m(r,c);

}

template<class TYPE>
matrix<TYPE>::~matrix() 
{
  long r;

  for(r=0;r<row;r++)
    delete [] entry[r];
  delete [] entry;

}

template<class TYPE>
void matrix<TYPE>::clear()
{
  long r;

  for(r=0;r<row;r++)
    delete [] entry[r];
  delete [] entry;

  row = col = 0;
}


template<class TYPE>
matrix<TYPE> &matrix<TYPE>::operator=(matrix<TYPE> &m) //assignment
{
  long r,c;

  if (this != &m)
    {
      clear();
      row = m.rows();
      col = m.cols();

      entry = new TYPE*[row];

      for(r=1;r<=row;r++)
        {
          entry[r-1] = new TYPE[col];

          for(c=1;c<=col;c++)
            (*this)(r,c) = m(r,c);

        }
    }//end then
  
  return *this;
}

template<class TYPE>
TYPE &matrix<TYPE>::operator()(const long &r, const long &c) //index
{
  if ((r>0) && (c>0) && (r<=row) && (c<=col))
  return entry[r-1][c-1];
}

template<class TYPE>
long matrix<TYPE>::rows() const
{
  return row;
}

template<class TYPE>
long matrix<TYPE>::cols() const
{
  return col;
}

