/*
 * =========================================================================================
 * Name        : eventLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This library contains functions used for event management
 * =========================================================================================
 */

#include "requestLib.h"

void loadRequests(char *fName, L1List<VM_Request> &rList)
{
    // TODO: write your code to load requests. Each request is separated by a whitespace
    ifstream inFile(fName);

    if (inFile)
    {
        string line;
        string buf;

        VM_Request request;

        rList.insertHead(request); /// add dummy object

        while (!inFile.eof())
        {    
            getline(inFile,line);
            if(line.empty()) continue;
            // On Windows, lines on file ends with \r\n. So you have to remove \r
            if (line[line.length() - 1] == '\r')
                line.erase(line.length() - 1);
            while(line[line.length() - 1] == ';' || line[line.length() - 1] == ' ') line.erase(line.length() - 1);
            
            stringstream stream(line);
            while (!stream.eof())
            {
                getline(stream, buf, ' ');
                if(buf.empty()) continue;
                strcpy(rList[0].code,(char *)buf.data());
                rList.insertHead(request); /// add dummy object for next turn
            }
        }
        rList.removeHead(); /// remove the first dummy

        rList.reverse();
        inFile.close();
    }
    else
    {
        cout << "The file is not found!";
    }
}