/*
 * QR Code generator library (C)
 *
 * Copyright (c) Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/qr-code-generator-library
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */

// #include <assert.h>
#define assert (void)
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "Helpers/QRCodeGen.hpp"

#ifndef QRCODEGEN_TEST
	#define testable static  // Keep functions private
#else
	// Expose private functions
	#ifndef __cplusplus
		#define testable
	#else
		// Needed for const variables because they are treated as implicitly 'static' in C++
		#define testable extern
	#endif
#endif

/*---- Forward declarations for private functions ----*/

// Regarding all public and private functions defined in this source file:
// - They require all pointer/array arguments to be not null.
// - They only read input scalar/array arguments, write to output pointer/array
//   arguments, and return scalar values; they are "pure" functions.
// - They don't read mutable global variables or write to any global variables.
// - They don't perform I/O, read the clock, print to console, etc.
// - They allocate a small and constant amount of stack memory.
// - They don't allocate or free any memory on the heap.
// - They don't recurse or mutually recurse. All the code
//   could be inlined into the top-level public functions.
// - They run in at most quadratic time with respect to input arguments.
//   Most functions run in linear time, and some in constant time.
//   There are no unbounded loops or non-obvious termination conditions.
// - They are completely thread-safe if the caller does not give the
//   same writable buffer to concurrent calls to these functions.

testable void appendBitsToBuffer(unsigned int val, int numBits, uint8_t buffer[], int *bitLen);

testable void appendErrorCorrection(uint8_t data[], int version, enum qrcodegen_Ecc ecl, uint8_t result[]);
testable int getNumDataCodewords(int version, enum qrcodegen_Ecc ecl);
testable int getNumRawDataModules(int version);

testable void calcReedSolomonGenerator(int degree, uint8_t result[]);
testable void calcReedSolomonRemainder(const uint8_t data[], int dataLen,
	const uint8_t generator[], int degree, uint8_t result[]);
testable uint8_t finiteFieldMultiply(uint8_t x, uint8_t y);

testable void initializeFunctionModules(int version, uint8_t qrcode[]);
static void drawWhiteFunctionModules(uint8_t qrcode[], int version);
static void drawFormatBits(enum qrcodegen_Ecc ecl, enum qrcodegen_Mask mask, uint8_t qrcode[]);
testable int getAlignmentPatternPositions(int version, uint8_t result[7]);
static void fillRectangle(int left, int top, int width, int height, uint8_t qrcode[]);

static void drawCodewords(const uint8_t data[], int dataLen, uint8_t qrcode[]);
static void applyMask(const uint8_t functionModules[], uint8_t qrcode[], enum qrcodegen_Mask mask);
static long getPenaltyScore(const uint8_t qrcode[]);

testable bool getModule(const uint8_t qrcode[], int x, int y);
testable void setModule(uint8_t qrcode[], int x, int y, bool isBlack);
testable void setModuleBounded(uint8_t qrcode[], int x, int y, bool isBlack);

testable int calcSegmentBitLength(enum qrcodegen_Mode mode, size_t numChars);
testable int getTotalBits(const struct qrcodegen_Segment segs[], size_t len, int version);
static int numCharCountBits(enum qrcodegen_Mode mode, int version);



/*---- Private tables of constants ----*/

// For checking text and encoding segments.
static const char *ALPHANUMERIC_CHARSET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

