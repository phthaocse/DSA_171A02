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

bool isIdValid(char* ID, MyAVLTree* root){
	if(strlen(ID) != 4) return false;
	if(root->findID(ID)) return true;
	return false;
}

int compareTime();

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

void process2(MyAVLNode* pR, int& n,int type,double Alat){
	if(pR == NULL) return;
	AVLNode<VM_Record,double>* lroot = pR->latt.getRoot();

	if(type == 1){//North
		while(lroot){
			if(lroot->_data.latitude>= Alat) lroot = lroot->_pLeft;
			else{
				break;
			}
		}
		if(lroot == NULL){n++;}

	}
	else{//South
		while(lroot){
			if(lroot->_data.latitude< Alat){
				lroot = lroot->_pRight;
			}
			else{
				break;
			}
		}
		if(lroot == NULL){n++;}
	}
	process2(pR->_pLeft,n,type,Alat);
	process2(pR->_pRight,n,type,Alat);
}

void R1(MyAVLTree* root, char* request){
	if(root == NULL) return;
	//xu ly request
	cout << request << ": ";
    const char* delim = "_";
    char* id1, *id2, _time[6];
    memcpy(_time,&request[12],6);
    id1 = strtok(&request[2],delim);
    id2 = strtok(&request[7],delim);
    _time[6] = '\0'; id1[4] = '\0'; id2[4] = '\0';
    if(isIdValid(id1,root) && isIdValid(id2,root)){
    	cout << "-1" << endl;
    }
    //Xu ly thoi gian
    int t = atoi(_time);
    struct tm tmp;
    tmp.tm_hour = t/10000;
    tmp.tm_min = t/100 - (t/10000)*100;
    tmp.tm_sec = t - (t/100)*100;

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

void R3(MyAVLTree* root, char* request){
	char dir = request[strlen(request)-1];
	if(dir != 'N' && dir != 'S'){
		cout << request << ": -1" << endl;
		return;
	}
	cout << request << ": ";
	double Alat;
	const char* delim = "_";
	Alat = atof(strtok(&request[2],delim));// lay kinh do long
	MyAVLNode* pR = root->getRoot();
	int n = 0;
	int type = (dir == 'S')? 0:1;
	process2(pR,n,type,Alat);
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
	case '3':
		R3(rTree,request.code); break;
	default: return false;
	}
    return true;
}


