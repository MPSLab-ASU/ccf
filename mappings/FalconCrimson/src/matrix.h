/* Simple implementation of Matrix
 * 
 *
 * Author: Mahesh Balasubramanian
 */


#ifndef MATRIX_H
#define MATRIX_H

#include<vector>
#include<iostream>
#include<assert.h>

using namespace std;

template <typename Object>
class Matrix 
{
  private:
      vector<vector<Object>> array;
  
  public:
      Matrix( ) : array(0)
      {
         for(auto & thisRow : array)
          thisRow.resize(0);
      }
      Matrix( int rows, int cols ) : array ( rows )
      {
        for(auto & thisRow : array)
          thisRow.resize( cols );
      }
      Matrix( vector<vector<Object>> v) : array( v )
      { }
      Matrix( vector<vector<Object>> && v ) : array(std::move( v ))
      { }
      
      void reshape( int rows, int cols )
      {
        array.resize( rows ); 
               
        for(auto & thisRow : array)
          thisRow.resize( cols );
          
      }
      vector<Object> row( int ith )
      {
        vector<Object> ret; 
        
        for(int j = 0; j < (int) cols(); j++)
          ret.push_back(array[ith][j]);  
        
        return ret;
      }
      bool row_is_zero( int ith )
      {
        for(int j = 0; j < (int) cols(); j++)
          if(array[ith][j] > 0)
            return false;

        return true;
      }
      bool find_element( int value)
      {
        for(int i=0; i < (int) rows(); i++)
          for( int j=0; j < (int) cols(); j++)
            if(array[i][j] == value)
              return true; 

        return false;
      }
      vector<int> row_find( int ith, int value )
      {
        vector<int> ret;
        for(int j = 0; j < (int) cols(); j++)
          if(array[ith][j] == value)
            ret.push_back(j);

        return ret;
      }
      int row_find( int value )
      {
        for(int i=0; i<(int) rows(); i++)
          for(int j=0; j<(int) cols(); j++)
            if(array[i][j]==value)
              return i;
        return -1;  
      }
      void set_element_zero(int ith, int jth)
      { 
        //for(int j = 0; j < (int) cols(); j++)
          array[ith][jth] = 0; 
      }
      void set_row_zero(int ith)
      {
        for(int j = 0; j < (int) cols(); j++)
          array[ith][j] = 0;
      }
      vector<Object> col( int ith )
      { 
        vector<Object> ret;

        for(int j = 0; j < (int) rows(); j++)
          ret.push_back(array[j][ith]);
        
        return ret;
      }
      bool col_is_zero( int ith )
      {
        for(int j = 0; j < (int) rows(); j++)
          if(array[j][ith] > 0)
            return false;

        return true;
      }
      vector<int> col_find( int ith, Object value )
      {
        vector<int> ret;
        for(int j = 0; j < (int) rows(); j++)
          if(array[j][ith] == value)
            ret.push_back(j);

        return ret;
      }
      int col_find( int value )
      {
        for(int i=0; i<(int) rows(); i++)
          for(int j=0; j<(int) cols(); j++)
            if(array[i][j]==value)
              return j;
        return -1;
      }
      void set_col_zero(int jth)
      {
        for(int j = 0; j < (int) rows(); j++)
          array[j][jth] = 0;

      }
      void empty()
      {
        array.clear();  
      } 
      // Function for TEC.
      vector<pair<int, int> > find_empty_PEs(int time, int SizeX)
      {
        vector<pair<int, int>> pe; 
        for(int i= time*SizeX; i<((time*SizeX)+SizeX); i++)
          for(int j=0; j<(int) cols(); j++)
            if(array[i][j] == INF)
              pe.push_back(make_pair(i, j)); 
            
        return pe;
      }
      vector<int> find_mapped_coordinates(int time, int SizeX)
      {
        vector<int> mapped;
        for(int i= time*SizeX; i<((time*SizeX)+SizeX); i++)
          for(int j=0; j<(int) cols(); j++)
            if(array[i][j] != INF)
              mapped.push_back(array[i][j]);

        return mapped;
      }
      void free_current_timeslot(int time, int SizeX)
      { 
        for(int i= time*SizeX; i<((time*SizeX)+SizeX); i++)
          for(int j=0; j<(int) cols(); j++)
              array[i][j] = INF; 
      }
      Matrix<int> copy_timeslot (int time, int SizeX)
      {
        Matrix<int> timeslot(SizeX, cols()); 
        for(int i=0; i<SizeX; i++)
          for(int j=0; j<(int) cols(); j++)
            timeslot[i][j] = array[(time*SizeX)+i][j]; 
              
        return timeslot; 
      }   
      void restore_timeslot(int time, int SizeX, Matrix<int> timeslot)
      {
         for(int i=0; i<SizeX; i++)
          for(int j=0; j<(int) cols(); j++)
            array[(time*SizeX)+i][j] = timeslot[i][j];  
      }
      void restore(Matrix<int> rhs)
      {
          array.clear(); 
          for(int thisRow=0; thisRow  < (int) rhs.rows(); thisRow++)
            this->array[thisRow] = rhs[thisRow];
      }
      const vector<Object> & operator[]( int row ) const
      { return array[row]; }
  
