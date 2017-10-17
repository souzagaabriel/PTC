//#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "Enquadramento.h"
#include "Serial.h"
#include <thread>

using namespace std;


Serial rf("/dev/pts/19", B9600);


Enquadramento *radio= new Enquadramento(rf, 8, 40);
ARQ radio1(*radio);

string msg = "um~} teste\r\n";

char buffer[32];
//char aux_buffer[40];
int n = 0;
char ex = 0x7e;

void enviar(){
    char user;
    cout << "Deseja envia mensagem ? (s(sim) ou n(não))";
    cin >> user;
    
    if(user == 's'){
        radio1.envia(reinterpret_cast<unsigned char *>(&msg[0]), msg.size());
        cout << endl;
        sleep(2);
    }
}

void receber(){
    radio1.recebe();
}

int main() {
    while(true){
        
	thread first(receber);
	thread second(enviar);

	first.join();
	second.join();
    }
    return 0;
}
