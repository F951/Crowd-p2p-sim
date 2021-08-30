#ifndef GLOBAL_H
#define GLOBAL_H

//#include "/homes/rscheihi/carlos/SIMULADOR/libcppsim-0.2.1/src/cppsim.hh"
#include "../../src/cppsim.hh"
#include "../../src/rng.hh"
#include "../../src/sqsPrio.hh"
#include <iostream>
#include <map>
#include <sstream>
#include <set>
#include <vector>
#include <list>
#include <assert.h>
#include <fstream>
#include <string>
#include <limits>
#include <queue>
#include <math.h>
#include <algorithm>
//#include "../../bigint/BigIntegerLibrary.hh"
#include <openssl/bn.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>



using namespace std;

// #define CANTIDAD_PEERS_ENVIO 30
// #define SIZE_IMAGEN 1000

// class escritura_a_archivo
// {
// public:
//   static ofstream& GetStream()
//   {
//     static ofstream myfile_logs("LOG_DEBUG");
//     return myfile_logs;
//   }
// };


//#define TIEMPO_DE_TRABAJO 2
//~ #define DEBUG_WSE 1
#define DEBUG_CPU_TIME 0
#define DEBUG_UTILIZACION 0
//#define MOSTRAR_SALIDA_TLC 1
#define MOSTRAR_SALIDA_WSE_CLIENT 0


//#define TIME_GEN1 0.00006
//#define TIME_GEN2 0.0015
//#define TIME_GEN3 0.000001
//#define TIME_PEER1 0.00115
////#define TIME_PEER2 0.00097    //SACO ESTO, PORQUE USO LOS TIEMPOS DE ACA ABAJO
//#define CTE_SCORES  0.0025
//#define CTE_AGREGACION_PEERS 0.0000025
//#define CTE_AGREGACION_WSE 0.0000015
//#define CTE_CONSENSO_WSE 0.00127
//#define CTE_UPD_FREQ 0.0006
//
//#define LATENCIA_BD_IMG 0.003
//#define LATENCIA_BD_TASK 0.00005
//#define LATENCIA_PEER_WSE 0.2
//#define LATENCIA_RED_P2P 0.035
//
//#define DELAY_BASE_ISP 0.441
//#define LATENCIA_SOLICITUD_TAREA_ISP 0.0001887
//#define LATENCIA_SOLICITUD_OBJETO_ISP 0.0001887
//#define LATENCIA_OBJETO_OPCIONES_ISP 0.283 	//1.5MB					//0.566	para 3 MB, tasa de transferencia 5,3 MBps
//#define LATENCIA_TRABAJO_TERMINADO_ISP 0.0003773
//#define LATENCIA_TRABAJO_AGREGADO_ISP 0.0003773
//
//#define DELAY_BASE_P2P 0.228
//#define LATENCIA_SOLICITUD_TAREA_P2P 0.0001153
//#define LATENCIA_SOLICITUD_OBJETO_P2P 0.0001153
//#define LATENCIA_OBJETO_OPCIONES_P2P 0.173	//1.5MB			//0.346 para 3 MB, tasa de tranferencia 8,67 MBps
//#define LATENCIA_TRABAJO_TERMINADO_P2P 0.0002307
//#define LATENCIA_TRABAJO_AGREGADO_P2P 0.0002307

//**********************************************************************************************
//VALORES EN HORAS:
#define	TIME_GEN1		1.66666666666667E-08
#define	TIME_GEN2		4.16666666666667E-07
#define	TIME_GEN3		2.77777777777778E-10
#define	TIME_PEER1		3.19444444444445E-07
#define CTE_SCORES		6.94444444444445E-07
#define	CTE_AGREGACION_PEERS		6.94444444444444E-10
#define	CTE_AGREGACION_WSE		4.16666666666667E-10
#define	CTE_CONSENSO_WSE		3.52777777777778E-07
#define	CTE_UPD_FREQ		1.66666666666667E-07

#define LATENCIA_BD_IMG		8.33333333333333E-07
#define	LATENCIA_BD_TASK		1.38888888888889E-08
#define	LATENCIA_PEER_WSE		5.55555555555556E-05
#define	LATENCIA_RED_P2P		9.72222222222222E-06

