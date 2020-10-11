#include <ap_int.h>

// Versions 1, 2, 3 differ in reconfiguration parallelism.
// Version 1 has smallest area and highest latency
// Version 3 has largest area and lowest latency
#define VERSION 1

#define BLOCK_WIDTH 			128
#define BLOCK_BYTE_IDX_WIDTH 	4
#define BLOCK_BYTES_WIDTH 		16
#define KEY_WIDTH 				128
#define PREKEY_WIDTH 			256
#define SBOX_WIDTH 				4
#define SBOX_DOMAIN_RANGE		16
#define ROUND_IDX_WIDTH 		6
#define ROUNDS_COUNT			32
#define SLAYER_SBOX_COUNT 		32
#define SBOX_TYPE_COUNT_LOG		3

//#define PHI 0x9e3779b9l
#define PHI 0xb979379el // little-endian representation

typedef ap_uint<8>						serpent_byte_t;
typedef ap_uint<32>						serpent_word_t;
typedef ap_uint<BLOCK_WIDTH> 			serpent_block_t;
typedef ap_uint<KEY_WIDTH> 				serpent_key_t;
typedef ap_uint<BLOCK_WIDTH> 			serpent_roundkey_t;
typedef ap_uint<PREKEY_WIDTH> 			serpent_prekey_t;
typedef ap_uint<SBOX_WIDTH> 			serpent_sbox_t;
typedef ap_uint<ROUND_IDX_WIDTH> 		serpent_round_idx_t;
typedef ap_uint<SBOX_TYPE_COUNT_LOG> 	serpent_sbox_idx_t;

static const serpent_sbox_t sBoxClean[8][SBOX_DOMAIN_RANGE] = {{ 3, 8,  15,  1, 10,  6,  5, 11, 14, 13,  4,  2,  7,  0,  9, 12},
														{15, 12,  2,  7,  9,  0,  5, 10,  1, 11, 14,  8,  6, 13,  3,  4},
														{ 8, 6,   7,  9,  3, 12, 10, 15, 13,  1, 14,  4,  0, 11,  5,  2},
														{ 0, 15, 11,  8, 12,  9,  6,  3, 13,  1,  2,  4, 10,  7,  5, 14},
														{ 1, 15,  8,  3, 12,  0, 11,  6,  2,  5,  4, 10,  9, 14,  7, 13},
														{15, 5,   2, 11,  4, 10,  9, 12,  0,  3, 14,  8, 13,  6,  7,  1},
														{ 7, 2,  12,  5,  8,  4,  6, 11, 14,  9,  1, 15, 13,  3, 10,  0},
														{ 1, 13, 15,  0, 14,  8,  2, 11,  7,  4, 12, 10,  9,  3,  5,  6}};
static serpent_sbox_t sBoxMasked[8][SLAYER_SBOX_COUNT][SBOX_DOMAIN_RANGE];

serpent_block_t serpent_encrypt(
					serpent_block_t 	plaintext,
					serpent_key_t 		key,
					serpent_block_t 	maskIn,
					serpent_block_t 	maskOut
				);
serpent_block_t remaskOutput(serpent_block_t ciphertext, serpent_block_t maskIn, serpent_block_t maskOut);
void 			reconfigure(
					serpent_block_t 	maskIn,
					serpent_block_t 	maskOut
				);
serpent_block_t initialPermutation(
					serpent_block_t 	ctx
				);
serpent_block_t finalPermutation(
					serpent_block_t 	ctx
				);
serpent_block_t substitutionLayer(
					serpent_block_t 	block,
					serpent_sbox_idx_t	idx
				);
serpent_block_t substitutionLayerKey(
					serpent_block_t 	block,
					serpent_sbox_idx_t	idx
				);
serpent_block_t linearTransformation(
					serpent_block_t 	ctx
				);
serpent_block_t invLinearTransformation(
					serpent_block_t 	ctx
				);
serpent_block_t addRoundKey(
					serpent_block_t 	ctx,
					serpent_roundkey_t	roundKey
				);
serpent_word_t rotWord(
					serpent_word_t		word,
					int		i
				);
serpent_word_t shiftWord(
					serpent_word_t		word,
					int		i
				);
serpent_prekey_t updatePrekey(
					serpent_prekey_t	prekey,
					serpent_round_idx_t i
				);
serpent_roundkey_t getRoundKey(
					serpent_prekey_t	prekey,
					serpent_round_idx_t i
				);
serpent_word_t swapEndianity (
					serpent_word_t word
				);
serpent_word_t reverseWord (
					serpent_word_t word
				);
serpent_block_t swapReverse (
					serpent_block_t block
				);
serpent_block_t reverseSwap (
					serpent_block_t block
				);
