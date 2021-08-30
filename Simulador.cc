/**/

#include "glob.h"
#include "generador/gen_rnd.h"
#include "Observer.h"
#include "applicationLayer/TlcProtocol.h"
#include "p2pLayer/PastryProtocol.h"
#include "p2pLayer/StateBuilder.h"
#include "transportLayer/redtubo.h"
#include "wse/WSE.h"
#include "isp/ISP.h"
#include "applicationLayer/ClientTLCWSE.h"
#include "applicationLayer/Voluntario.h"
#include "BD/BD.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <locale>

class mySystem: public process
{

private:

    double simLen;
    char name[128];

    //Number of Nodes
    int nodes;

    // Cache Size
    int cacheSize, localCacheSize, lcacheSize;

    // Observer
    handle<Observer> obs;

    // WSE
    handle<WSE> wse;
    //BD
    handle<BD> bd;

    // ISP
    handle<ISP> *isp;

    //Peers with our solution
    handle<TlcProtocol> *Peers;
    handle<ClientTlcWse> *clientes_wse_Peers;
    handle<Voluntario> *voluntarios;

    // Network to state builder
    vector<vector<handle<PastryProtocol>*>> pastryNetwork;
    vector<handle<PastryProtocol>*> pastryNetwork_aux;

    //Queries
    int totalQueries, sentQueries;

    int MAX_DEBIT, Q_PER_SEC;

    //Comandos a ejecutar (DAG)
    char traces[2048];

    //Generador de tasa de llegada
    handle<Gen_rnd> generator;

    //Protocolo Red Tub
    handle<RedTubo> *transport;

    // Pastry's parameters
    int leafset;

    //Endinf
    int *ends;

    //int Nuser;//,client_wse_cte,cte_atencion,interArrT;

    //~ int Rep_Strategy;
    //~ int Case_Responsable;
    //~ int Peer_Selection;
    int normal_rate;
    int Peers_envio;
    double Thao;
    double iteracion_sim;
    double limite_TTL, umbral_votacion;
    int AGREGACION_PEERS;
    int flag_escenario;
    double cte_traffJam_isp;
    double cte_traffJam_p2p;
    int flagArquitectura;
    int caso;
    bool flagDisponibles = false;
    bool flagHistorias = false;
    double max_work_time;

public:
    mySystem(	const string &_name,
                double _sl,
                int _totalQueries,
                int _cacheSize,
                int _localCacheSize,
                int _lcacheSize,
                int _nodes,
                char *_traces,
                int _leafset,
                double _Thao,
                int _Peers_envio,
                int _iteracion_sim,
                int _normal_rate,
                double _limite_TTL,
                double _umbral_votacion,
                int _AGREGACION_PEERS,
                int _flag_escenario,
                double _cte_traffJam_isp,
                double _cte_traffJam_p2p,
                int _flagArquitectura,
                int _caso,
                int _flagDisponibles,
                double _max_work_time,
                int _flagHistorias
            ) : process( _name )
    {
        simLen         	= _sl;
        Thao           	= _Thao;
        totalQueries   	= _totalQueries;
        sentQueries    	= 0;
        cacheSize      	= _cacheSize;
        localCacheSize 	= _localCacheSize;
        lcacheSize     	= _lcacheSize;
        nodes          	= _nodes;
        leafset	     	= _leafset;
        strcpy(traces, 	_traces);
        //~ ends           	= new int[nodes+1];
        Thao				= _Thao;
        //~ Rep_Strategy   	= _RS;
        //~ Peer_Selection 	= _PS;
        //~ Case_Responsable 	= _CS;
        Peers_envio		= _Peers_envio;
        iteracion_sim		= _iteracion_sim;
        normal_rate		= _normal_rate;
        limite_TTL		= _limite_TTL;
        umbral_votacion	= _umbral_votacion;
        AGREGACION_PEERS = _AGREGACION_PEERS;
        flag_escenario = _flag_escenario;
        cte_traffJam_isp = _cte_traffJam_isp;
        cte_traffJam_p2p = _cte_traffJam_p2p;
        flagArquitectura = _flagArquitectura;
        caso = _caso;
        if (_flagDisponibles == 1) flagDisponibles=true;
        max_work_time = _max_work_time;
        if (_flagHistorias == 1) flagHistorias=true;
    }

protected:

    void inner_body( void );

};


