#ifndef BD_H
#define BD_H

#include "../glob.h"
#include "Msje_WSE_BD.h"
#include "MsgBD.h"
#include "../wse/WSE.h"
#include "../wse/Task.h"
#include "../wse/MessageServer.h"

//#include "LruA.h"

//#define BDCACHESIZE 8000


class BD: public process
{

private:

    int contador_finalizadas;
    bool busy;

public:
    handle<WSE> *wse;
    list<Mje_WSE_BD *> queue_in;
    map<int, Rtas_Peers *> tareas_resueltas;

    list<MsgBD *> queue_in2;
    map<int, MsgBD *> tareas_resueltas2;
    //~ map<int,int> tareas_resueltas2;
    //~ list<int> tareas_resueltas2;

    map<int, vector<int>> asignacion_tarea_red;
    int  nQ;
    double tiempo1, tiempo2;
    int peers;

    double mean;
    double variance;
    double stdDeviation;
    double maxElement;
    double minElement;
    int nro_redes;

    ~BD()
    {
        //  answers.clear();
        //delete ANSWERS;
    }

    BD ( const string &name, int _nQ, handle<WSE> *_wse,
         int _peers, int _nro_redes): process ( name )
    {
        busy = false;
        nQ = _nQ;
        contador_finalizadas = 0;
        busy = 0;
        wse = _wse;
        tiempo1 = 0;
        tiempo2 = 0;
        peers = _peers;
        mean = 0.0;
        variance = 0.0;
        stdDeviation = 0.0;
        maxElement = 0.0;
        minElement = 0.0;
        nro_redes = _nro_redes;
    }

    void inner_body ( void );

    void add_request ( Mje_WSE_BD *bn )
    {
        queue_in.push_back(bn);
        // if (MOSTRAR_SALIDA) cout << "Mensaje recibido en la Base de Datos" << endl;
    }

    void add_request2 ( MsgBD *bn )
    {
        queue_in2.push_back(bn);
    }

    bool get_busy()
    {
        return busy;
    }

    void print()
    {
        cout << "BD" << endl;
    }

    long int randomTTL();

    void guardar_tarea(Rtas_Peers *rta)
    {
        tareas_resueltas[rta->id_tarea] = rta;
    }

    void print_tareas_resueltas();
    double calcular_perc_consenso();
    void calcular_tiempos_promedio(double &, double &);
    //~ void calcular_tiempos_promedio(	double &tiempo_servicio_promedio,	double &tiempo_procesamiento_promedio, double variance, double stdDeviation, double maxElement, double minElement, double mean);

    int get_mensajes_pendientes()
    {
        return queue_in.size();
    }

    void calcular_tiempos_por_red();

    void calcular_timestamps();

    void calcular_tiempos_promedio2(	double &tiempo_servicio_promedio, double &tiempo_procesamiento_promedio );

    double calcular_perc_consenso2() ;

    void guardar_tarea2(MsgBD *m);

    void debug(double time, string msg  );



};


#endif
