//#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "Enquadramento.h"
#include "Serial.h"

using namespace std;


Serial rf("/dev/pts/6", B9600);
Serial tf("/dev/pts/17", B9600);


Enquadramento *radio= new Enquadramento(rf, 8, 40);
Enquadramento *radio2= new Enquadramento(tf, 8, 40);

string msg = "um~} teste\r\n";

char buffer[32];
//char aux_buffer[40];
int n = 0;
char ex = 0x7e;

void recebe(){
    ARQ receb(*radio2);

    n = tf.read(buffer,32);
    receb.recebe(reinterpret_cast<unsigned char *>(buffer), n);

}
int main() {

    ARQ radio1(*radio);
    radio1.envia(reinterpret_cast<unsigned char *>(&msg[0]), msg.size());
    //radio.envia((char*)"1.2.3.4.5.6.7.8.9.0 m", 19);
    sleep(2);
    recebe();

    cout << endl;
    return 0;
}
