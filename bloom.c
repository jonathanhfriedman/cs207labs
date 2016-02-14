#include "bloom.h"
#include <math.h>

index_t hash1(bloom_filter_t *B, key_t k) {
	// Adapted from https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
	// The above source adapted this algorithm from Cormen et al. 1990
	// A is a "random-looking real number"
	double A = 0.5 * (sqrt(5) - 1);
	uint64_t s = floor(A * pow(2, 64));
	// Add 3 to make zero not always map to zero
	uint64_t x = (k + 3) * s;
	// Extract the log_2(size) most significant bits from x
	x = x >> (int) (64 - floor(log2(B->size)));
	return (x % B->size);
}

index_t hash2(bloom_filter_t *B, key_t k) {
	// Adapted from http://stackoverflow.com/a/12996028
	k++; // solves k=0 case
	k = ((k >> 32) ^ k) * 0x45d9f3b;
    k = ((k >> 32) ^ k) * 0x45d9f3b;
    k = ((k >> 32) ^ k);
    return (k % B->size);
}

void set_bit(bloom_filter_t *B, index_t i) {
	index_t mask = pow(2, i % 64);
	B->table[i >> 6] |= mask;
}

index_t get_bit(bloom_filter_t *B, index_t i) {
	index_t idx = B->table[i >> 6];
	index_t mask = pow(2, i % 64);
	return (idx & mask) >> (i % 64);
}

void bloom_init(bloom_filter_t *B, index_t size_in_bits) {
	B->size = size_in_bits;
	B->count = 0;
	// If size_in_bits isn't exactly divisible by sizeof(index_t), need to add one more index_t
	// For example, size = 129 bits means we need three index_t's to store all of it
	int add_one = (size_in_bits % sizeof(index_t) == 0) ? 0 : 1;
	B->table = (index_t *) malloc(((size_in_bits / sizeof(index_t))
	 + add_one) * sizeof(index_t));
	for (int i=0; i<size_in_bits; i++) {
		B->table[i] = 0;
	}
}
void bloom_destroy(bloom_filter_t *B) {
	free(B->table);
}

int bloom_check(bloom_filter_t *B, key_t k) {
	for (int i=0; i<N_HASHES; i++) {
		index_t idx = (hash1(B, k) + i * hash2(B, k)) % B->size;
		if (!get_bit(B, idx)) return 0;
	}
	return 1;
}
void bloom_add(bloom_filter_t *B, key_t k) {
	for (int i=0; i<N_HASHES; i++) {
		index_t idx = (hash1(B, k) + i * hash2(B, k)) % B->size;
		set_bit(B, idx);
	}
	B->count++;
}