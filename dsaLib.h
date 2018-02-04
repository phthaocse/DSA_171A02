/*
 * =========================================================================================
 * Name        : dsaLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 * =========================================================================================
 */

#ifndef A02_DSALIB_H
#define A02_DSALIB_H


#include <string>
#include <math.h>
#include <vector>
#include <cstring>


using namespace std;

#define ID_MAX_LENGTH   16

class DSAException {
    int     _error;
    string  _text;
public:

    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};


template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext;
    L1Item() : pNext(NULL) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
};

template <class T>
class L1List {
    L1Item<T>   *_pHead;// The head pointer of linked list
    size_t      _size;// number of elements in this list
    void* pAVL;
public:
    L1List() : _pHead(NULL), _size(0), pAVL(NULL) {}
    ~L1List(){};

    void    clean();
    bool    isEmpty() {
        return _pHead == NULL;
    }
    size_t  getSize() {
        return _size;
    }
    L1Item<T>* getHead(){return _pHead;} // get pHead
   	void setHead(L1Item<T>* p){this->_pHead = p;}
   	void setVoid(void* p){this->pAVL = p;}
   	void* getVoid(){return pAVL;}

    T&      at(int i);
    T&      operator[](int i);

    bool    find(T& a, int& idx);
    T*      find(T& a);
    T*      findLast(T& a);
    int     insert(int i, T& a);
    int     remove(int i);
    int     removeAll(T& a);

    int     push_back(T& a);
    int     insertHead(T& a);

    int     removeHead();
    int     removeLast();

    void    reverse();

    void    traverse(void (*op)(T&)) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data);
            p = p->pNext;
        }
    }
    void    traverse(void (*op)(T&, void*), void* pParam) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data, pParam);
            p = p->pNext;
        }
    }
};

/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
    if (_pHead == NULL) {
        _pHead = new L1Item<T>(a);
    }
    else {
        L1Item<T>   *p = _pHead;
        while (p->pNext) p = p->pNext;
        p->pNext = new L1Item<T>(a);
    }

    _size++;
    return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
    L1Item<T>   *p = new L1Item<T>(a);
    p->pNext = _pHead;
    _pHead = p;
    _size++;
    return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
    if(_pHead) {
        L1Item<T>* p = _pHead;
        _pHead = p->pNext;
        delete p;
        _size--;
        return 0;
    }
    return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
    if(_pHead) {
        if(_pHead->pNext) {
            L1Item<T>* prev = _pHead;
            L1Item<T>* pcur = prev->pNext;
            while(pcur->pNext) {
                prev = pcur;
                pcur = pcur->pNext;
            }
            delete pcur;
            prev->pNext = NULL;
        }
        else {
            delete _pHead;
            _pHead = NULL;
        }
        _size--;
        return 0;
    }
    return -1;
}

template<class T>
T& L1List<T>::operator [](int idx)
{
	//if(_pHead == NULL)
	L1Item<T>* p = _pHead;
	while(idx>0 &&p)
	{
		p = p->pNext;
		idx--;
	}
	if(p) return p->data;
}

/************************************************************************
 * This section is for AVL tree
 ************************************************************************/
template <class T, class K>
struct AVLNode {
    T           _data;
    AVLNode<T,K>   *_pLeft, *_pRight;
    K			_key;
//#ifdef AVL_USE_HEIGHT
    int         _height;
    AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), _height(1) {}
//#else
    int         _bFactor;
    AVLNode(T &a, K &key) : _data(a), _pLeft(NULL), _pRight(NULL), _bFactor(0), _key(key) {}
//#endif
};


template <class T, class K>
class AVLTree {
    AVLNode<T,K> *_pRoot;
public:
    AVLTree() : _pRoot(NULL) {}
    ~AVLTree() { destroy(_pRoot); }
    AVLNode<T,K>* getRoot(){return _pRoot;}

    bool find(T& key, T* &ret) { return find(_pRoot, key, ret); }
    bool insert(T& data, K&key) { return insert(_pRoot, data, key); }
    bool remove(T& data, K&key) { return remove(_pRoot, data, key); }
    void traverseNLR(void (*op)(T&)) { traverseNLR(_pRoot, op); }
    void traverseLNR(void (*op)(T&)) { traverseLNR(_pRoot, op); }
    void traverseLRN(void (*op)(T&)) { traverseLRN(_pRoot, op); }

protected:
    void destroy( AVLNode<T,K>* &pR);
    bool find(AVLNode<T,K> *pR, K& key, T* &ret);
    bool insert(AVLNode<T,K>* &pR, T& a, K& key);
    bool remove(AVLNode<T,K>* &pR, T& a,  K& key);
    void traverseNLR(AVLNode<T,K> *pR, void (*op)(T&));
    void traverseLNR(AVLNode<T,K> *pR, void (*op)(T&));
    void traverseLRN(AVLNode<T,K> *pR, void (*op)(T&));

