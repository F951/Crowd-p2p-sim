#ifndef Rtas_Peers_H
#define Rtas_Peers_H
#include "../glob.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "../p2pLayer/PastryMessage.h"



class Rtas_Peers
{
public:
    int consenso;
    int id_tarea;
    int id_img;
    double TTL_tarea;
    int rtas_esperadas;
    int rtas_recibidas;
    int prioridad_tarea;
    double tiempo_generacion;
    double tiempo_inicio;
    double tiempo_finalizacion;
    list<int> opciones;
    list<int> opciones_originales;
    map<int, list<int> > votacion;
    list<int> peers_consultados_1;
    list<int> peers_consultados_2;
    list<int> peers_que_respondieron_1;
    list<int> peers_que_respondieron_2;
    vector<double> TimeStamps;
    vector<int> contadores_TimeStamps;
    NodeEntry *Responsable_agreg;
    int responsable_asignado = 0;
    int responsable_ingresado = 0;			//Uso esto como flag para determinar si el responsable puede o no accederse.
    int reenviada;
    int enviada_al_wse_1;
    int enviada_al_wse_2;
    int red_asignada;
    int flag_primer_mensaje;			//Para distinguir, en el caso de las historias, a qué mensaje se le aplica la latencia de red en el ISP.

    Rtas_Peers()
    {
        for (int i = 0; i < 17; i++)
        {
            TimeStamps.push_back(0);
            contadores_TimeStamps.push_back(0);
        }
    }

    ~Rtas_Peers()
    {
        //free( src);
        //~ free(Responsable_agreg);
        //~ if (responsable_ingresado==1){
        //~ delete Responsable_agreg;
        //~ }

        //~ free(opciones_originales);
        //~ free(votacion);
        //~ free(peers_consultados_1);
        //~ free(peers_consultados_2);
        //~ free(peers_que_respondieron_1);
        //~ free(peers_que_respondieron_2);
        //~ delete opciones;
        //~ delete opciones_originales;
        //~ delete votacion;
        //~ delete peers_consultados_1;
        //~ delete peers_consultados_2;
        //~ delete peers_que_respondieron_1;
        //~ delete peers_que_respondieron_2;
        //~ opciones.clear();
        //~ opciones_originales.clear();
        //~ votacion.clear();
        //~ peers_consultados_1.clear();
        //~ peers_consultados_2.clear();
        //~ peers_que_respondieron_1.clear();
        //~ peers_que_respondieron_2.clear();
    }

    Rtas_Peers(int _id_tarea, int _id_img, double _ttl, int _esp, int _prioridad_tarea, list<int> &_opciones_originales, double _tiempo_inicio, double _tiempo_generacion)
    {
        id_tarea			= _id_tarea;
        id_img 				= _id_img;
        TTL_tarea    		= _ttl;
        rtas_esperadas 		= _esp;
        rtas_recibidas 		= 0;
        prioridad_tarea		= _prioridad_tarea;
        consenso = 0;
        reenviada = 0;
        enviada_al_wse_1 = 0;
        enviada_al_wse_2 = 0;
        for (list<int>::iterator it = _opciones_originales.begin(); it != _opciones_originales.end(); it++)
        {
            opciones.push_back(0);
            opciones_originales.push_back(*it);
        }
        tiempo_inicio		= _tiempo_inicio;
        tiempo_generacion	= _tiempo_generacion;
        responsable_asignado = 0;
        //~ Responsable_agreg=new NodeEntry(NULL,-1);

        for (int i = 0; i < 17; i++)
        {
            TimeStamps.push_back(0);
            contadores_TimeStamps.push_back(0);
        }

    }

