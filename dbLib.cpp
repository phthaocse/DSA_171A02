/*
 * =========================================================================================
 * Name        : dbLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This file implements functions used for database management
 * =========================================================================================
 */

#include "dbLib.h"

#include <time.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#define __PI 3.14159265358979323846
#define earthRadiusKm 6371.0

using namespace std;

void    strPrintTime(char* des, time_t& t) {
    tm *pTime = gmtime(&t);
    strftime(des, 26, "%Y-%m-%d %H:%M:%S", pTime);
}


struct MyAVLNode{
	char			_ID[ID_MAX_LENGTH];
	AVLTree<VM_Record,time_t>  timet;//
	AVLTree<VM_Record,double>   longt;
	AVLTree<VM_Record,double>  latt;
	MyAVLNode   *_pLeft, *_pRight;
	int         _bFactor;
	MyAVLNode(VM_Record& data) :  _pLeft(NULL), _pRight(NULL), _bFactor(0) {
		strcpy(_ID,data.id);
		timet.insert(data,data.timestamp);
		longt.insert(data, data.latitude);
		latt.insert(data, data.longitude);
	}
};


class MyAVLTree{
	 MyAVLNode *_pRoot;
public:
	 MyAVLTree() : _pRoot(NULL) {}
	~MyAVLTree() { destroy(_pRoot); }

	 bool insert(VM_Record& data){ return _insert(_pRoot, data); }

protected:
	 void destroy(MyAVLNode* &pR);
	 bool _insert(MyAVLNode* &pR, VM_Record& data);

	 void _rotLeft(MyAVLNode* &pR);
	 void _rotRight(MyAVLNode* &pR);
	 void _rotLR(MyAVLNode* &pR);
	 void _rotRL(MyAVLNode* &pR);

	 bool _balanceLeft(MyAVLNode* &pR);
	 bool _balanceRight(MyAVLNode* &pR);
};


bool MyAVLTree::_insert(MyAVLNode* &pR, VM_Record& data){
	if(pR == NULL){
		pR = new MyAVLNode(data);
		return true;
	}
	if(strcmp(data.id,pR->_ID) == 0){
		pR->latt.insert(data,data.latitude);
		pR->longt.insert(data,data.longitude);
		pR->timet.insert(data,data.timestamp);
		return false;
	}
	if(strcmp(data.id,pR->_ID) < 0){
		if(_insert(pR->_pLeft,data) == false) return false;
		return _balanceLeft(pR);
	}
	else{
		if(_insert(pR->_pRight,data) == false) return false;
		return _balanceRight(pR);
	}
}

void loadVMDB(char* fName, L1List<VM_Record> &db) {
    ifstream inFile(fName);

    if (inFile) {
        string line;
        getline(inFile , line);// skip the first line
        VM_Record record;

        db.insertHead(record);/// add dummy object
        MyAVLTree recordTree;
        while (getline(inFile , line)) {
            /// On Windows, lines on file ends with \r\n. So you have to remove \r
            if (line[line.length() - 1] == '\r')
                line.erase(line.length() - 1);
            if (line.length() > 0) {
                if (parseVMRecord((char*)line.data(), record))/// parse and store data directly
                   // db.insertHead(record);/// add dummy object for next turn
                	recordTree.insert(record);
            }
        }
        db.removeHead();/// remove the first dummy

     //   db.reverse();
        inFile.close();
    }
    else {
        cout << "The file is not found!";
    }
}

bool parseVMRecord(char *pBuf, VM_Record &bInfo) {
    // TODO: write code to parse a record from given line
}

void process(L1List<VM_Request>& requestList, L1List<VM_Record>& rList) {
    void*   pGData = NULL;
    initVMGlobalData(&pGData);

    while (!requestList.isEmpty()) {
        if(!processRequest(requestList[0], rList, pGData))
            cout << requestList[0].code << " is an invalid event\n";
        requestList.removeHead();
    }

    releaseVMGlobalData(pGData);
}

void printVMRecord(VM_Record &b) {
    printf("%s: (%0.5f, %0.5f), %s\n", b.id, b.longitude, b.latitude, ctime(&b.timestamp));
}

/// This function converts decimal degrees to radians
inline double deg2rad(double deg) {
    return (deg * __PI / 180);
}

///  This function converts radians to decimal degrees
inline double rad2deg(double rad) {
    return (rad * 180 / __PI);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}
