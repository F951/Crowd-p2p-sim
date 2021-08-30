#ifndef Task_H
#define Task_H
#include "../glob.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "MessageServer.h"

class Task
{
private:

    int id_tarea;
    int id_img;
    double TTL_tarea;
    //NodeEntry* Responsable_agreg;		//Cambiamos esto por un entero, así no hay problemas al liberar el objeto.
    int Responsable_agregacion;
    int consenso;
    int rtas_recibidas;			//Puede estar en las entradas de 'en_proceso'. Es para debug, se puede comentar al llevarlo al multicore.
    double tiempo_generacion;
    double tiempo_inicio;
    double tiempo_finalizacion;

    map<int, list<int> > votacion;
    list<int> peers_consultados_1;					//Sirve para el envío de historias (tareas enviadas al mismo grupo de peers).
    //~ list<int> peers_consultados_2;				//Es sólo para debug
    //~ list<int> peers_que_respondieron_1;			//Es sólo para debug
    //~ list<int> peers_que_respondieron_2;			//Es sólo para debug

    int nro_opciones;
    //list<int> opciones;					//Esta debería estar, pero ver qué es más eficiente
    //list<int> opciones_originales;		//Esto no sirve de nada. Solo importa la cantidad de opciones a votar.

    int reenviada; 						//Debería usarse para las entradas de 'en_proceso'.
    
    int red_asignada;					//Caso de redes múltiples
    
    
    //int enviada_al_wse_1; 				//Esto servía para comparar, en el caso 2, si las respuestas eran del primer o segundo envío.
    //int enviada_al_wse_2; 				//Esto servía para comparar, en el caso 2, si las respuestas eran del primer o segundo envío.

    //int flag_primer_mensaje;			//Para distinguir, en el caso de las historias, a qué mensaje se le aplica la latencia de red en el ISP.


    //int rtas_esperadas;		//No hace falta, es H
    //int prioridad_tarea;	//No usado.
    //vector<double> TimeStamps;			//Esto es para más adelante, cuando hagamos el trabajo de optimización. A lo mejor conviene poner esto en el observer
    //vector<int> contadores_TimeStamps;	//Esto es para más adelante, cuando hagamos el trabajo de optimización. A lo mejor conviene poner esto en el observer

public:

    ~Task()
    {

    }

    Task(int _id_tarea, int _id_img, double _ttl, int _nro_opciones, double _tiempo_generacion, double _tiempo_inicio, int _red_asignada) //, double _tiempo_generacion)
    {
        id_tarea			= _id_tarea;
        id_img 				= _id_img;
        TTL_tarea    		= _ttl;
        nro_opciones = _nro_opciones;

        rtas_recibidas = 0;
        consenso = 0;
        reenviada = 0;


        tiempo_inicio		= _tiempo_inicio;
        tiempo_generacion	= _tiempo_generacion;
        tiempo_finalizacion	= 0;
        consenso	= 0;
        Responsable_agregacion = 0;		//Para el Proposal 2
        //~ enviada_al_wse_1=0;			//Para el Proposal 2
        //~ enviada_al_wse_2=0;			//Para el Proposal 2

        red_asignada = _red_asignada;

    }

    //~ void copy(Task* & rta_in)


    void print()
    {
        cout << "#Task" << id_tarea << "-id_img: " << id_img << endl;
        cout << "#Task" << id_tarea << "-TTL_tarea: " << TTL_tarea << endl;
        cout << "#Task" << id_tarea << "-rtas_recibidas: " << rtas_recibidas << endl;
        //~ cout << "#Task"<<id_tarea<<"-tiempo_generacion: " << tiempo_generacion << endl;
        cout << "#Task" << id_tarea << "-tiempo_inicio: " << tiempo_inicio << endl;
        cout << "#Task" << id_tarea << "-tiempo_finalizacion: " << tiempo_finalizacion << endl;
        cout << "#Task" << id_tarea << "-reenviada: " << reenviada << endl;
        cout << "#Task" << id_tarea << "-consenso: " << consenso << endl;
        //~ cout << "#Task"<<id_tarea<<"-enviada al servidor 1: " << enviada_al_wse_1 << endl;
        //~ cout << "#Task"<<id_tarea<<"-enviada al servidor 2: " << enviada_al_wse_2 << endl;

        //~ printLista(peers_consultados_1,"peers_consultados_1",id_tarea);
        //~ printLista(peers_consultados_2,"peers_consultados_2",id_tarea);
        //~ printLista(peers_que_respondieron_1,"peers_que_respondieron_1",id_tarea);
        //~ printLista(peers_que_respondieron_2,"peers_que_respondieron_2",id_tarea);

        //~ cout<< "#"<<id_tarea<<"Mapa de votación: " << endl;
        //~ imprimir_mapa_votacion();
        cout << "#" << id_tarea << "---------------------------------" << endl;
    }

