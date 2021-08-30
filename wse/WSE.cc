#include "WSE.h"
#include "../BD/BD.h"
#include "../p2pLayer/NodeEntry.h"
#include "../applicationLayer/TlcProtocol.h"
#include "../applicationLayer/ClientTLCWSE.h"
#include "../isp/ISP.h"
#include "../applicationLayer/Query.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//#include <random>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <algorithm>
#include <random>
#include <chrono>
void WSE::inner_body(void)
{
    int debug_wse = 1;
    int debug_DL = 0;

    MessageServer *mS;

    if (flagArquitectura == 3) 						//Arquitectura 3 - Kobayashi
    {
        LATENCIA_SERVER_FLIGHT_PATH = 1 / 3600;	//Para que sea en horas.		//Trabajo de Kobayashi
    }
    else
    {
        LATENCIA_SERVER_FLIGHT_PATH = 0;
    }

    en_proceso.clear();
    std::stringstream str;
    string ptr;
    BIGNUM *hashValue ;
    const char *buffer;
    int indice;

    Mje_Gen_WSE *mG;
    MessageWSE *m;
    Rtas_Peers *rta;

    vector<int> longitud_en_proceso;
    vector<int> longitud_queue_in;
    vector<double> tiempos;

    double lower_bound = parametro_TTL * 0.95; 		//TTL - Para crear el random flotante (double), uso un generador de libcppsim.
    double upper_bound = parametro_TTL * 1.05;
    rng<double> *generador_double = new rngUniform( "Uniform", lower_bound, upper_bound );

    int contador_miss_en_proceso = 0;

    list<int>::iterator it_peers_consultados;
    (*observer)->T_inicio = this->time();
    int responsable_agreg_asignado = 0;
    

    //Vectores de estadísticas para calcular los puntajes de los voluntarios.

    int flag_escenario = 1;

    if (flagArquitectura == 2) 			//En este caso enviamos los modelos a algunos peers.
    {
        if (0)
        {
            cout << "(WSE): Enviar el modelo de DL a los peers automáticos. " << endl;
        }
		int red_asignada = rand() % nro_redes;				// Elijo red
        select_peers_DL(red_asignada, voluntarios_DL);
        send_model(red_asignada, voluntarios_DL);
        if (0)
        {
            cout << "(WSE): Modelo DL enviado con exito. Time: " << this->time() << endl;
        }
        //~ print_vec(voluntarios_DL,"Voluntarios a los que se envía el modelo",0);
    }


    activeTasks.clear();

    assert(activeTasks.size() == 0);

    while(1)
    {

        //--------------------------------------------------------------------------------------------------VERSION ACTUAL
        while (end_WSE == 1 ) 				//Final de la simulación
        {
            debug(time(), "(WSE): END - Passivate ");
            passivate();
        }

        if(queue_in2.empty() && queue_in_Gen2.empty() )
        {
            debug(time(), "(WSE): Passivate ");
            if (activeTasks.size() == 0)
            {
                if (gen_stop == 0)
                {
                    passivate();
                }
                else
                {
                    //~ finalizacion();
                }
            }
            passivate();
        }
        
        //~ debug(time(), "activate");
        
        if (!queue_in_Gen2.empty() )
        {
            debug(time(), "(WSE): process queue2gen ");
            ASSERT(!queue_in_Gen2.empty());
            mS =  queue_in_Gen2.front();		// Leo el mensaje
            queue_in_Gen2.pop_front();		// Saco el mensaje de la cola
            //~ mS->print();
            
            int red_asignada = rand() % nro_redes;
            insert_task(mS, time(), red_asignada	);			
			
			if (flagArquitectura == 1 && !isRelevant())		// "&&" = short circuiting
			{
				debug(time(), "Arch1. Task "+to_string(mS->getTaskID())+ " processed automatically by the server - Irrelevant -> send to BD");
				endTask(mS->getTaskID(),1);
			}else{
				//Enviar el mensaje a la red.
				send_message2(mS->getTaskID());
			}
            delete mS;
        }

        if (!queue_in2.empty() )
        {
            ASSERT(!queue_in2.empty());
            mS =  queue_in2.front();		// Leo el mensaje
            queue_in2.pop_front();		// Saco el mensaje de la cola

            debug(time(), "Message from " + to_string(mS->getSrc()));
            //~ showActiveTasks();
            //~ print_vec(tareas_DL,"Tareas con DL", 0);
            //~ mS->print();
            
            if (flagDisponibles) restControl(mS);
            
			switch (caso){
				case 0:
				{
					baselineAlgorithm(mS);
					break;
				}
				
				case 1:
				{
					proposal1Algorithm(mS);
					break;
				}
				
				case 2:
				{
					proposal2Algorithm(mS);
					break;
				}
			}
        }
    }
}


long int WSE::randomTTL()
{
    long int r = rand();

    return r;
}


void WSE::encontrar_maximos(vector <double> &ranking_voluntarios, vector <int> &peers_seleccionados_reenvio, int H, int red_asignada)
{
    //~ cout << "Encontrar máximos en WSE" << endl;
    //~ print_vec_double(ranking_voluntarios,"Ranking");

    vector<double> v;
    for (int j = lim_inf[red_asignada]; j < lim_sup[red_asignada]; j++) 		//Modificación para múltiples redes.
    {
        v.push_back(ranking_voluntarios[j]);
    }
    //~ cout << "Encontrar máximos de ranking para la red: "<< red_asignada << endl;
    //~ print_vec_double(v,"ranking en encontrar maximos");
    cout.precision(4);
    for (int i = 0; i < H; i++)
    {
        int maxElementIndex = std::max_element(v.begin(), v.end()) - v.begin();
        //~ double maxElement = *std::max_element(v.begin(), v.end());
        //cout << "maxElementIndex:" << maxElementIndex << ", maxElement:" << maxElement << '\n';
        if (std::isinf(v[maxElementIndex]))
        {
            v[maxElementIndex] = -1;
            i = i - 1;
        }
        else
        {
            peers_seleccionados_reenvio.push_back(maxElementIndex + lim_inf[red_asignada]);
        }
        v[maxElementIndex] = -1;
    }
    //~ cout << "Fin Encontrar máximos en WSE" << endl;
    //~ print_vec_double(ranking_voluntarios,"Ranking");
    //~ for (int i=0; i<(int)ranking_voluntarios.size();i++){
    //~ cout << i<< ": " << ranking_voluntarios[i] << endl;
    //~ assert(ranking_voluntarios[i]>0.0);
    //~ }
    //~ cout << "Salgo de Encontrar máximos en WSE" << endl;
}


