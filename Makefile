
proto: proto.o ARQ.o Enquadramento.o Serial.o 	
	g++ -g -o proto proto.o ARQ.o Enquadramento.o Serial.o 
	-rm -f *.o *~
	
proto.o: proto.cpp
	@echo Executando makefile protoMAU - O protocolo MAU humorado
	g++ -g -c proto.cpp -std=c++11
		
Serial.o: Serial.cpp Serial.h
	g++ -g -c Serial.cpp -std=c++11
	
Enquadramento.o: Enquadramento.cpp Enquadramento.h
	g++ -g -c Enquadramento.cpp -std=c++11

ARQ.o:	ARQ.cpp ARQ.h
	g++ -g -c ARQ.cpp -std=c++11
	
clean:
	-rm -f *.o *~
	

