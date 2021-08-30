#ifndef _MSG_WSE_H
#define _MSG_WSE_H

#include "../glob.h"
#include "ClientWSE.h"
#include "../p2pLayer/NodeEntry.h"

// Message sent by WSE

enum MessageWSEType
{
    WSE_to_PEER = 0,
    PEER_to_WSE = 1,
    MODEL = 550,
    MODEL_OK = 551,
    TLC_TO_CLIENT = 552
};

class MessageWSE
{

public:

    handle<ClientWSE> *src;
    string key;
    BIGNUM *query;

    double TTL_tarea;
    list<int> opciones;
    int prioridad_tarea;
    int tag;
    int id_imagen;

    int version;
    int source;//peer or user
    int sentido;

    //BIGNUM* responsable_agreg;
    NodeEntry *responsable_agreg;
    Rtas_Peers *rta;
    MessageWSEType type;
    int desconexion_voluntario;

    ~MessageWSE()
    {
        //free( src);
        BN_free(query);
        //~ free(rta);
        delete rta;
        free(responsable_agreg);
    }

    //========================================================================================================= Constructores nuevos
    MessageWSE(handle<ClientWSE> *_src,  BIGNUM *_query, string _key, int _srcK, double _TTL_tarea, int _tag, int _id_imagen, Rtas_Peers *_rta)
    {
        //~ cout << "DEBUG MessageWSE IN"<< endl;
        src    = _src;
        query  = BN_dup(_query);
        key.assign(_key);
        source = _srcK;
        TTL_tarea	    = _TTL_tarea;
        tag				= _tag;
        id_imagen		= _id_imagen;
        assert(_rta != NULL);
        //~ rta=_rta;
        //~ _rta->print_rta();
        //~ cout << "DEBUG MessageWSE - intento copiar rta"<< endl;
        rta = new Rtas_Peers();
        rta->copy(_rta);
        //~ cout << "DEBUG MessageWSE - copiar rta OK"<< endl;
        //~ rta->copy(*_rta);
        desconexion_voluntario = 0;
        //~ cout << "DEBUG MessageWSE OUT"<< endl;
        //~ delete _rta;
    }

    MessageWSE(handle<ClientWSE> *_src,  BIGNUM *_query, string _key, int _srcK, double _TTL_tarea, int _tag, int _id_imagen, Rtas_Peers *_rta, NodeEntry *_responsable_agreg)
    {
        //~ cout << "DEBUG MessageWSE IN"<< endl;
        src    = _src;
        query  = BN_dup(_query);
        key.assign(_key);
        source = _srcK;
        TTL_tarea	    = _TTL_tarea;
        tag				= _tag;
        id_imagen		= _id_imagen;
        assert(_rta != NULL);
        //rta=_rta;
        //~ cout << "DEBUG MessageWSE - intento copiar rta"<< endl;
        rta = new Rtas_Peers();
        rta->copy(_rta);
        //~ cout << "DEBUG MessageWSE - copiar rta OK"<< endl;
        //~ rta->copy(*_rta);
        if (_responsable_agreg)
        {
            responsable_agreg = new NodeEntry(_responsable_agreg->getNodeID(), _responsable_agreg->getIP()) ;
        }
        desconexion_voluntario = 0;
        //~ cout << "DEBUG MessageWSE OUT"<< endl;
        //~ delete _rta;
    }
    //=========================================================================================================

    string getKey()
    {
        return key;
    }

    BIGNUM *getQuery()
    {
        return query;
    }

    void setAnswer(int _version)
    {
        version = version;
    }

    int getVersion()
    {
        return version;
    }

    void set_type(MessageWSEType _type)
    {
        type = _type;
    }

    int getType()
    {
        return type;
    }

};


#endif
