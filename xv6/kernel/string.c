#include "types.h"
#include "x86.h"
#include "fs.h"

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

int
memcmp(const void *v1, const void *v2, uint n)
{
  const uchar *s1, *s2;
  
  s1 = v1;
  s2 = v2;
  while(n-- > 0){
    if(*s1 != *s2)
      return *s1 - *s2;
    s1++, s2++;
  }

  return 0;
}

void*
memmove(void *dst, const void *src, uint n)
{
  const char *s;
  char *d;

  s = src;
  d = dst;
  if(s < d && s + n > d){
    s += n;
    d += n;
    while(n-- > 0)
      *--d = *--s;
  } else
    while(n-- > 0)
      *d++ = *s++;

  return dst;
}

// memcpy exists to placate GCC.  Use memmove.
void*
memcpy(void *dst, const void *src, uint n)
{
  return memmove(dst, src, n);
}

int
strncmp(const char *p, const char *q, uint n)
{
  while(n > 0 && *p && *p == *q)
    n--, p++, q++;
  if(n == 0)
    return 0;
  return (uchar)*p - (uchar)*q;
}

char*
strncpy(char *s, const char *t, int n)
{
  char *os;
  
  os = s;
  while(n-- > 0 && (*s++ = *t++) != 0)
    ;
  while(n-- > 0)
    *s++ = 0;
  return os;
}

// Like strncpy but guaranteed to NUL-terminate.
char*
safestrcpy(char *s, const char *t, int n)
{
  char *os;
  
  os = s;
  if(n <= 0)
    return os;
  while(--n > 0 && (*s++ = *t++) != 0)
    ;
  *s = 0;
  return os;
}

int
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

int
findKeyInString(uchar* key, int keyLength, uchar* datastr)
{
  //keyLength = strlen((char*)key);
  int i, j;
  for (i=0; i<BSIZE; i+=32) //max total tag size
  {
    j=0;
    while(j<10 &&  // j is key index 
	  i+j<BSIZE && // i/32 tag's jth byte
	  key[j] && // if the key has anything at that point (null ref check)
	  datastr[i+j] && // if the datastr has something at that point (null ref check) 
 	  key[j]==datastr[i+j]) // check if the character are the same
    {
      j++;
    }
    if (j == keyLength &&
	!key[j] &&
	!datastr[i+j])
    {
      return i+j-keyLength;
    }
  }
  return -1;
}

int 
findTheEnd(uchar* datastr)
{
  int i=0;
  while(i<BSIZE &&
	datastr[i])
  {
    i+=32;
  }
  if (i==BSIZE)
  {
    return -1;
  }
  return i;
}