void WSE::encontrar_maximos_disponibles(vector <double> &ranking_voluntarios, vector <int> &peers_seleccionados_reenvio, int H, int red_asignada)
{
    //~ cout << "Encontrar máximos en WSE" << endl;
    //~ print_vec_double(ranking_voluntarios,"Ranking");

    vector<double> v;
    vector<int> indices_aux;
    for (int j = lim_inf[red_asignada]; j < lim_sup[red_asignada]; j++) 		//Modificación para múltiples redes.
    {
        //~ cout << " "<< j;
        if (peers_disponibles[red_asignada].count(j) != 0)
        {
            v.push_back(ranking_voluntarios[j]);
            indices_aux.push_back(j);									//Esto logra amortiguar la diferencia entre los peers disponibles y no disponibles. Se usa para mapear y recuperar el ID de peer correcto.
        }
    }
    //~ cout << endl;

    //~ cout << "Encontrar máximos de ranking para la red: "<< red_asignada << endl;
    //~ print_vec(indices_aux,"Pre-selección de peers",0);
    cout.precision(4);
    for (int i = 0; i < H; i++)
    {
        int maxElementIndex = std::max_element(v.begin(), v.end()) - v.begin();
        //~ double maxElement = *std::max_element(v.begin(), v.end());
        //~ cout << "maxElementIndex:" << maxElementIndex << ", maxElement:" << maxElement << '\n';
        if (std::isinf(v[maxElementIndex]))
        {
            v[maxElementIndex] = -1;
            i = i - 1;
        }
        else
        {
            //~ cout << ' ' << indices_aux[maxElementIndex];
            peers_seleccionados_reenvio.push_back(indices_aux[maxElementIndex]);
        }
        v[maxElementIndex] = -1;
        //~ cout << endl;
    }

    //~ cout << "Fin Encontrar máximos en WSE" << endl;
    //~ print_vec_double(ranking_voluntarios,"Ranking");
    //~ for (int i=0; i<(int)ranking_voluntarios.size();i++){
    //~ cout << i<< ": " << ranking_voluntarios[i] << endl;
    //~ assert(ranking_voluntarios[i]>0.0);
    //~ }
    //~ cout << "Salgo de Encontrar máximos en WSE" << endl;

}


void WSE::encontrar_maximos_NO_disponibles(vector <double> &ranking_voluntarios, vector <int> &peers_seleccionados_reenvio, int H, int red_asignada)
{
    vector<double> v;
    vector<int> indices_aux;
    //~ cout << "Red Asignada: "<< red_asignada<< endl;
    for (int j = lim_inf[red_asignada]; j < lim_sup[red_asignada]; j++) 		//Modificación para múltiples redes.
    {
        //~ cout << " "<< j;
        if (peers_disponibles[red_asignada].count(j) == 0)
        {
            v.push_back(ranking_voluntarios[j]);
            indices_aux.push_back(j);									//Esto logra amortiguar la diferencia entre los peers disponibles y no disponibles. Se usa para mapear y recuperar el ID de peer correcto.

        }
    }
    //~ cout << endl;
    //~ cout << "Encontrar máximos de ranking para la red: "<< red_asignada << endl;
    //~ print_vec(indices_aux,"Pre-selección de peers",0);
    cout.precision(4);
    for (int i = 0; i < H; i++)
    {
        int maxElementIndex = std::max_element(v.begin(), v.end()) - v.begin();
        //~ double maxElement = *std::max_element(v.begin(), v.end());
        //~ cout << "maxElementIndex:" << maxElementIndex << ", maxElement:" << maxElement << '\n';
        if (std::isinf(v[maxElementIndex]))
        {
            v[maxElementIndex] = -1;
            i = i - 1;
        }
        else
        {
            //~ cout << ' ' << indices_aux[maxElementIndex];
            peers_seleccionados_reenvio.push_back(indices_aux[maxElementIndex]);
        }
        v[maxElementIndex] = -1;
        //~ cout << endl;
    }
}


void WSE::encontrar_minimos(vector <double> &ranking_voluntarios, vector <int> &peers_seleccionados_reenvio, int H, int red_asignada)
{
    //Esta función se usa para encontrar los peers con menor utilización, y así seleccionar los responsables
    //de realizar la agregación de tareas en la red (CASO 2 del simulador).
    std::vector<double> v;
    for (int j = lim_inf[red_asignada]; j < lim_sup[red_asignada]; j++) 		//Modificación para múltiples redes.
    {
        v.push_back(ranking_voluntarios[j]);
    }
    //~ cout << "Encontrar mínimos de ranking para la red: "<< red_asignada << endl;
    //~ print_vec_double(v,"ranking en encontrar mínimos");
    cout.precision(4);
    for (int i = 0; i < H; i++)
    {
        int minElementIndex = std::min_element(v.begin(), v.end()) - v.begin();
        //~ double minElement = *std::min_element(v.begin(), v.end());
        //~ cout << "minElementIndex:" << minElementIndex << ", minElement:" << minElement << '\n';
        if (std::isinf(v[minElementIndex]))
        {
            v[minElementIndex] = 1000000;
            i = i - 1;
        }
        else
        {
            peers_seleccionados_reenvio.push_back(minElementIndex + lim_inf[red_asignada]);
            //~ cout << "peers_seleccionados_reenvio[" << i << "]="<< peers_seleccionados_reenvio[i] << endl;
        }
        //~ peers_seleccionados_reenvio.push_back(minElementIndex+lim_inf[red_asignada]);
        v[minElementIndex] = 1000000;
    }
    //~ print_vec_double(v,"ranking en encontrar mínimos");
}



void WSE::print_vec(vector<int> &vector_in, const char *nombre, int indice)
{
    //cout.open ("cout.txt",ios_base::app);
    cout << "#" << indice << ". Vector: " << nombre << endl;
    cout << "#" << indice << ".";
    for (int k = 0; k < (int)(vector_in.size()); k++)
    {
        cout << " " << vector_in[k];
    }
    cout << "\n";
}


void WSE::print_vec_double(vector<double> &vector_in, const char *nombre)
{
    cout << "Vector: " << nombre << endl;
    cout.precision(4);
    for (int k = 0; k < (int)(vector_in.size()); k++)
    {
        cout << " " << vector_in[k];
    }
    cout << "\n";
}


void WSE::finalizacion()
{
    debug(time(), "SERVER END ===============================================================================================================");
    handle<ClientTlcWse> *puntero_cliente;
    //~ print_vec_double(ranking_voluntarios,"Ranking voluntarios");
    for (int i = 0; i < (int)ranking_voluntarios.size(); i++)
    {
        puntero_cliente = clientes_wse_Peers[i];
        //~ cout <<  "i: "<<  i << ". ranking_voluntarios[i] "<<  ranking_voluntarios[i] << ". Habilidad:" << (*puntero_cliente)->habilidad << endl ;
        //~ cout <<  "porc_utilizacion_voluntarios[i] "<<  porc_utilizacion_voluntarios[i]<< endl ;
    }
    ends[total_NP] = 1;
    end_WSE = 1;
    //~ cout << "DETECCION DE PARADA. queue_in.size(): " << queue_in.size() << endl;
    if (cacheSize != 0)
    {
        for (int i = 0; i < total_NP; i++)
        {
            puntero_cliente = clientes_wse_Peers[i];
            (*puntero_cliente)->terminar();
        }
        /*****Terminar red de transporte**********/
        //La termina el nodo pastry número 0
    }
    /**************Terminar ISP****************/
    for (int j = 0; j < nro_redes; j++)
    {
        (*isp[j])->terminar();
    }

    (*observer)->tiempo_tareas_1 = (*bd)->tiempo1;
    (*observer)->tiempo_tareas_2 = (*bd)->tiempo2;
    (*observer)->ends_obs[total_NP] = 1;
    (*observer)->T_finalizacion = this->time();
    //~ passivate();
}


void WSE::control_TTLs()
{
    if (0)
    {
		cout << "control de TTLs" << endl;
        cout << "this->time(): " << this->time() << " - control_TTLs_anterior: " << control_TTLs_anterior << " - parametro_TTL: " << parametro_TTL << endl;
    }
    if ( (this->time() - control_TTLs_anterior) > 0.5*parametro_TTL )
    {
        //~ revisar_vencimientos();
        checkTaskTimeOut();
        control_TTLs_anterior = this->time();
    }
}


