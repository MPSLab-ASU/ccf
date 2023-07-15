/* Mapping Info
 *
 * Author: Mahesh Balasubramanian
 * Date: May, 2020
 *
 * Func: Used to track information of mapping.
 *       
 *      
 *
 */

#ifndef MAPPING_INFO_H_
#define MAPPING_INFO_H_


class Mapping_Info
{
  public:
    Mapping_Info() { mapped = false; I=0; J=0; T=0; };
    ~Mapping_Info() { };

  private:
    bool mapped;
    int I;
    int J;
    int T; 
    
  public:
    void set_mapped(int I, int J, int T) { mapped = true; this->I = I; this->J = J; this->T = T; } ; 
    bool is_Mapped() {return mapped; };
    void get_mapped( int &X, int &Y, int &Z) { X = I; Y = J; Z = T; }
    
}; 

#endif
