#include "TlcProtocol.h"
#include <iostream>
#include <fstream>
#include "../auxiliar/Util.h"
void TlcProtocol::inner_body(void)
{
    int MOSTRAR_SALIDA_TLC = 0;

    while (1)
    {
        while ( flag_terminar == 1 )
        {
            (*replicator)->end();
            passivate();
        }
        
        //~ if(queue_in.empty()){
        if(queue_in.empty() && queue_in_lower.empty()){
			//~ debug(time(), "TLC - passivate");
			passivate();
		}            

        if((*observer)->getIteracion() != iteracion)
        {
            //    inlinks.clear();
            //  RCACHE->reset();
            iteracion = (*observer)->getIteracion();
            checkWaitTimeOut();
        }
        
        while(!queue_in_lower.empty())
        {
            PastryMessage *temp1 = (PastryMessage *)queue_in_lower.front();
            queue_in_lower.pop_front();

            TlcMessage *temp = (TlcMessage *)temp1->getData();

            if(temp1->finalDestination)
                temp->setFinalDest();

            if(temp1->getType() != 5)
                delete temp1;
			
            queue_in.push_back(temp);
            debug(time(), "TLC - queue_in.push_back(temp);");
        }
        
        ASSERT(!queue_in.empty());

        TlcMessage *tm = queue_in.front();
        
        queue_in.pop_front();

        // cout << "tm->getKey() antes de algoritmo TLC: " << tm->getKey() << endl;fflush(stdout);
        //~ cout << "tm->getType() antes de algoritmo TLC: " << tm->getType() << endl;fflush(stdout);
        
        TlcAlgorithm(tm);
        
    }
}


