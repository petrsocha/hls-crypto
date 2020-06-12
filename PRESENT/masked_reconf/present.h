#include <ap_int.h>

// Versions 1, 2, 3 differ in reconfiguration parallelism.
// Version 1 has smallest area and highest latency
// Version 3 has largest area and lowest latency
#define VERSION 1

#define BLOCK_WIDTH 			64
#define BLOCK_BIT_IDX_WIDTH 	6
#define KEY_WIDTH 				80
#define SBOX_WIDTH 				4
#define SBOX_DOMAIN_RANGE		16
#define ROUND_IDX_WIDTH 		5
#define SLAYER_SBOX_COUNT 		16

typedef ap_uint<BLOCK_WIDTH> 			present_block_t;
typedef ap_uint<BLOCK_BIT_IDX_WIDTH> 	present_block_bit_idx_t;
typedef ap_uint<KEY_WIDTH> 				present_key_t;
typedef ap_uint<SBOX_WIDTH> 			present_sbox_t;
typedef ap_uint<ROUND_IDX_WIDTH> 		present_round_idx_t;

static const	present_block_bit_idx_t perm[BLOCK_WIDTH] 				= {0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51, 4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55, 8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59, 12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63};
static const 	present_block_bit_idx_t permInv[BLOCK_WIDTH] 			= {0,  4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 1,  5,  9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61, 2,  6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62,  3,  7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63};
static const 	present_sbox_t 			sBoxClean[SBOX_DOMAIN_RANGE] 	= {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};
static 			present_sbox_t 			sBoxMasked[2][SLAYER_SBOX_COUNT][SBOX_DOMAIN_RANGE];

present_block_t present_encrypt(
					present_block_t 	plaintext,
					present_key_t 		key,
					present_block_t 	maskIn,
					present_block_t 	maskOut
				);
void 			reconfigure(
					present_block_t 	maskIn,
					present_block_t 	maskOut
				);
present_block_t addRoundKey(
					present_block_t 	ctx,
					present_key_t 		key
				);
present_block_t sLayer(
					present_block_t 	ctx,
					present_round_idx_t round
				);
present_block_t pLayer(
					present_block_t 	ctx
				);
present_block_t pInvLayer(
					present_block_t 	ctx
				);
present_key_t 	updateKey(
					present_key_t 		key,
					present_round_idx_t round
				);