    void rotLeft(AVLNode<T,K>* &pR);
    void rotRight(AVLNode<T,K>* &pR);
    void rotLR(AVLNode<T,K>* &pR);
    void rotRL(AVLNode<T,K>* &pR);

    bool balanceLeft(AVLNode<T,K>* &pR);
    bool balanceRight(AVLNode<T,K>* &pR);
};

//  implementation default avl

template<class T, class K>
void AVLTree<T,K>::rotLeft(AVLNode<T,K>*& pR){
	if(pR==NULL) return;
	AVLNode<T,K>* p = pR->_pRight;
	pR->_pRight = p->_pLeft;
	p->_pLeft = pR;
	pR=p;
}

template<class T, class K>
void AVLTree<T,K>::rotRight(AVLNode<T,K>*& pR){
	if(pR==NULL) return;
	AVLNode<T,K>* p = pR->_pLeft;
	pR->_pLeft = p->_pRight;
	p->_pRight = pR;
	pR = p;
}

template<class T, class K>
void AVLTree<T,K>::rotLR(AVLNode<T,K>*& pR){
	rotLeft(pR->_pLeft);
	rotRight(pR);
}

template<class T, class K>
void AVLTree<T,K>::rotRL(AVLNode<T,K>*& pR){
	rotRight(pR->_pRight);
	rotLeft(pR);
}

template<class T, class K>
bool AVLTree<T,K>::balanceLeft(AVLNode<T,K>*& pR){
	if(pR->_bFactor == 0){pR->_bFactor = -1; return true;}
	if(pR->_bFactor== 1){ pR->_bFactor = 0; return false;}
	if(pR->_pLeft->_bFactor == -1){// L-L
		rotRight(pR);
		pR->_bFactor  = 0;
		pR->_pRight->_bFactor = 0;
		return false;
	}
	if(pR->_pLeft->_bFactor == 0){ // use for remove function
		rotRight(pR);
		pR->_bFactor = 1;
		pR->_pRight->_bFactor = -1;
		return true;
	}
	rotLR(pR); // L-R
	if(pR->_bFactor == -1){
		pR->_pLeft->_bFactor = 0;
		pR->_bFactor = 0;
		pR->_pRight->_bFactor = 1;
		return false;
	}
	if(pR->_bFactor == 1){
		pR->_pLeft->_bFactor = -1;
		pR->_bFactor  = 0;
		pR->_pRight->_bFactor = 0;
		return false;
	}
	pR->_pLeft->_bFactor = 0;
	pR->_bFactor = 0;
	pR->_pRight->_bFactor = 0;
	return false;
}

template<class T, class K>
bool AVLTree<T,K>::balanceRight(AVLNode<T,K>*& pR){
	if(pR->_bFactor == 0){pR->_bFactor = 1; return true;}
	if(pR->_bFactor == -1){ pR->_bFactor = 0; return false;}
	if(pR->_pRight->_bFactor == 1){
		rotLeft(pR);
		pR->_bFactor = 0;
		pR->_pLeft->_bFactor = 0;
		return false;
	}
	if(pR->_pRight->_bFactor == 0){ // use for remove function
		rotLeft(pR);
		pR->_bFactor = -1;
		pR->_pLeft->_bFactor = 1;
		return true;
	}
	rotRL(pR); // R-L
	if(pR->_bFactor == 1){
		pR->_pRight->_bFactor = 0;
		pR->_bFactor = 0;
		pR->_pLeft->_bFactor = -1;
		return false;
	}
	if(pR->_bFactor == -1){
		pR->_pRight->_bFactor = 1;
		pR->_bFactor = 0;
		pR->_pLeft->_bFactor  = 0;
		return false;
	}
	pR->_pRight->_bFactor  = 0;
	pR->_bFactor = 0;
	pR->_pLeft->_bFactor  = 0;
	return false;
}

template<class T, class K>
bool AVLTree<T,K>::insert(AVLNode<T,K>*& pR,T& a,K& key){
	if(pR == NULL){
		pR = new AVLNode<T,K>(a,key);
		return true;
	}
	if(key < pR->_key){
		if(insert(pR->_pLeft,a,key) == false) return false;
		return balanceLeft(pR);
	}
	else{
		if(insert(pR->_pRight,a,key) == false) return false;
		return balanceRight(pR);
	}
}