    void set_genT(double time)
    {
        tiempo_generacion = time;
    }

    void set_InitT(double time)
    {
        tiempo_inicio = time;
    }

    void set_FinalT(double time)
    {
        tiempo_finalizacion = time;
    }

    void set_Consenso(int _consenso)
    {
        consenso = _consenso;
    }
    void set_Reenviada(int _reenviada)
    {
        reenviada = _reenviada;
    }
    void set_Recibidas(int _recibidas)
    {
        rtas_recibidas = _recibidas;
    }
    void set_TTL(double _TTL)
    {
        TTL_tarea = _TTL;
    }
    void addRecibidas()
    {
        rtas_recibidas++;
    }

    int getTaskID()
    {
        return id_tarea;
    }
    int getImgID()
    {
        return id_img;
    }
    int getTTL()
    {
        return TTL_tarea;
    }
    int getOptionNr()
    {
        return nro_opciones;
    }
    int getRed()
    {
        return red_asignada;
    }
    int getReenviada()
    {
        return reenviada;
    }
    int getRecibidas()
    {
        return rtas_recibidas;
    }
    int getConsenso()
    {
        return consenso;
    }
    double getInitT()
    {
        return tiempo_inicio;
    }
    double getGenT()
    {
        return tiempo_generacion;
    }
    double getFinalT()
    {
        return tiempo_finalizacion;
    }
    map<int, list<int>>  getVotacion()
    {
        return votacion;
    }
    void resetVotingList()
    {
        votacion.clear();
    }
    
    list<int> getPeersConsultados(){
		return peers_consultados_1;
	}


	void setPeersConsultados(list<int> peers)
    {
        peers_consultados_1=peers;
    }
    
    
    void imprimir_mapa_votacion()
    {
        cout << "# ==============VOTACION:\n";
        for(map<int, list<int> >::const_iterator it = votacion.begin(); it != votacion.end(); ++it)
        {
            list<int> lista(it->second);
            cout << "Elemento: " << it->first << endl;
            printLista(lista, "---", id_tarea);
        }
        cout << "==============\n";
    }


    void printLista(std::list<int> &lista, const char *nombre, int indice)
    {
        std::list<int>::iterator it_lista;
        cout << "#" << indice << ". ==============Lista: " << nombre << "\n";
        for (it_lista = lista.begin(); it_lista != lista.end(); ++it_lista)
        {
            cout << ' ' << *it_lista;
        }
        cout << "\n#" << indice << ". ==============\n";
    }

    void accumulateAnswers(MessageServer *mS)
    {
        //~ imprimir_mapa_votacion();

        int id_peer = mS->getSrc();
        list<int>::iterator it1;	//Con it1 recorro la lista de opciones del mensaje que llega.
        int jj = 0;
        list<int> opciones = mS->getOpciones();
        //~ printLista(opciones, "accumulateAnswers - opciones en mensaje", id_tarea);
        for (it1 = opciones.begin(); it1 != opciones.end(); it1++)
        {
            //~ if ( (*it1) != 0)
            for ( int i = 0; i< (*it1); i++)		//!!! Importante: hay que acumular de esta forma por el peso de los expertos.
            {
                votacion[jj].push_back(id_peer);
            }
            jj++;
        }
        //~ imprimir_mapa_votacion();
    }

    void calculateConsensus(double umbral_votacion, int H)
    {
        //~ imprimir_mapa_votacion();
        for (int i = 0; i < (int)votacion.size(); i++)
        {
            if (0)
            {
                cout << "(OPCION <<" << i << "<< - NUMERO DE RESPUESTAS: " << votacion[i].size() << "\tUmbral: " << umbral_votacion << ", H: " << H << ", umbral*H: " << umbral_votacion *H << endl;
            }
            if ((int) votacion[i].size() > umbral_votacion * H )
            {
                //~ print_vec(votacion[i],"Votación para la opción");
                //~ printLista(votacion[i], "Votación para la opción", id_tarea);
                consenso = 1;
                //~ cout << "Tarea: "<< id_tarea<< ": HAY CONSENSO "<< endl;
            }
        }
        //~ imprimir_mapa_votacion();
    }


};


#endif
