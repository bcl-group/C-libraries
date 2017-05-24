///////////////////////////////////////////////////////////////////////////////
// ReadRiff.h


#ifndef __ReadRiff_h__
#define __ReadRiff_h__


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
  
  WORD toWORD(BYTE a, BYTE b) { return (WORD(a) << 8) | b; }
  DWORD toDWORD(BYTE a, BYTE b, BYTE c, BYTE d)
    { return (DWORD(toWORD(a, b)) << 16) | toWORD(c, d); }
  QWORD toQWORD(BYTE a, BYTE b, BYTE c, BYTE d,
		BYTE e, BYTE f, BYTE g, BYTE h)
    { return  (QWORD(toDWORD(a, b, c, d)) << 32) |  toDWORD(e, f, g, f); }

public:
  ReadRiff(char *name, char *mode)
    : isLittleEndien(true),
      fp(fopen(name, mode))
  { if (!fp) exit(1); }

  ~ReadRiff() { fclose(fp); }
  
  void setLittleEndien(void) { isLittleEndien = true; }  // LittleEndien ������
  void setBigEndien(void) { isLittleEndien = false; }    // BigEndien ������

  // ���������ɤߡ����Υ���󥯤��������֤��֤�
  // return: ���Υ���󥯤ν�������
  DWORD readSize(void);
  DWORD readSize_L(void);
  DWORD readSize_B(void);
  
  // DWORD �ο��ͤ��ɤ�
  // return: �ɤ�� DWORD ��
  DWORD readDWORD(void);;
  DWORD readDWORD_L(void);
  DWORD readDWORD_B(void);

  // WORD �ο��ͤ��ɤ�
  // return: �ɤ�� DWORD ��
  WORD readWORD(void);
  WORD readWORD_L(void);
  WORD readWORD_B(void);

  // BYTE �ο��ͤ��ɤ�
  // return: �ɤ�� BYTE ��
  BYTE readBYTE(void);

  // 4 �Ĥ�ʸ�����ɤ�
  // fourcc: 4 ʸ����ʸ����
  // return: fourcc ���������ä��� true
  bool readFOURCC(char *fourcc);

  // 4 �Ĥ�ʸ�����ɤ�
  // return: 4 �Ĥ�ʸ��
  char *readFOURCC(void);
  
  // IEEE 754 80bit ��ư�������� ���ɤ�
  // return: double �Ǹ�������
  double readFloat80(void);
  double readFloat80_L(void);
  double readFloat80_B(void);
  char *readFully(char *buf, int size);
  BYTE *readFully(BYTE *buf, int size);
  long getFilePointer(void);
  void seek(long pos);
};


//////////////////////////////////////////////////////////////////
// ShowRiff definition

class ShowRiff
{
 private:
  char *fcc,*fccHandler;
  int dwFlags ,wPriority, wLanguage, dwScale,dwRate;
  int dwStart, dwLength ,  dwQuality, dwSampleSize;
  int biSize, biWidth, biHeight, biPlanes, biBitCount, biSizeImage;
  int biXPelsPerMeter, biYPelsPerMeter, biClrUsed , biClrImportant;
  int wFormatTag, nChannels, nSamplesPerSec, nAvgBytesPerSec;
  int nBlockAlign, wBitsPerSample,dwChunkOffset, dwChunkLength;
  int dwMicroSecPerFrame, dwMaxBytesPerSec, dwPaddingGranluarity;
  int dwTotalFrames, dwInitialFrames , dwStreams; 
  int dwSuggestedBufferSize;
  int dwWidth, dwHeight;

  int  numChannels, numSampleFrames, numSampleSize;
  double  numSampleRate;
  BYTE *compressionName;
  int  offset, blockSize;
  char  *biCompression, *compressionType,*ckid;

  ReadRiff rr;
  char fccType[5];

  void showMainAVIHeader(char *space, int /* size */);
  void showAVIStreamHeader(char *space, int /* size */);
  void showBITMAPINFOHEADER(char * space, int /* size */);
  void showWAVEFORMATEX(char * space, int /* size */);
  char showAVIINDEXENTRY(char * space, int size);
  void showAIFFCOMM(char * space, int size);
  void showAIFFAPCM(char * space, int size);
  void showAsString(char * space, int size);
  void showRiff(int indent, int chunk_size);

public:
  //  void showRiff(void) { showRiff(0, 0);  }
  ShowRiff(char *name)
    : rr(name, "r")
  {    fccType[4] = '\0';  showRiff(0, 0);} 

  int getWidth(void){return dwWidth;}
  int getHeight(void){return dwHeight;}
  int getTotalFrames(void){return dwTotalFrames;}
  int getbiSizeImage(void){return biSizeImage;}
  int getbiBitCount(void){return biBitCount;}
};

#endif // __ReadRiff_h__


