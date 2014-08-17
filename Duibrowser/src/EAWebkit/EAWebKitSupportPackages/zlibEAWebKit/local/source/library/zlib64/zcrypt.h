// zcrypt.h -- interface for PKZip-compatible encryption/decryption
//   
// Copyright (C) 1998 Xceed Software Inc.
// All rights reserved.

#ifndef _ZCRYPT_H
#define _ZCRYPT_H

#include <windows.h>
#include "zlib.h"

#define Z_CRYPT_OK              0
#define Z_CRYPT_ERROR           1
#define Z_CRYPT_BADPASSWORD     2

// Length of the random header. Do not change :)

#define RAND_HEAD_LEN 12

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct z_crypt_s {

    // private variables

    uLong	 keys[3];			// encryption keys

    // input variables

    Byte     header[RAND_HEAD_LEN];		// random header for the current file
	char	*password;			// first byte in 0 terminated password
    WORD	 filetime;          // Low order word of last modified file time


} z_crypt;

typedef z_crypt *z_cryptp;

// Call cryptInit to initialise encryption data. A 12-byte encrypted
// header is provided when this function returns. For a Zip file,
// this header precedes the compressed file data.
//
// - requires the password and filetime variables in z_crypt structure

void cryptInit(z_cryptp cp);

// Call crypt to encrypt your data in place
//
// - requires the buf and bufsize variables in z_crypt structure

void crypt(z_cryptp cp, Byte *buf, uLong bufsize);

// Call cryptEnd to free the encryption data (reserved)

// void cryptEnd(z_cryptp cp);

// Call decryptInit to initialise decryption data. Side effect of this
// function: the header in the z_crypt structure is decrypted.
//
// -requires the password and header variables in z_crypt structure
// -returns Z_CRYPT_OK if it worked, Z_CRYPT_ERROR if it failed and
//  Z_CRYPT_BADPASSWORD if the password is incorrect.

int decryptInit(z_cryptp cp);

// Call decrypt to decrypt your data in place
//
// - requires the buf and bufsize variables in z_crypt structure

void decrypt(z_cryptp cp, Byte *buf, uLong bufsize);

// Call decryptEnd to free the encryption data

// void decryptEnd(z_cryptp cp);

// Other prototypes required for zcrypt.c

void randomizeHeader(z_cryptp cp);
Byte decryptByte(z_cryptp cp);
Byte updateKeys(z_cryptp cp, Byte c);
void initKeys(z_cryptp cp);

#ifdef __cplusplus
};
#endif

#endif /* _ZCRYPT_H */