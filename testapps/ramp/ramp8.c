#include "ctn_os.h"

int main()
{
 unsigned char v[256];
 int i;

 for (i = 0; i < 256; i++) {
  v[i] = i;
 }

 for (i = 0; i < 256; i++) {
  write(1, v, 256);
 }

 return 0;
}

