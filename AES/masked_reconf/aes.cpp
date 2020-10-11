#include <stdio.h>
#include "aes.h"

aes_block_t aes_encrypt (aes_block_t plaintext, aes_key_t key, aes_block_t maskIn, aes_block_t maskOut) {

#pragma HLS RESOURCE variable=sBoxClean core=ROM_nP_LUTRAM

	reconfigure(maskIn, maskOut);

	aes_key_t roundKey = key;
	aes_block_t ciphertext = plaintext;

	ciphertext = addRoundKey(ciphertext, roundKey);

	for(int i = 0; i < 10; i++){
	#pragma HLS pipeline

		roundKey = updateKey(roundKey, i);
		ciphertext = subBytes(ciphertext, i);
		ciphertext = shiftRows(ciphertext);
		ciphertext = ((i == 9) ? ciphertext : mixColumns(ciphertext));
		ciphertext = addRoundKey(ciphertext, roundKey);

	}

	ciphertext = remaskOutput(ciphertext, maskIn, maskOut);

	return ciphertext;

}

aes_block_t remaskOutput(aes_block_t ciphertext, aes_block_t maskIn, aes_block_t maskOut){

	aes_block_t remask = maskOut ^ invMixColumns(maskIn);
	return ciphertext ^ remask;

}

void reconfigure(aes_block_t maskIn, aes_block_t maskOut) {

	aes_block_t mask1[2] = {maskIn, maskOut};
	aes_block_t mask2Inv[2] = {invShiftRows(invMixColumns(maskOut)), invShiftRows(invMixColumns(maskIn))};

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

#endif

			L3: for(int k = 0; k < SLAYER_SBOX_COUNT; k++){
			#pragma HLS unroll

				aes_sbox_t idx = mask1[i](8*k + 7, 8*k) ^ (aes_sbox_t) j;
				aes_sbox_t val = sBoxClean[j] ^ mask2Inv[i](8*k + 7, 8*k);
				sBoxMasked[i][k][idx] = val;

			}

		}

	}

}

// XOR layer
aes_block_t addRoundKey(aes_block_t ctx, aes_key_t roundKey){

	return ctx ^ roundKey;

}

aes_block_t subBytes(aes_block_t ctx, aes_round_idx_t round){

	aes_block_t substitutedCtx;

	// Apply 16 S-boxes
	for(int i=0; i<16; i++){
	#pragma HLS unroll

		substitutedCtx(i*8 + 7, i*8) = sBoxMasked[round%2][i][ctx(i*8 + 7, i*8)];

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

aes_block_t invShiftRows(aes_block_t ctx){

	aes_block_t shiftedCtx;

	for(int i=0; i<16; i++){
	#pragma HLS unroll

		shiftedCtx(i*8 + 7, i*8) = ctx(perm[i]*8 + 7, perm[i]*8);

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

aes_block_t invMixColumns(aes_block_t ctx){

	aes_block_t invMixedColumns;

	for(int i=0; i<4; i++){
	#pragma HLS unroll

		invMixedColumns(i*32 + 31, i*32) = invMixColumn(ctx(i*32 + 31, i*32));

	}

	return invMixedColumns;

}

aes_word_t mixColumn(aes_word_t column){

	aes_byte_t b0 = column(31, 24);
	aes_byte_t b1 = column(23, 16);
	aes_byte_t b2 = column(15, 8);
	aes_byte_t b3 = column(7, 0);

	aes_byte_t b0b1 = b0 ^ b1;
	aes_byte_t b1b2 = b1 ^ b2;
	aes_byte_t b2b3 = b2 ^ b3;
	aes_byte_t b3b0 = b3 ^ b0;

	aes_byte_t b0b1x2 = 0;
	aes_byte_t b1b2x2 = 0;
	aes_byte_t b2b3x2 = 0;
	aes_byte_t b3b0x2 = 0;

	b0b1x2(7, 1) = b0b1(6, 0);
	b1b2x2(7, 1) = b1b2(6, 0);
	b2b3x2(7, 1) = b2b3(6, 0);
	b3b0x2(7, 1) = b3b0(6, 0);

	if(b0b1[7]) b0b1x2 = b0b1x2 ^ 0x1B;
	if(b1b2[7]) b1b2x2 = b1b2x2 ^ 0x1B;
	if(b2b3[7]) b2b3x2 = b2b3x2 ^ 0x1B;
	if(b3b0[7]) b3b0x2 = b3b0x2 ^ 0x1B;

	aes_word_t mixedColumn;

	mixedColumn(31, 24) = b0b1x2 ^ b1 ^ b2 ^ b3;
	mixedColumn(23, 16) = b0 ^ b1b2x2 ^ b2 ^ b3;
	mixedColumn(15, 8) = b0 ^ b1 ^ b2b3x2 ^ b3;
	mixedColumn(7, 0) = b0 ^ b1 ^ b2 ^ b3b0x2;

	return mixedColumn;

}

aes_word_t invMixColumn(aes_word_t column){

	aes_word_t mixedColumn = mixColumn(column);

	aes_byte_t a0 = mixedColumn(31, 24);
	aes_byte_t a1 = mixedColumn(23, 16);
	aes_byte_t a2 = mixedColumn(15, 8);
	aes_byte_t a3 = mixedColumn(7, 0);

	aes_byte_t a1a3 = a1 ^ a3;
	aes_byte_t a0a2 = a0 ^ a2;

	aes_byte_t a1a3x2 = 0;
	aes_byte_t a0a2x2 = 0;

	a1a3x2(7, 1) = a1a3(6, 0);
	a0a2x2(7, 1) = a0a2(6, 0);

	if(a1a3[7]) a1a3x2 = a1a3x2 ^ 0x1B;
	if(a0a2[7]) a0a2x2 = a0a2x2 ^ 0x1B;

	aes_byte_t a1a3x4 = 0;
	aes_byte_t a0a2x4 = 0;

	a1a3x4(7, 1) = a1a3x2(6, 0);
	a0a2x4(7, 1) = a0a2x2(6, 0);

	if(a1a3x2[7]) a1a3x4 = a1a3x4 ^ 0x1B;
	if(a0a2x2[7]) a0a2x4 = a0a2x4 ^ 0x1B;

	aes_word_t invMixedColumn;

	invMixedColumn(31, 24) = a0a2x4 ^ a0;
	invMixedColumn(23, 16) = a1a3x4 ^ a1;
	invMixedColumn(15, 8) = a0a2x4 ^ a2;
	invMixedColumn(7, 0) = a1a3x4 ^ a3;

	return invMixedColumn;

}

aes_word_t gFunc(aes_word_t word, aes_round_idx_t round){

	aes_byte_t b0 = word(31, 24);
	aes_byte_t b1 = word(23, 16);
	aes_byte_t b2 = word(15, 8);
	aes_byte_t b3 = word(7, 0);

	aes_byte_t s0 = sBoxClean[b1];
	aes_byte_t s1 = sBoxClean[b2];
	aes_byte_t s2 = sBoxClean[b3];
	aes_byte_t s3 = sBoxClean[b0];

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
