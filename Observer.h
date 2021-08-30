#ifndef _OBSERVER_h
#define _OBSERVER_h

#include "glob.h"
#include "wse/WSE.h"
#include "isp/ISP.h"

class WSE;

class Observer: public process
{

private:

    string s;

    vector<int> miss; //Dejo esto para no cambiar los misses de la parte de Leafset en TlcProtocol.cc
    vector<int> miss_global;
    vector<int> miss_local;
    vector<int> hitLocal;
    vector<int> hitGlobal;
    vector<int> hitWaiting;
    vector<int> hitLCache;
    vector<int> objArrival;

    vector<int> nqueriesIn;
    int nqueriesOut;
    int np;

    vector<int> hops;
    vector<double> latencia;
    double deltaT;
    bool busy, ends;

    vector<int> nReplicas;
    int iteracion;

    vector<int> load;
    int peso_respuestas_expertos;


public:

    int totalQueries;

    int Peers_envio;
    int normal_rate;
    //~ int nodes;
    int cacheSize;
    int localCacheSize;
    int L_cacheSize;
    int iteracion_sim;
    double TTL;
    //~ double client_wse_cte;
    vector <double> utilizacion;
    double utilizacion_promedio;

    vector <int> finalizacion;
    vector <int> broadcasts;
    vector <int> terminadas;
    vector <int> nFin;
    vector <int> ends_obs;

    double T_inicio = 0;
    double T_finalizacion = 0;
    double umbral_votacion;
    double Task_service_time;
    double Task_processing_time;
    vector<int> contador_recepcion1;
    vector<int> contador_recepcion2;
    vector<int> contador_recepcion3;
    vector<int> contador_recepcion4;
    vector<int> contador_recepcion5;
    vector<int> contador_recepcion6;
    int contador_responsable_elim;
    vector<int> contador_responsable_env;
    int contador_enviadas;
    vector<int> contador_recibidas;
    double tiempo_tareas_1;
    double tiempo_tareas_2;
    //Nuevos registros (poner las gráficas de esto en otra carpeta, ya que tienen el mismo propósito de encontrar el error del caso 2).
    vector<double> Time_tag0_client;
    vector<double> Time_tag1_client;
    vector<double> Time_tag2_client;
    vector<double> Time_tag3_client;
    double Time_tagGen_wse;
    int N_tagGen_wse;
    int contador_consenso;
    int contador_miss_en_proceso;
    int contador_consenso_negativo;
    vector<double> Time_tagPeer_wse;
    vector<double> Time_tag3_wse;
    vector<double> Time_tagReenv_wse;
    vector<int> N_tag0_client;
    vector<int> N_tag1_client;
    vector<int> N_tag2_client;
    vector<int> N_tag3_client;
    vector<int> N_tagPeer_wse;
    vector<int> N_tag3_wse;
    vector<int> N_tagReenv_wse;
    vector<int> N_ask_obj;
    vector<int> N_ask_replica;


    vector<double> t_red_p2p;
    vector<double> t_red_isp;
    double t_consenso_peers;
    double t_consenso_server;
    double t_scores_server;
    double t_upd_freq_server;
    double t_proc_server;
    vector<int> contador_isp_Sol_Img;
    vector<int> contador_isp_env_Img;
    vector<int> contador_isp_env_tarea;
    vector<int> contador_isp_resultados;
    int tareas_finalizadas;
    int tareas_finalizadas_previas;
    double terminadas_TP;
    vector<int> tareas_finalizadas_net;
    vector<int> tareas_finalizadas_previas_net;
    vector<double> terminadas_TP_net;

    double mean_DB;
    //~ double interArrT_DB;
    double stdDeviation_DB;
    double maxElement_DB;
    double minElement_DB;

    vector<double> mean_DB_per_net;
    //~ vector<double> interArrT_DB_per_net;
    vector<double> stdDeviation_DB_per_net;
    vector<double> maxElement_DB_per_net;
    vector<double> minElement_DB_per_net;

    handle<WSE> *wse;
    std::map<BIGNUM *, int> codigos;
    std::map<int, BIGNUM *> codigos2;
    vector<int> lim_inf;
    vector<int> lim_sup;
    vector<int> NP;
    int flag_escenario;
    int nro_redes;
    vector<double> TimeStamps;
    double cte_traffJam_isp;
    double cte_traffJam_p2p;
    int flagArquitectura;
    int caso;