void mySystem::inner_body( void )
{

    int peso_respuestas_expertos = 3;
    //~ bool flagDisponibles = true;	//Paránetro de entrada.
    int habilidad = 0;
    char name[128];
    srand(std::time(NULL));

    //Definición de múltiples redes. //------------------------------------------
    int nro_redes = 1;
    vector<int> NP;
    vector<int> lim_inf;
    vector<int> lim_sup;
    if (flag_escenario != 4)
    {
        nro_redes = 1;
    }
    assert(( (nro_redes == 1) && (flag_escenario < 4)  )  ||  ( (nro_redes > 1) && (flag_escenario == 4)  ));
    for (int j = 0; j < nro_redes; j++)
    {
        NP.push_back(nodes + 100 * j);
        if (j == 0)
        {
            lim_inf.push_back(0);
            lim_sup.push_back(NP[j]);
        }
        else
        {
            lim_inf.push_back(lim_inf[j - 1] + NP[j - 1]);
            lim_sup.push_back(lim_inf[j] + NP[j]);
        }
        //cout << "NP: " <<NP[j] << ". lim_inf: " <<lim_inf[j] << ". lim_sup: "<< lim_sup[j] << endl;
    }//----------------------------------------------------------------------------
    
    
    int total_NP = accumulate(NP.begin(), NP.end(), 0);
    nodes = total_NP;
    ends = new int[total_NP + 1];
    for ( int i = 0; i < total_NP + 1; i++)
    {
        ends[i] = 0;
    }

    // WSE
    wse = new WSE("WSE", &bd, total_NP, NP, ends, Peers_envio, limite_TTL, totalQueries, umbral_votacion, nro_redes, lim_inf, lim_sup, flagArquitectura, cacheSize,caso,flagDisponibles,flagHistorias);
    wse->set_WSE_obj(&wse);
    wse->AGREGACION_PEERS = AGREGACION_PEERS;

    // ISP
    isp = new handle<ISP>[nro_redes];
    for (int j = 0; j < nro_redes; j++)
    {
        isp[j] = new ISP("ISP" + to_string(j), NP[j], j, cte_traffJam_isp, flagArquitectura);
        wse->isp.push_back(&isp[j]);
        isp[j]->wse = &wse;
    }


    // Observer stores statistics
    obs = new Observer(total_NP, nro_redes, lim_inf, lim_sup, "OBSERVER", totalQueries, cacheSize, Peers_envio, normal_rate,
                       iteracion_sim, localCacheSize, lcacheSize, limite_TTL,	umbral_votacion, flag_escenario,
                       cte_traffJam_isp, cte_traffJam_p2p, flagArquitectura, caso, peso_respuestas_expertos, flagDisponibles,max_work_time);
    obs->wse = &wse;
    wse->set_handle_obs(&obs);
    for (int j = 0; j < nro_redes; j++)
    {
        isp[j]->set_handle_obs(&obs);
    }

    // BD
    bd = new BD("BD", totalQueries, &wse, nodes, nro_redes);

    bd->activate();
    for (int j = 0; j < nro_redes; j++)
    {
        isp[j]->activate();
    }

    obs->activate();
    wse->activate();
    wse->cacheSize = cacheSize;

    // Query generator
    generator = new Gen_rnd("GENERATOR", traces, &totalQueries, total_NP,
                            &obs, ends,&wse, //Peer_Selection, 
                            normal_rate,limite_TTL
                           );


    transport = new handle<RedTubo> [nro_redes];
    // Initialize Red Tubo
    if (cacheSize != 0)
    {
        for (int j = 0; j < nro_redes; j++)
        {
            transport[j] = new RedTubo("RED" + to_string(j), lim_inf[j], j, cte_traffJam_p2p);
            transport[j]->set_handle_obs(&obs);
            transport[j]->activate();
        }
    }


    //Initialize Array Tcl Peers
    clientes_wse_Peers = new handle<ClientTlcWse>[nodes];
    Peers = new handle<TlcProtocol>[nodes];
    handle<PastryProtocol> *pastryPeers = new handle<PastryProtocol>[nodes];
    voluntarios = new handle<Voluntario>[nodes];

    Hash *h ;

    handle<ClientTlcWse> *c;
    handle<Replicator> *r;
    vector<StateBuilder *> sb;

    int i = 0;
    for ( int j = 0; j < nro_redes; j++ )
    {
        //Inicitalize each layer on each peer
        for ( int k = 0; k < NP[j]; k++ )
        {
            //~ cout << i << endl;
            sprintf(name, "Pastry_%d_%d", i, j);
            // create a random nodeID
            h = new Hash();
            BIGNUM *nodeid = h->CreateRandomKey();
            obs->codigos[nodeid] = i;
            obs->codigos2[i] = nodeid;
            //~ cout << "Nodo: " << i << ". Código: " << BN_bn2hex(nodeid) << endl;
            if (cacheSize != 0)
            {
                pastryPeers[i] = new PastryProtocol(name, i, nodeid, leafset, &obs);
                pastryPeers[i]->set_H_obj( &pastryPeers[i] );
                pastryPeers[i]->addLowerLayer(&transport[j]);
                pastryPeers[i]->activate();
            }

            sprintf(name, "ClientWSE_%d_%d", i, j);
            c = new handle<ClientTlcWse>;
            habilidad = ((rand() % 10)) > 8;
            (*c) = new ClientTlcWse(name, &wse,
                                    habilidad,
                                    umbral_votacion,
                                    nodeid,
                                    &obs,
                                    totalQueries,
                                    ends,
                                    j,
                                    flagArquitectura,
                                    total_NP,
                                    caso
                                   );
            (*c)->set_ClientWSE_obj(c);
            (*c)->set_isp(&isp[j]);
            (*c)->AGREGACION_PEERS = AGREGACION_PEERS;
            sprintf(name, "Voluntario_%d", i);
            voluntarios[i] = new Voluntario(
                name,
                habilidad,
                &obs,
                c,
                j,
                i,
                flag_escenario,
                peso_respuestas_expertos,
                flagDisponibles,
                max_work_time
            );
            (*c)->set_voluntario(&voluntarios[i]);
            if (cacheSize != 0)
            {
                sprintf(name, "Replicator_%d_%d", i, j);
                r = new handle<Replicator>;
                (*r) = new Replicator(name);


                sprintf(name, "TLC_%d_%d", i, j);
                Peers[i] = new TlcProtocol( name,
                                            nodeid,
                                            cacheSize,
                                            localCacheSize,
                                            lcacheSize,
                                            c,
                                            &obs,
                                            r,
                                            totalQueries,
                                            ends,
                                            nodes,
                                            Thao,
                                            //~ Rep_Strategy,
                                            //~ Case_Responsable,
                                            j
                                          );

                Peers[i]->set_H_obj(&Peers[i]);
                Peers[i]->addLowerLayer(&pastryPeers[i]);
                Peers[i]->set_isp(&isp[j]);

                (*c)->set_tlc(&Peers[i]);
                (*c)->activate();
                (*r)->set_tlc(&Peers[i]);
                (*r)->activate();
                //PRUEBO PARA TENER SIEMPRE LOS HANDLES
                clientes_wse_Peers[i] = (*c);
                (*(clientes_wse_Peers[i]->I_obj))->id_peer_client = i;

            }
            else
            {
                clientes_wse_Peers[i] = (*c);
                (*(clientes_wse_Peers[i]->I_obj))->id_peer_client = i;
            }



            //~ generator->add_peer(&Peers[i]);
            //if (cacheSize!=0) {
            //	wse->add_peer(&Peers[i]);
            //	Peers[i]->activate();
            //	//PRUEBO PARA TENER SIEMPRE LOS HANDLES
            //	wse->add_peer(&clientes_wse_Peers[i]);
            //}else{
            //	wse->add_peer(&clientes_wse_Peers[i]);
            //}

            wse->add_peer(&clientes_wse_Peers[i]);

            isp[j]->add_peer(&clientes_wse_Peers[i]);

            // STATE BUILDER
            if (cacheSize != 0)
            {
                pastryNetwork_aux.push_back(&pastryPeers[i]);
            }
            voluntarios[i]->activate();
            i++;
        }

        if (cacheSize != 0)
        {
            pastryNetwork.push_back(pastryNetwork_aux);
            //~ StateBuilder* sb = new StateBuilder(pastryNetwork[j], leafset);
            sb.push_back(new StateBuilder(pastryNetwork[j], leafset));
            sb[j]->execute();
            pastryNetwork_aux.clear();
        }

    }
    //~ cout << "DEBUG 3 "<< endl;
    generator->activate();



    //~ cout << "Simulador: Comienza SIMULACION "<<endl; fflush(stdout);
    hold( simLen ); // Stops when the simulation time is achieve
    //~ cout << "Simulador: Simulacion FINALIZADA "<<endl; fflush(stdout);



    if (cacheSize != 0)
    {
        for ( int i = 0 ; i < nodes ; i ++)
        {
            pastryPeers[i]->freePastryPeers();
            Peers[i]->freeTlc();
        }
    }

    generator->freeGen_rnd();

    //pastryNetwork.clear();
    //free(sb);

    // print statistics all simulation
    end_simulation( );
}


