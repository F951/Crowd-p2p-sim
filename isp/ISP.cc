#include "ISP.h"
#include "../applicationLayer/ClientTLCWSE.h"

void ISP::inner_body( )
{

    double DELAY = 0;
    MessageServer *mS ;
    while(1)
    {
        if ( flag_terminar == 1)
        {
            //~ debug(time(), "ISP, flag terminar, passivate");
            passivate();
        }
        if ( queue_in2.empty() )
        {
            //~ debug(time(), "ISP, queue2 empty, passivate");
            passivate();
        }
        ASSERT( !queue_in2.empty() );
        mS = queue_in2.front();
        queue_in2.pop_front();
        
        if (mS->getFirstMessageFlag()==1){
			busy = true;
			hold(DELAY_BASE_ISP*cte_traffJam_isp);	//HOLD TENIENDO EN CUENTA LAS ESTADISTICAS DE LATENCIA DE RED
			busy = false;
			debug_traffic(DELAY_BASE_ISP, cte_traffJam_isp  );
			(*observer)->t_red_isp[red_nro]+=DELAY_BASE_ISP*cte_traffJam_isp;
        }

        //~ debug(time(), "Message from " + to_string(mS->getSrc()));
        //~ mS->print();
		//~ assert(mS->getType() != RESULT_DL);
        switch (mS->getType())
        {
			case SEND_DL_MODEL:
			{
				debug(time(), "SEND_DL_MODEL");
				assert(flagArquitectura==2);
				DELAY = 4.51/3600.0;		//En 4,91 segundos se envían los 26 MB del modelo, a 5,3 MBps
				busy = true;
				hold(DELAY * cte_traffJam_isp);
				busy = false;
				debug_traffic(DELAY, cte_traffJam_isp  );
				(*observer)->t_red_isp[red_nro] += DELAY * cte_traffJam_isp;
				deliver2Peer(mS);
				break;
			}
			
			case DL_MODEL_OK:
			{
				debug(time(), "DL_MODEL_OK");
				assert(flagArquitectura==2);
				DELAY = LATENCIA_SOLICITUD_OBJETO_ISP;		//Msje corto
				busy = true;
				hold(DELAY * cte_traffJam_isp);
				busy = false;
				debug_traffic(DELAY, cte_traffJam_isp  );
				(*observer)->t_red_isp[red_nro] += DELAY * cte_traffJam_isp;
				deliver2Server(mS);
				break;
			}
			
			case SEND_TASK:
			{
				debug(time(), "SEND_TASK");
				DELAY = LATENCIA_SOLICITUD_TAREA_ISP;
				(*observer)->contador_isp_env_tarea[red_nro]++;
				busy = true;
				hold(DELAY * cte_traffJam_isp);
				busy = false;
				debug_traffic(DELAY, cte_traffJam_isp  );
				(*observer)->t_red_isp[red_nro] += DELAY * cte_traffJam_isp;
				deliver2Peer(mS);
				break;
			}

			case ASK_OBJ:
			{
				debug(time(), "ASK_OBJ");
				DELAY = LATENCIA_SOLICITUD_OBJETO_ISP;
				busy = true;
				hold(DELAY * cte_traffJam_isp);
				busy = false;
				//~ cout << "DEBUG traffic - DELAY: " << DELAY <<", cte_traffJam_isp: " << cte_traffJam_isp <<", DELAY*cte_traffJam_isp: " << DELAY*cte_traffJam_isp << endl;
				(*observer)->contador_isp_Sol_Img[red_nro]++;
				(*observer)->t_red_isp[red_nro] += DELAY * cte_traffJam_isp;
				deliver2Server(mS);
				break;
			}

			case SEND_OBJ:
			{
				debug(time(), "SEND_OBJ");
				DELAY = LATENCIA_OBJETO_OPCIONES_ISP;
				(*observer)->contador_isp_env_Img[red_nro]++;
				busy = true;
				hold(DELAY * cte_traffJam_isp);
				busy = false;
				debug_traffic(DELAY, cte_traffJam_isp  );
				(*observer)->t_red_isp[red_nro] += DELAY * cte_traffJam_isp;
				deliver2Peer(mS);
				break;
			}
			
			case SEND_OBJ_OWNER:
			{
				debug(time(), "SEND_OBJ_OWNER");
				DELAY = LATENCIA_OBJETO_OPCIONES_ISP;
				(*observer)->contador_isp_env_Img[red_nro]++;
				busy = true;
				hold(DELAY * cte_traffJam_isp);
				busy = false;
				debug_traffic(DELAY, cte_traffJam_isp  );
				(*observer)->t_red_isp[red_nro] += DELAY * cte_traffJam_isp;
				deliver2Peer(mS);
				break;
			}

			case SEND_RESULT:
			{
				debug(time(), "SEND_RESULT");
				DELAY = LATENCIA_TRABAJO_TERMINADO_ISP;
				busy = true;
				hold(DELAY * cte_traffJam_isp);
				busy = false;
				debug_traffic(DELAY, cte_traffJam_isp  );
				(*observer)->contador_isp_resultados[red_nro]++;
				(*observer)->t_red_isp[red_nro] += DELAY * cte_traffJam_isp;
				deliver2Server(mS);
				break;
			}
			
			case RESULT_DL:
			{
				debug(time(), "SEND_RESULT");
				DELAY = LATENCIA_TRABAJO_TERMINADO_ISP;
				busy = true;
				hold(DELAY * cte_traffJam_isp);
				busy = false;
				debug_traffic(DELAY, cte_traffJam_isp  );
				(*observer)->contador_isp_resultados[red_nro]++;
				(*observer)->t_red_isp[red_nro] += DELAY * cte_traffJam_isp;
				deliver2Server(mS);
				break;
			}
			
			case CONNECT:
			{
				debug(time(), "CONNECT");
				DELAY = LATENCIA_SOLICITUD_OBJETO_ISP;		//Msje corto
				busy = true;
				hold(DELAY * cte_traffJam_isp);
				busy = false;
				debug_traffic(DELAY, cte_traffJam_isp  );
				(*observer)->t_red_isp[red_nro] += DELAY * cte_traffJam_isp;
				deliver2Server(mS);
				break;
			}
			
			case DISCONNECT:
			{
				debug(time(), "DISCONNECT");
				DELAY = LATENCIA_SOLICITUD_OBJETO_ISP;		//Msje corto
				busy = true;
				hold(DELAY * cte_traffJam_isp);
				busy = false;
				debug_traffic(DELAY, cte_traffJam_isp  );
				(*observer)->t_red_isp[red_nro] += DELAY * cte_traffJam_isp;
				deliver2Server(mS);
				break;
			}

			default:
			{
				mS->print();
				assert(false);
				debug(time(), "ERROR");
				DELAY = 0;
				break;
			}

        }
    }
}