// For generating error correction codes.
testable const int8_t ECC_CODEWORDS_PER_BLOCK[4][41] = {
	// Version: (note that index 0 is for padding, and is set to an illegal value)
	//0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
	{-1,  7, 10, 15, 20, 26, 18, 20, 24, 30, 18, 20, 24, 26, 30, 22, 24, 28, 30, 28, 28, 28, 28, 30, 30, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Low
	{-1, 10, 16, 26, 18, 24, 16, 18, 22, 22, 26, 30, 22, 22, 24, 24, 28, 28, 26, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28},  // Medium
	{-1, 13, 22, 18, 26, 18, 24, 18, 22, 20, 24, 28, 26, 24, 20, 30, 24, 28, 28, 26, 30, 28, 30, 30, 30, 30, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Quartile
	{-1, 17, 28, 22, 16, 22, 28, 26, 26, 24, 28, 24, 28, 22, 24, 24, 30, 28, 28, 26, 28, 30, 24, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // High
};

// For generating error correction codes.
testable const int8_t NUM_ERROR_CORRECTION_BLOCKS[4][41] = {
	// Version: (note that index 0 is for padding, and is set to an illegal value)
	//0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
	{-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25},  // Low
	{-1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49},  // Medium
	{-1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68},  // Quartile
	{-1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81},  // High
};

// For automatic mask pattern selection.
static const int PENALTY_N1 = 3;
static const int PENALTY_N2 = 3;
static const int PENALTY_N3 = 40;
static const int PENALTY_N4 = 10;



/*---- High-level QR Code encoding functions ----*/

// Public function - see documentation comment in header file.
bool qrcodegen_encodeText(const char *text, uint8_t tempBuffer[], uint8_t qrcode[],
		enum qrcodegen_Ecc ecl, int minVersion, int maxVersion, enum qrcodegen_Mask mask, bool boostEcl) {

	size_t textLen = strlen(text);
	if (textLen == 0)
		return qrcodegen_encodeSegmentsAdvanced(NULL, 0, ecl, minVersion, maxVersion, mask, boostEcl, tempBuffer, qrcode);
	size_t bufLen = qrcodegen_BUFFER_LEN_FOR_VERSION(maxVersion);

	struct qrcodegen_Segment seg;
	if (qrcodegen_isNumeric(text)) {
		if (qrcodegen_calcSegmentBufferSize(qrcodegen_Mode_NUMERIC, textLen) > bufLen)
			goto fail;
		seg = qrcodegen_makeNumeric(text, tempBuffer);
	} else if (qrcodegen_isAlphanumeric(text)) {
		if (qrcodegen_calcSegmentBufferSize(qrcodegen_Mode_ALPHANUMERIC, textLen) > bufLen)
			goto fail;
		seg = qrcodegen_makeAlphanumeric(text, tempBuffer);
	} else {
		if (textLen > bufLen)
			goto fail;
		for (size_t i = 0; i < textLen; i++)
			tempBuffer[i] = (uint8_t)text[i];
		seg.mode = qrcodegen_Mode_BYTE;
		seg.bitLength = calcSegmentBitLength(seg.mode, textLen);
		if (seg.bitLength == -1)
			goto fail;
		seg.numChars = (int)textLen;
		seg.data = tempBuffer;
	}
	return qrcodegen_encodeSegmentsAdvanced(&seg, 1, ecl, minVersion, maxVersion, mask, boostEcl, tempBuffer, qrcode);

fail:
	qrcode[0] = 0;  // Set size to invalid value for safety
	return false;
}


// Public function - see documentation comment in header file.
bool qrcodegen_encodeBinary(uint8_t dataAndTemp[], size_t dataLen, uint8_t qrcode[],
		enum qrcodegen_Ecc ecl, int minVersion, int maxVersion, enum qrcodegen_Mask mask, bool boostEcl) {

	struct qrcodegen_Segment seg;
	seg.mode = qrcodegen_Mode_BYTE;
	seg.bitLength = calcSegmentBitLength(seg.mode, dataLen);
	if (seg.bitLength == -1) {
		qrcode[0] = 0;  // Set size to invalid value for safety
		return false;
	}
	seg.numChars = (int)dataLen;
	seg.data = dataAndTemp;
	return qrcodegen_encodeSegmentsAdvanced(&seg, 1, ecl, minVersion, maxVersion, mask, boostEcl, dataAndTemp, qrcode);
}


// Appends the given sequence of bits to the given byte-based bit buffer, increasing the bit length.
testable void appendBitsToBuffer(unsigned int val, int numBits, uint8_t buffer[], int *bitLen) {
	assert(0 <= numBits && numBits <= 16 && (unsigned long)val >> numBits == 0);
	for (int i = numBits - 1; i >= 0; i--, (*bitLen)++)
		buffer[*bitLen >> 3] |= ((val >> i) & 1) << (7 - (*bitLen & 7));
}



/*---- Error correction code generation functions ----*/

// Appends error correction bytes to each block of the given data array, then interleaves bytes
// from the blocks and stores them in the result array. data[0 : rawCodewords - totalEcc] contains
// the input data. data[rawCodewords - totalEcc : rawCodewords] is used as a temporary work area
// and will be clobbered by this function. The final answer is stored in result[0 : rawCodewords].
testable void appendErrorCorrection(uint8_t data[], int version, enum qrcodegen_Ecc ecl, uint8_t result[]) {
	// Calculate parameter numbers
	// assert(0 <= (int)ecl && (int)ecl < 4 && qrcodegen_VERSION_MIN <= version && version <= qrcodegen_VERSION_MAX);
	int numBlocks = NUM_ERROR_CORRECTION_BLOCKS[(int)ecl][version];
	int blockEccLen = ECC_CODEWORDS_PER_BLOCK[(int)ecl][version];
	int rawCodewords = getNumRawDataModules(version) / 8;
	int dataLen = rawCodewords - blockEccLen * numBlocks;
	int numShortBlocks = numBlocks - rawCodewords % numBlocks;
	int shortBlockDataLen = rawCodewords / numBlocks - blockEccLen;

	// Split data into blocks and append ECC after all data
	uint8_t generator[30];
	calcReedSolomonGenerator(blockEccLen, generator);
	for (int i = 0, j = dataLen, k = 0; i < numBlocks; i++) {
		int blockLen = shortBlockDataLen;
		if (i >= numShortBlocks)
			blockLen++;
		calcReedSolomonRemainder(&data[k], blockLen, generator, blockEccLen, &data[j]);
		j += blockEccLen;
		k += blockLen;
	}

	// Interleave (not concatenate) the bytes from every block into a single sequence
	for (int i = 0, k = 0; i < numBlocks; i++) {
		for (int j = 0, l = i; j < shortBlockDataLen; j++, k++, l += numBlocks)
			result[l] = data[k];
		if (i >= numShortBlocks)
			k++;
	}
	for (int i = numShortBlocks, k = (numShortBlocks + 1) * shortBlockDataLen, l = numBlocks * shortBlockDataLen;
			i < numBlocks; i++, k += shortBlockDataLen + 1, l++)
		result[l] = data[k];
	for (int i = 0, k = dataLen; i < numBlocks; i++) {
		for (int j = 0, l = dataLen + i; j < blockEccLen; j++, k++, l += numBlocks)
			result[l] = data[k];
	}
}


// Returns the number of 8-bit codewords that can be used for storing data (not ECC),
// for the given version number and error correction level. The result is in the range [9, 2956].
testable int getNumDataCodewords(int version, enum qrcodegen_Ecc ecl) {
	int v = version, e = (int)ecl;
	assert(0 <= e && e < 4 && qrcodegen_VERSION_MIN <= v && v <= qrcodegen_VERSION_MAX);
	return getNumRawDataModules(v) / 8 - ECC_CODEWORDS_PER_BLOCK[e][v] * NUM_ERROR_CORRECTION_BLOCKS[e][v];
}


// Returns the number of data bits that can be stored in a QR Code of the given version number, after
// all function modules are excluded. This includes remainder bits, so it might not be a multiple of 8.
// The result is in the range [208, 29648]. This could be implemented as a 40-entry lookup table.
testable int getNumRawDataModules(int version) {
	assert(qrcodegen_VERSION_MIN <= version && version <= qrcodegen_VERSION_MAX);
	int result = (16 * version + 128) * version + 64;
	if (version >= 2) {
		int numAlign = version / 7 + 2;
		result -= (25 * numAlign - 10) * numAlign - 55;
		if (version >= 7)
			result -= 18 * 2;  // Subtract version information
	}
	return result;
}



/*---- Reed-Solomon ECC generator functions ----*/

// Calculates the Reed-Solomon generator polynomial of the given degree, storing in result[0 : degree].
testable void calcReedSolomonGenerator(int degree, uint8_t result[]) {
	// Start with the monomial x^0
	assert(1 <= degree && degree <= 30);
	memset(result, 0, degree * sizeof(result[0]));
	result[degree - 1] = 1;

	// Compute the product polynomial (x - r^0) * (x - r^1) * (x - r^2) * ... * (x - r^{degree-1}),
	// drop the highest term, and store the rest of the coefficients in order of descending powers.
	// Note that r = 0x02, which is a generator element of this field GF(2^8/0x11D).
	uint8_t root = 1;
	for (int i = 0; i < degree; i++) {
		// Multiply the current product by (x - r^i)
		for (int j = 0; j < degree; j++) {
			result[j] = finiteFieldMultiply(result[j], root);
			if (j + 1 < degree)
				result[j] ^= result[j + 1];
		}
		root = finiteFieldMultiply(root, 0x02);
	}
}


// Calculates the remainder of the polynomial data[0 : dataLen] when divided by the generator[0 : degree], where all
// polynomials are in big endian and the generator has an implicit leading 1 term, storing the result in result[0 : degree].
testable void calcReedSolomonRemainder(const uint8_t data[], int dataLen,
		const uint8_t generator[], int degree, uint8_t result[]) {

	// Perform polynomial division
	assert(1 <= degree && degree <= 30);
	memset(result, 0, degree * sizeof(result[0]));
	for (int i = 0; i < dataLen; i++) {
		uint8_t factor = data[i] ^ result[0];
		memmove(&result[0], &result[1], (degree - 1) * sizeof(result[0]));
		result[degree - 1] = 0;
		for (int j = 0; j < degree; j++)
			result[j] ^= finiteFieldMultiply(generator[j], factor);
	}
}


// Returns the product of the two given field elements modulo GF(2^8/0x11D).
// All inputs are valid. This could be implemented as a 256*256 lookup table.
testable uint8_t finiteFieldMultiply(uint8_t x, uint8_t y) {
	// Russian peasant multiplication
	uint8_t z = 0;
	for (int i = 7; i >= 0; i--) {
		z = (z << 1) ^ ((z >> 7) * 0x11D);
		z ^= ((y >> i) & 1) * x;
	}
	return z;
}



/*---- Drawing function modules ----*/

// Clears the given QR Code grid with white modules for the given
// version's size, then marks every function module as black.
testable void initializeFunctionModules(int version, uint8_t qrcode[]) {
	// Initialize QR Code
	int qrsize = version * 4 + 17;
	memset(qrcode, 0, ((qrsize * qrsize + 7) / 8 + 1) * sizeof(qrcode[0]));
	qrcode[0] = (uint8_t)qrsize;

	// Fill horizontal and vertical timing patterns
	fillRectangle(6, 0, 1, qrsize, qrcode);
	fillRectangle(0, 6, qrsize, 1, qrcode);

	// Fill 3 finder patterns (all corners except bottom right) and format bits
	fillRectangle(0, 0, 9, 9, qrcode);
	fillRectangle(qrsize - 8, 0, 8, 9, qrcode);
	fillRectangle(0, qrsize - 8, 9, 8, qrcode);

	// Fill numerous alignment patterns
	uint8_t alignPatPos[7] = {0};
	int numAlign = getAlignmentPatternPositions(version, alignPatPos);
	for (int i = 0; i < numAlign; i++) {
		for (int j = 0; j < numAlign; j++) {
			if ((i == 0 && j == 0) || (i == 0 && j == numAlign - 1) || (i == numAlign - 1 && j == 0))
				continue;  // Skip the three finder corners
			else
				fillRectangle(alignPatPos[i] - 2, alignPatPos[j] - 2, 5, 5, qrcode);
		}
	}

	// Fill version blocks
	if (version >= 7) {
		fillRectangle(qrsize - 11, 0, 3, 6, qrcode);
		fillRectangle(0, qrsize - 11, 6, 3, qrcode);
	}
}


// Draws white function modules and possibly some black modules onto the given QR Code, without changing
// non-function modules. This does not draw the format bits. This requires all function modules to be previously
// marked black (namely by initializeFunctionModules()), because this may skip redrawing black function modules.
static void drawWhiteFunctionModules(uint8_t qrcode[], int version) {
	// Draw horizontal and vertical timing patterns
	int qrsize = qrcodegen_getSize(qrcode);
	for (int i = 7; i < qrsize - 7; i += 2) {
		setModule(qrcode, 6, i, false);
		setModule(qrcode, i, 6, false);
	}

	// Draw 3 finder patterns (all corners except bottom right; overwrites some timing modules)
	for (int i = -4; i <= 4; i++) {
		for (int j = -4; j <= 4; j++) {
			int dist = abs(i);
			if (abs(j) > dist)
				dist = abs(j);
			if (dist == 2 || dist == 4) {
				setModuleBounded(qrcode, 3 + j, 3 + i, false);
				setModuleBounded(qrcode, qrsize - 4 + j, 3 + i, false);
				setModuleBounded(qrcode, 3 + j, qrsize - 4 + i, false);
			}
		}
	}

	// Draw numerous alignment patterns
	uint8_t alignPatPos[7] = {0};
	int numAlign = getAlignmentPatternPositions(version, alignPatPos);
	for (int i = 0; i < numAlign; i++) {
		for (int j = 0; j < numAlign; j++) {
			if ((i == 0 && j == 0) || (i == 0 && j == numAlign - 1) || (i == numAlign - 1 && j == 0))
				continue;  // Skip the three finder corners
			else {
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++)
						setModule(qrcode, alignPatPos[i] + l, alignPatPos[j] + k, k == 0 && l == 0);
				}
			}
		}
	}

	// Draw version blocks
	if (version >= 7) {
		// Calculate error correction code and pack bits
		int rem = version;  // version is uint6, in the range [7, 40]
		for (int i = 0; i < 12; i++)
			rem = (rem << 1) ^ ((rem >> 11) * 0x1F25);
		long data = (long)version << 12 | rem;  // uint18
		assert(data >> 18 == 0);

		// Draw two copies
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 3; j++) {
				int k = qrsize - 11 + j;
				setModule(qrcode, k, i, (data & 1) != 0);
				setModule(qrcode, i, k, (data & 1) != 0);
				data >>= 1;
			}
		}
	}
}


