#ifndef _MSG_SERVER_H
#define _MSG_SERVER_H

#include "../glob.h"

enum MsgType
{
    //WSE_to_PEER = 0 ,
    //PEER_to_WSE = 1,
    //MODEL = 550,
    //MODEL_OK = 551,
    //TLC_TO_CLIENT = 552
    //-----------------------------------------------------
    GEN_2_SERVER = 0,
    SEND_TASK = 1,
    ASK_OBJ = 2,
    SEND_OBJ = 3,
    SEND_OBJ_OWNER = 4,
    SEND_RESULT = 5,
    AGGREGATE_TASK = 6,
    SEND_DL_MODEL = 7,
    DL_MODEL_OK = 8,
    RESULT_DL = 9,
    CONNECT = 10,
    DISCONNECT = 11
	//-----------------------------------------------------
	
};

class MessageServer
{

private:
    //-----------------------------------------------------
    MsgType type;

    int taskID;

    int imgID;

    //BIGNUM* query;

    double taskTTL;

    int src;

    int dest;

    int nro_opciones;

    list<int> opciones;

    int desconexion_voluntario;

    double tiempo_generacion;

    double tiempo_inicio;

    double tiempo_finalizacion;
    
    //~ NodeEntry responsable_agregacion(BIGNUM * i,0);
    NodeEntry* responsable_agregacion;
    
    int responsable_agregacion_ingresado;

	map<int, list<int> > votes;			//Proposal 2.
	
	int consenso;
	
	int flagFirstMessage;

public:
    ////-----------------------------------------------------
    //handle<ClientWSE>* src;		//Esto ahora no haría falta. El WSE tenía los handles, pero el ISP es quien hace el routing. Entonces, pasar la lista al ISP y transportar solo un entero.
    //string key; 				//Ver para qué sirve
    //BIGNUM* query;				//Ver para qué sirve
    //
    //double TTL_tarea;
    //list<int> opciones; int prioridad_tarea; int tag;
    //int id_imagen;
    //
    //int version;
    //int source;//peer or user
    //int sentido;
    //
    ////BIGNUM* responsable_agreg;
    //NodeEntry* responsable_agreg;
    //Rtas_Peers* rta;
    //MsgType type;
    //int desconexion_voluntario;
    ////-----------------------------------------------------


    ~MessageServer()
    {
        //BN_free(query);

        ////-----------------------------------------------------
        ////free( src);
        //BN_free(query);
        ////~ free(rta);
        //delete rta;
        //free(responsable_agreg);
        ////-----------------------------------------------------
        
        //~ if (responsable_agregacion_ingresado==1){
			//~ delete responsable_agregacion;
		//~ }
    }

    //========================================================================================================= Constructores nuevos
    MessageServer( MsgType _type, int _taskID, int _imgID, //BIGNUM* _query,
                   double _taskTTL, int _src, int _dest, int _nro_opciones, int _desconexion_voluntario	)
    {
        type = _type;
        taskID = _taskID;
        imgID = _imgID;
        //query = _query;
        taskTTL = _taskTTL;
        src = _src;
        dest = _dest;
        nro_opciones = _nro_opciones;
        desconexion_voluntario = desconexion_voluntario;
        for (int i = 0; i < 6; i++)
        {
            opciones.push_back(-1);
        }

        tiempo_generacion = 0;
        tiempo_inicio = 0;
        tiempo_finalizacion = 0;
        responsable_agregacion_ingresado = 0;
		flagFirstMessage = 1;
    }
    //========================================================================================================= Constructores viejos
    //MessageServer(handle<ClientWSE>* _src,  BIGNUM* _query, string _key, int _srcK, double _TTL_tarea, int _tag, int _id_imagen, Rtas_Peers* _rta)
    //{
    //	src    = _src;
    //	query  = BN_dup(_query);
    //	key.assign(_key);
    //	source = _srcK;
    //	TTL_tarea	    = _TTL_tarea;
    //    tag				= _tag;
    //	id_imagen		= _id_imagen;
    //	assert(_rta!=NULL);
    //	rta = new Rtas_Peers();
    //	rta->copy(_rta);
    //	desconexion_voluntario=0;
    //}

    //=========================================================================================================


