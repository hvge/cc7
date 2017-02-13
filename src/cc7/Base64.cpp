/*
 * Copyright 2016 Juraj Durech <durech.juraj@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cc7/Base64.h>
#include <cc7/Utilities.h>

namespace cc7
{
	// -----------------------------------------------------------------
	// BASE64 conversions
	//
	// Just for curiosity, following routines are based on my previous
	// work created for Intype text editor :)
	// -----------------------------------------------------------------
	
	// MARK: Encoder -
	static const char * s_enc_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	static size_t _EstimateEncodedLength(size_t len, size_t wrap_output)
	{
		size_t n = ((len + 2) / 3) * 4;
		if (wrap_output > 0) {
			n += strlen("\n") * ((n / wrap_output) + 1);
		}
		return n;
	}

	bool Base64_Encode(const ByteRange & range, size_t wrap_size, std::string & out_string)
	{
		out_string.clear();
		
		if (wrap_size > 0) {
			if (utilities::AlignValue<4>(wrap_size) != wrap_size) {
				CC7_ASSERT(false, "wrap_size must be divisible by 4");
				return false;
			}
		}
		
		out_string.reserve(_EstimateEncodedLength(range.size(), wrap_size));
		
		char block_4[4];
		const byte * in_p   = range.data();
		size_t in_len       = range.size();
		size_t wrap_pos     = 0;
		
		while (in_len >= 3) {
			// Process all aligned triplets
			block_4[0] = s_enc_table[  (in_p[0] & 0xfc) >> 2                            ];
			block_4[1] = s_enc_table[ ((in_p[0] & 0x03) << 4) + ((in_p[1] & 0xf0) >> 4) ];
			block_4[2] = s_enc_table[ ((in_p[1] & 0x0f) << 2) + ((in_p[2] & 0xc0) >> 6) ];
			block_4[3] = s_enc_table[   in_p[2] & 0x3f                                  ];
			in_len -= 3;
			in_p   += 3;
			out_string.append(block_4, 4);
			if (wrap_size) {
				wrap_pos += 4;
				if (wrap_pos >= wrap_size) {
					out_string.append("\n");
					wrap_pos = 0;
				}
			}
		}
		if (in_len > 0) {
			// Process the rest of unaligned bytes
			block_4[0] = s_enc_table[  (in_p[0] >> 2) & 0x3f ];
			block_4[1] = s_enc_table[ ((in_p[0] << 4) + (--in_len ? in_p[1] >> 4 : 0)) & 0x3f ];
			block_4[2] = (in_len ? s_enc_table[ ((in_p[1] << 2) + (--in_len ? (in_p[2]) >> 6 : 0)) & 0x3f ] : '=');
			block_4[3] = '=';
			out_string.append(block_4, 4);
		}
		return true;
	}
	
	
	// MARK: Decoder -
	
	/*
	 Decoder table contains conversion from arbitrary 8 bit character to radix value.
	 If the translated value is equal to 0xff then the input character is invalid.
	 */
	static const byte s_dec_table[256] =
	{
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f,
		0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
		0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
		0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	};
	
	static bool Base64_DecodeNoWrap(const std::string & str, size_t sequence_start, size_t sequence_length,
									ByteArray & out_data,
									bool & end_marker)
	{
		if (sequence_length == 0) {
			// Not a real end marker, but this is the end of processing.
			end_marker = true;
			return true;
		}
		
		// Check sequence length.
		if ((sequence_length & 3) != 0) {
			// Wrong size of the sequence. No assertion, because we're using
			// this routine also for non-wrapped strings.
			return false;
		}
		if (sequence_start + sequence_length > str.length()) {
			// Internal error. The provided sequence is out of the input string's range.
			CC7_ASSERT(false, "Internal error. Provided block for decoding is too long");
			return false;
		}
		
		//
		// Reserve bytes in the byte array. The produced_size is a worst case
		// estimation for length of final data.
		//
		size_t blocks_count  = sequence_length / 4;
		size_t block_size    = blocks_count * 3;
		out_data.reserve(out_data.size() + block_size);
		
		// Input pointer
		const byte * block_4 = reinterpret_cast<const byte*>(str.c_str()) + sequence_start;
		
		// Check if last block contains padding and thus requires additional processing.
		end_marker = block_4[sequence_length - 1] == '=' || block_4[sequence_length - 2] == '=';
		if (end_marker) {
			// Decrease number of "fast" blocks. We will process last one in a separate branch.
			blocks_count--;
		}
		
		// Process all non-padded blocks in fast way, without padding validation.
		// If this sequence will contain padding then this will be treated as error.
		byte c[4];
		while (blocks_count > 0) {
			
			c[0] = s_dec_table[ block_4[0] ];
			c[1] = s_dec_table[ block_4[1] ];
			c[2] = s_dec_table[ block_4[2] ];
			c[3] = s_dec_table[ block_4[3] ];
			if (c[0] == 0xff || c[1] == 0xff ||
				c[2] == 0xff || c[3] == 0xff) {
				// wrong data
				return false;
			}
			
			out_data.push_back((c[0] << 2) | (c[1] >> 4));
			out_data.push_back((c[1] << 4) | (c[2] >> 2));
			out_data.push_back((c[2] << 6) |  c[3]);
			
			blocks_count--;
			block_4 += 4;
		}
		
		if (end_marker) {
			// Last block contains a padding marker and requires more checks for correct processing.
			c[0] = s_dec_table[ block_4[0] ];
			c[1] = s_dec_table[ block_4[1] ];
			if (c[0] == 0xff || c[1] == 0xff) {
				// wrong data...
				return false;
			}
			// First byte should be always decoded
			out_data.push_back((c[0] << 2) | (c[1] >> 4));
			
			if (block_4[2] == '=') {
				// Last two characters should be padding markers
				if (block_4[3] != '=') {
					// Wrong. Seqence like 'XY=Z'
					return false;
				}
				// the rest is correct, last byte is already decoded
				//
			} else if (block_4[3] == '=') {
				// Last char is padding marker, translate 3rd. character in the block
				c[2] = s_dec_table[ block_4[2] ];
				
				if (c[2] == 0xff) {
					// Last non-padded character is invalid. Sequence like 'XY?='
					return false;
				}
				// c3 is correct and last character is padding
				out_data.push_back((c[1] << 4) | (c[2] >> 2));
			} else {
				// This migh never happen. The 'end_marker' claims that the sequence
				// contains padding marker, but the deep inspection is telling something else.
				// Seems that we somehow processed less or more bytes as was planned.
				CC7_ASSERT(false, "Internal error.");
				return false;
			}
		}
		return true;
	}
	
	bool Base64_Decode(const std::string & string, size_t wrap_size, ByteArray & out_data)
	{
		bool result = false;
		out_data.clear();
		if (wrap_size > 0) {
			//
			// wrap impl.
			//
			if (utilities::AlignValue<4>(wrap_size) != wrap_size) {
				CC7_ASSERT(false, "wrap_size must be divisible by 4");
				return false;
			}

			// Calculate estimated data size (just to eliminate multiple reallocations in the data buffer)
			size_t size = string.size();
			size_t div = size / (wrap_size + 1);
			size_t rem = size % (wrap_size + 1);
			// subtract a number of possible new line characters
			size -= div + (rem > 0 ? 1 : 0);
			// final expected size of data
			size_t byte_size = ((size + 3) / 4) * 3;
			out_data.reserve(byte_size);
			
			// Current & End pointer
			const char * str_p   = string.c_str();
			const char * str_end = string.c_str() + string.length();
			result = true;
			
			bool end_marker = false;
			while (result && str_p < str_end) {
				// Find begin of the line, by skipping leading whitespaces
				while (str_p < str_end) {
					char c = *str_p;
					if (!isspace(c)) {
						break;
					}
					str_p++;
				}
				// Find end of the line, by skipping non-whitespace characters
				const char * line_begin = str_p;
				while (str_p < str_end) {
					char c = *str_p;
					if (isspace(c)) {
						break;
					}
					str_p++;
				}
				const char * line_end = str_p;
				size_t line_length = line_end - line_begin;
				if (line_length > 0) {
					// There's some sequence of non-space characters.
					if (end_marker) {
						// previous line did end with end-marker. If there's a next line, then this is an error.
						out_data.clear();
						return false;
					}
					// The rest of the decoding is handled in the "NoWrap" routine.
					result = Base64_DecodeNoWrap(string, line_begin - string.c_str(), line_length, out_data, end_marker);
				}
			}
			//
		} else {
			//
			// no wrap impl.
			//
			bool foo;
			result = Base64_DecodeNoWrap(string, 0, string.length(), out_data, foo);
		}
		if (!result) {
			out_data.clear();
		}
		return result;
	}

} // cc7
