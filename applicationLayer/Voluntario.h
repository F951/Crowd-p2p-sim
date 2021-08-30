#ifndef _Voluntario_H
#define _Voluntario_H

#include "../glob.h"
#include "../wse/ClientWSE.h"
#include "../wse/WSE.h"
#include "../wse/MessageWSE.h"
#include "Query.h"
#include "Entry.h"
#include "TlcMessage.h"
#include "../isp/ISP.h"



class Voluntario: public process
{
public:

    list<MessageWSE *> queue_in;
    list<MessageServer *> queue_in2;
    double rate_trabajo;
    rng<double> *work_time1, *work_time2;
    int habilidad = 0;
    map<int, Rtas_Peers *> en_proceso_local;
    handle<Observer> *observer;
    int red_nro;
    //Fin simulacion
    int flag_terminar;
    int id_peer_client;
    //~ handle<ClientTlcWse> *client;
    handle<ClientWSE> *client;
    //~ handle<ClientWSE> *I_obj;
    int busy;
    int flag_descanso;
    int flag_escenario;
    int peso_respuestas_expertos;

	//Tiempo de descanso: tiempo en que el usuario está desconectado luego de realizar una sesión de trabajo.
    rng<double> *gen_rest_time;
    double rest_time;
	double max_work_time;
	double acummulatedWorkTime;
	bool flagDisponibles;
	
    ~Voluntario ( )
    {
        //free (wse);
        //free(tlc);
    }

    Voluntario ( const string &name,
                 int _habilidad,
                 handle<Observer> *_obs,
                 handle<ClientTlcWse> *_c,
                 int _red_nro,
                 int _id_peer_client,
                 int _flag_escenario,
                 int _peso_respuestas_expertos,
                 bool _flagDisponibles,
                 double _max_work_time
               )
        : process ( name )
    {
        habilidad = _habilidad;
        flag_terminar = 0;
        observer = _obs;
        client = (handle<ClientWSE> *)_c;
        red_nro = _red_nro;
        id_peer_client = _id_peer_client;
        busy = 0;
        flag_descanso = 1;
        flag_escenario = _flag_escenario;
        peso_respuestas_expertos = _peso_respuestas_expertos;
        
        gen_rest_time = new rngUniform( "Uniform_rest", 12, 24);	//Debería ser 12 o 24 hs.
		gen_rest_time->reset();
		flagDisponibles = _flagDisponibles;
		max_work_time = _max_work_time;
        //~ if (habilidad==1){
        //~ cout << "Experto: " << this->name() << endl;
        //~ }
    }

    void inner_body(void);

    bool get_busy(void)
    {
        return busy;
    }

    void add_request(MessageWSE *m)
    {
        queue_in.push_back(m);
    }
    void add_request2(MessageServer *m)
    {
        queue_in2.push_back(m);
    }

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


    void terminar(  )
    {
        flag_terminar = 1;
    }

    void send2client(MessageServer *m);

    void debug(double time, string msg  );

    void vote(list<int> &opciones);
    
    void restControl(double workTime);


};



#endif
