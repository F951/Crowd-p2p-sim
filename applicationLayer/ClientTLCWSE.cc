#include "ClientTLCWSE.h"
#include "TlcProtocol.h"

#include <iostream>
#include <fstream>
#include <ctime>

void ClientTlcWse::inner_body(void)
{
    int debug_cliente = 1;
    int debug_DL = 0;
    
    int sentQueries = 0;
    
    srand (static_cast <unsigned> (this->time()));
    
    vector <int> tareas_repetidas;
    H = (*observer)->Peers_envio;
        
    while(1)
    {
        while ( flag_terminar == 1 ) 			//Final de simulación.
        {
            //~ if (debug_cliente) {cout <<"ClientTlcWSE ================= TERMINAR - Passivate"<<endl;}
            passivate();
        }

        if(ClientWSE::queue_in_wse2.empty())
        {
            //~ if (debug_cliente) {cout <<"ClientTlcWSE ================= queue2 empty - Passivate"<<endl;}
            passivate();
        }
        //~ debug(time(), "Activate and check for messages");
        ASSERT(!ClientWSE::queue_in_wse2.empty());

        MessageServer *temp;
        temp = ClientWSE::queue_in_wse2.front();		
        ClientWSE::queue_in_wse2.pop_front();
        
        debug(time(), "New message from " + to_string(temp->getSrc()) + " FLAG_DL: " + to_string(flagDL));
        //~ temp->print();
        
        fflush(stdout);
        cout.flush();
        
        if (temp->getType()==SEND_DL_MODEL){
			receiveDLmodel(temp);
		} else if (flagArquitectura==2 && flagDL == 1 && ( 
				(temp->getType()==SEND_OBJ_OWNER && caso==0) ||
				(temp->getType()==SEND_OBJ && caso!=0)
				) 	)
		{
			DLprocessing(temp);
		}else{
			switch (caso){
				case 0:
				{
					baselineAlgorithm(temp, sentQueries);
					break;
				}
				case 1:
				{
					proposal1Algorithm(temp, sentQueries);
					break;
				}
				case 2:
				{
					proposal2Algorithm(temp, sentQueries);
					break;
				}	
			}
		}
        
    }

}

long int ClientTlcWse::getTTL(BIGNUM *b)
{
    long int query_TTL;
    long int last_TTL;
    Entry *old = (*tlc)->RCACHE->check(b);
    if(old == NULL)
    {
        query_TTL = MIN_TTL;
    }
    else
    {
        last_TTL = old->getTimeOut();
        query_TTL = ClientTlcWse::getTimeIncremental(last_TTL);

    }
    return query_TTL;
}

long int ClientTlcWse::getTimeIncremental(long int last_TTL)
{
    //   cout << " incremental" << endl;
    // TODO: Search function!!!!
    last_TTL = this->time() + last_TTL + STATIC_TIMEOUT;
    if((last_TTL - this->time()) > MAX_TTL)
        last_TTL = this->time() + MAX_TTL;
    return last_TTL;
}

void ClientTlcWse::set_tlc(handle<TlcProtocol> *t)
{
    tlc = t;
    (*I_obj)->set_tlc_of_client(tlc);
}

void ClientTlcWse::sendWSEQuery2(Query *q)
{
    MessageServer *wseQuery = new MessageServer(ASK_OBJ, q->getTaskID(), 0, //hashValue,
                               0, serverID, id_peer_client, 0, 0);
                               
	sendMessage2Server(wseQuery);
}

void ClientTlcWse::printLista(std::list<int> &lista, const char *nombre, int indice)
{
    std::list<int>::iterator it_lista;
    cout << "#" << indice << ". ==============Lista: " << nombre << "\n";
    cout << "#" << indice << ". ";
    for (it_lista = lista.begin(); it_lista != lista.end(); ++it_lista)
    {
        //~ std::cout << ' ' << *it_lista;
        cout << ' ' << *it_lista;
    }
    cout << "\n#" << indice << ". ==============\n";
}


void ClientTlcWse::diferencias_en_listas(list<int> &lista1, list<int> &lista2, int indice)
{
    vector <int> diferencias;
    list<int>::iterator it_prueba1, it_prueba2;
    for (it_prueba1 = lista1.begin(); it_prueba1 != lista1.end(); ++it_prueba1)
    {
        it_prueba2 = find(lista2.begin(), lista2.end(), *it_prueba1);
        if(it_prueba2 == lista2.end())
        {
            diferencias.push_back(*it_prueba1);
        }
    }
    (*wse)->print_vec(diferencias, "diferencias", indice);
}

