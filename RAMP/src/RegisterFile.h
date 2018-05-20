/*
* RegisterFile.h
*
* Created on: Aug 10, 2012
* Author: mahdi
*
*  Author: Shail Dave
*  Last Edited on: 26 Nov 2016
*/

#ifndef REGISTERFILE_H_
#define REGISTERFILE_H_

class REGISTER_FILE
{
public:
  REGISTER_FILE(int size);
  virtual ~REGISTER_FILE();

  bool Reset();
  bool isAvailable();
  bool Assign();

  void Allocate(int n);
  void Release(int i);

  int get_No_Used_Register();
  int getAvailableNumber();

private:
  int size;
  int index;
};

#endif /* REGISTERFILE_H_ */
