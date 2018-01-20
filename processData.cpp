/*
 * ==========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 2 - Data structures and Algorithms - Fall 2017
 * ==========================================================================================
 */
#include <iostream>
#include <vector>
#include <functional>
#include <math.h>
#include <cstdlib>
#include "requestLib.h"
#include "dbLib.h"


using namespace std;

#define GPS_DISTANCE_ERROR 0.005


bool initVMGlobalData(void** pGData) {
    // TODO: allocate and initialize global data
    // return false if failed
    return true;
}

void releaseVMGlobalData(void* pGData) {
    // TODO: do your cleanup, left this empty if you don't have any dynamically allocated data

}

void R1(MyAVLTree* root, char* request){

}

void R2(MyAVLTree* root, char* request){
	char dir = request[strlen(request)-1];
	if(dir != 'W' && dir != 'E'){
		cout << request << ": -1" << endl;
		return;
	}
	double Along;
	const char* delim = "_";
	Along = atof(strtok(&request[2],delim));// lay kinh do long
	AVLNode<VM_Record,double>* lroot = root->getRoot()->longt.getRoot();

}

bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData) {
    // TODO: Your code goes here
    // return false for invlaid events
	MyAVLTree* rTree;
	rTree = (MyAVLTree*)recordList.getVoid();

	switch(request.code[0]){
	case '1':
		R1(rTree,request.code); break;
	case '2':
		R2(rTree,request.code); break;
	default: return false;
	}
    return true;
}


