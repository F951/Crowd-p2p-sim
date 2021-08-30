#include "BD.h"
#include <cmath>

void BD::inner_body(void)
{
    int MOSTRAR_SALIDA = 0;
    while(1)
    {
        //==========================================================================================================================
        while  ((*wse)->end_WSE)
        {
            debug(time(), "End Server, passivate ");
            passivate();
        }

        if(queue_in2.empty() )
        {
            debug(time(), "Check solved tasks ");
            if ((int)tareas_resueltas2.size() >= 0.9*nQ)
            {
                //~ calcular_timestamps();
                debug(time(), "FINALIZACION EN BD ___________________________________________________________________  tareas_resueltas.size(): "+to_string(tareas_resueltas2.size()) );
                calcular_tiempos_promedio2(tiempo1, tiempo2);
                //~ calcular_tiempos_por_red2();
                (*(*wse)->observer)->Task_processing_time = (*(*wse)->observer)->Task_processing_time / (double)tareas_resueltas2.size();
                (*wse)->perc_consenso = calcular_perc_consenso2();
                (*wse)->finalizacion();
                (*wse)->end_WSE = 1;
                debug(time(), "All tasks received ");
            }
            else
            {
                debug(time(), "Still active tasks ");
            }

            passivate();
        }
        ASSERT(!queue_in2.empty());
        contador_finalizadas++;
        (*(*wse)->observer)->tareas_finalizadas++;
        debug(time(), "tareas_finalizadas: " + to_string((*(*wse)->observer)->tareas_finalizadas));

        MsgBD *m = queue_in2.front();

        if (m->consenso == 1)
        {
            (*(*wse)->observer)->contador_consenso++;
        }
        else
        {
            (*(*wse)->observer)->contador_consenso_negativo++;
        }
        
        (*(*wse)->observer)->Task_service_time += m->tiempo_inicio - m->tiempo_generacion;
        (*(*wse)->observer)->Task_processing_time += m->tiempo_finalizacion - m->tiempo_inicio;
        guardar_tarea2(m);
        (*(*wse)->observer)->tareas_finalizadas_net[m->red_asignada]++;
        //~ asignacion_tarea_red[m->rta->red_asignada].push_back(m->rta->id_tarea);
        queue_in2.pop_front();

        //========================================================================================================================== ANTERIOR
        //while  ((*wse)->end_WSE){
        //	//~ cout<<"Pasivar BD, wse->end_WSE = " << (*wse)->end_WSE <<endl;
        //	passivate();
        //}
        //
        //if(queue_in.empty() ){
        //	//~ cout << "BD PASSIVATE - queue in size(): " << queue_in.size() << ". tareas_resueltas.size(): "<< tareas_resueltas.size() << endl;
        //	if ((int)tareas_resueltas.size()>=0.9*nQ)
        //	{
        //		calcular_timestamps();
        //		if (MOSTRAR_SALIDA) {cout << "FINALIZACION EN BD ___________________________________________________________________  tareas_resueltas.size(): "<<  tareas_resueltas.size() << endl;}
        //		calcular_tiempos_promedio(tiempo1, tiempo2);
        //		calcular_tiempos_por_red();
        //		(*wse)->perc_consenso=calcular_perc_consenso();
        //		(*wse)->finalizacion();
        //		(*wse)->end_WSE = 1;
        //	}
        //	if (MOSTRAR_SALIDA) {cout<<"Pasivar BD, wse->end_WSE = " << (*wse)->end_WSE <<endl;  }
        //	passivate();
        //}
        //ASSERT(!queue_in.empty());
        //if (MOSTRAR_SALIDA) {cout<<"Tarea finalizada recibida en la BD"<<endl;  }
        //contador_finalizadas++;
        //(*(*wse)->observer)->tareas_finalizadas++;
        //debug(time(), "tareas_finalizadas: "+to_string((*(*wse)->observer)->tareas_finalizadas));
        //
        //Mje_WSE_BD* m = queue_in.front();
        //
        //m->rta->TimeStamps[16]+=this->time();		//TS16
        //m->rta->contadores_TimeStamps[16]++;
        //
        //if (m->rta->consenso==1){(*(*wse)->observer)->contador_consenso++;}else{(*(*wse)->observer)->contador_consenso_negativo++;}
        //
        //(*(*wse)->observer)->Task_service_time += m->rta->tiempo_inicio - m->rta->tiempo_generacion;
        //(*(*wse)->observer)->Task_processing_time += m->rta->tiempo_finalizacion - m->rta->tiempo_inicio;
        //
        //guardar_tarea(m->rta);
        //(*(*wse)->observer)->tareas_finalizadas_net[m->rta->red_asignada]++;
        //asignacion_tarea_red[m->rta->red_asignada].push_back(m->rta->id_tarea);
        //queue_in.pop_front();
    }
}

long int BD::randomTTL()
{
    long int r = rand();

    return r;
}

