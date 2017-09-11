#ifndef ARQ_H
#define ARQ_H
 
#include <iostream>
#include <fstream>
#include "fcstab.h"
#include <stdio.h>
#include <errno.h>

#include <cstdint>
#include "Serial.h"

class arq{
private:

   string tipo_msg;
   int num_seq, id, tam;
   char * msg_encapsulada;

public:

   arq(string tipo);
   char * mensagem(char * buffer,int len);
}
