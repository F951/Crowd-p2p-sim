
class MsgBD
{
public:
    int id_tarea, id_img;

    int consenso;

    double tiempo_generacion;

    double tiempo_inicio;

    double tiempo_finalizacion;

    int red_asignada;

    MsgBD (int _id_tarea, int _id_img, int _consenso, double _tiempo_generacion, double _tiempo_inicio, double _tiempo_finalizacion, int _red_asignada)
    {
        id_tarea = _id_tarea;

        id_img	= _id_img;

        consenso  = _consenso;

        tiempo_generacion = _tiempo_generacion;

        tiempo_inicio = _tiempo_inicio;

        tiempo_finalizacion = _tiempo_finalizacion;

        red_asignada = _red_asignada;

    }
};