void ClientTlcWse::debug(double time, string msg  )
{
    if (0) cout << "[" << time << "][" << name() << "][ " << msg << " ]" << endl;
}

void ClientTlcWse::print_msg_model(MessageWSE *temp)
{
	cout << "(Cliente) " << name() << ": Msg WSE MODEL"  <<  endl;
	cout << "(Cliente) " << name() << ": Msg src: " << (*(temp->src))->name() << endl;
	cout << "(Cliente) " << name() << ": Msg key: " << temp->getKey() << endl;
	cout << "(Cliente) " << name() << ": Msg type: " << temp->getType() << endl;
	cout << "(Cliente) " << name() << ": Msg temp->TTL_tarea: " << temp->TTL_tarea << endl;
	cout << "(Cliente) " << name() << ": Msg temp->tag: " << temp->tag << endl;
	cout << "(Cliente) " << name() << ": Msg temp->id_imagen: " << temp->rta->id_img << endl;
	cout << "(Cliente) " << name() << ": Msg temp->rta->id_tarea: " << temp->rta->id_tarea << endl;
}

void ClientTlcWse::sendMessage2Server(MessageServer *m)
{
    //~ debug(time(), "Send message 2 server");
    m->set_Src(id_peer_client);
    m->set_Dest( (*wse)->total_NP );

    (*isp)->add_message2(m);
    if( (*isp)->idle() && !(*isp)->get_busy() )
    {
        (*isp)->activateAfter( current() );
    }
}

void ClientTlcWse::forward2volunteer(MessageServer *m)
{
    (*voluntario)->add_request2(m);
    if( (*voluntario)->idle() && !(*voluntario)->get_busy() )
    {
        (*voluntario)->activateAfter( current() );
    }
}

void ClientTlcWse::terminar(  )
    {
		//~ cout << "Terminar " << name()  << endl;
        flag_terminar = 1;
        (*tlc)->terminar();
        //~ (*tlc)->activateAfter(current());
    }

void ClientTlcWse::baselineAlgorithm(MessageServer * temp , int sentQueries){
	int indice;
    Query *q;
	std::stringstream str;
    BIGNUM *hashValue ;
    Hash *h;
	//------------------------------------------------------------------------------------------BASELINE
	switch (temp->getType())
	{
		case SEND_TASK:
		{
			debug(time(), "Prepare query to Server, task:" + to_string(temp->getTaskID()));
			(*observer)->N_tag0_client[red_nro]++;
			(*observer)->Time_tag0_client[red_nro] += this->time();
			temp->set_type(ASK_OBJ);
			sendMessage2Server(temp);
			break;
		}
		
		case SEND_OBJ_OWNER:
		{
			debug(time(), "Received object, send to volunteer, task:" + to_string(temp->getTaskID()));
			(*observer)->N_tag2_client[red_nro]++;
			(*observer)->Time_tag2_client[red_nro] += this->time();
			temp->set_type(SEND_OBJ);
			forward2volunteer(temp);
			break;
		}
		
		case SEND_RESULT:
		{
			debug(time(), "Received answer from volunteer, send to server, task:" + to_string(temp->getTaskID()));
			//Tomar respuesta del voluntario y enviarla al server.
			sendMessage2Server(temp);
			break;
		}
		
		default:
		{
		
			break;
		}
	}
	//------------------------------------------------------------------------------------------BASELINE
}

