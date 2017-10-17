A1_objects = A.o Enquadramento.o Serial.o arq.o
B1_objects = B.o Enquadramento.o Serial.o arq.o

all : placa1 placad

A1: $(placad_objects)
	g++ -pthread -o A1 $(placad_objects)

B1: $(placa1_objects)
	g++ -pthread -o B1 $(placa1_objects)

B.o: B.cpp
	g++ -pthread -g -c B.cpp -std=c++11
A.o: A.cpp
	g++ -pthread -g -c A.cpp -std=c++11
	
Serial.o: Serial.cpp Serial.h
	g++ -pthread -g -c Serial.cpp -std=c++11
	
Enquadramento.o: Enquadramento.cpp Enquadramento.h
	g++ -pthread -g -c Enquadramento.cpp -std=c++11

arq.o: arq.cpp arq.h
	g++ -pthread -g -c arq.cpp -std=c++11




clean:
	rm -f *.o A1 B1
