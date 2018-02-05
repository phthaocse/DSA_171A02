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
//	if(strlen(ID) != 4) return false;
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
		return process0(timeRoot->_pLeft,timeI,ret);
	}
	else if(compareTime(*timeIn,timeI) == -1){
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
			if(lroot->_data.longitude<= Along){
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
			if(lroot->_data.latitude<= Alat){
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
void process3_(AVLNode<VM_Record,time_t>* timeRoot, int& time1, int& time2,double Along, double Alat, double&R, bool& b){
	if(timeRoot == NULL) return;
	struct tm* timeIn;
	timeIn = gmtime(&timeRoot->_key);
	if(timeIn->tm_hour >= time1){
		if(timeIn->tm_hour < time2 || (timeIn->tm_hour == time2 && timeIn->tm_hour == 0 && timeIn->tm_sec == 0)){
			double dis = distanceEarth(Alat,Along,timeRoot->_data.latitude,timeRoot->_data.longitude);
			if(dis <= R) {b = true; return;}
		}
	}
	process3_(timeRoot->_pLeft,time1,time2,Along,Alat,R,b);
	process3_(timeRoot->_pRight,time1,time2,Along,Alat,R,b);

}

void process3(MyAVLNode* pR, int& n,double Along, double Alat,double R, int time1, int time2){
	if(pR == NULL) return;
	AVLNode<VM_Record,time_t>* timeRoot = pR->timet.getRoot();
	bool b = false;
	process3_(timeRoot,time1,time2,Along,Alat,R,b);
	if(b){  n++;}
	process3(pR->_pLeft,n,Along,Alat,R,time1,time2);
	process3(pR->_pRight,n,Along,Alat,R,time1,time2);

}
void R1(MyAVLTree* root, char* request){
	if(root == NULL) return;
	//xu ly request
    const char* delim = "_";
    char* id1, *id2, _time[6];

    id1 = strtok(&request[2],delim);
    id1[strlen(id1)] = '\0';
    id2 = strtok(&request[2+strlen(id1)+1],delim);
    id2[strlen(id2)] = '\0';
    memcpy(_time,&request[2+strlen(id1)+strlen(id2)+2],6);
    _time[6] = '\0';

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
		cout << "2: -1" << endl;
		return;
	}

	double Along;
	const char* delim = "_";
	Along = atof(strtok(&request[2],delim));// lay kinh do long
	MyAVLNode* pR = root->getRoot();
	int n = 0;
	int type = (dir == 'W')? 0:1;
	process1(pR,n,type,Along);
	cout << "2: " <<  n << endl;
}

void R3(MyAVLTree* root, char* request){
	char dir = request[strlen(request)-1];
	if(dir != 'N' && dir != 'S'){
		cout << "3: -1" << endl;
		return;
	}
	double Alat;
	const char* delim = "_";
	Alat = atof(strtok(&request[2],delim));// lay kinh do long
	MyAVLNode* pR = root->getRoot();
	int n = 0;
	int type = (dir == 'S')? 0:1;
	process2(pR,n,type,Alat);
	cout << "3: " << n << endl;
}

void R4(MyAVLTree* root, char* request){
	double Along,Alat,R;
	int Time1, Time2;
	char* along,*alat,*time1,*r;
	const char* delim = "_";
	int rsize = strlen(request);

	along = strtok(&request[2],delim);
	along[strlen(along)] = '\0';
	alat = strtok(&request[2+strlen(along)+1],delim);
	alat[strlen(alat)] = '\0';
	r = strtok(&request[2+strlen(along)+strlen(alat)+2],delim);
	r[strlen(r)] = '\0';
	time1 = strtok(&request[2+strlen(along)+strlen(alat)+strlen(r)+3],delim);
	time1[strlen(time1)] = '\0';
	int tmpsize = 2+strlen(along)+strlen(alat)+strlen(r)+strlen(time1)+4;
	char time2[rsize - tmpsize];
	memcpy(time2,&request[tmpsize],rsize - tmpsize);
	time2[strlen(time2)] = '\0';
	Along = atof(along);
	Alat = atof(alat);
	R = atof(r);
	Time1 = atoi(time1);
	Time2 = atoi(time2);
	//cout << Along << " " << Alat << " " << R << " " << Time1 << " " << Time2 << endl;
	int result = 0;
	process3(root->getRoot(),result,Along,Alat,R,Time1,Time2);
	cout <<"4: " << result <<endl;
}

void process4(AVLNode<VM_Record,time_t>* timeRoot,double Along,double Alat,double R,int& state,int& n){
	if(timeRoot == NULL) return;
	process4(timeRoot->_pLeft,Along,Alat,R,state,n);
	double dis = distanceEarth(Alat,Along,timeRoot->_data.latitude,timeRoot->_data.longitude);
	if(dis <= R){
		if(state == -1){
			state = 0;//nam trong tram
			n++;
		}
		else if(state == 1){
			state = 0;
			n++;
		}
	}
	else{
		if(state == -1) state = 1;
		else if(state == 0) state = 1;
	}

	process4(timeRoot->_pRight,Along,Alat,R,state,n);
}

void R5(MyAVLTree* root, char* request){
	double Along,Alat,R;
	char* along,*alat,*id,*r = new char();
	const char* delim = "_";
	int rsize = strlen(request);

	id = strtok(&request[2],delim);
	id[strlen(id)] = '\0';
	along = strtok(&request[2+strlen(id)+1],delim);
	along[strlen(along)] = '\0';
	alat = strtok(&request[2+strlen(along)+strlen(id)+2],delim);
	alat[strlen(alat)] = '\0';
	int tmpsize = 2+strlen(along)+strlen(id)+strlen(alat)+3;
	memcpy(r,&request[tmpsize],rsize - tmpsize);
	r[strlen(r)] = '\0';
	Along = atof(along);
	Alat = atof(alat);
	R = atof(r);

	MyAVLNode* pR;
	if(!root->findID(id,pR)){
		cout << "5: " << endl;
		return;
	}

	int state = -1; // khoi tao trang thai ban dau
	int n = 0;
	process4(pR->timet.getRoot(),Along,Alat,R,state,n);
//	cout << id << " " << R << " " << Along << " " << Alat << endl;
	cout <<"5: " << n <<endl;

}
bool testRequest(VM_Request & request){
	if(request.code[strlen(request.code)-1] == '_') return false;
	int ndelim = 0;
	for(int i = 0; i < strlen(request.code);i++){
		if(request.code[i] == '_') ndelim++;
	}
	switch(request.code[0]){
	case '1':
		if(ndelim != 3) return false;break;
	case '2':{
		if(ndelim != 2) return false;
		else{
			char dir = request.code[strlen(request.code)-1];
			if(dir != 'W' && dir != 'E') return false;
		}
		break;
	}
	case '3':{
		if(ndelim != 2) return false;
		else{
			char dir = request.code[strlen(request.code)-1];
			if(dir != 'N' && dir != 'S') return false;
		}
		break;
	case '4':
		if(ndelim != 5) return false;break;
	case '5':
		if(ndelim != 4) return false;break;
	}
	default: return true;
	}
	return true;
}

bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData) {
    // TODO: Your code goes here
    // return false for invlaid events
	if(!testRequest(request)) return false;

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
	case '4':
		R4(rTree,request.code); break;
	case '5':
		R5(rTree,request.code); break;
	default: return false;
	}
    return true;
}


