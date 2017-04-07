#ifndef _HASH_H_
#define _HASH_H_

#include "Bucket.h"

typedef struct Hash_P{
	Bucket * FirstBucket;
	Bucket * LastBucket;
}hash_buckets;

typedef struct hash{
	int buckets;
	hash_buckets * Hash_Table;
}HT;

#endif