    //~ void copy(Rtas_Peers* & rta_in)
    void copy(Rtas_Peers *rta_in)
    {
        id_tarea = rta_in->id_tarea;
        consenso = rta_in->consenso;
        id_img = rta_in->id_img;
        TTL_tarea = rta_in->TTL_tarea;
        rtas_esperadas = rta_in->rtas_esperadas;
        rtas_recibidas = rta_in->rtas_recibidas;
        prioridad_tarea = rta_in->prioridad_tarea;
        reenviada = rta_in->reenviada;
        tiempo_generacion = rta_in->tiempo_generacion;
        tiempo_inicio = rta_in->tiempo_inicio;
        tiempo_finalizacion = rta_in->tiempo_finalizacion;
        votacion = rta_in->votacion;

        responsable_ingresado = rta_in->responsable_ingresado;

        red_asignada = rta_in->red_asignada;
        flag_primer_mensaje = rta_in->flag_primer_mensaje;
        enviada_al_wse_1 = rta_in->enviada_al_wse_1;
        enviada_al_wse_2 = rta_in->enviada_al_wse_2;
        std::list<int>::iterator it_lista;
        //~ cout << "DEBUG copy rta - intento copiar Responsable_agreg"<< endl;

        if (rta_in->responsable_ingresado == 1)
        {
            //~ cout << "DEBUG copy rta - intento copiar Responsable_agreg 2"<< endl;
            Responsable_agreg = new NodeEntry(rta_in->Responsable_agreg->getNodeID(), rta_in->Responsable_agreg->getIP());
        }


        //if (rta_in->Responsable_agreg!=NULL){
        //	//~ cout << "DEBUG copy rta - intento copiar Responsable_agreg 1"<< endl;
        //	if (rta_in->responsable_ingresado==1){
        //		//~ cout << "DEBUG copy rta - intento copiar Responsable_agreg 2"<< endl;
        //		Responsable_agreg=new NodeEntry(rta_in->Responsable_agreg->getNodeID(),rta_in->Responsable_agreg->getIP());
        //	}
        //}

        //~ cout << "DEBUG copy rta - copiar Responsable_agreg OK"<< endl;
        for (it_lista = rta_in->peers_consultados_1.begin(); it_lista != rta_in->peers_consultados_1.end(); it_lista++)
        {
            peers_consultados_1.push_back(*it_lista);
        }
        for (it_lista = rta_in->peers_consultados_2.begin(); it_lista != rta_in->peers_consultados_2.end(); it_lista++)
        {
            peers_consultados_2.push_back(*it_lista);
        }
        for (it_lista = rta_in->peers_que_respondieron_1.begin(); it_lista != rta_in->peers_que_respondieron_1.end(); it_lista++)
        {
            peers_que_respondieron_1.push_back(*it_lista);
        }
        for (it_lista = rta_in->peers_que_respondieron_2.begin(); it_lista != rta_in->peers_que_respondieron_2.end(); it_lista++)
        {
            peers_que_respondieron_2.push_back(*it_lista);
        }
        for (it_lista = rta_in->opciones.begin(); it_lista != rta_in->opciones.end(); it_lista++)
        {
            opciones.push_back(*it_lista);
        }
        for (it_lista = rta_in->opciones_originales.begin(); it_lista != rta_in->opciones_originales.end(); it_lista++)
        {
            opciones_originales.push_back(*it_lista);
        }
        for (int i = 0; i < 17; i++)
        {
            TimeStamps[i] = rta_in->TimeStamps[i];
            contadores_TimeStamps[i] = rta_in->contadores_TimeStamps[i];
        }
        //~ cout << "DEBUG copy rta - copia Lista"<< endl;

        //cout << "RTA - DEBUG 1 "<< endl;
        //for (int i=0; i<17; i++){
        //	TimeStamps[i]=rta_in.TimeStamps[i];
        //	contadores_TimeStamps[i]=rta_in.contadores_TimeStamps[i];
        //}
        //
        //id_tarea=rta_in.id_tarea;
        //consenso=rta_in.consenso;
        //id_img=rta_in.id_img;
        //TTL_tarea=rta_in.TTL_tarea;
        //rtas_esperadas=rta_in.rtas_esperadas;
        //rtas_recibidas=rta_in.rtas_recibidas;
        //prioridad_tarea=rta_in.prioridad_tarea;
        //reenviada=rta_in.reenviada;
        //tiempo_generacion=rta_in.tiempo_generacion;
        //tiempo_inicio=rta_in.tiempo_inicio;
        ////tiempo_inicio=rta_in.tiempo_inicio;
        //tiempo_finalizacion=rta_in.tiempo_finalizacion;
        //cout << "RTA - DEBUG 2 "<< endl;
        ////Duplicar las listas de opciones y de peers consultados (para los casos en que haga falta).
        //std::list<int>::iterator it_lista;
        //
        //for (it_lista=rta_in.peers_consultados_1.begin(); it_lista!=rta_in.peers_consultados_1.end(); it_lista++){
        //	peers_consultados_1.push_back(*it_lista);
        //}
        //cout << "RTA - DEBUG 3 "<< endl;
        //
        //for (it_lista=rta_in.peers_consultados_2.begin(); it_lista!=rta_in.peers_consultados_2.end(); it_lista++){
        //	peers_consultados_2.push_back(*it_lista);
        //}
        //
        //
        //for (it_lista=rta_in.peers_que_respondieron_1.begin(); it_lista!=rta_in.peers_que_respondieron_1.end(); it_lista++){
        //	peers_que_respondieron_1.push_back(*it_lista);
        //}
        //
        //
        //for (it_lista=rta_in.peers_que_respondieron_2.begin(); it_lista!=rta_in.peers_que_respondieron_2.end(); it_lista++){
        //	peers_que_respondieron_2.push_back(*it_lista);
        //}
        //
        //
        //for (it_lista=rta_in.opciones.begin(); it_lista!=rta_in.opciones.end(); it_lista++){
        //	opciones.push_back(*it_lista);
        //}
        //
        //
        //for (it_lista=rta_in.opciones_originales.begin(); it_lista!=rta_in.opciones_originales.end(); it_lista++){
        //	opciones_originales.push_back(*it_lista);
        //}
        //
        //votacion=rta_in.votacion;
        //red_asignada=rta_in.red_asignada;
        //flag_primer_mensaje=rta_in.flag_primer_mensaje;
    }

