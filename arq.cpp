#include <iostream>
#include <fstream>
#include "fcstab.h"
#include <stdio.h>
#include <errno.h>

#include <cstdint>
#include "Serial.h"
#include "arq.h"

using namespace std;

arq::arq(char tipo){
   tipo_msg = tipo;
   tam = 0;
   num_seq = 0;
}

char * arq::mensagem(char * buffer,int len){
   msg_encapsulada[tam] = tipo_msg;
 
   tam++;
 
   if(num_seq == 0){
      msg_encapsulada[tam] = '0';
      num_seq = 1;
      tam++;
   }else {
      msg_encapsulada[tam] = '1';
      num_seq = 0;
      tam ++;
   }   
  
   for(int i = 0; i<len;i++){
      msg_encapsulada[tam] == buffer[i]; 
      tam ++;
   }
   char * retorno = msg_encapsulada;
   return (retorno);
   
}