// Draws two copies of the format bits (with its own error correction code) based
// on the given mask and error correction level. This always draws all modules of
// the format bits, unlike drawWhiteFunctionModules() which might skip black modules.
static void drawFormatBits(enum qrcodegen_Ecc ecl, enum qrcodegen_Mask mask, uint8_t qrcode[]) {
	// Calculate error correction code and pack bits
	assert(0 <= (int)mask && (int)mask <= 7);
	int data = 0;
	switch (ecl) {
		case qrcodegen_Ecc_LOW     :  data = 1;  break;
		case qrcodegen_Ecc_MEDIUM  :  data = 0;  break;
		case qrcodegen_Ecc_QUARTILE:  data = 3;  break;
		case qrcodegen_Ecc_HIGH    :  data = 2;  break;
		default:  assert(false);
	}
	data = data << 3 | (int)mask;  // ecl-derived value is uint2, mask is uint3
	int rem = data;
	for (int i = 0; i < 10; i++)
		rem = (rem << 1) ^ ((rem >> 9) * 0x537);
	data = data << 10 | rem;
	data ^= 0x5412;  // uint15
	assert(data >> 15 == 0);

	// Draw first copy
	for (int i = 0; i <= 5; i++)
		setModule(qrcode, 8, i, ((data >> i) & 1) != 0);
	setModule(qrcode, 8, 7, ((data >> 6) & 1) != 0);
	setModule(qrcode, 8, 8, ((data >> 7) & 1) != 0);
	setModule(qrcode, 7, 8, ((data >> 8) & 1) != 0);
	for (int i = 9; i < 15; i++)
		setModule(qrcode, 14 - i, 8, ((data >> i) & 1) != 0);

	// Draw second copy
	int qrsize = qrcodegen_getSize(qrcode);
	for (int i = 0; i <= 7; i++)
		setModule(qrcode, qrsize - 1 - i, 8, ((data >> i) & 1) != 0);
	for (int i = 8; i < 15; i++)
		setModule(qrcode, 8, qrsize - 15 + i, ((data >> i) & 1) != 0);
	setModule(qrcode, 8, qrsize - 8, true);
}


