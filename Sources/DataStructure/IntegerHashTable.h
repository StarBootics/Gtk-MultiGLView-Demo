/*
 * IntegerHashTable.h
 * 
 * Copyright 2022 Guillaume Saumure <gsaumure@cgocable.ca>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#ifndef INTEGER_HASH_TABLE_H
#define INTEGER_HASH_TABLE_H

#include <stdlib.h>

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef TRUE
	#define TRUE 1
#endif

typedef struct IntegerHashTable IntegerHashTable;

typedef struct IntegerHashTableBucket
{
	char* Key;
	int Value;
	struct IntegerHashTableBucket* NextBucket;
	
} IntegerHashTableBucket;  

struct IntegerHashTable
{
	size_t TableMax;
	IntegerHashTableBucket** Table;
	IntegerHashTableBucket* CurrentBucket; 
	size_t CurrentIndex;
	size_t BucketsMax;
	char* (*GetBucketKey)(IntegerHashTable*);
	int (*GetBucketValue)(IntegerHashTable*);
	void (*UpdateBucket)(IntegerHashTable*, int);
	int (*LookupBucket)(IntegerHashTable*, char*);
	void (*AddBucket)(IntegerHashTable*, char*, int);
	int (*RemoveBucket)(IntegerHashTable*, char*);
	void (*ResetBuckets)(IntegerHashTable*);
	void* (*NextBucket)(IntegerHashTable*);
	size_t (*BucketsCount)(IntegerHashTable*);
	void (*RemoveAllBuckets)(IntegerHashTable*);
	void (*Wipeout)(IntegerHashTable*);
};

void IntegerHashTable_Init(IntegerHashTable*, size_t);

#endif

