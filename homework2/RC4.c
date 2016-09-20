/*
	Author: 	Gregory Smith
	Class:   	Network Security
	HW: 		Homework 2
	File: 		RC4.c
	Date: 		09/05/2016
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char uns8;
typedef unsigned short uns16;

static uns8 state[256], x, y;
uns8 keyLength;

void rc4init(uns8 * key, uns16 length){
	int i;
	uns8 t;
	uns8 j;
	uns8 k = 0;
	keyLength = length;
	
	for(i = 256; i--; )
		state[i] = i;
	
	for(i = 0, j = 0; i < 256; i++, j = (j + 1) % length){
		t = state[i], state[i] = state[k += key[j] + t], state[k] = t;
	}
	
	x = 0;
	y = 0;
}

uns8 rc4step(){
	uns8 t;
	
	t = state[y += state[++x]], state[y] = state[x], state[x] = t;
	
	return (state[(state[x] + state[y]) % 256]);	// minor bug correction: without the mod of sizeof(state) (256)
																			//  the index will frequently go past the end of the state array
}

int main(int argc, char ** argv){
	FILE * filePtr;
	// get key, keyLength and message from command-line
	if(argc != 4){
		printf("(%d) Usage: RC4 <key> <key length> <'message'>\n", argc);
		exit(-1);
	}
	char * key = argv[1];
	int length = atoi(argv[2]);
	char * message = argv[3];
	int msgLength = strlen(message);
	
	printf("Key = %s\nKeyLength = %d\nMessage = %s\n", key, length, message);
	
	// Encryption
	rc4init(key, length);
	
	// ignore first 512 octets
	int i;
	for(i = 0; i < 512; i++){
		rc4step();
	}
	
	// Encrypt message
	char encryptedMsg[msgLength];
	for(i = 0; i < msgLength; i++){
		encryptedMsg[i] = message[i] ^ rc4step();	// XOR octet with each message character
	}
	encryptedMsg[i] = '\0';
	
	// print encrypted message
	printf("Encrypted Message = %s\n", encryptedMsg);
	// write encrypted message to file
	filePtr = fopen("RC4_output.txt", "w");
	fwrite(encryptedMsg, sizeof(char), msgLength, filePtr);
	fclose(filePtr);
	
	// Decryption
	rc4init(key, length);
	
	// ignore first 512 octets
	for(i = 0; i < 512; i++){
		rc4step();
	}
	
	
	// Decrypt message
	char decryptedMsg[msgLength];
	for(i = 0; i < msgLength; i++){
		decryptedMsg[i] = encryptedMsg[i] ^ rc4step();
	}
	decryptedMsg[i] = '\0';
	
	// print decrypted message
	printf("Decrypted Message = %s\n", decryptedMsg);
	
}