/*
 * =========================================================================================
 * Name        : dbLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This library contains functions used for database management
 * =========================================================================================
 */

#ifndef DSA171A2_DBLIB_H
#define DSA171A2_DBLIB_H

#include <string>
#include <string.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <functional>

#include "dsaLib.h"
#include "requestLib.h"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define ID_MAX_LENGTH   16

struct VM_Record {
    char    id[ID_MAX_LENGTH];
    time_t  timestamp;
    double  longitude, latitude;

    // default constructor
    VM_Record() {
        id[0] = 0;
    }
    VM_Record(const char* busID) {
        strcpy(id, busID);
    }
    // copy constructor
    VM_Record(VM_Record& bus) : timestamp(bus.timestamp), longitude(bus.longitude), latitude(bus.latitude) {
        strcpy(id, bus.id);
    }
   // bool operator <(double& key, VM_Record& data);
   // bool operator <(time_t& key, VM_Record& data);
};



void    printVMRecord(VM_Record &);
void    strPrintTime(char* des, time_t& t);
bool    parseVMRecord(char*, VM_Record &);
void    loadVMDB(char*, L1List<VM_Record> &);
double  distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);

bool processRequest(VM_Request &, L1List<VM_Record> &, void *);// from processData.cpp

/// NOTE: student may create this function to allocate some global data
bool initVMGlobalData(void** pGData);
/// NOTE: student must defined this function if they use dynamically allocated global data.
/// If student do not use any dynamic global data, please define this function as empty function
/// in your code (file processData.cpp) as follows
/// void releaseBusGlobalData() {}
void releaseVMGlobalData(void* pGData);

void process(L1List<VM_Request>& requestList, L1List<VM_Record>& recordList);

struct MyAVLNode{
	char			_ID[ID_MAX_LENGTH];
	AVLTree<VM_Record,time_t>  timet;//
	AVLTree<VM_Record,double>   longt;
	AVLTree<VM_Record,double>  latt;
	MyAVLNode   *_pLeft, *_pRight;
	int         _bFactor;
	MyAVLNode(){};
	MyAVLNode(VM_Record& data) :  _pLeft(NULL), _pRight(NULL), _bFactor(0) {
		strcpy(_ID,data.id);
		timet.insert(data,data.timestamp);
		longt.insert(data,data.longitude);
		latt.insert(data,data.latitude);
	}
};


class MyAVLTree{
	 MyAVLNode *_pRoot;
public:
	 MyAVLTree() : _pRoot(NULL) {}
	~MyAVLTree() { destroy(_pRoot); }

	 bool insert(VM_Record& data){ return _insert(_pRoot, data); }
	 MyAVLNode* getRoot(){return _pRoot;}
	//  void traverseLNR(void (*op)(T&)) { traverseLNR(_pRoot, op); }
	 void printID(){return printID(_pRoot,0);}
	 bool findID(char* ID,MyAVLNode*& ret){ return findID(_pRoot,ID,ret);}

protected:
	 void destroy(MyAVLNode* &pR);
	 bool _insert(MyAVLNode* &pR, VM_Record& data);
	 bool findID(MyAVLNode* &pR,char* ID,MyAVLNode*& ret);
	 void printID(MyAVLNode* &pR,int level);

	// void traverseLNR(MyAVLNode*pR, void (*op)(VM_Record&));
	 void _rotLeft(MyAVLNode* &pR);
	 void _rotRight(MyAVLNode* &pR);
	 void _rotLR(MyAVLNode* &pR);
	 void _rotRL(MyAVLNode* &pR);

	 bool _balanceLeft(MyAVLNode* &pR);
	 bool _balanceRight(MyAVLNode* &pR);
};


#endif //DSA171A2_DBLIB_H