void TlcProtocol::TlcAlgorithm(TlcMessage *tm)
{
	debug(time(), "TlcAlgorithm. Start");
    int MOSTRAR_SALIDA_TLC = 0;
    int DEBUG_TLC = 0;

    Entry *e;
    LCache *lc;
    Query *q;
    NodeEntry *lcPeer;

    switch (tm->getType())
    {

		case TLC_LC_ACK:
		{
			lc = tm->getLCache();
			lcack.push_back(lc);
			delete tm;
			break;
		}

		case TLC_WSE:
		{
			//*****************************************************************************************/
			//Saco la entry "e" del mensaje "tm" que recibo del WSE y la inserto en la Rcaché.
			//Si el Peer recibió este mensaje, es porque se lo pidió al WSE. Esto quiere decir que el Peer es
			//el responsable de este objeto.
			//Posteriormente recorro outstanding y waiting.
			//Waiting es una lista de ID de objetos. Lo único que hago es borrar la entrada del mensaje que llegó.
			//Es como si tuviera una lista de flags, y sacara el flag de la consulta que recibí del WSE. Es sólo un flag.
			//Outstanding, por otro lado, se recorre hasta que se encuentra el término del objeto que se recibió.
			//Si se encuentra, se clona la entrada "e", y se envía un mensaje al nodo "source" del mensaje que quedó guardado en la lista.
			//Outstanding también puede traducirse como pendiente.
			//Luego, si se cumple una condición, se guarda el Inlink. Esto todavía no lo entiendo bien.
			//Pareciera ser que, si el ID de este Peer no coincide con el del mensaje,
			//se enlaza el penúltimo elemento del path con la entrada de caché. Si el ÚLTIMO
			//Elemento del Path es este Peer, el penúltimo es el vecino (con el que hay comunicación directa
			//en una topología de anillo). Por lo tanto, lo que se hace es establecer como Inlink para la
			//entrada "e", el vecino por el cuál vino el mensaje.
			//*****************************************************************************************/
			e = tm->getEntry();
			//q=tm->getQuery();
			
			//~ debug(time(), "Llega ensaje con etiqueta TLC_WSE.  TLC: INSERT GLOBAL " +truncateNodeId(pid)+ " - key " << e->key );
			debug(time(), "Llega ensaje con etiqueta TLC_WSE.  TLC: INSERT GLOBAL - key " + to_string(e->key) );
			//COMENTADO:::::::

			RCACHE->insertEntry ( e->clone() );
			
			//ESTO ES DE DEPURACION, EN EL ORIGINAL NO ESTABA. LO AGREGO PARA ANALIZAR LAS RELACIONES ENTRE HITS Y MISSES DE CACHE, CON LOS OBJETOS QUE LLEGAN DESDE EL WSE LUEGO DE SOLICITARLOS.
			(*observer)->add_objArrival(red_asignada);
			//Itero respecto de las listas de mensajes "outstanding" y "waiting".
			vector<TlcMessage *>::iterator it;
			vector< Query *>::iterator at;
			it = outstanding.begin();

			//~ debug(time(), "TLC_WSE. Recorro outstanding - Muestro mensajes TLC para ver los destinos de las respuestas." );
			debug(time(), "TLC_WSE. Recorro outstanding - Muestro mensajes TLC para ver los destinos de las respuestas." );
			//~ cout << "tamaño outstanding: "<< outstanding.size() << endl;
			//~ cout << "tamaño waiting: "<< waiting.size() << endl;

			while(it != outstanding.end())
			{
				
				//Comparo el hash del mensaje con los de los de la lista
				//~ cout << "it->hashValue: "<< BN_bn2hex(((*it)->getQuery())->hashValue) << ", e->hashValue: "<< BN_bn2hex(e->hashValue) << endl;
				if(BN_cmp(((*it)->getQuery())->hashValue, e->hashValue) == 0)    //BN_cmp devuelve 0 si los números son iguales.
				{
					debug(time(), "TLC_WSE. Encuentro mensaje en outstanding. Envio respuesta" );
					TlcMessage *tmp = (*it);
					//~ tmp->print_info();
					Entry *clone = e->clone();
					sendReply(clone, tmp);          //Si coincide, respondo con SendReply() y tmp (es el mensaje de outstanding)
					RCACHE->update(e->hashValue);   //Por qué actualizo el valor en la Rcaché?
					outstanding.erase(it);          //Lo borro de la lista "outstanding"
					if (tmp->getPathSize() >= 2)
					{
						if(BN_cmp(((*pastry)->ne)->getNodeID(), (tmp->src)->getNodeID()) != 0)
							storeInlink(e->hashValue, tmp->path[tmp->getPathSize() - 2]);       //Guardo el recorrido del mensaje
					}
				}
				else
				{
					it++;   //Cambio it para que lea el próximo mensaje de la lista
				}
			}
			debug(time(), "TLC_WSE. Recorro waiting." );
			at = waiting.begin();
			while(at != waiting.end())      //Recorro la lista "waiting"
			{
				if(BN_cmp((*at)->hashValue, e->hashValue) == 0) //Si coincide, borro el mensaje de la lista
				{
					debug(time(), "TLC_WSE. Encuentro mensaje en waiting. Lo elimino" );
					waiting.erase(at);
				}
				else
				{
					at++;
				}
			}

			delete tm;
			break;
		}
		
		case TLC_LOOKUP:
		{
			//*****************************************************************************************/
			//Saco la query del mensaje "tm" y la pongo en una nueva query "q".
			//Comparo el ID de este Peer con el último nodo que figura en el path del mensaje.
			//Si son distintos, añado el ID de este Peer al Path del mensaje.
			//Luego se lee del mensaje la variable booleana FinalDestination ("it's true when the message has arrived to the responsible peer"-TlcMessage.h)
			//Si es el responsable, le digo al observador cuántos Hops hubo, pasándole el valor de PathSize.
			//Luego de los controles de arriba para Path y Hops, me fijo si el objeto está en la Rcaché.
			//Si está y no es vieja, agrego un HitGlobal (sea o no el responsable), envío la respuesta al Peer que pidió el objeto,
			//actualizo la entrada de al Rcaché (¿Qué hace el update? Actualiza la frecuencia y el timestamp del entry).
			//Luego, si vino de más lejos, guardo el InLink (relaciono el ID de objeto con el vecino de donde vino).
			//Si está y es vieja, o si no está, pero soy el responsable, pongo el objeto en outstanding (lo marco como "pendiente").
			//También agrego un Miss de caché, y le mando un mensaje al WSE pidiendo el objeto.
			//Si no está y no soy el responsable (la última opción),
			//reviso la caché Local (cuidado: no la Lcaché, sino la parte LOCAL de la Rcaché).
			//si la entrada aparece y es vieja, la saco de la Rcaché Local.
			//si no es vieja, añado un hitLocal, envío el objeto al Peer que lo pide, y actualizo la entrada de la Rcaché Local.
			//Si no está tampoco en la caché Local, añado un Miss (CREO QUE ESTE NO ESTABA EN EL ORIGINAL) y reviso la Lcaché (Location caché)
			//Si la entrada está, busco el "DONANTE" (¿Es lo mismo que responsable?). Añado un HitLocal, contesto y dejo
			//vinculado al DONANTE con la query.
			//Si la entrada no está en la Lcaché, o si el Donante es igual al source del pedido, voy a GOSALIDA
			//En GOSALIDA me fijo si está en "waiting". Si está en waiting, pongo el mensaje en outstanding (pendiente),
			//agrego un addHitWaiting (esto es para distinguirlo de los hitLocales comunes) y le paso los Hops al observer.
			//Si no está en waiting, sigo pasando el mensaje.
			//Finalmente hago controles de PathSize:
			//Si PathSize==2 (el vecino pidió el objeto), añado una entrada a la Lcaché. También creo un amortiguador (¿Que es? ¿Es una réplica?).
			//Si PathSize==1 (este Peer armó el mensaje pidiendo el objeto) lo agrego a waiting.

			//ACÁ ES MUY INTERESANTE, CASI AL FINAL, CUANDO SE RELACIONA WAITING CON OUTSTANDING. WAITING ES UN FLAG, OUTSTANDING ES UNA LISTA DE MENSAJES (QUE PODRIAN SER RESPUESTAS PENDIENTES)
			//*****************************************************************************************/
			q = tm->getQuery();
			debug(time(), "TLC_LOOKUP" );
			//~ cout << "TLC_LOOKUP. ID peer: "<<BN_bn2hex(pid)<<". hash objeto: "<<BN_bn2hex(q->hashValue) << endl;
			if((tm->path).size() > 0)
			{
				debug(time(), "TLC_LOOKUP. El mensaje pidiendo el objeto de la tarea viene de otro peer" );
				if(BN_cmp(((*pastry)->ne)->getNodeID(), (tm->path[(tm->path).size() - 1])->getNodeID()) != 0)
					tm -> addPath((*pastry)->ne);
			}
			else
			{
				debug(time(), "TLC_LOOKUP. El mensaje pidiendo el objeto de la tarea viene de este peer. Lo añado al Path" );
				tm -> addPath((*pastry)->ne);
			}
			debug(time(), "TLC_LOOKUP. Check finalDest");
			if (tm->getFinalDest())
			{
				debug(time(), "TLC_LOOKUP. Soy responsable de la query:" +to_string(q->getTaskID())+" pedida por el peer: "+to_string(tm->src->getIP()));
				(*observer)->addHops(tm->path.size() - 1, red_asignada);
				//printPath(tm);
				//Calculate average queue size
				//setAvgQueue();
			}
			debug(time(), "TLC_LOOKUP. check global cache entries");
			e = RCACHE->check(q->hashValue);
			debug(time(), "TLC_LOOKUP. got entry, check old");
			if(e != NULL && !e->old(this->time()))
			{
				debug(time(), "TLC_LOOKUP. Entry here and not old");
				debug(time(), "TLC_LOOKUP. Tarea:" +to_string(e->key)+". Solicitante: "+to_string(q->src->getIP())+". El objeto está en la Rcache Global");
				
				(*observer)->addHitGlobal(red_asignada);
				sendReply(e->clone(), tm );
				RCACHE->update(e->hashValue);
				if (tm->getPathSize() >= 2)
				{
					debug(time(), "TLC_LOOKUP. Tarea:" +to_string(e->key)+". Solicitante: "+to_string(q->src->getIP())+". PathSize="+to_string(tm->getPathSize()) +", mayor o igual que 2. Guardar inlink en este PEER.");
					if(BN_cmp(((*pastry)->ne)->getNodeID(), (tm->src)->getNodeID()) != 0)
						storeInlink(q->hashValue, tm->path[tm->getPathSize() - 2]);
				}
			}
			// if it is the final Dest or it the entry is old.
			else if( (e == NULL && tm->getFinalDest()) || (e != NULL && e->old(this->time())) )
			{
				debug(time(), "TLC_LOOKUP. Entry here and old, or not here but i'm responsible");
				if( (e == NULL && tm->getFinalDest()))
				{
					debug(time(), "TLC_LOOKUP. Entry not here and finaldest");
				}
				if( (e != NULL && e->old(this->time())))
				{
					debug(time(), "TLC_LOOKUP. Entry here and old");
					debug(time(), "TLC_LOOKUP. Tarea:" +to_string(e->key)+". Solicitante: "+to_string(q->src->getIP())+". El objeto está en la global pero es viejo.");
				}
				
				debug(time(), "TLC_LOOKUP. add to outstanding");
				
				//CONTROL DE OUTSTANDING (FER)...........................................................
				int pedir = 1;
				for (int i = 0; i < (int)outstanding.size(); i++)
				{
					if (   BN_cmp(q->hashValue,outstanding[i]->query->hashValue )==0 )
					{
						pedir = 0;
					}
				}//...........................................................................
				outstanding.push_back(tm);
				(*observer)->addMiss_global(red_asignada);
				e = tm->getEntry();
				(*(*client)->wse)->consultas_enviadas_desde_tlc++;
				debug(time(), "TLC_LOOKUP. Entry added to outstanding");
				debug(time(), "TLC_LOOKUP. Tamaño waiting: "+to_string(waiting.size())+". Tamaño outstanding: "+to_string(outstanding.size()));
				
				//CONTROL DE OUTSTANDING (FER)...........................................................
				if (pedir == 1)
				{
					debug(time(), "TLC_LOOKUP. Pedir tarea al Server.");
					///(*client)->sendWSEQuery(q);
					(*client)->sendWSEQuery2( q );
					(*observer)->N_ask_obj[red_asignada]++;
				}
				else
				{
					//~ debug(time(), "TLC_LOOKUP. Tarea:" +to_string(q->rta->id_tarea)+". Solicitante: "+to_string(q->src->getIP())+". La consulta ya está en outstanding.");
					debug(time(), "TLC_LOOKUP. La consulta ya está en outstanding.");
				}
				//...........................................................................
			}
			else
			{
				debug(time(), "TLC_LOOKUP. Check local");
				// check local cache entries
				e = localCache->check(q->hashValue);
				debug(time(), "TLC_LOOKUP. Check local OK");
				
				if(e != NULL && e->old(this->time()))
				{
					debug(time(), "TLC_LOOKUP. Entry here and old");
					//~ debug(time(), "TLC_LOOKUP. Tarea:" +to_string(q->rta->id_tarea)+". Está en la Rcaché local, pero es vieja.");
					debug(time(), "TLC_LOOKUP. Tarea:" +to_string(e->key)+". Está en la Rcaché local, pero es vieja.");
					localCache->remove(e);
					e = NULL;
				}
				if (e != NULL)
				{
					debug(time(), "TLC_LOOKUP. Entry here and not old");
					//~ debug(time(), "TLC_LOOKUP. Tarea:" +to_string(q->rta->id_tarea)+". Está en la Rcaché local.");
					debug(time(), "TLC_LOOKUP. Tarea:" +to_string(e->key)+". Está en la Rcaché local.");
					(*observer)->addHitLocal(red_asignada);
					// send reply message and update entry
					//~ debug(time(), "TLC_LOOKUP. Tarea:" +to_string(q->rta->id_tarea)+". Enviar respuesta presente en la Rcache Local.");
					debug(time(), "TLC_LOOKUP. Tarea:" +to_string(e->key)+". Enviar respuesta presente en la Rcache Local.");
					//~ e->rta->print_rta();
					sendReply(e->clone(), tm);
					localCache->update(e->hashValue);
				}
				else
				{
					debug(time(), "TLC_LOOKUP. Entry NULL, not in global or local cache. Check Lcache");
					(*observer)->addMiss_local(red_asignada);
					// check Lcache
					lc = LCACHE->check(q->hashValue);
					// if found add src to Lcache if expiration is ok
					if ( lc != NULL)
					{
						//~ debug(time(), "TLC_LOOKUP. Está en la Lcache. Pido Donante.");
						lcPeer = lc->getDonante(q->src, (double)this->time() );
						if ( lcPeer != NULL && lcPeer != q->src) //if ( lc !=NULL && lc->getLocation()!=q->src)
						{
							(*observer)->addHitLCache(red_asignada);
							//~ debug(time(), "TLC_LOOKUP. El donante existe y no es quien envió la query.");
							
							lc->addNewLocation(lcPeer, this->time());
							//~ debug(time(), "TLC_LOOKUP. Envío con sendLcache.");
							
							sendLCache(lc->clone(), tm);
							/*if(q->src!=(*pastry)->ne)
							{
								lc->addNewLocation(q->src,this->time());
							}*/
						}
						else goto GOSALIDA;
					}
					else
					{
	GOSALIDA:
						debug(time(), "TLC_LOOKUP. Go salida");
						vector<Query *>::iterator ite;
						ite = waiting.begin();
						bool wait = false;
						bool force = false;
						
						if (tm->getForceRouting())
						{
							force = true;
							debug(time(), "TLC_LOOKUP. Tarea:" +to_string(q->getTaskID())+". Tiene force_routing.");
						}
						
						//~ debug(time(), "TLC_LOOKUP. Tarea:" +to_string(q->rta->id_tarea)+". Busco en waiting si no tiene force_routing.");
						//~ debug(time(), "TLC_LOOKUP. Tarea:"+e->key+". Busco en waiting si no tiene force_routing.");
						
						while(ite != waiting.end() && !wait && !force)
						{
							if( BN_cmp(q->hashValue, (*ite)->hashValue) == 0)
							{
								(*observer)->addHitWaiting(red_asignada);
								(*observer)->addHops(tm->path.size() - 1, red_asignada);
								//cout << "outstanding q:" << q->id << " wait " << (*ite)->id << endl;
								outstanding.push_back(tm);
								wait = true;
								//~ debug(time(), "TLC_LOOKUP. Tarea:" +to_string(q->rta->id_tarea)+". Está en waiting.");
								debug(time(), "TLC_LOOKUP. Está en waiting.");
								break;
							}
							ite++;
						}
						if(!wait)
						{
							debug(time(), "TLC_LOOKUP. Continue Routing.");
							continueRouting(tm);
						}

						if (tm->getPathSize() == 2  )
						{
							//~ debug(time(), "TLC_LOOKUP. PathSize==2. Inserto en Lcaché. Routing.");
							//NodeEntry* nextStep = (*pastry)->route(q->hashValue);
							if(q->src != (*pastry)->ne)
							{
								//cout << "route1 " << BN_bn2hex(q->hashValue) << endl;fflush(stdout);
								//~ LCache *temp = new LCache(q->term,
								LCache *temp = new LCache(to_string(q->getTaskID()),
														  q->hashValue,
														  q->src,
														  (*pastry)->route(q->hashValue),
														  this->time());
								temp->CreateAmortiguador(Thao);
								temp->getDonante(q->src, (double)this->time());
								LCACHE->insertLCache(temp);
							}
						}
					}
					if (tm->getPathSize() == 1)
					{
						debug(time(), "TLC_LOOKUP. PathSize==1. Inserto en waiting.");
						waiting.push_back(tm->query);
					}
				}
			}
			debug(time(), "TLC_LOOKUP. Salgo de TLC_LOOKUP.");
			break;
		}

		case TLC_REPLY:
		{
			//*****************************************************************************************/
			//Si llego acá, es porque recibo el objeto de un Peer que me lo envió desde TLC_LOOKUP (Ya sea porque lo encontró en la Rcaché o Lcaché, o porque lo pidió al WSE o a otro Peer).
			//Al recibir el objeto, hago el trabajo (esto ya es código de crowdsourcing).
			//Luego contesto al WSE, enviando primero un mensaje al clienteWSE. (Código de crowdsourcing).
			//¿Siempre recibo y hago el trabajo? ¿Está bien esto?
			//Se me ocurre que quedaría mejor moviendo esto al clienteWSE, que es una capa superior. Así queda mejor delimitada cada cosa.
			//Luego recorro la lista de waiting, y si está la entrada, la borro.
			//Luego recorro la lista de outstanding. Si la query está en outstanding, envío la respuesta al nodo que corresponda.
			//Esto es para el caso de que este Peer haya hecho de intermediario. ¿LAS DE OUTSTANDING SIEMPRE SON PARA CONTESTAR A OTRO?
			//Luego recorro la lista de LC acknowledge, y si la entrada está, la borro.
			//Después aumento el contador de QueriesIn (Queries que son enviadas correctamente a los usuarios de los Peers),
			//y envío al observador la latencia de la consulta.
			//Si es la última consulta, respecto de las que creó el generador para este Peer, envío una señal para finalizar la sim.
			//*****************************************************************************************/
			debug(time(), "Mensaje TLC_REPLY llega desde peer:" +to_string((*observer)->codigos[(tm->src)->getNodeID()]));
			if (MOSTRAR_SALIDA_TLC)
			{
				cout << "Mensaje con etiqueta TLC_REPLY llega al peer " << ((*pastry)->ne)->getIP() << " desde el peer: " << (*observer)->codigos[(tm->src)->getNodeID()] << endl;
			}
			
			e = tm->getEntry();
			q = tm->getQuery();
			(*observer)->addHops(1, red_asignada);
			debug(time(), "TLC_REPLY: Objeto enviado al cliente");
			
			//~ Rtas_Peers *rta = new Rtas_Peers();
			//~ rta->copy(e->rta);
			//~ rta->copy(e->rta);
			//~ rta->id_img = e->rta->id_img;
			//~ rta->Responsable_agreg = e->rta->Responsable_agreg;
			int id_img = 0;
			double TTL_tarea = 0;
			///MessageWSE *respuesta_al_peer = new MessageWSE(((*client)->get_ClientWSE_obj()), e->hashValue, e->key, 0, TTL_tarea, 2, id_img, rta, (tm->src));
			///respuesta_al_peer->set_type(TLC_TO_CLIENT);
			///(*(*client)->get_ClientWSE_obj())->receiveWSE(respuesta_al_peer);
			///if( (*(*client)->get_ClientWSE_obj())->idle() && !(*(*client)->get_ClientWSE_obj())->get_busy() )
			///	(*(*client)->get_ClientWSE_obj())->activateAfter( current() );
			//MessageServer( MsgType _type, int _taskID, int _imgID, //BIGNUM* _query, double _taskTTL, int _src, int _dest, int _nro_opciones, int _desconexion_voluntario	)   
			MessageServer *respuesta_al_peer = new MessageServer(SEND_OBJ, e->key,e->imgID,e->timeOut,((*pastry)->ne)->getIP(),((*pastry)->ne)->getIP(),e->nro_opciones,0);
			respuesta_al_peer->set_aggregationPeer(e->aggregationPeer);
			forward2client(respuesta_al_peer);
			
			// save in localCache
			localCache->insertEntry(e->clone());
			
			vector<Query *>::iterator at;
			at = waiting.begin();


			while(at != waiting.end())
			{
				if(BN_cmp((*at)->hashValue, e->hashValue) == 0)
				{
					waiting.erase(at);
				}
				else
				{
					at++;
				}
			}
			vector<TlcMessage *>::iterator it1;

			it1 = outstanding.begin();
			while(it1 != outstanding.end())
			{
				if(BN_cmp(e->hashValue, ((*it1)->getQuery())->hashValue) == 0)
				{
					//(*observer)->addHitLocal();
					//cout << "TLC: REPLY OUTSTANDING"<< ((*it1)->getQuery())->id << endl;
					TlcMessage *tmp = *it1;
					sendReply(e->clone(), tmp);
					outstanding.erase(it1);
				}
				else
				{
					it1++;
				}
			}
			vector<LCache *>::iterator ilc;
			ilc = lcack.begin();
			while(ilc != lcack.end())
			{
				if(BN_cmp(e->hashValue, (*ilc)->hashValue) == 0)
					lcack.erase(ilc);
				else
					ilc++;
			}

			//Query *q = tm->getQuery();
			//  cout << "Arriving Query " << q->id <<"/" << this->time()  << endl;
			(*observer)->addNQueriesIn(red_asignada);
			(*observer)->addLatencia((double)((this->time()) - (q->t_start)), red_asignada);

			terminadas++;
			(*observer)->terminadas[(*pastry)->ne->getIP()] = terminadas;

			delete tm->getQuery();
			delete tm;
			break;
		}

		case TLC_LCACHE:
		{
			//*****************************************************************************************/
			//Añado al Path este Peer.
			//Armo un puntero a la Lcaché y saco la query del mensaje.
			//Luego me fijo si la query está en la Rcaché LOCAL.
			//Si no está o si es vieja, reviso si está en la lista waiting o lcack (es decir, compruebo flags).
			//Entonces me fijo si está en la Lcaché. Si está, activo un flag.

			//Si está en waiting, y no está en la lista lcack, ni en la Lcaché, la pido a la red con TLC_LOOKUP, la agrego a outstanding y le agrego un Hop.
			//Si no está en waiting o está en alguna de las Lcaché, lo envío ahí.
			//Si no se cumple lo anterior, quiere decir que está en la Rcaché Local, por lo que agarro la entrada y contesto al source.
			//*****************************************************************************************/

			//ADD TO PATH
			if((tm->path).size() > 0)
			{
				if(BN_cmp(((*pastry)->ne)->getNodeID(), (tm->path[(tm->path).size() - 1])->getNodeID()) != 0)
					tm -> addPath((*pastry)->ne);
			}
			else
			{
				tm -> addPath((*pastry)->ne);
			}
			
			lc = tm->getLCache();
			q = tm->getQuery();

			//~ if (MOSTRAR_SALIDA_TLC) {cout <<  "TLC: LCACHE RECEIVED -" << q->id << " key " << BN_bn2hex(q->hashValue)<< endl;fflush(stdout);}
			Entry *tmp = localCache->check(q->hashValue);

			if(tmp == NULL || tmp->old(this->time()) )
			{
				//cout << "not in local cache?" << endl;
				vector<Query *>::iterator at;
				bool wait = false;
				at = waiting.begin();
				while(at != waiting.end())
				{
					if(BN_cmp((*at)->hashValue, q->hashValue) == 0)
					{
						//cout << "ID wait"<< (*at)->id  << endl;
						wait = true;
						break;
					}
					at++;
				}
				bool isLcack = false;

				vector<LCache *>::iterator ilc;
				ilc = lcack.begin();
				while(ilc != lcack.end())
				{
					if(BN_cmp(q->hashValue, (*ilc)->hashValue) == 0)
					{
						isLcack = true;
						break;
					}
					ilc++;
				}
				bool isLack2 = false;
				LCache *lc_temp = LCACHE->check(q->hashValue);
				if (lc_temp != NULL)
				{
					if (BN_cmp(((lc_temp->getLocation())->getNodeID()), (q->src)->getNodeID()) == 0)
					{
						isLack2 = true;
					}
				}
				if(wait && !isLcack && !isLack2)
				{
					//COMENTADO::
					(*observer)->addHitLocal(red_asignada); //DEBERIA SER HitLcaché???
					//        cout << "Hops:" << tm->path.size() << endl;
					(*observer)->addHops(tm->path.size() - 1, red_asignada);
					tm->setType(TLC_LOOKUP);
					outstanding.push_back( tm);

					//printPath(tm);
				}
				else //
				{
					continueRouting(tm, lc->getNextStep());
					delete lc;
				}
			}
			else
			{
				//cout << "replying" << endl;
				(*observer)->addHitLocal(red_asignada);
				//          cout << "Local 2-" << q->id << endl;
				sendReply(tmp->clone(), tm);
				localCache->update(tmp->hashValue);
			}
			//delete lc;
			break;
		}

		case TLC_REPLICATE:
		{
			//*****************************************************************************************/
			//Acá simplemente me llega una query y la pido al WSE.
			//Buscar diferencia con otra opción del case que también pida la consulta al WSE.
			//Se usa en TLC_LOOKUP, acá, y también en LeafsetBubble.
			//Si me llega este mensaje, tengo que pedir al WSE el objeto... Y ahí ver si lo guardo en alguna memoria mía.
			//La replicación en este trabajo consiste en replicar la responsabilidad.
			//¿Implica esto que al recibir el objeto, lo guardo en la Rcaché global de este Peer?
			//Claro, porque en las líneas que siguen acá abajo, le estamos mandando un mensaje al WSE
			//como si nosotros fuéramos el responsable de ese objeto.
			//*****************************************************************************************/
			
			// add entry to replicaCache
			debug(time(), "TLC-REPLICATE Received!!.");
			e = tm->getEntry();
			(*(*client)->wse)->consultas_enviadas_desde_tlc++;
			///(*client)->sendWSEQuery( new Query(0, (char *)(e->key).c_str(), BN_dup( e->hashValue), this->time(), e->TTL_tarea, e->rta));
			(*client)->sendWSEQuery2( new Query(0, e->key,e->imgID,e->nro_opciones, BN_dup( e->hashValue), this->time()) );
			(*observer)->N_ask_replica[red_asignada]++;
			delete tm;
			break;
		}

		case TLC_SEND_RESP:
		{
			//*****************************************************************************************/
			//Recibo este mensaje desde el cliente del mismo nodo, y tengo que enviarlo
			//con algo parecido a sendReply, hacia el responsable.
			//El ID del responsable tiene que estar en un formato determinado.
			//*****************************************************************************************/
			debug(time(), "TLC_SEND_RESP. Enviar respuesta al responsable: ");
			Entry *e = tm->getEntry();
			//~ debug(time(), "TLC_SEND_RESP. Enviar respuesta al responsable: " +to_string(e->aggregationPeer->getIP()));
			//~ debug(time(), "TLC_SEND_RESP. Fuente en tm: " +to_string((tm->src)->getIP())+". Destino en tm: " +to_string((tm->entry->rta->Responsable_agreg)->getIP()));
			//~ sendReply(e->clone(), tm);          //Si coincide, respondo con SendReply() y tmp (es el mensaje de outstanding)
			sendReplyForAggregation(e->clone(),tm);          //Si coincide, respondo con SendReply() y tmp (es el mensaje de outstanding)
			break;
		}

		case TLC_AGGREGATE:
		{
			(*observer)->addHops(1, red_asignada);
			//*****************************************************************************************/
			//Recibo este mensaje desde el cliente del mismo nodo, y tengo que enviarlo
			//con algo parecido a sendReply, hacia el responsable.
			//El ID del responsable tiene que estar en un formato determinado.
			//*****************************************************************************************/
			debug(time(), "Llega mje TLC_REPLY  desde el peer (tm->destNode)->getIP(): " +to_string((tm->destNode)->getIP())+". (tm->src)->getIP():" +to_string((tm->src)->getIP()));
			
			e = tm->getEntry();
			//~ q = tm->getQuery();
			//MessageWSE* respuesta_al_peer = new MessageWSE(((*client)->get_ClientWSE_obj()), e->hashValue,e->key, 0, e->TTL_tarea, e->opciones,e->prioridad_tarea, 3, e->id_imagen,(tm->src));
			///MessageWSE *respuesta_al_peer = new MessageWSE(((*client)->get_ClientWSE_obj()), e->hashValue, e->key, 0, e->TTL_tarea, 3, e->id_imagen, e->rta, (tm->src));
			///respuesta_al_peer->tag = 4;
			///respuesta_al_peer->set_type(TLC_TO_CLIENT);
			///(*(*client)->get_ClientWSE_obj())->receiveWSE(respuesta_al_peer);
			///if( (*(*client)->get_ClientWSE_obj())->idle() && !(*(*client)->get_ClientWSE_obj())->get_busy() )
			///	(*(*client)->get_ClientWSE_obj())->activateAfter( current() );
			//MessageServer( MsgType _type, int _taskID, int _imgID, //BIGNUM* _query, double _taskTTL, int _src, int _dest, int _nro_opciones, int _desconexion_voluntario	)   
			MessageServer *respuesta_al_peer = new MessageServer(AGGREGATE_TASK, e->key,e->imgID,e->timeOut,((*pastry)->ne)->getIP(),((*pastry)->ne)->getIP(),e->nro_opciones,0);
			respuesta_al_peer->set_opciones(tm->getOpciones());
			//~ cout<< "opciones en TLC"<< endl;
			//~ respuesta_al_peer->print_opciones();
			forward2client(respuesta_al_peer);
			break;
			
		}
			//~ cout <<"(TLC) Salgo del algoritmo TLC" << endl;

	}
}