void WSE::checkTaskTimeOut()
{
    double multiplicador_TTL2 = 1;
    vector<int> indices;
    for(map<int, Task *>::const_iterator it = activeTasks.begin(); it != activeTasks.end(); ++it)
    {
        double _tiempo_inicio = it->second->getInitT();
        //~ if ( (this->time() - _tiempo_inicio ) > multiplicador_TTL2 * it->second->getTTL() )
        if ( (this->time() - _tiempo_inicio ) > it->second->getTTL() )
        {
            //if ( (this->time() - _tiempo_inicio ) > 1*it->second->TTL_tarea ){
            indices.push_back(it->second->getTaskID());
        }
    }
    for (int i = 0; i < (int)indices.size(); i++)
    {
        endTask(indices[i], activeTasks[indices[i]]->getConsenso());
    }
    indices.clear();
}

int WSE::seleccion_peer_agregacion(vector<double> &porc_utilizacion_voluntarios, int red_asignada)
{
    int elegido = 0;
    vector<int> top_20;
    encontrar_minimos(porc_utilizacion_voluntarios, top_20, 20, red_asignada);	//Encontrar 20 peers con utilización mínima
    elegido = select_random(top_20) + lim_inf[red_asignada];
    return elegido;
}


int WSE::select_random(vector<int> &top_20)
{
    //Random Seed-------------------------------------------------
    int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    //------------------------------------------------------------
    std::uniform_int_distribution<int> distribution(0, 19);
    int number = distribution(generator);
    //~ cout << "Random int: " << number << endl;
    return number;
}


int WSE::random_task_filter_server(double prob)
{
    int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double rand =distribution(generator);
    debug(time(), "rand: "+to_string(rand)+", prob: "+to_string(prob));
    int filter = ( rand <= prob);
    return filter;
}

void WSE::mostrar_redes()
{
    cout << "***********************************************************************************" << endl;
    cout << "nro redes WSE: " << nro_redes << endl;
    for (int j = 0; j < nro_redes; j++)
    {
        cout << "red " << j << ": lim_inf: " << lim_inf[j] << ". lim_sup: " << lim_sup[j] << endl;
    }
    cout << "***********************************************************************************" << endl;
}


void WSE::mostrar_peers_disponibles(int id_tarea)
{
    for (int i = 0; i < (int)peers_disponibles.size(); i++)
    {
        cout << "Tarea - " << id_tarea << ". Count Peer disponibles red " << i << ": " << peers_disponibles[i].size() << "; ";
    }
    cout << endl;
}


void WSE::incremento_flags(list<int> &lista, vector <int> &opciones_totales_voluntarios, int nro_opciones)
{
    std::list<int>::iterator it_lista;
    int chosen;
    //~ cout<< "==============Lista de peers consultados en función de incremento: " << "\n";
    for (it_lista = lista.begin(); it_lista != lista.end(); ++it_lista)
    {
        chosen = *it_lista;
        ends[chosen]++;
        (*observer)->ends_obs[chosen]++;
        for (int k = 0; k < (int)(nro_opciones); k++)
        {
            opciones_totales_voluntarios[chosen] += 1;
        }
        //~ cout << ' ' << *it_lista;
    }
    //~ cout << "\n==============\n";
}


void WSE::update_timestamp(int id_tarea, int TimeStamp, double this_time)
{
    //~ cout << "UPDATE TIMESTAMP" << endl;
    if (en_proceso.count(id_tarea) != 0)
    {
        //~ en_proceso[id_tarea]->TimeStamps[TimeStamp]+=this_time;
        //~ en_proceso[id_tarea]->contadores_TimeStamps[TimeStamp]++;
        en_proceso[id_tarea]->TimeStamps[TimeStamp - 1] += this_time;
        en_proceso[id_tarea]->contadores_TimeStamps[TimeStamp - 1]++;
    }
}


void WSE::select_peers_DL(int red_asignada, vector <int> &voluntarios)
{
    int chosen;
    int debug_DL = 0;
    double porcentaje_peers_gpu = 0.2;
    if (debug_DL)
    {
        cout << "(WSE): Entro a send model " << endl;
    }
    //Armar una lista de peers automáticos.
    for (int jj = 0; jj < (int)(NP[red_asignada] * porcentaje_peers_gpu); jj++)
    {
        int repeticion = 1;
        if (voluntarios.size() != 0)
        {
            while (repeticion == 1)
            {
                repeticion = 0;
                chosen = lim_inf[red_asignada] + rand() % NP[red_asignada];
                assert(chosen != total_NP);
                //~ cout << "Chosen: " << chosen << endl;
                for (int i = 0; i < (int)voluntarios.size(); i++)
                {
                    if (voluntarios[i] == chosen)
                    {
                        repeticion = 1;
                    }
                }
            }
        }
        else
        {
            chosen = lim_inf[red_asignada] + rand() % NP[red_asignada];
            //~ cout << "Chosen: " << chosen << endl;
        }
        voluntarios.push_back(chosen);
    }
    //~ if (1) {print_vec(voluntarios,"voluntarios para enviar el modelo",0);}
}

//~ void WSE::select_random_peer_DL(Mje_Gen_WSE *mG, vector <int> &voluntarios)
int WSE::select_random_peer_DL(vector <int> &voluntarios_DL)
{
    int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::uniform_int_distribution<int> distribution(0, (int)(voluntarios_DL.size() - 1));
    int chosen = distribution(generator);
    if (0)
    {
        cout << "Cantidad de Peers DL disponibles: " << voluntarios_DL.size() << ". Voluntario DL elegido para realizar la tarea: " << voluntarios_DL[chosen] << endl;
    }
    return voluntarios_DL[chosen];
}

void WSE::send_model(int red_asignada, vector <int> &voluntarios)
{
	int debug_wse = 0;
    int chosen;
    MessageServer *mS;

    if (flagArquitectura != 2)
    {
        cout << "ERROR ARQUITECTURA" << endl;
    }
    assert(flagArquitectura == 2);
    
	double DELAY = 4.51 / 3600.0;		//En 4,91 segundos se envían los 26 MB del modelo, a 5,3 MBps
	
    for (int jj = 0; jj < (int)(voluntarios.size()); jj++)
    {
        chosen = voluntarios[jj];
        if (0)
        {
            cout << " Chosen = " << chosen << endl;
        }
		mS = new MessageServer(SEND_DL_MODEL, 0, 0,	0, serverID, chosen, 0, 0);
		busy = true;
		hold(DELAY);	//Delay TENIENDO EN CUENTA LAS ESTADISTICAS DE LATENCIA DE RED
		busy = false;	
        (*isp[red_asignada])->add_message2(mS);
        if( (*isp[red_asignada])->idle() && !(*isp[red_asignada])->get_busy() )
        {
            (*isp[red_asignada])->activateAfter( current() );
        }
		if (0)
        {
            cout << " Msje enviado a = " << chosen << endl;
        }
    }
}