	int DL_tasks;
	bool flagDisponibles;
	int disconnections;
	double max_work_time;

    ~Observer()
    {

    }

    Observer(	int _np,
                int _nro_redes,
                vector<int> &_lim_inf,
                vector<int> &_lim_sup,
                const string &name,
                int _totalQueries, int _cacheSize,
                int _Peers_envio, int _normal_rate,
                //int _nodes,
                int _iteracion_sim,
                int _localCacheSize,
                int _L_cacheSize,
                double _limite_TTL,
                //~ double _client_wse_cte,
                double _umbral_votacion,
                int _flag_escenario,
                double _cte_traffJam_isp,
                double _cte_traffJam_p2p,
                int _flagArquitectura,
                int _caso,
                int _peso_respuestas_expertos,
                bool _flagDisponibles,
                double _max_work_time

            ): process(name)
    {

        np = _np;
        nro_redes = _nro_redes;
        for (int j = 0; j < nro_redes; j++)
        {
            lim_inf.push_back(_lim_inf[j]);
            lim_sup.push_back(_lim_sup[j]);
            NP.push_back(lim_sup[j] - lim_inf[j]);
        }

        nqueriesOut = 0;

        deltaT = 0.1;
        ends = false;
        busy = false;
        umbral_votacion = _umbral_votacion;
        mean_DB = 0;
        //~ interArrT_DB=0;
        stdDeviation_DB = 0;
        maxElement_DB = 0;
        minElement_DB = 0;
        iteracion = 0;
        tareas_finalizadas = 0;
        tareas_finalizadas_previas = 0;
        terminadas_TP = 0;
        t_consenso_server = 0;
        t_scores_server = 0;
        t_upd_freq_server = 0;
        t_proc_server = 0;
        t_consenso_peers = 0;
        Time_tagGen_wse = 0;
        N_tagGen_wse = 0;
        contador_consenso = 0;
        contador_miss_en_proceso = 0;
        contador_responsable_elim = 0;
        contador_enviadas = 0;
        DL_tasks = 0;
        disconnections=0;
        flagDisponibles = _flagDisponibles;
        max_work_time = _max_work_time;

        for (int i = 0; i < nro_redes; i++)
        {
            miss.push_back(0);
            hitLocal.push_back(0);
            hitGlobal.push_back(0);
            hitWaiting.push_back(0);
            hitLCache.push_back(0);
            nqueriesIn.push_back(0);
            hops.push_back(0);
            latencia.push_back(0);
            nReplicas.push_back(0);
            miss_global.push_back(0);
            miss_local.push_back(0);
            objArrival.push_back(0);
            contador_recepcion1.push_back(0);
            contador_recepcion2.push_back(0);
            contador_recepcion3.push_back(0);
            contador_recepcion4.push_back(0);
            contador_recepcion5.push_back(0);
            contador_recepcion6.push_back(0);
            contador_responsable_env.push_back(0);
            contador_recibidas.push_back(0);
            t_red_p2p.push_back(0);
            t_red_isp.push_back(0);


            Time_tag0_client.push_back(0);
            Time_tag1_client.push_back(0);
            Time_tag2_client.push_back(0);
            Time_tag3_client.push_back(0);
            Time_tagPeer_wse.push_back(0);
            Time_tag3_wse.push_back(0);
            Time_tagReenv_wse.push_back(0);
            N_tag0_client.push_back(0);
            N_tag1_client.push_back(0);
            N_tag2_client.push_back(0);
            N_tag3_client.push_back(0);
            N_tagPeer_wse.push_back(0);
            N_tag3_wse.push_back(0);
            N_tagReenv_wse.push_back(0);
            N_ask_obj.push_back(0);
            N_ask_replica.push_back(0);
            contador_isp_Sol_Img.push_back(0);
            contador_isp_env_Img.push_back(0);
            contador_isp_env_tarea.push_back(0);
            contador_isp_resultados.push_back(0);
            mean_DB_per_net.push_back(0);
            //~ interArrT_DB_per_net.push_back(0);
            stdDeviation_DB_per_net.push_back(0);
            maxElement_DB_per_net.push_back(0);
            minElement_DB_per_net.push_back(0);
            tareas_finalizadas_net.push_back(0);
            tareas_finalizadas_previas_net.push_back(0);
            terminadas_TP_net.push_back(0);


        }
        for(int i = 0; i < np ; i++)
        {
            load.push_back(0);
        }

        for(int i = 0; i < np ; i++)
        {
            finalizacion.push_back(0);
            broadcasts.push_back(0);
            terminadas.push_back(0);
            nFin.push_back(0);
            ends_obs.push_back(0);
        }
        ends_obs.push_back(0); //Ultimo elemento de ends_obs correspondiente al WSE.

        for(int i = 0; i < np ; i++)
        {
            utilizacion.push_back(0.0);
        }
        utilizacion_promedio = 0;

        iteracion_sim	= _iteracion_sim;
        totalQueries	= _totalQueries;
        cacheSize		= _cacheSize;
        L_cacheSize		= _L_cacheSize;
        localCacheSize	= _localCacheSize;
        Peers_envio		= _Peers_envio;
        normal_rate		= _normal_rate;
        //~ nodes			= _nodes;
        TTL				= _limite_TTL;
        //~ client_wse_cte	= _client_wse_cte;
        flag_escenario = _flag_escenario;
        flagArquitectura = _flagArquitectura;
        cte_traffJam_isp = _cte_traffJam_isp;
        cte_traffJam_p2p = _cte_traffJam_p2p;
        caso = _caso;
        peso_respuestas_expertos = _peso_respuestas_expertos;

        for (int i = 0; i < 20; i++)
        {
            TimeStamps.push_back(0);
        }
    }

