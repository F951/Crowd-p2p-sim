#include "redtubo.h"

void RedTubo::inner_body( )
{
    //int cont_flag =0;
    double DELAY = 0;
    int debug_red = 0;

    while(1)
    {

        if ( flag_terminar == 1 )
        {
            passivate();
        }
        if ( queue_in.empty() )
        {
            passivate();
        }
        ASSERT( !queue_in.empty() );
        MessageT *m = queue_in.front();
        queue_in.pop_front();

        switch (m->getType())
        {
			case T_P2P:			//Now it's the only type, no more broadcast msgs.
			{
				// Simulates sending a message to other peer 	//--------------------------------------------
				busy = true;
				hold(DELAY_BASE_P2P* cte_traffJam_p2p);	//HOLD TENIENDO EN CUENTA LAS ESTADISTICAS DE LATENCIA DE RED QUE ME PASÓ VERO.
				busy = false;
				(*observer)->t_red_p2p[red_nro] += DELAY_BASE_P2P* cte_traffJam_p2p;			//--------------------------------------------
				debug_traffic(DELAY_BASE_P2P, cte_traffJam_p2p);
			 
				switch (m->getDataType())
				{
				case SOLICITUD_TAREA:
				{
					debug(time(), "red tubo: LATENCIA_SOLICITUD_TAREA_P2P");
					DELAY = LATENCIA_SOLICITUD_TAREA_P2P;
					break;
				}
				case SOLICITUD_OBJETO:
				{
					debug(time(), "red tubo: LATENCIA_SOLICITUD_OBJETO_P2P");
					DELAY = LATENCIA_SOLICITUD_OBJETO_P2P;
					break;
				}
				case OBJETO_OPCIONES:
				{
					debug(time(), "red tubo: LATENCIA_OBJETO_OPCIONES_P2P");
					DELAY = LATENCIA_OBJETO_OPCIONES_P2P;
					break;
				}
				case TRABAJO_TERMINADO:
				{
					debug(time(), "red tubo: LATENCIA_TRABAJO_TERMINADO_P2P");
					DELAY = LATENCIA_TRABAJO_TERMINADO_P2P;
					break;
				}
				case TRABAJO_AGREGADO:
				{
					debug(time(), "red tubo: LATENCIA_TRABAJO_AGREGADO_P2P");
					DELAY = LATENCIA_TRABAJO_AGREGADO_P2P;
					break;
				}
				default:
				{
					debug(time(), "red tubo: ERROR");
					break;
				}
				}
				busy = true;
				hold(DELAY * cte_traffJam_p2p);	//HOLD TENIENDO EN CUENTA LAS ESTADISTICAS DE LATENCIA DE RED QUE ME PASÓ VERO.
				busy = false;
				(*observer)->t_red_p2p[red_nro] += DELAY * cte_traffJam_p2p;
				debug_traffic(DELAY, cte_traffJam_p2p);
				
				/* Destination comes within the MessageT */
				int dest = m->getDest();
				dest = dest - lim_inf;
				debug(time(), "red_nro: " + to_string(red_nro) + ". dest: " + to_string(dest) + ". lim_inf: " + to_string(lim_inf) + ", (*nodes[dest])->ne->getIP(): " + to_string((*nodes[dest])->nid));
				
				(*nodes[dest])->deliver(m);
				if( (*nodes[dest])->idle() && !(*nodes[dest])->get_busy() )
					(*nodes[dest])->activateAfter( current() );
				break;
			}
        }
    }
}


void RedTubo::debug_traffic(double DELAY, double cte_traffJam_p2p  )
{
    if (0) cout << "DEBUG traffic P2P - DELAY: " << DELAY <<", cte_traffJam_p2p: " << cte_traffJam_p2p <<", DELAY*cte_traffJam_p2p: " << DELAY*cte_traffJam_p2p << endl;
}

void RedTubo::debug(double time, string msg  )
{
    if (0) cout << "[" << time << "][P2P net][ " << msg << " ]" << endl;
}
