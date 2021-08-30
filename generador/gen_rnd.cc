#include "gen_rnd.h"
#include "../applicationLayer/TlcProtocol.h"
#include "../applicationLayer/Query.h"
#include <stdlib.h>
#include <ctime>
#include "../wse/MessageServer.h"


void Gen_rnd::inner_body( )
{
    string line, terms;
    int id = 0;

    const char *buffer;
    // cout << "Archivo Trazas: " << traces_file << " - Get_Query.h" << endl;
    endStream.open( traces_file );

    int id_img;
    int id_tarea;
    double TTL = 0;

    Mje_Gen_WSE *m;
    list<int> opciones;
    int prioridad;

    //La primera linea se tira si se usa el Log de Yahoo
    getline( endStream, line );

    //~ int indice_frecuencia=0;
    //~ double CTE_acumulado=0;

    int nro_opciones;
    double newRate = (1.0 / normal_rate);
    //~ delete arrival_time;
    arrival_time = new rngExp("Arrive Time", newRate);				//Para el arribo según la exponencial decreciente.
    arrival_time->reset();


    // ofstream archivo_trazas_debug("archivo_trazas_debug.txt");
    int min_id_img = 1;
    int max_id_img = 50000;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib_id_img(min_id_img, max_id_img);		//En este caso la distribución incluye al máximo

    int min_opciones = 1;
    int max_opciones = 6;
    std::random_device rdOp;  //Will be used to obtain a seed for the random number engine
    std::mt19937 genOp(rdOp()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib_opciones(min_opciones, max_opciones);		//En este caso la distribución incluye al máximo

    ////=================================================================================================================================================
    ////(CODIGO QUE USE PARA SOLUCIONAR LOS ERRORES DE DEE COPY PARA LAS VOTACIONES, CONSENSO, TP, etc.)
    ////GENERADOR COMO EL DE JAVA.
    //for (int i=0;i<*totalQueries;i++){
    //	id_tarea=i;
    //	id_img = distrib_id_img(gen);
    //	//id_img = ThreadLocalRandom.current().nextInt(min, max + 1);
    //
    //	//~ prioridad_tarea = ThreadLocalRandom.current().nextInt(min, max + 1);
    //	prioridad = 0;
    //	//~ int esperadas = H;
    //	//~ ttl = Util.TTL;			//Es constante
    //	//~ double ttl = parametro_TTL;			//Es constante
    //	double tiempo_inicio=0;
    //	double tiempo_generacion=0;
    //
    //	//~ int nro_opciones = ThreadLocalRandom.current().nextInt(min, max + 1);
    //	int nro_opciones = distrib_opciones(genOp);;
    //	list<int> opciones;
    //	for (int i=0;i<nro_opciones;i++){
    //		opciones.push_back(i);
    //	}
    //
    //	//~ BigInteger hash = Util.intToBI(id_tarea);
    //	//~ m.body=new TaskContainer(id_tarea,id_img,ttl,esperadas, prioridad_tarea, opciones_originales, tiempo_inicio, tiempo_generacion, hash);
    //	//~ m.body.PrintTaskContainer();
    //
    //	m = new  Mje_Gen_WSE(id_tarea, id_img,TTL, opciones, prioridad,0);
    //	(*wse)->add_request_Gen(m);
    //	if ((*wse)->idle() && !(*wse)->get_busy())
    //		(*wse)->activateAfter(current());
    //	(*observer)->addNQueriesOut();
    //
    //	sentQueries++;
    //	//~ cout << "Time: " << this->time() <<". SentQueries: "<<sentQueries<< ". totalQueries: "<< *totalQueries<< endl;
    //	if (sentQueries >= (*totalQueries) )     {
    //		endStream.close();
    //		(*wse)->gen_stop=1;
    //		passivate();
    //	}
    //
    //	CTE=0;
    //	hold(CTE);
    //	id++;
    //}
    //=================================================================================================================================================



    string ptr;
	std::stringstream str;
    BIGNUM *hashValue ;

    while( 1 )
    {
        if( ! getline( endStream, line ) )
        {
            (*wse)->gen_stop = 1;
            passivate();
        }
        //~ debug(time(), "Start preparing message");
        prev = actual;
        actual = this->time();
        //double time = actual - prev;

        tokens.clear();
        util->Tokenize( line, tokens, "\t");
        
        buffer = tokens[0].c_str();
        id_tarea = atoi (buffer);

        buffer = tokens[1].c_str();
        id_img = atoi (buffer);

        //buffer = tokens[2].c_str();
		//TTL = atof (buffer);
		
		double lower_bound = parametro_TTL * 0.95; 		//TTL - Para crear el random flotante (double), uso un generador de libcppsim.
		double upper_bound = parametro_TTL * 1.05;
		rng<double> *generador_double = new rngUniform( "Uniform", lower_bound, upper_bound );
        double TTL = generador_double->value();

        opciones.clear();
        buffer = tokens[3].c_str();
        nro_opciones = atoi (buffer);
        for (int i = 0; i < nro_opciones; i++)
        {
            opciones.push_back(i);
        }
        if ((rand() % 100) < 50)
            prioridad = 0; //Prioridad desactivada entonces usa TTL
        else
            prioridad = 1; //Prioridad activada

        //~ cout << "Generator - Time: " << this->time() <<". Prepare message"<< endl;
        //--------------------------------------------------------------------------------------------------ENVIO ANTERIOR
        //m = new  Mje_Gen_WSE(id_tarea, id_img,TTL, opciones, prioridad,this->time());
        //(*wse)->add_request_Gen(m);
        //if ((*wse)->idle() && !(*wse)->get_busy())
        //	(*wse)->activateAfter(current());
        //(*observer)->addNQueriesOut();
        //--------------------------------------------------------------------------------------------------ENVIO ANTERIOR

        //--------------------------------------------------------------------------------------------------ENVIO ACTUAL
        //~ str.str(std::string());			str << id_img;			str.str();
        //~ hashValue = h->GenerateKey((char*)(str.str()).c_str());		ptr=(char*)(str.str()).c_str();
        //~ MessageServer* mS = new MessageServer(GEN_2_SERVER,sentQueries, id_img,hashValue, TTL, 0,0, 6,-1);
        MessageServer *mS = new MessageServer(GEN_2_SERVER, sentQueries, id_img, TTL, 0, 0, nro_opciones, -1);	//Sin Hash en query
        mS->set_genT(time());
        //~ mS->print();			//~ mS->print_opciones();
        (*wse)->add_request_Gen2(mS);
        if ((*wse)->idle() && !(*wse)->get_busy())
            (*wse)->activateAfter(current());
        (*observer)->addNQueriesOut();
        //--------------------------------------------------------------------------------------------------ENVIO ACTUAL

        //~ cout << "generator - Time: " << this->time() << ". SentQueries: " << sentQueries << ". totalQueries: " << *totalQueries << ". Server busy: " << (*wse)->get_busy() << endl;
        sentQueries++;

        if (sentQueries >= (*totalQueries) )
        {
            //~ cout << "generator - Time: " << this->time() << ". All queries sent" << endl;
            endStream.close();
            (*wse)->gen_stop = 1;
            passivate();
        }





        /*****************************************************************************************************/
        ////Descomentar esto para tener NR variable en la simulación. Experimentos transversales
        //if (sentQueries > (*totalQueries*0.3)) {newRate = newRate *0.25;}
        //if (sentQueries > (*totalQueries*0.6)) {newRate = newRate *4.0;}
        /*****************************************************************************************************/
        //Descomentar esto para usar NR como en el simulador original (NR FIJA)

        CTE = arrival_time->value();
        //~ cout << "CTE: "<< CTE << ", CTE en us: "<<CTE*3600*1000000 << endl;
        /*****************************************************************************************************/
        ////Descomentar esto para tener ráfagas de datos. (PICOS Y VALLES EN CTE)
        //if ( sentQueries % ( ((int)(*totalQueries))/10) == 0 ){		//Para las ráfagas.
        //	CTE=15.0;
        //}

        //~ CTE=2.77777777777778E-08;
        //~ CTE = 100;
        //~ CTE = 0;
        //~ debug(time(), "sentqueries: "+to_string(sentQueries));
        /*****************************************************************************************************/
        //Descomentar para configurar TIEMPO ENTRE ARRIBOS en lugar de tasa de arribo - CTE FIJO.
        //~ CTE=interArrT;														//Para medir el tiempo de resolución de tareas promedio del sistema sin carga.
        //cout << "Tiempo entre consultas ( CTE=rngExp(1/normal_rate) ): " << CTE << "\n" << endl;
        /*****************************************************************************************************/
        //Descomentar para tener el tiempo entre arribos gobernado por la distribución Weibull que imita a la bibliografía de desastres naturales.
        //if ( sentQueries > f_acumuladas[indice_frecuencia] ){
        //	indice_frecuencia++;
        //	//~ cout << "indice_frecuencia: "<<indice_frecuencia << endl;
        //}
        //while (frecuencias_entrada[indice_frecuencia] == 0) {
        //	hold(3600);
        //
        //	CTE_acumulado+=3600;
        //	// archivo_trazas_debug << "Tarea: " << sentQueries << ". nQ: " << *totalQueries<< ". f_acumuladas[indice_frecuencia]: " << f_acumuladas[indice_frecuencia] << ". Indice: " << indice_frecuencia<< "/" << frecuencias_entrada.size()<< ". Frecuencia: " << frecuencias_entrada[indice_frecuencia] << ". IAT: " << CTE << ". Hora: " << (double)CTE_acumulado/(double)3600.0<< ". Día: " << (double)CTE_acumulado/(double)86400.0 << ". Time(hours)"<< (double)time()/3600.0 << ". Time(days)"<<(double)time()/(3600.0*24.0)<< endl;
        //	//~ cout << "indice_frecuencia: "<<indice_frecuencia << endl;
        //	indice_frecuencia++;
        //}
        ////CTE=(double)7200.0/frecuencias_entrada[indice_frecuencia];
        //CTE=(double)3600.0/frecuencias_entrada[indice_frecuencia];
        //
        //// archivo_trazas_debug << "Tarea: " << sentQueries << ". nQ: " << *totalQueries<< ". f_acumuladas[indice_frecuencia]: " << f_acumuladas[indice_frecuencia] << ". Indice: " << indice_frecuencia<< "/" << frecuencias_entrada.size()<< ". Frecuencia: " << frecuencias_entrada[indice_frecuencia] << ". IAT: " << CTE << ". Hora: " << (double)CTE_acumulado/(double)3600.0<< ". Día: " << (double)CTE_acumulado/(double)86400.0 << ". Time(hours)"<< (double)time()/3600.0 << ". Time(days)"<<(double)time()/(3600.0*24.0)<< endl;
        /*****************************************************************************************************/
        //~ cout << "generator - Time: " << this->time() << ". start hold, cte=" << CTE << endl;
        hold( CTE );
        //~ CTE_acumulado+=CTE;
        
		//~ cout << "Tarea: " << id << " generada en T = " << this->time() << endl;
        

        id++;
    }



    //endStream.close();
}

int Gen_rnd::getZipf (int alpha, int n)
{

    static bool first = true;      // Static first time flag
    static double c = 0;          // Normalization constant
    double z;                     // Uniform random number (0 < z < 1)
    double sum_prob;              // Sum of probabilities
    double zipf_value = 0;          // Computed exponential value to be returne

    // Compute normalization constant on first call only
    if (first == true)
    {
        for (int i = 1; i <= n; i++)
            c = c + (1.0 / pow((double) i, alpha));
        c = 1.0 / c;
        first = false;
        //cout << "PRIMERO" << endl;
    }

    // Pull a uniform random number (0 < z < 1)
    do
    {
        z = (((double) rand()) / (RAND_MAX + 1.0));
        // cout << "Z VALUE " << z << endl;
    }
    while ((z == 0) || (z == 1));
    // Map z to the value
    sum_prob = 0;
    for (int i = 1; i <= n; i++)
    {
        //      cout <<"Z " << z << endl;
        //     cout << "SUM PROB: " << sum_prob << endl;
        //      cout << "C " << c <<endl;
        //      cout << "ALPHA " << alpha << endl;
        sum_prob = sum_prob + (c / pow((double) i, alpha));
        if (sum_prob >= z)
        {
            zipf_value = i;
            break;
        }
    }
    //   cout << "ZIPF VALUE " << zipf_value << endl;
    // Assert that zipf_value is between 1 and N
    ASSERT((zipf_value >= 1) && (zipf_value <= n));
    return zipf_value ;
}


void Gen_rnd::setQueryRate( int newRate)
{
    // newRate queries/sec
    switch (QUERY_RATE_STRATEGY)
    {
    // DELTA T TIME
    case 0:
        if(this->time() > QUERY_DELTA_T )
        {
            double lambda = (1.0 / newRate);
            delete arrival_time;
            arrival_time = new rngExp("Arrive Time", lambda);
            arrival_time ->reset();
        }
        break;

    // Q QUERIES
    case 1:
        if( (lastStepQueries + QUERY_DELTA_Q) < sentQueries)
        {
            lastStepQueries = sentQueries;
            double lambda = (1.0 / newRate);
            delete arrival_time;
            arrival_time = new rngExp("Arrive Time", lambda);
            arrival_time ->reset();
        }
        break;
    }
}


double Gen_rnd::rand_val(int seed)
{
    const long a = 16807; // Multiplier
    const long m = 2147483647;// Modulus
    const long q = 127773;// m div a
    const long r = 2836;// m mod a
    static long x; // Random int value
    long x_div_q; // x divided by q
    long x_mod_q; // x modulo q
    long x_new; // New x value

    // Set the seed if argument is non-zero and then return zero
    if (seed > 0)
    {
        x = seed;
        return(0.0);
    }
    //
    // // RNG using integer arithmetic
    x_div_q = x / q;
    x_mod_q = x % q;
    x_new = (a * x_mod_q) - (r * x_div_q);
    if (x_new > 0)
        x = x_new;
    else
        x = x_new + m;
    //
    // // Return a random value between 0.0 and 1.0
    return((double) x / m);
    // }


}

void Gen_rnd::generar_distribucion(vector<double> &arr_frec, vector<double> &arr_frec_acc)
{
    int nrolls = *totalQueries;
    int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::weibull_distribution<double> distribution(3, 3.5);	//Con el segundo parámetro se consigue controlar la duración del evento. Un parámetro menor comprime la gráfica horizontalmente.
    //~ cout <<  "Empiezo: "  << std::endl;
    //~ std::ofstream archivo_salida;
    //~ archivo_salida.open ("salida_F.csv");

    int p[8] = {};
    int f_por_dia = 0;
    for (int i = 0; i < nrolls; ++i)
    {
        double number = distribution(generator);
        if (0 < number && number < 8)
        {
            ++p[int(number)];
            f_por_dia++;
        }
        else
        {
            i--;
        }
    }
    //~ archivo_salida << "day" <<";" << "F" << ";F_acc"<< std::endl;
    int F_acc = 0;
    for (int j = 0; j < 8; j++)
    {
        F_acc += p[j];
        //~ archivo_salida << j<< ";"<< p[j]<<";"<< F_acc <<  std::endl;
    }

    //~ archivo_salida.close();

    //~ archivo_salida.open ("salida_f.csv");
    //~ archivo_salida << "day_" <<";" << "f"<< ";f_acc" << std::endl;
    int f_acc = 0;
    //for para encontrar las distribuciones de cada día (2 x día)
    for (int j = 0; j < 8; ++j)
    {
        int nrolls2;
        for (int k = 0; k < 2; k++)
        {
            int cantidad = p[j] / 4.0;
            if (k % 2 == 0)
            {
                nrolls2 = cantidad;		// p[j]*1/4
            }
            else
            {
                nrolls2 = p[j] - cantidad; // p[j]*3/4
            }
            //~ cout << "Día :" << j << ". Mitad: " << k%2 << ". nrolls2=" << nrolls2<< endl;
            std::weibull_distribution<double> distribution(1.5, 10);
            int p2[12] = {};
            int f_por_dia_J = 0;
            for (int i = 0; i < nrolls2; ++i)
            {
                double number2 = distribution(generator);
                if (0 <= number2 && number2 < 12)
                {
                    ++p2[int(number2)];
                    f_por_dia_J++;
                }
                else
                {
                    i--;
                }
            }
            //~ cout <<  "f_por_dia " << j <<":" << f_por_dia_J << std::endl;
            for (int i = 0; i < 12; ++i)
            {
                f_acc += p2[i];
                //~ cout <<  ( (24*j) + k*12 + ((i)) ) /24.0 <<";" << p2[i] << std::endl;		//Acá se imprime el punto de muestra y la frecuencia correspondiente.
                //~ archivo_salida <<  ( (24*j) + k*12 + ((i)) ) /24.0 <<";" << p2[i] << ";" << f_acc << std::endl;
                frecuencias_entrada.push_back(p2[i]);	//Ingreso la frecuencia al arreglo de frecuencias.
            }//std::cout << std::endl;


        }
    }
    int frecuencias_total = 0;
    for (int i = 0; i < (int)frecuencias_entrada.size(); i++)
    {
        frecuencias_total += frecuencias_entrada[i];
        if (i == 0)
        {
            f_acumuladas.push_back( frecuencias_entrada[i] );
        }
        else
        {
            f_acumuladas.push_back( frecuencias_entrada[i] +  f_acumuladas[i - 1] );
        }
    }
    //~ cout << "frecuencias_total: " << frecuencias_total<< endl;


}

void Gen_rnd::debug(double time, string msg  )
{
    if (0) cout << "[" << time << "][Gen_rnd:" << name() << "][ " << msg << " ]" << endl;
}
