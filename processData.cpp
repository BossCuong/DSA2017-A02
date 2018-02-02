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
    VM_database()
    {
        id[0] = 0;
    }
    VM_database(const char *busID)
    {
        strcpy(id, busID);
    }
};
inline bool operator==(VM_database &lhs, char *rhs)
{
    return strcmp(lhs.id, rhs) == 0;
}
inline bool operator==(char *lhs, VM_database &rhs)
{
    return strcmp(rhs.id, lhs) == 0;
}
inline bool operator==(VM_database &lhs, VM_database &rhs)
{
    return strcmp(lhs.id, rhs.id) == 0;
}

inline bool operator!=(VM_database &lhs, char *rhs)
{
    return !(lhs == rhs);
}
inline bool operator!=(char *lhs, VM_database &rhs)
{
    return !(rhs == lhs);
}
inline bool operator!=(VM_database &lhs, VM_database &rhs)
{
    return !(lhs == rhs);
}

inline bool operator>(VM_database &lhs, char *rhs)
{
    return strcmp(lhs.id, rhs) > 0;
}
inline bool operator>(char *lhs, VM_database &rhs)
{
    return strcmp(rhs.id, lhs) > 0;
}
inline bool operator>(VM_database &lhs, VM_database &rhs)
{
    return strcmp(lhs.id, rhs.id) > 0;
}

inline bool operator<(VM_database &lhs, char *rhs)
{
    return strcmp(lhs.id, rhs) < 0;
}
inline bool operator<(char *lhs, VM_database &rhs)
{
    return strcmp(rhs.id, lhs) < 0;
}
inline bool operator<(VM_database &lhs, VM_database &rhs)
{
    return strcmp(lhs.id, rhs.id) < 0;
}

inline bool operator>=(VM_database &lhs, char *rhs)
{
    return strcmp(lhs.id, rhs) >= 0;
}
inline bool operator>=(char *lhs, VM_database &rhs)
{
    return strcmp(rhs.id, lhs) >= 0;
}
inline bool operator>=(VM_database &lhs, VM_database &rhs)
{
    return strcmp(lhs.id, rhs.id) >= 0;
}

