#include <stdio.h>
#include "aes.h"

int main () {

	aes_block_t plaintext;
	aes_key_t key;
	aes_block_t ciphertext;

	int retVal = 0;

	// Test 1

	plaintext(127, 64) = 0x6BC1BEE22E409F96;
	plaintext(63, 0) = 0xE93D7E117393172A;
	key(127, 64) = 0x2B7E151628AED2A6;
	key(63, 0) = 0xABF7158809CF4F3C;

	ciphertext = aes_encrypt(plaintext, key);

	if(ciphertext(127, 64) != 0x3AD77BB40D7A3660 || ciphertext(63, 0) != 0xA89ECAF32466EF97){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #1)\n";
	}

	// Test 2

	plaintext(127, 64) = 0xAE2D8A571E03AC9C;
	plaintext(63, 0) = 0x9EB76FAC45AF8E51;

	ciphertext = aes_encrypt(plaintext, key);

	if(ciphertext(127, 64) != 0xF5D3D58503B9699D || ciphertext(63, 0) != 0xE785895A96FDBAAF){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #2)\n";
	}

	// Test 3

	plaintext(127, 64) = 0x30C81C46A35CE411;
	plaintext(63, 0) = 0xE5FBC1191A0A52EF;

	ciphertext = aes_encrypt(plaintext, key);

	if(ciphertext(127, 64) != 0x43B1CD7F598ECE23 || ciphertext(63, 0) != 0x881B00E3ED030688){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #3)\n";
	}

	// Test 4

	plaintext(127, 64) = 0xF69F2445DF4F9B17;
	plaintext(63, 0) = 0xAD2B417BE66C3710;

	ciphertext = aes_encrypt(plaintext, key);

	if(ciphertext(127, 64) != 0x7B0C785E27E8AD3F || ciphertext(63, 0) != 0x8223207104725DD4){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #4)\n";
	}

	// Test 5

	plaintext(127, 64) = 0;
	plaintext(63, 0) = 0;
	key(127, 64) = 0;
	key(63, 0) = 0;

	ciphertext = aes_encrypt(plaintext, key);

	if(ciphertext(127, 64) != 0x66e94bd4ef8a2c3b || ciphertext(63, 0) != 0x884cfa59ca342b2e){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #5)\n";
	}

	// Test 6

	plaintext(127, 64) = 0;
	plaintext(63, 0) = 0;
	key(127, 64) = 0xFFFFFFFFFFFFFFFF;
	key(63, 0) = 0xFFFFFFFFFFFFFFFF;

	ciphertext = aes_encrypt(plaintext, key);

	if(ciphertext(127, 64) != 0xa1f6258c877d5fcd || ciphertext(63, 0) != 0x8964484538bfc92c){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #6)\n";
	}

	// Test 7

	plaintext(127, 64) = 0xFFFFFFFFFFFFFFFF;
	plaintext(63, 0) = 0xFFFFFFFFFFFFFFFF;
	key(127, 64) = 0;
	key(63, 0) = 0;

	ciphertext = aes_encrypt(plaintext, key);

	if(ciphertext(127, 64) != 0x3f5b8cc9ea855a0a || ciphertext(63, 0) != 0xfa7347d23e8d664e){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #7)\n";
	}

	// Test 8

	plaintext(127, 64) = 0xFFFFFFFFFFFFFFFF;
	plaintext(63, 0) = 0xFFFFFFFFFFFFFFFF;
	key(127, 64) = 0xFFFFFFFFFFFFFFFF;
	key(63, 0) = 0xFFFFFFFFFFFFFFFF;

	ciphertext = aes_encrypt(plaintext, key);

	if(ciphertext(127, 64) != 0xbcbf217cb280cf30 || ciphertext(63, 0) != 0xb2517052193ab979){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #8)\n";
	}


	if(retVal){
		std::cout << "C Simulation failed\n";
	} else {
		std::cout << "C Simulation successful\n";
	}

	return retVal;
}
