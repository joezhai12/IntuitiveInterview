/**
 * @file serialization.cpp
 * @brief code borrowed from http://beej.us/guide/bgnet/html/#serialization
 */
#include "serialization/serialization.hpp"

uint32_t pack754(float value, unsigned bits, unsigned exp_bits)
{
	float f_norm;
	int shift;
	long long sign, exp, significand;
	unsigned significand_bits = bits - exp_bits - 1; // -1 for sign bit

	if (value == 0.0) return 0; // get this special case out of the way

	// check sign and begin normalization
	if (value < 0) { sign = 1; f_norm = -value; }
	else { sign = 0; f_norm = value; }

	// get the normalized form of value and track the exponent
	shift = 0;
	while(f_norm >= 2.0) { f_norm /= 2.0; shift++; }
	while(f_norm < 1.0) { f_norm *= 2.0; shift--; }
	f_norm = f_norm - 1.0;

	// calculate the binary form (non-float) of the significand data
	significand = f_norm * ((1LL<<significand_bits) + 0.5f);

	// get the biased exponent
	exp = shift + ((1<<(exp_bits-1)) - 1); // shift + bias

	// return the final answer
	return (sign<<(bits-1)) | (exp<<(bits-exp_bits-1)) | significand;
}

float unpack754(uint32_t value, unsigned bits, unsigned exp_bits)
{
	float result;
	long long shift;
	unsigned bias;
	unsigned significand_bits = bits - exp_bits - 1; // -1 for sign bit

	if (value == 0) return 0.0;

	// pull the significand
	result = (value&((1LL<<significand_bits)-1)); // mask
	result /= (1LL<<significand_bits); // convert back to float
	result += 1.0f; // add the one back on

	// deal with the exponent
	bias = (1<<(exp_bits-1)) - 1;
	shift = ((value>>significand_bits)&((1LL<<exp_bits)-1)) - bias;
	while(shift > 0) { result *= 2.0; shift--; }
	while(shift < 0) { result /= 2.0; shift++; }

	// sign it
	result *= (value>>(bits-1))&1? -1.0: 1.0;

	return result;
}

void packu32(unsigned char *buf, uint32_t i)
{
	*buf++ = i>>24; *buf++ = i>>16;
	*buf++ = i>>8;  *buf++ = i;
}

uint32_t unpacku32(unsigned char *buf)
{
	return ((uint32_t)buf[0]<<24) |
		   ((uint32_t)buf[1]<<16) |
		   ((uint32_t)buf[2]<<8)  |
		   buf[3];
}

void packf32(unsigned char *buf, float f)
{
	uint32_t fhold;

	for(int i = 0; i < 4; ++i){
		fhold = pack754_32(f); // convert to IEEE 754
		packu32(buf, fhold);
		buf += 4;
	}
}

float unpackf32(unsigned char *buf)
{
	float f;
	uint32_t fhold;
	for(int i = 0; i < 4; ++i){
		fhold = unpacku32(buf);
		f = unpack754_32(fhold);
		buf += 4;
	}
	return f;
}