#include "include.h"

unsigned char toint(char c)
{
    if (isxdigit(c) == 0)
    {
        c = '0';
    }

    if (isdigit(c))
    {
        c -= '0';
    }
    else
    {
        if (islower(c))
        {
            c = toupper(c);
        }
        c = c - 'A' + 10;
    }

    return ((unsigned char)c);
}

/*将0-15的整型数转化成字符0-F*/
char tochar(int x)
{
    char c;

    if((x>=0) && (x<=9))
    {
        c = x + 0x30;
    }
    else if((x>9) && (x<=15))
    {
        c = x + 0x37;
    }
    else
    {
        c = 0;
    }

    return c;
}


char *itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;
	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;

} /* NCL_Itoa */


char *hex_to_string(unsigned int val, char *string)
{
	unsigned char byte;
	unsigned char bh=0, bl=0, i;
	char *ptr = string;

	if(val == 0)
	{
		*ptr++ = '0';
		*ptr++ = '0';

		ptr = 0;
		return string;
	}

	for(i=0; i<4; i++)
	{
		byte = (unsigned char)(val >> (3-i)*8);
		bh = (byte>>4) & 0x0F;
		bl = byte & 0x0F;
		if((bh!=0) || (bl!=0))
		{
			if(bh>=10)
			{
				*ptr++ = 'A' + (bh-10);
			}
			else
			{
				*ptr++ = '0' + bh;
			}

			if(bl>=10)
			{
				*ptr++ = 'A' + (bl-10);
			}
			else
			{
				*ptr++ = '0' + bl;
			}
		}
	}
	*ptr = 0;

	return string;
}


/* 返回字符串foundStr第n次出现在字符串str中的位置。如果没找到，返回NULL
   必须符合标准函数strstr的调用条件 */
char *strStrfN(const char *str, char const *foundStr, unsigned int n)
{
	char *pStr;

	if (n == 0)
		return 0;

	pStr = (char*)str;
	while (n--)
	{
		pStr = strstr(pStr, foundStr);
		if (pStr == 0)
			break;
		else
		{
			if (n)
			{
				if (*(++pStr) == 0)
				{
					pStr = 0;
					break;
				}
			}
		}
	};

	return (pStr);
}




int checkCPUendian(void)
{
	union
	{
		unsigned long int i;
		unsigned char s[4];
	} c;

	c.i = 0x12345678;
	return (0x12 == c.s[0]);
}


u16 htons(u16 n)
{
	return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

/**
 * Convert an u16_t from network- to host byte order.
 *
 * @param n u16_t in network byte order
 * @return n in host byte order
 */
u16 ntohs(u16 n)
{
	return checkCPUendian()? n: htons(n);
}

/**
 * Convert an u32_t from host- to network byte order.
 *
 * @param n u32_t in host byte order
 * @return n in network byte order
 */
u32 htonl(u32 n)
{
	return ((n & 0xff) << 24) |
	       ((n & 0xff00) << 8) |
	       ((n & 0xff0000UL) >> 8) |
	       ((n & 0xff000000UL) >> 24);
}

u32 ntohl(u32 n)
{
	return checkCPUendian()? n : htonl(n);
}


/* 字符串数字转换成BCD码，如"18706702289" -> 0x01,0x87,0x06,0x70,0x22,0x89 */
unsigned int getBCDFromStr(unsigned char *dest, char *src)
{
	unsigned int srcLen, i, j;

	srcLen = strlen(src);
	for(i=0; i<srcLen; i++)
	{
		if (isxdigit( *(src + i) ) == 0)
		{
			return 0;
		}
	}

	if(srcLen%2 == 0)
	{
		for(i=0,j=0; i<srcLen; i+=2)
		{
			dest[j++] = ((src[i]-'0')<<4) | (src[i+1]-'0');
		}
	}
	else
	{
		j=0;
		dest[j++] = src[0] - '0';
		for(i=1; i<srcLen; i+=2)
		{
			dest[j++] = ((src[i]-'0')<<4) | (src[i+1]-'0');
		}
	}

	return j;
}
