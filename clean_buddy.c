#include "mm.h"

#include <stdlib.h>
// #include "test_mm.h"
#include "syscall.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define MEM_SIZE 128      // 0x1000     // 0x10000000 = 256MB
#define MEM_SIZE 0x400    // = 1024
#define MIN_BLOCK_SIZE 16 // 0x10 // 4KB
#define TREE_BIT_MAP_SIZE (((MEM_SIZE / MIN_BLOCK_SIZE) / 8) * 2)

#define SET_BIT(bitmap, i) (bitmap[i / 8] |= (1 << (i % 8)))    // todo check
#define CLEAR_BIT(bitmap, i) (bitmap[i / 8] &= ~(1 << (i % 8))) // todo check
#define GET_BIT(bitmap, i) (bitmap[i / 8] & (1 << (i % 8)))     // todo check

#define GET_SIBLING(i) (i % 2 ? i + 1 : i - 1)
#define GET_PARENT(i) ((i - 1) / 2) // todo check

int next_power_of_2(int n);
int get_index_level(int index);
int get_size_level(int size);
int get_block_from_index(int index);

char tree_bitmap[TREE_BIT_MAP_SIZE]; // 2 bits por bloque = 256MB / 4KB * 2 = 128K bits = 16KB

// char * tree_bitmap;
void *start;

void mem_init(void *ptr, int size)
{
    start = ptr;
    memset(tree_bitmap, 0, TREE_BIT_MAP_SIZE);
    // printf("tree_bitmap: %p\n", tree_bitmap);
    // malloc?
}

void *my_alloc(int index, int level)
{
    if (level == 0)
    {
        if (GET_BIT(tree_bitmap, index))
        {
            return NULL;
        }
        SET_BIT(tree_bitmap, index);
        return start + get_block_from_index(index) * MIN_BLOCK_SIZE; // todo -> MAL -> debería ser start + get_block_from_index(index) * MIN_BLOCK_SIZE
    }

    if (GET_BIT(tree_bitmap, index) && !GET_BIT(tree_bitmap, index * 2 + 1) && !GET_BIT(tree_bitmap, index * 2 + 2))
    {
        return NULL;
    }
    // esto puesto que si el bloque está ocupado, pero sus hijos están libres, significa que el bloque no está dividido, sino que está ocupado

    void *ptr = my_alloc(index * 2 + 1, level - 1);
    if (ptr == NULL)
    {
        ptr = my_alloc(index * 2 + 2, level - 1);
        if (ptr == NULL)
        {
            return NULL;
        }
    }
    SET_BIT(tree_bitmap, index); // lo marco como ocupado
    return ptr;
}

void *my_malloc(int size)
{
    int real_size = next_power_of_2(size);
    int level = get_size_level(real_size);

    return my_alloc(0, level);
}

void my_free_idx(int index)
{
    if (index == 0)
    {
        CLEAR_BIT(tree_bitmap, index);
        return;
    }

    CLEAR_BIT(tree_bitmap, index); // lo marco como libre

    if (GET_BIT(tree_bitmap, GET_SIBLING(index))) // si el "buddy" está ocupado
    {
        return;
    }
    my_free_idx(GET_PARENT(index));
}

void my_free(void *ptr)
{
    if ((ptr - start) % MIN_BLOCK_SIZE != 0)
    {
        return;
    }

    int index = ((ptr - start) / MIN_BLOCK_SIZE) + MEM_SIZE / MIN_BLOCK_SIZE - 1; // arrancamos por el bloque de granularidad maxima
    my_free_idx(index);
}

char memory[MEM_SIZE];

void test_my_alloc()
{
    mem_init(memory, MEM_SIZE);
    /*
    for (int i = 0; i < 64 * 2 - 1; i++)
    {
        int n = get_block_from_index(i);
        printf("%d: %d\n", i, n);
    }
    */

    // printf("%d\n", get_block_from_index(2));
    // printf("%d\n", 1 << 3);

    printf("memory: %p\n", memory);
    void *punterito = my_malloc(MEM_SIZE / 2);
    void *punterito2 = my_malloc(MEM_SIZE / 4);
    // void *punterito3 = my_malloc(MEM_SIZE / 4);
    printf("punterito: %p\n", punterito);
    printf("punterito + MEM_SIZE/2: %p\n", punterito + MEM_SIZE / 2);
    printf("punterito2: %p\n", punterito2);
    for (int i = 0; i < 10; i++)
    {
        printf("bit %d: %d\n", i, GET_BIT(tree_bitmap, i));
    }
    /*
    my_free(punterito);
    for (int i = 0; i < 10; i++)
    {
        printf("bit %d: %d\n", i, GET_BIT(tree_bitmap, i));
    }
    */
}

int main()
{
    test_my_alloc();
    return 0;
}

int next_power_of_2(int n)
{
    int count = 0;
    if (n && !(n & (n - 1)))
        return n;
    while (n != 0)
    {
        n >>= 1;
        count += 1;
    }
    return 1 << count;
}

int get_index_level(int index)
{
    int level = 0;
    for (int acum = 0; index > acum; level++)
    {
        acum += 2 << level;
    }
    return level;
}

int get_size_level(int size)
{
    int level = 0;
    for (; 2 * size - 1 < MEM_SIZE; level++)
    {
        size *= 2;
    }
    return level;
}

int get_block_from_index(int index)
{
    int level = get_index_level(index);
    return (index + 1 - (1 << level)) * ((MEM_SIZE / MIN_BLOCK_SIZE) / (1 << level));
}
