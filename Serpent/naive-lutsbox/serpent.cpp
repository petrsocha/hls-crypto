#include "serpent.h"

serpent_block_t serpent_encrypt (serpent_block_t plaintext, serpent_key_t key) {

#pragma HLS RESOURCE variable=sBox core=ROM_nP_LUTRAM

	serpent_block_t ciphertext = plaintext;

	serpent_prekey_t prekey = 0;
	prekey(255, 128) = key;
	prekey[120] = 1; // pad the 128-bit key (little-endian!)

	serpent_key_t roundKey;

	for(int i = 0; i < 31; i++){
	#pragma HLS pipeline

		prekey = updatePrekey(prekey, i);
		roundKey = getRoundKey(prekey, i);
		ciphertext = addRoundKey(ciphertext, roundKey);
		ciphertext = substitutionLayer(ciphertext, i%8);
		ciphertext = linearTransformation(ciphertext);

	}

	// Last round
	prekey = updatePrekey(prekey, 31);
	roundKey = getRoundKey(prekey, 31);
	ciphertext = addRoundKey(ciphertext, roundKey);
	ciphertext = substitutionLayer(ciphertext, 7);

	prekey = updatePrekey(prekey, 32);
	roundKey = getRoundKey(prekey, 0);
	ciphertext = addRoundKey(ciphertext, roundKey);

	return ciphertext;

}

serpent_block_t addRoundKey(serpent_block_t ctx, serpent_roundkey_t	roundKey){

	return ctx ^ roundKey;

}

serpent_roundkey_t getRoundKey(serpent_prekey_t prekey, serpent_round_idx_t i){

	serpent_key_t roundKey;
	int k = (3 - i)%8;

	roundKey = substitutionLayer(prekey(127,0), k);

	return roundKey;

}

serpent_prekey_t updatePrekey(serpent_prekey_t prekey, serpent_round_idx_t i){

	serpent_word_t wn8 = prekey(255, 224);
	serpent_word_t wn7 = prekey(223, 192);
	serpent_word_t wn6 = prekey(191, 160);
	serpent_word_t wn5 = prekey(159, 128);
	serpent_word_t wn4 = prekey(127, 96);
	serpent_word_t wn3 = prekey(95, 64);
	serpent_word_t wn2 = prekey(63, 32);
	serpent_word_t wn1 = prekey(31, 0);

	serpent_word_t k = ((serpent_word_t)i) << 2; // i*4

	serpent_word_t w0 = wn8 ^ wn5 ^ wn3 ^ wn1 ^ PHI ^ swapEndianity(k);
	w0 = rotWord(w0, 11);
	serpent_word_t w1 = wn7 ^ wn4 ^ wn2 ^ w0 ^ PHI ^ swapEndianity(k+1);
	w1 = rotWord(w1, 11);
	serpent_word_t w2 = wn6 ^ wn3 ^ wn1 ^ w1 ^ PHI ^ swapEndianity(k+2);
	w2 = rotWord(w2, 11);
	serpent_word_t w3 = wn5 ^ wn2 ^ w0 ^ w2 ^ PHI ^ swapEndianity(k+3);
	w3 = rotWord(w3, 11);

	serpent_prekey_t newPrekey;
	newPrekey(255, 224) = wn4;
	newPrekey(223, 192) = wn3;
	newPrekey(191, 160) = wn2;
	newPrekey(159, 128) = wn1;
	newPrekey(127, 96) = w0;
	newPrekey(95, 64) = w1;
	newPrekey(63, 32) = w2;
	newPrekey(31, 0) = w3;

	return newPrekey;

}

serpent_block_t substitutionLayer(serpent_block_t block, serpent_sbox_idx_t idx){

	serpent_block_t permutedBlock = initialPermutation(block);
	serpent_block_t substituedBlock;

	for(int i = 0; i < 32; i++){
	#pragma HLS unroll

		substituedBlock(i*4 + 3, i*4) = sBox[idx][permutedBlock(i*4 + 3, i*4)];

	}

	return finalPermutation(substituedBlock);

}

