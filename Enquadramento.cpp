#include "Enquadramento.h"
#include <time.h>

using namespace std;
time_t mytimer;

Enquadramento::Enquadramento(Serial & dev, int bytes_min, int bytes_max) : porta(dev){
	min_bytes = bytes_min;
	max_bytes = bytes_max;
	estado = ocioso;
	//printf(" Estado inicial: "); cout << ocioso;
}

void  Enquadramento::envia(unsigned char * buffer, int bytes){

	unsigned char aux_buffer[max_bytes];
	
	//Debug
	//cout << "\n\nMensagem no Buffer              :     " ;
	//cout.write((char*) buffer, bytes);
	/*cout << "\nBuffer Hex sem enquadramento: "<< bytes << ":    ";
	for(int u = 0; u<bytes; u++){
		if(buffer[u] == 0x7E || buffer[u] == 0x7D){printf("   ");}
		printf(" %x", buffer[u]);
	}*/
	
	// Gera CRC e adiciona  ao buffer
	gen_crc( buffer,bytes);
	
	//Debug
	cout << "\nBuffer TX Hex com CRC:           "<< bytes + 2<< ":    ";
	for(int u = 0; u<bytes+2; u++){
		//cout << " " << hex << char(buffer[u]) ;
		if(buffer[u] == 0x7E || buffer[u] == 0x7D){printf("   ");}
		printf(" %x", buffer[u]);
	}
	
	// Prepara o frame
	aux_buffer[0] = 0x7E;// Sentinela início mensagem
	int j = 1;

	for(int i = 0; i<bytes+2; i++){
		if(buffer[i] == 0x7E || buffer[i] == 0x7D){
			aux_buffer[j] = 0x7D;
			j++;
			aux_buffer[j] = buffer[i] xor 0x20;
		}else{
			aux_buffer[j] = buffer[i];
		}
		j++;
	}	
	aux_buffer[j] = 0x7E;// Sentinela final mensagem
	j++;
	
	//Debug
	/*cout << "\nBuffer Hex com enquadramento: " << j << ": ";
	for(int u = 0; u<j; u++){
		printf(" %x", aux_buffer[u]);
	}*/
	
	// Envia o frame	
	int n = 0;
	if( (j>=min_bytes) && (j<=max_bytes) ){		
		n = porta.write((char*) aux_buffer, j);//(encap, j);	
		cout << "\nEnviou " << n << " bytes." << endl;
	}else{
		cout << "\nTamanho da mensagem fora dos limites!!! Mensagem não enviada!"<< endl;
	}
		
}


int  Enquadramento::recebe(unsigned char* buffer){
	
	bool rx = false, _crc = false;
	n_bytes = 0;
	//----------------------------------------------
	//printf("\n\nEntrando em while recepção. BLOQUEANTE!!!  \n");

	while(!rx){
		rx = handle(porta.read_byte(), buffer);	
	}
	
	printf("\n\nMensagem recebida !!!  ");
	//printf(n_bytes);
	cout << "\nBuffer RX Hex com CRC:           "<< n_bytes ;
	//-------------------------------------------
	_crc = check_crc(buffer, n_bytes);
	if(_crc){
			printf ("        CRC Ok! ");
			return n_bytes;
	}else{
			printf ("        CRC ERROR!");
			return -1;
	}	
		
	
}

 // aqui se implementa a máquina de estados de recepção
 // retorna true se reconheceu um quadro completo

bool  Enquadramento::handle(char byte, unsigned char* in_buffer){
//Estados {ocioso, rx, esc};   
//int n_bytes; bytes recebidos pela MEF até o momento 
 //int estado; estado atual da MEF

 //printf("%d", estado); printf(":%x ", byte);
 
  switch ( estado )
  {	//-------------------------------------------------------
     case ocioso :
		 	if(byte == 0x7E ){
		 		estado = rx;
		 		
			}
			return false;			
     break;
 	//--------------------------------------------------------
     case rx :
			if(byte == 0x7E && n_bytes > 0){
		 		estado = ocioso;
		 		//printf(" %d \nFim de quadro!!!\n", estado);
		 		//cout.write(in_buffer, n_bytes);
		 		return true;// Fim de quadro !!!
			}else if(byte == 0x7D){
				estado = esc;
				return false;
			}else{
				in_buffer[n_bytes] = byte;
				n_bytes++;
				return false;
			}
     break;
 	//-------------------------------------------------------------
     case esc :
			if(byte == 0x7E || byte == 0x7D){
		 		estado = ocioso;
		 		//bzero(buffer);
		 		n_bytes = 0;
		 		return false;
			}else{
				estado = rx;
				in_buffer[n_bytes] = byte xor 0x20;
				n_bytes++;
				return false;
			}
     break;     
 	//-----------------------------------------------------------
     default :
       printf ("Estado da MEF invalido!\n");
       return false;
  }	
}
bool Enquadramento::check_crc(unsigned char * buffer, int len){		
	uint16_t validacao;
	
	validacao = pppfcs16(PPPINITFCS16,buffer,len);
	if(validacao == PPPGOODFCS16)return true;
	return false;	     
}

void Enquadramento::gen_crc( unsigned char * buffer, int len){
	//uint16_t aux;
	union crc_16
	{
		uint16_t crc_value;
		unsigned char crc_bytes[2];
	};
	crc_16 crc;
         
   crc.crc_value = pppfcs16(PPPINITFCS16,buffer,len);
  // printf("\ncrc1 %x", c.crc_value);
   crc.crc_value ^= 0xffff;
   //printf("\ncrc2 %x", c.crc_value);
   
   buffer[len] =  crc.crc_bytes[0] ;//(aux & 0x00ff);     
   buffer[len + 1] = crc.crc_bytes[1];//(aux >> 8);
   
   //printf("\nbuffer1 %x", buffer[len]);
   //printf("\nbuffer2 %x", buffer[len+1]);
}

uint16_t Enquadramento::pppfcs16(uint16_t fcs, unsigned char * cp, int len){

   assert(sizeof (uint16_t) == 2);
   assert(((uint16_t) -1) > 0);

   while (len--) fcs = (fcs >> 8) ^ fcstab[(fcs ^ *cp++) & 0xff];
   return (fcs);

}