    void print_rta()
    {
        cout << "#" << this->id_tarea << "------------------ id_tarea ------#: " << this->id_tarea << endl;
        cout << "#" << this->id_tarea << "id_img: " << this->id_img << endl;
        cout << "#" << this->id_tarea << "TTL_tarea: " << this->TTL_tarea << endl;
        cout << "#" << this->id_tarea << "rtas_esperadas: " << this->rtas_esperadas << endl;
        cout << "#" << this->id_tarea << "rtas_recibidas: " << this->rtas_recibidas << endl;
        cout << "#" << this->id_tarea << "prioridad_tarea: " << this->prioridad_tarea << endl;
        cout << "#" << this->id_tarea << "tiempo_generacion: " << this->tiempo_generacion << endl;
        cout << "#" << this->id_tarea << "tiempo_inicio: " << this->tiempo_inicio << endl;
        cout << "#" << this->id_tarea << "tiempo_finalizacion: " << this->tiempo_finalizacion << endl;
        cout << "#" << this->id_tarea << "reenviada: " << this->reenviada << endl;
        cout << "#" << this->id_tarea << "enviada al servidor 1: " << this->enviada_al_wse_1 << endl;
        cout << "#" << this->id_tarea << "enviada al servidor 2: " << this->enviada_al_wse_2 << endl;
        printLista(opciones, "opciones", id_tarea);
        //~ printLista(peers_consultados_1,"peers_consultados_1",this->id_tarea);
        //~ printLista(peers_consultados_2,"peers_consultados_2",this->id_tarea);
        //~ printLista(peers_que_respondieron_1,"peers_que_respondieron_1",this->id_tarea);
        //~ printLista(peers_que_respondieron_2,"peers_que_respondieron_2",this->id_tarea);

        //~ cout<< "#"<<this->id_tarea<<"Mapa de votación: " << endl;
        //~ imprimir_mapa_votacion();
        cout << "#" << this->id_tarea << "---------------------------------" << endl;
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
        cout << "#" << indice << ". ";
        for (it_lista = lista.begin(); it_lista != lista.end(); ++it_lista)
        {
            //~ std::cout << ' ' << *it_lista;
            cout << ' ' << *it_lista;
        }
        cout << "\n#" << indice << ". ==============\n";
    }


};


#endif
