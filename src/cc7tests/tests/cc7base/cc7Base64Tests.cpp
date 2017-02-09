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

#include <cc7tests/CC7Tests.h>
#include <cc7/CC7.h>

namespace cc7
{
namespace tests
{
	class cc7Base64Tests : public UnitTest
	{
	public:
		cc7Base64Tests()
		{
			CC7_REGISTER_TEST_METHOD(testEncodeDecode);
			CC7_REGISTER_TEST_METHOD(testNoWrap);
			CC7_REGISTER_TEST_METHOD(testNoWrapBadData);
			CC7_REGISTER_TEST_METHOD(testWrap);
			CC7_REGISTER_TEST_METHOD(testWrapBadData);
		}
		
		// UNIT TESTS
		
		void testEncodeDecode()
		{
			// Good scenarios
			ByteArray max_data = getTestRandomData(1025);
			bool result;
			for (size_t test_size = 0; test_size < max_data.size(); test_size++) {
				
				ByteRange source_data = max_data.byteRange().subRangeTo(test_size);
				std::string plain, padded64, padded76;
				result = Base64_Encode(source_data, 0, plain);
				ccstAssertTrue(result);
				if (!result) return;
				result = Base64_Encode(source_data, 64, padded64);
				ccstAssertTrue(result);
				if (!result) return;
				result = Base64_Encode(source_data, 76, padded76);
				ccstAssertTrue(result);
				if (!result) return;
				
				ByteArray plain_dec, padded64_dec, padded76_dec;
				result = Base64_Decode(plain, 0, plain_dec);
				ccstAssertTrue(result);
				if (!result) return;
				result = plain_dec == source_data;
				ccstAssertTrue(result);
				if (!result) return;
				
				result = Base64_Decode(padded64, 64, padded64_dec);
				ccstAssertTrue(result);
				if (!result) return;
				result = padded64_dec == source_data;
				ccstAssertTrue(result);
				if (!result) return;
				
				result = Base64_Decode(padded76, 76, padded76_dec);
				ccstAssertTrue(result);
				if (!result) return;
				result = padded76_dec == source_data;
				ccstAssertTrue(result);
				if (!result) return;
			}
		}
		
		void testNoWrap()
		{
			bool result;
			
			// Fixed scenarios
			std::string str1("SGVsbG8gd29ybGQ=");
			std::string str2("SGVsbG8gd29yZA==");
			
			ByteArray out;
			result = Base64_Decode(str1, 0, out);
			ccstAssertTrue(result);
			std::string out_str = CopyToString(out);
			ccstAssertEqual(out_str, "Hello world");
			
			result = Base64_Decode(str2, 0, out);
			ccstAssertTrue(result);
			out_str = CopyToString(out);
			ccstAssertEqual(out_str, "Hello word");
		}
		
		void testNoWrapBadData()
		{
			bool result;
			ByteArray out;
			
			// Wrong scenarios ... must fail
			std::string str3("SGVsbG8gd29ybGQ");
			std::string str4("SGVsbG8gd29yZA=");
			std::string str5("SGVs_G8gd29ybGQ=");
			std::string str6("SGVsbG8gd29y?A==");
			std::string str7("SGVsbG8gd29ybA=X");
			std::string str8("SGVsbG8gd29yb===");
			std::string str9("SGVsbG8gd29y====");
			std::string strA("SGV=bG8gd29ybGQ=");
			std::string strB("SGVsbG8gd29yZA==\n");
			std::string strC(" SGVsbG8gd29yZA==");
			
			result = Base64_Decode(str3, 0, out);
			ccstAssertFalse(result);
			result = Base64_Decode(str4, 0, out);
			ccstAssertFalse(result);
			result = Base64_Decode(str5, 0, out);
			ccstAssertFalse(result);
			result = Base64_Decode(str6, 0, out);
			ccstAssertFalse(result);
			result = Base64_Decode(str7, 0, out);
			ccstAssertFalse(result);
			result = Base64_Decode(str8, 0, out);
			ccstAssertFalse(result);
			result = Base64_Decode(str9, 0, out);
			ccstAssertFalse(result);
			result = Base64_Decode(strA, 0, out);
			ccstAssertFalse(result);
			result = Base64_Decode(strB, 0, out);
			ccstAssertFalse(result);
			result = Base64_Decode(strC, 0, out);
			ccstAssertFalse(result);
			
			// Check if internal decoder table is correct
			const char * valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
			std::string valid("SGVs");
			for (int i = 0; i <= 0xff; i++) {
				// inject wrong character...
				std::string wrong = valid;
				if (strchr(valid_chars, i) != NULL) {
					// this is valid character
					continue;
				}
				byte uc = i & 0xff;
				wrong[ 0 ] = uc;
				bool result = Base64_Decode(wrong, 0, out);
				ccstAssertFalse(result);
				wrong[ 0 ] = 'x';
				wrong[ 1 ] = uc;
				result = Base64_Decode(wrong, 0, out);
				ccstAssertFalse(result);
				wrong[ 1 ] = 'x';
				wrong[ 2 ] = uc;
				result = Base64_Decode(wrong, 0, out);
				ccstAssertFalse(result);
				if (uc == '=') {
					// Next validation makes no sense if injected char is padding marker
					continue;
				}
				wrong[ 2 ] = 'x';
				wrong[ 3 ] = uc;
				result = Base64_Decode(wrong, 0, out);
				ccstAssertFalse(result);
			}
		}
		