void BD::print_tareas_resueltas()
{
    cout << "======================================================================================================" << endl;
    cout << "Lista de tareas resueltas" << endl;
    cout << "======================================================================================================" << endl;
    for(map<int, Rtas_Peers *>::const_iterator it = tareas_resueltas.begin(); it != tareas_resueltas.end(); ++it)
    {
        cout << "====================Tarea número: " << it->first << "\n";
        cout << "Tarea número: " << it->second->id_tarea << endl;
        cout << "id_img: " << it->second->id_img << endl;
        cout << "tiempo_generacion: " << it->second->tiempo_generacion << endl;
        cout << "tiempo_inicio: " << it->second->tiempo_inicio << endl;
        cout << "tiempo_finalizacion: " << it->second->tiempo_finalizacion << endl;
    }
    cout << "======================================================================================================" << endl;
}

double BD::calcular_perc_consenso()
{
    double consenso_total = 0.0;
    for(map<int, Rtas_Peers *>::const_iterator it = tareas_resueltas.begin(); it != tareas_resueltas.end(); ++it)
    {
        consenso_total += (double)it->second->consenso;
    }
    consenso_total = consenso_total / (double)tareas_resueltas.size();
    return consenso_total;
}


double BD::calcular_perc_consenso2()
{
    double consenso_total = 0.0;
    for(map<int, MsgBD *>::const_iterator it = tareas_resueltas2.begin(); it != tareas_resueltas2.end(); ++it)
    {
        consenso_total += (double)it->second->consenso;
    }
    consenso_total = consenso_total / (double)tareas_resueltas2.size();
    return consenso_total;
}


void BD::calcular_tiempos_promedio(	double &tiempo_servicio_promedio, double &tiempo_procesamiento_promedio )
{
    double tiempo_generacion_acumulado = 0;
    double tiempo_inicio_acumulado = 0;
    double tiempo_finalizacion_acumulado = 0;
    double cantidad_tareas = (double)tareas_resueltas.size();
    for(map<int, Rtas_Peers *>::const_iterator it = tareas_resueltas.begin(); it != tareas_resueltas.end(); ++it)
    {
        tiempo_generacion_acumulado = tiempo_generacion_acumulado + it->second->tiempo_generacion;
        tiempo_inicio_acumulado += it->second->tiempo_inicio;
        tiempo_finalizacion_acumulado += it->second->tiempo_finalizacion;
    }
    tiempo_servicio_promedio = tiempo_inicio_acumulado - tiempo_generacion_acumulado;
    tiempo_procesamiento_promedio = tiempo_finalizacion_acumulado - tiempo_inicio_acumulado;
    tiempo_servicio_promedio = tiempo_servicio_promedio / cantidad_tareas;
    tiempo_procesamiento_promedio = tiempo_procesamiento_promedio / cantidad_tareas;
    double tiempo_procesamiento = 0.0;
    vector<double> v_tiempo_procesamiento;			//Para los valores máximo y mínimo.
    double mean = 0.0;
    double variance = 0.0;
    double stdDeviation = 0.0;
    double maxElement = 0.0;
    double minElement = 0.0;
    for(map<int, Rtas_Peers *>::const_iterator it = tareas_resueltas.begin(); it != tareas_resueltas.end(); ++it)
    {
        mean += ( it->second->tiempo_finalizacion - it->second->tiempo_inicio ) / tareas_resueltas.size();
    }
    //~ cout << "ProcessingT" << endl;
    for(map<int, Rtas_Peers *>::const_iterator it = tareas_resueltas.begin(); it != tareas_resueltas.end(); ++it)
    {
        tiempo_procesamiento = it->second->tiempo_finalizacion - it->second->tiempo_inicio;
        //~ cout << tiempo_procesamiento << endl;
        v_tiempo_procesamiento.push_back(tiempo_procesamiento);
        variance += pow(tiempo_procesamiento - mean, 2);
    }
    variance = variance / tareas_resueltas.size();
    stdDeviation = sqrt(variance);
    maxElement = *std::max_element(v_tiempo_procesamiento.begin(), v_tiempo_procesamiento.end());
    minElement = *std::min_element(v_tiempo_procesamiento.begin(), v_tiempo_procesamiento.end());
    (*(*wse)->observer)->maxElement_DB = maxElement;
    (*(*wse)->observer)->minElement_DB = minElement;
    (*(*wse)->observer)->mean_DB = mean;
    (*(*wse)->observer)->stdDeviation_DB = stdDeviation;
}

void BD::calcular_tiempos_promedio2(	double &tiempo_servicio_promedio, double &tiempo_procesamiento_promedio )
{
    double tiempo_generacion_acumulado = 0;
    double tiempo_inicio_acumulado = 0;
    double tiempo_finalizacion_acumulado = 0;
    double cantidad_tareas = (double)tareas_resueltas2.size();
    for(map<int, MsgBD *>::const_iterator it = tareas_resueltas2.begin(); it != tareas_resueltas2.end(); ++it)
    {
        tiempo_generacion_acumulado += it->second->tiempo_generacion;
        tiempo_inicio_acumulado += it->second->tiempo_inicio;
        tiempo_finalizacion_acumulado += it->second->tiempo_finalizacion;
    }
    tiempo_servicio_promedio = tiempo_inicio_acumulado - tiempo_generacion_acumulado;
    tiempo_procesamiento_promedio = tiempo_finalizacion_acumulado - tiempo_inicio_acumulado;
    tiempo_servicio_promedio = tiempo_servicio_promedio / cantidad_tareas;
    tiempo_procesamiento_promedio = tiempo_procesamiento_promedio / cantidad_tareas;
}