int main( int argc, char *argv[] )
{
    //setlocale (LC_ALL, "C");
    setlocale (LC_ALL, "en_US.UTF-8");
    setlocale (LC_NUMERIC, "de_DE.UTF-8");

    char traces[2048];

    //===========================================================================================================
    //Lectura de parámetros
    //===========================================================================================================
    int totalQueries  = atoi(argv[1]);  //Total queries to process
    int cacheSize     = atoi(argv[2]);       //Size of Rcache
    //ASSERT( cacheSize != 0 );
    int localCacheSize     = atoi(argv[3]);       //Size in Local Rcache
    int lcacheSize     = atoi(argv[4]);       //Size of LCache
    int nodes  = atoi(argv[5]);              // Number of nodes in the system
    int leafset = atoi(argv[6]);		// Leafset size
    strcpy( traces, argv[7]);

    double Thao = atof(argv[8]);
    //~ int Rep_Strategy = atoi(argv[9]);
    //~ int Peer_Selection = atoi(argv[9]);
    //~ int Case_Responsable = atoi(argv[11]);
    int Peers_envio = atoi(argv[9]);
    int iteracion_sim = atoi(argv[10]);
    int normal_rate = atoi(argv[11]);
    double limite_TTL = atof(argv[12]);
    double umbral_votacion = atof(argv[13]);
    int caso = atoi(argv[14]);
    int AGREGACION_PEERS = 1;
    if (caso != 2)
    {
        AGREGACION_PEERS = 0;
    }
    if (caso == 0)
    {
        cacheSize = 0;
        localCacheSize = 0;
        lcacheSize = 0;
    }
    int flag_escenario = atoi(argv[15]);
    double cte_traffJam_isp;
    double cte_traffJam_p2p;
    cte_traffJam_isp = atoi(argv[16]);
    cte_traffJam_p2p = atoi(argv[17]);

    int flagArquitectura = atoi(argv[18]); //0: propia, 1: Ofli, 2: Morteza, 3: Kobayashi, 4: traffic Jam
    if (flagArquitectura != 4)
    {
        cte_traffJam_isp = 1;
        cte_traffJam_p2p = 1;
    }

	int flagDisponibles = atoi(argv[19]); 
	double max_work_time = atof(argv[20]);
	int flagHistorias = atoi(argv[21]);
	
    //~ cout << "AGREGACION PEERS" << AGREGACION_PEERS << endl;
    //~ cout << "Cache Size" << cacheSize << endl;
    //~ cout << "Total Queries " << totalQueries << endl;
    //~ cout << "Cache Size " << cacheSize << endl;
    //~ cout << "Local Cache Size " << localCacheSize << endl;
    //~ cout << "LCache Size " << lcacheSize << endl;
    //~ cout << "Number Nodes " << nodes << endl;
    //~ cout << "Leafset Size " << leafset << endl;									//NO USADO
    //~ cout << "Traces " << traces << endl;
    //~ cout << "Number of user (no usado)" << nodes << endl; 						//NO USADO
    //~ cout << "Thao " << Thao << endl;
    //~ cout << "H " << Peers_envio << endl;
    //~ cout << "iteracion_sim " << iteracion_sim << endl;
    //~ cout << "normal_rate " << normal_rate << endl;
    //~ cout << "Limite TTL: " << limite_TTL << endl;
    //~ cout << "Constante hold Client_WSE: " << client_wse_cte << endl; 			//NO USADO (parámetro fijo con límites de aleatoriedad)
    //~ cout << "Constante de atención en los peers: " << cte_atencion << endl; 	//NO USADO (parámetro fijo con límites de aleatoriedad)
    //~ cout << "Umbral de consenso: " << umbral_votacion << endl; 					//NO USADO (fijo)



    sched_t schedType = SQSDLL;


    //simulation::instance( )->begin_simulation( new sqsPrio( ) );

    //Línea para mostrar lo que va ocurriendo con todos los procesos del simulador en el transcurso de la simulación:
    //~ process::getLogger()->setLevel( 0 );

    simulation::instance( )->begin_simulation( makeSqs( schedType) );


    handle<mySystem> system( new mySystem("System main", 10000000e100,
                                          totalQueries,
                                          cacheSize,
                                          localCacheSize,
                                          lcacheSize,
                                          nodes,
                                          traces,
                                          leafset,
                                          Thao,
                                          //~ Rep_Strategy,
                                          //~ Peer_Selection,
                                          //~ Case_Responsable,
                                          Peers_envio,
                                          iteracion_sim,
                                          normal_rate,
                                          limite_TTL,
                                          umbral_votacion,
                                          AGREGACION_PEERS,
                                          flag_escenario,
                                          cte_traffJam_isp,
                                          cte_traffJam_p2p,
                                          flagArquitectura,
                                          caso,
                                          flagDisponibles,
                                          max_work_time,
                                          flagHistorias
                                         ) );

    system->activate( );

    simulation::instance( )->run( );

    simulation::instance( )->end_simulation( );

    //cout <<endl<< "done!" << endl;
    return 0;
}

