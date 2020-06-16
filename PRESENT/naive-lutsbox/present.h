#include <ap_int.h>

#define BLOCK_WIDTH 			64
#define BLOCK_BIT_IDX_WIDTH 	6
#define KEY_WIDTH 				80
#define SBOX_WIDTH 				4
#define SBOX_DOMAIN_RANGE		16
#define ROUND_IDX_WIDTH 		5

typedef ap_uint<BLOCK_WIDTH> present_block_t;
typedef ap_uint<BLOCK_BIT_IDX_WIDTH> present_block_bit_idx_t;
typedef ap_uint<KEY_WIDTH> present_key_t;
typedef ap_uint<SBOX_WIDTH> present_sbox_t;
typedef ap_uint<ROUND_IDX_WIDTH> present_round_idx_t;

static const present_sbox_t sBox[SBOX_DOMAIN_RANGE] = {0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2};
static const present_block_bit_idx_t perm[BLOCK_WIDTH] = {0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50, 3, 19, 35, 51, 4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23, 39, 55, 8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43, 59, 12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63};

present_block_t present_encrypt(present_block_t plaintext, present_key_t key);

present_block_t addRoundKey(present_block_t ctx, present_key_t key);
present_block_t pLayer(present_block_t ctx);
present_block_t sLayer(present_block_t ctx);
present_key_t updateKey(present_key_t key, present_round_idx_t round);
