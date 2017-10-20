//#include <SoftwareSerial.h>
#include "ARQ.h"
  
//SoftwareSerial mySerial(4, 10); // 4-RX, 10-TX

//Enquadramento enquad(Serial, mySerial, 8, 256);
//ARQ arq(mySerial, enquad);

Enquadramento enquad(Serial, 8, 256);
ARQ arq(enquad);

float timer = 0, val = 10000;

class App{  
  public:
      union {
          unsigned char buf[sizeof(uint8_t) * 266];
          struct {
              char flag;
              uint8_t id;
              uint16_t counter;
              uint16_t data;      
              float timestamp;
              unsigned char bufferRX[256];
          } valores;
      } app;      
      static const void processaMsg();
};

App user;

static const void App::processaMsg(){
         // mySerial.print("\nProcessando Mensagem...Counter:  ");
          //mySerial.println((uint16_t) user.app.valores.bufferRX[3]);
}

void setup() { 
  
    // Dados de uma aplicação qualquer
    user.app.valores.flag = 0xff;
    user.app.valores.id = 0x01;
    user.app.valores.counter = 0x00;
    user.app.valores.timestamp = millis();    
    
    Serial.begin(9600);//define taxa de transmissão da porta serial
    Serial.flush();//Esvasia o buffer de entrada da porta serial.    
    //mySerial.begin(9600);     
    //mySerial.flush();
    
    pinMode(13, OUTPUT);//Para manter o led do pino 13 apagado.

    arq.init( user.app.valores.bufferRX, user.processaMsg);
    
    //mySerial.println(F("Inicializando.................................... "));
}
    
void loop(){    
  
    arq.run();
    if(millis() > timer + val){
        //mySerial.print(F("\nTimer App:  "));
        //user.app.valores.timestamp = millis();
        
        arq.envia(user.app.buf, 10); 
        user.app.valores.counter++;            
        timer=millis();
    }   
 
}


