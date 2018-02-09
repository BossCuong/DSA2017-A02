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
	ifstream inFile;
	inFile.open(fName);
	if (inFile)
	{
        VM_Request request;
		rList.insertHead(request); /// add dummy object
		string str;
		while (inFile)
		{
			if (inFile >> str)
			{
				if (str[str.length() - 1] == ';')
					str.erase(str.length() - 1);
				strcpy(rList[0].code,(char*)str.data());
				rList.insertHead(request);
			}
		}
		rList.removeHead(); /// remove the first dummy
        rList.reverse();
	}
	else
	{
		cout << "The file is not found!";
		return;
	}
	inFile.close();
}