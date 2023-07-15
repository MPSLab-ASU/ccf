/*
 * REGIREGISTERFILE.cpp
 *
 *  Created on: Aug 10, 2012
 *      Author: mahdi
 */

#include "REGIREGISTERFILE.h"

REGI_REGISTER_FILE::REGI_REGISTER_FILE(int size)
{
    this->size = size;
    index = 0;
}

REGI_REGISTER_FILE::~REGI_REGISTER_FILE()
{

}

bool REGI_REGISTER_FILE::Reset()
{
    if (index == 0)
    {
        return -1;
    }

    index--;
    return 0;
}

int REGI_REGISTER_FILE::getAvailableNumber()
{

    return size - index;
}

void REGI_REGISTER_FILE::Allocate(int n)
{
    index = n;
}
void REGI_REGISTER_FILE::Release(int i)
{
    index = index - i;
}
int REGI_REGISTER_FILE::get_No_Used_Register()
{
    return index;
}

bool REGI_REGISTER_FILE::isAvailable()
{
    return (index < size);
}

bool REGI_REGISTER_FILE::Assign()
{
    if (index == size)
    {
        return -1;
    }
    else
        index++;
    return 0;
}