void TlcProtocol::sendEndBroadcast()
{

}


void TlcProtocol::printPath(TlcMessage *tm)
{
    int MOSTRAR_SALIDA_TLC = 0;
    Query *q  = tm->getQuery();
    //ORIGINAL:::::: const char* s = UTIL::myBN2HEX( q->hashValue);
    //~ const char* s = BN_bn2hex(q->hashValue);
    //ORIGINAL::::::  if (MOSTRAR_SALIDA){cout<< "--------- PATH for " << s << " ---------"<<endl;fflush(stdout);}
    if (MOSTRAR_SALIDA_TLC)
    {
        cout << "--------- PATH for " << q->hashValue << " ---------" << endl;
        fflush(stdout);
    }
    // free((char*)s);
    for(unsigned int k = 0; k < tm->path.size(); k++)
    {
        //ORIGINAL:::::: const char* t = UTIL::myBN2HEX((tm->path[k])->getNodeID()) ;
        //ORIGINAL:::::: const char* t = BN_bn2hex((tm->path[k])->getNodeID()) ;

        //ORIGINAL:::::: if (MOSTRAR_SALIDA){cout << k << " -> " <<  t <<endl;fflush(stdout);}
        //if (MOSTRAR_SALIDA){cout << k << " -> " <<  (tm->path[k])->getNodeID() <<endl;fflush(stdout);}
        if (MOSTRAR_SALIDA_TLC)
        {
            cout << k << " -> " <<  (tm->path[k])->getIP()  << endl;
            fflush(stdout);
        }
        // free((char*)t);
    }
    //(*observer)->addHops(tm->path.size()-1);
    cout << "--------------------------------------------------------------------" << endl;
    fflush(stdout);


}