// Calculates the positions of alignment patterns in ascending order for the given version number,
// storing them to the given array and returning an array length in the range [0, 7].
testable int getAlignmentPatternPositions(int version, uint8_t result[7]) {
	if (version == 1)
		return 0;
	int numAlign = version / 7 + 2;
	int step;
	if (version != 32) {
		// ceil((size - 13) / (2*numAlign - 2)) * 2
		step = (version * 4 + numAlign * 2 + 1) / (2 * numAlign - 2) * 2;
	} else  // C-C-C-Combo breaker!
		step = 26;
	for (int i = numAlign - 1, pos = version * 4 + 10; i >= 1; i--, pos -= step)
		result[i] = pos;
	result[0] = 6;
	return numAlign;
}


// Sets every pixel in the range [left : left + width] * [top : top + height] to black.
static void fillRectangle(int left, int top, int width, int height, uint8_t qrcode[]) {
	for (int dy = 0; dy < height; dy++) {
		for (int dx = 0; dx < width; dx++)
			setModule(qrcode, left + dx, top + dy, true);
	}
}



/*---- Drawing data modules and masking ----*/

// Draws the raw codewords (including data and ECC) onto the given QR Code. This requires the initial state of
// the QR Code to be black at function modules and white at codeword modules (including unused remainder bits).
static void drawCodewords(const uint8_t data[], int dataLen, uint8_t qrcode[]) {
	int qrsize = qrcodegen_getSize(qrcode);
	int i = 0;  // Bit index into the data
	// Do the funny zigzag scan
	for (int right = qrsize - 1; right >= 1; right -= 2) {  // Index of right column in each column pair
		if (right == 6)
			right = 5;
		for (int vert = 0; vert < qrsize; vert++) {  // Vertical counter
			for (int j = 0; j < 2; j++) {
				int x = right - j;  // Actual x coordinate
				bool upward = ((right + 1) & 2) == 0;
				int y = upward ? qrsize - 1 - vert : vert;  // Actual y coordinate
				if (!getModule(qrcode, x, y) && i < dataLen * 8) {
					bool black = ((data[i >> 3] >> (7 - (i & 7))) & 1) != 0;
					setModule(qrcode, x, y, black);
					i++;
				}
				// If there are any remainder bits (0 to 7), they are already
				// set to 0/false/white when the grid of modules was initialized
			}
		}
	}
	assert(i == dataLen * 8);
}


