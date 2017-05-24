///////////////////////////////////////////////////////////////////////////////
// myread.h


#ifndef __myread_h__
#define __myread_h__


#include <stdio.h>
#include <string.h>
#include <malloc.h>


inline bool strequal(const char *str1, const char *str2)
{
  return strcmp(str1, str2) == 0;
}


typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long long QWORD;


/*
 * RIFF �ե�������ɤि��Υ᥽�åɷ�
 * �������� _L ���Ĥ��Ƥ����Τ� Little Endien
 * �������� _B ���Ĥ��Ƥ����Τ� Big Endien
 * �������ˤʤˤ�Ĥ��Ƥ��ʤ���Τ� setLittleEndien() ���� setBigEndien() 
 *   �˺��������
 */


class ReadRiff
{
  bool isLittleEndien;
  FILE *fp;
  
  WORD toWORD(BYTE a, BYTE b)
  {
    return (WORD(a) << 8) | b;
  }
  
  DWORD toDWORD(BYTE a, BYTE b, BYTE c, BYTE d)
  {
    return (DWORD(toWORD(a, b)) << 16) | toWORD(c, d);
  }
  
  QWORD toQWORD(BYTE a, BYTE b, BYTE c, BYTE d,
		BYTE e, BYTE f, BYTE g, BYTE h)
  {
    return  (QWORD(toDWORD(a, b, c, d)) << 32) |  toDWORD(e, f, g, f);
  }

public:
  
  ReadRiff(char *name, char *mode)
    : isLittleEndien(true),
      fp(fopen(name, mode))
  {
    if (!fp)
      exit(1);
  }

  ~ReadRiff()
  {
    delete fp;
  }
  
  // LittleEndien ������
  void setLittleEndien(void) { isLittleEndien = true; }
  
  // BigEndien ������
  void setBigEndien(void) { isLittleEndien = false; }
  
  // ���������ɤߡ����Υ���󥯤��������֤��֤�
  // return: ���Υ���󥯤ν�������
  DWORD readSize(void)
  {
    if (isLittleEndien) return readSize_L();
    else                return readSize_B();
  }
  
  DWORD readSize_L(void)
  {
    DWORD to = readDWORD_L();
    return to + getFilePointer();
  }
  
  DWORD readSize_B(void)
  {
    DWORD to = readDWORD_B();
    return to + getFilePointer();
  }
  
  // DWORD �ο��ͤ��ɤ�
  // return: �ɤ�� DWORD ��
  DWORD readDWORD(void)
  {
    if (isLittleEndien) return readDWORD_L();
    else                return readDWORD_B();
  }
  
  DWORD readDWORD_L(void)
  {
    BYTE buf[4];
    readFully(buf, sizeof(buf));
    return toDWORD(buf[3], buf[2], buf[1], buf[0]);
  }

  DWORD readDWORD_B(void)
  {
    BYTE buf[4];
    readFully(buf, sizeof(buf));
    return toDWORD(buf[0], buf[1], buf[2], buf[3]);
  }

  // WORD �ο��ͤ��ɤ�
  // return: �ɤ�� DWORD ��
  WORD readWORD(void)
  {
    if (isLittleEndien) return readWORD_L();
    else                return readWORD_B();
  }
  
  WORD readWORD_L(void)
  {
    BYTE buf[2];
    readFully(buf, sizeof(buf));
    return toWORD(buf[1], buf[0]);
  }
  
  WORD readWORD_B(void)
  {
    BYTE buf[2];
    readFully(buf, sizeof(buf));
    return toWORD(buf[0], buf[1]);
  }

  // BYTE �ο��ͤ��ɤ�
  // return: �ɤ�� BYTE ��
  BYTE readBYTE(void)
  {
    BYTE buf;
    readFully(&buf, 1);
    return buf;
  }

  // 4 �Ĥ�ʸ�����ɤ�
  // fourcc: 4 ʸ����ʸ����
  // return: fourcc ���������ä��� true
  bool readFOURCC(char *fourcc)
  {
    return strequal(fourcc, readFOURCC());
  }
  
  // 4 �Ĥ�ʸ�����ɤ�
  // return: 4 �Ĥ�ʸ��
  char *readFOURCC(void)
  {
    static char buf[5];
    readFully(buf, 4);
    return buf;
  }
  
  // IEEE 754 80bit ��ư�������� ���ɤ�
  // return: double �Ǹ�������
  double readFloat80(void)
  {
    if (isLittleEndien) return readFloat80_L();
    else                return readFloat80_B();
  }
  
  double readFloat80_L(void)
  {
    char buf[10];
    readFully(buf, sizeof(buf));
    QWORD f = toQWORD(buf[7], buf[6], buf[5], buf[4],
		      buf[3], buf[2], buf[1], buf[0]);
    QWORD mask1 = QWORD(0xffff0000) << 32;
    f = (f & mask1) | ((f << 1) & ~mask1);
    QWORD mask2 = QWORD(0xc0000000) << 32;
    f = (f & mask2) | ((f << 4) & ~mask2);
    return *(double *)&f;
  }
  
  double readFloat80_B(void)
  {
    char buf[10];
    readFully(buf, sizeof(buf));
    QWORD f = toQWORD(buf[0], buf[1], buf[2], buf[3],
		    buf[4], buf[5], buf[6], buf[7]);
    QWORD mask1 = QWORD(0xffff0000) << 32;
    f = (f & mask1) | ((f << 1) & ~mask1);
    QWORD mask2 = QWORD(0xc0000000) << 32;
    f = (f & mask2) | ((f << 4) & ~mask2);
    return *(double *)&f;
  }
  
  char *readFully(char *buf, int size)
  {
    if (fread(buf, size, 1, fp) < 1)
      exit(1);
    return buf;
  }
  
  BYTE *readFully(BYTE *buf, int size)
  {
    if (fread((char *)buf, size, 1, fp) < 1)
      exit(1);
    return buf;
  }

  long getFilePointer(void)
  {
    return ftell(fp);
  }

  void seek(long pos)
  {
#ifndef SEEK_SET
#define SEEK_SET 0
#endif
    fseek(fp, pos, SEEK_SET);
  }
};


#endif // __ReadRiff_h__
