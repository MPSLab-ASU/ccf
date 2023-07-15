/* Priority Height
 *
 * Author: Mahesh Balasubramanian
 * Date: May, 2020
 *
 * Func: This function caluclates the height
 *       based priorities for each node.
 *       Useful to check if DFG is executable on a given CGRA. 
 *
 */ 

#ifndef PRIORITYHEIGHT_H_
#define PRIORITYHEIGHT_H_


class PriorityHeight
{
  public:
    PriorityHeight();
    ~PriorityHeight();
  
  private:
    int Height; 
    bool Height_initiated; 
  public:
    void set_Height(int height);
    int  get_Height();
    bool is_Height_Initiated();
};

#endif
  