		void testWrap()
		{
			std::string input("TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2Np\n"
							   "bmcgZWxpdCwgc2VkIGRvIGVpdXNtb2QgdGVtcG9yIGluY2lkaWR1bnQgdXQgbGFi\n"
							   "b3JlIGV0IGRvbG9yZSBtYWduYSBhbGlxdWEuIFV0IGVuaW0gYWQgbWluaW0gdmVu\n"
							   "aWFtLCBxdWlzIG5vc3RydWQgZXhlcmNpdGF0aW9uIHVsbGFtY28gbGFib3JpcyBu\n"
							   "aXNpIHV0IGFsaXF1aXAgZXggZWEgY29tbW9kbyBjb25zZXF1YXQuIER1aXMgYXV0\n"
							   "ZSBpcnVyZSBkb2xvciBpbiByZXByZWhlbmRlcml0IGluIHZvbHVwdGF0ZSB2ZWxp\n"
							   "dCBlc3NlIGNpbGx1bSBkb2xvcmUgZXUgZnVnaWF0IG51bGxhIHBhcmlhdHVyLiBF\n"
							   "eGNlcHRldXIgc2ludCBvY2NhZWNhdCBjdXBpZGF0YXQgbm9uIHByb2lkZW50LCBz\n"
							   "dW50IGluIGN1bHBhIHF1aSBvZmZpY2lhIGRlc2VydW50IG1vbGxpdCBhbmltIGlk\n"
							   "IGVzdCBsYWJvcnVtLg==");
			const char * expected_output =  "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do"
											" eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut"
											" enim ad minim veniam, quis nostrud exercitation ullamco laboris"
											" nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor"
											" in reprehenderit in voluptate velit esse cillum dolore eu"
											" fugiat nulla pariatur. Excepteur sint occaecat cupidatat non"
											" proident, sunt in culpa qui officia deserunt mollit anim id"
											" est laborum.";
			ByteArray output_data;
			bool result = Base64_Decode(input, 64, output_data);
			ccstAssertTrue(result);
			std::string output = CopyToString(output_data);
			ccstAssertEqual(expected_output, output);
			
			input.insert(0, "\n");
			input.append("\n");
			result = Base64_Decode(input, 64, output_data);
			ccstAssertTrue(result);
			output = CopyToString(output_data);
			ccstAssertEqual(expected_output, output);
			
			input = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2Np";
			result = Base64_Decode(input, 64, output_data);
			ccstAssertTrue(result);
			output = CopyToString(output_data);
			ccstAssertEqual("Lorem ipsum dolor sit amet, consectetur adipisci", output);
			
			input = "                            TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2Np                         ";
			result = Base64_Decode(input, 64, output_data);
			ccstAssertTrue(result);
			output = CopyToString(output_data);
			ccstAssertEqual("Lorem ipsum dolor sit amet, consectetur adipisci", output);

			input = "\n\n  \nTG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2Np\n\n  \n\n\n";
			result = Base64_Decode(input, 64, output_data);
			ccstAssertTrue(result);
			output = CopyToString(output_data);
			ccstAssertEqual("Lorem ipsum dolor sit amet, consectetur adipisci", output);
			
			input = "\nTG9y\nZW0g\naXBz\ndW0g\nZG9s\nb3Ig\nc2l0\nIGFt\nZXQs\nIGNv\nbnNl\nY3Rl\ndHVy\nIGFk\naXBp\nc2Np\n\n";
			result = Base64_Decode(input, 64, output_data);
			ccstAssertTrue(result);
			output = CopyToString(output_data);
			ccstAssertEqual("Lorem ipsum dolor sit amet, consectetur adipisci", output);
			
			// various non empty strings leading to empty data
			input = "                                    ";
			result = Base64_Decode(input, 64, output_data);
			ccstAssertTrue(result);
			ccstAssertEqual(output_data.size(), 0);
			input = " ";
			result = Base64_Decode(input, 64, output_data);
			ccstAssertTrue(result);
			ccstAssertEqual(output_data.size(), 0);
			input = "  ";
			result = Base64_Decode(input, 64, output_data);
			ccstAssertTrue(result);
			ccstAssertEqual(output_data.size(), 0);
			input = "";
			result = Base64_Decode(input, 64, output_data);
			ccstAssertTrue(result);
			ccstAssertEqual(output_data.size(), 0);
		}
		
		void testWrapBadData()
		{
			std::string input;
			ByteArray output_data;
			bool result;
			
			input = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2N\n"
					"bmcgZWxpdCwgc2VkIGRvIGVpdXNtb2QgdGVtcG9yIGluY2lkaWR1bnQgdXQgbG==\n";
			result = Base64_Decode(input, 64, output_data);
			ccstAssertFalse(result);
			
			input = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2N=\n"
					"bmcgZWxpdCwgc2VkIGRvIGVpdXNtb2QgdGVtcG9yIGluY2lkaWR1bnQgdXQgbG==\n";
			result = Base64_Decode(input, 64, output_data);
			ccstAssertFalse(result);
		}
	};
	
	CC7_CREATE_UNIT_TEST(cc7Base64Tests, "cc7")
	
} // cc7::tests
} // cc7
