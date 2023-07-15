/*
 * REGISchedulingInfo.h
 *
 *  Created on: Jul 24, 2012
 *      Author: mahdi
 */

#ifndef REGISCHEDULINGINFO_H_
#define REGISCHEDULINGINFO_H_

class REGI_Scheduling_Info
{
    public:
        REGI_Scheduling_Info();
        virtual ~REGI_Scheduling_Info();

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

};

#endif /* REGISCHEDULINGINFO_H_ */
