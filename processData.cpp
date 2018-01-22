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

void process1(MyAVLNode* pR, int& n,int type,double Along){
	if(pR == NULL) return;
	AVLNode<VM_Record,double>* lroot = pR->longt.getRoot();
	//cout << endl << pR->_ID;

	if(type == 1){//East
		while(lroot){
			if(lroot->_data.longitude >= Along) lroot = lroot->_pLeft;
			else{
				break;
			}
		}
		if(lroot == NULL){n++;}

	}
	else{//west
		while(lroot){
			if(lroot->_data.longitude< Along){
				lroot = lroot->_pRight;
			}
			else{
				break;
			}
		}
		if(lroot == NULL){n++;}
	}
	process1(pR->_pLeft,n,type,Along);
	process1(pR->_pRight,n,type,Along);
}

void R1(MyAVLTree* root, char* request){

}

void R2(MyAVLTree* root, char* request){
	char dir = request[strlen(request)-1];
	if(dir != 'W' && dir != 'E'){
		cout << request << ": -1" << endl;
		return;
	}
	cout << request << ": ";
	double Along;
	const char* delim = "_";
	Along = atof(strtok(&request[2],delim));// lay kinh do long
	MyAVLNode* pR = root->getRoot();
	int n = 0;
	int type = (dir == 'W')? 0:1;
	process1(pR,n,type,Along);
	cout << n << endl;
}

bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData) {
    // TODO: Your code goes here
    // return false for invlaid events
	MyAVLTree* rTree;
	rTree = (MyAVLTree*)recordList.getVoid();
	if(rTree == NULL) cout << "fail" << endl;
	//rTree->printID();
	switch(request.code[0]){
	case '1':
		R1(rTree,request.code); break;
	case '2':
		R2(rTree,request.code); break;
	default: return false;
	}
    return true;
}