template<class T, class K>
bool AVLTree<T,K>::find(AVLNode<T,K> *pR, K& key, T* &ret){
	if(pR == NULL) return false;
	else if(key < pR->_key) return find(pR->_pLeft,key,ret);
	else if(key > pR->_key) return find(pR->_pRight,key,ret);
	else{
		ret = pR;
		return true;
	}
}

template<class T, class K>
void AVLTree<T,K>::destroy(AVLNode<T,K>*& pR){
	if(pR == NULL) return;
	delete pR;
	pR = NULL;
}

template<class T, class K>
void AVLTree<T,K>::traverseNLR(AVLNode<T,K> *pR, void (*op)(T&)){
	if(pR == NULL) return;
	op(pR->_data);
	traverseNLR(pR->_pLeft,op);
	traverseNLR(pR->_pRight,op);
}

template<class T, class K>
void AVLTree<T,K>::traverseLNR(AVLNode<T,K> *pR, void (*op)(T&)){
	if(pR == NULL) return;
	traverseLNR(pR->_pLeft,op);
	op(pR->_data);
	traverseLNR(pR->_pRight,op);
}

template<class T, class K>
void AVLTree<T,K>::traverseLRN(AVLNode<T,K> *pR, void (*op)(T&)){
	if(pR == NULL) return;
	traverseLRN(pR->_pLeft,op);
	traverseLRN(pR->_pRight,op);
	op(pR->_data);
}

template<class T, class K>
bool AVLTree<T,K>::remove(AVLNode<T,K>* &pR, T& a,  K& key){
	if(pR==NULL) return false;
	if(key < pR->_key){
		if(remove(pR->pLeft,a,key) ){
		if(pR->_bFactor == -1){pR->_bFactor = 0; return true;}
		if(pR->_bFactor == 0){pR->_bFactor = 1; return false;}
		return !balanceRight(pR);
		}
	}
	if(key > pR->_key){
		if(remove(pR->pRight,a,key)){
		if(pR->_bFactor == 1){ pR->_bFactor = 0; return true;}//1
		if(pR->_bFactor == 0){ pR->_bFactor= -1; return false;}//2
		return !balanceLeft(pR);
		}
	}
	if(pR->_pLeft == NULL && pR->_pRight == NULL){
			delete pR; pR = NULL; return true;
	}
	if(pR->_pLeft  == NULL){
		AVLNode<T,K>* p = pR;
		pR=pR->_pRight;
		delete p; return true;
	}
	if(pR->_pRight== NULL){
		AVLNode<T,K>* p = pR;
		pR=pR->_pLeft;
		delete p; return true;
	}
	AVLNode<T,K>* p =pR->_pRight;
	while(p->_pLeft) p = p->_pLeft;
	pR->_data = p->_data;
	if(remove(pR->_pRight,a,key)){
		if(pR->_bFactor == 1){ pR->_bFactor = 0; return true;}//1
		if(pR->_bFactor == 0){ pR->_bFactor = -1; return false;}//2
		return !balanceLeft(pR);
	}

}
/*
template <class T>
struct MyAVLNode{
	char			_ID[ID_MAX_LENGTH];
	AVLTree<VM_Record,T>  timet;//
	AVLTree<VM_Record,T>   longt;
	AVLTree<VM_Record,T>  latt;
	MyAVLNode   *_pLeft, *_pRight;
	int         _bFactor;
	MyAVLNode(VM_Record& data) :  _pLeft(NULL), _pRight(NULL), _bFactor(0) {
		strcpy(_ID,data.id);
		timet.insert(data,data.timestamp);
		longt.insert(data, data.latitude);
		latt.insert(data, data.longitude);
	}
};

template <class T>
class MyAVLTree{
	 MyAVLNode<T> *_pRoot;
public:
	 bool insert(VM_Record& data){ return _insert(_pRoot, data); }

protected:
	 bool _insert(MyAVLNode<T>* &pR, VM_Record& data);

	 void _rotLeft(MyAVLNode<T>* &pR);
	 void _rotRight(MyAVLNode<T>* &pR);
	 void _rotLR(MyAVLNode<T>* &pR);
	 void _rotRL(MyAVLNode<T>* &pR);

	 bool _balanceLeft(MyAVLNode<T>* &pR);
	 bool _balanceRight(MyAVLNode<T>* &pR);
};

template<class T>
bool MyAVLTree<T>::_insert(MyAVLNode<T>* &pR, VM_Record& data){
	if(pR == NULL){
		pR = new MyAVLNode<T>(data);
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
		return balanceLeft(pR);
	}
	else{
		if(_insert(pR->_pRight,data) == false) return false;
		return balanceRight(pR);
	}
}
*/
#endif //A02_DSALIB_H
