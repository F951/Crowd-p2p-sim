#ifndef _Query_h
#define _Query_h

#include "../glob.h"
#include "../p2pLayer/NodeEntry.h"
class Query
{
public:

    //char* term;
    //~ string term;
    int taskID;
    int imgID;
    int nro_opciones;
    BIGNUM *hashValue;
    int id;
    int rte;
    double t_start;
    int hops;
    NodeEntry *src;

    long int timeOut;
    bool fc;
    
    //~ Rtas_Peers *rta;
    //~ double TTL_tarea;
    //~ list<int> opciones;
    //~ int prioridad_tarea;

    long int getTimeOut()
    {
        return timeOut;
    }
    
    int getTaskID()
    {
        return taskID;
    }

    void renewTimeOut(long int thistime)
    {
        timeOut = thistime + TIMEOUT_Q;
    }

    void forceRouting()
    {
        fc = true;
    }
    bool getForceRouting()
    {
        return fc;
    }

	//Query(int _id, string _term, BIGNUM* _hashValue,  double _t_start, double _TTL_tarea, list<int> _opciones, int _prioridad_tarea,    Rtas_Peers* _rta)
    //~ Query(int _id, string _term, BIGNUM *_hashValue,  double _t_start, double _TTL_tarea, Rtas_Peers *_rta)
    //~ Query(int _id, string _term, BIGNUM *_hashValue,  double _t_start)
    Query(int _id, int _taskID,int _imgID,int _nro_opciones, BIGNUM *_hashValue, double _t_start)
    {
        //~ term = _term;
        taskID = _taskID;
        imgID = _imgID;
        nro_opciones = _nro_opciones;
        //term=(char*)malloc(strlen(ptr)+1);
        //strcpy(term,ptr);
        //term[strlen(ptr)]='\0';
        // term=ptr;
        id = _id;
        t_start = _t_start;
        hops = 0;
        hashValue = _hashValue;

        timeOut = _t_start + TIMEOUT_Q;
        fc = false;
        
        //~ TTL_tarea          = _TTL_tarea;
        //~ rta = _rta;
    }

    void setQuerySrc(NodeEntry *_ne)
    {
        src = _ne;
    }
};
#endif