void BD::calcular_tiempos_por_red()
{
    for (int i = 0; i < nro_redes; i++)
    {
        if (asignacion_tarea_red.count(i) != 0)
        {
            double tiempo_procesamiento = 0.0;
            vector<double> v_tiempo_procesamiento;			//Para los valores máximo y mínimo.
            double mean = 0.0;
            double variance = 0.0;
            double stdDeviation = 0.0;
            double maxElement = 0.0;
            double minElement = 0.0;
            //Acceder sólo a las entradas que corresponden a la asignación de red.
            for(map<int, Rtas_Peers *>::const_iterator it = tareas_resueltas.begin(); it != tareas_resueltas.end(); ++it)
            {
                if (it->second->red_asignada == i)
                {
                    mean += ( it->second->tiempo_finalizacion - it->second->tiempo_inicio ) / tareas_resueltas.size();
                }
            }
            for(map<int, Rtas_Peers *>::const_iterator it = tareas_resueltas.begin(); it != tareas_resueltas.end(); ++it)
            {
                if (it->second->red_asignada == i)
                {
                    tiempo_procesamiento = it->second->tiempo_finalizacion - it->second->tiempo_inicio;
                    v_tiempo_procesamiento.push_back(tiempo_procesamiento);
                    variance += pow(tiempo_procesamiento - mean, 2);
                }
            }
            variance = variance / tareas_resueltas.size();
            stdDeviation = sqrt(variance);
            maxElement = *std::max_element(v_tiempo_procesamiento.begin(), v_tiempo_procesamiento.end());
            minElement = *std::min_element(v_tiempo_procesamiento.begin(), v_tiempo_procesamiento.end());

            (*(*wse)->observer)->maxElement_DB_per_net[i] = maxElement;
            (*(*wse)->observer)->minElement_DB_per_net[i] = minElement;
            (*(*wse)->observer)->mean_DB_per_net[i] = mean;
            (*(*wse)->observer)->stdDeviation_DB_per_net[i] = stdDeviation;
        }
        else
        {
            //Asigno valores 0 a todas las métricas.
            (*(*wse)->observer)->maxElement_DB_per_net[i] = 0;
            (*(*wse)->observer)->minElement_DB_per_net[i] = 0;
            (*(*wse)->observer)->mean_DB_per_net[i] = 0;
            (*(*wse)->observer)->stdDeviation_DB_per_net[i] = 0;
        }
    }
}

void BD::calcular_timestamps()
{
    //Cada tarea tiene un vector con los timestamps acumulados, y el vector de enteros para promediarlos (número de mediciones).
    //Debería entonces calcular esos promedios de cada timestamp en cada tarea.

    //Un segundo cálculo es mezclar las mediciones de todas las tareas, en un vector que tiene que estar en el observador.
    //En ese caso hay que normalizar respecto del número de tareas (o idealmente respecto del número de tareas que tuvieron resultados distintos de cero).
    for(map<int, Rtas_Peers *>::const_iterator it = tareas_resueltas.begin(); it != tareas_resueltas.end(); ++it)
    {
        for (int i = 0; i < 17; i++)
        {
            if (it->second->contadores_TimeStamps[i] != 0)
            {
                it->second->TimeStamps[i] = (double)it->second->TimeStamps[i] / (double)it->second->contadores_TimeStamps[i];
            }
            else
            {
                it->second->TimeStamps[i] = 0;
            }

            /*** ACA TIENE QUE IR LA RESTA ENTRE TIMESTAMPS SUCESIVOS ***/
            (*(*wse)->observer)->TimeStamps[i] += (double)it->second->TimeStamps[i] / (double)tareas_resueltas.size();
        }

    }
}

void BD::debug(double time, string msg  )
{
    if (0) cout << "[" << time << "][BD:" << name() << "][ " << msg << " ]" << endl;
}


void BD::guardar_tarea2(MsgBD *m)
{
    //~ MsgBD m1 = *m;
    //~ cout << "m1.taskID" << m1.id_tarea << endl;
    //~ cout << "m1.id_img" << m1.id_img << endl;
    //~ cout << "m1.tiempo_generacion" << m1.tiempo_generacion << endl;
    //~ cout << "m1.tiempo_finalizacion" << m1.tiempo_finalizacion << endl;
    //~ cout << "m1.tiempo_inicio" << m1.tiempo_inicio << endl;
    //~ cout << "m1.red_asignada" << m1.red_asignada<< endl;

    tareas_resueltas2[m->id_tarea] = m;
}