#define DELAY_BASE_ISP		0.0001225
#define	LATENCIA_SOLICITUD_TAREA_ISP		5.24166666666667E-08
#define	LATENCIA_SOLICITUD_OBJETO_ISP		5.24166666666667E-08
#define	LATENCIA_OBJETO_OPCIONES_ISP		7.86111111111111E-05
#define	LATENCIA_TRABAJO_TERMINADO_ISP		1.04805555555556E-07
#define	LATENCIA_TRABAJO_AGREGADO_ISP		1.04805555555556E-07

#define DELAY_BASE_P2P		6.33333333333333E-05
#define	LATENCIA_SOLICITUD_TAREA_P2P		3.20277777777778E-08
#define	LATENCIA_SOLICITUD_OBJETO_P2P		3.20277777777778E-08
#define	LATENCIA_OBJETO_OPCIONES_P2P		4.80555555555556E-05
#define	LATENCIA_TRABAJO_TERMINADO_P2P		6.40833333333333E-08
#define	LATENCIA_TRABAJO_AGREGADO_P2P		6.40833333333333E-08

//**********************************************************************************************
// //**********************************************************************************************
// //VALORES DE PRUEBA PARA CORREGIR EL DELAY:
// #define	TIME_GEN1								60
// #define	TIME_GEN2		                        1500
// #define	TIME_GEN3		                        1
// #define	TIME_PEER1		                        1150
// #define CTE_SCORES		                        2500
// #define	CTE_AGREGACION_PEERS                    3
// #define	CTE_AGREGACION_WSE	                    2
// #define	CTE_CONSENSO_WSE	                    1270
// #define	CTE_UPD_FREQ		                    600

// #define LATENCIA_BD_IMG		                    3000
// #define	LATENCIA_BD_TASK	                    50
// #define	LATENCIA_PEER_WSE	                    200000
// #define	LATENCIA_RED_P2P	                    35000

// #define DELAY_BASE_ISP							441000
// #define	LATENCIA_SOLICITUD_TAREA_ISP		    189
// #define	LATENCIA_SOLICITUD_OBJETO_ISP		    189
// #define	LATENCIA_OBJETO_OPCIONES_ISP		    283000
// #define	LATENCIA_TRABAJO_TERMINADO_ISP		    377
// #define	LATENCIA_TRABAJO_AGREGADO_ISP		    377

// #define DELAY_BASE_P2P		                    228000
// #define	LATENCIA_SOLICITUD_TAREA_P2P		    115
// #define	LATENCIA_SOLICITUD_OBJETO_P2P		    115
// #define	LATENCIA_OBJETO_OPCIONES_P2P		    173000
// #define	LATENCIA_TRABAJO_TERMINADO_P2P		    231
// #define	LATENCIA_TRABAJO_AGREGADO_P2P		    231

//~ //**********************************************************************************************
//**********************************************************************************************
//VALORES 0
//#define	TIME_GEN1								0
//#define	TIME_GEN2		                        0
//#define	TIME_GEN3		                        0
//#define	TIME_PEER1		                        0
//#define CTE_SCORES		                        0
//#define	CTE_AGREGACION_PEERS                    0
//#define	CTE_AGREGACION_WSE	                    0
//#define	CTE_CONSENSO_WSE	                    0
//#define	CTE_UPD_FREQ		                    0
//
//#define LATENCIA_BD_IMG		                    0
//#define	LATENCIA_BD_TASK	                    0
//#define	LATENCIA_PEER_WSE	                    0
//#define	LATENCIA_RED_P2P	                    0
//
//#define DELAY_BASE_ISP							10
//#define	LATENCIA_SOLICITUD_TAREA_ISP		    5
//#define	LATENCIA_SOLICITUD_OBJETO_ISP		    0
//#define	LATENCIA_OBJETO_OPCIONES_ISP		    0
//#define	LATENCIA_TRABAJO_TERMINADO_ISP		    0
//#define	LATENCIA_TRABAJO_AGREGADO_ISP		    0
//
//#define DELAY_BASE_P2P		                    0
//#define	LATENCIA_SOLICITUD_TAREA_P2P		    0
//#define	LATENCIA_SOLICITUD_OBJETO_P2P		    0
//#define	LATENCIA_OBJETO_OPCIONES_P2P		    0
//#define	LATENCIA_TRABAJO_TERMINADO_P2P		    0
//#define	LATENCIA_TRABAJO_AGREGADO_P2P		    0

