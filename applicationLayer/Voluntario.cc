#include "ClientTLCWSE.h"
#include "TlcProtocol.h"

#include <iostream>
#include <fstream>
#include <ctime>

void Voluntario::inner_body(void)
{
    int debug_voluntario = 1;

    srand (static_cast <unsigned> (this->time()));
    double T_trabajo;

    //Tiempo de trabajo: tiempo que tarda un voluntario en procesar una imagen.
    //work_time = new rngUniform( "Work Time", 0.1,20 );
    //work_time1 = new rngNormal( "Normal", 1.0, 0.1 );			//Base de tiempo en segundos
    work_time1 = new rngNormal( "Normal", 0.000278, 0.0000278 );		//Base de tiempo en horas
    //work_time2 = new rngExp( "NegExp 2", 0.6 );					//Base de tiempo en segundos
    work_time2 = new rngExp( "NegExp 2", 0.000167 );					//Base de tiempo en horas
    work_time1->reset();
    work_time2->reset();
    //double const_work_time=0.4;			//Base de tiempo en segundos
    double const_work_time = 0.000111;			//Base de tiempo en horas
    

    //Límites del tiempo que puede demorar un voluntario en atender una tarea (15 minutos).
    //~ double lower_bound_atencion = 0;		//Base de tiempo en segundos
    //~ double upper_bound_atencion = 900;		//Base de tiempo en segundos
    double lower_bound_atencion = 0;			//Base de tiempo en horas
    double upper_bound_atencion = 0.25;			//Base de tiempo en horas
    rng<double> *generador_tiempo_atencion = new rngUniform( "Uniform", lower_bound_atencion, upper_bound_atencion );

    MessageServer *temp;

	rest_time = 0;
	acummulatedWorkTime=0;
	

    while(1)
    {
        while ( flag_terminar == 1 )
        {
            passivate();
        }

        if(queue_in2.empty())
        {
            passivate();
			
			double tiempo_atencion = generador_tiempo_atencion->value();		//debug(time(), "Hold tiempo de atención: "+to_string(tiempo_atencion));
			busy = true;
			hold(tiempo_atencion);							//El tiempo de atención solo se aplica cuando el usuario se conecta. Si el usuario está conectado procesando mensajes, no hace falta aplicarlo.
			busy = false;
			debug(time(), "Hold atencion OK");
        }
        
        ASSERT(!queue_in2.empty());
        temp = queue_in2.front();
        queue_in2.pop_front();
		debug(time(), "Process message");

        double aux_workTime1 = work_time1->value( );
        if (aux_workTime1 < 0)
        {
            aux_workTime1 = 0;
        }
        double aux_workTime2 = work_time2->value( );
        T_trabajo = const_work_time + aux_workTime1 + aux_workTime2;		//debug(time(), "Hold tiempo de trabajo: "+to_string(T_trabajo));
        assert(T_trabajo >= 0);
        busy = true;
        hold(T_trabajo);
        busy = false;
        debug(time(), "Hold trabajo OK");
        debug(time(), "OPTION NUMBER: "+to_string(temp->getOptionNr()));
        (*observer)->utilizacion[ id_peer_client ] += T_trabajo;
        assert((*observer)->utilizacion[ id_peer_client ] >= 0);

        list<int> opciones(temp->getOptionNr(), -1);
        vote(opciones);
        //~ printLista(opciones,"OPCIONES elegidas",temp->getTaskID());
        temp->set_opciones(opciones);
		debug(time(), "Opciones en el mensaje preparado en el voluntario");
		//~ temp->print_opciones();
		
        temp->set_type(SEND_RESULT);
        debug(time(), "Enviando respuesta hacia peer");
        send2client(temp);

		if (flagDisponibles) restControl(T_trabajo);
    }
}

void Voluntario::printLista(std::list<int> &lista, const char *nombre, int indice)
{
    std::list<int>::iterator it_lista;
    cout << "#" << indice << ". ==============Lista: " << nombre << "\n";
    cout << "#" << indice << ". ";
    for (it_lista = lista.begin(); it_lista != lista.end(); ++it_lista)
    {
        cout << ' ' << *it_lista;
    }
    cout << "\n#" << indice << ". ==============\n";
}

void Voluntario::mostrar_tareas_en_proceso_local()
{
    for(map<int, Rtas_Peers *>::const_iterator it = en_proceso_local.begin(); it != en_proceso_local.end(); ++it)
    {
        cout << it->second->id_tarea << ",";
    }
    cout << "\n";
}


void Voluntario::debug(double time, string msg  )
{
    if (0) cout << "[" << time << "][Voluntario:" << name() << "][ " << msg << " ]" << endl;
}

void Voluntario::send2client(MessageServer *m)
{
    (*client)->receiveWSE2(m);
    if ( (*(client))->idle() && !(*(client))->get_busy() )
    {
        (*(client))->activateAfter( current() );
    }
}

void Voluntario::vote(list<int> &opciones)
{
    float ar;
    //SENTENCIAS PARA MODIFICAR OPCIONES (TRABAJO) SEGÚN FLAG DE HABILIDAD.
    list<int>::iterator it;
    for (it = opciones.begin(); it != opciones.end(); it++)
    {
        if (habilidad == 1)
        {
            ar = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.0);		//~ cout <<"Probabilidad elegida (se compara con 0.5) "<< ar << endl;
            *it = peso_respuestas_expertos * (ar < 0.9);										//Si es experto las respuestas valen más
        }
        else
        {
            ar = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.0);		//~ cout <<"Probabilidad elegida (se compara con 0.5) "<< ar << endl;
            *it = (ar < 0.5);
        }
    }
}

void Voluntario::restControl(double workTime)
{
	//Acumulo tiempo de trabajo
	acummulatedWorkTime += workTime;
	debug(time(), "__________________________________________________________________________________restControl");
	if ( acummulatedWorkTime >= max_work_time ) {
		(*observer)->disconnections++;
		//Enviar mensaje de desconexion al servidor.
		static_cast<ClientTlcWse*>(&**client)->sendDisconnectionMsg(); //;
		//Hacer el hold de desconexion-descanso.
		rest_time=gen_rest_time->value();
		busy=true; hold(rest_time); busy=false;
		//Enviar mensaje de reconexión.
		static_cast<ClientTlcWse*>(&**client)->sendConnectionMsg(); //;
		//Reiniciar el contador de tiempo de trabajo acumulado.
		acummulatedWorkTime=0;
	}
}
