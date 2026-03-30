/*	Routines for managing the OS/2 arena, which is the 512MB virtual memory space where OS/2 data lives.
	All addresses within this region can be trivially converted between segment:offset form and flat
	form. Memory is allocated from the OS/2 arena in 64KB (segment-sized) chunks.	*/

#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "os2arena.h"

PVOID OS2ArenaBase;
DWORD OS2ArenaBitmap[ARENA_SIZE / (HUNK_SIZE * 32)];	/* Bitmap for managing the OS/2 arena, with one bit per block.	*/

typedef DWORD uint32_t;
typedef ULONGLONG uint64_t;

struct bitarray {
	size_t nbits;
	size_t arr_size;
	uint32_t* arr;
};

struct bitarray bitmap;

/*	Initializes the OS/2 arena. Returns zero if successful.		*/
int InitOS2Arena() {
	PVOID pCommitted;
	
	/*	Reserves the 512MB block containing the OS/2 arena.	*/
	OS2ArenaBase = VirtualAlloc(NULL, ARENA_SIZE, MEM_RESERVE, PAGE_READWRITE);
	if (OS2ArenaBase == NULL) return 1;

	/*	Commits the entire OS/2 arena, except for the first 64KB (for null-pointer protection).	*/
	pCommitted = VirtualAlloc(PTR_ADD(OS2ArenaBase, 0x10000), ARENA_SIZE - 0x10000, MEM_COMMIT, PAGE_READWRITE);
	if (pCommitted == NULL) return 1;

	/*	Allocate and initialize the bitmap managing the OS/2 arena.	*/
	memset(OS2ArenaBitmap, 0, sizeof(OS2ArenaBitmap));
	bitmap.nbits = ARENA_SIZE / HUNK_SIZE;
	bitmap.arr_size = sizeof(OS2ArenaBitmap);
	bitmap.arr = OS2ArenaBitmap;

	/*	Mark the first 64KB as off-limits.	*/
	OS2Reserve(0, 1);

	return 0;
}

/*	Maps an OS/2 pointer into the host's address space.	*/
PVOID OS2Map(FLATPTR Os2Ptr) {
	return PTR_ADD(OS2ArenaBase, Os2Ptr);
}

/*	Maps a host pointer into the OS/2 arena.	*/
FLATPTR OS2Unmap(PVOID HostPtr) {
	return (FLATPTR)((FLATPTR)HostPtr - (FLATPTR)OS2ArenaBase);
}

/* Clears bits from src starting from pos */
uint32_t bit_clear(uint32_t src, size_t bits, size_t pos) {
	uint32_t mask = (((uint64_t)1 << bits) - 1) << pos;
	return src & ~mask;
}

/* Sets bits from src starting from pos */
uint32_t bit_set(uint32_t src, size_t bits, size_t pos) {
	uint32_t mask = (((uint64_t)1 << bits) - 1) << pos;
	return src | mask;
}

/* Sets nbits in the bitarray starting from pos */
void bitarray_set(struct bitarray* bitmap, size_t pos, size_t nbits) {
	if ((pos + nbits) > bitmap->nbits) nbits = bitmap->nbits - pos;

	uint32_t array_index = pos / 32;
	uint32_t bit_index = pos % 32;
	uint32_t bits_set_here = ((32 - bit_index) > nbits) ? nbits : (32 - bit_index);

	bitmap->arr[array_index] = bit_set(bitmap->arr[array_index], bits_set_here, bit_index);

	nbits -= bits_set_here;

	array_index++;

	while (nbits >= 32) {
		bitmap->arr[array_index++] = 0xFFFFFFFF;
		nbits -= 32;
	}

	if (nbits) {
		bitmap->arr[array_index] = bit_set(bitmap->arr[array_index], nbits, 0);
	}
}

/* Clears nbits in the bitarray starting from pos */
void bitarray_clear(struct bitarray* bitmap, size_t pos, size_t nbits) {
	if ((pos + nbits) > bitmap->nbits) nbits = bitmap->nbits - pos;

	uint32_t array_index = pos / 32;
	uint32_t bit_index = pos % 32;
	uint32_t bits_set_here = ((32 - bit_index) > nbits) ? nbits : (32 - bit_index);

	bitmap->arr[array_index] = bit_clear(bitmap->arr[array_index], bits_set_here, bit_index);

	nbits -= bits_set_here;

	array_index++;

	while (nbits >= 32) {
		bitmap->arr[array_index++] = 0x00000000;
		nbits -= 32;
	}

	if (nbits) {
		bitmap->arr[array_index] = bit_clear(bitmap->arr[array_index], nbits, 0);
	}
}

/* Bit-scan reverse: returns the position of the last set bit */
uint32_t bsr(uint32_t dword) {
	uint32_t index = 31;

	while (!(dword & 0x80000000) && index) {
		dword <<= 1;
		index--;
	}

	return index;
}

/* Bit scan forward: returns the position of the first set bit */
uint32_t bsf(uint32_t dword) {
	uint32_t index = 0;

	while (!(dword & 0x1) && index < 32) {
		dword >>= 1;
		index++;
	}

	return index;
}

