/**
 * @file serialization.hpp
 * @brief code borrowed from http://beej.us/guide/bgnet/html/#serialization
 */
#pragma once

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#define pack754_32(value) (pack754((value), 32, 8))
#define pack754_64(value) (pack754((value), 64, 11))
#define unpack754_32(value) (unpack754((value), 32, 8))
#define unpack754_64(value) (unpack754((value), 64, 11))

uint32_t pack754(float value, unsigned bits, unsigned exp_bits);

float unpack754(uint32_t value, unsigned bits, unsigned exp_bits);

void packu32(unsigned char *buf, uint32_t i);

uint32_t unpacku32(unsigned char *buf);

void packf32(unsigned char *buf, float f);

float unpackf32(unsigned char *buf);