void WSE::insert_task(MessageServer *mS, double time, int _red_asignada)
{
    debug(time, "Server -----------------> Insert task: " + to_string(mS->getTaskID()) + ". activeTasks.size(): " + to_string(activeTasks.size()) + ". queue_in_Gen2.size(): " + to_string(queue_in_Gen2.size()) );
    debug(time, "Server -----------------> Insert task. activeTasks.size(): " + to_string(activeTasks.size()));
    //~ showActiveTasks();
    Task *task1 = new Task(mS->getTaskID(), mS->getImgID(), mS->getTTL(), mS->getOptionNr(), mS->getGenT(), time, _red_asignada);	//, mS->getGenT());
    //~ if (mS->getTaskID() != 0)
    //~ {
        //~ assert(activeTasks.count(mS->getTaskID()) == 0);
    //~ }
    activeTasks[mS->getTaskID()] = task1;						// Inserto la tarea en en_proceso.
    
    if (caso==2){
		int indice = mS->getTaskID();
		int responsable_agreg_asignado;
		int red_asignada=activeTasks[indice]->getRed();
		if (contador_tareas_enviadas <20 ){		//Al principio, se elige al azar. Luego con el método.
			responsable_agreg_asignado = lim_inf[red_asignada] + rand()%NP[red_asignada];
		}else{
			responsable_agreg_asignado = seleccion_peer_agregacion(porc_utilizacion_voluntarios, red_asignada);
		}
		assert(responsable_agreg_asignado>=lim_inf[red_asignada] && responsable_agreg_asignado<lim_sup[red_asignada]);
		NodeEntry *Responsable_agreg=new NodeEntry((*observer)->codigos2[responsable_agreg_asignado],responsable_agreg_asignado);
		aggregationPeers[mS->getTaskID()]=Responsable_agreg;
	}
    
}

void WSE::send_message2(int taskID)
{
    //~ debug(time(), "Server - Send message");
    int chosen;
    MessageServer *mS;
    Task *task1 = activeTasks[taskID];
    
    list<int> peers_consultados;
	//~ if (flagArquitectura==2 && flag_DL_confirmados && random_task_filter_server(0.2) ){
		//~ debug(time(), "Send task "+to_string(taskID)+ " to DL peer #################################################################################");
		//~ tareas_DL.push_back(taskID);
		//~ peers_consultados.push_back(select_random_peer_DL(voluntarios_DL_confirmados));
		//~ print_vec(tareas_DL,"Tareas con DL", 0);
	//~ }else{
	debug(time(), "Send task "+to_string(taskID)+ " to regular peers");
	peers_consultados = select_volunteers2(taskID, 0);
	//~ }
			
    std::list<int>::iterator it_lista;
    for (it_lista = peers_consultados.begin(); it_lista != peers_consultados.end(); ++it_lista)
    {
        chosen = *it_lista;
        //Envío todo al Peer, menos la imagen (id_img)
        mS = new MessageServer(SEND_TASK, task1->getTaskID(), task1->getImgID(),
                               task1->getTTL(), serverID, chosen, task1->getOptionNr(), 0);
        busy = true;
        hold(TIME_GEN2);
        busy = false;
        if (contador_historia != 0){
			mS->set_FirstMessage(0);
		}
        (*observer)->Time_tagGen_wse += TIME_GEN2;
        (*observer)->N_tagGen_wse++;
        (*isp[task1->getRed()])->add_message2(mS);
        if( (*isp[task1->getRed()])->idle() && !(*isp[task1->getRed()])->get_busy() )
        {
            (*isp[task1->getRed()])->activateAfter( current() );
        }
        debug(time(), "Server, message SENT to: " + to_string(chosen));
        enviadas++;
        (*observer)->contador_enviadas = enviadas;
    }
    
        
    if (flagHistorias == 1) 		
        contador_historia++;
    
    if (contador_historia == max_lenght_historia)
        contador_historia = 0;
    
}

void WSE::sendObject2Peer(MessageServer *mS)
{
    Task *task = activeTasks[mS->getTaskID()];
    mS->set_type(SEND_OBJ_OWNER);
    mS->set_Dest(mS->getSrc());
    mS->set_Src(serverID);
    mS->set_optionNumber(task->getOptionNr());
    mS->set_TTL(task->getTTL());    
    debug(time(), "Server, SEND OBJECT. OptionNR: " + to_string(mS->getOptionNr()));
    //~ m->rta->flag_primer_mensaje=1;
    busy = true;
    hold(LATENCIA_BD_IMG);
    busy = false;
    t_com_wse_bd += LATENCIA_BD_IMG;
    (*isp[task->getRed()])->add_message2(mS);
    if( (*isp[task->getRed()])->idle() && !(*isp[task->getRed()])->get_busy() )
    {
        (*isp[task->getRed()])->activateAfter( current() );
    }
    (*observer)->contador_responsable_env[task->getRed()]++;
    (*observer)->Time_tagPeer_wse[task->getRed()] += this->time();
    (*observer)->N_tagPeer_wse[task->getRed()]++;
}

void WSE::sendObject2PeerProp2(MessageServer *mS)
{
    Task *task = activeTasks[mS->getTaskID()];
    //~ int red_asignada = task->getRed();
    mS->set_type(SEND_OBJ_OWNER);
    mS->set_Dest(mS->getSrc());
    mS->set_Src(serverID);
    mS->set_optionNumber(task->getOptionNr());
    mS->set_TTL(task->getTTL());
    debug(time(), "Server, SEND OBJECT. OptionNR: " + to_string(mS->getOptionNr()));
    //~ m->rta->flag_primer_mensaje=1;

    busy = true;
    hold(LATENCIA_BD_IMG);
    busy = false;
    t_com_wse_bd += LATENCIA_BD_IMG;
    (*isp[task->getRed()])->add_message2(mS);
    if( (*isp[task->getRed()])->idle() && !(*isp[task->getRed()])->get_busy() )
    {
        (*isp[task->getRed()])->activateAfter( current() );
    }
    (*observer)->contador_responsable_env[task->getRed()]++;
    (*observer)->Time_tagPeer_wse[task->getRed()] += this->time();
    (*observer)->N_tagPeer_wse[task->getRed()]++;
}



list<int>  WSE::select_volunteers2(int indice, int contador_historia)
{
    Task *task1 = activeTasks[indice];
    int red_asignada = task1->getRed();
	int chosen = 0;
    list<int> peers_consultados;
	list<int>::iterator it_peers_consultados;
    if (!flagDisponibles){
		//Sin desconexión de voluntarios:
		chosen =  lim_inf[red_asignada] + rand() % NP[red_asignada];
		peers_consultados.push_back(chosen);
		for (int i = 1; i < H; i++)
		{
			int repeticion = 1;
			while (repeticion == 1)
			{
				repeticion = 0;
				chosen =  lim_inf[red_asignada] + rand() % NP[red_asignada];
				for (it_peers_consultados = peers_consultados.begin(); it_peers_consultados != peers_consultados.end(); it_peers_consultados++)
				{
					if (*it_peers_consultados == chosen)
					{
						repeticion = 1;
					}
				}
			}
			peers_consultados.push_back(chosen);
		}
		incremento_flags(peers_consultados, opciones_totales_voluntarios, task1->getOptionNr());
	}else{
		//Con desconexión de voluntarios por descanso:
		Task *task1 = activeTasks[indice];
		int red_asignada = task1->getRed();
		if (contador_historia==0) {
			if ((int)peers_disponibles[red_asignada].size() < H) {
				add_All_AvailablePeers(peers_consultados,red_asignada);		//Peers disponibles es de la clase, H también.
				add_N_UnavailablePeers(peers_consultados,red_asignada, H - (int)peers_disponibles[red_asignada].size());		//Peers disponibles es de la clase, H también.
			}else{	
				add_N_AvailablePeers(peers_consultados,red_asignada, H);			//Peers disponibles es de la clase, H también.
			}
		}else{
			//Copio los peers consultados de la tarea anterior, cabecera de la historia
			peers_consultados=activeTasks[indice-1]->getPeersConsultados();
			//~ activeTasks[indice]->setPeersConsultados();
		}
		activeTasks[indice]->setPeersConsultados(peers_consultados);
		incremento_flags(peers_consultados, opciones_totales_voluntarios, task1->getOptionNr());
	}
	return 	peers_consultados;
}