void ISP::deliver2Server(MessageServer *m )
{
    debug(time(), "Deliver 2 Server");
    (*wse)->add_request2(m);
    if((*wse)->idle() && !(*wse)->get_busy())
    {
        (*wse)->activateAfter(current());
    }

}

void ISP::deliver2Peer(MessageServer *m )
{
    handle<ClientTlcWse> *peer = clientes_wse_Peers[m->getDest()];
    debug(time(), "Deliver 2 Peer");
    (*peer)->receiveWSE2(m);
    if ((*peer)->idle() && !(*peer)->get_busy() )
        (*peer)->activateAfter( current() );
}

void ISP::printMSG(MessageWSE *m)
{
    cout << "Mensaje en ISP de tipo: " << m->getType() << endl;
    cout << "Tarea: " << m->rta->id_tarea << endl;
    cout << "TAG: " << m->tag << endl;
    cout << "SRC: " << (*(m->src))->id_peer_client << endl;
}

void ISP::debug(double time, string msg  )
{
    if (0) cout << "[" << time << "][ISP][ " << msg << " ]" << endl;
}

void ISP::debug_traffic(double DELAY, double cte_traffJam_isp  )
{
    if (0) cout << "DEBUG traffic - DELAY: " << DELAY <<", cte_traffJam_isp: " << cte_traffJam_isp <<", DELAY*cte_traffJam_isp: " << DELAY*cte_traffJam_isp << endl;
}
