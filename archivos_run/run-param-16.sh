#!/bin/bash
# Queries  CacheSize LocalCacheSize LCacheSize Peers LeafsetSize DAG Users Thao REP_STRATEGY PEER_SELECTION CASE_RESPONSABLE

LCACHE=50
USERS=0
THAO=0.5
RS=2 #REPLICATION STRATEGY  0:Leafset  1:Bubble  2:TLC
PS=0 #PEER SELECTION  0:Uniform  1:Zipf
CR=0 #CASE RESPONSABLE 0:

name=`basename "$0"`

#ITERACION=1

while read CACHE LOCALCACHE LCACHE QUERIES NORMAL_RATE PEERS_ENVIO PEERS TTL
do
	#for i in {1..3}	#Iteraciones
	for i in 1	#Iteraciones
	do
		for s in 30 #CTE_TRABAJO_PEERS
		do	
			for u in 60 #UMBRAL_VOTACION
			do		
				for v in 0.001 #INTER_ARR_T
				do			
					#CACHE=0
					#LOCALCACHE=0
					#LCACHE=0				
					ITERACION=$i
					#./Simulador $QUERIES $CACHE $LOCALCACHE $LCACHE $PEERS 16 ../../MAQUINABSP/log_queries2.txt $USERS $THAO $RS $PS $CR $PEERS_ENVIO $ITERACION $NORMAL_RATE $TTL $RATE_TRABAJO $CTE_WSE >> "resultado-$name-`hostname`.out" #&
					CTE_TRABAJO_PEERS=$s
					CTE_HOLD_WSE=10
					UMBRAL_VOTACION=$u
					INTER_ARR_T=$v
					./Simulador $QUERIES $CACHE $LOCALCACHE $LCACHE $PEERS 16 ../../MAQUINABSP/log_queries2.txt $USERS $THAO $RS $PS $CR $PEERS_ENVIO $ITERACION $NORMAL_RATE $TTL $CTE_TRABAJO_PEERS $CTE_HOLD_WSE $UMBRAL_VOTACION $INTER_ARR_T >> "resultado-$name-`hostname`.out" #&
				done
			done
		done
	done
done < values16.txt 

echo "*** FINAL ***" >> "resultado-$name-`hostname`.out" #&

#./Simulador $QUERIES $CACHE $LOCALCACHE $LCACHE $PEERS 16 ../../MAQUINABSP/log_queries2.txt $USERS $NORMAL_RATE $RS $PS $CR $PEERS_ENVIO $ITERACION
#> "results/result-$NORMAL_RATE-$PEERS-$CACHE-$CACHE-`hostname`.out" &

