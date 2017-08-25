#include <iostream>
#include "Serial.h"
 
using namespace std;
 int n = 0;
int main() {
  Serial rf("/dev/ttyUSB0", B9600);
  char buffer[32];
 
  int c = 0;
  while (true) { 
    n = rf.read(buffer, 32);     
    cout.write(buffer, n);
    c += n;
    if (c > 19) {
      c = 0;
      cout << endl;
    }
  }
}
