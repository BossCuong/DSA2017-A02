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
    *pGData = new L1List<VM_database>();
    // return false if failed
    return true;
}
void releaseVMGlobalData(void* pGData) {
    // TODO: do your cleanup, left this empty if you don't have any dynamically allocated data
    delete (L1List<VM_database>*)pGData;
}

// void creatingData(VM_Record& record ,void* pGData)
// {
//     VM_database key(record.id);
//     AVLTree<VM_database>* p = (AVLTree<VM_database>*)pGData;
//     VM_database* x;

//     if(!p->find(key,x))
//     {
//         p->insert(key);
//         p->find(key,x);
//         x->data.insert(record);
//     }
//     else
//         x->data.insert(record);
// }
void creatingData(VM_Record& record ,void* pGData)
{
    VM_database key(record.id);
    L1List<VM_database>* p = (L1List<VM_database>*)pGData;
    L1Item<VM_database>* x;

    if(!p->find(key,x))
    {
        p->insertHead(key);
        (p->at(0)).data.insert(record);
    }
    else
        x->data.data.insert(record);
}
void re1_parseTime(time_t &dest,time_t &src,VM_Request &request)
{
    //parse time to tm
    struct tm *tm = gmtime(&src);
    string buf = request.code;
    int pos = buf.find_last_of('_');
    buf = buf.substr(pos + 1);
    tm->tm_hour = stoi(buf.substr(0, 2));
    tm->tm_min = stoi(buf.substr(2, 2));
    tm->tm_sec = stoi(buf.substr(4, 2));

    dest = timegm(tm);
}
bool request_1(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    L1List<VM_database>* database = (L1List<VM_database>*)pGData;
    L1Item<VM_database> *x1,*x2;

    //Parse request code to ID
    string VM_tag1 = request.code;
    string VM_tag2 = VM_tag1.substr(7,4);
    VM_tag1 = VM_tag1.substr(2,4);

    VM_database key1((char*)VM_tag1.data());
    VM_database key2((char*)VM_tag2.data());

    //Find ID of VM
    if(database->find(key1,x1) && database->find(key2,x2))
    {   
        //Get time to key to find in AVL database
        VM_Record key;
        re1_parseTime(key.timestamp,recordList[0].timestamp,request);

        VM_Record *data1, *data2;

        //Use time key to find record of ID 
        if(x1->data.data.find(key,data1) && x2->data.data.find(key,data2))
        {
            string dir_long,dir_lat;
            if((data1->longitude - data2->longitude) >= 0) dir_long = "E";else dir_long = "W";
            if((data1->latitude - data2->latitude) >= 0) dir_lat = "N";else dir_lat = "S";
            double distance = distanceEarth(data1->latitude,data1->longitude,data2->latitude,data2->longitude);

            //Print result
            cout << request.code << ": " << dir_long << " " << dir_lat << " " << distance << endl;
            return true;
        }
    }
    cout << request.code << ": " << -1 << endl;
    return true;
}

bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData) {
    // TODO: Your code goes
    if(((AVLTree<VM_database>*)pGData)->isEmpty()) 
        recordList.traverse(&creatingData,pGData);
    if(request.code[0] == '1')
    {
        return request_1(request,recordList,pGData);
    }
    return false; // return false for invlaid events
}


