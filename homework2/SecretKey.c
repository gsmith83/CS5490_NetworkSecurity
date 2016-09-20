/*
	Author: 	Gregory Smith
	Class:   	Network Security
	HW: 		Homework 2
	File: 		SecretKey.c
	Date: 		09/05/2016
*/
#include <stdio.h>
#include <stdlib.h>

int stab1[16][16];
int stab2[16][16];
int stab3[16][16];
int stab4[16][16];
int stab5[16][16];
int stab6[16][16];
int stab7[16][16];
int stab8[16][16];
FILE * filePtr;
char * key;
unsigned char leftmask = 0xf0;
unsigned char rightmask = 0x0f;

unsigned char * encrypt(unsigned char * input){
	
	// xor with key, byte by byte
	int i;
	unsigned char * output = (unsigned char *) malloc(sizeof(unsigned char) * 9);
	for(i = 0; i < 8; i++){
		output[i] = input[i] ^ key[i];
	}
	output[8] = '\0';
	
	// character-by-character substitution
	// splits the charcter's encoding value into left 4 bits and right 4 bits, 
	// then looks up in substitution tables by row and column respectively
	output[0] = stab1[(output[0] & leftmask) >> 4][output[0] & rightmask];
	output[1] = stab2[(output[1] & leftmask) >> 4][output[1] & rightmask];
	output[2] = stab3[(output[2] & leftmask) >> 4][output[2] & rightmask];
	output[3] = stab4[(output[3] & leftmask) >> 4][output[3] & rightmask];
	output[4] = stab5[(output[4] & leftmask) >> 4][output[4] & rightmask];
	output[5] = stab6[(output[5] & leftmask) >> 4][output[5] & rightmask];
	output[6] = stab7[(output[6] & leftmask) >> 4][output[6] & rightmask];
	output[7] = stab8[(output[7] & leftmask) >> 4][output[7] & rightmask];
	
	// permutation (circular left bit shift output as a whole bit pattern)
	// circular left shifts the bits byte by byte, but still carries over the 
	// remaining bit to the appropriate byte
	unsigned char currentFirstBit = 0, prevFirstBit = 0;
	for(i = 7; i >= 0; i--){
		
		// capture first bit of this element
		currentFirstBit = output[i] & 0x80;
		
		// left shift the element
		output[i] <<= 1;
		
		// append previous last bit
		output[i] |= prevFirstBit >> 7;
		
		prevFirstBit = currentFirstBit;
	}
	// update first bit of first element
	output[7] |= prevFirstBit >> 7;
	
	return output;
}

unsigned char * decrypt(unsigned char * input){
	// reverse the permutation (circular right bit shift output as a whole bit pattern)
	int i, j;
	unsigned char * output = (unsigned char *) malloc(sizeof(unsigned char) * 9);
	unsigned char currentLastBit = 0, prevLastBit = 0;
	for(i = 0; i < 8; i++){
		output[i] = input[i];
		
		// capture last bit of this element
		currentLastBit = output[i] & 0x01;
		
		// right shift the element
		output[i] >>= 1;
		
		// append previous last bit
		output[i] |= prevLastBit << 7;
		
		prevLastBit = currentLastBit;
		
	}
	// update first bit of first element
	output[0] |= prevLastBit << 7;
	
	output[8] = '\0';
	
	// retrieve chars from s tables
	// goes through the s table and when it finds the character's encoding value,
	// reconstructs the original number by row and column index
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(output[0] == stab1[i][j]){
				output[0] = (i << 4) | j;
				goto end_loop1;
			}
		}
	}
	end_loop1:
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(output[1] == stab2[i][j]){
				output[1] = (i << 4) | j;
				goto end_loop2;
			}
		}
	}
	end_loop2:
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(output[2] == stab3[i][j]){
				output[2] = (i << 4) | j;
				goto end_loop3;
			}
		}
	}
	end_loop3:
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(output[3] == stab4[i][j]){
				output[3] = (i << 4) | j;
				goto end_loop4;
			}
		}
	}
	end_loop4:
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(output[4] == stab5[i][j]){
				output[4] = (i << 4) | j;
				goto end_loop5;
			}
		}
	}
	end_loop5:
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(output[5] == stab6[i][j]){
				output[5] = (i << 4) | j;
				goto end_loop6;
			}
		}
	}
	end_loop6:
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(output[6] == stab7[i][j]){
				output[6] = (i << 4) | j;
				goto end_loop7;
			}
		}
	}
	end_loop7:
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(output[7] == stab8[i][j]){
				output[7] = (i << 4) | j;
				goto end_loop8;
			}
		}
	}
	end_loop8:
	
	
	//reverse the xor, byte by byte
	for(i = 0; i < 8; i++){
		output[i] = output[i] ^ key[i];
	}
	
	return output;
}

