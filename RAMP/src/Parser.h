/*
 * Parser.h
 *
 * Created on: Sep 5, 2012
 * Author: mahdi
 *
 * Last edited: 26 Nov 2016
 * Author: Shail Dave
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "CGRA.h"
#include <fstream>
#include <sstream>

class Parser
{
    public:
        Parser(string nodeFile, string edgeFile);
        bool ParseDFG(DFG* myDFG);
        virtual ~Parser();

        string getNodeFileName();
        string getEdgeFileName();
    private:
        string nodeFile;
        string edgeFile;

};

#endif /* PARSER_H_ */