void WSE::showActiveTasks()
{
    cout << "# Active Tasks: " << activeTasks.size() << endl;
    for(map<int, Task *>::const_iterator it = activeTasks.begin(); it != activeTasks.end(); ++it)
    {
        cout << it->second->getTaskID() << ",";
    }
    cout << "\n";
}

void WSE::endTask(int indice, int consenso)
{
    int multiplicador_TTL2 = 3;
    Task *task = activeTasks[indice];
    if (consenso == 1 || consenso == 2)
    {
		if (consenso==2){(*observer)->DL_tasks++;}
        task->set_Consenso(1);
        //if ((this->time() - (task->getInitT()) ) >= multiplicador_TTL2 * parametro_TTL )
        if (!validTTL(indice) )
        {
            (*observer)->contador_recepcion5[task->getRed()]++;
        }
    }
    else
    {
        if ( task->getReenviada() == 1 )
        {
            (*observer)->contador_recepcion6[task->getRed()]++;
        }
    }

    //**********************************
    //UPDATE FLIGHT PATH (KOBAYASHI - 3)
    //**********************************
    int probabilidad_update;
    if (flagArquitectura==3){
		probabilidad_update=random_task_filter_server(0.2);
		if (probabilidad_update){
			busy=true; hold(LATENCIA_SERVER_FLIGHT_PATH);busy=false;
		}
    }
	
	//~ cout << "task: " << indice << " - Final_T: " << this->time() << endl;
    
    task->set_FinalT(this->time());

    //Crear mensaje para BD.
    MsgBD *mBD;
    debug(time(), "MsgBD ready ");
    mBD = new MsgBD( task->getTaskID(), task->getImgID(), task->getConsenso(), task->getGenT(), task->getInitT(), task->getFinalT(),	task->getRed());

    //Enviar mensaje a BD.
    busy = true;
    hold(LATENCIA_BD_TASK);
    busy = false;
    t_com_wse_bd += (LATENCIA_BD_TASK);

    (*bd)->add_request2(mBD);
    if ((*bd)->idle() && !(*bd)->get_busy())
    {
        (*bd)->activateAfter(current() );
    }
    debug(time(), "MsgBD sent ");
    //Borrar tarea
    activeTasks.erase(indice);
    debug(time(), "Task " + to_string(indice) + " removed");
}

bool WSE::validTTL(int indice)
{
    bool valid;
    if ( ((this->time() - (activeTasks[indice]->getInitT()) ) <= activeTasks[indice]->getTTL() ))
    {
        valid = true;
    }
    else
    {
        valid = false;
    }
    return valid;
}

bool WSE::allAnswers(int indice)
{
    bool allAnswersHere;
    debug(time(), "allAnswers?  task->getRecibidas=" + to_string(activeTasks[indice]->getRecibidas()) + ", H=" + to_string(H));
    //if ( activeTasks[indice]->getRecibidas() >= H)
    if ( activeTasks[indice]->getRecibidas() == H || activeTasks[indice]->getRecibidas() == 2*H)	//Para unificar todos los proposals.
    {
        allAnswersHere = true;
    }
    else
    {
        allAnswersHere = false;
    }
    return allAnswersHere;
}


void WSE::calculateConsensus(int indice)
{
    debug(time(), "Calculate consensus ");
    activeTasks[indice]->calculateConsensus(umbral_votacion, H);
    debug(time(), "Task " + to_string(indice) + ", Consensus " + to_string(activeTasks[indice]->getConsenso()));
    //for (int i=0; i<(int)(activeTasks[indice])->votacion.size();i++)  {
    //	//~ if (1) {cout << "(WSE) OPCION <<" << i << "<< - NUMERO DE RESPUESTAS: "<< (activeTasks[indice])->votacion[i].size() <<"\tUmbral: " <<umbral_votacion<<", H: " << H << ", umbral*H: "<< umbral_votacion*H << endl;}
    //	if ((int) (activeTasks[indice])->votacion[i].size() > umbral_votacion*H )	{
    //		//~ print_vec(activeTasks[indice]->votacion[i],"Votación para la opción");
    //		activeTasks[indice]->consenso=1;
    //	}
    //}
    busy = true;
    hold(CTE_CONSENSO_WSE);
    busy = false;
    (*observer)->t_consenso_server += CTE_CONSENSO_WSE;
}

void WSE::updateFrequencies(int indice)
{
    debug(time(), "Update frequencies ");
    map<int, list<int>> votingMap = activeTasks[indice]->getVotacion();
    list<int>::iterator it_opciones;
    int id_peer;
    for (int i = 0; i < (int) votingMap.size(); i++)
    {
        for (it_opciones = votingMap[i].begin(); it_opciones != votingMap[i].end(); it_opciones++)
        {
            id_peer = *it_opciones;
            frecuencias_voluntarios[id_peer] += 1;
        }
        busy = true;
        hold(CTE_UPD_FREQ);
        busy = false;
        (*observer)->t_upd_freq_server += CTE_UPD_FREQ;
    }
}

void WSE::updateScores()
{
    debug(time(), "Update scores ");
    //~ cout << "Actualizar_puntajes en WSE" << endl;
    //~ print_vec_double(ranking_voluntarios,"Ranking");
    //~ print_vec(SolvedTasks,"SolvedTasks",0);
    //~ print_vec(frecuencias_voluntarios,"frecuencias_voluntarios",0);
    //~ print_vec(opciones_totales_voluntarios,"opciones_totales_voluntarios",0);
    //~ print_vec_double(porc_utilizacion_voluntarios,"porc_utilizacion_voluntarios");
    double tiempo_actual = this->time();
    for (int i = 0; i < (int)ranking_voluntarios.size(); i++)
    {
        porc_utilizacion_voluntarios[i] = (*observer)->utilizacion[i] / tiempo_actual;
        if ((((double)opciones_totales_voluntarios[i]) != 0 )    &&   ( (double)ends[i] != 0 )    &&   ( (double)opciones_totales_voluntarios[i] != 0)  && (porc_utilizacion_voluntarios[i] != 0) )
        {
            ranking_voluntarios[i] = ((double)SolvedTasks[i] / (double)ends[i]) * ((double)frecuencias_voluntarios[i] / (double)opciones_totales_voluntarios[i]) +  (1.0 / porc_utilizacion_voluntarios[i]);
        }
        else
        {
            if ((double)porc_utilizacion_voluntarios[i] != 0)
            {
                ranking_voluntarios[i] = (1.0 / porc_utilizacion_voluntarios[i]);
            }
            else
            {
                ranking_voluntarios[i] = 0;
            }
        }
    }
    busy = true;
    hold(CTE_SCORES);
    busy = false;
    (*observer)->t_scores_server += CTE_SCORES;
}

