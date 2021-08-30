#ifndef Mje_Gen_WSE_H
#define Mje_Gen_WSE_H


class Mje_Gen_WSE
{
public:
    int id_tarea, id_img;
    // list<int> imagen;
    double TTL;
    list<int> opciones;
    int prioridad;
    double tiempo_generacion;
    Mje_Gen_WSE (int _id_tarea, int _id_img, double _TTL, list<int> _op, int _prioridad, double _tiempo_generacion)
    {
        id_tarea = _id_tarea;
        id_img   = _id_img;
        TTL	    = _TTL;
        prioridad = _prioridad;
        for (list<int>::iterator it = _op.begin(); it != _op.end(); it++)
        {
            opciones.push_back( *it );
        }
        tiempo_generacion = _tiempo_generacion;

    }
};
#endif
