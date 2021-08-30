#include "Observer.h"
// #include <sstream>
#include <iostream>
#include <fstream>
#include <numeric>

#include <ctime>



void Observer::inner_body(void)
{
    int escribir_estadisticas = 0;
    // ofstream archivo_salida_longitudes;
    // if (escribir_estadisticas==1) {archivo_salida_longitudes.open ("registros/archivo_salida_longitudes_obs.txt",ios_base::app);}
    // ofstream archivo_salida_tiempos;
    // if (escribir_estadisticas==1) {archivo_salida_tiempos.open ("registros/archivo_salida_tiempos_obs.txt",ios_base::app);}
    // ofstream archivo_salida_msjes;
    // if (escribir_estadisticas==1) {archivo_salida_msjes.open ("registros/archivo_salida_msjes_obs.txt",ios_base::app);}

    //~ stringstream outStream;
    //~ outStream << "time nQueriesOut nQueriesIn hitLocal hitGlobal hitLCache miss avHop nReplicas Gini avLatency" << endl;
    stringstream outStream;

    s = outStream.str();


    //cout <<	 "nIt"  <<  ";"<< "nQ" <<  ";"<< "C_Size" <<  ";"<< "locC_Sz"<< ";"<< "LC_Sze";
    //cout <<  ";"<< "H" <<  ";"<< "NR" <<  ";"<< "peers" <<  ";" << "TTL" <<  ";"<< "umbral_votacion"<<  ";"<< "caso";
    //cout <<  ";"<< "time" <<  ";"<< "Final_T" <<  ";"<< "terminadas" << ";"<< "tareas_finalizadas";
    //cout <<  ";"<< "max_L_pend" << ";"<<  "max_L_msg" <<  ";"<<  "av_L_pend" <<  ";"<< "av_L_msg" <<  ";"<< "W_av_L_pend";
    //cout <<  ";"<< "W_av_L_msg" <<  ";"<< "virtual_t_wse_gen" <<  ";"<< "virtual_t_wse_peers" <<  ";"<< "cnt_eli";
    //cout <<  ";"<< "cnt_env" <<  ";"<< "av_taskT1" <<  ";"<< "av_taskT2" <<  ";"<< "Ut_prom";
    //cout <<  ";"<< "Task_service_time" <<  ";"<< "Task_processing_time" ;
    //cout <<  ";"<< "t_com_wse_bd" <<  ";" <<"t_consenso_peers" <<  ";" << "t_consenso_server" << ";"<<"t_scores_server" <<  ";"<< "t_upd_freq_server" <<  ";"<< "t_proc_server" ;
    //cout <<  ";"<< "Time_tagGen_wse" << ";"<< "N_tagGen_wse" <<  ";" << "nQ_Out" <<";"<< "contador_consenso"<<";"<< "contador_consenso_negativo"<<  ";"<< "perc_consenso";
    //cout <<  ";"<< "TP" << ";" << "maxElement_DB" << ";" << "minElement_DB" << ";" << "mean_DB" << ";" << "stdDeviation_DB"<< ";" << "escenario";
    //for (int j=0;j<nro_redes;j++){
    //	cout <<  ";" << "nQ_In"+to_string(j) <<  ";" << "hitLoc"+to_string(j) <<  ";"  << "hitGlob"+to_string(j);
    //	cout <<  ";" << "hitLC_"+to_string(j) <<  ";" << "miss_gl_"+to_string(j) <<  ";" << "miss_loc_"+to_string(j) <<  ";"<< "avHop"+to_string(j);
    //	cout <<  ";" << "nReplicas_"+to_string(j) <<  ";" << "gini_"+to_string(j) <<  ";" << "avLat_"+to_string(j);
    //	cout <<  ";"<< "TP_net_"+to_string(j) <<  ";"<< "objArrival_"+to_string(j) <<  ";"<< "ut_prom_net_"+to_string(j) <<  ";"<< "gini_util_net_"+to_string(j) <<  ";"<< "Cnt1_"+to_string(j);
    //	cout <<  ";"<< "Cnt2_"+to_string(j) <<  ";"<< "Desc_EP_"+to_string(j) <<  ";"<< "Cnt_reenvio_"+to_string(j) <<  ";"<< "Cnt_TTL2_"+to_string(j) <<  ";"<< "No_agree_"+to_string(j);
    //	cout <<  ";"<< "t_red_p2p_"+to_string(j) <<  ";" << "t_red_isp_"+to_string(j);
    //	cout <<  ";"<< "Time_tag0_client_"+to_string(j) <<  ";"<<  "Time_tag1_client_"+to_string(j) <<  ";"<< 	 "Time_tag2_client_"+to_string(j) <<  ";"<< 	 "Time_tag3_client_"+to_string(j);
    //	cout <<  ";"<<  "Time_tagPeer_wse_"+to_string(j) <<  ";"<< 	 "Time_tag3_wse_"+to_string(j) <<  ";"<< 	 "Time_tagReenv_wse_"+to_string(j);
    //	cout <<  ";"<< "N_tag0_client_"+to_string(j) <<  ";"<< 	 "N_tag1_client_"+to_string(j) <<  ";"<< 	 "N_tag2_client_"+to_string(j) <<  ";"<< 	 "N_tag3_client_"+to_string(j);
    //	cout <<  ";"<<  "N_tagPeer_wse_"+to_string(j) <<  ";"<<  "N_tag3_wse_"+to_string(j) <<  ";"<< 	 "N_tagReenv_wse_"+to_string(j);
    //	cout <<  ";"<< "N_ask_obj_"+to_string(j) <<  ";"<< "N_ask_replica_"+to_string(j) ;
    //	cout <<  ";"<< "contador_isp_Sol_Img_"+to_string(j) <<  ";"<< "contador_isp_env_Img_"+to_string(j) <<  ";"<< "contador_isp_env_tarea_"+to_string(j) <<  ";"<< "contador_isp_resultados_"+to_string(j);
    //}
    //for (int j=0;j<17;j++){
    //	cout <<  ";"<< "TS"+to_string(j);
    //}
    //cout<<  ";"<< "flagArquitectura"<<  ";"<< "cte_traffJam_isp"<<  ";"<< "cte_traffJam_p2p"<<  ";"<< "peso_respuestas_expertos"<<";"<< "TP_finalizadas"<<  ";"<< "perc_consenso";;
    //cout << endl;



    //int contador_ceros=0;
    vector<double> longitudes_en_proceso;
    vector<double> longitudes_cola_msjes;
    double longitud_actual = 0;
    double longitud_cola_msjes = 0;
    vector<double> vector_tiempos;
    double tiempo_actual = 0;
    int registrar = 1;
    //cout << outStream.str();   fflush(stdout);
    //double OBSERVATION_INTERVAL = 14400;
    //double OBSERVATION_INTERVAL = 7200;
    double OBSERVATION_INTERVAL = 10000000;
    //double OBSERVATION_INTERVAL = 7200*2;

    std::clock_t c_start = std::clock();


    while (1)
    {

        hold(OBSERVATION_INTERVAL);
        if (registrar == 1)
        {
            tiempo_actual = this->time();
            vector_tiempos.push_back(tiempo_actual);

            //Calcular longitud de "en_proceso"
            longitud_actual = ((*wse)->en_proceso).size();
            longitudes_en_proceso.push_back(longitud_actual);
            //~ cout << "registrar en proceso " << longitud_actual << endl;

            //Calcular longitud de cola de mensajes de peers
            longitud_cola_msjes = ((*wse)->queue_in).size();
            longitudes_cola_msjes.push_back(longitud_cola_msjes);
            //~ cout << "registrar msjes " << longitud_cola_msjes << endl;


            //Medir timepo
            //~ cout << "registrar tiempo " << tiempo_actual << endl;
        }

        /**************** Comentar para tener observaciones múltiples *****************/
        if (ends_obs[np] == 1)
        {
            registrar = 0;
            //cout << "Observer - end_nodes==1"<< endl;
        }
        /******************************************************************************/

        /**************** Comentar para tener observaciones múltiples *****************/
        if ((this->time() >= DELTA_O) || (registrar == 0) )
        {
            /******************************************************************************/
            //~ cout << "Observer check " << tiempo_actual << endl;
            //cout << "(OBS) Condicion 1: "<< (this->time() >= DELTA_O) << endl;
            //cout << "(OBS) Condicion 2: "<< (registrar == 0) << endl;
            //hold(DELTA_O);


            //Imprimir vector de longitudes de "en_proceso"
            //if (escribir_estadisticas==1) {
            //	for (int k=0;k<(int)(longitudes_en_proceso.size());k++){
            //		archivo_salida_longitudes << longitudes_en_proceso[k] << '\n';
            //	}archivo_salida_longitudes << "\r\n";
            //	//Imprimir vector de longitudes de cola de entrada de msjes de peers en el wse.
            //	for (int k=0;k<(int)(longitudes_cola_msjes.size());k++){
            //		archivo_salida_msjes << longitudes_cola_msjes[k] << '\n';
            //	}archivo_salida_msjes << "\r\n";
            //	//Imprimir vector de tiempos para las longitudes
            //	for (int k=0;k<(int)(vector_tiempos.size());k++){
            //		archivo_salida_tiempos << vector_tiempos[k] << '\n';
            //	}archivo_salida_tiempos << "\n";
            //}
            double maximo_en_proceso = *std::max_element(longitudes_en_proceso.begin(), longitudes_en_proceso.end());
            double maximo_queue_in = *std::max_element(longitudes_cola_msjes.begin(), longitudes_cola_msjes.end());
            double suma_en_proceso = accumulate(longitudes_en_proceso.begin(), longitudes_en_proceso.end(), 0);
            double suma_queue_in = accumulate(longitudes_cola_msjes.begin(), longitudes_cola_msjes.end(), 0);
            double promedio_en_proceso = suma_en_proceso / T_finalizacion;
            double promedio_queue_in = suma_queue_in / T_finalizacion;
            //if (escribir_estadisticas==1) {
            //	archivo_salida_longitudes << "Maximo en proceso: "<< maximo_en_proceso << "\r\n";
            //	archivo_salida_msjes << "Maximo queue in : "<< maximo_queue_in << "\r\n";
            //	archivo_salida_longitudes << "Promedio en proceso: "<< promedio_en_proceso << "\r\n";
            //	archivo_salida_msjes << "Promedio queue in: "<< promedio_queue_in << "\r\n";
            //	archivo_salida_longitudes << "Promedio ponderado en proceso: " << (*wse)->promedio_ponderado_en_proceso << "\r\n";
            //	archivo_salida_msjes << "Promedio ponderado queue in: " << (*wse)->promedio_ponderado_queue_in << "\r\n";
            //}


            // compute statistics
            outStream.str("");

            vector<double> avHop;
            vector<double> avLatency;
            for (int j = 0; j < nro_redes; j++)
            {
                avHop.push_back(0);
                avLatency.push_back(0);
            }
            for (int j = 0; j < nro_redes; j++)
            {
                if(nqueriesIn[j] > 0)
                {
                    avHop[j] = (double)hops[j] / nqueriesIn[j];
                    avLatency[j] = (double)latencia[j] / nqueriesIn[j];
                }
            }

            //  else
            // {
            // avHop=-1;
            // avLatency-1;
            // }

            //~ double gini = Observer::getGini();

            double throughput = 0;
            //~ throughput = totalQueries/T_finalizacion;						//Cuando hay una sola observación por simulación
            terminadas_TP = (double)tareas_finalizadas - tareas_finalizadas_previas;
            tareas_finalizadas_previas = tareas_finalizadas;
            throughput = terminadas_TP / OBSERVATION_INTERVAL; 				//Cuando se usan observaciones múltiples.

            vector<double> throughput_net;
            for (int j = 0; j < nro_redes; j++)
            {
                throughput_net.push_back(0);
            }
            for (int j = 0; j < nro_redes; j++)
            {
                terminadas_TP_net[j] = (double)tareas_finalizadas_net[j] - tareas_finalizadas_previas_net[j];
                tareas_finalizadas_previas_net[j] = tareas_finalizadas_net[j];
                throughput_net[j] = terminadas_TP_net[j] / OBSERVATION_INTERVAL; 				//Cuando se usan observaciones múltiples.
            }

            //if (cacheSize!=0){
            //	throughput = nqueriesIn/T_finalizacion;
            //	//~ cout << "Consultas Totales: " << (*wse)->consultas_totales << endl;
            //}else{
            //	throughput = (*wse)->consultas_totales/T_finalizacion;
            //	//~ cout << "Consultas Totales: " << (*wse)->consultas_totales << endl;
            //}
            for(int i = 0; i < np; i++)
            {
                utilizacion_promedio += (utilizacion[i]) / np;
                //utilizacion_promedio[i]+=utilizacion->at(i)/DELTA_O;
            }


            //=============== Escribir el arreglo con la utilización de los peers a un archivo.
            //~ if ( ((int)(this->time())) % 1000 == 0) {
            //~ ofstream archivo_utilizacion;
            //~ archivo_utilizacion.open ("registros/archivo_utilizacion.csv",ios_base::app);
            //~ for(int i =0; i < (np-1); i++)
            //~ {
            //~ archivo_utilizacion << utilizacion[i] << ";";
            //~ }
            //~ archivo_utilizacion << utilizacion[np] << endl;
            //~ }
            //===============

            //=============== Escribir Tiempo de Ejecución del simulador para comparar cómo varía con nQ y peers.
            std::clock_t c_end = std::clock();
            long double time_elapsed_ms = 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC;
            // ofstream archivo_tiempos_ejecucion;
            // archivo_tiempos_ejecucion.open ("registros/archivo_tiempos_ejecucion.csv",ios_base::app);
            // archivo_tiempos_ejecucion << iteracion_sim << ";" << np << ";" << totalQueries << ";"<< time_elapsed_ms/1000.0 << endl;
            //===============

            {
                //t_proc_server =  t_consenso_server	 + t_scores_server + t_upd_freq_server + Time_tagGen_wse + (*wse)->virtual_t_wse_peers;
                //outStream << iteracion_sim  <<  ";"<< totalQueries <<  ";"<< cacheSize <<  ";"<< localCacheSize<< ";"<< L_cacheSize;
                //outStream <<  ";"<< Peers_envio <<  ";"<< normal_rate <<  ";"<< np <<  ";" << TTL <<  ";"<< umbral_votacion<<  ";"<< caso;
                //outStream <<  ";"<< this->time() <<  ";"<< T_finalizacion <<  ";"<< terminadas_TP << ";"<< tareas_finalizadas;
                //outStream <<  ";"<< maximo_en_proceso << ";"<<  maximo_queue_in <<  ";"<<  promedio_en_proceso <<  ";"<< promedio_queue_in <<  ";"<< (*wse)->promedio_ponderado_en_proceso;
                //outStream <<  ";"<< (*wse)->promedio_ponderado_queue_in <<  ";"<< (*wse)->virtual_t_wse_gen <<  ";"<< (*wse)->virtual_t_wse_peers <<  ";"<< contador_responsable_elim;
                //outStream <<  ";"<< contador_enviadas <<   ";"<< tiempo_tareas_1 <<  ";"<< tiempo_tareas_2 <<  ";"<< utilizacion_promedio;
                //outStream <<  ";"<< Task_service_time <<  ";"<< Task_processing_time ;
                //outStream <<  ";"<< (*wse)->t_com_wse_bd <<  ";" <<t_consenso_peers <<  ";" << t_consenso_server << ";"<<t_scores_server <<  ";"<< t_upd_freq_server <<  ";"<< t_proc_server ;
                //outStream <<  ";"<< Time_tagGen_wse << ";"<< N_tagGen_wse <<  ";" << nqueriesOut <<";"<< contador_consenso << ";"<< contador_consenso_negativo << ";"<< (*wse)->perc_consenso;
                //outStream <<  ";"<<throughput<< ";" << maxElement_DB << ";" << minElement_DB << ";" << mean_DB << ";" << stdDeviation_DB<< ";" << flag_escenario;
                //
                //for (int j=0;j<nro_redes;j++){
                //	double gini = Observer::getGini_per_net(j,NP[j]);
                //	double utilizacion_promedio_red= Observer::get_AveUtilizacion_per_net(j,NP[j]);
                //	utilizacion_promedio_red=utilizacion_promedio_red/OBSERVATION_INTERVAL;
                //	double gini_utilizacion_red= Observer::getGini_utilizacion_per_net(j,NP[j]);
                //	outStream <<  ";" << nqueriesIn[j] <<  ";" << hitLocal[j] <<  ";"  << hitGlobal[j];
                //	outStream <<  ";" << hitLCache[j] <<  ";" << miss_global[j] <<  ";" << miss_local[j] <<  ";"<< avHop[j];
                //	outStream <<  ";" << nReplicas[j] <<  ";" << gini <<  ";" << avLatency[j];
                //	outStream <<  ";"<< throughput_net[j] <<  ";"<< objArrival[j] <<  ";"<< utilizacion_promedio_red <<  ";"<< gini_utilizacion_red <<  ";"<< contador_recepcion1[j];
                //	outStream <<  ";"<< contador_recepcion2[j] <<  ";"<< contador_recepcion3[j] <<  ";"<< contador_recepcion4[j] <<  ";"<< contador_recepcion5[j] <<  ";"<< contador_recepcion6[j];
                //	outStream <<  ";"<< t_red_p2p[j] <<  ";" << t_red_isp[j];
                //	outStream <<  ";"<< Time_tag0_client[j] <<  ";"<<  Time_tag1_client[j] <<  ";"<< 	 Time_tag2_client[j] <<  ";"<< 	 Time_tag3_client[j];
                //	outStream <<  ";"<<  Time_tagPeer_wse[j] <<  ";"<< 	 Time_tag3_wse[j] <<  ";"<< 	 Time_tagReenv_wse[j];
                //	outStream <<  ";"<< N_tag0_client[j] <<  ";"<< 	 N_tag1_client[j] <<  ";"<< 	 N_tag2_client[j] <<  ";"<< 	 N_tag3_client[j];
                //	outStream <<  ";"<<  N_tagPeer_wse[j] <<  ";"<<  N_tag3_wse[j] <<  ";"<< 	 N_tagReenv_wse[j];
                //	outStream <<  ";"<< N_ask_obj[j] <<  ";"<< N_ask_replica[j] ;
                //	outStream <<  ";"<< contador_isp_Sol_Img[j] <<  ";"<< contador_isp_env_Img[j] <<  ";"<< contador_isp_env_tarea[j] <<  ";"<< contador_isp_resultados[j];
                //}
                //outStream <<  ";"<< TimeStamps[0] ;
                //for (int j=1;j<17;j++){
                //	//outStream <<  ";"<< TimeStamps[j] - TimeStamps[j-1] ;
                //	outStream <<  ";"<< TimeStamps[j];
                //}
                //outStream <<  ";"<< flagArquitectura<<  ";"<< cte_traffJam_isp<<  ";"<< cte_traffJam_p2p<<  ";"<< peso_respuestas_expertos<<";"<< tareas_finalizadas/T_finalizacion<<  ";"<<  (*wse)->perc_consenso;
                //outStream << endl;



				double gini = Observer::getGini_per_net(0,NP[0]);
                ////CODIGO QUE USE PARA LA VERIFICACION NO ALEATORIA PARA SOLUCIONAR LOS ERRORES DE DEEP COPY
                //~ outStream 	<< "===================================================================================================================";
                //~ outStream 	<< "nQ" <<":\t"<<totalQueries << "\n";
                //~ outStream 	<< "H" <<":\t"<<Peers_envio << "\n";
                //~ outStream 	<< "NR" <<":\t"<<normal_rate << "\n";
                //~ outStream 	<< "peers" <<":\t"<<np << "\n";
                //~ outStream 	<< "TTL" <<":\t"<<TTL << "\n";
                //~ outStream 	<< "umbral_votacion" <<":\t"<<umbral_votacion << "\n";
                //~ outStream 	<< "time" <<":\t"<<this->time() << "\n";
                //~ outStream 	<< "Final_T" <<":\t"<<T_finalizacion << "\n";
                //~ outStream 	<< "terminadas" <<":\t"<<tareas_finalizadas << "\n";
                //~ outStream 	<< "TP_finalizadas" <<":\t"<<(tareas_finalizadas/T_finalizacion) << "\n";
                //~ outStream 	<< "perc_consenso" <<":\t"<<(*wse)->perc_consenso  << "\n";
                //~ outStream 	<< "Contador_consenso" <<":\t"<<contador_consenso  << "\n";
                //~ outStream 	<< "Cnt_reenvio" <<":\t"<<contador_recepcion4[0] << "\n";
                //~ outStream 	<< "contador_elim_TTL" <<":\t"<< contador_responsable_elim << "\n";
                //~ outStream 	<< "TaskEndT" <<":\t"<< Task_processing_time << "\n";
                //~ outStream 	<< "gini" <<":\t"<< gini << "\n";
                //~ outStream 	<< "avHop" <<":\t"<< avHop[0] << "\n";
                //~ outStream 	<< "t_red_p2p" <<":\t"<< t_red_p2p[0] << "\n";
                //~ outStream 	<< " t_red_isp" <<":\t"<<  t_red_isp[0] << "\n";

                outStream 	<< "Proposal" << ";"<< "Arch" << ";" << "nQ" << ";" << "H" <<  ";" << "NR" <<  ";" << "peers" <<  ";" << "TTL" <<  ";" << "umbral_votacion";
                outStream 	<< ";" << "time" <<  ";" << "Final_T" << ";" << "t_red_isp_0" << ";" << "terminadas";
                outStream 	<< ";" << "TP_finalizadas" <<  ";" << "perc_consenso" <<  ";" << "Contador_consenso";
                outStream	<< ";" << "Cnt_reenvio" << ";" << "contador_elim_TTL";
                outStream	<< ";" << "TaskServeT" << ";" << "TaskEndT";
                outStream	<< ";" << "Gini" << ";" << "avHop"<< ";" << "t_red_p2p"<< ";" << "DL_tasks"<< ";" << "flagDisponibles"<< ";" << "max_work_time"<< ";" << "disconnections" << endl;
                outStream 	<< caso  << ";"<< flagArquitectura  << ";" << totalQueries  << ";" << Peers_envio << ";" << normal_rate << ";" <<  np <<  ";" << TTL <<  ";" << umbral_votacion / (double)100;
                outStream 	<<  ";" <<  this->time() << ";" <<  T_finalizacion << ";" << t_red_isp[0] << ";" << tareas_finalizadas;
                outStream 	<<  ";" << tareas_finalizadas / T_finalizacion <<  ";" << (*wse)->perc_consenso <<  ";" << contador_consenso;
                outStream	<< ";" << contador_recepcion4[0] << ";" << contador_responsable_elim;
                outStream	<< ";" << Task_service_time << ";" << Task_processing_time;
                outStream	<< ";" << gini << ";" << avHop[0] << ";" << t_red_p2p[0]<< ";" << DL_tasks<< ";" << flagDisponibles <<  ";" << max_work_time<<  ";" << disconnections<< endl;

            }

            //~ (*wse)->mostrar_tareas_en_proceso(0);
            //~ (*wse)->print_vec((*wse)->tareas_DL,"Tareas pendientes con DL", 0);

            s.append(outStream.str());
            cout << outStream.str();
            fflush(stdout);

            // limpiar variables
            nqueriesOut = 0;
            contador_responsable_elim = 0;
            contador_enviadas = 0;

            for(int i = 0; i < nro_redes; i++)
            {
                nqueriesIn[i] = 0;
                hitLocal[i] = 0;
                hitGlobal[i] = 0;
                hitLCache[i] = 0;
                objArrival[i] = 0;
                miss[i] = 0;
                hops[i] = 0;
                latencia[i] = 0;
                nReplicas[i] = 0;
                contador_recepcion1[i] = 0;
                contador_recepcion2[i] = 0;
                contador_recepcion3[i] = 0;
                contador_recepcion4[i] = 0;
                contador_recepcion5[i] = 0;
                contador_recepcion6[i] = 0;
                contador_responsable_env[i] = 0;
                contador_recibidas[i] = 0;
                Time_tagReenv_wse[i] = 0;
                Time_tag0_client[i] = 0;
                Time_tag1_client[i] = 0;
                Time_tag2_client[i] = 0;
                Time_tag3_client[i] = 0;
                Time_tagPeer_wse[i] = 0;
                Time_tag3_wse[i] = 0;
                N_tag0_client[i] = 0;
                N_tag1_client[i] = 0;
                N_tag2_client[i] = 0;
                N_tag3_client[i] = 0;
                N_tagPeer_wse[i] = 0;
                N_tag3_wse[i] = 0;
                N_tagReenv_wse[i] = 0;
                N_ask_obj[i] = 0;
                N_ask_replica[i] = 0;
                t_red_p2p[i] = 0;
                t_red_isp[i] = 0;
                contador_isp_Sol_Img[i] = 0;
                contador_isp_env_Img[i] = 0;
                contador_isp_env_tarea[i] = 0;
                contador_isp_resultados[i] = 0;

            }
            utilizacion_promedio = 0;

            for(int i = 0; i < np; i++)
            {
                load[i] = 0;
            }
            for(int i = 0; i < np; i++)
            {
                utilizacion[i] = 0;
            }


            /**************** Comentar para tener Acumulación de métricas en observaciones múltiples *****************/

            //~ contador_responsable_elim=0;
            //~ contador_responsable_env=0;
            //~ contador_enviadas=0;
            //~ contador_recibidas=0;

            t_consenso_peers = 0;
            t_consenso_server = 0;
            t_scores_server = 0;
            t_upd_freq_server = 0;
            t_proc_server = 0;
            Task_service_time = 0;
            Task_processing_time = 0;
            Time_tagGen_wse = 0;
            N_tagGen_wse = 0;
            contador_consenso = 0;
            contador_consenso_negativo = 0;
            contador_responsable_elim = 0;
            DL_tasks = 0;

            //~ Time_tagReenv_wse=0;
            //~ Time_tag0_client=0;
            //~ Time_tag1_client=0;
            //~ Time_tag2_client=0;
            //~ Time_tag3_client=0;
            //~ Time_tagPeer_wse=0;
            //~ Time_tag3_wse=0;
            //~ N_tag0_client=0;
            //~ N_tag1_client=0;
            //~ N_tag2_client=0;
            //~ N_tag3_client=0;
            //~ N_tagPeer_wse=0;
            //~ N_tag3_wse=0;
            //~ N_tagReenv_wse=0;
            //~ N_ask_obj=0;
            //~ N_ask_replica=0;
            //~ contador_isp_Sol_Img=0;
            //~ contador_isp_env_Img=0;
            //~ contador_isp_env_tarea=0;
            //~ contador_isp_resultados=0;
            //tareas_finalizadas=0;					//Esta variable no tiene que reiniciarse. Por eso se calcula la diferencia para obtener "terminadas_TP"
            //tareas_finalizadas_previas=0;			//Esta variable no tiene que reiniciarse. Por eso se calcula la diferencia para obtener "terminadas_TP"
            terminadas_TP = 0;

            (*wse)->promedio_ponderado_en_proceso = 0;
            (*wse)->promedio_ponderado_queue_in = 0;
            (*wse)->virtual_t_wse_gen = 0;
            (*wse)->virtual_t_wse_peers = 0;
            (*wse)->t_com_wse_bd = 0;
            /*********************************************************************************************************/
            // dormir
            if (ends || (*wse)->ends[np] || ends_obs[np] == 1)
            {
                // archivo_salida.close();
                ends = true;
                // archivo_salida_msjes.close();
                // archivo_salida_longitudes.close();
                // archivo_salida_tiempos.close();
                passivate();
            }

            /**************** Comentar para tener observaciones múltiples *****************/
        }
        /******************************************************************************/
    }


}