void WSE::resendOrTerminate(int indice)
{
    debug(time(), "Resend or Terminate ");
    double multiplicador_TTL2 = 3;
    Task *task1 = activeTasks[indice];
    double TTL2 = multiplicador_TTL2 * ( task1->getTTL() );
    int red_asignada = task1->getRed();

    if ( ( task1->getConsenso() == 0 ) && ( task1->getReenviada() == 0 ))
    {
        //~ reenvio_tarea_reemplazo(peers_seleccionados_reenvio, indice, cacheSize, red_asignada, ranking_voluntarios,  H );
        vector<int >  peers = selectPeersResend(indice);
        debug(time(), "Resend " + to_string(indice));
        //~ print_vec(peers,"PEERS",indice);
        resend(indice, peers);
    }
    else
    {
        if ( ( time() - task1->getInitT() ) >= TTL2 )
        {
            (*observer)->contador_recepcion5[red_asignada]++;
        }
        if (task1->getConsenso() == 0)
        {
            (*observer)->contador_recepcion6[red_asignada]++;
        }
        //~ if ( ( ( time() - task1->getInitT() ) >= TTL2 ) ||     (allAnswers(indice) && caso!=2) || task1->getConsenso() == 1)  //El TTL2 se agotó
        //~ {
			debug(time(), "Terminate " + to_string(indice));
			endTask(indice, task1->getConsenso());
        //~ }
    }
}

vector<int > WSE::selectPeersResend(int indice)
{
    vector<int > peers_seleccionados_reenvio;
    Task *task1 = activeTasks[indice];
    int red_asignada = task1->getRed();
    //Encontrar los H puntajes máximos (si la desconexión de peers con descanso está desactivada).
    //~ encontrar_maximos(ranking_voluntarios, peers_seleccionados_reenvio, H, red_asignada);
    
    //Caso en que hay desconexión de peers por descanso:
    if (((int)peers_disponibles[red_asignada].size() ) > H){
    	encontrar_maximos_disponibles(ranking_voluntarios,peers_seleccionados_reenvio,H,red_asignada);
    }else{
    	//Buscar los peers con puntaje más alto entre los no disponibles.
    	int diferencia= H - (int)peers_disponibles[red_asignada].size();
    	encontrar_maximos_NO_disponibles(ranking_voluntarios,peers_seleccionados_reenvio,diferencia,red_asignada);
    	//Luego incluir a los disponibles.
    	set<int>::iterator it;
    	for (it=peers_disponibles[red_asignada].begin(); it!=peers_disponibles[red_asignada].end(); ++it){
    		peers_seleccionados_reenvio.push_back(*it);
    	}
    }
    
    //~ for (int i=0; i<(int)peers_seleccionados_reenvio.size();i++){
    	//~ assert(peers_seleccionados_reenvio[i]>=lim_inf[red_asignada] && peers_seleccionados_reenvio[i]<lim_sup[red_asignada]);
    //~ }
    //~ print_vec(peers_seleccionados_reenvio, "Peers en selectPeersResend", indice);
    return peers_seleccionados_reenvio;
}

void WSE::resend(int indice, vector<int> peers_seleccionados_reenvio)
{
    debug(time(), "RESEND " + to_string(indice));
    //~ print_vec(peers_seleccionados_reenvio,"peers_seleccionados_reenvio",indice);
    int chosen = 0;
    int enviadas = 0;

    Task *task1 = activeTasks[indice];
    int red_asignada = task1->getRed();
    
    (*observer)->contador_recepcion4[red_asignada]++;

    task1->set_Reenviada(1);
    //~ task1->set_Recibidas(0);
    //~ task1->votacion.clear();
    task1->resetVotingList();

    double multiplicador_TTL2 = 3;
    double TTL2 = multiplicador_TTL2 * ( activeTasks[indice]->getTTL() );
	
	activeTasks[indice]->set_TTL(TTL2);
	
    busy = true;
    hold(LATENCIA_BD_TASK);
    busy = false;
    t_com_wse_bd += LATENCIA_BD_TASK;

    MessageServer *mS;

    while (peers_seleccionados_reenvio.size() != 0)
    {
        chosen = peers_seleccionados_reenvio.back();
        debug(time(), "RESEND to: " + to_string(chosen));
        ends[chosen]++;		//incrementar ends[id_peer]
        //~ task1->peers_consultados_2.push_back(chosen);
        (*observer)->ends_obs[chosen]++;
        enviadas++;		//Incrementar el contador de enviadas del WSE (sirve para controlar ends[NP] entrando al while).
        
		mS = new MessageServer(SEND_TASK, task1->getTaskID(), 0,	
							   task1->getTTL(), serverID, chosen, task1->getOptionNr(), 0);
        
		if ( aggregationPeers.count(indice) !=0 ){
			mS->set_aggregationPeer(aggregationPeers[indice]);			//Aggregation peer for proposal2.
		}
		
        busy = true;
        hold(TIME_GEN2);
        busy = false;
        (*isp[red_asignada])->add_message2(mS);
        if( (*isp[red_asignada])->idle() && !(*isp[red_asignada])->get_busy() )
        {
            (*isp[red_asignada])->activateAfter( current() );
        }
        (*observer)->Time_tagReenv_wse[red_asignada] += time();
        (*observer)->N_tagReenv_wse[red_asignada]++;
        //~ debug(time(), "PEER "+to_string(chosen)+" REMOVIDO DE LA LISTA DE REENVIO");
        peers_seleccionados_reenvio.pop_back();
        //~ print_vec(peers_seleccionados_reenvio,"peers_seleccionados_reenvio",indice);
    }
    //~ debug(time(), "EXIT RESEND");
}

void WSE::debug(double time, string msg  )
{
    if (0) cout << "[" << time << "][WSE:" << name() << "][ " << msg << " ]" << endl;
}

void WSE::baselineAlgorithm(MessageServer * mS )
{
	int indice = mS->getTaskID();
	////------------------------------------------------------------------------------------------BASELINE
	switch (mS->getType())
	{
		case ASK_OBJ:
		{
			debug(time(), "Peer asks for object. Check if task is active. ");
			if (activeTasks.count(indice) != 0) 	//La tarea está en "activeTasks"
			{
				debug(time(), "Task is active. Is TTL valid? ");
				if ( !validTTL(indice))	 	//Controlamos si el TTL es válido
				{
					debug(time(), "Invalid TTL, terminate task ");
					endTask(indice, 0);   //TTL vencido - Sacar la tarea de en_proceso, enviar la tarea a la BD, y no contestar al responsable. Que la tarea le quede al responsable sin contestar.
					(*observer)->contador_responsable_elim++;
				}
				else
				{
					debug(time(), "Valid TTL, send Obj to peer ");
					sendObject2Peer(mS);
				}
			}
			else
			{
				debug(time(), "Task "+to_string(indice)+" not active in Server");
				//La tarea NO está en "activeTasks"
			}
			virtual_t_wse_peers += TIME_PEER1;
			busy = true;
			hold(TIME_PEER1);
			busy = false;
			debug(time(), "Hold TIME_PEER1 OK");
			break;
		}
		
		case SEND_RESULT:		//Case3 anterior
		{
		
			int indice = mS->getTaskID();
			int id_peer = mS->getSrc();
			debug(time(), "Result from peer " + to_string(id_peer) + ", task " + to_string(indice));
			consultas_totales++;
			SolvedTasks[id_peer]++;
			debug(time(), "Check if present ");
			if (activeTasks.count(indice) != 0) 	//La tarea está en "activeTasks"
			{
				int red_asignada = activeTasks[indice]->getRed();
				(*observer)->Time_tag3_wse[red_asignada] += this->time();
				(*observer)->N_tag3_wse[red_asignada]++;
				(*observer)->contador_recibidas[red_asignada] = consultas_totales;
				activeTasks[indice]->addRecibidas();	//Aumento el contador de respuestas recibidas.
				debug(time(), "Check TTL ");
				if ( validTTL(indice) )
				{
					activeTasks[indice]->accumulateAnswers(mS);
					debug(time(), "Check H answers ");
					if ( allAnswers(indice) )
					{
						debug(time(), "All answers received ");
						calculateConsensus(indice);
						updateFrequencies(indice);
						updateScores();
						resendOrTerminate(indice);			//Si hay que reenviar, se eligen los peers
					}
					(*observer)->contador_recepcion1[red_asignada]++;
				}
				else
				{
					debug(time(), "Invalid TTL, terminate task" + to_string(indice));
					calculateConsensus(indice);
					updateFrequencies(indice);
					endTask(indice, 0);   //TTL vencido - Sacar la tarea de en_proceso, enviar la tarea a la BD, y no contestar al responsable. Que la tarea le quede al responsable sin contestar.                        
					(*observer)->contador_recepcion2[red_asignada]++;
					(*observer)->contador_responsable_elim++;
				}
			}
			else
			{
				debug(time(), "NO existe en 'activeTasks' la tarea " + to_string(indice));	
				(*observer)->contador_miss_en_proceso++;
				//~ (*observer)->contador_recepcion3[red_asignada]++;
			}
			delete mS;
			break;
		}
		
		case RESULT_DL:		
		{
			checkResultDL(mS);
			delete mS;
			break;
		}
		
		case DL_MODEL_OK:		
		{
			int id_peer = mS->getSrc();
			voluntarios_DL_confirmados.push_back(id_peer);
			if (voluntarios_DL_confirmados.size() == voluntarios_DL.size()){
        		flag_DL_confirmados=1;
        	}
			delete mS;
			break;
		}
		
		default:
		{
			delete mS;
			//ignore message
		}
	}
	//------------------------------------------------------------------------------------------BASELINE
}

