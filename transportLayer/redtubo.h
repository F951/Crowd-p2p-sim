#ifndef REDTUBO_H
#define REDTUBO_H

#include "../glob.h"
#include "MessageT.h"
#include "NodeT.h"
#include "../Observer.h"

/** Main class of the transport Layer **/

class Observer;

class RedTubo: public process
{


protected:

    list<MessageT *> queue_in;

    /* nodes in the upperLayer */
    vector <handle<NodeT>* > nodes;
    handle<Observer> *observer;

    bool busy;

    /* process function */
    void inner_body( void );
    int flag_terminar;
    int lim_inf;
    int red_nro;
    double cte_traffJam_p2p;
public:

    ~RedTubo()
    {
        nodes.clear();
    }

    RedTubo ( const string &name, int _lim_inf, int _red_nro, double _cte_traffJam_p2p): process( name )
    {
        busy = false;
        flag_terminar = 0;
        lim_inf = _lim_inf;
        red_nro = _red_nro;
        cte_traffJam_p2p = _cte_traffJam_p2p;
    }

    /* adds a Peer in the upperLayer */
    void add_Node( handle<NodeT> *n )
    {
        nodes.push_back(n);
    }

    /* returns the busy state of the redTubo */
    bool get_busy()
    {
        return busy;
    }

    /* adds a MessageT in the queue */
    void add_message( MessageT *m )
    {
        //cout << "Redtubo: inTubo "<< m->getDest() << endl;
        queue_in.push_back( m );
    }

    void print()
    {
        cout << "REDTUBO" << endl;
    }

    void set_handle_obs(handle<Observer> *_obs)
    {
        observer = _obs;
    }

    void terminar(  )
    {
        flag_terminar = 1;
    }
    
    void debug_traffic(double DELAY, double cte_traffJam_p2p  );
    
	void debug(double time, string msg  );
};

#endif
