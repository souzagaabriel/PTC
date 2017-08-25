#ifndef FRAMING_H
#define FRAMING_H
 
#include <iostream>
//#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <errno.h>

#include <cstdint>
#include "Serial.h"
 
class Enquadramento {
public:
 Enquadramento(Serial & dev, int bytes_min, int bytes_max);
 //~Enquadramento();
 
 // envia o quadro apontado por buffer
 // o tamanho do quadro é dado por bytes
 void envia(char * buffer, int bytes);
 
 // espera e recebe um quadro, armazenando-o em buffer
 // retorna o tamanho do quadro recebido
 int recebe(char * buffer);

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
 bool handle(char byte, char * buffer);
 
};
 
#endif
