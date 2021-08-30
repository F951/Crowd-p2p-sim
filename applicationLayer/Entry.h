#ifndef _ENTRY_h
#define _ENTRY_h
#include "../glob.h"
#include "../wse/Rtas_peers.h"


/* Class that represents an entry in Rcache or local Cache */

class Rtas_Peers;
class Entry
{

public:
    long priority;
    //~ string key;
    int key;			//Es el taskID
	int imgID;
    int nro_opciones;
    BIGNUM *hashValue;
    bool isReplicated;
    bool flag;
    int size;

    // new parameters to handle WSE entries
    int version;
    long int timeOut;

    int frequency;
    
    //El peer de agregación tiene que estar desde TLC_WSE, por lo que tiene que estar con los objetos en las Entry.
    NodeEntry * aggregationPeer;


    long int getTimeOut()
    {
        return timeOut;
    }

    ~Entry()
    {
        BN_free(hashValue);
    }

    //Proposal 1
    Entry(double _priority, int _key,int _imgID,int _nro_opciones, BIGNUM *_hashValue,  int _version, int _size, long int time) //, double _TTL_tarea, list<int> _op, int _prioridad_tarea, int _id_imagen, Rtas_Peers *_rta)
    {
        priority = _priority;
        key = _key;
        imgID = _imgID;
        nro_opciones = _nro_opciones;
        hashValue = BN_dup(_hashValue);
        frequency = 0;
        size = _size;
        flag = true;
        isReplicated = false;
        timeOut = time;
        version = _version;
    }
    
    //Proposal 2
    Entry(double _priority, int _key,int _imgID,int _nro_opciones, BIGNUM *_hashValue,  int _version, int _size, long int time, NodeEntry* _aggregationPeer ) 
    {
        priority = _priority;
        key = _key;
        imgID = _imgID;
        nro_opciones = _nro_opciones;
        hashValue = BN_dup(_hashValue);
        frequency = 0;
        size = _size;
        flag = true;
        isReplicated = false;
        timeOut = time;
        version = _version;
        aggregationPeer = _aggregationPeer;
    }

    // when the entry is used is updated (higher priority)
    void update(long timestamps)
    {
        //      cout << "updateEntry" << endl;
        //     fflush(stdout);
        priority = timestamps;
    }


    void updateFrequency()
    {
        frequency = frequency + 1;
    }



    // checks it the ttl is still valid
    bool old(long timestamps)
    {
        if(timestamps > this->timeOut)
        {
            return true;
        }
        else
            return false;
    }

    // helps when sending an entry to other peer
    Entry *clone()
    {
        Entry *tmp = new Entry(this->priority,
                               this->key,
                               this->imgID,
                               this->nro_opciones,
                               this->hashValue,
                               this->version,
                               this->size,
                               this->timeOut,
                               this->aggregationPeer
                               //,this->TTL_tarea, this->opciones, this->prioridad_tarea, this->id_imagen, this->rta
                               );

        return tmp;
    }

    void reset()
    {
        frequency = 0;
    }

};

#endif
