/*
* RegisterFile.cpp
*
* Created on: Aug 10, 2012
* Author: mahdi
*
*  Author: Shail Dave
*  Last Edited on: 26 Nov 2016
*/

#include "RegisterFile.h"

REGISTER_FILE::REGISTER_FILE(int size)
{
  this->size = size;
  index = 0;
}

REGISTER_FILE::~REGISTER_FILE()
{
}

bool REGISTER_FILE::Reset()
{
  if (index == 0)
    return -1;

  index--;
  return 0;
}

int REGISTER_FILE::getAvailableNumber()
{
  return size - index;
}

void REGISTER_FILE::Allocate(int n)
{
  index = n;
}

void REGISTER_FILE::Release(int i)
{
  index = index - i;
}

int REGISTER_FILE::get_No_Used_Register()
{
  return index;
}

bool REGISTER_FILE::isAvailable()
{
  return (index < size);
}

bool REGISTER_FILE::Assign()
{
  if (index == size)
    return -1;
  else
    index++;

  return 0;
}