// XORs the data modules in this QR Code with the given mask pattern. Due to XOR's mathematical
// properties, calling applyMask(..., m) twice with the same value is equivalent to no change at all.
// This means it is possible to apply a mask, undo it, and try another mask. Note that a final
// well-formed QR Code symbol needs exactly one mask applied (not zero, not two, etc.).
static void applyMask(const uint8_t functionModules[], uint8_t qrcode[], enum qrcodegen_Mask mask) {
	assert(0 <= (int)mask && (int)mask <= 7);  // Disallows qrcodegen_Mask_AUTO
	int qrsize = qrcodegen_getSize(qrcode);
	for (int y = 0; y < qrsize; y++) {
		for (int x = 0; x < qrsize; x++) {
			if (getModule(functionModules, x, y))
				continue;
			bool invert = false;
			switch ((int)mask) {
				case 0:  invert = (x + y) % 2 == 0;                    break;
				case 1:  invert = y % 2 == 0;                          break;
				case 2:  invert = x % 3 == 0;                          break;
				case 3:  invert = (x + y) % 3 == 0;                    break;
				case 4:  invert = (x / 3 + y / 2) % 2 == 0;            break;
				case 5:  invert = x * y % 2 + x * y % 3 == 0;          break;
				case 6:  invert = (x * y % 2 + x * y % 3) % 2 == 0;    break;
				case 7:  invert = ((x + y) % 2 + x * y % 3) % 2 == 0;  break;
				default:  assert(false);
			}
			bool val = getModule(qrcode, x, y);
			setModule(qrcode, x, y, val ^ invert);
		}
	}
}


// Calculates and returns the penalty score based on state of the given QR Code's current modules.
// This is used by the automatic mask choice algorithm to find the mask pattern that yields the lowest score.
static long getPenaltyScore(const uint8_t qrcode[]) {
	int qrsize = qrcodegen_getSize(qrcode);
	long result = 0;

	// Adjacent modules in row having same color
	for (int y = 0; y < qrsize; y++) {
		bool colorX;
		for (int x = 0, runX; x < qrsize; x++) {
			if (x == 0 || getModule(qrcode, x, y) != colorX) {
				colorX = getModule(qrcode, x, y);
				runX = 1;
			} else {
				runX++;
				if (runX == 5)
					result += PENALTY_N1;
				else if (runX > 5)
					result++;
			}
		}
	}
	// Adjacent modules in column having same color
	for (int x = 0; x < qrsize; x++) {
		bool colorY;
		for (int y = 0, runY; y < qrsize; y++) {
			if (y == 0 || getModule(qrcode, x, y) != colorY) {
				colorY = getModule(qrcode, x, y);
				runY = 1;
			} else {
				runY++;
				if (runY == 5)
					result += PENALTY_N1;
				else if (runY > 5)
					result++;
			}
		}
	}

	// 2*2 blocks of modules having same color
	for (int y = 0; y < qrsize - 1; y++) {
		for (int x = 0; x < qrsize - 1; x++) {
			bool  color = getModule(qrcode, x, y);
			if (  color == getModule(qrcode, x + 1, y) &&
			      color == getModule(qrcode, x, y + 1) &&
			      color == getModule(qrcode, x + 1, y + 1))
				result += PENALTY_N2;
		}
	}

	// Finder-like pattern in rows
	for (int y = 0; y < qrsize; y++) {
		for (int x = 0, bits = 0; x < qrsize; x++) {
			bits = ((bits << 1) & 0x7FF) | (getModule(qrcode, x, y) ? 1 : 0);
			if (x >= 10 && (bits == 0x05D || bits == 0x5D0))  // Needs 11 bits accumulated
				result += PENALTY_N3;
		}
	}
	// Finder-like pattern in columns
	for (int x = 0; x < qrsize; x++) {
		for (int y = 0, bits = 0; y < qrsize; y++) {
			bits = ((bits << 1) & 0x7FF) | (getModule(qrcode, x, y) ? 1 : 0);
			if (y >= 10 && (bits == 0x05D || bits == 0x5D0))  // Needs 11 bits accumulated
				result += PENALTY_N3;
		}
	}

	// Balance of black and white modules
	int black = 0;
	for (int y = 0; y < qrsize; y++) {
		for (int x = 0; x < qrsize; x++) {
			if (getModule(qrcode, x, y))
				black++;
		}
	}
	int total = qrsize * qrsize;
	// Find smallest k such that (45-5k)% <= dark/total <= (55+5k)%
	for (int k = 0; black*20L < (9L-k)*total || black*20L > (11L+k)*total; k++)
		result += PENALTY_N4;
	return result;
}



/*---- Basic QR Code information ----*/

// Public function - see documentation comment in header file.
int qrcodegen_getSize(const uint8_t qrcode[]) {
	assert(qrcode != NULL);
	int result = qrcode[0];
	assert((qrcodegen_VERSION_MIN * 4 + 17) <= result
		&& result <= (qrcodegen_VERSION_MAX * 4 + 17));
	return result;
}


