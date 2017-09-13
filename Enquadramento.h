#ifndef FRAMING_H
#define FRAMING_H
 
#include <iostream>
#include <fstream>
#include <assert.h>
#include "fcstab.h"
#include <stdio.h>
#include <errno.h>
#include <cstdint>
#include "Serial.h"
//#include "arq.h"
 
class Enquadramento {
public:
 Enquadramento(Serial & dev, int bytes_min, int bytes_max);
 //~Enquadramento();
 
 // envia o quadro apontado por buffer
 // o tamanho do quadro é dado por bytes
 void envia(unsigned char * buffer, int bytes);
 
 // espera e recebe um quadro, armazenando-o em buffer
 // retorna o tamanho do quadro recebido
 int recebe(unsigned char * buffer);

private:
 int min_bytes, max_bytes; // tamanhos mínimo e máximo de quadro
 Serial & porta; 
 char buffer[4096]; // quadros no maximo de 4 kB (hardcoded)
 
 enum Estados {ocioso, rx, esc};
 
 // bytes recebidos pela MEF até o momento 
 int n_bytes;
 
 // estado atual da MEF
 int estado;
 
 // aqui se implementa a máquina de estados de recepção
 // retorna true se reconheceu um quadro completo
 bool handle(char byte, unsigned char * buffer);

// verifica o CRC do conteúdo contido em "buffer". Os dois últimos 
// bytes desse buffer contém o valor de CRC
bool check_crc(unsigned char * buffer, int len);
 
// gera o valor de CRC dos bytes contidos em buffer. O valor de CRC
// é escrito em buffer[len] e buffer[len+1]
void gen_crc( unsigned char * buffer, int len);
 
// calcula o valor de CRC dos bytes contidos em "cp".
// "fcs" deve ter o valor PPPINITFCS16
// O resultado é o valor de CRC (16 bits)
// OBS: adaptado da RFC 1662 (enquadramento no PPP)
 uint16_t pppfcs16(uint16_t fcs, unsigned char * cp, int len);
 
};
 
#endif
