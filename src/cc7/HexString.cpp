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

#include <cc7/HexString.h>

namespace cc7
{
	// MARK: Encoder -
	
	static const char s_hex_table_uc[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	static const char s_hex_table_lc[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	
	bool HexString_Encode(const ByteRange & in_data, bool use_lowercase, std::string & out_string)
	{
		const char * table = use_lowercase ? s_hex_table_lc : s_hex_table_uc;
		
		out_string.reserve(in_data.size() << 1);
		auto data_it  = in_data.cbegin();
		auto data_end = in_data.cend();
		byte val;
		while (data_it != data_end) {
			val = *data_it;
			out_string.push_back(table[val >> 4]);
			out_string.push_back(table[val & 15]);
			data_it++;
		}
		return true;
	}
	
	// MARK: Decoder -
	
	bool HexString_Decode(const std::string & in_string, ByteArray & out_data)
	{
		size_t str_len = in_string.length();
		
		// Reserve buffer for data
		out_data.reserve((str_len >> 1) + (str_len & 1));
		
		const char * str_p = in_string.c_str();
		char lc, uc;
		byte lv, uv;
		if (str_len & 1) {
			// odd number of hexadecimal characters
			lc = *str_p++;
			if (lc >= '0' && lc <= '9') {
				lv = lc - '0';
			} else if (lc >= 'A' && lc <= 'F') {
				lv = lc - 'A' + 10;
			} else if (lc >= 'a' && lc <= 'f') {
				lv = lc - 'a' + 10;
			} else {
				// failure
				out_data.clear();
				return false;
			}
			out_data.push_back(lv);
			str_len--;
		}
		
		while (str_len >= 2) {
			uc = str_p[0];
			if (uc >= '0' && uc <= '9') {
				uv = uc - '0';
			} else if (uc >= 'A' && uc <= 'F') {
				uv = uc - 'A' + 10;
			} else if (uc >= 'a' && uc <= 'f') {
				uv = uc - 'a' + 10;
			} else {
				// failure
				out_data.clear();
				return false;
			}
			
			lc = str_p[1];
			if (lc >= '0' && lc <= '9') {
				lv = lc - '0';
			} else if (lc >= 'A' && lc <= 'F') {
				lv = lc - 'A' + 10;
			} else if (lc >= 'a' && lc <= 'f') {
				lv = lc - 'a' + 10;
			} else {
				// failure
				out_data.clear();
				return false;
			}
			out_data.push_back((uv << 4) | lv);
			str_p	+= 2;
			str_len -= 2;
		}
		// success
		return true;
	}

}