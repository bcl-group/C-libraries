#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "ReadRiff.h"

DWORD ReadRiff::readSize(void)
{
    if (isLittleEndien) return readSize_L();
    else                return readSize_B();
}

DWORD ReadRiff::readSize_L(void)
{
    DWORD to = readDWORD_L();
    return to + getFilePointer();
}

DWORD ReadRiff::readSize_B(void)
{
    DWORD to = readDWORD_B();
    return to + getFilePointer();
}

DWORD ReadRiff::readDWORD(void)
{
    if (isLittleEndien) return readDWORD_L();
    else                return readDWORD_B();
}
  
DWORD ReadRiff::readDWORD_L(void)
{
    BYTE buf[4];
    readFully(buf, sizeof(buf));
    return toDWORD(buf[3], buf[2], buf[1], buf[0]);
}

DWORD ReadRiff::readDWORD_B(void)
{
    BYTE buf[4];
    readFully(buf, sizeof(buf));
    return toDWORD(buf[0], buf[1], buf[2], buf[3]);
}

WORD ReadRiff::readWORD(void)
{
    if (isLittleEndien) return readWORD_L();
    else                return readWORD_B();
}
  
WORD ReadRiff::readWORD_L(void)
{
    BYTE buf[2];
    readFully(buf, sizeof(buf));
    return toWORD(buf[1], buf[0]);
}
  
WORD ReadRiff::readWORD_B(void)
{
    BYTE buf[2];
    readFully(buf, sizeof(buf));
    return toWORD(buf[0], buf[1]);
}

BYTE ReadRiff::readBYTE(void)
{
    BYTE buf;
    readFully(&buf, 1);
    return buf;
}

bool ReadRiff::readFOURCC(char *fourcc)
{
    return strequal(fourcc, readFOURCC());
}
  
char* ReadRiff::readFOURCC(void)
{
    static char buf[5];
    readFully(buf, 4);
    return buf;
}
  
double ReadRiff::readFloat80(void)
{
    if (isLittleEndien) return readFloat80_L();
    else                return readFloat80_B();
}
  
double ReadRiff::readFloat80_L(void)
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
  
double ReadRiff::readFloat80_B(void)
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

char* ReadRiff::readFully(char *buf, int size)
  {
    if (fread(buf, size, 1, fp) < 1)
      exit(1);
    return buf;
  }
  
BYTE* ReadRiff::readFully(BYTE *buf, int size)
  {
    if (fread((char *)buf, size, 1, fp) < 1)
      exit(1);
    return buf;
  }

long ReadRiff::getFilePointer(void)
  {
    return ftell(fp);
  }

void ReadRiff::seek(long pos)
  {
#ifndef SEEK_SET
#define SEEK_SET 0
#endif
    fseek(fp, pos, SEEK_SET);
  }

//////////////////////////////////////

void ShowRiff::showMainAVIHeader(char *space, int /* size */)
  {
    dwMicroSecPerFrame = rr.readDWORD();
    dwMaxBytesPerSec = rr.readDWORD();
    dwPaddingGranluarity = rr.readDWORD();
    int flags = (int)rr.readDWORD();
    dwFlags = flags;
    dwTotalFrames = rr.readDWORD();
    dwInitialFrames = rr.readDWORD();
    dwStreams = rr.readDWORD();
    dwSuggestedBufferSize = rr.readDWORD();
    dwWidth = rr.readDWORD();
    dwHeight = rr.readDWORD();
  }
  
void ShowRiff::showAVIStreamHeader(char *space, int /* size */)
  {
    strncpy(fccType, rr.readFOURCC(), 4);
    fcc = fccType;
    fccHandler = rr.readFOURCC();
    int flags = rr.readDWORD();
    dwFlags = flags;
   
    printf("\n");
    wPriority = rr.readWORD();
    wLanguage = rr.readWORD();
    dwInitialFrames = rr.readDWORD();
    dwScale = rr.readDWORD();
    dwRate = rr.readDWORD();
    dwStart = rr.readDWORD();
    dwLength = rr.readDWORD();
    dwSuggestedBufferSize = rr.readDWORD();
    dwQuality = rr.readDWORD();
    dwSampleSize = rr.readDWORD();
  }

