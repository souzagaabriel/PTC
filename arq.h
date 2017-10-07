//
// Created by douglas on 21/09/17.
//

#ifndef UNTITLED_ARQ_H
#define UNTITLED_ARQ_H


#include <iostream>
#include <fstream>
#include <string>
#include "fcstab.h"
#include <stdio.h>
#include <errno.h>

#include <cstdint>
#include "Serial.h"
#include "fila.h"

class Enquadramento;

class ARQ {
private:

    //enum Vocabulario {DATA0, DATA1, ACK0, ACK1};
    enum TipoEvento {
        Payload, Quadro, TimeOut
    };

    struct Evento {
        TipoEvento tipo;
        int bytes;
        unsigned char *msg;

        Evento(TipoEvento t, unsigned char *p, int len) : tipo(t), msg(p), bytes(len) {}
    };

    enum Estados { Ocioso, Espera};

    Estados estado;
    int rseq, tseq;
    Enquadramento & enq;

    fila<char> *Fila;
    bool msgEmEspera;


    void handle(Evento &e);

public:

    ARQ(Enquadramento & e);

    void envia( unsigned char *buffer, int bytes);

    int recebe();
};


#endif //UNTITLED_ARQ_H

