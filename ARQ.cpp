// Created by douglas on 21/09/17.
// Editado por fernandomuller 

#include "ARQ.h"

#include <iostream>
#include <fstream>
#include "fcstab.h"
#include <stdio.h>
#include <errno.h>

#include <cstdint>
#include <cstring>

#define FData 0
#define FAck 1

using namespace std;

ARQ::ARQ(Enquadramento & e): enquad(e) {
    estado = Ocioso;
    rseq = 0;
    tseq = 0;
}

void ARQ::init(int fd, unsigned char * _app_buffer, void (*funcaoApp)(void)){
	fd_serial = fd;
	int op_serial = fcntl(fd_serial, F_GETFL);
	fcntl(fd_serial, F_SETFL, op_serial | O_NONBLOCK);
	app_buffer = _app_buffer;
	aloha_interval = 1;
	fApp = funcaoApp;
}

void ARQ::run(){

	//verifica a serial durante 2 ms
    timer = {0,2000};	
	FD_ZERO(&r);
	FD_SET(fd_serial, &r); 
					   		  
	int n = select(fd_serial+1, &r, NULL, NULL, &timer);
						
	if (FD_ISSET(fd_serial, &r)) { 
			E.tipo = quadro;
			handle(E);										
	} 
}

void ARQ::envia(unsigned char * buffer, int bytes){
	printf ("\nARQ :: envia()	Estado MEF: %d " , estado);
    memcpy(arq.campo.msg, buffer, bytes);
    E.tipo = payload;
    E.ptr = arq.campo.msg;
    E.bytes = bytes;
    handle(E);     
           
	cout << "Mensagem enviada...ACK recebido com sucesso!\n";	
}

int ARQ::recebe(){
    //Evento E = Evento(quadro, app_buffer, 0);
    E.tipo = quadro;
    handle(E);
    return rx_bytes;
}