void ClientTlcWse::proposal1Algorithm(MessageServer * temp , int sentQueries){
	int indice;
    Query *q;
	std::stringstream str;
    BIGNUM *hashValue ;
    Hash *h;
	//------------------------------------------------------------------------------------------PROPOSAL 1
	switch (temp->getType())
	{
		case SEND_TASK:
		{
			debug(time(), "Prepare query to net, task:" + to_string(temp->getTaskID()));
			(*observer)->N_tag0_client[red_nro]++;
			(*observer)->Time_tag0_client[red_nro] += this->time();
			//~ hashValue = h->GenerateKey((char*)(to_string(temp->getImgID())).c_str());
			char buffer[20]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
			string aux=(to_string(temp->getImgID())).c_str();
			strcpy(buffer, aux.c_str());
			hashValue = h->GenerateKey(&buffer[0]);

			q = new Query(sentQueries, temp->getTaskID(), temp->getImgID(), temp->getOptionNr(), hashValue,  this->time() );
			sentQueries++;
			(*tlc)->add_query( q );
			if ((*tlc)->idle() && !(*tlc)->get_busy())
				(*tlc)->activateAfter(current());
			debug(time(), "Query sent, task:" + to_string(temp->getTaskID()));					
			break;
		}

		case SEND_OBJ_OWNER:
		{
			debug(time(), "Received object, send to volunteer, task:" + to_string(temp->getTaskID()));
			(*observer)->N_tag1_client[red_nro]++;
			(*observer)->Time_tag1_client[red_nro]+=this->time();
			//~ hashValue = h->GenerateKey((char*)(to_string(temp->getImgID())).c_str());
			char buffer[20]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
			string aux=(to_string(temp->getImgID())).c_str();
			strcpy(buffer, aux.c_str());
			hashValue = h->GenerateKey(&buffer[0]);
			Entry *newEntry = new Entry(this->time(), temp->getTaskID(), temp->getImgID(), temp->getOptionNr(), hashValue,  0, ENTRY_SIZE, temp->getTTL());
			TlcMessage *tm = new TlcMessage(TLC_WSE, (*tlc)->msgID, newEntry);
			(*tlc)->msgID=(*tlc)->msgID+1;
			(*tlc)->add_message(tm);
			if ((*tlc)->idle() && !(*tlc)->get_busy())
				(*tlc)->activateAfter(current());
			break;
			//~ debug(time(), "delete message");					
			//~ delete temp;
			//~ debug(time(), "delete message OK");					
		}
		
		case SEND_OBJ:
		{
			debug(time(), "Received object, send to volunteer, task:" + to_string(temp->getTaskID()));
			(*observer)->N_tag2_client[red_nro]++;
			(*observer)->Time_tag2_client[red_nro] += this->time();
			forward2volunteer(temp);
			break;           
		}

		case SEND_RESULT:
		{
			debug(time(), "Received answer from volunteer, send to server, task:" + to_string(temp->getTaskID()));
			//Tomar respuesta del voluntario y enviarla al server.
			sendMessage2Server(temp);
			break;
		}

		default:
		{
			break;
		}
	}
	//------------------------------------------------------------------------------------------PROPOSAL 1
}

