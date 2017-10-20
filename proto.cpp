/* 
  Arquivo: proto.cpp
  Author:  fernandomuller
  Data:    Outubro 2017
  
  Arquivo de teste do Protocolo  
   
  Para compilar:
  $ g++ proto.cpp -o proto Enquadramento.cpp Serial.cpp ARQ.cpp -std=c++11
   
  Para verificar o dispositivo USB:
  $ ls -lh /dev/ttyU*
  
*/

#include "ARQ.h"

using namespace std; 

//Objetos da aplicação(app)
class App{  
  public:
      union {
          unsigned char buf[sizeof(uint8_t) * 266];
          struct {
              char flag;
              uint8_t id;
              uint16_t counter;// Contador da app local 
              uint16_t data;     
              float timestamp;
              unsigned char bufferRX[256];
          } valores;
      } app;      

};

App user;

void processaMsg(void){
      	cout << "\nProcessando Mensagem...Counter: " ;
      	printf(" %x \n", user.app.valores.bufferRX[3]);// Contador da app remota
}

// Variáveis da app para manipulação do timer da app
time_t timerapp;
int interval_app = 4;//  segundos

// Objetos do protocolo
Serial serial("/dev/ttyUSB0", B9600);
Enquadramento enquad(serial, 8, 256);
ARQ arq(enquad);

int main() {	
	
	//Inicializa aplicação
	user.app.valores.flag = 0xee;
    user.app.valores.id = 0x02;
    user.app.valores.counter = 0x00;
	timerapp = time(NULL);
	
	//Inicializa protocolo passando a serial, o bufferRX e a função da aplicação
	arq.init(serial.get(), &user.app.valores.bufferRX[0], processaMsg);	
		
	while(1){
		
		//Verifica se chegou algo na serial e
		// chama arq.handle(quadro)
		arq.run();
		
		// Timer Aplicação! Envia a msg de dados da aplicação
 		if((time(NULL) - timerapp) >= interval_app){
 			
 			cout << "\nTimeOut App !!!" << endl;
 			
 			arq.envia(user.app.buf, 10);
 			user.app.valores.counter++;	
			
			timerapp += interval_app;
		}
		
 	}   
}

