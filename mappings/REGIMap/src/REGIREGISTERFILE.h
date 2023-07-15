/*
 * REGIREGISTERFILE.h
 *
 *  Created on: Aug 10, 2012
 *      Author: mahdi
 */
#ifndef REGIREGISTERFILE_H_
#define REGIREGISTERFILE_H_

class REGI_REGISTER_FILE
{
    public:
        REGI_REGISTER_FILE(int size);
        virtual ~REGI_REGISTER_FILE();

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

#endif /* REGIREGISTERFILE_H_ */
