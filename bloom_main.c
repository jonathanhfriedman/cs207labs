#include "bloom.h"
#include <stdlib.h>
#include <time.h>

void test_hash() {
	// Tests the hash function for Part 1
	bloom_filter_t B;
	index_t idx = 100;
	B.size = idx;
	key_t test_values[6] = {0, 1, 2, 3, 13, 97};
	key_t k;
	for (int i=0; i<6; i++) {
		k = test_values[i];
		printf("hash1(%llu)=%llu\thash2(%llu)=%llu\n", k, hash1(&B, k), k, hash2(&B, k));
	}
}

void test_hash_initialization() {
	// Very quick and dirty test of the bloom filter implementation for Part 3
	bloom_filter_t B;
	bloom_init(&B, 1000);
	bloom_add(&B, 123);
	bloom_add(&B, 784);
	printf("size: %llu\ncount: %llu\nshould be 1: %d\nshould be 0: %d\nshould be 1: %d\n",
		B.size, B.count, bloom_check(&B, 123), bloom_check(&B, 1), bloom_check(&B, 784));
	bloom_destroy(&B);
}

void smoke_test() {
	bloom_filter_t B;
	bloom_init(&B, 1000);

	for (int i=1; i<=70; i++) {
		bloom_add(&B, i);
	}
	
	int s = 0;
	for (int i=1; i<B.size; i++) {
		if (get_bit(&B, i)) s++;
	}

	printf("%d bits set\n", s);
}

void random_array(uint64_t* arr) {
	for (int i=0; i<100; i++) {
		arr[i] = rand() % 1000000;
	}
}

void experiment(uint64_t* arr1, uint64_t* arr2) {
	bloom_filter_t B;
	bloom_init(&B, 1000);

	// Add random numbers to filter
	for (int i=0; i<100; i++) {
		bloom_add(&B, arr1[i]);
	}

	// Count number of set bits
	int s = 0;
	for (int i=1; i<B.size; i++) {
		if (get_bit(&B, i)) s++;
	}

	// See whether other random numbers are in filter
	int s2 = 0;
	for (int i=0; i<100; i++) {
		if (bloom_check(&B, arr2[i])) s2++;
	}

	printf("Bits set: %d\tNumbers from second array returning true: %d\n", s, s2);
}

int main() {
	srand(time(NULL));
	// test_hash(); // uncomment to test hash functions
	// test_hash_initialization(); // uncomment to test bloom filter implementation
	// smoke_test(); // uncomment for smoke test
	uint64_t arr1[100];
	random_array(arr1);
	uint64_t arr2[100];
	random_array(arr2);

	experiment(arr1, arr2);
	return 0;
}