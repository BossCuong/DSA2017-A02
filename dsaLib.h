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

using namespace std;

class DSAException
{
    int _error;
    string _text;

  public:
    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char *text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string &getErrorText() { return _text; }
};

template <class T>
struct L1Item
{
    T data;
    L1Item<T> *pNext;
    L1Item() : pNext(NULL) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
};

template <class T>
class L1List
{
    L1Item<T> *_pHead; // The head pointer of linked list
    L1Item<T> *_pTail; // The tail pointer of linked list
    size_t _size;      // number of elements in this list
  public:
    L1List() : _pHead(NULL), _pTail(NULL), _size(0) {}
    ~L1List();

    void clean();
    bool isEmpty()
    {
        return _pHead == NULL;
    }
    size_t getSize()
    {
        return _size;
    }

    T &at(int i);
    T &operator[](int i);

    bool find(T &a, int &idx);
    bool find(T &a, T *&ret);
    T *find(T &a);
    T *findLast(T &a);
    int insert(int i, T &a);
    int remove(int i);
    bool remove(T &a);
    int removeAll(T &a);

    int push_back(T &a);
    int insertHead(T &a);

    int removeHead();
    int removeLast();

    void reverse();

    void traverse(void (*op)(T &))
    {
        L1Item<T> *p = _pHead;
        while (p)
        {
            op(p->data);
            p = p->pNext;
        }
    }
    void traverse(void (*op)(T &, void *), void *pParam)
    {
        L1Item<T> *p = _pHead;
        while (p)
        {
            op(p->data, pParam);
            p = p->pNext;
        }
    }
};
/// List Destructor
template <class T>
L1List<T>::~L1List()
{
    if (_pHead)
    {
        while (_pHead)
        {
            L1Item<T> *p = _pHead;
            _pHead = _pHead->pNext;
            delete p;
        }
        _pHead = NULL;
    }
    if (_pTail)
    {
        delete _pTail;
        _pTail = NULL;
    }
}
/// List Indexer,Acess an item in list by index
/// Return data of item if success
template <class T>
T &L1List<T>::at(int i)
{
    if (_pHead == NULL || i < 0 || i >= _size)
        throw new DSAException(-1, "List empty or wrong index");
    L1Item<T> *_pRun = _pHead;
    while (_pRun && i > 0)
    {
        _pRun = _pRun->pNext;
        i--;
    }
    return _pRun->data;
}
template <class T>
T &L1List<T>::operator[](int i)
{
    if (_pHead == NULL || i < 0 || i >= _size)
        throw new DSAException(-1, "List empty or wrong index");
    L1Item<T> *_pRun = _pHead;
    while (_pRun && i > 0)
    {
        _pRun = _pRun->pNext;
        i--;
    }
    return _pRun->data;
}
///Find data in list
///Return true and return idx by reference if success
template <class T>
bool L1List<T>::find(T &a, int &idx)
{
    if (_pHead == NULL)
        return false;

    idx = 0;
    L1Item<T> *_pRun = _pHead;
    while (_pRun)
    {
        if (_pRun->data == a)
            break;
        _pRun = _pRun->pNext;
        ++idx;
    }
    if (_pRun == NULL)
        return false;
    else
        return true;
}
///Find data in list
///Return true and return pointer to that element by reference if success
template <class T>
bool L1List<T>::find(T &a, T *&ret)
{
    if (_pHead == NULL)
        return false;

    L1Item<T> *_pRun = _pHead;
    while (_pRun)
    {
        if (_pRun->data == a)
            break;
        _pRun = _pRun->pNext;
    }
    if (_pRun == NULL)
        return false;
    else
    {
        ret = &_pRun->data;
        return true;
    }
}
/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a)
{
    if (_pHead == NULL)
    {
        _pHead = new L1Item<T>(a);
    }
    else
    {
        L1Item<T> *p = _pHead;
        while (p->pNext)
            p = p->pNext;
        p->pNext = new L1Item<T>(a);
    }

    _size++;
    return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a)
{
    L1Item<T> *p = new L1Item<T>(a);
    p->pNext = _pHead;
    _pHead = p;
    _size++;
    return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead()
{
    if (_pHead)
    {
        L1Item<T> *p = _pHead;
        _pHead = p->pNext;
        delete p;
        _size--;
        return 0;
    }
    return -1;
}
template <class T>
bool L1List<T>::remove(T& data)
{
    if(_pHead == NULL) return false;
    if(_pHead->data == data)
    {
        L1Item<T>* p = _pHead;
        _pHead = _pHead->pNext;
        delete p;
        _size--;
        return true;
    }
    L1Item<T> *pPre,*pCur;
    pCur = _pHead;
    while(pCur && pCur->data != data)
    {
        pPre = pCur;
        pCur = pCur->pNext;
    }
    if(pCur == NULL) return false;

    pPre->pNext = pCur->pNext;
    delete pCur;
    _size--;
    return true;
}
/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast()
{
    if (_pHead)
    {
        if (_pHead->pNext)
        {
            L1Item<T> *prev = _pHead;
            L1Item<T> *pcur = prev->pNext;
            while (pcur->pNext)
            {
                prev = pcur;
                pcur = pcur->pNext;
            }
            delete pcur;
            prev->pNext = NULL;
        }
        else
        {
            delete _pHead;
            _pHead = NULL;
        }
        _size--;
        return 0;
    }
    return -1;
}
/// Reverse List
template <class T>
void L1List<T>::reverse()
{
    if (_pHead == NULL)
        throw new DSAException(-1, "Empty List");
    if (_size == 1)
        return;
    L1Item<T> *pPre = NULL;
    L1Item<T> *pCur = _pHead;
    L1Item<T> *p;
    while (pCur)
    {
        p = pCur->pNext;
        pCur->pNext = pPre;
        pPre = pCur;
        pCur = p;
    }
    _pHead = pPre;
}

/************************************************************************
 * This section is for AVL tree
 ************************************************************************/
enum BFactor
{
    EH = 0,
    LH = -1,
    RH = 1
};
template <class T>
struct AVLNode
{
    T _data;
    AVLNode<T> *_pLeft, *_pRight;
    // #ifdef AVL_USE_HEIGHT
    //     int         _height;
    //     AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), _height(1) {}
    // #else
    BFactor b;
    AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), b(EH) {}
    // #endif
};

