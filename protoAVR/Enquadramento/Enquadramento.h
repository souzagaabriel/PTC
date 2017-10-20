#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "fcstab.h"


class Enquadramento {
public:
 Enquadramento(Stream &serial, int bytes_min, int bytes_max);
 
 Enquadramento(Stream &serial, Stream &d, int bytes_min, int bytes_max);
 
 // envia o quadro apontado por buffer
 // o tamanho do quadro é dado por bytes
 void envia(unsigned char * buffer, int bytes);
 
 // espera e recebe um quadro, armazenando-o em buffer
 // retorna o tamanho do quadro recebido
 int recebe(unsigned char * buffer);
 
  Stream & getSerial(){return porta;}
  
private:

 unsigned char * app_buffer;
	
 int min_bytes, max_bytes; // tamanhos mínimo e máximo de quadro
 
 Stream & porta, &debug; 
 unsigned char buffer[256];  
 
 enum Estados {ocioso, rx, esc};
 
 // bytes recebidos pela MEF até o momento 
 int n_bytes;
 
 // estado atual da MEF
 int estado; 
 
 // aqui se implementa a máquina de estados de recepção
 // retorna true se reconheceu um quadro completo
 bool handle(unsigned char byte);
 
 bool check_crc( unsigned char * buffer, int len); 

 void gen_crc( unsigned char * buffer, int len);
 
 uint16_t pppfcs16(uint16_t fcs,  unsigned char * cp, int len);
 
};
 
//#endif
