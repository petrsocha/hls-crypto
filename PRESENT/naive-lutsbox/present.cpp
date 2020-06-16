#include "present.h"

present_block_t present_encrypt (present_block_t plaintext, present_key_t key) {

	present_block_t ciphertext = plaintext;
	present_key_t roundKey = key;

	ciphertext = addRoundKey(ciphertext, roundKey);

	for(int i = 1; i < 32; i++){
	#pragma HLS pipeline

		roundKey = updateKey(roundKey, i);
		ciphertext = sLayer(ciphertext);
		ciphertext = pLayer(ciphertext);
		ciphertext = addRoundKey(ciphertext, roundKey);

	}

	return ciphertext;

}

// XOR layer
present_block_t addRoundKey(present_block_t ctx, present_key_t roundKey){

	return ctx ^ roundKey(79, 16);

}

// Substitution layer
present_block_t sLayer(present_block_t ctx) {

	present_block_t substitutedCtx;

	// Apply 16 S-boxes
	for(int i=0; i<16; i++){
	#pragma HLS unroll

		substitutedCtx(i*4 + 3, i*4) = sBox[ctx(i*4 + 3, i*4)];

	}

	return substitutedCtx;
}

// Permutation layer
present_block_t pLayer(present_block_t ctx) {

	present_block_t permutedCtx;

	// Apply bit permutation
	for(int i=0; i<=63; i++) {
	#pragma HLS unroll

		permutedCtx[perm[i]] = ctx[i];

	}

	return permutedCtx;
}

// Round key expansion
present_key_t updateKey(present_key_t key, present_round_idx_t round) {

	present_key_t newKey;

	// Rotate 61 bits to the left
	newKey(79, 61) = key(18, 0);
	newKey(60, 0) = key(79, 19);
	// Apply S-Box to the leftmost four bits
	newKey(79, 76) = sBox[newKey(79, 76)];
	// XOR with round counter
	newKey(19, 15) = newKey(19, 15) ^ round;

	return newKey;

}