// TODO
void TlcProtocol::continueRouting(TlcMessage *tm, NodeEntry *neD)
{
    //cout << "Tlc: continueRouting LCACHE " << endl;
    tm->setType(TLC_LOOKUP);
    tm->dest = (tm->getQuery())->hashValue;

    PastryMessage *pm = new PastryMessage(PASTRY_SENDDIRECT, (*pastry)->ne, neD, tm);
    pm->setDataType((DataType)tm->getDataType());

    (*pastry)->add_message(pm);
    if((*pastry)->idle() && !(*pastry)->get_busy())
        (*pastry)->activateAfter(current());


}


void TlcProtocol::continueRouting(TlcMessage *tm)
{
    //cout << "Tlc: continueRouting " << endl;

    PastryMessage *pm = new PastryMessage(PASTRY_SENDDHT, (*pastry)->ne, tm->dest, tm);
    pm->setDataType((DataType)tm->getDataType());

    (*pastry)->add_message(pm);
    if((*pastry)->idle() && !(*pastry)->get_busy())
        (*pastry)->activateAfter(current());

}


void TlcProtocol::sendLCache(LCache *lc, TlcMessage *tm)
{

    TlcMessage *reply = new TlcMessage(TLC_LCACHE, msgID, (*pastry)->ne, (lc->getLocation()));
    reply->setDataType(SOLICITUD_OBJETO);

    msgID = msgID + 1;
    reply->setLCache(lc->clone());
    reply->setQuery(tm->getQuery());
    reply->setPath(tm->path);

    PastryMessage *pm = new PastryMessage( PASTRY_SENDDIRECT,
                                           (*pastry)->ne,
                                           lc->getLocation(),
                                           reply);
    pm->setDataType(SOLICITUD_OBJETO);

    (*pastry)->add_message(pm);
    if((*pastry)->idle() && !(*pastry)->get_busy())
        (*pastry)->activateAfter(current());
    // delete tm;

    TlcMessage *lc_ack = new TlcMessage(TLC_LC_ACK, msgID, (*pastry)->ne, (tm->getQuery())->src );
    lc_ack->setDataType(SOLICITUD_OBJETO);

    msgID = msgID + 1;
    lc_ack->setLCache(lc->clone());
    lc_ack->setQuery(tm->getQuery());
    lc_ack->setPath(tm->path);

    PastryMessage *pm2 = new PastryMessage( PASTRY_SENDDIRECT,
                                            (*pastry)->ne,
                                            (tm->getQuery())->src,
                                            lc_ack);
    pm2->setDataType(SOLICITUD_OBJETO);

    (*pastry)->add_message(pm2);
    if((*pastry)->idle() && !(*pastry)->get_busy())
        (*pastry)->activateAfter(current());

}


