// ffs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdint>
#include <chrono>

using rt_int32_t = int32_t;
using rt_uint32_t = uint32_t;
using rt_uint8_t = uint8_t;

static int __rt_ffs_puny(rt_int32_t value) {
    int position = 1; // position start from 1

    if (value == 0)
    {
        return 0; // 0 means no bit 1
    }

    // search half range
    if ((value & 0xFFFF) == 0)
    {
       // is lower 16bit 0
       position += 16;
       value >>= 16;
    }
    if ((value & 0xFF) == 0)
    {
       // is lower 8bit 0
       position += 8;
       value >>= 8;
    }
    if ((value & 0xF) == 0)
    {
        // is lower 4bit 0
        position += 4;
        value >>= 4;
    }
    if ((value & 0x3) == 0)
    {
        // is lower 2bit 0
        position += 2;
        value >>= 2;
    }
    if ((value & 0x1) == 0)
    {
        // is lower 1bit 0
        position += 1;
    }

    return position;
}

const rt_uint8_t __lowest_bit_bitmap_tiny[] =
{
    /*  0 - 7  */  0,  1,  2, 27,  3, 24, 28, 32,
    /*  8 - 15 */  4, 17, 25, 31, 29, 12, 32, 14,
    /* 16 - 23 */  5,  8, 18, 32, 26, 23, 32, 16,
    /* 24 - 31 */ 30, 11, 13,  7, 32, 22, 15, 10,
    /* 32 - 36 */  6, 21,  9, 20, 19
};

/**
 * @brief This function finds the first bit set (beginning with the least significant bit)
 * in value and return the index of that bit.
 *
 * Bits are numbered starting at 1 (the least significant bit).  A return value of
 * zero from any of these functions means that the argument was zero.
 *
 * @param value is the value to find the first bit set in.
 *
 * @return return the index of the first bit set. If value is 0, then this function
 * shall return 0.
 */
static int __rt_ffs_tiny(int value)
{
    return __lowest_bit_bitmap_tiny[(rt_uint32_t)(value & (value - 1) ^ value) % 37];
}

const rt_uint8_t __lowest_bit_bitmap[] =
{
    /* 00 */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 10 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 20 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 30 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 40 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 50 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 60 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 70 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 80 */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 90 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* A0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* B0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* C0 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* D0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* E0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* F0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief This function finds the first bit set (beginning with the least significant bit)
 * in value and return the index of that bit.
 *
 * Bits are numbered starting at 1 (the least significant bit).  A return value of
 * zero from any of these functions means that the argument was zero.
 *
 * @param value is the value to find the first bit set in.
 *
 * @return Return the index of the first bit set. If value is 0, then this function
 *         shall return 0.
 */
static int __rt_ffs(int value)
{
    if (value == 0)
    {
        return 0;
    }

    if (value & 0xff)
    {
        return __lowest_bit_bitmap[value & 0xff] + 1;
    }

    if (value & 0xff00)
    {
        return __lowest_bit_bitmap[(value & 0xff00) >> 8] + 9;
    }

    if (value & 0xff0000)
    {
        return __lowest_bit_bitmap[(value & 0xff0000) >> 16] + 17;
    }

    return __lowest_bit_bitmap[(value & 0xff000000) >> 24] + 25;
}

//#if defined(__has_builtin)
//#if __has_builtin(__builtin_ctz)
//#define HAS_BUILTIN_CTZ 1
//#else
//#define HAS_BUILTIN_CTZ 0
//#endif
//#else
//// If __has_builtin is not available, assume GCC or Clang older than the feature.
//#if defined(__GNUC__) || defined(__clang__)
//#define HAS_BUILTIN_CTZ 1
//#else
//#define HAS_BUILTIN_CTZ 0
//#endif
//#endif
//
//#ifdef HAS_BUILTIN_CTZ
//int __rt_ffs_builtin(int value) {
//#if defined(__GNUC__) || defined(__clang__)
//    //if (value ^ (value & (value - 1))) { // Ensure only one bit is set
//    if (value) {
//        return 1 + __builtin_ctz(value);
//    }
//    else {
//        return 0;
//    }
//#else
//    if (value) {
//        unsigned long position;
//        _BitScanForward(&position, value);
//
//        return 1 + position;
//    }
//    else {
//        return 0;
//    }
//#endif
//}
//#endif

#if defined(_MSC_VER)
#define __HAS_BUILTIN_CTZ__
#include <intrin.h>
#define CTZ(x) ([](unsigned int val) -> int { unsigned long pos; return val ? _BitScanForward(&pos, val), 1 + pos : 0; })(x)
#elif defined(__GNUC__) || defined(__clang__)
#define __HAS_BUILTIN_CTZ__
#define CTZ(x) ((x) ? 1 + __builtin_ctz(x) : 0)

#elif defined(__ARMCC_VERSION)  // Keil ARM Compiler
#include "arm_math.h"
#define __HAS_BUILTIN_CTZ__
#define CTZ(x) ((x) ? 1 + __CLZ(__RBIT(x)) : 0)

#elif defined(__ICCARM__)     // IAR ARM Compiler
#include <intrinsics.h>
#define __HAS_BUILTIN_CTZ__
#define CTZ(x) ((x) ? 1 + __CLZ(__RBIT(x)) : 0)

#else
#message "Don't know if compiler has builtin ctz"
#endif

#ifdef __HAS_BUILTIN_CTZ__
int __rt_ffs_builtin(rt_int32_t value) {
    return CTZ(value);
}
#else
#define __rt_ffs_builtin __rt_ffs_puny
#endif


static void measure(int (*ffs)(int), const char * ffs_name)
{
    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

    // Code to measure
    int x;
    for (uint32_t i = 0; i < 0xFFFFFFFF; i++) {
        x = ffs(i);
    }

    // Record end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::printf("%s ", ffs_name);
    std::cout << "execution time: " << duration.count() << " microseconds" << std::endl;
}

static bool verify()
{
    bool result = true;

    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < 0xFFFFFFFF; i++) {
        int r1 = __rt_ffs(i);
        int r2 = __rt_ffs_tiny(i);
        int r3 = __rt_ffs_puny(i);
        int r4 = __rt_ffs_builtin(i);

        if (r1 != r2 || r2 != r3 || r3 != r4) {
            std::printf("Verification failed at %d, %d, %d, %d, %d.\n", i, r1, r2, r3, r4);
            result = false;
            break;
        }
    }

    // Record end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

#if defined(_MSC_VER)
    std::printf("Verification execution time: %lld microseconds\n", duration.count());
#else
    std::printf("Verification execution time: %ld microseconds\n", duration.count());
#endif

    return result;
}

int main()
{
    //for (auto i = 0; i < 16; i++) {
    //    unsigned long pos = 0;
    //    unsigned int r = _BitScanForward(&pos, i);
    //    std::printf("%d: %d %d %d\n", i, __rt_ffs_builtin(i), pos, r);
    //}

    if (verify()) {
        measure(__rt_ffs, "ffs");
        measure(__rt_ffs_tiny, "tiny_ffs");
        measure(__rt_ffs_puny, "puny_ffs");
        measure(__rt_ffs_builtin, "builtin_ffs");
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
