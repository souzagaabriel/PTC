#include "Enquadramento.h"
#include "fcstab.h"
#include <assert.h>
#include "arq.h"
#include <time.h>

using namespace std;
time_t mytimer;

Enquadramento::Enquadramento(Serial & dev, int bytes_min, int bytes_max) : porta(dev){
	min_bytes = bytes_min;
	max_bytes = bytes_max;
	estado = ocioso;
	printf(" Estado inicial: "); cout << ocioso;
}

void  Enquadramento::envia(char * buffer, int bytes){

	char aux_buffer[max_bytes];
	
	cout << "\nMsg no Buffer: " << buffer;
	cout << "\nBytes: " << bytes<< endl;
	cout << "Buffer Hex 1:     ";
	for(int u = 0; u<bytes; u++){
		//cout << " " << hex << char(buffer[u]) ;
		if(buffer[u] == 0x7E || buffer[u] == 0x7D){printf("   ");}
		printf(" %x", buffer[u]);
	}
	
	gen_crc(buffer,bytes);

	// Prepara o frame
	aux_buffer[0] = 0x7E;// Sentinela início mensagem
	int j = 1;

	for(int i = 0; i<bytes; i++){
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
	
	
	cout << "\nBuffer Hex 2" << j << ": ";
	for(int u = 0; u<j; u++){
		printf(" %x", aux_buffer[u]);
	}
	
        //ARQ
        arq enviar('d');
        auto encap = enviar.mensagem(buffer,bytes);

	// Envia o frame	
	int n = 0;
	if( (j>=min_bytes) && (j<=max_bytes) ){		
		n = porta.write(encap, j);	
		cout << "\nEnviou " << n << " bytes." << endl;
	}else{
		cout << "\nTamanho da mensagem fora dos limites!!! Mensagem não enviada!"<< endl;
	}
		
}



int  Enquadramento::recebe(char* buffer){
	
	bool rx = false;
	n_bytes = 0;
	//----------------------------------------------
	printf("\nEntrando em while recepção. BLOQUEANTE!!!  \n");
		
	while(!rx){
		rx = handle(porta.read_byte(), buffer);	
	}
	
	printf("\nSaiu while recepção !!!  \n");
	//-------------------------------------------
		
	
	//mytimer = time(NULL);	
	//printf("Timer iniciado: %ld \n\n", mytimer);
	/*while(1){
		if((time(NULL) - mytimer) >= 1){						
			mytimer += intervalo_timer;
		}
	}*/
		
	return n_bytes;
}

 // aqui se implementa a máquina de estados de recepção
 // retorna true se reconheceu um quadro completo

bool  Enquadramento::handle(char byte, char* in_buffer){
//Estados {ocioso, rx, esc};   
//int n_bytes; bytes recebidos pela MEF até o momento 
 //int estado; estado atual da MEF

 printf("%d", estado); printf(":%x ", byte);
 
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
		 		printf(" %d \nFim de quadro!!!\n", estado);
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
bool Enquadramento::check_crc(char * buffer, int len){
   char * aux;
   uint16_t validacao;
   int lenAux = len - 2;
   
   for(int i = 0;i < lenAux; i++){
      aux[i] = buffer[i];   
   }

   validacao = pppfcs16(PPPINITFCS16,aux,lenAux);
   validacao ^= 0xffff;

   if(buffer[len - 1] == (validacao & 0x00ff) and buffer[len] == ((validacao >> 8) & 0x00ff)) return true;
   else return false;   
}

void Enquadramento::gen_crc(char * buffer, int len){
   uint16_t aux;
   aux = pppfcs16(PPPINITFCS16,buffer,len);
   aux ^= 0xffff;
   
   buffer[len] = (aux & 0x00ff);
   buffer[len + 1] = ((aux >> 8) & 0x00ff); 
}

uint16_t Enquadramento::pppfcs16(uint16_t fcs, char * cp, int len){

   assert(sizeof (uint16_t) == 2);
   assert(((uint16_t) -1) > 0);

   while (len--) fcs = (fcs >> 8) ^ fcstab[(fcs ^ *cp++) & 0xff];
   return (fcs);

}

