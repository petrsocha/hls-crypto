#include <stdio.h>
#include "present.h"

int main () {

	present_block_t plaintext;
	present_key_t key;
	present_block_t ciphertext;

	int retVal = 0;

	plaintext = 0;
	key = 0;
	ciphertext = present_encrypt(plaintext, key);

	if(ciphertext != 0x5579C1387B228445){
		retVal++;
		std::cout << "Present encryption output mismatch (key=0, pt=0)\n";
	}

	plaintext = 0xFFFFFFFFFFFFFFFF;
	ciphertext = present_encrypt(plaintext, key);

	if(ciphertext != 0xA112FFC72F68417B){
		retVal++;
		std::cout << "Present encryption output mismatch (key=0, pt=F)\n";
	}

	key = 0xFFFFFFFFFFFFFFFF;
	key(79,64) = 0xFFFF;
	ciphertext = present_encrypt(plaintext, key);

	if(ciphertext != 0x3333DCD3213210D2){
		retVal++;
		std::cout << "Present encryption output mismatch (key=F, pt=F)\n";
	}

	plaintext = 0;
	ciphertext = present_encrypt(plaintext, key);

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
