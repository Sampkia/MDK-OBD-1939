#ifndef PUBLIC_H_
#define PUBLIC_H_

#define min(a,b)				(a < b ? a : b)
#define max(a,b)				(a > b ? a : b)

#define setbit(val, bitn) (val |= 1<< bitn)
#define clrbit(val, bitn) (val &= ~(1<< bitn))
#define getbit(val, bitn) ((val >> bitn) & 1)

unsigned char toint(char c);
char *itoa(int value, char *string, int radix);
char *hex_to_string(unsigned int val, char *string);
char *strStrfN(const char *str, char const *foundStr, unsigned int n);
unsigned int getBCDFromStr(unsigned char *dest, char *src);
u32 ntohl(u32 n);
u32 htonl(u32 n);
u16 ntohs(u16 n);
u16 htons(u16 n);
int checkCPUendian(void);
char tochar(int x);

#endif
