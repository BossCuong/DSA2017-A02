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
void initDatabase(VM_Record& record ,void* pGData)
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

    //Key contain ID to find ID in database  
    VM_database list_key1((char*)VM_tag1.data());
    VM_database list_key2((char*)VM_tag2.data());

    //Find ID of VM,return reference of ID database to x1,x2
    if(database->find(list_key1,x1) && database->find(list_key2,x2))
    {   
        //Get time to key to find in AVL database
        VM_Record avl_key;
        re1_parseTime(avl_key.timestamp,recordList[0].timestamp,request);

        VM_Record *record1, *record2;

        //Use time key to find record have time in request,return reference of record to data1,data2
        if(x1->data.data.find(avl_key,record1) && x2->data.data.find(avl_key,record2))
        {
            //Set relative of VM
            string relative_long,relative_lat;
            if((record1->longitude - record2->longitude) >= 0) relative_long = "E"; else relative_long = "W";
            if((record1->latitude - record2->latitude) >= 0)   relative_lat = "N";  else relative_lat = "S";
            //Caculating distance of two VM
            double distance = distanceEarth(record1->latitude,record1->longitude,record2->latitude,record2->longitude);

            //Print result
            cout << request.code << ": " << relative_long << " " << relative_lat << " " << distance << endl;
            return true;
        }
    }
    
    cout << request.code << ": " << -1 << endl;
    return true;
}
////
////
struct re2_3_data
{
    double coordinate;
    int cnt;
    re2_3_data() : coordinate(0),cnt(0){}
};
void re2_E(VM_Record &record, void *p,bool &terminate)
{
    if ((record.longitude - ((re2_3_data *)p)->coordinate) >= 0)
    {
        ((re2_3_data *)p)->cnt++;
        terminate = true;
    }
}
void process_re2_E(VM_database &record, void *p)
{
    bool terminate = false;
    record.data.traverseNLR(&re2_E,p,terminate);
}
void re2_W(VM_Record &record, void *p,bool &terminate)
{
    if ((record.longitude - ((re2_3_data *)p)->coordinate) < 0)
    {
        ((re2_3_data *)p)->cnt++;
        terminate = true;
    }
}
void process_re2_W(VM_database &record, void *p)
{
    bool terminate = false;
    record.data.traverseNLR(&re2_W,p,terminate);
}
bool request_2(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    //
    string coordinate = request.code, relative;
    int pos = coordinate.find_first_of('_');
    coordinate = coordinate.substr(pos + 1);
    pos = coordinate.find_first_of('_');
    relative = coordinate.substr(pos + 1);
    coordinate = coordinate.substr(0, pos);

    L1List<VM_database>* database = (L1List<VM_database>*)pGData;
    void *p = new re2_3_data();
    ((re2_3_data *)p)->coordinate = stod(coordinate);

    if (relative == "E")
        database->traverse(&process_re2_E, p);
    else if (relative == "W")
        database->traverse(&process_re2_W, p);
    else
        return false;

    //Print result
    cout << request.code << ": " << ((re2_3_data *)p)->cnt << endl;
    delete (re2_3_data *)p;
    return true;
}
bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData) {
    // TODO: Your code goes
    if(((AVLTree<VM_database>*)pGData)->isEmpty()) 
        recordList.traverse(&initDatabase,pGData);

    if(request.code[0] == '1')
    {
        return request_1(request,recordList,pGData);
    }
    if(request.code[0] == '2')
    {
        return request_2(request,recordList,pGData);
    }

    return false; // return false for invlaid events
}


