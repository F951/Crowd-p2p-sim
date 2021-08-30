#ifndef _CLIENTTLCWSE_H
#define _CLIENTTLCWSE_H

#include "../glob.h"
#include "../wse/ClientWSE.h"
#include "../wse/WSE.h"
#include "../wse/MessageWSE.h"
#include "../wse/MessageServer.h"
#include "../wse/Task.h"
#include "Query.h"
#include "Entry.h"
#include "Voluntario.h"
#include "TlcMessage.h"
#include "../isp/ISP.h"

class TlcProtocol;


class ClientTlcWse: public ClientWSE
{
public:

    //   list<MessageWSE *> queue_in;
    handle<WSE> *wse;
    handle<ClientWSE> *I_obj;
    handle<TlcProtocol> *tlc;
    handle<Voluntario> *voluntario;
    Hash *h;
    double rate_trabajo;
    rng<double> *work_time1, *work_time2;
    int habilidad = 0;

    BIGNUM *pid;
    int totalQueries;
    handle<Observer> *observer;
    handle<ISP> *isp;

    //Fin simulacion
    int *ends;
    double cte_atencion;
    bool end_cliente = 0;

    map<int, Rtas_Peers *> en_proceso_local;
    
    map<int, Task *> activeTasks;	
    
    map<int, int > activeTasksStatus;
    
    int consultas_totales = 0;
    int contador_miss_en_proceso = 0;
    int enviadas = 0;
    int H;
    int flag_terminar;
    int red_nro;
    int flagArquitectura;
    int serverID;
    double umbral_votacion;

    int flagDL = 0;
    
    int caso;

    ~ClientTlcWse ( )
    {
        //free (wse);
        //free(tlc);
    }

    ClientTlcWse ( const string &name, handle<WSE> *_wse,
                   int _habilidad,
                   double _umbral_votacion,
                   BIGNUM *_pid,
                   handle<Observer> *_obs,
                   int _tQueries,
                   int *_ends,
                   int _red_nro,
                   int _flagArquitectura,
                   int _serverID,
                   int _caso
                 )
        : ClientWSE ( name )
    {
        busy = false;
        wse = _wse;
        work_time1 = new rngNormal( "Normal", 1.0, 0.1 );
        work_time2 = new rngExp( "NegExp 2", 0.6 );
        work_time1->reset();
        work_time2->reset();
        habilidad = _habilidad;
        umbral_votacion = _umbral_votacion / 100.0;
        pid  = _pid;
        observer = _obs;
        totalQueries = _tQueries;
        ends = _ends;
        flag_terminar = 0;
        red_nro = _red_nro;
        flagArquitectura = _flagArquitectura;
        serverID = _serverID;
        caso = _caso;
        // cout << "Habilidad peer: " << habilidad << endl;
    }

    void inner_body(void);

    void set_tlc(handle<TlcProtocol> *t);

    void set_isp(handle<ISP> *_isp)
    {
        isp = _isp;
    }

    /*Implemented to make the connection with WSE */
    void set_ClientWSE_obj(handle<ClientTlcWse> *t)
    {
        I_obj = (handle<ClientWSE> *)t;
    }

    handle<ClientWSE> *get_ClientWSE_obj()
    {
        return I_obj;
    }

    void set_voluntario(handle<Voluntario> *v)
    {
        voluntario = v;
    }

    void sendWSEQuery(Query *q);
    
    void sendWSEQuery2(Query *q);

    long int getTTL(BIGNUM *);
    long int getTimeIncremental(long int);

    void printLista(std::list<int> &, const char *, int);

    void mostrar_tareas_en_proceso_local();
    void mostrar_tareas_en_proceso();

    void print_rta(Rtas_Peers *&rta);
    void acumular_rtas(Rtas_Peers *rta, int id_peer, int indice);

    int calcular_consenso(int indice,
                          int peers_reenvio,
                          map<int, Rtas_Peers *> &en_proceso_local,
                          double umbral_votacion,
                          int H,
                          MessageWSE *&temp
                         );
    void diferencias_en_listas(list<int> &lista1, list<int> &lista2, int indice);

	void terminar(  );
    
    void debug(double time, string msg  );

    void print_msg_model(MessageWSE *temp);

    void sendMessage2Server(MessageServer *m);

    void forward2volunteer(MessageServer *m);
    
    void baselineAlgorithm(MessageServer *mS, int sentQueries);
    
    void proposal1Algorithm(MessageServer *mS, int sentQueries);
    
    void proposal2Algorithm(MessageServer *mS, int sentQueries);
    
    void taskStatus(int indice);
		
    void insert_task(MessageServer *mS, double time, int _red_asignada);
    
    bool validTTL(int indice);

    bool allAnswers(int indice);
    
    void calculateConsensus(int indice);
    
    void sendAggregatedAnswers(int indice);
    
    bool TTLterminated(int indice);

    bool allAnswersProp2(int indice);

	void DLprocessing(MessageServer* temp);
	
	void receiveDLmodel(MessageServer* temp);
	
	void sendConnectionMsg();
	
	void sendDisconnectionMsg();
};



#endif
