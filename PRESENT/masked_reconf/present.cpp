#include "present.h"

present_block_t present_encrypt (present_block_t plaintext, present_key_t key, present_block_t maskIn, present_block_t maskOut) {

	present_block_t ciphertext = plaintext;
	present_key_t roundKey = key;

	reconfigure(maskIn, maskOut);

	ciphertext = addRoundKey(ciphertext, roundKey);     

	ENCRL: for(int i = 1; i < 32; i++){
	#pragma HLS pipeline

		roundKey = updateKey(roundKey, i);
		ciphertext = sLayer(ciphertext, i);
		ciphertext = pLayer(ciphertext);
		ciphertext = addRoundKey(ciphertext, roundKey); 
                
	}

	return ciphertext;

}

void reconfigure(present_block_t maskIn, present_block_t maskOut) {


	present_block_t mask1[2] = {maskOut, maskIn};
	present_block_t mask2InvP[2] = {pInvLayer(maskIn), pInvLayer(maskOut)};

#pragma HLS RESOURCE variable=sBoxMasked core=RAM_1P_LUTRAM

#if VERSION==1

#pragma HLS ARRAY_PARTITION variable=sBoxMasked complete dim=2

	L1: for(int i = 0; i < 2; i++){
	#pragma HLS unroll

		L2: for(int j = 0; j < SBOX_DOMAIN_RANGE; j++){
		#pragma HLS pipeline

#elif VERSION==2

#pragma HLS ARRAY_PARTITION variable=sBoxMasked complete dim=2
#pragma HLS ARRAY_PARTITION variable=sBoxMasked complete dim=1

	L2: for(int j = 0; j < SBOX_DOMAIN_RANGE; j++){
	#pragma HLS pipeline

		L1: for(int i = 0; i < 2; i++){
		#pragma HLS unroll

#elif VERSION==3

#pragma HLS ARRAY_PARTITION variable=sBoxMasked complete dim=2
#pragma HLS ARRAY_PARTITION variable=sBoxMasked complete dim=1

	L1: for(int i = 0; i < 2; i++){
	#pragma HLS unroll

		L2: for(int j = 0; j < SBOX_DOMAIN_RANGE; j++){
		#pragma HLS unroll

#endif

			L3: for(int k = 0; k < SLAYER_SBOX_COUNT; k++){
			#pragma HLS unroll

				present_sbox_t idx = mask1[i](4*k + 3, 4*k) ^ (present_sbox_t) j;
				present_sbox_t val = sBoxClean[j] ^ mask2InvP[i](4*k + 3, 4*k);
				sBoxMasked[i][k][idx] = val;

			}

		}

	}

}

// XOR layer
present_block_t addRoundKey(present_block_t ctx, present_key_t roundKey){

	return ctx ^ roundKey(79, 16);

}

// Substitution layer
present_block_t sLayer(present_block_t ctx, present_round_idx_t round) {

	present_block_t substitutedCtx;

	// Apply 16 S-boxes
	SLAYERL: for(int i=0; i<16; i++){
	#pragma HLS unroll

			substitutedCtx(i*4 + 3, i*4) = sBoxMasked[round%2][i][ctx(i*4 + 3, i*4)];

	}

	return substitutedCtx;
}

// Permutation layer
present_block_t pLayer(present_block_t ctx) {

	present_block_t permutedCtx;

	// Apply bit permutation
	PLAYERL: for(int i=0; i<=63; i++) {
	#pragma HLS unroll

		permutedCtx[perm[i]] = ctx[i];

	}

	return permutedCtx;
}

// Inverse Permutation layer
present_block_t pInvLayer(present_block_t ctx) {

	present_block_t permutedCtx;

	// Apply bit permutation
	PINVLAYERL: for(int i=0; i<=63; i++) {
	#pragma HLS unroll

		permutedCtx[permInv[i]] = ctx[i];

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
	newKey(79, 76) = sBoxClean[newKey(79, 76)];
	// XOR with round counter
	newKey(19, 15) = newKey(19, 15) ^ round;

	return newKey;

}
