//#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "Enquadramento.h"
#include "Serial.h"

using namespace std;


Serial rf("/dev/ttyUSB0", B9600);

Enquadramento radio(rf, 8, 40);

string msg = "um~} teste\r\n";

char buffer[32];
//char aux_buffer[40];
int n = 0;
char ex = 0x7e;

int main() {

    //cout << "Isso é um teste !!!" << endl;
    //cout << "msg.size(): " << msg.size() << endl;

    ARQ radio1(radio);
    radio1.envia(&msg[0], msg.size());
    //radio.envia((char*)"1.2.3.4.5.6.7.8.9.0 m", 19);
    sleep(2);
    n = rf.read(buffer, 32);
    cout << "Recebeu " << n << " bytes. \n";

    cout.write(buffer, n);

    cout << endl;
    return 0;
