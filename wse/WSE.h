#ifndef WSE_H
#define WSE_H

#include "../glob.h"
#include "Rtas_peers.h"
#include "MessageWSE.h"
#include "MessageServer.h"
#include "LruA.h"
#include "Task.h"

#include "../auxiliar/Util.h"
#include "../auxiliar/Hash.h"
#include "../Observer.h"

#include <iostream>
#include "../generador/Mje_Gen_WSE.h"
#include <ctime>

#include "NPRand.h"

//#define WSECACHESIZE 8000

class TlcProtocol;
class ClientTlcWse;
class BD;

class Observer;
class ISP;

class WSE: public process
{

private:
    vector<handle<TlcProtocol> *> Peers;
    vector<handle<ClientTlcWse> *> clientes_wse_Peers;

public:

    handle<BD> *bd;
    list<Mje_Gen_WSE *> queue_in_Gen; 	//Msjes del Generador
    list<MessageServer *> queue_in_Gen2; 	//Msjes del Generador

    list<MessageWSE *> queue_in;			//Msjes de los peers
    list<MessageServer *> queue_in2;			//Msjes de los peers

    handle<WSE> *puntero_propio;
    handle<Observer> *observer;
    vector<handle<ISP>*> isp;

    map<int, Rtas_Peers *> en_proceso;	//Lista numerada de tareas, con la ID de imagen, el TTL, y un contador para ver cuándo respondieron todos los peers
    map<int, Task *> activeTasks;		//Lista numerada de tareas, con la ID de imagen, el TTL, y un contador para ver cuándo respondieron todos los peers

    bool busy;
    int cacheSize = 0;

    int end_WSE;
    int *ends;
    int Nuser;
    Hash *h;
    int H;
    double rate_reenvio;
    int totalQueries;
    list<int> lista_reenvio;
    double parametro_TTL;
    int total_NP;
    int serverID;
    vector<int> NP;
    double umbral_votacion;
    int enviadas;

    double promedio_ponderado_en_proceso;
    double promedio_ponderado_queue_in;
    double start_virtual_t_wse_gen;
    double stop_virtual_t_wse_gen;
    double virtual_t_wse_gen;
    double start_virtual_t_wse_peers;
    double stop_virtual_t_wse_peers;
    double virtual_t_wse_peers;
    double t_com_wse_bd;
    double perc_consenso;
    long double cpu_time_Wse;
    int consultas_totales;
    int consultas_enviadas_desde_peers = 0;	//Es para comparar con consultas_totales, a ver si se están enviando bien y se reciben mal (error de ceros).
    int consultas_enviadas_desde_tlc = 0;	//Es para comparar con consultas_totales, a ver si se están enviando bien y se reciben mal (error de ceros).
    int gen_stop = 0;
    double control_TTLs_anterior = 0;
    int debug_repeticion_responsables = 0;
    vector <int> SolvedTasks;
    int AGREGACION_PEERS = 0;
    int nro_redes;
    vector<int> lim_inf;
    vector<int> lim_sup;
    //~ map<int, set<int> *> peers_disponibles;
    map<int, set<int> > peers_disponibles;
    int flagArquitectura;
    double LATENCIA_SERVER_FLIGHT_PATH;

    vector <double> ranking_voluntarios;

    vector<int> tareas_DL;

    vector<int> voluntarios_DL;
    vector<int> voluntarios_DL_confirmados;
    int flag_DL_confirmados = 0;
    NPRand randomGen;

    vector <int> frecuencias_voluntarios;
    vector <int> opciones_totales_voluntarios;
    vector <int> TotalTasks;
    vector <double> porc_utilizacion_voluntarios;
	int contador_tareas_enviadas = 0;
	
	bool flagDisponibles;

	int caso;
	//~ int contador_historia;
	int contador_historia;
    int max_lenght_historia = 10;
    bool flagHistorias;
	
	map <int,NodeEntry*> aggregationPeers;
	
    ~WSE()
    {

    }