serpent_block_t linearTransformation(serpent_block_t ctx){

	serpent_word_t x0 = ctx(127, 96);
	serpent_word_t x1 = ctx(95, 64);
	serpent_word_t x2 = ctx(63, 32);
	serpent_word_t x3 = ctx(31, 0);

	x0 = rotWord(x0, 13);
	x2 = rotWord(x2, 3);
	x1 = x1 ^ x0 ^ x2;
	x3 = x3 ^ x2 ^ shiftWord(x0, 3);
	x1 = rotWord(x1, 1);
	x3 = rotWord(x3, 7);
	x0 = x0 ^ x1 ^ x3;
	x2 = x2 ^ x3 ^ shiftWord(x1, 7);
	x0 = rotWord(x0, 5);
	x2 = rotWord(x2, 22);

	serpent_block_t transformedCtx;
	transformedCtx(127, 96) = x0;
	transformedCtx(95, 64) = x1;
	transformedCtx(63, 32) = x2;
	transformedCtx(31, 0) = x3;

	return transformedCtx;

}

serpent_block_t initialPermutation(serpent_block_t ctx){

	serpent_block_t ctxSR = swapReverse(ctx);
	serpent_block_t permutedCtx;

	for(int i = 0; i < 32; i++){
	#pragma HLS unroll

		permutedCtx[(4 * i + 0)] = ctxSR[(i + 0)];
		permutedCtx[(4 * i + 1)] = ctxSR[(i + 32)];
		permutedCtx[(4 * i + 2)] = ctxSR[(i + 64)];
		permutedCtx[(4 * i + 3)] = ctxSR[(i + 96)];

	}

	return reverseSwap(permutedCtx);

}

serpent_block_t finalPermutation(serpent_block_t ctx){

	serpent_block_t ctxSR = swapReverse(ctx);
	serpent_block_t permutedCtx;

	for(int i = 0; i < 32; i++){
	#pragma HLS unroll

		permutedCtx[(i + 0)] = ctxSR[(4 * i + 0)];
		permutedCtx[(i + 32)] = ctxSR[(4 * i + 1)];
		permutedCtx[(i + 64)] = ctxSR[(4 * i + 2)];
		permutedCtx[(i + 96)] = ctxSR[(4 * i + 3)];

	}

	return reverseSwap(permutedCtx);

}

serpent_word_t rotWord(serpent_word_t word, int i){

	serpent_word_t rotatedWord;
	serpent_word_t wordBE = swapEndianity(word);

	rotatedWord = ((wordBE << i) | (wordBE >> (32 - i)));

	return swapEndianity(rotatedWord);

}

serpent_word_t shiftWord(serpent_word_t word, int i){

	serpent_word_t shiftedWord = 0;
	serpent_word_t wordBE = swapEndianity(word);

	shiftedWord = wordBE << i;

	return swapEndianity(shiftedWord);

}

serpent_word_t swapEndianity ( serpent_word_t word ){

	serpent_word_t wordSw;

	wordSw(7, 0) = word(31, 24);
	wordSw(15, 8) = word(23, 16);
	wordSw(23, 16) = word(15, 8);
	wordSw(31, 24) = word(7, 0);

	return wordSw;
}

serpent_word_t reverseWord ( serpent_word_t word ){

	serpent_word_t reversedWord;

	for(int i = 0; i < 32; i++){
	#pragma HLS unroll

		reversedWord[31-i] = word[i];

	}

	return reversedWord;

}

serpent_block_t swapReverse ( serpent_block_t block ){

	serpent_block_t ret;

	ret(127, 96) = reverseWord(swapEndianity(block(127, 96)));
	ret(95, 64) = reverseWord(swapEndianity(block(95, 64)));
	ret(63, 32) = reverseWord(swapEndianity(block(63, 32)));
	ret(31, 0) = reverseWord(swapEndianity(block(31, 0)));

	return ret;

}

serpent_block_t reverseSwap ( serpent_block_t block ){

	serpent_block_t ret;

	ret(127, 96) = swapEndianity(reverseWord(block(127, 96)));
	ret(95, 64) = swapEndianity(reverseWord(block(95, 64)));
	ret(63, 32) = swapEndianity(reverseWord(block(63, 32)));
	ret(31, 0) = swapEndianity(reverseWord(block(31, 0)));

	return ret;

}