    void print()
    {
        //~ cout << "MsgServer[Type:"<< getStringType()<<"][taskID:"<< taskID<<"][imgID:"<<imgID<<"][query:"<<query<<"][TTL:"<< taskTTL<<"][src:"<< src<<"][dest:"<<dest<<"][Nop:"<<nro_opciones<<"][deconexion:"<<desconexion_voluntario<< "]" <<endl;
        cout << "MsgServer[Type:" << getStringType() << "][taskID:" << taskID << "][imgID:" << imgID << "][TTL:" << taskTTL << "][src:" << src << "][dest:" << dest << "][Nop:" << nro_opciones << "][deconexion:" << desconexion_voluntario << "]" << endl;
        if (type == SEND_RESULT)
        {
            cout << "Opciones: ";
            list<int>::iterator it;
            for (it = opciones.begin(); it != opciones.end(); it++)
            {
                cout << (*it) << ", ";
            }
            cout << endl;
        }
    }

    void print_opciones()
    {
        //~ cout << "MsgServer[Type:"<< getStringType()<<"][taskID:"<< taskID<<"][imgID:"<<imgID<<"][TTL:"<< taskTTL<<"][query:"<<query<<"][src:"<< src<<"][dest:"<<dest<<"][Nop:"<<nro_opciones<<"][deconexion:"<<desconexion_voluntario<< "]" <<  endl;
        cout << "MsgServer[Type:" << getStringType() << "][taskID:" << taskID << "][imgID:" << imgID << "][TTL:" << taskTTL << "][src:" << src << "][dest:" << dest << "][Nop:" << nro_opciones << "][deconexion:" << desconexion_voluntario << "]" <<  endl;
        cout << "Opciones: ";
        list<int>::iterator it;
        for (it = opciones.begin(); it != opciones.end(); it++)
        {
            cout << (*it) << ", ";
        }
        cout << endl;
    }

    string getStringType()
    {
        switch (type)
        {
        case 0:
            return "GEN_2_SERVER";
        case 1:
            return "SEND_TASK";
        case 2:
            return "ASK_OBJ";
        case 3:
            return "SEND_OBJ";
        case 4:
            return "SEND_OBJ_OWNER";
        case 5:
            return "SEND_RESULT";
        case 6:
            return "AGGREGATE_TASK";
        case 7:
            return "SEND_DL_MODEL";
        case 8:
            return "DL_MODEL_OK";
        case 9:
            return "RESULT_DL";
        case 10:
            return "CONNECT";
        case 11:
            return "DISCONNECT";
        default:
            return "None";
        }

    }
    //~ BIGNUM* getQuery()
    //~ {
    //~ return query;
    //~ }


    void set_type(MsgType _type)
    {
        type = _type;
    }
    void set_Src(int _src)
    {
        src = _src;
    }
    void set_Dest(int _dest)
    {
        dest = _dest;
    }
    void set_TTL(int _TTL)
    {
        taskTTL = _TTL;
    }

    void set_genT(double time)
    {
        tiempo_generacion = time;
    }

    void set_InitT(double time)
    {
        tiempo_inicio = time;
    }

    void set_FinalT(double time)
    {
        tiempo_finalizacion = time;
    }
    
    void set_optionNumber(int opNbr)
    {
        nro_opciones = opNbr;
    }
    void set_consensus(int _consenso)
    {
        consenso = _consenso;
    }
    
    void set_aggregationPeer(NodeEntry* _responsable_agregacion)
    {
        //~ responsable_agregacion->setNodeID(_responsable_agregacion->getNodeID());
        //~ responsable_agregacion->setIP(_responsable_agregacion->getIP());
        responsable_agregacion = _responsable_agregacion;
        responsable_agregacion_ingresado=1;
    }
    
    void set_FirstMessage(int _flagFirstMessage)
    {
        flagFirstMessage = _flagFirstMessage;
    }
    int getFirstMessageFlag()
    {
        return flagFirstMessage;
    }
    
    MsgType getType()
    {
        return type;
    }

    int getTaskID()
    {
        return taskID;
    }
    int getImgID()
    {
        return taskID;
    }
    int getTTL()
    {
        return taskTTL;
    }
    int getOptionNr()
    {
        return nro_opciones;
    }
    int getSrc()
    {
        return src;
    }
    int getDest()
    {
        return dest;
    }
    double getInitT()
    {
        return tiempo_inicio;
    }
    double getGenT()
    {
        return tiempo_generacion;
    }
    double getFinalT()
    {
        return tiempo_finalizacion;
    }
    double getConsenso()
    {
        return consenso;
    }

    list<int> getOpciones()
    {
        return opciones;
    }

    NodeEntry* getResponsableAgregacion()
    {
        return responsable_agregacion;
    }
    int getAggregationPeerID()
    {
        return responsable_agregacion->getIP();
    }

    void set_opciones(list<int> _opciones)
    {
        opciones = _opciones;
    }
    
	void set_votes(map <int,list<int> > _votes)
    {
		votes=_votes;
	}

};


#endif