    WSE ( 			const string &name,
                    handle<BD> *_bd,
                    int _total_NP,
                    vector<int> &_NP,
                    int *_ends,
                    int _H,
                    double _parametro_TTL,
                    int _totalQueries,
                    double _umbral_votacion,
                    int _nro_redes,
                    vector<int> &_lim_inf,
                    vector<int> &_lim_sup,
                    int _flagArquitectura,
                    int _cacheSize,
                    int _caso,
                    bool _flagDisponibles,
                    bool _flagHistorias
                    
        ): process ( name )
    {
        busy = false;
        cacheSize = _cacheSize;
        bd = _bd;
        h = new Hash();
        NP              = _NP;
        ends            = _ends;
        H		= _H;
        end_WSE			= 0;
        parametro_TTL = _parametro_TTL;
        totalQueries 	= _totalQueries;
        umbral_votacion = _umbral_votacion / 100.0;

        promedio_ponderado_en_proceso = 0;
        promedio_ponderado_queue_in = 0;
        virtual_t_wse_gen = 0;
        virtual_t_wse_peers = 0;
        t_com_wse_bd = 0;
        perc_consenso = 0;
        cpu_time_Wse = 0;
        consultas_totales = 0;
        enviadas = 0;
        nro_redes = _nro_redes;
        total_NP = _total_NP ;
        serverID = _total_NP ;
        contador_historia=0;
        for (int j = 0; j < nro_redes; j++)
        {
            NP.push_back(_NP[j]);
            lim_inf.push_back(_lim_inf[j]);
            lim_sup.push_back(_lim_sup[j]);
        }

        //~ for (int j = 0; j < nro_redes; j++)
        //~ {
            //~ peers_disponibles[j] = new set<int>;
        //~ }

        flagArquitectura = _flagArquitectura;
        caso = _caso;
        
        randomGen.set_seed(0);

        for (int i = 0; i < total_NP; i++)
        {
            SolvedTasks.push_back(0);
        }
        for (int i = 0; i < total_NP; i++)
        {
            TotalTasks.push_back(0);
        }
        for (int i = 0; i < total_NP; i++)
        {
            frecuencias_voluntarios.push_back(0);
        }
        for (int i = 0; i < total_NP; i++)
        {
            opciones_totales_voluntarios.push_back(0);
        }
        for (int i = 0; i < total_NP; i++)
        {
            ranking_voluntarios.push_back(0);
        }
        for (int i = 0; i < total_NP; i++)
        {
            porc_utilizacion_voluntarios.push_back(0);
        }
        
        flagHistorias = _flagHistorias;
        flagDisponibles = _flagDisponibles;
        if (flagDisponibles) peers_disponibles_init();

    }

    /*Implemented to make the connection with WSE */
    void set_WSE_obj(handle<WSE> *t)
    {
        puntero_propio = t;
    }

    void set_handle_obs(handle<Observer> *_obs)
    {
        observer = _obs;
    }

    void inner_body ( void );

    void add_request ( MessageWSE *bn )
    {
        queue_in.push_back(bn);
    }

    void add_request_Gen( Mje_Gen_WSE *_m)
    {
        queue_in_Gen.push_back(_m);
    }

    void add_request2 ( MessageServer *bn )
    {
        queue_in2.push_back(bn);
    }

    void add_request_Gen2( MessageServer *_m)
    {
        //~ cout << "WSE - new message in, taskID: " << _m->getTaskID() << endl;
        queue_in_Gen2.push_back(_m);
    }

    bool get_busy()
    {
        return busy;
    }

    void print()
    {
        cout << "WSE" << endl;
    }

    long int getTTL(BIGNUM *);

    long int randomTTL();

    int calcular_votacion(int,
                          int, map<int, Rtas_Peers *> &,
                          vector <int> &, vector <int> &, vector <int> &, vector <double> &, vector <double> &,
                          double umbral_votacion,
                          int H,
                          int total_NP,
                          vector<int> &NP,
                          int *,
                          vector<handle<TlcProtocol> *> &,
                          vector<handle<ClientTlcWse> *> &,
                          handle<BD> *bd,
                          vector<int> &,
                          vector<int> &,
                          vector<double> &,
                          vector<int> &peers_seleccionados_reenvio
                         );

    void encontrar_maximos(vector<double> &, vector<int> &, int H, int red_asignada);

    void encontrar_minimos(vector<double> &, vector<int> &, int H, int red_asignada);

    void print_vec(vector<int> &, const char *, int );

    void print_vec_double(vector<double> &, const char * );

    void add_peer(handle<TlcProtocol> *p)
    {
        Peers.push_back(p);
    }

    void add_peer(handle<ClientTlcWse> *p)
    {
        clientes_wse_Peers.push_back(p);
    }

    void free_WSE()
    {
        Peers.clear();
        queue_in.clear();
        queue_in_Gen.clear();
    }

    void end_replicas()
    {
        end_WSE = 1;
    }




    void printLista(std::list<int> &lista, const char *nombre)
    {
        std::list<int>::iterator it_lista;
        cout << "==============Lista: " << nombre << "\n";
        for (it_lista = lista.begin(); it_lista != lista.end(); ++it_lista)
        {
            cout << ' ' << *it_lista;
        }
        cout << "\n==============\n";
    }


    void actualizar_frecuencias(vector <int> &vector_frecuencias,
                                map<int, list<int> > &votacion);

