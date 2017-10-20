//
// Created by douglas on 21/09/17.
// Editado por fernandomuller 
//						
//
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#ifndef UNTITLED_ARQ_H
#define UNTITLED_ARQ_H

#include "Enquadramento.h"

class ARQ {
private:

    enum TipoEvento {payload, quadro, timeout};

    struct Evento {
        TipoEvento tipo;
        int bytes;
        unsigned char *ptr;
		Evento() { tipo = timeout;} 
        Evento(TipoEvento t, unsigned char *p, int len) : tipo(t), ptr(p), bytes(len) {}
    };
    
	Evento E = Evento(quadro, arq.buf, 0);
	
    enum Estados { ocioso, espera};

    Estados estado;
    int rseq, tseq;
    Enquadramento & enquad;

    void handle(Evento & ev);
	
	float timer;
	int fd_serial;	
	
	int rx_bytes;
	
	float aloha_interval;
	
	unsigned char aux[10];
	union 
	{
		unsigned char buf[256];
		struct{
			unsigned char control;
			unsigned char msg[255];		
		}campo;

	}arq;

	void (* fApp)(void);
	
public:
	unsigned char * app_buffer;
 	Stream & debug;
 	
 	ARQ(Enquadramento & enq);
    ARQ(Stream &debug, Enquadramento & enq);
	void init( unsigned char * app_buffer, void (*func)(void));
	void run();
	
    void envia( unsigned char *buffer, int bytes);
    int recebe();
};


#endif //UNTITLED_ARQ_H
