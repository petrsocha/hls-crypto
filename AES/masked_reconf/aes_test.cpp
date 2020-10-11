#include <stdio.h>
#include "aes.h"

int main () {

	aes_block_t plaintext;
	aes_key_t key;
	aes_block_t ciphertext;
	aes_block_t mask1, mask2;

	int retVal = 0;

	// Test 1

	plaintext(127, 64) = 0x6BC1BEE22E409F96;
	plaintext(63, 0) = 0xE93D7E117393172A;
	key(127, 64) = 0x2B7E151628AED2A6;
	key(63, 0) = 0xABF7158809CF4F3C;

	mask1(127, 64) = 0xAB547C8A654123DC;
	mask1(63, 0) = 0x32A4156A41C24FE3;
	mask2(127, 64) = 0x1245A48964B5CDE1;
	mask2(63, 0) = 0xED421FD56C56A134;

	plaintext = plaintext ^ mask1;
	ciphertext = aes_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0x3AD77BB40D7A3660 || ciphertext(63, 0) != 0xA89ECAF32466EF97){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #1)\n";
	}

	// Test 2

	plaintext(127, 64) = 0xAE2D8A571E03AC9C;
	plaintext(63, 0) = 0x9EB76FAC45AF8E51;

	mask1(127, 64) = 0x12312c1b23d15eDC;
	mask1(63, 0) = 0x12456d231e231a23;
	mask2(127, 64) = 0x6542315D654A235E;
	mask2(63, 0) = 0xF126456E23150123;

	plaintext = plaintext ^ mask1;
	ciphertext = aes_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0xF5D3D58503B9699D || ciphertext(63, 0) != 0xE785895A96FDBAAF){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #2)\n";
	}

	// Test 3

	plaintext(127, 64) = 0x30C81C46A35CE411;
	plaintext(63, 0) = 0xE5FBC1191A0A52EF;

	mask1(127, 64) = 0xA415465A12300F04;
	mask1(63, 0) = 0x254321A23123C123;
	mask2(127, 64) = 0x4543204564320451;
	mask2(63, 0) = 0xE454560D560EC056;

	plaintext = plaintext ^ mask1;
	ciphertext = aes_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0x43B1CD7F598ECE23 || ciphertext(63, 0) != 0x881B00E3ED030688){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #3)\n";
	}

	// Test 4

	plaintext(127, 64) = 0xF69F2445DF4F9B17;
	plaintext(63, 0) = 0xAD2B417BE66C3710;

	mask1(127, 64) = 0x45521120654123DC;
	mask1(63, 0) = 0x32A4156A41C24FE3;
	mask2(127, 64) = 0x1245A48464B5CDE1;
	mask2(63, 0) = 0xED421DD56C56A134;

	plaintext = plaintext ^ mask1;
	ciphertext = aes_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0x7B0C785E27E8AD3F || ciphertext(63, 0) != 0x8223207104725DD4){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #4)\n";
	}

	// Test 5

	plaintext(127, 64) = 0;
	plaintext(63, 0) = 0;
	key(127, 64) = 0;
	key(63, 0) = 0;

	mask1(127, 64) = 0x7456410A654123DC;
	mask1(63, 0) = 0x32A4154501C24FE3;
	mask2(127, 64) = 0x124EDCFA64B5CDE1;
	mask2(63, 0) = 0xED4E121023112334;

	plaintext = plaintext ^ mask1;
	ciphertext = aes_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0x66e94bd4ef8a2c3b || ciphertext(63, 0) != 0x884cfa59ca342b2e){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #5)\n";
	}

	// Test 6

	plaintext(127, 64) = 0;
	plaintext(63, 0) = 0;
	key(127, 64) = 0xFFFFFFFFFFFFFFFF;
	key(63, 0) = 0xFFFFFFFFFFFFFFFF;

	mask1(127, 64) = 0xAB850789412123DC;
	mask1(63, 0) = 0x3287945123465303;
	mask2(127, 64) = 0x9874521465401DE1;
	mask2(63, 0) = 0xED44785644515234;

	plaintext = plaintext ^ mask1;
	ciphertext = aes_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0xa1f6258c877d5fcd || ciphertext(63, 0) != 0x8964484538bfc92c){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #6)\n";
	}

	// Test 7

	plaintext(127, 64) = 0xFFFFFFFFFFFFFFFF;
	plaintext(63, 0) = 0xFFFFFFFFFFFFFFFF;
	key(127, 64) = 0;
	key(63, 0) = 0;

	mask1(127, 64) = 0xAB452A456748201C;
	mask1(63, 0) = 0x32AA5A4156C24FE3;
	mask2(127, 64) = 0x1A45A484A4564DE1;
	mask2(63, 0) = 0xED464651A256A134;

	plaintext = plaintext ^ mask1;
	ciphertext = aes_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0x3f5b8cc9ea855a0a || ciphertext(63, 0) != 0xfa7347d23e8d664e){
		retVal++;
		std::cout << "AES encryption output mismatch (test vector #7)\n";
	}

	// Test 8

	plaintext(127, 64) = 0xFFFFFFFFFFFFFFFF;
	plaintext(63, 0) = 0xFFFFFFFFFFFFFFFF;
	key(127, 64) = 0xFFFFFFFFFFFFFFFF;
	key(63, 0) = 0xFFFFFFFFFFFFFFFF;

	mask1(127, 64) = 0xAB56455610A3165C;
	mask1(63, 0) = 0x3A5A7ED6521AD456;
	mask2(127, 64) = 0x1254E5CF3A45CDE1;
	mask2(63, 0) = 0xED428DD39146A134;

	plaintext = plaintext ^ mask1;
	ciphertext = aes_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

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
