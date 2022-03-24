all:
	g++ Dinic_Serial.C -Wall -Werror -O3 -o Dinic_Serial
	./Dinic_Serial
debug:
	g++ Dinic_Serial.C -g -DDEBUG -Wall -Werror -O0 -o Dinic_Serial
	./Dinic_Serial > debug.txt