/*
 * Parser.h
 *
 *  Created on: Sep 5, 2012
 *      Author: mahdi
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "CGRA.h"
#include <fstream>
#include <sstream>


class Parser
{
    public:
    public:
        Parser(string nodeFile, string edgeFile);
        bool ParseDFG(REGI_DFG* myDFG);
        virtual ~Parser();

        string getFileName();
    private:
        string nodeFile;
        string edgeFile;

};

#endif /* PARSER_H_ */
