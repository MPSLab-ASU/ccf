/*
* SchedulingInfo.h
*
* Created on: Jul 24, 2012
* Author: mahdi

* Last edited on: 26 Nov, 2016
* Author: Shail Dave
*/

#ifndef SCHEDULINGINFO_H_
#define SCHEDULINGINFO_H_

class Scheduling_Info
{
  public:
    Scheduling_Info();
    virtual ~Scheduling_Info();

  private:
    int ASAP;
    int ALAP;
    int Feasible_ASAP;
    int Feasible_ALAP;

    int Height;
    int CURRENT;
    int Modulo_Current;

    int SCC_Delay;

    bool ASAP_initiated;
    bool ALAP_initiated;
    bool Height_initiated;
    bool CURRENT_initiated;
    bool Feasible_ASAP_initiated;
    bool Feasible_ALAP_initiated;

    int slack;
    int II;
  public:

    void set_ASAP(int time);
    void set_ALAP(int time);
    void set_Height(int time);

    void set_Feasible_ASAP(int time);
    void set_Feasible_ALAP(int time);
    void set_Current(int time, int II);
    void reset_Current();
    void reset_Feasible_ASAP();
    void reset_Feasible_ALAP();
    void reset_ASAP();
    void reset_ALAP();

    int get_ASAP();
    int get_ALAP();

    int get_Height();
    int get_Feasible_ASAP();
    int get_Feasible_ALAP();
    int get_Current();
    int get_Modulo_Current();
    bool is_ASAP_Initiated();
    bool is_ALAP_Initiated();
    bool is_Height_Initiated();
    bool is_Feasible_ASAP_Initiated();
    bool is_Feasible_ALAP_Initiated();
    bool is_CURRENT_Initiated();

    void set_slack(int time);
    void set_slack();
    void update_slack(int time);
    int get_slack();

    void set_SCC_Delay(int delay);
    int get_SCC_Delay();
    int get_II();
};

#endif /* SCHEDULINGINFO_H_ */
