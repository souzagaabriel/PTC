//#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "Enquadramento.h"
#include "Serial.h"

using namespace std;


Serial rf("/dev/pts/18", B9600);


Enquadramento *radio= new Enquadramento(rf, 8, 40);

string msg = "um~} teste\r\n";

char buffer[32];
//char aux_buffer[40];
int n = 0;
char ex = 0x7e;

int main() {

while(true){
    char user;
ARQ radio1(*radio);
    cout << "Deseja envia mensagem ? (s(sim) ou n(não))";
    cin >> user;

    switch(user){
       case 's':
       {
	radio1.envia(reinterpret_cast<unsigned char *>(&msg[0]), msg.size());
	//radio.envia((char*)"1.2.3.4.5.6.7.8.9.0 m", 19);
	sleep(2);

	cout << endl;
       break;
       }
      
      default:
       radio1.recebe();
       break;
    }
}
    return 0;
}
