#include "../wse/Rtas_peers.h"

class Mje_WSE_BD
{
public:
    int id_tarea, id_img;
    double TTL;
    list<int> opciones;
    int prioridad;
    Rtas_Peers *rta;

    Mje_WSE_BD (int _id_tarea, int _id_img, double _TTL, list<int> _op, int _prioridad, Rtas_Peers *_rta)
    {
        id_tarea = _id_tarea;
        id_img	= _id_img;
        TTL	    = _TTL;
        prioridad = _prioridad;
        for (list<int>::iterator it = _op.begin(); it != _op.end(); it++)
        {
            opciones.push_back( *it );
        }
        rta = new Rtas_Peers();
        assert(_rta != NULL);
        //~ rta->copy(*_rta);
        rta->copy(_rta);

    }
};
