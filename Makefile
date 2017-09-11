all: msg tx

msg:
	@echo Executando makefile
	
tx: main.o Enquadramento.o Serial.o arq.o
	g++ -g -o tx main.o Enquadramento.o Serial.o arq.o

main.o: main.cpp
	g++ -g -c main.cpp -std=c++11
	
Serial.o: Serial.cpp Serial.h
	g++ -g -c Serial.cpp -std=c++11
	
Enquadramento.o: Enquadramento.cpp Enquadramento.h
	g++ -g -c Enquadramento.cpp -std=c++11

arq.o: arq.cpp arq.h
	g++ -g -c arq.cpp -std=c++11
	
clean:
	-rm -f *.o *~