// simple method to populate unique substitution tables
void createSTables(){
	int i, j;
	int x = 0; // value 0 to 255 inclusive to be assigned to s tables
	int y = 3; // addendum for table element variation
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			stab1[j][i] = x;										// each table switches i and j and increments the addendum twice
			stab2[(i + y++) % 16][(j + y++) % 16] = x; 
			stab3[(j + y++) % 16][(i + y++) % 16] = x;
			stab4[(i + y++) % 16][(j + y++) % 16] = x;
			stab5[(j + y++) % 16][(i + y++) % 16] = x;
			stab6[(i + y++) % 16][(j + y++) % 16] = x;
			stab7[(j + y++) % 16][(i + y++) % 16] = x;
			stab8[(i + y++) % 16][(j + y++) % 16] = x;
			
			x++;
		}
	}
}

// does a round of encryption and decryption (then 16 additional rounds) on a 8-character
// input string. Repeats this for a string identical to the first but for one flipped bit.
// sample strings 'abcdefgh' and 'ajcdefgh' were chosen because only the 2nd byte
// has one changed bit. User may input their own sample input and key on command line
int main(int argc, char ** argv){
	unsigned char * sampleInput = "abcdefgh";	// 8 chars as input
	key = "secretke";	// 64-bit key as an array of 8 chars
	int i;
	filePtr = fopen("SecretKey_output.txt", "w");
	
	// input is first argument, key is second
	if(argc > 1)
		sampleInput = argv[1];
	if(argc > 2)
		key = argv[2];
	
	createSTables();
	
	// Encrypt input 
	printf("Input = %s\nSecret Key = %s\n", sampleInput, key);
	fprintf(filePtr, "Input: %s\n", sampleInput);
	
	// Encryption rounds
	// 16 rounds
	unsigned char * output = encrypt(sampleInput);
	fprintf(filePtr, "Encryption (Output) Round 0: %s\n", output);
	
	for(i = 0; i < 16; i++){
		output = encrypt(output);
		
		fprintf(filePtr, "Encryption (Output) Round %d: %s\n", i, output);
	}
	
	// print encrypted message
	printf("Message encrypted = ");
	for(i = 0; i < 8; i++)
		printf("%c", output[i]);
	printf("\n");
	
	fprintf(filePtr, "Final ciphertext: %s\n\n", output);
	
	// Decryption rounds
	unsigned char * decrypted = decrypt(output);
	fprintf(filePtr, "Decryption (Output) Round 0: %s\n", decrypted);
	
	for(i = 0; i < 16; i++){
		decrypted = decrypt(decrypted);
		
		fprintf(filePtr, "Decryption (Output) Round %d: %s\n",i, decrypted);
	}
	
	// print decrypted message
	printf("Message decrypted = ");
	for(i = 0; i < 8; i++)
		printf("%c", decrypted[i]);
	printf("\n");
	
	fprintf(filePtr, "Final plaintext: %s\n\n", decrypted);
		
	// Encrypt input with a single bit changed (note: b = 98 = 0110 0010 while j = 106 = 0110 1010)
	sampleInput = "ajcdefgh";
	printf("Input = %s\nSecret Key = %s\n", sampleInput, key);
	fprintf(filePtr, "Input: %s\n", sampleInput);
	
	// Encryption rounds
	// 16 rounds
	output = encrypt(sampleInput);
	fprintf(filePtr, "Encryption (Output) Round 0: %s\n", output);
	
	for(i = 0; i < 16; i++){
		output = encrypt(output);
		
		fprintf(filePtr, "Encryption (Output) Round %d: %s\n", i, output);
	}
	
	// print encrypted message
	printf("Message encrypted = ");
	for(i = 0; i < 8; i++)
		printf("%c", output[i]);
	printf("\n");
	
	fprintf(filePtr, "Final ciphertext: %s\n\n", output);
	
	// Decryption rounds
	decrypted = decrypt(output);
	fprintf(filePtr, "Decryption (Output) Round 0: %s\n", decrypted);
	
	for(i = 0; i < 16; i++){
		decrypted = decrypt(decrypted);
		
		fprintf(filePtr, "Decryption (Output) Round %d: %s\n",i, decrypted);
	}
	
	// print decrypted message
	printf("Message decrypted = ");
	for(i = 0; i < 8; i++)
		printf("%c", decrypted[i]);
	printf("\n");
	
	fprintf(filePtr, "Final plaintext: %s\n\n", decrypted);
		
	fflush(filePtr);
	fclose(filePtr);
}