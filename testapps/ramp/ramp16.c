#include "ctn_os.h"

int main()
{
 unsigned short v[256];
 int i;

 for (i = 0; i < 256; i++) {
  v[i] = i;
 }

 for (i = 0; i < 256; i++) {
  write(1, v, 512);
 }

 return 0;
}

