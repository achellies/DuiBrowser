// zcrypt.c -- code for PKZip-compatible encryption/decryption
//   
// Copyright (C) 1998 Xceed Software Inc.
// All rights reserved.

#include "zcrypt.h"

// Initialise the z_crypt structure so you can start encrypting data

void cryptInit(z_cryptp cp)
{
    randomizeHeader(cp);                    // The header precedes the compressed data
    initKeys(cp);                           // Initialises the keys with the password
    crypt(cp, cp->header, RAND_HEAD_LEN);   // Further initialises the keys using the header
}

// Encrypt data in-place

void crypt(z_cryptp cp, Byte *buf, uLong bufsize)
{
    Byte  ztemp;
    uLong i;

    for (i = 0; i < bufsize; i++) 
    {
        ztemp = decryptByte(cp);
        updateKeys(cp, buf[i]);
        buf[i] = ztemp ^ buf[i];
    }
}

// Doesn't do anything for now because nothing is allocated

//void cryptEnd(z_cryptp cp)
//{
//    Reserved
//}

// Randomize the 12 byte header and include a portion of the filetime
// parameter supplied in the z_crypt structure. The filetime portion
// can be used later to discover if the supplied decryption password
// is valid or not.

void randomizeHeader(z_cryptp cp)
{
    int   i;
    DWORD rdword;

    rdword = GetTickCount() ^ (cp->filetime << 5); 

    for (i=0; i < RAND_HEAD_LEN-2; i++)
    {
        cp->header[i] = (Byte) ((((rdword >> (i*3)) & 0xff) + (i*23)) % 256);
    }

    cp->header[RAND_HEAD_LEN-2] = cp->filetime & 0xff;
    cp->header[RAND_HEAD_LEN-1] = (cp->filetime >> 8) & 0xff;
}

// Initialise the encryption keys with the password

void initKeys(z_cryptp cp)
{
  char *passwd = cp->password;

  cp->keys[0] = 305419896L;
  cp->keys[1] = 591751049L;
  cp->keys[2] = 878082192L;
 
  while (*passwd != '\0') 
  {
    updateKeys(cp, *passwd);
    passwd++;
  }
}

// Update the keys with a byte of plaintext

Byte updateKeys(z_cryptp cp, Byte b)
{
    cp->keys[0] = uniqueCrc32(cp->keys[0], b);
    cp->keys[1] += cp->keys[0] & 0xff;
    cp->keys[1] = cp->keys[1] * 134775813L + 1;
    cp->keys[2] = uniqueCrc32(cp->keys[2], cp->keys[1] >> 24);
    return b;
}

// Return the next byte in the pseudo-random sequence
 
Byte decryptByte(z_cryptp cp)
{
   unsigned short temp;

   temp = (unsigned short)cp->keys[2] | 2;
   return (Byte)(((unsigned short)(temp * (temp ^ 1)) >> 8) & 0xff);
}

// Initialise the z_crypt structure so you can start decrypting data

int decryptInit(z_cryptp cp)
{
    initKeys(cp);                           // Initialise the keys with the password
    decrypt(cp, cp->header, RAND_HEAD_LEN); // Further init. the keys using the header

    // Return code depends on whether the password checks out (not 100% proof though)

    return (cp->header[RAND_HEAD_LEN-1] == ((cp->filetime >> 8) & 0xff)) ? Z_CRYPT_OK : Z_CRYPT_BADPASSWORD;
}

// Decrypt data in-place

void decrypt(z_cryptp cp, Byte *buf, uLong bufsize)
{
    uLong i;

    for (i = 0; i < bufsize; i++) 
    {
        buf[i] ^= decryptByte(cp); 
        updateKeys(cp, buf[i]);
    }
}
