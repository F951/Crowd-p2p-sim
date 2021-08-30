#!/bin/bash


THAO=0.5
#RS=2 #REPLICATION STRATEGY  0:Leafset  1:Bubble  2:TLC
PS=0 #PEER SELECTION  0:Uniform  1:Zipf
#CR=0 #CASE RESPONSABLE 0:

name=`basename "$0"`

CASO=$1
mkdir -p Resultados_transversales_$CASO
rm Resultados_transversales/*

for ITERACION in 1	#Iteraciones
do
for ESCENARIO in 1 #ESCENARIO											
do	
#for ARCH in  {0,1,2,3,4} #ARQUITECTURA
for ARCH in 0 #ARQUITECTURA
do	
	#for QUERIES in 5000 #QUERIES
	#for QUERIES in {300,3000} #QUERIES
	for QUERIES in {1000,3000} #QUERIES
	do	
		for PEERS in 500 #PEERS
		#for PEERS in {250,500,750,1000} #PEERS
		#for PEERS in {500,1000} #PEERS
		do		
			for NORMAL_RATE in 1137283 #NORMAL_RATE (TASA DE ARRIBO)
			#for NORMAL_RATE in 10 #NORMAL_RATE (TASA DE ARRIBO)
			do	
				for H in 30 #H 
				#for H in {10,30,50} #H 
				#for H in {10,30} #H 
				do		
				for CACHE in 30 #CACHE
				do	
				for LOCALCACHE in 30 #LOCALCACHE
				do
				for LCACHE in 150 #LCACHE
				do
					for TTL in 72 #TTL
					#for TTL in 10000000 #TTL
					#for TTL in {72,72,72,72} #TTL
					do			
						for UMBRAL_VOTACION in 50 #UMBRAL_VOTACION
						#for UMBRAL_VOTACION in {10,30,50,70,90} #UMBRAL_VOTACION
						#for UMBRAL_VOTACION in {30,70} #UMBRAL_VOTACION
						do		
							for CTE_TRAFFIC_ISP in 2 #CTE_TRAFFIC_ISP
							do		
							for CTE_TRAFFIC_P2P in 2 #CTE_TRAFFIC_ISP
							do								
							for ALLOW_DISC in {0,1} #Permitir desconexión de voluntarios por descanso.
							do								
							for MAX_WORK_TIME in {0.1,0.3,0.5,0.7,0.9} #CTE_TRAFFIC_ISP
							do	
							for HISTORIAS in {0,1} #Permitir desconexión de voluntarios por descanso.
							do															
#time ./Simulador $QUERIES $CACHE $LOCALCACHE $LCACHE $PEERS 16 ../../MAQUINABSP/log_queries_10002.txt $USERS $THAO $RS $PS $CR $H $ITERACION $NORMAL_RATE $TTL $CTE_TRABAJO_PEERS $CTE_ATENCION $UMBRAL_VOTACION $INTER_ARR_T $CASO $ESCENARIO $CTE_TRAFFIC_ISP $CTE_TRAFFIC_P2P $ARCH	#>> "Resultados_transversales/resultado-$name-caso$CASO-Arch$ARCH-peers$PEERS-nQ$QUERIES-H$H-TTL$TTL-esc$ESCENARIO-thr$UMBRAL_VOTACION-NR$NORMAL_RATE-`hostname`.out" #&		
./Simulador $QUERIES $CACHE $LOCALCACHE $LCACHE $PEERS 16 ../../MAQUINABSP/log_queries_10002.txt $THAO $H $ITERACION $NORMAL_RATE $TTL $UMBRAL_VOTACION $CASO $ESCENARIO $CTE_TRAFFIC_ISP $CTE_TRAFFIC_P2P $ARCH $ALLOW_DISC $MAX_WORK_TIME $HISTORIAS # >> "Resultados_transversales_$CASO/resultado-$name-caso$CASO-Arch$ARCH-peers$PEERS-nQ$QUERIES-H$H-TTL$TTL-esc$ESCENARIO-thr$UMBRAL_VOTACION-NR$NORMAL_RATE-`hostname`.out" #&		
#valgrind --leak-check=yes -v --xtree-memory=yes ./Simulador $QUERIES $CACHE $LOCALCACHE $LCACHE $PEERS 16 ../../MAQUINABSP/log_queries_10002.txt $USERS $THAO $RS $PS $CR $H $ITERACION $NORMAL_RATE $TTL $CTE_TRABAJO_PEERS $CTE_ATENCION $UMBRAL_VOTACION $INTER_ARR_T $CASO $ESCENARIO $CTE_TRAFFIC_ISP $CTE_TRAFFIC_P2P $ARCH	#>> "Resultados_transversales/resultado-$name-caso$CASO-Arch$ARCH-peers$PEERS-nQ$QUERIES-H$H-TTL$TTL-esc$ESCENARIO-thr$UMBRAL_VOTACION-NR$NORMAL_RATE-`hostname`.out" #&		
#valgrind --leak-check=yes -v --xtree-leak=yes ./Simulador $QUERIES $CACHE $LOCALCACHE $LCACHE $PEERS 16 ../../MAQUINABSP/log_queries_10002.txt $USERS $THAO $RS $PS $CR $H $ITERACION $NORMAL_RATE $TTL $CTE_TRABAJO_PEERS $CTE_ATENCION $UMBRAL_VOTACION $INTER_ARR_T $CASO $ESCENARIO $CTE_TRAFFIC_ISP $CTE_TRAFFIC_P2P $ARCH	#>> "Resultados_transversales/resultado-$name-caso$CASO-Arch$ARCH-peers$PEERS-nQ$QUERIES-H$H-TTL$TTL-esc$ESCENARIO-thr$UMBRAL_VOTACION-NR$NORMAL_RATE-`hostname`.out" #&		
								#echo "Resultados_transversales/resultado-$name-caso$CASO-peers$PEERS-nQ$QUERIES-H$H-TTL$TTL-esc$ESCENARIO-thr$UMBRAL_VOTACION-`hostname`.out"
													

								done
								done
								done
								done
							done
						done
					done
				done
				done
				done
				done
			done
		done
	done
done
done
done

echo "*** FINAL ***" >> "resultado-$name-`hostname`.out" #&

#./Simulador $QUERIES $CACHE $LOCALCACHE $LCACHE $PEERS 16 ../../MAQUINABSP/log_queries2.txt $USERS $NORMAL_RATE $RS $PS $CR $H $ITERACION
#> "results/result-$NORMAL_RATE-$PEERS-$CACHE-$CACHE-`hostname`.out" &