void TlcProtocol::sendReply(Entry *_e, TlcMessage *tm)
{
        
    if (tm->getType() == 7)
    {
		//~ cout <<  "(Nodo TLC) TLC SEND REPLY PARA AGREGATE - PEER: " << (*pastry)->ne->getIP() << " envía el objeto al peer: " << tm->destNode->getIP() << " con la función sendReply() PARA AGREGAR" << endl;
        //~ TlcMessage *reply = new TlcMessage(TLC_AGGREGATE, msgID, (*pastry)->ne, tm->entry->rta->Responsable_agreg);
        TlcMessage *reply = new TlcMessage(TLC_AGGREGATE, msgID, (*pastry)->ne, tm->destNode);
        reply->setDataType(TRABAJO_TERMINADO);

        tm->setDataType(TRABAJO_TERMINADO);
        tm->setType(TLC_AGGREGATE);
		
        //~ msgID = msgID + 1;
        //~ reply->setEntry(_e);
        //~ reply->setQuery(tm->getQuery());
        //~ PastryMessage *pm = new PastryMessage(PASTRY_SENDDIRECT,(*pastry)->ne,tm->destNode, reply);
        //~ PastryMessage *pm = new PastryMessage(PASTRY_SENDDIRECT, (*pastry)->ne, tm->entry->rta->Responsable_agreg, reply);
        //~ PastryMessage *pm = new PastryMessage(PASTRY_SENDDIRECT, (*pastry)->ne, tm->destNode, reply);
        PastryMessage *pm = new PastryMessage(PASTRY_SENDDIRECT, (*pastry)->ne, tm->destNode, tm);
        pm->setDataType(TRABAJO_TERMINADO);
		
        (*pastry)->add_message(pm);
        if((*pastry)->idle() && !(*pastry)->get_busy())
            (*pastry)->activateAfter(current());
    }
    else
    {
		setAvgQueue();
		overloaded++;
		// (*observer)->addNQueries();
		(*observer)->addLoad(((*pastry)->ne)->getIP());
		Query *q = tm->getQuery();
        TlcMessage *reply = new TlcMessage(TLC_REPLY, msgID, (*pastry)->ne, q->src);
        reply->setDataType(OBJETO_OPCIONES);

        msgID = msgID + 1;
        reply->setEntry(_e);
        debug(time(), "TLC SEND REPLY (Responsable): Mando objeto al peer que va a trabajar. Peer de destino: "+to_string(q->src->getIP()));
        //~ debug(time(), "TLC SEND REPLY. ID responsable: "+BN_bn2hex(pid)+". hash objeto: "+BN_bn2hex(q->hashValue));		//No puedo imprimir de esta forma los valores hexa.
        //~ cout << "TLC SEND REPLY. ID responsable: "<<BN_bn2hex(pid)<<". hash objeto: "<<BN_bn2hex(q->hashValue) << endl;
        reply->setQuery(tm->getQuery());
        PastryMessage *pm = new PastryMessage(PASTRY_SENDDIRECT, (*pastry)->ne, q->src, reply);
        pm->setDataType(OBJETO_OPCIONES);

        (*pastry)->add_message(pm);
        if((*pastry)->idle() && !(*pastry)->get_busy())
            (*pastry)->activateAfter(current());
    }
}

