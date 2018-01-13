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
#include "requestLib.h"
#include "dbLib.h"

using namespace std;

#define GPS_DISTANCE_ERROR 0.005
struct VM_database
{
    char id[ID_MAX_LENGTH];
    AVLTree<VM_Record> data;
     // default constructor
    VM_database() {
        id[0] = 0;
    }
    VM_database(const char* busID) {
        strcpy(id, busID);
    }
};
inline bool operator==(VM_database& lhs, char* rhs) {
    return strcmp(lhs.id, rhs) == 0;
}
inline bool operator==(char* lhs, VM_database& rhs)  {
    return strcmp(rhs.id, lhs) == 0;
}
inline bool operator==(VM_database& lhs, VM_database& rhs) {
    return strcmp(lhs.id, rhs.id) == 0;
}

inline bool operator!=(VM_database& lhs,char* rhs) {
    return !(lhs == rhs);
}
inline bool operator!=(char* lhs, VM_database& rhs)  {
    return !(rhs == lhs);
}
inline bool operator!=(VM_database& lhs, VM_database& rhs) {
    return !(lhs == rhs);
}

inline bool operator>(VM_database& lhs, char* rhs) {
    return strcmp(lhs.id, rhs) > 0;
}
inline bool operator>(char* lhs, VM_database& rhs)  {
    return strcmp(rhs.id, lhs) > 0;
}
inline bool operator>(VM_database& lhs, VM_database& rhs) {
    return strcmp(lhs.id, rhs.id) > 0;
}

inline bool operator<(VM_database& lhs, char* rhs) {
    return strcmp(lhs.id, rhs) < 0;
}
inline bool operator<(char* lhs, VM_database& rhs)  {
    return strcmp(rhs.id, lhs) < 0;
}
inline bool operator<(VM_database& lhs, VM_database& rhs) {
    return strcmp(lhs.id, rhs.id) < 0;
}

inline bool operator>=(VM_database& lhs, char* rhs) {
    return strcmp(lhs.id, rhs) >= 0;
}
inline bool operator>=(char* lhs, VM_database& rhs)  {
    return strcmp(rhs.id, lhs) >= 0;
}
inline bool operator>=(VM_database& lhs, VM_database& rhs) {
    return strcmp(lhs.id, rhs.id) >= 0;
}

inline bool operator<=(VM_database& lhs, char* rhs) {
    return strcmp(lhs.id, rhs) <= 0;
}
inline bool operator<=(char* lhs, VM_database& rhs)  {
    return strcmp(rhs.id, lhs) <= 0;
}
inline bool operator<=(VM_database& lhs, VM_database& rhs) {
    return strcmp(lhs.id, rhs.id) <= 0;
}
bool initVMGlobalData(void** pGData) {
    // TODO: allocate and initialize global data
    *pGData = new AVLTree<VM_database>();
    // return false if failed
    return true;
}
void releaseVMGlobalData(void* pGData) {
    // TODO: do your cleanup, left this empty if you don't have any dynamically allocated data
    delete (AVLTree<VM_database>*)pGData;
}

void creatingData(VM_Record& record ,void* pGData)
{
    VM_database key(record.id);
    AVLTree<VM_database>* p = (AVLTree<VM_database>*)pGData;
    VM_database* x; //

    if(!p->find(key,x))
    {
        p->insert(key);
        p->find(key,x);
        x->data.insert(record);
    }
    else
        x->data.insert(record);
}
bool request_1(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    // L1List<VM_database>* p = (L1List<VM_database>*)pGData;
    // L1Item<VM_database> *x1,*x2;
    // string buf = request.code;
    // int pos1,pos2;
    // pos1 = buf.find_first_of('_');
    // pos2 = buf.find_last_of('_');
    // L1Item<VM_Record> vec1;
    // L1Item<VM_Record> vec2;
    // if(p->find(buf.substr(pos1+1,)))
    // else cout << "-1";
    // string buf1 = request.code;
    // string buf2 = buf1.substr(7);
    // buf1 = buf1.substr(2,4);
    // VM_database key1((char*)buf1.data());
    // VM_database key2((char*)buf2.data());
    // if(p->find(key1,x1) && p->find(key2,x2))
    // {

    // }
    // else cout << request.code << ": " << -1 << endl;
    // return true;
}
bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData) {
    // TODO: Your code goes
    if(((AVLTree<VM_database>*)pGData)->isEmpty())
    {   
        void (*foo)(VM_Record&,void*);
        foo = creatingData;
    
        recordList.traverse(foo,pGData);
    }
    if(request.code[0] == '1')
    {
        return request_1(request,recordList,pGData);
    }
    
    return false; // return false for invlaid events
}