//**********************************************************************************************


//---------------------------------------------------------------------------------------------------------------------------------------------
#define MAX_BUFFER 2048
#define ENTRY_SIZE 1
#define LCACHE_SIZE 1
#define ASSERT(x) assert(x)
#define MAX_SEQUENTIAL 5
//#define TIME_VALID 0 //TTL
#define OVERLOAD_THRESHOLD 25 //Max Load
// VALORES ORIGINALES:  #define DELTA_O 100 // Observer check
#define DELTA_O 15000000 // Observer check
//#define DELTA_O 3600 // Observer check - 4 horas


//#define DELTA_T 1 //Replicator Check period
//#define DELTA_T 50 //Replicator Check period
#define DELTA_T 1500 //Replicator Check period

//#define REP_STRATEGY 0  // 0: Leafset   1: Bubble    2: TLC
//#define PEER_SELECTION 0 // 0: Random   1: Zipf    2:static_debug
//#define CASE_RESPONSABLE 0 // variable para Leafset y Bubble 1:replicas contactan a WSE 0: replicas borran entradas con TTL vencido


#define QUERY_RATE_STRATEGY 0
// VALORES ORIGINALES:  #define QUERY_DELTA_T 10000 //CADA 10000 Unidades de Tiempo
// VALORES ORIGINALES:  #define QUERY_DELTA_Q 10000 //CADA 10000 QUERIES
#define QUERY_DELTA_T 10000 //CADA 10000 Unidades de Tiempo
#define QUERY_DELTA_Q 10000 //CADA 10000 QUERIES
// VALORES ORIGINALES:  #define MIN_TTL 100 //1 Hour
// VALORES ORIGINALES:  #define MAX_TTL 86400 //24 Hours
#define MIN_TTL 172800 //1 Hour
#define MAX_TTL 172800 //24 Hours
// VALORES ORIGINALES:  #define TTL_WSE 180 // 3min
#define TTL_WSE 180// 3min
#define TIMEOUT_Q 100 // Time out de las consultas

// VALORES ORIGINALES:  #define WSECACHESIZE 8000
#define WSECACHESIZE 8000
#define SIZE_ANSWER 1
#define STATIC_TIMEOUT 100 // cambiar por dinamico

#define PEER 411
#define USER 412
#define GEN_TAREA 413

//PASTRY CONF
#define BASE 16
#define BITS 160
#define B 4
#define D 10

// FLASH CROWDS
#define FLASH_CROWD false  // se utiliza o no el flash crowd
#define CROWD_TIME 5.0 // segundos con flash crowd
#define NORMAL_TIME 500.0 //segundos con tasa normal
// #define NORMAL_RATE 1000.0 // tasa de arribo sin flash crowd
#define CROWDED_RATE 5000.0 // tasa de arribo con flash crowd

/****************/
//SERVICIOS
//#define USER      500
//#define END_QUERY 501
//#define IS        502
//#define MERGE     503

//--DEBUGING
#define DEBUG

#ifdef DEBUG
#define dout cout
#else
#define dout if(false) cout
#endif

// class Mje_WSE_BD
// {
// public:
//    int id;
//    Mje_WSE_BD (int _id)
//    {
// 	   id = _id;
//    }
// };

//~ class Rtas_Peers {		//Forwarding para poder usarlas en los mensajes de la BD.
//~ public:
//~ Rtas_Peers();
//~ Rtas_Peers(int _id_img, double _ttl, int _esp, int _prioridad_tarea, list<int>_opciones_originales, double _tiempo_inicio, double _tiempo_generacion);
//~ void copy(Rtas_Peers* &);
//~ };



#endif