void TlcProtocol::sendReplyForAggregation(Entry *_e,TlcMessage *tm)
{
        
    assert (tm->getType() == 7);
    
	//~ cout <<  "(Nodo TLC) TLC SEND REPLY PARA AGREGATE - PEER: " << (*pastry)->ne->getIP() << " envía el objeto al peer: " << tm->destNode->getIP() << " con la función sendReply() PARA AGREGAR" << endl;
	TlcMessage *reply = new TlcMessage(TLC_AGGREGATE, msgID, (*pastry)->ne, tm->destNode);
	reply->setDataType(TRABAJO_TERMINADO);

	tm->setDataType(TRABAJO_TERMINADO);
	tm->setType(TLC_AGGREGATE);
	
	PastryMessage *pm = new PastryMessage(PASTRY_SENDDIRECT, (*pastry)->ne, tm->destNode, tm);
	pm->setDataType(TRABAJO_TERMINADO);
	
	(*pastry)->add_message(pm);
	if((*pastry)->idle() && !(*pastry)->get_busy())
		(*pastry)->activateAfter(current());

}

void TlcProtocol::sendReplica(Entry *e, NodeEntry *nodeEntry)
{
    TlcMessage *replica = new TlcMessage(TLC_REPLICATE, msgID, (*pastry)->ne, nodeEntry);
    replica->setDataType(SOLICITUD_OBJETO);

    msgID = msgID + 1;
    replica->setEntry(e);
    (*observer)->addReplicas(red_asignada);
    //cout << "addReplicas en TlcProtocol::sendReplica" << endl;
    PastryMessage *pm = new PastryMessage (PASTRY_SENDDIRECT, (*pastry)->ne, nodeEntry, replica);
    pm->setDataType(SOLICITUD_OBJETO);

    (*pastry)->add_message(pm);
    if((*pastry)->idle() && !(*pastry)->get_busy())
        (*pastry)->activateAfter(current());
}



