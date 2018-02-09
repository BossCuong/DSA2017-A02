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
    bool isValid;
    // default constructor
    VM_database()
    {
        isValid = true;
        id[0] = 0;
    }
    VM_database(const char *busID)
    {
        strcpy(id, busID);
        isValid = true;
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
    *pGData = new AVLTree<VM_database>();
    // return false if failed
    return true;
}
void releaseVMGlobalData(void *pGData)
{
    // TODO: do your cleanup, left this empty if you don't have any dynamically allocated data
    delete (AVLTree<VM_database> *)pGData;
}
struct re7_database
{
    char id[ID_MAX_LENGTH];
    double distance;
    re7_database()
    {
        id[0] = 0;
        distance = 0;
    }
};
inline bool operator<(re7_database &lhs, re7_database &rhs)
{
    return lhs.distance < rhs.distance;
}
inline bool operator>=(re7_database &lhs, re7_database &rhs)
{
    return lhs.distance >= rhs.distance;
}
struct request_data
{
    void* pData;
    double longitude;
    double latitude;
    int number_of_VM;
    time_t t1,t2;
    double radius;
    int h1, h2;
    bool isOutCircle;
    int cnt;
    int percent_cnt;
    double min_distance;
    L1List<string> lessThan2km;
    L1List<string> lessThan2km_greatThan500m;
    L1List<string> lessThan500m;
    L1List<string> lessThan300m;

    double max_distance;
    AVLTree<string> lessThan1km_andStuck;
    AVLTree<re7_database> lessThan2km_greatThan1km;
    AVLTree<string> lessThan2km_greatThan1km_75;
    request_data()
    {
        pData = NULL;
        longitude = 0;
        latitude = 0;
        number_of_VM = 0;
        t1 =0;
        t2 = 0;
        radius =0;
        h1 = 0;
        h2 = 0;
        isOutCircle = true;
        cnt = 0;
        percent_cnt = 0;
        min_distance = -1;
        max_distance = -1;
    }
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

    AVLTree<VM_database> *database = (AVLTree<VM_database> *)pGData;
    VM_database *x1, *x2;