void ClientTlcWse::proposal2Algorithm(MessageServer * temp , int sentQueries){
	int indice;
    Query *q;
	std::stringstream str;
    BIGNUM *hashValue ;
    Hash *h;
	//------------------------------------------------------------------------------------------PROPOSAL 2
	switch (temp->getType())
	{
		case SEND_TASK:
		{
			debug(time(), "Prepare query to net, task:" + to_string(temp->getTaskID()));
			(*observer)->N_tag0_client[red_nro]++;
			(*observer)->Time_tag0_client[red_nro] += this->time();
			//~ hashValue = h->GenerateKey((char*)(to_string(temp->getImgID())).c_str());
			char buffer[20]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
			string aux=(to_string(temp->getImgID())).c_str();
			strcpy(buffer, aux.c_str());
			hashValue = h->GenerateKey(&buffer[0]);
			q = new Query(sentQueries, temp->getTaskID(), temp->getImgID(), temp->getOptionNr(), hashValue,  this->time() );
			sentQueries++;
			(*tlc)->add_query( q );
			if ((*tlc)->idle() && !(*tlc)->get_busy())
				(*tlc)->activateAfter(current());
			debug(time(), "Query sent, task:" + to_string(temp->getTaskID()));				
			//~ delete temp;	
			break;
		}

		case SEND_OBJ_OWNER:
		{
			debug(time(), "Received object, send to peer, task:" + to_string(temp->getTaskID()));
			debug(time(), "Task:" + to_string(temp->getTaskID())+", responsable peer. Aggregation Peer:" + to_string(temp->getAggregationPeerID()));
			(*observer)->N_tag1_client[red_nro]++;
			(*observer)->Time_tag1_client[red_nro]+=this->time();
			//~ hashValue = h->GenerateKey((char*)(to_string(temp->getImgID())).c_str());
			char buffer[20]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
			string aux=(to_string(temp->getImgID())).c_str();
			strcpy(buffer, aux.c_str());
			hashValue = h->GenerateKey(&buffer[0]);
			Entry *newEntry = new Entry(this->time(), temp->getTaskID(), temp->getImgID(), temp->getOptionNr(), hashValue,  0, ENTRY_SIZE, temp->getTTL(),temp->getResponsableAgregacion());
			TlcMessage *tm = new TlcMessage(TLC_WSE, (*tlc)->msgID, newEntry);
			(*tlc)->msgID=(*tlc)->msgID+1;
			(*tlc)->add_message(tm);
			if ((*tlc)->idle() && !(*tlc)->get_busy())
				(*tlc)->activateAfter(current());
			//~ debug(time(), "delete message");					
			//~ delete temp;
			//~ debug(time(), "delete message OK");		
			break;
		}
		
		case SEND_OBJ:
		{
			debug(time(), "Received object, send to volunteer, task:" + to_string(temp->getTaskID()));
			debug(time(), "Task:" + to_string(temp->getTaskID())+", Aggregation Peer:" + to_string(temp->getAggregationPeerID()));
			(*observer)->N_tag2_client[red_nro]++;
			(*observer)->Time_tag2_client[red_nro] += this->time();
			forward2volunteer(temp);
			break;           
		}

		case SEND_RESULT:
		{
			debug(time(), "Received answer from volunteer, send to aggregation responsible, task:" + to_string(temp->getTaskID()));
			//Enviar respuesta al responsable de agregación.			
			hashValue = h->GenerateKey((char*)(to_string(temp->getImgID())).c_str());
			Entry *newEntry = new Entry(this->time(), temp->getTaskID(), temp->getImgID(), temp->getOptionNr(), hashValue,  0, ENTRY_SIZE, temp->getTTL());
			TlcMessage *tm = new TlcMessage(TLC_SEND_RESP, (*tlc)->msgID, (*(*tlc)->pastry)->ne, temp->getResponsableAgregacion(),temp->getOpciones());
			tm->setEntry(newEntry);
        	(*tlc)->msgID=(*tlc)->msgID+1;
			(*tlc)->add_message(tm);
			if ((*tlc)->idle() && !(*tlc)->get_busy())
				(*tlc)->activateAfter(current());
			//~ delete temp;
        	break;
			}
		
		case AGGREGATE_TASK:
		{
			debug(time(), "Received answer to AGGREGATE");
			(*observer)->N_tag3_client[red_nro]++;
			(*observer)->Time_tag3_client[red_nro]+=this->time();
			(*wse)->consultas_totales++;
			(*observer)->contador_recibidas[red_nro]=consultas_totales;
			
			int indice = temp->getTaskID();
			
			if (activeTasks.count(indice) != 0) {	//La tarea está en "activeTasks"
				debug(time(), "Task already in activeTasks");
			}else{
				insert_task(temp, time(), red_nro	);
				debug(time(), "Task inserted");
			}
			debug(time(), "Opciones que llegan al peer de agregación");
			//~ temp->print_opciones();
			activeTasks[indice]->addRecibidas();
			
			
			//Prueba sin control por TTL vencido.
			activeTasks[indice]->accumulateAnswers(temp);
			if ( allAnswersProp2(indice) )						//calculate consensus if received == H or 2H
			{
				debug(time(), "All answers received ");
				calculateConsensus(indice);
				sendAggregatedAnswers(indice);
				activeTasks[indice]->resetVotingList();
			}

			break;
		}

		default:
		{
			break;
		}
	}
	//------------------------------------------------------------------------------------------PROPOSAL 2
}


bool ClientTlcWse::TTLterminated(int indice){
	if (activeTasksStatus[indice]==1)
		return true;
	else
		return false;
		
}


void ClientTlcWse::insert_task(MessageServer *mS, double time, int _red_asignada)
{
    debug(time, " -----------------> Insert task: " + to_string(mS->getTaskID()) + ". activeTasks.size(): " + to_string(activeTasks.size()) );
    debug(time, " -----------------> Insert task. activeTasks.size(): " + to_string(activeTasks.size()));

    //~ showActiveTasks();
    Task *task1 = new Task(mS->getTaskID(), mS->getImgID(), mS->getTTL(), mS->getOptionNr(), mS->getGenT(), time, _red_asignada);	//, mS->getGenT());
    if (mS->getTaskID() != 0)
    {
        assert(activeTasks.count(mS->getTaskID()) == 0);
    }
    activeTasks[mS->getTaskID()] = task1;						// Inserto la tarea en en_proceso.
    
    activeTasksStatus[mS->getTaskID()] = 0;						//Tasks results are not sent to server yet.
}