void ARQ::handle(Evento& ev){

	printf ("\n\nARQ :: handle()   Estado MEF: %d " , estado);
	
	int n = 0;
	
 	switch (estado)
 	{
        case Ocioso://-------------------------------------------------------
            switch ( ev.tipo )
			{	
				 case payload :		 	
						cout << "Enviando payload... ";
						
						estado = Espera;						
						arq.campo.control = FData | (tseq << 1);
						tseq = !tseq;
						
                   		//memcpy(aux + 1, ev.ptr, ev.bytes);
                   		//ev.ptr = aux;
                   		
                   		//Antes de enviar payload verifica a serial
                   		// 2 ms

                   		timer = {0,2000};	
						FD_ZERO(&r);
					   	FD_SET(fd_serial, &r); 
					   		  
						n = select(fd_serial+1, &r, NULL, NULL, &timer);
						
						if (FD_ISSET(fd_serial, &r)) {
							
							rx_bytes = enquad.recebe(app_buffer);
							
							cout << "Ops! Chegou quadro... ";
							printf ("\nBuffer  recebido  Hex  (com CRC):");
							for(int h = 0; h<rx_bytes; h++){
								printf(" %x", app_buffer[h]);
							}
							
							if((app_buffer[0] & 0x01) == FData ){
						           	aux[0] = app_buffer[0] ^ 0x03;
						           	aux[1] = 0xee;
						           	enquad.envia(aux,8);
						           	printf("AcK enviado...");
						   	}else{
						       		printf("AcK Fora de sequência!");
						   	}								
						}   	 			
						 
						//sleep(5);  

						enquad.envia(arq.buf, ev.bytes+1);
						
						cout << "Aguardando ACK!!! ";
												
						//aloha_interval = rand		
						timer = {aloha_interval,0};// Reinicia o timer	
						FD_ZERO(&r);
			   			FD_SET(fd_serial, &r); 
			   			  
						n = select(fd_serial+1, &r, NULL, NULL, &timer);
						
						if (FD_ISSET(fd_serial, &r)) {        	 
							ev.tipo = quadro;
							handle(ev);        				
						}          
						      		    
						if (n <= 0) { 
							ev.tipo = timeout;
							handle(ev);
				   		} 
				 		break;
				 case quadro :
						rx_bytes = enquad.recebe(app_buffer);
						
						printf ("\nBuffer  recebido  Hex  (com CRC):");
						for(int h = 0; h<rx_bytes; h++){
								printf(" %x",  app_buffer[h]);
						}printf(" \n");						
						
						if((app_buffer[0] & 0x01) == FData ){
                        	aux[0] = app_buffer[0] ^ 0x03;
                        	aux[1] = 0xee;
                        	enquad.envia(aux,8);
                        	
                        	cout << "AcK enviado..." << endl;
                        	
                        	//Processa mensagem usando
                        	//a função passada pela aplicação
                        	fApp();
                        	
                    	}else{
                    		cout << "AcK Fora de sequência!" << endl;
                    	}
				 		break;    
				 default :
				   		printf ("Evento invalido!\n");
			}
            break;

        case Espera://-------------------------------------------------------
            
            switch ( ev.tipo )
			{	
				 case payload :		
				 		// armazenar, aguardar confirmação, enviar
                   		 printf ("Evento payload não implementado no estado espera!\n");
                   		 printf ("\nProtocolo foi pro brejo..?");
				 		 break;
				 case quadro :
						 rx_bytes = 0;
						 rx_bytes = enquad.recebe(app_buffer);
						 
						 printf ("\nBuffer  recebido  Hex  (com CRC):");
						 for(int h = 0; h<rx_bytes; h++){
								printf(" %x",  app_buffer[h]);
						 }printf(" \n");
						 
						 if(rx_bytes >= 8){
						 
								 if((app_buffer[0] & 0x01) == FData ){//
								 	cout << "Quadro de dados..." << endl;
								 	if(rx_bytes > 8){
				                		aux[0] = app_buffer[0] ^ 0x03;
				                		aux[1] = 0xee;                      	
				                		cout << "Enviando ACK..." << endl;
				                		enquad.envia(aux,8);
								 	}       
								 	
								 	//Processa mensagem usando
				                	//a função passada pela aplicação
				                	fApp();                 	 
				                	                       	
				                	//estado=Espera: Retransmitir imediatamente
				                	enquad.envia(arq.buf, ev.bytes+1);
									cout << "Aguardando ACK!!! ";
												
									//aloha_interval = rand	
									timer = {aloha_interval,0};// Reinicia o timer	
									FD_ZERO(&r);
						   			FD_SET(fd_serial, &r);   
						   			
									n = select(fd_serial+1, &r, NULL, NULL, &timer);
									if (FD_ISSET(fd_serial, &r)) {       	 
										ev.tipo = quadro;
										handle(ev);
										break;        				
									}                		    
									if (n <= 0) { 
										ev.tipo = timeout;
										handle(ev);
										break;
							   		} 
				                	
				            	 }else if(app_buffer[0] ^ (tseq << 1)  == FAck ){
				            		cout<< "ACK OK!!!..."<<endl;               		
				            		estado = Ocioso;
				            	 }else{
				            	 	cout<< "Nem Data nem ACK!!!!..."<<endl; 
				            	 }
                    	 }else{
                    	 		cout << "\nTimeout ACK!!! Retransmitindo...";
								enquad.envia(arq.buf, ev.bytes+1);
						
								srand(time(NULL));
								aloha_interval = (rand() % 10) +1;	
								timer = {aloha_interval,0};
						
								FD_ZERO(&r);
					   			FD_SET(fd_serial, &r);
					   			   
								n = select(fd_serial+1, &r, NULL, NULL, &timer);
						
								if (FD_ISSET(fd_serial, &r)) {  
									ev.tipo = quadro;          	 
									handle(ev); 
									break;       				
								}     
										  		    
								if (n <= 0) { 
									ev.tipo = timeout;
									handle(ev);
									break;
						   		} 
						 		
                    	 }
                    	 
				 		 break;
				 case timeout :
						cout << "\nTimeout ACK!!!	       Retransmitindo...";
						
						enquad.envia(arq.buf, ev.bytes+1);
						
						srand(time(NULL));
						aloha_interval = (rand() % 10) +1;	
						timer = {aloha_interval,0};
						
						FD_ZERO(&r);
			   			FD_SET(fd_serial, &r);
			   			   
						n = select(fd_serial+1, &r, NULL, NULL, &timer);
						
						if (FD_ISSET(fd_serial, &r)) {  
							ev.tipo = quadro;          	 
							handle(ev); 
							break;       				
						}     
						          		    
						if (n <= 0) { 
							ev.tipo = timeout;
							handle(ev);
							break;
				   		} 
				 		break;     
				 default :
				   		printf ("Evento invalido!\n");
			}
            break;
        default:
            break;
    }	
   
}
