#include <stdio.h>
#include "present.h"

int main () {

	present_block_t plaintext;
	present_key_t key;
	present_block_t ciphertext;
	present_block_t mask1 = 0x123456789abcdef5;
	present_block_t mask2 = 0xabcdef0123456789;

	int retVal = 0;

	plaintext = 0;
	key = 0;
	plaintext = plaintext ^ mask1;
	ciphertext = present_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext != 0x5579C1387B228445){
		retVal++;
		std::cout << "Present encryption output mismatch (key=0, pt=0)\n";
	}

	mask1 = 0xcdab75584ef321cf;
	mask2 = 0xa45c3b45678ac312;

	plaintext = 0xFFFFFFFFFFFFFFFF;
	plaintext = plaintext ^ mask1;
	ciphertext = present_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext != 0xA112FFC72F68417B){
		retVal++;
		std::cout << "Present encryption output mismatch (key=0, pt=F)\n";
	}

	mask1 = 0x456879456cbd12a8;
	mask2 = 0x31456c123b456d78;

	plaintext = 0xFFFFFFFFFFFFFFFF;
	key = 0xFFFFFFFFFFFFFFFF;
	key(79,64) = 0xFFFF;
	plaintext = plaintext ^ mask1;
	ciphertext = present_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext != 0x3333DCD3213210D2){
		retVal++;
		std::cout << "Present encryption output mismatch (key=F, pt=F)\n";
	}

	mask1 = 0x7a45c645b678d9e1;
	mask2 = 0x12a45c6b5d6e78f6;

	plaintext = 0;
	plaintext = plaintext ^ mask1;
	ciphertext = present_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext != 0xE72C46C0F5945049){
		retVal++;
		std::cout << "Present encryption output mismatch (key=F, pt=0)\n";
	}

	if(retVal){
		std::cout << "C Simulation failed\n";
	} else {
		std::cout << "C Simulation successful\n";
	}

	return retVal;
}
