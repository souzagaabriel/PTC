#include "Enquadramento.h"
//#define Debug

Enquadramento::Enquadramento(Stream & dev, int bytes_min, int bytes_max) : porta(dev){
	min_bytes = bytes_min;
	max_bytes = bytes_max;
	estado = ocioso;
}

Enquadramento::Enquadramento(Stream & dev, Stream & deb, int bytes_min, int bytes_max) : porta(dev), debug(deb){
	min_bytes = bytes_min;
	max_bytes = bytes_max;
	estado = ocioso;
}

void  Enquadramento::envia(unsigned char * buf, int bytes){
	#ifdef Debug
           // debug.print(F("\nEnq :: envia() "));
			//debug.print(F("Bytes:"));debug.println(bytes);
			debug.println(F("\nEnq::Buffer  para envio com arq:"));
			for(int h = 0; h<bytes; h++){
					debug.print( buf[h], HEX);
					debug.print(" ");
			}debug.println();
	#endif		
	
	// Gera CRC e adiciona  ao buffer
	gen_crc( buf, bytes);
	
	#ifdef Debug
		/*debug.println(F("\nEnq::Buffer  para envio com arq e CRC):"));
		for(int h = 0; h<bytes+2; h++){
				debug.print( buf[h], HEX);
				debug.print(" ");
		}debug.println();*/
	#endif
	
	// Prepara o frame
	buffer[0] = 0x7E;// Sentinela início mensagem
	int j = 1;

	for(int i = 0; i<bytes+2; i++){
		if(buf[i] == 0x7E || buf[i] == 0x7D){
			buffer[j] = 0x7D;
			j++;
			buffer[j] = buf[i] xor 0x20;
		}else{
			buffer[j] = buf[i];
		}
		j++;
	}	
	
	buffer[j] = 0x7E;// Sentinela final mensagem
	j++;
	
	#ifdef Debug
		/*debug.println(F("\nBuffer  para envio com arq, CRC e flags):"));
		for(int h = 0; h<j; h++){
				debug.print( buffer[h], HEX);
				debug.print(" ");
		}debug.println();*/
	#endif

	// Envia o frame	
	int n = 0;
	if( (j>=min_bytes) && (j<=max_bytes) ){	
		n = porta.write(buffer, j);
		#ifdef Debug
			debug.print(F("Enviou bytes:"));debug.println(n);
		#endif
	}else{
		#ifdef Debug
			debug.print(F("\nTamanho da mensagem fora dos limites!!! Mensagem não enviada!"));
		#endif
	}
		
}



int  Enquadramento::recebe(unsigned char* _buffer){

	app_buffer = _buffer;
	boolean rx = false, _crc = false;
	n_bytes = 0;
	
	//Entrando em while recepção. BLOQUEANTE!!! 		
	while(!rx){
		rx = handle( porta.read());	
	}
	//Saiu while recepção !!!
	
	#ifdef Debug
		debug.println(F("Mensagem Recebida!!!")); 
		debug.print(F("Buffer  RX com arq e CRC:"));
		debug.print(n_bytes);debug.print(" ");
		debug.print("\n ");
		for(int h = 0; h<n_bytes; h++){
					debug.print( _buffer[h], HEX);
					debug.print(" ");
		}debug.println();
	#endif			
	
	//Confere CRC!		
	_crc = check_crc(_buffer, n_bytes);
	if(_crc){
			#ifdef Debug
				debug.println(F("CRC OK!"));
			#endif
			return n_bytes;
	}else{
			#ifdef Debug
				debug.println(F("CRC NOK!!!"));
			#endif
			return 0;
	}		
}

 // aqui se implementa a máquina de estados de recepção
 // retorna true se reconheceu um quadro completo
bool  Enquadramento::handle(unsigned char byte){
//debug.println(F("Enq::handle()"));
	delay(10);
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
		 		return true;// Fim de quadro !!!
			}else if(byte == 0x7D){
				estado = esc;
				return false;
			}else{
				app_buffer[n_bytes] = byte;
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
				app_buffer[n_bytes] = byte xor 0x20;
				n_bytes++;
				return false;
			}
     break;     
 	//-----------------------------------------------------------
     default :
       return false;
  }	
}

bool Enquadramento::check_crc( unsigned char * buffer, int len){		
	uint16_t validacao;
	
	validacao = pppfcs16(PPPINITFCS16,buffer,len);
	if(validacao == PPPGOODFCS16)return true;
	return false;	     
}

void Enquadramento::gen_crc( unsigned char * buffer, int len){

	union crc_16
	{
		uint16_t crc_value;
		unsigned char crc_bytes[2];
	};
	crc_16 crc;
         
   crc.crc_value = pppfcs16(PPPINITFCS16,buffer,len);
   crc.crc_value ^= 0xffff;
   
   buffer[len] =  crc.crc_bytes[0] ;//(aux & 0x00ff);     
   buffer[len + 1] = crc.crc_bytes[1];//(aux >> 8);
   
}

uint16_t Enquadramento::pppfcs16(uint16_t fcs, unsigned char * cp, int len){

   while (len--) fcs = (fcs >> 8) ^ fcstab[(fcs ^ *cp++) & 0xff];
   return (fcs);

}