double Observer::getGini()
{
    double l = 0;
    double gini = 0;
    double mean = 0;
    for(int i = 0; i < np ; i++)
    {
        l = l + load[i];
    }
    mean = l / np;
    sort(load.begin(), load.end() );
    for (int i = 0; i < np; i++)
    {
        int j = i + 1;
        gini = gini + (( 2 * j - ( np - 1 )) * load[i] );
    }
    if (mean > 0)
    {
        gini = gini / (np * np * mean);
    }
    return gini;
}


//======================================================================
//Esta función parece tener errores.
/** Primero se accede al vector de cargas  con los límites de la red,
 * y luego se accede con el índice "i", y encima se ordena el vector de
 * cargas completo, por lo que se pierde qué carga corresponde a qué peer
 * El problema está primero en "sort(load.begin(),load.end() );".
 * Hay que hacer el sort solo en los límites que corresponden a la red.
 * Debería ser entonces:
 *  sort(load.begin()+lim_inf[red_asignada],load.begin()+lim_sup[red_asignada])
 *
 * */

//======================================================================
double Observer::getGini_per_net(int red_asignada, int np_red_asignada)
{
    //cout << "Get Gini per Net, red " << red_asignada << endl;
    //print_vec(load, "LOAD", 0);
    double l = 0;
    double gini = 0;
    double mean = 0;
    for(int i = lim_inf[red_asignada]; i < lim_sup[red_asignada] ; i++)
    {
        l = l + load[i];
    }
    mean = l / np_red_asignada;
    /**sort(load.begin(),load.end() );*/
    sort(load.begin() + lim_inf[red_asignada], load.begin() + lim_sup[red_asignada]);
    for (int i = 0; i < np_red_asignada; i++)
    {
        int j = i + 1;
        gini = gini + (( 2 * j - ( np_red_asignada - 1 )) * load[i + lim_inf[red_asignada]] );
    }
    if (mean > 0)
    {
        gini = gini / (np_red_asignada * np_red_asignada * mean);
    }
    //cout << "Gini Net"  << red_asignada << " : "<< gini << endl;
    return gini;

}

