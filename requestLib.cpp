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


void loadRequests(char* fName, L1List<VM_Request> &rList) {
    // TODO: write your code to load requests. Each request is separated by a whitespace
	ifstream ifile;
	ifile.open(fName);

	VM_Request temp;
	if(ifile.good())
	{
		string str;//luu cac phan khong can thiet
		bool b = true;
		while(getline(ifile,str))
		{
			istringstream ss(str);
			while( ss >> str){
				strcpy((temp.code), str.c_str());
				if(temp.code[strlen(temp.code)-1] == ';'){
					temp.code[strlen(temp.code)-1] = '\0';
					rList.push_back(temp);
					b =false;
					break;
				}
				else rList.push_back(temp);
			}
			if(b == false) break;
		}
	}
	ifile.close();
}