void ShowRiff::showBITMAPINFOHEADER(char * space, int /* size */)
  {
    biSize = rr.readDWORD();
    biWidth = rr.readDWORD();
    biHeight = rr.readDWORD();
    biPlanes = rr.readWORD();
    biBitCount = rr.readWORD();
    biCompression = rr.readFOURCC();
    biSizeImage = rr.readDWORD();
    biXPelsPerMeter = rr.readDWORD();
    biYPelsPerMeter = rr.readDWORD();
    biClrUsed = rr.readDWORD();
    biClrImportant = rr.readDWORD();
  }
  
void ShowRiff::showWAVEFORMATEX(char * space, int /* size */)
  {
    wFormatTag = rr.readWORD();
    nChannels = rr.readWORD();
    nSamplesPerSec = rr.readDWORD();
    nAvgBytesPerSec = rr.readDWORD();
    nBlockAlign = rr.readWORD();
    wBitsPerSample = rr.readWORD();
  }
  
char ShowRiff::showAVIINDEXENTRY(char * space, int size)
  {
    size /= 16;
    for (int i = 0; i < size; i++)
    {
      ckid = rr.readFOURCC();
      int flags = rr.readDWORD();
      dwFlags = flags;
      dwChunkOffset = rr.readDWORD();
      dwChunkLength = rr.readDWORD();
    }
     return(*ckid);
  }
  
void ShowRiff::showAIFFCOMM(char * space, int size)
  {
    numChannels = rr.readWORD();
    numSampleFrames = rr.readDWORD();
    numSampleSize = rr.readWORD();
    numSampleRate = rr.readFloat80();
    if (18 < size)
    {
      compressionType = rr.readFOURCC();
    }
    if (22 < size)
    {
      int len = rr.readBYTE();
      BYTE *buf = (BYTE *)alloca(len);
      rr.readFully(buf, len);
      compressionName = buf;
    }
  }
  
void ShowRiff::showAIFFAPCM(char * space, int size)
  {
    offset = rr.readDWORD();
    blockSize = rr.readDWORD();
  }
  
void ShowRiff::showAsString(char * space, int size)
  {
    BYTE *buf = (BYTE *)alloca(size);
    rr.readFully(buf, size);
  }


void ShowRiff::showRiff(int indent, int chunk_size)
{
    char *space = (char *)alloca(indent * 6 + 1);
    for (int i = 0; i < indent * 6; i++)
      space[i] = ' ';
    space[indent * 6] = '\0';

    while ((indent == 0 && 0 <= chunk_size) ||
	   (0 < indent && 0 < chunk_size))
    {
      char sort[5];
      strcpy(sort, rr.readFOURCC());
      
      if (sort[0] == '\0')
      {
	return;
      }
      
      if (strequal(sort, "RIFF"))
	rr.setLittleEndien();
      else if (strequal(sort, "FORM"))
	rr.setBigEndien();
      
      
      int size = rr.readDWORD();
      long now = rr.getFilePointer();
          
      if (strequal(sort, "RIFF") || strequal(sort, "LIST") ||
	  strequal(sort, "FORM"))
      {
	strcpy(sort, rr.readFOURCC());
	showRiff(indent + 1, size - 4);
      }
      
      // 各種ヘッダを表示
      if      (strequal(sort, "avih")) showMainAVIHeader   (space, size);
      else if (strequal(sort, "strh")) showAVIStreamHeader (space, size);
      else if (strequal(sort, "strf"))
	if (strequal(fccType, "vids")) showBITMAPINFOHEADER(space, size);
	else                           showWAVEFORMATEX    (space, size);
      else if (strequal(sort, "idx1")) showAVIINDEXENTRY   (space, size);
      else if (strequal(sort, "fmt ")) showWAVEFORMATEX    (space, size);
      else if (strequal(sort, "COMM")) showAIFFCOMM        (space, size);
      else if (strequal(sort, "APCM")) showAIFFAPCM        (space, size);
      else if (strequal(sort, "strn") ||
	       strequal(sort, "ISMP") ||
	       strequal(sort, "IDIT")) showAsString        (space, size);
      
      size = (size + 1) / 2 * 2;
      rr.seek(now + size);
      chunk_size -= size + 8;
    }
};