/* Find free bits from position n */
size_t find_free_bits(struct bitarray* bitmap, size_t n) {
	uint32_t array_index = n / 32;
	uint32_t bit_index = n % 32;
	uint32_t last_set_bit = bsr(bitmap->arr[array_index]);
	uint32_t free_bits;

	if (bitmap->arr[array_index] == 0 || last_set_bit < bit_index) {
		free_bits = 32 - bit_index;
	}
	else {
		free_bits = 0;
		uint32_t test_value = bitmap->arr[array_index] >> bit_index;

		while (!(test_value & 0x1)) {
			test_value >>= 1;
			free_bits++;
		}

		return free_bits;
	}

	array_index++;

	while (bitmap->arr[array_index] == 0) {
		if (array_index == (bitmap->arr_size - 1)) {
			free_bits += (bitmap->nbits & 31) ? (bitmap->nbits & 31) : 32;
			return free_bits;
		}

		free_bits += 32;
		array_index++;
	}

	free_bits += bsf(bitmap->arr[array_index]);

	return free_bits;

}

/* Find set bits from position n */
size_t find_set_bits(struct bitarray* bitmap, size_t n) {
	uint32_t array_index = n / 32;
	uint32_t bit_index = n % 32;
	uint32_t last_set_bit = bsr(~bitmap->arr[array_index]);
	uint32_t free_bits;

	if (bitmap->arr[array_index] == 0xFFFFFFFF || last_set_bit < bit_index) {
		free_bits = 32 - bit_index;
	}
	else {
		free_bits = 0;
		uint32_t test_value = ~(bitmap->arr[array_index] >> bit_index);

		while (!(test_value & 0x1)) {
			test_value >>= 1;
			free_bits++;
		}

		return free_bits;
	}

	array_index++;

	while (bitmap->arr[array_index] == 0xFFFFFFFF) {
		if (array_index == (bitmap->arr_size - 1)) {
			free_bits += (bitmap->nbits & 31) ? (bitmap->nbits & 31) : 32;
			return free_bits;
		}

		free_bits += 32;
		array_index++;
	}

	free_bits += bsf(~bitmap->arr[array_index]);

	return free_bits;

}

/* Find first free bit starting from pos, -1 if no more free bits */
uint32_t find_first_free_bit(struct bitarray* bitmap, size_t pos) {
	// FIXME: Optimize
	return find_set_bits(bitmap, pos) + pos;
}

/* Finds the index of the first free region of sufficient size (or -1 if not found) */
uint32_t bitarray_find_free_region(struct bitarray* bitmap, BLOCK_INDEX start, size_t size) {
	uint32_t index = start;

	while (index < bitmap->nbits) {
		uint32_t free_region = find_free_bits(bitmap, index);

		if (free_region >= size) {
			return index;
		}

		index = find_first_free_bit(bitmap, index + free_region);
	}

	return -1;
}

/*	Frees a region of the OS/2 arena for future allocations.	*/
void OS2Release(FLATPTR Os2Ptr, DWORD Blocks) {
	BLOCK_INDEX StartBlock = FLATPTR_TO_BI(Os2Ptr);
	bitarray_clear(&bitmap, StartBlock, Blocks);
}

/*	Marks a region of the OS/2 arena as off-limits for further allocations.	*/
void OS2Reserve(FLATPTR Os2Ptr, DWORD Blocks) {
	BLOCK_INDEX StartBlock = FLATPTR_TO_BI(Os2Ptr);
	bitarray_set(&bitmap, StartBlock, Blocks);
}

/*	Allocates a region of virtual address space in the OS/2 arena with a preferred base address.	*/
FLATPTR OS2Alloc(FLATPTR Base, DWORD Blocks) {
	BLOCK_INDEX BaseBlock = FLATPTR_TO_BI(Base);
	BLOCK_INDEX StartBlock = bitarray_find_free_region(&bitmap, BaseBlock, Blocks);

	if (StartBlock != -1) {
		FLATPTR AllocPtr = BI_TO_FLATPTR(StartBlock);
		OS2Reserve(AllocPtr, Blocks);
		return AllocPtr;
	}

	return NULL;
}

/*	Converts a flat 0:32 near pointer to a 16:16 far pointer in the OS/2 arena.	*/
DWORD DosFlatToSel(DWORD FlatPtr) {
	WORD Offset = FlatPtr & 0xFFFF;
	WORD HighPart = FlatPtr >> 16;
	WORD Segment = (HighPart << 3) | 7;	/*	Ring 3 LDT segment	*/
	return (Segment << 16) | Offset;
}

/*	Converts a 16:16 far pointer in the OS/2 arena to a flat 0:32 near pointer.	*/
DWORD DosSelToFlat(DWORD SegPtr) {
	WORD Selector = SegPtr >> 16;
	WORD Offset = SegPtr & 0xFFFF;
	WORD HighPart = Selector >> 3;
	return (HighPart << 16) | Offset;
}