void TlcProtocol::setAvgQueue()
{
    t_act = this->time() ;
    //cout << "TLC: T-actual "<< t_act<< endl;
    //cout << "TLC: avg_queue " << avg_queue<<endl;
    //fflush(stdout);

    avg_queue = avg_queue + ( (queue_in.size() + 1) * (t_act - t_pas) );
    t_pas = t_act;
    // cout << "TLC: AVG_QUEUE " << avg_queue << endl;
    // fflush(stdout);
}

void TlcProtocol::resetQueue()
{
    avg_queue = 0;
    overloaded = 0;
}

bool TlcProtocol::isReplicated(BIGNUM *nodeId, BIGNUM *key)
{
    map<BIGNUM *, Replicas *>::iterator it;
    it = replicaTrack.begin();
    // cout << "TLC-IsReplicated?"<< endl;

    while(it != replicaTrack.end())
    {
        if( BN_cmp(it->first, nodeId) == 0 && (it->second)->isContained(key)  )
        {
            //cout << "Esta Replicado" << endl;
            return true;
        }
        ++it;
    }
    return false;
}


bool TlcProtocol::isNewReplica(BIGNUM *id)
{
    map<BIGNUM *, Replicas *>::iterator it;
    it = replicaTrack.begin();
    while(it != replicaTrack.end())
    {
        if( BN_cmp(it->first, id) == 0 )
            return false;
        ++it;
    }

    return true;
}