// Public function - see documentation comment in header file.
bool qrcodegen_getModule(const uint8_t qrcode[], int x, int y) {
	assert(qrcode != NULL);
	int qrsize = qrcode[0];
	return (0 <= x && x < qrsize && 0 <= y && y < qrsize) && getModule(qrcode, x, y);
}


// Gets the module at the given coordinates, which must be in bounds.
testable bool getModule(const uint8_t qrcode[], int x, int y) {
	int qrsize = qrcode[0];
	assert(21 <= qrsize && qrsize <= 177 && 0 <= x && x < qrsize && 0 <= y && y < qrsize);
	int index = y * qrsize + x;
	int bitIndex = index & 7;
	int byteIndex = (index >> 3) + 1;
	return ((qrcode[byteIndex] >> bitIndex) & 1) != 0;
}


// Sets the module at the given coordinates, which must be in bounds.
testable void setModule(uint8_t qrcode[], int x, int y, bool isBlack) {
	int qrsize = qrcode[0];
	assert(21 <= qrsize && qrsize <= 177 && 0 <= x && x < qrsize && 0 <= y && y < qrsize);
	int index = y * qrsize + x;
	int bitIndex = index & 7;
	int byteIndex = (index >> 3) + 1;
	if (isBlack)
		qrcode[byteIndex] |= 1 << bitIndex;
	else
		qrcode[byteIndex] &= (1 << bitIndex) ^ 0xFF;
}


// Sets the module at the given coordinates, doing nothing if out of bounds.
testable void setModuleBounded(uint8_t qrcode[], int x, int y, bool isBlack) {
	int qrsize = qrcode[0];
	if (0 <= x && x < qrsize && 0 <= y && y < qrsize)
		setModule(qrcode, x, y, isBlack);
}



/*---- Segment handling ----*/

// Public function - see documentation comment in header file.
bool qrcodegen_isAlphanumeric(const char *text) {
	assert(text != NULL);
	for (; *text != '\0'; text++) {
		if (strchr(ALPHANUMERIC_CHARSET, *text) == NULL)
			return false;
	}
	return true;
}


// Public function - see documentation comment in header file.
bool qrcodegen_isNumeric(const char *text) {
	assert(text != NULL);
	for (; *text != '\0'; text++) {
		if (*text < '0' || *text > '9')
			return false;
	}
	return true;
}


// Public function - see documentation comment in header file.
size_t qrcodegen_calcSegmentBufferSize(enum qrcodegen_Mode mode, size_t numChars) {
	int temp = calcSegmentBitLength(mode, numChars);
	if (temp == -1)
		return SIZE_MAX;
	assert(0 <= temp && temp <= INT16_MAX);
	return ((size_t)temp + 7) / 8;
}


// Returns the number of data bits needed to represent a segment
// containing the given number of characters using the given mode. Notes:
// - Returns -1 on failure, i.e. numChars > INT16_MAX or
//   the number of needed bits exceeds INT16_MAX (i.e. 32767).
// - Otherwise, all valid results are in the range [0, INT16_MAX].
// - For byte mode, numChars measures the number of bytes, not Unicode code points.
// - For ECI mode, numChars must be 0, and the worst-case number of bits is returned.
//   An actual ECI segment can have shorter data. For non-ECI modes, the result is exact.
testable int calcSegmentBitLength(enum qrcodegen_Mode mode, size_t numChars) {
	const int LIMIT = INT16_MAX;  // Can be configured as high as INT_MAX
	if (numChars > (unsigned int)LIMIT)
		return -1;
	int n = (int)numChars;

	int result = -2;
	if (mode == qrcodegen_Mode_NUMERIC) {
		// n * 3 + ceil(n / 3)
		if (n > LIMIT / 3)
			goto overflow;
		result = n * 3;
		int temp = n / 3 + (n % 3 == 0 ? 0 : 1);
		if (temp > LIMIT - result)
			goto overflow;
		result += temp;
	} else if (mode == qrcodegen_Mode_ALPHANUMERIC) {
		// n * 5 + ceil(n / 2)
		if (n > LIMIT / 5)
			goto overflow;
		result = n * 5;
		int temp = n / 2 + n % 2;
		if (temp > LIMIT - result)
			goto overflow;
		result += temp;
	} else if (mode == qrcodegen_Mode_BYTE) {
		if (n > LIMIT / 8)
			goto overflow;
		result = n * 8;
	} else if (mode == qrcodegen_Mode_KANJI) {
		if (n > LIMIT / 13)
			goto overflow;
		result = n * 13;
	} else if (mode == qrcodegen_Mode_ECI && numChars == 0)
		result = 3 * 8;
	assert(0 <= result && result <= LIMIT);
	return result;
overflow:
	return -1;
}


// Public function - see documentation comment in header file.
struct qrcodegen_Segment qrcodegen_makeBytes(const uint8_t data[], size_t len, uint8_t buf[]) {
	assert(data != NULL || len == 0);
	struct qrcodegen_Segment result;
	result.mode = qrcodegen_Mode_BYTE;
	result.bitLength = calcSegmentBitLength(result.mode, len);
	assert(result.bitLength != -1);
	result.numChars = (int)len;
	if (len > 0)
		memcpy(buf, data, len * sizeof(buf[0]));
	result.data = buf;
	return result;
}


// Public function - see documentation comment in header file.
struct qrcodegen_Segment qrcodegen_makeNumeric(const char *digits, uint8_t buf[]) {
	assert(digits != NULL);
	struct qrcodegen_Segment result;
	size_t len = strlen(digits);
	result.mode = qrcodegen_Mode_NUMERIC;
	int bitLen = calcSegmentBitLength(result.mode, len);
	assert(bitLen != -1);
	result.numChars = (int)len;
	if (bitLen > 0)
		memset(buf, 0, ((size_t)bitLen + 7) / 8 * sizeof(buf[0]));
	result.bitLength = 0;

