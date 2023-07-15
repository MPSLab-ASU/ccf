/*
 * REGISchedulingInfo.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: mahdi
 */

#include "REGISchedulingInfo.h"

REGI_Scheduling_Info::~REGI_Scheduling_Info()
{
}

REGI_Scheduling_Info::REGI_Scheduling_Info()
{
    ASAP = -1;
    ALAP = -1;
    CURRENT = -1;
    Feasible_ASAP = -1;
    Feasible_ALAP = -1;
    ASAP_initiated = false;
    ALAP_initiated = false;
    CURRENT_initiated = false;
    Feasible_ASAP_initiated = false;
    Feasible_ALAP_initiated = false;
    slack = 0;
    Modulo_Current = 0;
    SCC_Delay = 0;

}

void REGI_Scheduling_Info::set_ASAP(int time)
{
    ASAP_initiated = true;
    ASAP = time;
}

void REGI_Scheduling_Info::set_ALAP(int time)
{
    ALAP_initiated = true;
    ALAP = time;
}

void REGI_Scheduling_Info::set_Height(int time)
{
    Height_initiated = true;
    Height = time;
}

void REGI_Scheduling_Info::set_Current(int time, int II)
{
    CURRENT_initiated = true;
    CURRENT = time;
    Modulo_Current = CURRENT % II;
    if (II == 1)
    {
        Modulo_Current = 0;
    }
}

void REGI_Scheduling_Info::set_Feasible_ASAP(int time)
{
    Feasible_ASAP_initiated = true;
    Feasible_ASAP = time;
}

void REGI_Scheduling_Info::set_Feasible_ALAP(int time)
{
    Feasible_ALAP_initiated = true;
    Feasible_ALAP = time;
}

int REGI_Scheduling_Info::get_ASAP()
{
    return ASAP;
}

int REGI_Scheduling_Info::get_ALAP()
{
    return ALAP;
}

int REGI_Scheduling_Info::get_Height()
{
    return Height;
}
int REGI_Scheduling_Info::get_Current()
{
    return CURRENT;
}

int REGI_Scheduling_Info::get_Modulo_Current()
{
    return Modulo_Current;
}

int REGI_Scheduling_Info::get_Feasible_ASAP()
{
    return Feasible_ASAP;
}

int REGI_Scheduling_Info::get_Feasible_ALAP()
{
    return Feasible_ALAP;
}

bool REGI_Scheduling_Info::is_ASAP_Initiated()
{
    return ASAP_initiated;
}

bool REGI_Scheduling_Info::is_ALAP_Initiated()
{
    return ALAP_initiated;
}

bool REGI_Scheduling_Info::is_Height_Initiated()
{
    return Height_initiated;
}
bool REGI_Scheduling_Info::is_Feasible_ASAP_Initiated()
{
    return Feasible_ASAP_initiated;
}

bool REGI_Scheduling_Info::is_Feasible_ALAP_Initiated()
{
    return Feasible_ALAP_initiated;
}

bool REGI_Scheduling_Info::is_CURRENT_Initiated()
{
    return CURRENT_initiated;
}

void REGI_Scheduling_Info::reset_Current()
{
    CURRENT_initiated = false;
    CURRENT = -1;
}

void REGI_Scheduling_Info::reset_Feasible_ASAP()
{
    Feasible_ASAP_initiated = false;
    Feasible_ASAP = -1;
}

void REGI_Scheduling_Info::reset_Feasible_ALAP()
{
    Feasible_ALAP_initiated = false;
    Feasible_ALAP = -1;
}
void REGI_Scheduling_Info::reset_ASAP()
{
    ASAP_initiated = false;
    ASAP = -1;
}
void REGI_Scheduling_Info::reset_ALAP()
{
    ALAP_initiated = false;
    ALAP = -1;
}

void REGI_Scheduling_Info::set_slack()
{
    slack = Feasible_ALAP - Feasible_ALAP;
}

void REGI_Scheduling_Info::set_slack(int time)
{
    slack = Feasible_ALAP - time;
}
void REGI_Scheduling_Info::update_slack(int time)
{
    slack = time - Feasible_ASAP;
}
int REGI_Scheduling_Info::get_slack()
{
    return slack;
}
void REGI_Scheduling_Info::set_SCC_Delay(int delay)
{
    SCC_Delay = delay;
}

int REGI_Scheduling_Info::get_SCC_Delay()
{
    return SCC_Delay;
}