    void seleccion_peers_reenvio(int red_asignada, int indice, vector <double> &ranking_voluntarios, int H, vector <int> &peers_seleccionados_reenvio);

    void print_rta(Rtas_Peers *&rta);

    void terminar_tarea(map<int, Rtas_Peers *> &, int, handle<BD>*& );

    void finalizacion();

    void control_TTLs();

    void revisar_vencimientos();

    void actualizar_puntajes(vector <int> &, vector <double> &,	vector <double> &,
                             vector <int> &,	vector <int> &,
                             vector<handle<TlcProtocol> *> &
                            );

    int reenvio_tarea( int,
                       int,
                       map<int, Rtas_Peers *> &,
                       vector <double> &,
                       int total_NP,
                       vector<int> &NP,
                       int *,
                       vector<handle<TlcProtocol> *> &
                     );

    void termino_tarea( int, map<int, Rtas_Peers *> &, int,  handle<BD> *	);

    void mostrar_tareas_en_proceso(int );

    void mostrar_tareas_en_proceso_cout(int indice);

    void acumular_rtas(Rtas_Peers *rta, int id_peer, int indice);

    //~ int calcular_consenso(int indice,
    //~ map<int, Rtas_Peers*> &en_proceso,
    //~ double umbral_votacion,
    //~ int H
    //~ );

    void calcular_consenso(int indice);

    int seleccion_peer_agregacion(vector<double> &, int red_asignada);

    int select_random(vector<int> &);

    void mostrar_redes();

    void mostrar_peers_disponibles(int id_tarea);

    void incremento_flags(list<int> &lista, vector <int> &opciones_totales_voluntarios, int nro_opciones);

    void encontrar_maximos_disponibles(vector <double> &ranking_voluntarios, vector <int> &peers_seleccionados_reenvio, int H, int red_asignada);

    void encontrar_maximos_NO_disponibles(vector <double> &ranking_voluntarios, vector <int> &peers_seleccionados_reenvio, int H, int red_asignada);

    void select_volunteers(int red_asignada, int contador_historia, Mje_Gen_WSE *mG, vector <int> opciones_totales_voluntarios);

    void send_message(int red_asignada, int *contador_historia, Mje_Gen_WSE *mG, int cacheSize, int debug_wse,
                      BIGNUM *hashValue, string ptr, double TTL, Rtas_Peers *rta, int max_lenght_historia, int flag_escenario);

    void reenvio_tarea_reemplazo(vector<int> &peers_seleccionados_reenvio, int indice, int cacheSize,
                                 int red_asignada, vector <double> &ranking_voluntarios, int H );

    void update_timestamp(int id_tarea, int TimeStamp, double this_time);

    int random_task_filter_server(double prob);

    void select_peers_DL(int red_asignada, vector <int> &voluntarios);

    //void select_random_peer_DL(Mje_Gen_WSE *mG, vector <int> &voluntarios);
    int select_random_peer_DL(vector <int> &voluntarios_DL);


    void send_model(int red_asignada, vector <int> &voluntarios_DL);

    void debug(double time, string msg  );

    void insert_task(MessageServer *, double time,  int _red_asignada);

    void send_message2(int taskID);

    list<int> select_volunteers2(int taskID, int contador_historia);

    void showActiveTasks();

    void endTask(int _indice, int _consenso);

    bool validTTL(int indice);

    bool allAnswers(int indice);

    void sendObject2Peer(MessageServer *mS);

    //~ void accumulateAnswers(MessageServer* mS, int indice);

    void calculateConsensus(int indice);

    void updateFrequencies(int indice);

    void updateScores();

    void resendOrTerminate(int indice);

    vector<int > selectPeersResend(int indice);

    void resend(int indice, vector<int> peers);
        
	void baselineAlgorithm(MessageServer *mS);
    
    void proposal1Algorithm(MessageServer *mS);
    
    void proposal2Algorithm(MessageServer *mS);
    
    void sendObject2PeerProp2(MessageServer *mS);
    
    void checkTaskTimeOut();
    
    int isRelevant();
    
    void checkResultDL(MessageServer *temp);
    
    void restControl(MessageServer* temp);

	int findNet(int peer);
	
	void add_All_AvailablePeers(list<int> &peers_consultados, int red_asignada);
	
	void add_N_AvailablePeers(list<int> &peers_consultados, int red_asignada, int N);
	
	void add_N_UnavailablePeers(list<int> &peers_consultados, int red_asignada, int N);
	
	void peers_disponibles_init(){
		int peer=0;
		for (int i=0;i<nro_redes;i++){
			while (peer < lim_sup[i]){
				peers_disponibles[i].insert(peer);
				peer++;
			} 
		}
	}

};



#endif