	unsigned int accumData = 0;
	int accumCount = 0;
	for (; *digits != '\0'; digits++) {
		char c = *digits;
		assert('0' <= c && c <= '9');
		accumData = accumData * 10 + (c - '0');
		accumCount++;
		if (accumCount == 3) {
			appendBitsToBuffer(accumData, 10, buf, &result.bitLength);
			accumData = 0;
			accumCount = 0;
		}
	}
	if (accumCount > 0)  // 1 or 2 digits remaining
		appendBitsToBuffer(accumData, accumCount * 3 + 1, buf, &result.bitLength);
	assert(result.bitLength == bitLen);
	result.data = buf;
	return result;
}


// Public function - see documentation comment in header file.
struct qrcodegen_Segment qrcodegen_makeAlphanumeric(const char *text, uint8_t buf[]) {
	assert(text != NULL);
	struct qrcodegen_Segment result;
	size_t len = strlen(text);
	result.mode = qrcodegen_Mode_ALPHANUMERIC;
	int bitLen = calcSegmentBitLength(result.mode, len);
	assert(bitLen != -1);
	result.numChars = (int)len;
	if (bitLen > 0)
		memset(buf, 0, ((size_t)bitLen + 7) / 8 * sizeof(buf[0]));
	result.bitLength = 0;

	unsigned int accumData = 0;
	int accumCount = 0;
	for (; *text != '\0'; text++) {
		const char *temp = strchr(ALPHANUMERIC_CHARSET, *text);
		assert(temp != NULL);
		accumData = accumData * 45 + (temp - ALPHANUMERIC_CHARSET);
		accumCount++;
		if (accumCount == 2) {
			appendBitsToBuffer(accumData, 11, buf, &result.bitLength);
			accumData = 0;
			accumCount = 0;
		}
	}
	if (accumCount > 0)  // 1 character remaining
		appendBitsToBuffer(accumData, 6, buf, &result.bitLength);
	assert(result.bitLength == bitLen);
	result.data = buf;
	return result;
}


// Public function - see documentation comment in header file.
struct qrcodegen_Segment qrcodegen_makeEci(long assignVal, uint8_t buf[]) {
	struct qrcodegen_Segment result;
	result.mode = qrcodegen_Mode_ECI;
	result.numChars = 0;
	result.bitLength = 0;
	if (0 <= assignVal && assignVal < (1 << 7)) {
		memset(buf, 0, 1 * sizeof(buf[0]));
		appendBitsToBuffer(assignVal, 8, buf, &result.bitLength);
	} else if ((1 << 7) <= assignVal && assignVal < (1 << 14)) {
		memset(buf, 0, 2 * sizeof(buf[0]));
		appendBitsToBuffer(2, 2, buf, &result.bitLength);
		appendBitsToBuffer(assignVal, 14, buf, &result.bitLength);
	} else if ((1 << 14) <= assignVal && assignVal < 1000000L) {
		memset(buf, 0, 3 * sizeof(buf[0]));
		appendBitsToBuffer(6, 3, buf, &result.bitLength);
		appendBitsToBuffer(assignVal >> 10, 11, buf, &result.bitLength);
		appendBitsToBuffer(assignVal & 0x3FF, 10, buf, &result.bitLength);
	} else
		assert(false);
	result.data = buf;
	return result;
}


// Public function - see documentation comment in header file.
bool qrcodegen_encodeSegments(const struct qrcodegen_Segment segs[], size_t len,
		enum qrcodegen_Ecc ecl, uint8_t tempBuffer[], uint8_t qrcode[]) {
	return qrcodegen_encodeSegmentsAdvanced(segs, len, ecl,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, -1, true, tempBuffer, qrcode);
}