      vector<Object> & operator[]( int row )
      { return array[row]; } 

      int rows() const
      { return array.size(); }
  
      int cols() const
      { return rows( ) ? array[0].size() : 0;}
  
      Matrix<Object>& operator=(Matrix<Object> rhs)
      {
        
        for(int thisRow=0; thisRow  < rhs.rows(); thisRow++)
            this->array[thisRow] = rhs[thisRow];
      
        return *this;
      }

      Matrix<Object> operator*(Matrix<Object> & rhs)
      { 
        int thisRow = rows();
        int thisCols = cols();
        int rhsRows = rhs.rows();
        int rhsCols = rhs.cols(); 
      
        assert(thisCols == rhsRows); // multiplication assertion

        Matrix<Object> ret(thisRow, rhsCols);

        for(int i=0; i  < thisRow; i++)
          for(int j = 0; j < rhsCols; j++)
              for(int k=0; k < thisCols; k++)
                ret[i][j] += this->array[i][k] * rhs[k][j];
        
        return ret;
      }
  
      Matrix<Object> transpose()
      {
        Matrix<Object> ret(cols(), rows());
    
        for( int thisRow = 0; thisRow < rows(); thisRow++)
          for(int thisCol = 0; thisCol < cols(); thisCol++)
            ret[thisCol][thisRow] = this->array[thisRow][thisCol];
  
        return ret;
      }
      
      void fill(Object value)
      {
        for( int thisRow = 0; thisRow < rows(); thisRow++)
          for(int thisCol = 0; thisCol < cols(); thisCol++)
            array[thisRow][thisCol] = value;
      }
      void set_zero()
      {
        for( int thisRow = 0; thisRow < rows(); thisRow++)
          for(int thisCol = 0; thisCol < cols(); thisCol++)
            array[thisRow][thisCol] = 0;
      }

      void set_ones()
      {
        for( int thisRow = 0; thisRow < rows(); thisRow++)
          for(int thisCol = 0; thisCol < cols(); thisCol++)
            array[thisRow][thisCol] = 1;
      }

      void set_value(int i, int j, int value)
      {
        array[i][j] = value;

      }

      bool is_equal(Matrix<Object> & rhs)
      {
        if(rows() != rhs.rows() || cols() != rhs.cols())
            return false;
        
        for( int thisRow = 0; thisRow < rows(); thisRow++)
          for(int thisCol = 0; thisCol < cols(); thisCol++)
            if(this->array[thisRow][thisCol] != rhs[thisRow][thisCol])
              return false;

        return true;
      }

      void print_matrix()
      {
        for( int thisRow = 0; thisRow < rows(); thisRow++)
        {
          for(int thisCol = 0; thisCol < cols(); thisCol++) 
            cout << array[thisRow][thisCol] << "  \t";
          cout<<endl;
        }
      }
      void print_row(int i)
      {
        for(int thisCol = 0; thisCol < cols(); thisCol++)
            cout << array[i][thisCol] << " ";

      }
      void print_col(int j)
      {
        for( int thisRow = 0; thisRow < rows(); thisRow++)
          cout << array[thisRow][j] << " ";
      }
};

#endif

