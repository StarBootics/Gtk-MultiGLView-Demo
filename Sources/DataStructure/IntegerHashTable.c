/*
 * IntegerHashTable.c
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
 
#include <stdlib.h>
#include <string.h>

#include "IntegerHashTable.h"

size_t IntegerHashTable_HashFunction(IntegerHashTable* This, char* Key)
{
	size_t Len = strlen(Key);
	size_t HashValue = 0;
	
	for(size_t CharID = 0; CharID < Len; CharID++)
	{
		HashValue = ((HashValue << 6) ^ (HashValue >> 2) ^ Key[CharID]) % This->TableMax;
	}
	
	return HashValue;
}

char* IntegerHashTable_GetBucketKey(IntegerHashTable* This)
{
	if (This->CurrentBucket != NULL)
	{
		return This->CurrentBucket->Key;
	}
	
	return NULL;
}

int IntegerHashTable_GetBucketValue(IntegerHashTable* This)
{
	if (This->CurrentBucket != NULL)
	{
		return This->CurrentBucket->Value;
	}
	
	return 0;
}

void IntegerHashTable_UpdateBucket(IntegerHashTable* This, int Value)
{
	if (This->CurrentBucket != NULL)
	{
		This->CurrentBucket->Value = Value;
	}
}

int IntegerHashTable_LookupBucket(IntegerHashTable* This, char* Key)
{
	size_t Index = IntegerHashTable_HashFunction(This, Key);
	IntegerHashTableBucket* Cursor = This->Table[Index];
	
	while (Cursor != NULL)
	{
		if (strcmp(Key, Cursor->Key) == 0)
		{
			This->CurrentBucket = Cursor;
			return TRUE;
		}
		
		Cursor = Cursor->NextBucket;
	}

	return FALSE;
}

void IntegerHashTable_AddBucket(IntegerHashTable* This, char* Key, int Value)
{
	if (IntegerHashTable_LookupBucket(This, Key) == FALSE)
	{
		IntegerHashTableBucket* NewBucket = calloc(1, sizeof(IntegerHashTableBucket));
		NewBucket->Key = strdup(Key);
		NewBucket->Value = Value;
		size_t Index = IntegerHashTable_HashFunction(This, Key);
		IntegerHashTableBucket* Cursor = This->Table[Index];
		NewBucket->NextBucket = Cursor;
		This->Table[Index] = NewBucket;
		This->CurrentBucket = NewBucket;
		This->BucketsMax++;
	}
	else
	{
		IntegerHashTable_UpdateBucket(This, Value);
	}
	
}

int IntegerHashTable_RemoveBucket(IntegerHashTable* This, char* Key)
{
	This->CurrentBucket = NULL;
	size_t Index = IntegerHashTable_HashFunction(This, Key);
	IntegerHashTableBucket* Cursor = This->Table[Index];
	IntegerHashTableBucket* Previous = NULL; 
	IntegerHashTableBucket* Head = NULL; 
	
	while (Cursor != NULL && strcmp(Key, Cursor->Key) != 0)
	{
		Previous = Cursor;
		Cursor = Cursor->NextBucket;
	}
	
	if (Cursor == NULL)
	{
		return FALSE;
	}
	
	if (Previous == NULL)
	{

		Head = This->Table[Index];
		This->Table[Index] = Cursor->NextBucket;
		free(Head->Key);
		free(Head);
		This->BucketsMax--;
	}
	else
	{
		Previous->NextBucket = Cursor->NextBucket;
		free(Cursor->Key);	
		free(Cursor);
		This->BucketsMax--;
	}
	
	return TRUE;
}

void IntegerHashTable_ResetBuckets(IntegerHashTable* This)
{
	This->CurrentBucket = NULL;
	This->CurrentIndex = (size_t)-1;
}

void* IntegerHashTable_NextBucket(IntegerHashTable* This)
{
	IntegerHashTableBucket* Cursor = NULL;
	
	if (This->CurrentBucket == NULL && This->CurrentIndex == (size_t)-1)
	{
		This->CurrentIndex = 0;
		
		for (size_t Index = This->CurrentIndex; Index < This->TableMax; Index++)
		{
			Cursor = This->Table[Index];
			
			if (Cursor != NULL)
			{
				This->CurrentIndex = Index;
				This->CurrentBucket = Cursor;
				
				return This->CurrentBucket;
			}
		}
		
	}
	else
	{
		if (This->CurrentBucket->NextBucket != NULL)
		{
			This->CurrentBucket = This->CurrentBucket->NextBucket;
			return This->CurrentBucket;
		}
		else
		{
			This->CurrentIndex++;
			
			while (This->CurrentIndex < This->TableMax)
			{
				Cursor = This->Table[This->CurrentIndex];
				
				if (Cursor != NULL)
				{
					This->CurrentBucket = Cursor;
					return This->CurrentBucket;
				}
				
				This->CurrentIndex++;
			}
		}
	}
	
	return NULL;
}

size_t IntegerHashTable_BucketsCount(IntegerHashTable* This)
{
	return This->BucketsMax;
}

void IntegerHashTable_RemoveAllBuckets(IntegerHashTable* This)
{
	IntegerHashTableBucket* Cursor = NULL;
	IntegerHashTableBucket* Temp = NULL;
	
	for (size_t Index = 0; Index < This->TableMax; Index ++)
	{
		Cursor = This->Table[Index];
		
		if (Cursor != NULL)
		{
			while (Cursor != NULL)
			{
			
				free(Cursor->Key);	
				Temp = Cursor;
				Cursor = Cursor->NextBucket;
				free(Temp);
				
			}
		}
		
		This->Table[Index] = NULL;
	}
}

void IntegerHashTable_Wipeout(IntegerHashTable* This)
{
	IntegerHashTable_RemoveAllBuckets(This);
	free(This->Table);
}

void IntegerHashTable_Init(IntegerHashTable* This, size_t TableMax)
{
	This->GetBucketKey = IntegerHashTable_GetBucketKey;
	This->GetBucketValue = IntegerHashTable_GetBucketValue;
    This->UpdateBucket = IntegerHashTable_UpdateBucket;
    This->LookupBucket = IntegerHashTable_LookupBucket;
    This->AddBucket = IntegerHashTable_AddBucket;
    This->RemoveBucket = IntegerHashTable_RemoveBucket;
	This->ResetBuckets = IntegerHashTable_ResetBuckets;
	This->NextBucket = IntegerHashTable_NextBucket;
	This->BucketsCount = IntegerHashTable_BucketsCount;
	This->RemoveAllBuckets = IntegerHashTable_RemoveAllBuckets;
	This->Wipeout = IntegerHashTable_Wipeout;
	
	if (TableMax < 10)
	{
		TableMax = 10;
	}
	
	This->TableMax = TableMax;
	This->BucketsMax = 0;
	This->CurrentIndex = (size_t)-1;
	
	This->Table = calloc(This->TableMax, sizeof(IntegerHashTableBucket*));
	
	if (This->Table == NULL)
	{
		exit(EXIT_FAILURE);
	}
	
}

