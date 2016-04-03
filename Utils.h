/* 
 * File:   Utils.h
 * Author: tuannguyen
 *
 * Created on 17 August 2015, 9:40 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <cstdlib>
#include <string>
#include <string.h>

static int blockHashing(const char * key, int length, int *saved)
{
    static const int c1 = 0xcc9e2d51;
    static const int c2 = 0x1b873593;
    static const int r1 = 15;
    static const int r2 = 13;
    static const int m = 5;
    static const int n = 0xe6546b64;

    int hash = 1;

    const int nblocks = length / 4;
    const int *blocks = (const int *) key;
    for (int i = 0; i < nblocks; i++) 
    {
	int k = blocks[i];
	k *= c1;
	k = (k << r1) | (k >> (32 - r1));
	k *= c2;
        //update block's hash as well as file's hash 
	hash ^= k;
	hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
        if (saved != NULL)
        {
            *saved ^= k;
            *saved = ((*saved << r2) | (*saved >> (32 - r2))) * m + n;
        }
    }

    const char *tail = (const char *) (key + nblocks * 4);
    int k1 = 0;

    switch (length & 3) {
	case 3:
		k1 ^= tail[2] << 16;
	case 2:
		k1 ^= tail[1] << 8;
	case 1:
		k1 ^= tail[0];

		k1 *= c1;
		k1 = (k1 << r1) | (k1 >> (32 - r1));
		k1 *= c2;
		hash ^= k1;
	}

    hash ^= length;
    hash ^= (hash >> 16);	
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);
    return hash;
}

#endif