void WSE::proposal1Algorithm(MessageServer * mS )
{
	int indice = mS->getTaskID();
	////------------------------------------------------------------------------------------------PROPOSAL 1
	switch (mS->getType())
	{
		case ASK_OBJ:
		{
			debug(time(), "Peer asks for object. Check if task is active. ");
			if (activeTasks.count(indice) != 0) 	//La tarea está en "activeTasks"
			{
				debug(time(), "Task is active. Is TTL valid? ");
				if ( !validTTL(indice))	 	//Controlamos si el TTL es válido
				{
					debug(time(), "Invalid TTL, terminate task ");
					endTask(indice, 0);   //TTL vencido - Sacar la tarea de en_proceso, enviar la tarea a la BD, y no contestar al responsable. Que la tarea le quede al responsable sin contestar.
					(*observer)->contador_responsable_elim++;
				}
				else
				{
					debug(time(), "Valid TTL, send Obj to peer ");
					sendObject2Peer(mS);
				}
			}
			else
			{
				debug(time(), "Task "+to_string(indice)+" not active in Server");
				//La tarea NO está en "activeTasks"
			}
			virtual_t_wse_peers += TIME_PEER1;
			busy = true;
			hold(TIME_PEER1);
			busy = false;
			debug(time(), "Hold TIME_PEER1 OK");
			break;
		}

		case SEND_RESULT:		//Case3 anterior
		{

			int indice = mS->getTaskID();
			int id_peer = mS->getSrc();
			debug(time(), "Result from peer " + to_string(id_peer) + ", task " + to_string(indice));
			consultas_totales++;
			SolvedTasks[id_peer]++;
			debug(time(), "Check if present ");
			if (activeTasks.count(indice) != 0) 	//La tarea está en "activeTasks"
			{
				int red_asignada = activeTasks[indice]->getRed();
				(*observer)->Time_tag3_wse[red_asignada] += this->time();
				(*observer)->N_tag3_wse[red_asignada]++;
				(*observer)->contador_recibidas[red_asignada] = consultas_totales;
				activeTasks[indice]->addRecibidas();	//Aumento el contador de respuestas recibidas.
				debug(time(), "Check TTL ");
				if ( validTTL(indice) )
				{
					activeTasks[indice]->accumulateAnswers(mS);
					debug(time(), "Check H answers ");
					if ( allAnswers(indice) )
					{
						debug(time(), "All answers received ");
						calculateConsensus(indice);
						updateFrequencies(indice);
						updateScores();
						resendOrTerminate(indice);			//Si hay que reenviar, se eligen los peers
					}
					(*observer)->contador_recepcion1[red_asignada]++;
				}
				else
				{
					debug(time(), "Invalid TTL, terminate task" + indice);
					calculateConsensus(indice);
					updateFrequencies(indice);
					endTask(indice, 0);   //TTL vencido - Sacar la tarea de en_proceso, enviar la tarea a la BD, y no contestar al responsable. Que la tarea le quede al responsable sin contestar.                        
					(*observer)->contador_recepcion2[red_asignada]++;
					(*observer)->contador_responsable_elim++;
				}
				//~ }
			}
			else
			{
				debug(time(), "NO existe en 'activeTasks' la tarea " + indice);
				(*observer)->contador_miss_en_proceso++;
				//~ (*observer)->contador_recepcion3[red_asignada]++;
			}
			delete mS;
			break;
		}
		
		case RESULT_DL:		
		{
			checkResultDL(mS);
			delete mS;
			break;
		}
		
		case DL_MODEL_OK:		
		{
			int id_peer = mS->getSrc();
			voluntarios_DL_confirmados.push_back(id_peer);
			if (voluntarios_DL_confirmados.size() == voluntarios_DL.size()){
        		flag_DL_confirmados=1;
        	}
			delete mS;
			break;
		}
		
		default:
		{
			delete mS;
			//ignore message
		}
	}
	////------------------------------------------------------------------------------------------PROPOSAL 1
}