inline bool operator<=(VM_database &lhs, char *rhs)
{
    return strcmp(lhs.id, rhs) <= 0;
}
inline bool operator<=(char *lhs, VM_database &rhs)
{
    return strcmp(rhs.id, lhs) <= 0;
}
inline bool operator<=(VM_database &lhs, VM_database &rhs)
{
    return strcmp(lhs.id, rhs.id) <= 0;
}
bool initVMGlobalData(void **pGData)
{
    // TODO: allocate and initialize global data
    *pGData = new L1List<VM_database>();
    // return false if failed
    return true;
}
void releaseVMGlobalData(void *pGData)
{
    // TODO: do your cleanup, left this empty if you don't have any dynamically allocated data
    delete (L1List<VM_database> *)pGData;
}
struct request_data
{
    double longitude;
    double latitude;
    double radius;
    int h1, h2;
    bool isOutCircle;
    int cnt;
    request_data() : longitude(0), latitude(0), cnt(0), isOutCircle(true) {}
};
bool process_request_1(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    void (*re1_parseTime)(time_t &, string &, time_t &) = [](time_t &dest, string &src, time_t &scale) {
        //parse time to tm
        struct tm *tm = gmtime(&scale);
        tm->tm_hour = stoi(src.substr(0, 2));
        tm->tm_min = stoi(src.substr(2, 2));
        tm->tm_sec = stoi(src.substr(4, 2));

        dest = timegm(tm);
    };

    L1List<VM_database> *database = (L1List<VM_database> *)pGData;
    VM_database *x1, *x2;

    stringstream stream(request.code);
    string buf, time_src;

    if (!getline(stream, buf, '_'))
        return false;

    //Key contain ID to find ID in database
    if (!getline(stream, buf, '_'))
        return false;
    VM_database list_key1((char *)buf.data());
    if (!getline(stream, buf, '_'))
        return false;
    VM_database list_key2((char *)buf.data());
    if (!getline(stream, buf, '_'))
        return false;
    time_src = buf;

    if (!stream.eof() || time_src.length() != 6)
        return false;
    //Find ID of VM,return reference of ID database to x1,x2

    if (database->find(list_key1, x1) && database->find(list_key2, x2))
    {
        //Get time to key to find in AVL database
        VM_Record avl_key;
        re1_parseTime(avl_key.timestamp, time_src, recordList[0].timestamp);

        VM_Record *record1, *record2;

        //Use time key to find record have time in request,return reference of record to data1,data2
        if (x1->data.find(avl_key, record1) && x2->data.find(avl_key, record2))
        {
            //Set relative of VM
            string relative_long, relative_lat;
            if ((record1->longitude - record2->longitude) >= 0)
                relative_long = "E";
            else
                relative_long = "W";
            if ((record1->latitude - record2->latitude) >= 0)
                relative_lat = "N";
            else
                relative_lat = "S";
            //Caculating distance of two VM
            double distance = distanceEarth(record1->latitude, record1->longitude, record2->latitude, record2->longitude);

            //Print result
            cout << request.code[0] << ": " << relative_long << " " << relative_lat << " " << distance << endl;
            return true;
        }
    }

    //If not find anything
    cout << request.code[0] << ": " << -1 << endl;
    return true;
}
bool process_request_2(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    void (*process_re2_E)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Fucntion check if record coordinate not in East
        void (*isNotEast)(VM_Record &, void *, bool &) = [](VM_Record &record, void *p, bool &terminate) {
            //If find it,set terminate to true to terminate traverse
            if (!((record.longitude - ((request_data *)p)->longitude) >= 0))
                terminate = true;
        };

        //Counting valid ID
        bool terminate = false;
        record.data.traverseNLR(isNotEast, p, terminate);
        if (!terminate)
            ((request_data *)p)->cnt++;
    };

    void (*process_re2_W)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Fucntion check if record coordinate not in West
        void (*isNotWest)(VM_Record &, void *, bool &) = [](VM_Record &record, void *p, bool &terminate) {
            if (!((record.longitude - ((request_data *)p)->longitude) < 0))
                terminate = true;
        };

        //Counting valid ID
        bool terminate = false;
        record.data.traverseNLR(isNotWest, p, terminate);
        if (!terminate)
            ((request_data *)p)->cnt++;
    };
    //

    stringstream stream(request.code);
    string buf, relative;
    if (!getline(stream, buf, '_'))
        return false;

    if (!getline(stream, buf, '_'))
        return false;
    request.params[0] = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    relative = buf;

    if (!stream.eof() || relative.length() != 1)
        return false;

    L1List<VM_database> *database = (L1List<VM_database> *)pGData;
    void *p = new request_data();
    ((request_data *)p)->longitude = request.params[0];

    if (relative == "E")
        database->traverse(process_re2_E, p);
    else if (relative == "W")
        database->traverse(process_re2_W, p);
    else
        return false;

    //Print result
    cout << request.code[0] << ": " << ((request_data *)p)->cnt << endl;
    delete (request_data *)p;
    return true;
}
bool process_request_3(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    void (*process_re3_N)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Fucntion check if record coordinate not in North
        void (*re3_counting_N)(VM_Record &, void *, bool &) = [](VM_Record &record, void *p, bool &terminate) {
            if (!((record.latitude - ((request_data *)p)->latitude) >= 0))
                terminate = true;
        };

        //Check valid ID
        bool terminate = false;
        record.data.traverseNLR(re3_counting_N, p, terminate);
        if (!terminate)
            ((request_data *)p)->cnt++;
    };

    void (*process_re3_S)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Fucntion check if record coordinate not in South
        void (*re3_counting_S)(VM_Record &, void *, bool &) = [](VM_Record &record, void *p, bool &terminate) {
            if (!((record.latitude - ((request_data *)p)->latitude) < 0))
                terminate = true;
        };

        //Check valid ID
        bool terminate = false;
        record.data.traverseNLR(re3_counting_S, p, terminate);
        if (!terminate)
            ((request_data *)p)->cnt++;
    };

    stringstream stream(request.code);
    string buf, relative;
    if (!getline(stream, buf, '_'))
        return false;

    if (!getline(stream, buf, '_'))
        return false;
    request.params[0] = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    relative = buf;

    if (!stream.eof() || relative.length() != 1)
        return false;

    L1List<VM_database> *database = (L1List<VM_database> *)pGData;
    void *p = new request_data();
    ((request_data *)p)->latitude = request.params[0];

    if (relative == "N")
        database->traverse(process_re3_N, p);
    else if (relative == "S")
        database->traverse(process_re3_S, p);
    else
        return false;

    //Print result
    cout << request.code[0] << ": " << ((request_data *)p)->cnt << endl;
    delete (request_data *)p;
    return true;
}
bool process_request_4(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    void (*process_re4)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Function check if VM go in to circle
        void (*re4_counting)(VM_Record &, void *, bool &) = [](VM_Record &record, void *p, bool &terminate) {
            request_data *re_data = (request_data *)p;
            double distance = distanceEarth(re_data->latitude, re_data->longitude, record.latitude, record.longitude);

            if (distance <= re_data->radius && record.timestamp >= re_data->h1 && record.timestamp <= re_data->h2)
            {
                re_data->cnt++;
                terminate = true;
            }
        };
        //Check valid ID
        bool terminate = false;
        record.data.traverseNLR(re4_counting, p, terminate);
    };

    stringstream stream(&request.code[2]);
    string buf;
    int i = 0;
    while (getline(stream, buf, '_'))
    {
        request.params[i] = stod(buf);
        i++;
    }

    L1List<VM_database> *database = (L1List<VM_database> *)pGData;
    void *p = new request_data();
    ((request_data *)p)->longitude = request.params[0];
    ((request_data *)p)->latitude = request.params[1];
    ((request_data *)p)->radius = request.params[2];

    //parse time to tm
    struct tm *tm = gmtime(&recordList[0].timestamp);
    tm->tm_min = 0;
    tm->tm_sec = 0;

    tm->tm_hour = request.params[3];
    ((request_data *)p)->h1 = timegm(tm);
    tm->tm_hour = request.params[4];
    ((request_data *)p)->h2 = timegm(tm);

    database->traverse(process_re4, p);

    //Print result
    cout << request.code << ": " << ((request_data *)p)->cnt << endl;
    delete (request_data *)p;
    return true;
}
bool process_request_5(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    void (*re5_counting)(VM_Record &, void *, bool &) = [](VM_Record &record, void *p, bool &terminate) {
        request_data *re_data = (request_data *)p;
        double distance = distanceEarth(re_data->latitude, re_data->longitude, record.latitude, record.longitude);
        if (distance <= re_data->radius && re_data->isOutCircle == true)
        {
            re_data->cnt++;
            re_data->isOutCircle = false;
        }
        else if (distance > re_data->radius)
        {
            re_data->isOutCircle = true;
        }
    };

    stringstream stream(&request.code[2]);
    string buf, relative;

    //Init key to find ID in database
    getline(stream, buf, '_');
    VM_database key((char *)buf.data());

    int i = 0;
    while (getline(stream, buf, '_'))
    {
        request.params[i] = stod(buf);
        i++;
    }

    L1List<VM_database> *database = (L1List<VM_database> *)pGData;

    void *p = new request_data();
    ((request_data *)p)->longitude = request.params[0];
    ((request_data *)p)->latitude = request.params[1];
    ((request_data *)p)->radius = request.params[2];

    VM_database *x;
    //Find VM_database and return it pointer of data to x by reference
    database->find(key, x);
    //Traverse AVL in data have key ID
    bool terminate = false; //ignore this
    x->data.traverseNLR(re5_counting, p, terminate);

    //Print result
    cout << request.code << ": " << ((request_data *)p)->cnt << endl;
    delete (request_data *)p;
    return true;
}
bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    // TODO: Your code goes
    //Init database for processRequest
    if (((AVLTree<VM_database> *)pGData)->isEmpty())
    {
        //Function pointer to creat dabase
        void (*initDatabase)(VM_Record &, void *) = [](VM_Record &record, void *pGData) {
            VM_database key(record.id);
            L1List<VM_database> *p = (L1List<VM_database> *)pGData;
            VM_database *x;

            if (!p->find(key, x))
            {
                p->insertHead(key);
                (p->at(0)).data.insert(record);
            }
            else
                x->data.insert(record);
        };
        //Traverse LiList and creat database
        recordList.traverse(initDatabase, pGData);
    }

    switch (request.code[0])
    {
    case '1':
        return process_request_1(request, recordList, pGData);
        break;
    case '2':
        return process_request_2(request, recordList, pGData);
        break;
    case '3':
        return process_request_3(request, recordList, pGData);
        break;
    case '4':
        return process_request_4(request, recordList, pGData);
        break;
    case '5':
        return process_request_5(request, recordList, pGData);
        break;
    case '6':
        return process_request_5(request, recordList, pGData);
        break;
    default:
        return false; // return false for invlaid events
    }
}