// Public function - see documentation comment in header file.
bool qrcodegen_encodeSegmentsAdvanced(const struct qrcodegen_Segment segs[], size_t len, enum qrcodegen_Ecc ecl,
		int minVersion, int maxVersion, int mask, bool boostEcl, uint8_t tempBuffer[], uint8_t qrcode[]) {
	assert(segs != NULL || len == 0);
	assert(qrcodegen_VERSION_MIN <= minVersion && minVersion <= maxVersion && maxVersion <= qrcodegen_VERSION_MAX);
	// assert(0 <= (int)ecl && (int)ecl <= 3 && -1 <= (int)mask && (int)mask <= 7);

	// Find the minimal version number to use
	int version, dataUsedBits;
	for (version = minVersion; ; version++) {
		int dataCapacityBits = getNumDataCodewords(version, ecl) * 8;  // Number of data bits available
		dataUsedBits = getTotalBits(segs, len, version);
		if (dataUsedBits != -1 && dataUsedBits <= dataCapacityBits)
			break;  // This version number is found to be suitable
		if (version >= maxVersion) {  // All versions in the range could not fit the given data
			qrcode[0] = 0;  // Set size to invalid value for safety
			return false;
		}
	}
	assert(dataUsedBits != -1);

	// Increase the error correction level while the data still fits in the current version number
	for (int i = (int)qrcodegen_Ecc_MEDIUM; i <= (int)qrcodegen_Ecc_HIGH; i++) {
		if (boostEcl && dataUsedBits <= getNumDataCodewords(version, (enum qrcodegen_Ecc)i) * 8)
			ecl = (enum qrcodegen_Ecc)i;
	}

	// Create the data bit string by concatenating all segments
	int dataCapacityBits = getNumDataCodewords(version, ecl) * 8;
	memset(qrcode, 0, qrcodegen_BUFFER_LEN_FOR_VERSION(version) * sizeof(qrcode[0]));
	int bitLen = 0;
	for (size_t i = 0; i < len; i++) {
		const struct qrcodegen_Segment *seg = &segs[i];
		unsigned int modeBits = 0;
		switch (seg->mode) {
			case qrcodegen_Mode_NUMERIC     :  modeBits = 0x1;  break;
			case qrcodegen_Mode_ALPHANUMERIC:  modeBits = 0x2;  break;
			case qrcodegen_Mode_BYTE        :  modeBits = 0x4;  break;
			case qrcodegen_Mode_KANJI       :  modeBits = 0x8;  break;
			case qrcodegen_Mode_ECI         :  modeBits = 0x7;  break;
			default:  assert(false);
		}
		appendBitsToBuffer(modeBits, 4, qrcode, &bitLen);
		appendBitsToBuffer(seg->numChars, numCharCountBits(seg->mode, version), qrcode, &bitLen);
		for (int j = 0; j < seg->bitLength; j++)
			appendBitsToBuffer((seg->data[j >> 3] >> (7 - (j & 7))) & 1, 1, qrcode, &bitLen);
	}

	// Add terminator and pad up to a byte if applicable
	int terminatorBits = dataCapacityBits - bitLen;
	if (terminatorBits > 4)
		terminatorBits = 4;
	appendBitsToBuffer(0, terminatorBits, qrcode, &bitLen);
	appendBitsToBuffer(0, (8 - bitLen % 8) % 8, qrcode, &bitLen);

	// Pad with alternate bytes until data capacity is reached
	for (uint8_t padByte = 0xEC; bitLen < dataCapacityBits; padByte ^= 0xEC ^ 0x11)
		appendBitsToBuffer(padByte, 8, qrcode, &bitLen);
	assert(bitLen % 8 == 0);

	// Draw function and data codeword modules
	appendErrorCorrection(qrcode, version, ecl, tempBuffer);
	initializeFunctionModules(version, qrcode);
	drawCodewords(tempBuffer, getNumRawDataModules(version) / 8, qrcode);
	drawWhiteFunctionModules(qrcode, version);
	initializeFunctionModules(version, tempBuffer);

	// Handle masking
	if (mask == qrcodegen_Mask_AUTO) {  // Automatically choose best mask
		long minPenalty = LONG_MAX;
		for (int i = 0; i < 8; i++) {
			drawFormatBits(ecl, (enum qrcodegen_Mask)i, qrcode);
			applyMask(tempBuffer, qrcode, (enum qrcodegen_Mask)i);
			long penalty = getPenaltyScore(qrcode);
			if (penalty < minPenalty) {
				mask = (enum qrcodegen_Mask)i;
				minPenalty = penalty;
			}
			applyMask(tempBuffer, qrcode, (enum qrcodegen_Mask)i);  // Undoes the mask due to XOR
		}
	}
	assert(0 <= (int)mask && (int)mask <= 7);
	drawFormatBits(ecl, (enum qrcodegen_Mask)mask, qrcode);
	applyMask(tempBuffer, qrcode, (enum qrcodegen_Mask)mask);
	return true;
}


// Returns the number of bits needed to encode the given list of segments at the given version.
// The result is in the range [0, 32767] if successful. Otherwise, -1 is returned if any segment
// has more characters than allowed by that segment's mode's character count field at the version,
// or if the actual answer exceeds INT16_MAX.
testable int getTotalBits(const struct qrcodegen_Segment segs[], size_t len, int version) {
	assert(segs != NULL || len == 0);
	assert(qrcodegen_VERSION_MIN <= version && version <= qrcodegen_VERSION_MAX);
	int result = 0;
	for (size_t i = 0; i < len; i++) {
		int numChars = segs[i].numChars;
		int bitLength = segs[i].bitLength;
		assert(0 <= numChars && numChars <= INT16_MAX);
		assert(0 <= bitLength && bitLength <= INT16_MAX);
		int ccbits = numCharCountBits(segs[i].mode, version);
		assert(0 <= ccbits && ccbits <= 16);
		// Fail if segment length value doesn't fit in the length field's bit-width
		if (numChars >= (1L << ccbits))
			return -1;
		long temp = 4L + ccbits + bitLength;
		if (temp > INT16_MAX - result)
			return -1;
		result += temp;
	}
	assert(0 <= result && result <= INT16_MAX);
	return result;
}


// Returns the bit width of the segment character count field for the
// given mode at the given version number. The result is in the range [0, 16].
static int numCharCountBits(enum qrcodegen_Mode mode, int version) {
	assert(qrcodegen_VERSION_MIN <= version && version <= qrcodegen_VERSION_MAX);
	int i = 0;
	if      ( 1 <= version && version <=  9)  i = 0;
	else if (10 <= version && version <= 26)  i = 1;
	else if (27 <= version && version <= 40)  i = 2;
	else  assert(false);

	switch (mode) {
		case qrcodegen_Mode_NUMERIC     : { const int temp[] = {10, 12, 14}; return temp[i]; }
		case qrcodegen_Mode_ALPHANUMERIC: { const int temp[] = { 9, 11, 13}; return temp[i]; }
		case qrcodegen_Mode_BYTE        : { const int temp[] = { 8, 16, 16}; return temp[i]; }
		case qrcodegen_Mode_KANJI       : { const int temp[] = { 8, 10, 12}; return temp[i]; }
		case qrcodegen_Mode_ECI         : return 0;
		// default:  assert(false);
        default: return -1;
	}
}