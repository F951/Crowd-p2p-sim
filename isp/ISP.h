#ifndef ISP_H
#define ISP_H

#include "../glob.h"
#include "../Observer.h"
#include "../wse/MessageWSE.h"
#include "../wse/MessageServer.h"
#include "../wse/WSE.h"

class TlcProtocol;
class ClientTlcWse;
class Observer;
class WSE;

class ISP: public process
{

protected:

    list<MessageWSE *> queue_in;
    list<MessageServer *> queue_in2;

    // /* nodes in the upperLayer */
    //vector <handle<NodeT>* > nodes;
    handle<Observer> *observer;

    bool busy;

    /* process function */
    void inner_body( void );
    int flag_terminar;

    //~ vector<handle<TlcProtocol> *> Peers;
    vector<handle<ClientTlcWse> *> clientes_wse_Peers;

public:


    int NP;
    handle<WSE> *wse;
    int red_nro;
    double cte_traffJam_isp;
    int flagArquitectura;

    ~ISP()
    {
        //~ nodes.clear();
        //~ Peers.clear();
        clientes_wse_Peers.clear();
    }


    ISP ( const string &name,  int _NP, int _red_nro, double _cte_traffJam_isp, int _flagArquitectura): process( name )
    {
        busy = false;
        NP  = _NP;
        flag_terminar = 0;
        red_nro = _red_nro;
        cte_traffJam_isp = _cte_traffJam_isp;
        flagArquitectura = _flagArquitectura;
    }

    //~ void add_peer(handle<TlcProtocol> * p) 	{
    //~ Peers.push_back(p);
    //~ }

    void add_peer(handle<ClientTlcWse> *p)
    {
        clientes_wse_Peers.push_back(p);
    }

    bool get_busy()
    {
        return busy;
    }

    void add_message( MessageWSE *m )
    {
        queue_in.push_back( m );
    }

    void add_message2( MessageServer *m )
    {
        queue_in2.push_back( m );
    }

    void print()
    {
        cout << "ISP" << endl;
    }

    void set_handle_obs(handle<Observer> *_obs)
    {
        observer = _obs;
    }

    void printMSG(MessageWSE *m);

    void debug(double time, string msg);

    void terminar(  )
    {
        flag_terminar = 1;
    }

    void deliver2Server(MessageServer *m );
    
    void deliver2Peer(MessageServer *m );
    
    void debug_traffic(double DELAY, double cte_traffJam_isp  );

};



#endif