    stringstream stream(request.code);
    string buf, time_src;
    if(request.code[1] != '_') return false;
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
        if (x1->isValid == true && x2->isValid == true)
        {
            //Get time to key to find in AVL database
            VM_Record avl_key;
            try{
            re1_parseTime(avl_key.timestamp, time_src, recordList[0].timestamp);
            }
            catch(exception& e)
            {
                return false;
            }

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
                cout << request.code[0] << ": " << relative_long << " " << relative_lat << " " << distance;
                return true;
            }
        }
    }

    //If not find anything
    cout << request.code[0] << ": " << "-1";
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
        if (record.isValid)
        {
            bool terminate = false;
            record.data.traverseNLR(isNotEast, p, terminate);
            if (!terminate)
                ((request_data *)p)->cnt++;
        }
    };

    void (*process_re2_W)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Fucntion check if record coordinate not in West
        void (*isNotWest)(VM_Record &, void *, bool &) = [](VM_Record &record, void *p, bool &terminate) {
            if (!((record.longitude - ((request_data *)p)->longitude) <= 0))
                terminate = true;
        };
        if (record.isValid)
        {
            //Counting valid ID
            bool terminate = false;
            record.data.traverseNLR(isNotWest, p, terminate);
            if (!terminate)
                ((request_data *)p)->cnt++;
        }
    };

    stringstream stream(request.code);
    string buf, relative;
    if(request.code[1] != '_') return false;
    if (!getline(stream, buf, '_'))
        return false;
    if(buf != "2") return false;
    if (!getline(stream, buf, '_'))
        return false;
    try{
    request.params[0] = stod(buf);
    }
    catch(exception& e)
    {
        request.params[0] = 0;
    }
    if (!getline(stream, buf, '_'))
        return false;
    relative = buf;

    if (!stream.eof() || relative.length() != 1)
        return false;

    AVLTree<VM_database> *database = (AVLTree<VM_database> *)pGData;
    void *p = new request_data();
    ((request_data *)p)->longitude = request.params[0];
    ((request_data *)p)->cnt = 0;
    if (relative == "E")
        database->traverseLNR(process_re2_E, p);
    else if (relative == "W")
        database->traverseLNR(process_re2_W, p);
    else
        return false;

    //Print result
    cout << request.code[0] << ": " << ((request_data *)p)->cnt;
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
        if (record.isValid)
        {
            //Check valid ID
            bool terminate = false;
            record.data.traverseNLR(re3_counting_N, p, terminate);
            if (!terminate)
                ((request_data *)p)->cnt++;
        }
    };

    void (*process_re3_S)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Fucntion check if record coordinate not in South
        void (*re3_counting_S)(VM_Record &, void *, bool &) = [](VM_Record &record, void *p, bool &terminate) {
            if (!((record.latitude - ((request_data *)p)->latitude) <= 0))
                terminate = true;
        };
        if (record.isValid)
        {
            //Check valid ID
            bool terminate = false;
            record.data.traverseNLR(re3_counting_S, p, terminate);
            if (!terminate)
                ((request_data *)p)->cnt++;
        }
    };

    stringstream stream(request.code);
    string buf, relative;
    if(request.code[1] != '_') return false;
    if (!getline(stream, buf, '_'))
        return false;
    if(buf != "3") return false;
    if (!getline(stream, buf, '_'))
        return false;
    try{
    request.params[0] = stod(buf);
    }
    catch(exception& e)
    {
        return false;
    }
    if (!getline(stream, buf, '_'))
        return false;
    relative = buf;

    if (!stream.eof() || relative.length() != 1)
        return false;

    AVLTree<VM_database> *database = (AVLTree<VM_database> *)pGData;
    void *p = new request_data();
    ((request_data *)p)->latitude = request.params[0];
    ((request_data *)p)->cnt = 0;
    if (relative == "N")
        database->traverseLNR(process_re3_N, p);
    else if (relative == "S")
        database->traverseLNR(process_re3_S, p);
    else
        return false;

    //Print result
    cout << request.code[0] << ": " << ((request_data *)p)->cnt;
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
                //If true,out from traverse data of ID
                ((request_data *)p)->cnt++;
                terminate = true;
            }
        };
        //Check valid ID
        if(record.isValid)
        {
            bool terminate = false;
            record.data.traverseNLR(re4_counting, p, terminate);
        }
    };
    

    //Get param
    void *p = new request_data();
    stringstream stream(request.code);
    string buf;
    if(request.code[1] != '_') return false;
    if (!getline(stream, buf, '_'))
        return false;
    
    try{
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->longitude = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->latitude = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->radius = stod(buf);

    if (!getline(stream, buf, '_'))
        return false;
    request.params[0] = stoi(buf);
    
    if (!getline(stream, buf, '_'))
        return false;
    request.params[1] = stoi(buf);
    }
    catch(exception& e)
    {
        return false;
    }
    if(!stream.eof()) return false;
    AVLTree<VM_database> *database = (AVLTree<VM_database> *)pGData;

    //parse time to tm
    struct tm *tm = gmtime(&recordList[0].timestamp);
    tm->tm_min = 0;
    tm->tm_sec = 0;

    tm->tm_hour = request.params[0];
    ((request_data *)p)->h1 = timegm(tm);
    tm->tm_hour = request.params[1];
    ((request_data *)p)->h2 = timegm(tm);
    
    //
    database->traverseLNR(process_re4, p);

    //Print result
    cout << request.code[0] << ": " << ((request_data *)p)->cnt;
    delete (request_data *)p;
    return true;
}
bool process_request_5(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    void (*re5_counting)(VM_Record &, void *) = [](VM_Record &record, void *p) {
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
    if(request.code[1] != '_') return false;
    //Init key to find ID in database
    if (!getline(stream, buf, '_'))
        return false;
    VM_database key((char *)buf.data());

    int i = 0;
    while (getline(stream, buf, '_'))
    {
        try{
        request.params[i] = stod(buf);
        }
        catch(exception& e)
        {
            return false;
        }
        i++;
    }
    if (i != 3)
        return false;
    AVLTree<VM_database> *database = (AVLTree<VM_database> *)pGData;

    void *p = new request_data();
    ((request_data *)p)->longitude = request.params[0];
    ((request_data *)p)->latitude = request.params[1];
    ((request_data *)p)->radius = request.params[2];
    ((request_data *)p)->isOutCircle = true;

    VM_database *x;
    //Find VM_database and return it pointer of data to x by reference
    if (database->find(key, x))
    {
        if (x->isValid)
        {
            //Traverse AVL in data have key ID
            x->data.traverseLNR(re5_counting, p);

            //Print result
            cout << request.code[0] << ": " << ((request_data *)p)->cnt;
            return true;
        }
    }
    cout << request.code[0] << ": " << "-1";
    delete (request_data *)p;
    return true;
}
bool process_request_6(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    void (*parseTime)(time_t &, string &, time_t &) = [](time_t &dest, string &src, time_t &scale) {
        //parse time to tm
        struct tm *tm = gmtime(&scale);
        tm->tm_hour = stoi(src.substr(0, 2));
        tm->tm_min = stoi(src.substr(2, 2));
        tm->tm_sec = 0;

        dest = timegm(tm);
    };
    void (*re6_counting)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Fucntion check if record coordinate not in East
        void (*inStation)(VM_Record &, void *,bool&) = [](VM_Record &record, void *p,bool& terminate) {
            //If find it,set terminate to true to terminate traverse
            request_data *re_data = (request_data *)p;
            double distance = distanceEarth(re_data->latitude, re_data->longitude, record.latitude, record.longitude);

            if (record.timestamp >= (re_data->t1 - 15*60) && record.timestamp <= (re_data->t1))
            {
                if(re_data->min_distance < 0) re_data->min_distance = distance;
                else if (distance < re_data->min_distance)
                {
                    re_data->min_distance  = distance;
                }
            }
            
        };
        request_data *re_data = (request_data *)p;
        //Counting valid ID
        if (record.isValid)
        {
            re_data->min_distance = -1;
            bool terminate = false;
            record.data.traverseLNR(inStation, p, terminate);
            if (re_data->min_distance >= 0)
            {
                //cout << re_data->min_distance << endl;
                string id = record.id;
                if (re_data->min_distance <= 0.3)
                    re_data->lessThan300m.insertHead(id);
                if (re_data->min_distance <= 0.5)
                    re_data->lessThan500m.insertHead(id);
                if (re_data->min_distance <= 2)
                    re_data->lessThan2km.insertHead(id);
                if (re_data->min_distance > 0.5 && re_data->min_distance <= 2)
                    re_data->lessThan2km_greatThan500m.insertHead(id);

                re_data->min_distance = -1;
            }
        }
    };
    stringstream stream(request.code);
    string buf, time_src;
    void *p = new request_data();
    if(request.code[1] != '_') return false;
    if (!getline(stream, buf, '_'))
        return false;
    
    try{
    //Get data form request code
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->longitude = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->latitude = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->number_of_VM = stod(buf);
    }
    catch(exception& e)
    {
        return false;
    }
    if (!getline(stream, buf, '_'))
        return false;
    time_src = buf;
    if (!stream.eof() && time_src.length() != 4)
        return false;
    try{
    parseTime(((request_data *)p)->t1,time_src,recordList[0].timestamp);
    }
    catch(exception& e)
    {
        return false;
    }
    AVLTree<VM_database> *database = (AVLTree<VM_database> *)pGData;
    ((request_data *)p)->pData = database;

    database->traverseRNL(re6_counting,p);

    void (*print_list)(string &) = [](string &id) {
        cout << " " << id;
    };
    request_data *re_data = (request_data *)p;

    cout << request.code[0] << ":";
    if (re_data->lessThan2km.getSize() < re_data->number_of_VM)
    {
        re_data->lessThan2km.traverse(print_list);
        cout << " - -1";
    }
    else if ((double(re_data->lessThan300m.getSize()) / re_data->number_of_VM) >= 0.75)
    {
        cout << " -1 -";
        re_data->lessThan2km.traverse(print_list);
    }
    else
    {
        re_data->lessThan500m.traverse(print_list);
        cout << " -";
        re_data->lessThan2km_greatThan500m.traverse(print_list);
    }
    delete (request_data *)p;
    return true;
}
bool process_request_7(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    void (*parseTime)(time_t &, string &, time_t &) = [](time_t &dest, string &src, time_t &scale) {
        //parse time to tm
        struct tm *tm = gmtime(&scale);
        tm->tm_hour = stoi(src.substr(0, 2));
        tm->tm_min = stoi(src.substr(2, 2));
        tm->tm_sec = 0;

        dest = timegm(tm);
    };
    void (*re7_counting)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Fucntion check if record coordinate not in East
        void (*inStation)(VM_Record &, void *,bool&) = [](VM_Record &record, void *p,bool& terminate) {
            //If find it,set terminate to true to terminate traverse
            request_data *re_data = (request_data *)p;
            double distance = distanceEarth(re_data->latitude, re_data->longitude, record.latitude, record.longitude);

            if (record.timestamp >= re_data->t1 && record.timestamp <= (re_data->t1 + 30*60))
            {
                if(re_data->min_distance < 0) re_data->min_distance = distance;
                else if (distance < re_data->min_distance)
                {
                    re_data->min_distance  = distance;
                }

                if(re_data->max_distance < 0 && distance >= 1 && distance <= 2) re_data->max_distance = distance;
                else if (distance > re_data->max_distance && distance >= 1 && distance <= 2)
                {
                    re_data->max_distance  = distance;
                }
               
            }
            
        };
        request_data *re_data = (request_data *)p;
        //Counting valid ID
        if (record.isValid)
        {
            re_data->min_distance = re_data->max_distance = -1;
            bool terminate = false;
            record.data.traverseLNR(inStation, p, terminate);
            if (re_data->min_distance >= 0)
            {
                re7_database re;
                strcpy(re.id,record.id);
                re.distance = re_data->max_distance;

                //cout << re_data->min_distance << endl;
                string id = record.id;
                if (re_data->min_distance <= 0.5)
                    re_data->lessThan500m.insertHead(id);
                if (re_data->min_distance < 1)
                    re_data->lessThan1km_andStuck.insert(id);
                if(re_data->min_distance <= 2)
                    re_data->lessThan2km.insertHead(id);
                if (re_data->min_distance >= 1 && re_data->min_distance <= 2 && re_data->max_distance > 0)
                {
                    re_data->lessThan2km_greatThan1km.insert(re);
                    re_data->cnt++;
                }

                re_data->min_distance = re_data->max_distance = -1;
            }
        }
    };
    stringstream stream(request.code);
    string buf, time_src;
    void *p = new request_data();
    if(request.code[1] != '_') return false;
    if (!getline(stream, buf, '_'))
        return false;
    try{
    //Get data form request code
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->longitude = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->latitude = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->number_of_VM = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->radius = stod(buf);
    }
    catch(exception &e)
    {
        return false;
    }
    if (!getline(stream, buf, '_'))
        return false;
    time_src = buf;
    if (!stream.eof() && time_src.length() != 4)
        return false;
    
    ((request_data *)p)->cnt = 0;
    try{
    parseTime(((request_data *)p)->t1,time_src,recordList[0].timestamp);
    }
    catch(exception& e)
    {
        return false;
    }
    AVLTree<VM_database> *database = (AVLTree<VM_database> *)pGData;
    ((request_data *)p)->pData = database;

    database->traverseRNL(re7_counting,p);

    void (*print_list)(string &) = [](string &id) {
        cout << " " << id;
    };
    void (*sort_result)(re7_database&,void*) = [](re7_database &record,void* p) {
        request_data *re_data = (request_data *)p;
        string id = record.id;
        if(re_data->percent_cnt > 0)
        {
            re_data->lessThan2km_greatThan1km_75.insert(id);
            re_data->percent_cnt--;
        }
        else
        {
            re_data->lessThan1km_andStuck.insert(id);
        }
     
    };
    request_data *re_data = (request_data *)p;

    cout << request.code[0] << ":";

    if ((double(re_data->lessThan500m.getSize()) / re_data->number_of_VM) < 0.7)
    {
        cout << " -1 -";
        if(re_data->lessThan2km.getSize() == 0) cout << " -1";
        else re_data->lessThan2km.traverse(print_list); 
    }
    else
    {
        re_data->percent_cnt = 0.75*re_data->cnt;
        re_data->lessThan2km_greatThan1km.traverseRNL(sort_result,p);
        
        if(re_data->lessThan1km_andStuck.isEmpty()) cout << " -1";
        else re_data->lessThan1km_andStuck.traverseLNR(print_list);
        cout << " -";
        if(re_data->lessThan2km_greatThan1km_75.isEmpty()) cout << " -1";
        else re_data->lessThan2km_greatThan1km_75.traverseLNR(print_list);
    }
    delete (request_data *)p;
    return true;
}
bool process_request_8(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    void (*parseTime)(time_t &, string &, time_t &) = [](time_t &dest, string &src, time_t &scale) {
        //parse time to tm
        struct tm *tm = gmtime(&scale);
        tm->tm_hour = stoi(src.substr(0, 2));
        tm->tm_min = stoi(src.substr(2, 2));
        tm->tm_sec = 0;

        dest = timegm(tm);
    };
    void (*re8_counting)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Fucntion check if record coordinate not in East
        void (*isDelay)(VM_Record &, void *, bool &) = [](VM_Record &record, void *p, bool &terminate) {
            //If find it,set terminate to true to terminate traverse
            request_data *re_data = (request_data *)p;
            double distance = distanceEarth(re_data->latitude, re_data->longitude, record.latitude, record.longitude);

            if (distance <= re_data->radius)
            {
                if(record.timestamp >= re_data->t1 && record.timestamp <= (re_data->t1 +59))
                {
                    terminate = true;
                }
            }
        };
        //Counting valid ID
        bool terminate = !record.isValid;
        record.data.traverseNLR(isDelay, p, terminate);
        if (terminate && record.isValid == true)
        {  
           cout << " " << record.id;
           record.isValid = false;
           ((request_data *)p)->cnt++;
        }
    };
    stringstream stream(request.code);
    string buf, time_src;
    void *p = new request_data();
    if(request.code[1] != '_') return false;
    if (!getline(stream, buf, '_'))
        return false;
    try{
    //Get data form request code
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->longitude = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->latitude = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->radius = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    }
    catch(exception &e)
    {
        return false;
    }
    time_src = buf;
    if (!stream.eof() && time_src.length() != 4)
        return false;
    try{
    parseTime(((request_data *)p)->t1,time_src,recordList[0].timestamp);
    }
    catch(exception& e)
    {
        return false;
    }
    AVLTree<VM_database> *database = (AVLTree<VM_database> *)pGData;
    ((request_data *)p)->pData = database;

    cout << request.code[0] << ":";
    database->traverseLNR(re8_counting,p);
    if(((request_data *)p)->cnt == 0) cout << " " << 0;
    delete (request_data *)p;
    return true;
}
bool process_request_9(VM_Request &request, L1List<VM_Record> &recordList, void *pGData)
{
    void (*parseTime)(time_t &, string &, time_t &) = [](time_t &dest, string &src, time_t &scale) {
        //parse time to tm
        struct tm *tm = gmtime(&scale);
        tm->tm_hour = stoi(src.substr(0, 2));
        tm->tm_min = stoi(src.substr(2, 2));
        tm->tm_sec = 0;

        dest = timegm(tm);
    };
    void (*re9_counting)(VM_database &, void *) = [](VM_database &record, void *p) {
        //Fucntion check if record coordinate not in East
        void (*isRecover)(VM_Record &, void *, bool &) = [](VM_Record &record, void *p, bool &terminate) {
            //If find it,set terminate to true to terminate traverse
            request_data *re_data = (request_data *)p;
            double distance = distanceEarth(re_data->latitude, re_data->longitude, record.latitude, record.longitude);

            if (distance <= re_data->radius)
            {
                if(record.timestamp >= re_data->t1 && record.timestamp <= (re_data->t1 +59))
                {
                    terminate = true;
                }
            }
        };
        //Counting valid ID
        bool terminate = record.isValid;
        record.data.traverseNLR(isRecover, p, terminate);
        if (terminate && record.isValid == false)
        {  
           cout << " " << record.id;
           ((request_data *)p)->cnt++;
           record.isValid = true;
        }
    };
    stringstream stream(request.code);
    string buf, time_src;
    void *p = new request_data();
    if(request.code[1] != '_') return false;
    if (!getline(stream, buf, '_'))
        return false;
    try{
    //Get data form request code
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->longitude = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->latitude = stod(buf);
    if (!getline(stream, buf, '_'))
        return false;
    ((request_data *)p)->radius = stod(buf);
    }
    catch(exception &e)
    {
        return false;
    }
    if (!getline(stream, buf, '_'))
        return false;
    time_src = buf;
    if (!stream.eof() && time_src.length() != 4)
        return false;
    
    try{
    parseTime(((request_data *)p)->t1,time_src,recordList[0].timestamp);
    }
    catch(exception &e)
    {
        return false;
    }
    AVLTree<VM_database> *database = (AVLTree<VM_database> *)pGData;
    ((request_data *)p)->pData = database;

    cout << request.code[0] << ":";
    database->traverseLNR(re9_counting,p);
    if(((request_data *)p)->cnt == 0) cout << " " << 0;
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
            AVLTree<VM_database> *p = (AVLTree<VM_database> *)pGData;
            VM_database *x;

            if (!p->find(key, x))
            {
                p->insert(key);
                p->find(key, x);
                x->data.insert(record);
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
        return process_request_6(request, recordList, pGData);
        break;
    case '7':
        return process_request_7(request, recordList, pGData);
        break;
    case '8':
        return process_request_8(request, recordList, pGData);
        break;
    case '9':
        return process_request_9(request, recordList, pGData);
        break;
    default:
        return false; // return false for invlaid events
    }
}