template <class T>
class AVLTree
{
    AVLNode<T> *_pRoot;

  public:
    AVLTree() : _pRoot(NULL) {}
    ~AVLTree() { destroy(_pRoot); }
    bool isEmpty()
    {
        return _pRoot == NULL;
    }

    bool find(T &key, T *&ret) { return find(_pRoot, key, ret); }
    bool insert(T &key) { return insert(_pRoot, key); }
    bool remove(T &key) { return remove(_pRoot, key); }
    void traverseNLR(void (*op)(T &)) { traverseNLR(_pRoot, op); }
    void traverseNLR(void (*op)(T &, void *, bool &), void *pParam, bool &terminate) { traverseNLR(_pRoot, op, pParam, terminate); }
    void traverseLNR(void (*op)(T &)) { traverseLNR(_pRoot, op); }
    void traverseLNR(void (*op)(T &, void *, bool &), void *pParam, bool &terminate) { traverseLNR(_pRoot, op, pParam, terminate); }
    void traverseLNR(void (*op)(T &, void *), void *pParam) { traverseLNR(_pRoot, op, pParam); }
    void traverseRNL(void (*op)(T &, void *), void *pParam) { traverseRNL(_pRoot, op, pParam); }
    void traverseLRN(void (*op)(T &)) { traverseLRN(_pRoot, op); }

  protected:
    void destroy(AVLNode<T> *&pR);
    bool find(AVLNode<T> *pR, T &key, T *&ret);
    bool insert(AVLNode<T> *&pR, T &a);
    bool remove(AVLNode<T> *&pR, T &a);

    void traverseNLR(AVLNode<T> *pR, void (*op)(T &));
    void traverseNLR(AVLNode<T> *pR, void (*op)(T &, void *, bool &), void *pParam, bool &terminate);
    void traverseLNR(AVLNode<T> *pR, void (*op)(T &));
    void traverseLNR(AVLNode<T> *pR, void (*op)(T &, void *), void *pParam);
    void traverseLNR(AVLNode<T> *pR, void (*op)(T &, void *, bool &), void *pParam, bool &terminate);
    void traverseRNL(AVLNode<T> *pR, void (*op)(T &, void *), void *pParam);
    void traverseLRN(AVLNode<T> *pR, void (*op)(T &));

