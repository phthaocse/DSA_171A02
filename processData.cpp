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
	MyAVLNode* tmp = new MyAVLNode();
	if(root->findID(ID,tmp)) return true;
	return false;
}

//tm1 > tm2 return 1; tm1 = tm2 return 0; tm1 < tm2 return -1
int compareTime(struct tm& tm1, struct tm& tm2){
	if(tm1.tm_hour > tm2.tm_hour) return 1;
	else if(tm1.tm_hour == tm2.tm_hour){
		if(tm1.tm_min > tm2.tm_min) return 1;
		else if(tm1.tm_min == tm2.tm_min){
			if(tm1.tm_sec > tm2.tm_sec) return 1;
			else if(tm1.tm_sec == tm2.tm_sec) return 0;
			else return -1;
		}
		else return -1;
	}
	else return -1;
}
bool process0(AVLNode<VM_Record,time_t>* timeRoot,struct tm& timeI,VM_Record& ret){
	if(timeRoot == NULL) {return false;}
	struct tm* timeIn;
	timeIn = gmtime(&timeRoot->_key);
	//cout << timeIn->tm_hour << timeIn->tm_min << timeIn->tm_sec << endl;
	if(compareTime(*timeIn,timeI) == 1) {
		printVMRecord(timeRoot->_data);
		return process0(timeRoot->_pLeft,timeI,ret);
	}
	else if(compareTime(*timeIn,timeI) == -1){
		printVMRecord(timeRoot->_data);
		return process0(timeRoot->_pRight,timeI,ret);
	}
	else{
		ret = timeRoot->_data;
		return true;
	}
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

    const char* delim = "_";
    char* id1, *id2, _time[6];
    memcpy(_time,&request[12],6);
    id1 = strtok(&request[2],delim);
    id2 = strtok(&request[7],delim);
    _time[6] = '\0'; id1[4] = '\0';id2[4] = '\0';
    if(!isIdValid(id1,root) || !isIdValid(id2,root)){
    	cout << "1: -1" << endl;
    	return;
    }
    //Xu ly thoi gian
    int t = atoi(_time);
    struct tm tmp;
    tmp.tm_hour = t/10000;
    tmp.tm_min = t/100 - (t/10000)*100;
    tmp.tm_sec = t - (t/100)*100;
	cout <<  tmp.tm_hour <<   tmp.tm_min << tmp.tm_sec << endl;
    MyAVLNode* data1 = new MyAVLNode();
    MyAVLNode* data2 = new MyAVLNode();
    root->findID(id1,data1);
    root->findID(id2,data2);

    VM_Record rec1,rec2;
    if(!process0(data1->timet.getRoot(),tmp,rec1) || !process0(data2->timet.getRoot(),tmp,rec2) ){
    	cout << "1: -1" << endl;
    	return;
    }

    double dis = distanceEarth(rec1.latitude,rec1.longitude,rec2.latitude,rec2.longitude);
    char dir1,dir2;
    dir1 = (rec1.longitude - rec2.longitude >= 0) ? 'E':'W';
    dir2 = (rec1.latitude - rec2.latitude>= 0) ? 'N':'S';
    cout << "1: " << dir1 << " "<< dir2 << " " << dis << endl;


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