    void inner_body( void );

    void addMiss(int red_asignada)
    {
        miss[red_asignada]++;
    }

    void addMiss_global(int red_asignada)
    {
        miss_global[red_asignada]++;
    }

    void addMiss_local(int red_asignada)
    {
        miss_local[red_asignada]++;
    }

    bool isBusy()
    {
        return busy;
    }

    void setBusy(bool p)
    {
        busy = p;
    }

    void addHitLocal(int red_asignada)
    {
        hitLocal[red_asignada]++;
    }

    void addHitGlobal(int red_asignada)
    {
        hitGlobal[red_asignada]++;
    }

    void addHitWaiting(int red_asignada)
    {
        hitWaiting[red_asignada]++;
    }

    void addHitLCache(int red_asignada)
    {
        hitLCache[red_asignada]++;
    }

    void add_objArrival(int red_asignada)
    {
        objArrival[red_asignada]++;
    }

    void addReplicas(int red_asignada)
    {
        nReplicas[red_asignada]++;
    }

    void addNQueriesIn(int red_asignada)
    {
        nqueriesIn[red_asignada]++;
    }

    void addNQueriesOut()
    {
        nqueriesOut++;
    }

    void addHops(int h, int red_asignada)
    {
        hops[red_asignada] = hops[red_asignada] + h;
    }

    void addLatencia(double t, int red_asignada)
    {
        latencia[red_asignada] = latencia[red_asignada] + t;
    }
    void end()
    {
        ends = true;
    }
    bool get_end()
    {
        return ends;
    }


    void print()
    {
        cout << s << endl;
    }

    void addLoad(int ip)
    {
        load[ip]++;
    }

    double getGini();

    double getGini_per_net(int red_asignada, int np_red_asignada);

    double getGini_utilizacion_per_net(int red_asignada, int np_red_asignada);

    double get_AveUtilizacion_per_net(int red_asignada, int np_red_asignada);

    int getIteracion();

    void print_vec(vector<int> &vector_in, const char *nombre, int indice)
    {
        //cout.open ("cout.txt",ios_base::app);
        cout << "#" << indice << ". Vector: " << nombre << endl;
        cout << "#" << indice << ".";
        for (int k = 0; k < (int)(vector_in.size()); k++)
        {
            cout << " " << vector_in[k];
        }
        cout << "\n";
    }

    void print_vec_double(vector<double> &vector_in, const char *nombre)
    {
        cout << "Vector: " << nombre << endl;
        cout.precision(4);
        for (int k = 0; k < (int)(vector_in.size()); k++)
        {
            cout << " " << vector_in[k];
        }
        cout << "\n";
    }

};

#endif
