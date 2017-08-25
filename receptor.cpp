#include <iostream>
//#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <errno.h>
#include "Enquadramento.h"
#include "Serial.h"
 
using namespace std; 

//  g++ -g receptor.cpp -oreceptor  Enquadramento.cpp Serial.cpp -std=c++11
// ls -lh /dev/ttyU*

Serial rf("/dev/ttyUSB1", B9600);

Enquadramento radio(rf, 8, 40);

char buffer[64];
int n = 0;

int main() {
	 /*
	cout << "Recebeu " << n << " bytes. \n";	 
	cout.write(buffer, n);	 
	cout << endl;*/
		
		// Bloqueante !!!!!
	while(1){
		n = radio.recebe(buffer);//printf(" \nN %d\n", n);
		printf ("\nBuffer recebido Hex:");
		for(int h = 0; h<n; h++){
			printf(" %x", buffer[h]);
		}	
		printf ("\nBuffer recebido char: ");
		cout.write(buffer, n);	
		n = 0;
	}
	printf ("\nEncerrando programa!\n");	
	return 0;
}