double Observer::get_AveUtilizacion_per_net(int red_asignada, int np_red_asignada)
{
    double utilizacion_promedio_red = 0;
    for(int i = lim_inf[red_asignada]; i < lim_sup[red_asignada]; i++)
    {
        utilizacion_promedio_red += (utilizacion[i]) / np_red_asignada;
        //utilizacion_promedio[i]+=utilizacion->at(i)/DELTA_O;
        //~ cout << "Peer " << i << ". utilizacion: " << utilizacion[i] << ". np_red_asignada: " << np_red_asignada <<". particula (uti/np): " << (utilizacion[i])/np_red_asignada << ". utilizacion acumulada: " << utilizacion_promedio_red << endl;
    }
    //~ cout << "utilizacion promedio funcion para red " << red_asignada << ": " << utilizacion_promedio_red<<  endl;
    return utilizacion_promedio_red;
}

int Observer::getIteracion()
{
    return iteracion;
}

//======================================================================
double Observer::getGini_utilizacion_per_net(int red_asignada, int np_red_asignada)
{
    double gini = 0;
    //~ cout << "inicio cálculo gini utilizacion" << endl;
    double l = 0;
    double mean = 0;
    for(int i = lim_inf[red_asignada]; i < lim_sup[red_asignada] ; i++)
    {
        //l=l+load[i];
        l = l + utilizacion[i];
        //~ cout << "Peer " << i << ". utilizacion: " << utilizacion[i] << ". utilizacion acumulada: " << l << endl;
    }

    mean = l / np_red_asignada;
    /**sort(load.begin(),load.end() );*/
    sort(utilizacion.begin() + lim_inf[red_asignada], utilizacion.begin() + lim_sup[red_asignada]);
    for (int i = 0; i < np_red_asignada; i++)
    {
        int j = i + 1;
        gini = gini + (( 2 * j - ( np_red_asignada - 1 )) * utilizacion[i + lim_inf[red_asignada]] );
    }
    if (mean > 0)
    {
        gini = gini / (np_red_asignada * np_red_asignada * mean);
    }
    //~ cout << "Final cálculo gini utilizacion" << endl;
    return gini;
}