void ClientTlcWse::calculateConsensus(int indice)
{
    debug(time(), "Calculate consensus ");
    activeTasks[indice]->calculateConsensus(umbral_votacion, H);
    debug(time(), "Task " + to_string(indice) + ", Consensus " + to_string(activeTasks[indice]->getConsenso()));
    busy = true;
    hold(CTE_CONSENSO_WSE);
    busy = false;
    (*observer)->t_consenso_server += CTE_CONSENSO_WSE;
}


void ClientTlcWse::sendAggregatedAnswers(int indice){
	
	MessageServer *mS = new MessageServer(SEND_RESULT, indice, activeTasks[indice]->getImgID(), //hashValue,
                               activeTasks[indice]->getTTL(), serverID, id_peer_client, activeTasks[indice]->getOptionNr(), 0);
	
	mS->set_votes(activeTasks[indice]->getVotacion());
	mS->set_consensus(activeTasks[indice]->getConsenso());
	debug(time(), "SEND ANSWERS TO SERVER ");
	sendMessage2Server(mS);
    
}

bool ClientTlcWse::validTTL(int indice)
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

bool ClientTlcWse::allAnswers(int indice)
{
    bool allAnswersHere;
    debug(time(), "allAnswers?  task->getRecibidas=" + to_string(activeTasks[indice]->getRecibidas()) + ", H=" + to_string(H));
    if ( activeTasks[indice]->getRecibidas() >= H)
    {
        allAnswersHere = true;
    }
    else
    {
        allAnswersHere = false;
    }
    return allAnswersHere;
}

bool ClientTlcWse::allAnswersProp2(int indice)
{
    bool allAnswersHere;
    debug(time(), "allAnswers?  task->getRecibidas=" + to_string(activeTasks[indice]->getRecibidas()) + ", H=" + to_string(H));
    if ( activeTasks[indice]->getRecibidas() == H || activeTasks[indice]->getRecibidas() == 2*H)
    {
        allAnswersHere = true;
    }
    else
    {
        allAnswersHere = false;
    }
    return allAnswersHere;
}

void ClientTlcWse::DLprocessing(MessageServer* temp)
{
	//~ assert(0);
	double LATENCIA_CLIENTE_PROC_AUTO = 0.05 / 3600;		//Para que sea en horas.
	//Simulo que el trabajo se hace con DL, y el resultado se envía al servidor.
	busy=true; hold(LATENCIA_CLIENTE_PROC_AUTO);busy=false;
	temp->set_type(RESULT_DL);		
	if( (*wse)->random_task_filter_server(0.62) ) {
		temp->set_consensus(2);
	}else{
		temp->set_consensus(0);
	}
	sendMessage2Server(temp);
	(*wse)->consultas_enviadas_desde_peers++;
}
	
void ClientTlcWse::receiveDLmodel(MessageServer* temp)
{
	double TIEMPO_MODELO = 15.0 / 60.0; 		//15 minutos.
	if (0) {cout <<"(Cliente) "<< name()<< ": cambiar FLAG DL " << endl;}
	assert(flagArquitectura==2);
	flagDL = 1;
	busy=true;
	hold(TIEMPO_MODELO);//Descargar el modelo
	busy=false;
	
	//Enviar confirmación de modelo recibido al WSE:
	(*wse)->consultas_enviadas_desde_peers++;
	temp->set_type(DL_MODEL_OK);
	if (0) {cout << name()<< ": Envío confimación de recepción de modelo " << endl;}
	sendMessage2Server(temp);
}

void ClientTlcWse::sendDisconnectionMsg(){
//~ void ClientWSE::sendDisconnectionMsg(){
	MessageServer *mS = new MessageServer(DISCONNECT, 0, 0, 
											0, serverID, id_peer_client, 0, 0);
	debug(time(), "SEND DISCONNECTION NOTIFICATION TO SERVER ---------------------------------------------------------------------------------------");
	sendMessage2Server(mS);
}

void ClientTlcWse::sendConnectionMsg(){
//~ void ClientWSE::sendConnectionMsg(){
	MessageServer *mS = new MessageServer(CONNECT, 0, 0, 
											0, serverID, id_peer_client, 0, 0);
	debug(time(), "SEND CONNECTION NOTIFICATION TO SERVER  ---------------------------------------------------------------------------------------");
	sendMessage2Server(mS);
}

