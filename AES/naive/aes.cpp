#include <stdio.h>
#include "aes.h"

aes_block_t aes_encrypt (aes_block_t plaintext, aes_key_t key) {

#pragma HLS RESOURCE variable=sBox core=ROM_nP_LUTRAM

	aes_key_t roundKey = key;
	aes_block_t ciphertext = plaintext;

	ciphertext = addRoundKey(ciphertext, roundKey);

	for(int i = 0; i < 10; i++){
	#pragma HLS pipeline

		roundKey = updateKey(roundKey, i);
		ciphertext = subBytes(ciphertext);
		ciphertext = shiftRows(ciphertext);
		ciphertext = ((i == 9) ? ciphertext : mixColumns(ciphertext));
		ciphertext = addRoundKey(ciphertext, roundKey);

	}

	return ciphertext;

}

// XOR layer
aes_block_t addRoundKey(aes_block_t ctx, aes_key_t roundKey){

	return ctx ^ roundKey;

}

aes_block_t subBytes(aes_block_t ctx){

	aes_block_t substitutedCtx;

	// Apply 16 S-boxes
	for(int i=0; i<16; i++){
	#pragma HLS unroll

		substitutedCtx(i*8 + 7, i*8) = sBox[ctx(i*8 + 7, i*8)];

	}

	return substitutedCtx;

}

aes_block_t shiftRows(aes_block_t ctx){

	aes_block_t shiftedCtx;

	for(int i=0; i<16; i++){
	#pragma HLS unroll

		shiftedCtx(perm[i]*8 + 7, perm[i]*8) = ctx(i*8 + 7, i*8);

	}

	return shiftedCtx;

}

aes_block_t mixColumns(aes_block_t ctx){

	aes_block_t mixedColumns;

	for(int i=0; i<4; i++){
	#pragma HLS unroll

		mixedColumns(i*32 + 31, i*32) = mixColumn(ctx(i*32 + 31, i*32));

	}

	return mixedColumns;

}

aes_word_t mixColumn(aes_word_t column){

	aes_byte_t b0 = column(31, 24);
	aes_byte_t b1 = column(23, 16);
	aes_byte_t b2 = column(15, 8);
	aes_byte_t b3 = column(7, 0);

	aes_byte_t b0x2 = 0;
	aes_byte_t b1x2 = 0;
	aes_byte_t b2x2 = 0;
	aes_byte_t b3x2 = 0;

	b0x2(7, 1) = b0(6, 0);
	b1x2(7, 1) = b1(6, 0);
	b2x2(7, 1) = b2(6, 0);
	b3x2(7, 1) = b3(6, 0);

	if(b0[7]) b0x2 = b0x2 ^ 0x1B;
	if(b1[7]) b1x2 = b1x2 ^ 0x1B;
	if(b2[7]) b2x2 = b2x2 ^ 0x1B;
	if(b3[7]) b3x2 = b3x2 ^ 0x1B;

	aes_byte_t b0x3 = b0 ^ b0x2;
	aes_byte_t b1x3 = b1 ^ b1x2;
	aes_byte_t b2x3 = b2 ^ b2x2;
	aes_byte_t b3x3 = b3 ^ b3x2;

	aes_word_t mixedColumn;

	mixedColumn(31, 24) = b0x2 ^ b1x3 ^ b2 ^ b3;
	mixedColumn(23, 16) = b0 ^ b1x2 ^ b2x3 ^ b3;
	mixedColumn(15, 8) = b0 ^ b1 ^ b2x2 ^ b3x3;
	mixedColumn(7, 0) = b0x3 ^ b1 ^ b2 ^ b3x2;

	return mixedColumn;

}

aes_word_t gFunc(aes_word_t word, aes_round_idx_t round){

	aes_byte_t b0 = word(31, 24);
	aes_byte_t b1 = word(23, 16);
	aes_byte_t b2 = word(15, 8);
	aes_byte_t b3 = word(7, 0);

	aes_byte_t s0 = sBox[b1];
	aes_byte_t s1 = sBox[b2];
	aes_byte_t s2 = sBox[b3];
	aes_byte_t s3 = sBox[b0];

	s0 = s0 ^ rconst[round];

	aes_word_t retVal;
	retVal(31, 24) = s0;
	retVal(23, 16) = s1;
	retVal(15, 8) = s2;
	retVal(7, 0) = s3;

	return retVal;

}

aes_key_t updateKey(aes_key_t key, aes_round_idx_t round){

	aes_word_t w0 = key(127, 96);
	aes_word_t w1 = key(95, 64);
	aes_word_t w2 = key(63, 32);
	aes_word_t w3 = key(31, 0);

	aes_word_t n0 = w0 ^ gFunc(w3, round);
	aes_word_t n1 = w1 ^ n0;
	aes_word_t n2 = w2 ^ n1;
	aes_word_t n3 = w3 ^ n2;

	aes_key_t newKey;
	newKey(127, 96) = n0;
	newKey(95, 64) = n1;
	newKey(63, 32) = n2;
	newKey(31, 0) = n3;

	return newKey;

}
