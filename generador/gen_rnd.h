#ifndef GEN_H
#define GEN_H

#include "../glob.h"
#include "../auxiliar/Util.h"
#include "../auxiliar/Hash.h"
#include "../Observer.h"
#include "../wse/WSE.h"
#include "../wse/MessageWSE.h"
#include "Mje_Gen_WSE.h"
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

class TlcProtocol;

class Gen_rnd: public process
{

    int *totalQueries, sentQueries, lastStepQueries;
    int NP;
    UTIL *util;
    Hash *h;
    char traces_file[2048];
    rng<double> *arrival_time;
    bool phase; //rafaga when true is a flash crowd, when false its normal time

    vector<double> frecuencias_entrada;
    vector<double> f_acumuladas;

    rng<double> *SelectSource;
    double prev, actual;
    ifstream endStream;
    vector<string> tokens;
    double CTE;
    double *init_time;
    double parametro_TTL;

    handle<Observer> *observer;
    handle<WSE> *wse;

    //ends simulation
    int *ends;
    int Nuser;
    int porcentaje_peers;
    int normal_rate;

protected:
    void inner_body( void );

public:

    void freeGen_rnd()
    {
        //~ Peers.clear();
        delete util;
        delete h;
        delete arrival_time;
        delete SelectSource;
        //delete[] opciones;

        //free(observer);
        //free(wse);
    }

    Gen_rnd ( const string &name, char *_traces_file, int *_totalQueries,
              int _NP,
              handle<Observer> *obs, int *_ends,
              handle<WSE> *_wse,
               int _normal_rate, double _parametro_TTL
            ): process( name )
    {
        totalQueries    = _totalQueries;
        sentQueries     = 0;
        wse             = _wse;
        lastStepQueries = 0;
        NP              = _NP;
        util            = new UTIL();
        h               = new Hash();
        strcpy( traces_file, _traces_file );
        ends            = _ends;
        phase = false;
        normal_rate = _normal_rate;
        double media1 = (1.0 / _normal_rate);
        arrival_time = new rngExp( "Arrive Time", media1 );

        arrival_time->reset();

        SelectSource = new rngUniform("SelectSource", 0, 100);
        SelectSource->reset();

        prev = 0.0;
        actual   = 0.0;
        observer = obs;
        generar_distribucion(frecuencias_entrada, f_acumuladas);
        parametro_TTL = _parametro_TTL;
        //~ cout << "frecuencias_entrada: " << endl;
        //~ for (int i= 0; i< f_acumuladas.size(); i++){
        //~ cout << frecuencias_entrada[i] << ",";
        //~ }
        //~ cout << endl;
        //~ cout << "f_acumuladas: " << endl;
        //~ for (int i= 0; i< f_acumuladas.size(); i++){
        //~ cout << f_acumuladas[i] << ",";
        //~ }
        //~ cout << endl;

    }

    int getZipf(int, int);
    void setQueryRate(int);
    double rand_val(int);
    void debug(double time, string msg  );

    void generar_distribucion(vector<double> &arr_frec, vector<double> &arr_frec_acc);


};
#endif
