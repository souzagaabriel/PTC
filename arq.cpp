//
// Created by douglas on 21/09/17.
//

#include "arq.h"
#include <iostream>
#include <fstream>
#include "fcstab.h"
#include <stdio.h>
#include <errno.h>

#include <cstdint>
#include <cstring>
#include "Serial.h"

using namespace std;

ARQ::ARQ(Enquadramento & e): enq(e) {
    estado = Ocioso;
    rseq = 0;
    tseq = 0;
    Fila = new fila<char>(10);
    msgEmEspera = false;
}

void ARQ::envia(unsigned char * buffer, int bytes) {
    if (msgEmEspera) {
        while (not Fila->vazia()) {
            TipoEvento e = Payload;
            Evento E = Evento(e, reinterpret_cast<unsigned char *>(Fila->desenfileira()), bytes);
            handle(E);
            while (E.tipo == Payload) {
                int i = 0;
                do {
                    i = enq.recebe();
                } while (i == 0);

                Evento R = Evento(Quadro, const_cast<unsigned char *>(enq.getBuffer()), i);
                handle(R);
                E.tipo = R.tipo;
            }
        }
    }
        TipoEvento e = Payload;
        Evento E = Evento(e, buffer, bytes);
        handle(E);
        while (E.tipo == Payload) {
            int i = 0;
            do {
                i = enq.recebe();
            } while (i == 0);

            Evento R = Evento(Quadro, const_cast<unsigned char *>(enq.getBuffer()), i);
            handle(R);
            E.tipo = R.tipo;
        }

    }

    int ARQ::recebe() {
        //TODO Testar Recebe
        int i = 0;
        do {
            i = enq.recebe();
        } while (i == 0);

        TipoEvento e = Quadro;
        Evento E = Evento(e,  const_cast<unsigned char *>(enq.getBuffer()), i);

        handle(E);
    }

#define FData 0
#define FAck 1

    void ARQ::handle(Evento &e) {

        switch (estado) {
            case Ocioso:
                switch (e.tipo) {
                    case Payload: {
                        unsigned char aux[512];

                        // tipo de quadro: bit 0
                        // sequencia: bit 1
                        aux[0] = FData | (tseq << 1);

                        if (tseq == 1) tseq = 0;
                        else tseq = 1;

                        memcpy(aux + 1, e.msg, e.bytes);

                        enq.envia(aux, e.bytes + 1);

                        estado = Espera;
                        break;
                    }

                    case Quadro: {
                        if (e.msg[0] == FAck | (rseq << 1)) {
                            cout << "Ack recebido" << e.msg[0] << endl;
                            if (rseq == 1) rseq = 0;
                            else rseq = 1;
                        } else {
                            cout << "Ack incorreto" << endl;
                        }
                        if (e.msg[0] == FData | (rseq << 1));
                        unsigned char aux2[512];
                        aux2[0] = FAck | (rseq << 1);

                        if (rseq == 1) rseq = 0;
                        else rseq = 1;

		 	for(int i = 0; i<5;i++){
                                aux2[i+1] = 'a';
                        }

                        enq.envia(aux2, 6);
                        estado = Ocioso;
                        cout << "Ack" << rseq << " enviado" << endl;
                        break;
                    }


                    case TimeOut:
                        break;
                }
                break;

            case Espera:
                switch (e.tipo) {
                    case Payload: {
                        // armazenar
                        unsigned char aux[512];

                        // tipo de quadro: bit 0
                        // sequencia: bit 1
                        aux[0] = FData | (tseq << 1);

                        if (tseq == 1) tseq = 0;
                        else tseq = 1;

                        memcpy(aux + 1, e.msg, e.bytes);
                        Fila->enfileira(reinterpret_cast<const char &>(aux));
                        msgEmEspera = true;
                        // reenviar dado
                        estado = Espera;
                        e.tipo = Payload;
                        break;
                    }
                    case Quadro:
                        if (e.msg[0] == FAck | (rseq << 1)) {
                            cout << "Ack recebido" << e.msg[0];
                            estado = Ocioso;
                        } else if (e.msg[0] == FData | (rseq << 1)) {
			    unsigned char aux2[512];
                            aux2[0] = FAck | (rseq << 1);

                            if (rseq == 1) rseq = 0;
                            else rseq = 1;

			    for(int i = 0; i<5;i++){
                                aux2[i+1] = 'a';
                            }

                            enq.envia(aux2, 6);
                            cout << "Ack" << rseq << " enviado" << endl;
                            estado = Espera;
                            e.tipo = Payload;
                        } else {
                            cout << "Ack incorreto";
                            estado = Espera;
                            e.tipo = Payload;
                        }
                        break;
                    case TimeOut:
                        break;
                }

                break;

            default:
                break;
        }
    }