void WSE::proposal2Algorithm(MessageServer * mS )
{
	//control_TTLs();
	debug(time(), "proposal2Algorithm. Active tasks size: " + to_string(activeTasks.size()));
	int indice = mS->getTaskID();
	////------------------------------------------------------------------------------------------PROPOSAL 2
	switch (mS->getType())
	{
		case ASK_OBJ:
		{
			debug(time(), "Peer asks for object. Check if task is active. ");
			if (activeTasks.count(indice) != 0) 	//La tarea está en "activeTasks"
			{
				debug(time(), "Task is active. Is TTL valid? ");
				if ( !validTTL(indice))	 	//Controlamos si el TTL es válido
				{
					debug(time(), "Invalid TTL, terminate task ");
					endTask(indice, 0);   //TTL vencido - Sacar la tarea de en_proceso, enviar la tarea a la BD, y no contestar al responsable. Que la tarea le quede al responsable sin contestar.
					(*observer)->contador_responsable_elim++;
				}
				else
				{
					debug(time(), "Valid TTL, send Obj to peer ");
					mS->set_aggregationPeer(aggregationPeers[mS->getTaskID()]);
					
					debug(time(), "Task:" + to_string(mS->getTaskID())+", Responsable Peer:" + to_string(aggregationPeers[mS->getTaskID()]->getIP()));
					sendObject2PeerProp2(mS);			// Acá hay que enviar el ID del responsable de agregación también
				}
			}
			else
			{
				debug(time(), "Task "+to_string(indice)+" not active in Server");
				//La tarea NO está en "activeTasks"
			}
			virtual_t_wse_peers += TIME_PEER1;
			busy = true;
			hold(TIME_PEER1);
			busy = false;
			debug(time(), "Hold TIME_PEER1 OK");
			break;
		}

		case SEND_RESULT:		//Case3 anterior
		{

			int indice = mS->getTaskID();
			int id_peer = mS->getSrc();
			debug(time(), "Result from peer " + to_string(id_peer) + ", task " + to_string(indice));
			consultas_totales++;
			SolvedTasks[id_peer]++;
			debug(time(), "Check if present ");
			if (activeTasks.count(indice) != 0) 	//La tarea está en "activeTasks"
			{
				int red_asignada = activeTasks[indice]->getRed();
				(*observer)->Time_tag3_wse[red_asignada] += this->time();
				(*observer)->N_tag3_wse[red_asignada]++;
				(*observer)->contador_recibidas[red_asignada] = consultas_totales;
				activeTasks[indice]->addRecibidas();	//Aumento el contador de respuestas recibidas.
				debug(time(), "Check TTL ");
				if ( validTTL(indice) )
				{
					activeTasks[indice]->set_Consenso(mS->getConsenso());		//Consensus is calculated in aggregation peers.
					debug(time(), "Task " + to_string(indice) + ", Consensus " + to_string(activeTasks[indice]->getConsenso()));
					updateFrequencies(indice);
					updateScores();
					resendOrTerminate(indice);			//Si hay que reenviar, se eligen los peers
				}
				else
				{
					debug(time(), "Invalid TTL, terminate task" + to_string(indice));
					activeTasks[indice]->set_Consenso(mS->getConsenso());		//Consensus is calculated in aggregation peers.
					debug(time(), "Task " + to_string(indice) + ", Consensus " + to_string(activeTasks[indice]->getConsenso()));
					updateFrequencies(indice);
					endTask(indice, 0);   //TTL vencido - Sacar la tarea de en_proceso, enviar la tarea a la BD, y no contestar al responsable. Que la tarea le quede al responsable sin contestar.                        
					(*observer)->contador_recepcion2[red_asignada]++;
					(*observer)->contador_responsable_elim++;
				}
				//~ }
			}
			else
			{
				debug(time(), "NO existe en 'activeTasks' la tarea " + to_string(indice));
				(*observer)->contador_miss_en_proceso++;
				//~ (*observer)->contador_recepcion3[red_asignada]++;
			}
			delete mS;
			break;
		}
			
		case RESULT_DL:		
		{
			checkResultDL(mS);
			delete mS;
			break;
		}
		
		case DL_MODEL_OK:		
		{
			int id_peer = mS->getSrc();
			voluntarios_DL_confirmados.push_back(id_peer);
			if (voluntarios_DL_confirmados.size() == voluntarios_DL.size()){
        		flag_DL_confirmados=1;
        	}
			delete mS;
			break;
		}
		
		default:
		{
			delete mS;
			//ignore message
		}
	}
	////------------------------------------------------------------------------------------------PROPOSAL 2
}

int WSE::isRelevant(){
	debug(time(), "Relevancy filter");
	double LATENCIA_SERVER_FILTRADO = 0.05 / 3600;		//Para que sea en horas. Arquitectura 1 - Ofli
	busy=true; hold(LATENCIA_SERVER_FILTRADO);busy=false;
	int relevant=random_task_filter_server(0.62);
	return relevant;
}

void WSE::checkResultDL(MessageServer* temp){
	assert(flagArquitectura==2);
	int indice = temp->getTaskID();
	int id_peer = temp->getSrc();
	debug(time(), "checkResultDL, task "+to_string(temp->getTaskID()));
	if (activeTasks.count(indice) != 0){
		if ( temp->getConsenso() ==2){				//Si está, vemos si tuvo consenso o no.
			activeTasks[indice]->set_Consenso(2);
			updateFrequencies(indice);
			endTask(indice, 2);   
		}else if (validTTL(indice)){
			resendOrTerminate(indice);
		}else{
			endTask(indice, 0);
		}
	}
	
	debug(time(), "Salgo de checkResultDL");
}

void WSE::restControl(MessageServer* temp){
	switch (temp->getType()){
		case CONNECT: 
		{
			peers_disponibles[findNet(temp->getSrc())].insert(temp->getSrc());
			break;
		}
		
		case DISCONNECT: 
		{
			peers_disponibles[findNet(temp->getSrc())].erase(temp->getSrc());
			break;
		}
		
		default: 
		{
			break;
		}
	}
}

int WSE::findNet(int peer){
	int net = 0;
	int acc = 0;
	for (int i=0;i<nro_redes;i++){
		acc += NP[i];
		if ( peer >  acc ){
			assert(i!=(nro_redes-1));
			net = i + 1;
		}
	}
	return net;
}


void WSE::add_All_AvailablePeers(list<int> &peers_consultados, int red_asignada){
	int chosen;
	set<int>::iterator it;
	if (peers_disponibles[red_asignada].size() !=0){
		for (it=peers_disponibles[red_asignada].begin(); it!=peers_disponibles[red_asignada].end(); ++it){
			chosen=*it;
			peers_consultados.push_back(chosen);
		}
	}
}

void WSE::add_N_AvailablePeers(list<int> &peers_consultados, int red_asignada, int N){
	int chosen;
	list<int>::iterator it_peers_consultados;
	for (int jj=0;jj<N;jj++){
		int repeticion=1;
		if (peers_consultados.size()!=0){
			while (repeticion==1){
				repeticion=0;
				chosen = lim_inf[red_asignada] + rand()%NP[red_asignada];
				assert(chosen!=total_NP);								//~ cout << "Chosen: " << chosen << endl;
				for (it_peers_consultados=peers_consultados.begin();it_peers_consultados!=peers_consultados.end();it_peers_consultados++){
					if (*it_peers_consultados == chosen){
						repeticion=1;
					}
				}
			}
		}else{
			chosen = lim_inf[red_asignada] + rand()%NP[red_asignada];	//~ cout << "Chosen: " << chosen << endl;
		}
		peers_consultados.push_back(chosen);
	}
}

void WSE::add_N_UnavailablePeers(list<int> &peers_consultados, int red_asignada, int N){
	int chosen;
	list<int>::iterator it_peers_consultados;
	//Elegir sólo entre los disponibles. Es decir, elegir H número aleatorios entre 0 y el size de los disponibles,
	//y luego poner el iterador al principio, e ir calculando it=peers_disponibles.begin(); chosen = it+rand()% ( peers_disponibles[red_asignada].size() )
	for (int jj=0;jj<H;jj++){
		int repeticion=1;
		if (peers_consultados.size()!=0){
			while (repeticion==1){
				repeticion=0;
				set<int>::iterator it_set1;
				it_set1 = peers_disponibles[red_asignada].begin();
				int offset1 = rand()%peers_disponibles[red_asignada].size();
				advance(it_set1,offset1);
				chosen=*it_set1;					//~ cout << "Chosen: " << chosen << endl;	assert(chosen!=total_NP);
				for (it_peers_consultados=peers_consultados.begin();it_peers_consultados!=peers_consultados.end();it_peers_consultados++)
				{
					if (*it_peers_consultados == chosen){
						repeticion=1;
					}
				}
			}
		}else{
			set<int>::iterator it_set1;
			it_set1 = peers_disponibles[red_asignada].begin();
			int offset1 = rand()%peers_disponibles[red_asignada].size();
			advance(it_set1,offset1);
			chosen=*it_set1;	//~ cout << "Chosen: " << chosen << endl;
		}
		peers_consultados.push_back(chosen);
	}
}


