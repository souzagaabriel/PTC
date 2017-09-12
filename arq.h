#ifndef ARQ_H
#define ARQ_H
 
#include <iostream>
#include <fstream>
#include <string>
#include "fcstab.h"
#include <stdio.h>
#include <errno.h>

#include <cstdint>
#include "Serial.h"

class arq{
private:

   char tipo_msg;
   int num_seq, tam;
   char * msg_encapsulada;

public:

   arq(char tipo);
   char * mensagem(char * buffer,int len);
};

 
#endif
