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
 * RIFF ファイルを読むためのメソッド群
 * ・末尾に _L がついているものは Little Endien
 * ・末尾に _B がついているものは Big Endien
 * ・末尾になにもついていないものは setLittleEndien() 又は setBigEndien() 
 *   に左右される
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
  
  void setLittleEndien(void) { isLittleEndien = true; }  // LittleEndien に設定
  void setBigEndien(void) { isLittleEndien = false; }    // BigEndien に設定

  // サイズを読み、このチャンクが終わる位置を返す
  // return: このチャンクの終わる位置
  DWORD readSize(void);
  DWORD readSize_L(void);
  DWORD readSize_B(void);
  
  // DWORD の数値を読む
  // return: 読んだ DWORD 値
  DWORD readDWORD(void);;
  DWORD readDWORD_L(void);
  DWORD readDWORD_B(void);

  // WORD の数値を読む
  // return: 読んだ DWORD 値
  WORD readWORD(void);
  WORD readWORD_L(void);
  WORD readWORD_B(void);

  // BYTE の数値を読む
  // return: 読んだ BYTE 値
  BYTE readBYTE(void);

  // 4 つの文字を読む
  // fourcc: 4 文字の文字列
  // return: fourcc と等しかったら true
  bool readFOURCC(char *fourcc);

  // 4 つの文字を読む
  // return: 4 つの文字
  char *readFOURCC(void);
  
  // IEEE 754 80bit 浮動小数点数 を読む
  // return: double で現した数
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