void TlcProtocol::trackReplica(BIGNUM *nodeId, BIGNUM *key)
{
    // bool flag = isReplicated(nodeId, key);
    bool flag = isNewReplica(nodeId);

    //No esta
    flag = 0;
    if(flag)
    {
        Replicas *r = new Replicas(key, nodeId);
        replicaTrack.insert(pair <BIGNUM *, Replicas *>(nodeId, r) );
        replicaTrack[nodeId] = r;

        (*observer)->addReplicas(red_asignada);
        //cout << "addReplicas en TlcProtocol::trackReplica1" << endl;
        //Existe hay q revisar
    }
    else
    {
        map<BIGNUM *, Replicas *>::iterator it5;
        it5 = replicaTrack.begin();

        while(it5 != replicaTrack.end())
        {
            if(BN_cmp(nodeId, it5->first) == 0)
            {
                (it5->second)->putReplica(key);
                (*observer)->addReplicas(red_asignada);
                //cout << "addReplicas en TlcProtocol::trackReplica2" << endl;
            }
            ++it5;
        }
    }
}


void TlcProtocol::storeInlink(BIGNUM *key, NodeEntry *n)
{
    bool inlinkStored = false;
    Inlink *i;
    map<BIGNUM *, Inlink *>::iterator it;
    it = inlinks.begin();

    // cout <<"-------------------------- I -------------" << endl;
    while(it != inlinks.end())
    {
        //cout<< BN_bn2hex(it -> first)<< endl;
        if(BN_cmp(it->first, n->getNodeID()) == 0)
        {
            inlinkStored = true;
            i = it ->second;
            break;
        }
        ++it;
    }
    //cout << "Storing Inlink -> "<<  inlinkStored <<endl;
    //Existe Inlink
    if( inlinkStored )
    {
        //cout <<"Put Query "<< BN_bn2hex(key)<< endl;
        //Consulta llega por ese inlink?
        i->putQuery(key);

        //Inlink no existe, se inserta y se inserta la query
    }
    else
    {
        //cout <<"New Inlink " << BN_bn2hex(n -> getNodeID() )<< endl;
        Inlink *i2 = new Inlink(n);
        i2->putQuery(key);

        inlinks.insert(pair<BIGNUM *, Inlink *>((n)->getNodeID(), i2));
        inlinks[n->getNodeID()] = i2;
    }
}


void TlcProtocol::freeMessage(TlcMessage *msg)
{
    free(msg->dest);
    free(msg);
}


void TlcProtocol::EndReplicator()
{
    //~ cout<<"End replicator "<<endl;
    (*replicator)->end();
}


void TlcProtocol::checkWaitTimeOut()
{
    vector<Query *>::iterator at;
    at = waiting.begin();
    while(at != waiting.end())
    {
        if((*at)->getTimeOut() < this->time() && !((*at)->getForceRouting()))
        {
            Query *q = (*at); // new Query(at->id,at->term,at->hashValue,0.0);
            q->renewTimeOut(this->time());
            q->forceRouting();
            this->add_query(q);
            waiting.erase(at);
        }
        else
        {
            at++;
        }
    }
}


void TlcProtocol::debug(double time, string msg  )
{
    if (0) cout << "[" << time << "][" << name() << "][ " << msg << " ]" << endl;
}

void TlcProtocol::forward2client(MessageServer *m )
{
	if (m->getType() != AGGREGATE_TASK){
		list <int>opciones(m->getOptionNr(),-1);
		m->set_opciones(opciones);
	}
	(*(*client)->get_ClientWSE_obj())->receiveWSE2(m);
	if( (*(*client)->get_ClientWSE_obj())->idle() && !(*(*client)->get_ClientWSE_obj())->get_busy() )
		(*(*client)->get_ClientWSE_obj())->activateAfter( current() );
}

void TlcProtocol::terminar(  )
{
	//~ cout << "Terminar" << name() << endl;
	flag_terminar = 1;
	(*pastry)->terminar();
	//~ (*pastry)->activateAfter(current());
	(*replicator)->end();
	(*replicator)->activateAfter(current());
}
		