    void rotLeft(AVLNode<T> *&pR);
    void rotRight(AVLNode<T> *&pR);
    void rotLR(AVLNode<T> *&pR);
    void rotRL(AVLNode<T> *&pR);

    bool balanceLeft(AVLNode<T> *&pR);
    bool balanceRight(AVLNode<T> *&pR);
};
template <class T>
void AVLTree<T>::destroy(AVLNode<T> *&pR)
{
    if (pR == NULL)
        return;
    destroy(pR->_pLeft);
    destroy(pR->_pRight);

    delete pR;
    pR = NULL;
}
template <class T>
bool AVLTree<T>::insert(AVLNode<T> *&pR, T &a)
{
    if (pR == NULL)
    {
        pR = new AVLNode<T>(a);
        return true;
    }
    if (a < pR->_data)
    {
        if (insert(pR->_pLeft, a) == false)
            return false;
        return balanceLeft(pR);
    }
    else
    {
        if (insert(pR->_pRight, a) == false)
            return false;
        return balanceRight(pR);
    }
}
template <class T>
bool AVLTree<T>::remove(AVLNode<T> *&pR, T &a)
{
    if (pR == NULL)
        return false;
    bool delLeft, ret;

    if (a < pR->_data)
    {
        delLeft = true;
        ret = remove(pR->_pLeft, a);
    }
    else if (a > pR->_data)
    {
        delLeft = false;
        ret = remove(pR->_pRight, a);
    }
    else
    {
        if (pR->_pLeft == NULL)
        {
            AVLNode<T> *p = pR;
            pR = pR->_pRight;
            delete p;
            p = NULL;
            return true;
        }
        else if (pR->_pRight == NULL)
        {
            AVLNode<T> *p = pR;
            pR = pR->_pLeft;
            delete p;
            p = NULL;
            return true;
        }

        AVLNode<T> *p = pR->_pRight;
        while (p->_pLeft)
            p = p->_pLeft;
        pR->_data = p->_data;

        delLeft = false;
        ret = remove(pR->_pRight, p->_data);
    }
    if (ret == false)
        return false;
    if (delLeft)
        return !balanceRight(pR);
    return !balanceLeft(pR);
}
template <class T>
void AVLTree<T>::rotRight(AVLNode<T> *&pR)
{
    if (pR == NULL)
        return;
    if (pR->_pLeft == NULL)
        return;
    AVLNode<T> *p = pR->_pLeft;
    pR->_pLeft = p->_pRight;
    p->_pRight = pR;
    pR = p;
}
template <class T>
void AVLTree<T>::rotLeft(AVLNode<T> *&pR)
{
    if (pR == NULL)
        return;
    if (pR->_pRight == NULL)
        return;
    AVLNode<T> *p = pR->_pRight;
    pR->_pRight = p->_pLeft;
    p->_pLeft = pR;
    pR = p;
}
template <class T>
void AVLTree<T>::rotLR(AVLNode<T> *&pR)
{
    rotLeft(pR->_pLeft);
    rotRight(pR);
}
template <class T>
void AVLTree<T>::rotRL(AVLNode<T> *&pR)
{
    rotRight(pR->_pRight);
    rotLeft(pR);
}
template <class T>
bool AVLTree<T>::balanceLeft(AVLNode<T> *&pR)
{
    if (pR->b == EH)
    {
        pR->b = LH;
        return true;
    }
    if (pR->b == RH)
    {
        pR->b = EH;
        return false;
    }

    //L-L case
    if (pR->_pLeft->b == LH)
    {
        rotRight(pR);
        pR->b = pR->_pRight->b = EH;
        return false;
    }
    else if (pR->_pLeft->b == EH) //L-E case
    {
        rotRight(pR);
        pR->b = RH;
        pR->_pRight->b = LH;
        return false;
    }
    //L-R case
    rotLR(pR);
    if (pR->b == LH)
    {
        pR->b = pR->_pLeft->b = EH;
        pR->_pRight->b = RH;
        return false;
    }
    else if (pR->b == RH)
    {
        pR->b = pR->_pRight->b = EH;
        pR->_pLeft->b = LH;
        return false;
    }
    else
    {
        pR->_pLeft->b = pR->_pRight->b = EH;
        return false;
    }
}
template <class T>
bool AVLTree<T>::balanceRight(AVLNode<T> *&pR)
{
    if (pR->b == EH)
    {
        pR->b = RH;
        return true;
    }
    if (pR->b == LH)
    {
        pR->b = EH;
        return false;
    }

    //R-R R-E case
    if (pR->_pRight->b == RH)
    {
        rotLeft(pR);
        pR->b = pR->_pLeft->b = EH;
        return false;
    }
    else if (pR->_pRight->b == EH) //R-E case
    {
        rotLeft(pR);
        pR->b = LH;
        pR->_pLeft->b = RH;
        return false;
    }
    //R-L case
    rotRL(pR);
    if (pR->b == RH)
    {
        pR->b = pR->_pRight->b = EH;
        pR->_pLeft->b = LH;
        return false;
    }
    else if (pR->b == LH)
    {
        pR->b = pR->_pLeft->b = EH;
        pR->_pRight->b = RH;
        return false;
    }
    else
    {
        pR->_pLeft->b = pR->_pRight->b = EH;
        return false;
    }
}
template <class T>
void AVLTree<T>::traverseNLR(AVLNode<T> *pR, void (*op)(T &))
{
    if (pR == NULL)
        return;
    op(pR->_data);
    traverseNLR(pR->_pLeft, op);
    traverseNLR(pR->_pRight, op);
}
template <class T>
void AVLTree<T>::traverseLNR(AVLNode<T> *pR, void (*op)(T &))
{
    if (pR == NULL)
        return;
    traverseLNR(pR->_pLeft, op);
    op(pR->_data);
    traverseLNR(pR->_pRight, op);
}
template <class T>
void AVLTree<T>::traverseLRN(AVLNode<T> *pR, void (*op)(T &))
{
    if (pR == NULL)
        return;
    traverseLRN(pR->_pLeft, op);
    traverseLRN(pR->_pRight, op);
    op(pR->_data);
}
template <class T>
void AVLTree<T>::traverseNLR(AVLNode<T> *pR, void (*op)(T &, void *, bool &), void *pParam, bool &terminate)
{
    if (pR == NULL || terminate == true)
        return;
    op(pR->_data, pParam, terminate);
    traverseNLR(pR->_pLeft, op, pParam, terminate);
    traverseNLR(pR->_pRight, op, pParam, terminate);
}
template <class T>
void AVLTree<T>::traverseLNR(AVLNode<T> *pR, void (*op)(T &, void *, bool &), void *pParam, bool &terminate)
{
    if (pR == NULL || terminate == true)
        return;
    traverseLNR(pR->_pLeft, op, pParam,terminate);
    op(pR->_data, pParam,terminate);
    traverseLNR(pR->_pRight, op, pParam,terminate);
}
template <class T>
void AVLTree<T>::traverseLNR(AVLNode<T> *pR, void (*op)(T &, void *), void *pParam)
{
    if (pR == NULL)
        return;
    traverseLNR(pR->_pLeft, op, pParam);
    op(pR->_data, pParam);
    traverseLNR(pR->_pRight, op, pParam);
}
template <class T>
void AVLTree<T>::traverseRNL(AVLNode<T> *pR, void (*op)(T &, void *), void *pParam)
{
    if (pR == NULL)
        return;
    traverseRNL(pR->_pRight, op, pParam);
    op(pR->_data, pParam);
    traverseRNL(pR->_pLeft, op, pParam);
}
template <class T>
bool AVLTree<T>::find(AVLNode<T> *pR, T &key, T *&ret)
{
    if (pR == NULL)
        return false;

    if (key < pR->_data)
        return find(pR->_pLeft, key, ret);
    if (key > pR->_data)
        return find(pR->_pRight, key, ret);
    else
    {
        ret = &(pR->_data);
        return true;
    }
}
#endif //A02_DSALIB_H
