#include <stdio.h>
#include "serpent.h"

int main () {

	serpent_block_t plaintext;
	serpent_key_t key;
	serpent_block_t ciphertext;
	serpent_block_t mask1, mask2;

	int retVal = 0;

	// Test 1

	plaintext(127, 64) = 0x0000000000000000;
	plaintext(63, 0) = 0x0000000000000000;
	key(127, 64) = 0x0000000000000000;
	key(63, 0) = 0x0000000000000000;

	mask1(127, 64) = 0xff123e5d423f4648;
	mask1(63, 0) = 0x5a42a6454bcf5432;
	mask2(127, 64) = 0x4562af4564ac2312;
	mask2(63, 0) = 0xac2edf4567e56d45;

	plaintext = plaintext ^ mask1;
	ciphertext = serpent_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0x3620b17ae6a993d0 || ciphertext(63, 0) != 0x9618b8768266bae9){
		std::cout << std::hex << ciphertext << std::endl;
		retVal++;
		std::cout << "Serpent encryption output mismatch (test vector #1)\n";
	}

	// Test 2

	plaintext(127, 64) = 0xffffffffffffffff;
	plaintext(63, 0) = 0xffffffffffffffff;
	key(127, 64) = 0xffffffffffffffff;
	key(63, 0) = 0xffffffffffffffff;

	mask1(127, 64) = 0xc89a1c6745135789;
	mask1(63, 0) = 0x234657d834e4a310;
	mask2(127, 64) = 0x56423e5c45c14564;
	mask2(63, 0) = 0x123156450f645612;

	plaintext = plaintext ^ mask1;
	ciphertext = serpent_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0x2dee675b6b740136 || ciphertext(63, 0) != 0x7da2a80fb44b8065){
		std::cout << std::hex << ciphertext << std::endl;
		retVal++;
		std::cout << "Serpent encryption output mismatch (test vector #2)\n";
	}

	// Test 3

	plaintext(127, 64) = 0x0000000000000000;
	plaintext(63, 0) = 0x0000000000000000;
	key(127, 64) = 0xffffffffffffffff;
	key(63, 0) = 0xffffffffffffffff;

	mask1(127, 64) = 0x564123416854564;
	mask1(63, 0) = 0x4560456a0156d544;
	mask2(127, 64) = 0x6f560456fff15645;
	mask2(63, 0) = 0x46504e5e12e60564;

	plaintext = plaintext ^ mask1;
	ciphertext = serpent_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0x941f09284b4cd56b || ciphertext(63, 0) != 0xba20ea50cfc36476){
		std::cout << std::hex << ciphertext << std::endl;
		retVal++;
		std::cout << "Serpent encryption output mismatch (test vector #3)\n";
	}

	// Test 4

	plaintext(127, 64) = 0xffffffffffffffff;
	plaintext(63, 0) = 0xffffffffffffffff;
	key(127, 64) = 0x0000000000000000;
	key(63, 0) = 0x0000000000000000;

	mask1(127, 64) = 0xffac23c1685f564;
	mask1(63, 0) = 0x45c0456aa15adf44;
	mask2(127, 64) = 0xaa560ca6aff15645;
	mask2(63, 0) = 0x46aa4e5e12ea0564;

	plaintext = plaintext ^ mask1;
	ciphertext = serpent_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0xb24a760ec4bfb905 || ciphertext(63, 0) != 0x02961713f0896be9){
		std::cout << std::hex << ciphertext << std::endl;
		retVal++;
		std::cout << "Serpent encryption output mismatch (test vector #4)\n";
	}

	// Test 5

	plaintext(127, 64) = 0x0000000000000000;
	plaintext(63, 0) = 0x0000000000000001;
	key(127, 64) = 0x1000000000000000;
	key(63, 0) = 0x0000000000000000;

	mask1(127, 64) = 0x46aa4e5e12ea0564;
	mask1(63, 0) = 0x45c0456aa15adf44;
	mask2(127, 64) = 0xffac23c1685f564;
	mask2(63, 0) = 0x46a5465046540564;

	plaintext = plaintext ^ mask1;
	ciphertext = serpent_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0x925ef2a36af239a5 || ciphertext(63, 0) != 0xece6ab39ab4ddffb){
		std::cout << std::hex << ciphertext << std::endl;
		retVal++;
		std::cout << "Serpent encryption output mismatch (test vector #5)\n";
	}

	// Test 6

	plaintext(127, 64) = 0x0123456789abcdef;
	plaintext(63, 0) = 0x0123456789abcdef;
	key(127, 64) = 0x0011223344556677;
	key(63, 0) = 0x8899aabbccddeeff;

	mask1(127, 64) = 0xadcccdde6854564;
	mask1(63, 0) = 0x456aaa6a0156d544;
	mask2(127, 64) = 0x6fc6045aacf15645;
	mask2(63, 0) = 0x465affff12e60564;

	plaintext = plaintext ^ mask1;
	ciphertext = serpent_encrypt(plaintext, key, mask1, mask2);
	ciphertext = ciphertext ^ mask2;

	if(ciphertext(127, 64) != 0x0a82db284bdce32f || ciphertext(63, 0) != 0x1535a7fc5a66c571){
		std::cout << std::hex << ciphertext << std::endl;
		retVal++;
		std::cout << "Serpent encryption output mismatch (test vector #6)\n";
	}


	if(retVal){
		std::cout << "C Simulation failed\n";
	} else {
		std::cout << "C Simulation successful\n";
	}

	return retVal;
}
