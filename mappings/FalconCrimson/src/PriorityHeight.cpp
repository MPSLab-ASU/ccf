/*
 * SchedulingInfo.cpp
 *
 *  Created on: May 13, 2012
 * Author: Mahesh 
 */

#include "PriorityHeight.h"


PriorityHeight::~PriorityHeight()
{

}

PriorityHeight::PriorityHeight()
{
  Height = 0;
  Height_initiated = false; 
}

void PriorityHeight::set_Height(int height)
{
  Height_initiated = true;
  Height = height;
}

int PriorityHeight::get_Height()
{
  return Height;
}

bool PriorityHeight::is_Height_Initiated()
{
  return Height_initiated; 
}
