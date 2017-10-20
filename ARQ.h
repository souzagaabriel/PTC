//
// Created by douglas on 21/09/17.
// Editado por fernandomuller 
//

#ifndef UNTITLED_ARQ_H
#define UNTITLED_ARQ_H

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h> 
#include <fstream>
#include <cstring>
#include <string>
#include "fcstab.h"
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <cstdint>
#include "Serial.h"
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

    enum Estados { Ocioso, Espera};

    Estados estado;
    int rseq, tseq;
    Enquadramento & enquad;

    void handle(Evento &ev);
	
	timeval timer;
	fd_set r;
	int fd_serial;
	
	unsigned char * app_buffer;
	int rx_bytes;
	
	int aloha_interval;
	
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

    ARQ(Enquadramento & enq);
	void init(int fd_serial, unsigned char * app_buffer, void (*func)(void));
	void run();
	
    void envia( unsigned char *buffer, int bytes);
    int recebe();
};


#endif //UNTITLED_ARQ_H
