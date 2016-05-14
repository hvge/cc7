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

#include <cc7tests/JSONReader.h>
#include <cc7tests/detail/TestUtilities.h>
#include <ctype.h>

namespace cc7
{
namespace tests
{
	struct JSONParserContext
	{
		const char * ptr;		// Pointer to the input string
		size_t	offset;
		size_t	length;
		size_t	line;
		size_t	lineBegin;
		
		int		stack;
		int		stackLimit;
		
		int		options;
		
		bool	unexpectedEndOfStream;
		char	consumedSeparator;
		
		std::string error;
		
		JSONParserContext(const std::string & str) :
			ptr(str.c_str()),
			offset(0),
			length(str.length()),
			line(0),
			lineBegin(0),
			stack(0),
			stackLimit(16),
			options(0),
			unexpectedEndOfStream(false),
			consumedSeparator(0)
		{
		}
	};
	
	
	struct NumberInfo
	{
		int		begin;
		int		end;
		int		exponent;
	};
	
	
	//
	// MARK: helper functions -
	//
	
	
	static void _SetParserError(JSONParserContext * ctx, const char * reason)
	{
		if (ctx->unexpectedEndOfStream || !reason) {
			reason = "Unexpected end of stream";
		}
		size_t line   = ctx->line + 1;
		size_t offset = ctx->offset - ctx->lineBegin + 1 - 1; // -1 due to offset is always one character forward...
		ctx->error  = detail::FormattedString("JSON parser error: %s (line %d, offset %d)", reason, line, offset);
	}
		
	
	// MARK: Stream reading
	
	static inline bool _IsEnd(JSONParserContext * ctx)
	{
		return (ctx->offset >= ctx->length) || !ctx->error.empty();
	}
	
	static inline const char * _Ptr(JSONParserContext * ctx)
	{
		return ctx->ptr + ctx->offset;
	}
	
	static inline const char * _ShouldReadPtr(JSONParserContext * ctx, size_t requiredSize)
	{
		if ((ctx->length - ctx->offset) >= requiredSize) {
			return _Ptr(ctx);
		}
		ctx->unexpectedEndOfStream = true;
		return nullptr;
	}
	
	static inline void _SkipCount(JSONParserContext * ctx, int count)
	{
		ctx->offset += count;
	}
	
	static char _SkipWhitespace(JSONParserContext * ctx)
	{
		if (_IsEnd(ctx)) {
			return 0;
		}
		
		do
		{
			char uc = ctx->ptr[ctx->offset++];
			if (!isspace(uc)) {
				return uc;
			}
			if (uc == '\n') {
				ctx->line++;
				ctx->lineBegin = ctx->offset - 1;
			}
			
		} while (ctx->offset < ctx->length);
		
		return 0;
	}
	
	
	// MARK: Stack & Other
	
	static inline bool _PushStack(JSONParserContext * ctx)
	{
		ctx->stack++;
		if (ctx->stack < ctx->stackLimit) {
			return true;
		}
		_SetParserError(ctx, "The processing stack is too deep");
		return false;
	}
	
	static inline bool _PopStack(JSONParserContext * ctx)
	{
		ctx->stack--;
		if (ctx->stack >= 0) {
			return true;
		}
		_SetParserError(ctx, "The processing stack underflow.");
		return false;
	}
	
	
	// MARK: Forward declarations -
	
	static JSONValue _ParseValue  (JSONParserContext * ctx, const char * allowedSeparators);
	static JSONValue _ParseObject (JSONParserContext * ctx);
	static JSONValue _ParseNumber (JSONParserContext * ctx);
	static JSONValue _ParseArray  (JSONParserContext * ctx);
	static JSONValue _ParseString (JSONParserContext * ctx);
	static bool _ParseEscapedCharacter(JSONParserContext * ctx, std::string & result);
	
	// MARK: Parser -
	
	//
	// Parse value
	//
	
	static JSONValue _ParseValue(JSONParserContext * ctx, const char * allowedSeparators)
	{
		ctx->consumedSeparator = 0;
		
		char uc = _SkipWhitespace(ctx);
		if (!uc) {
			// regular end
			return JSONValue();
		}
		if (uc == '"') {
			//
			// string
			//
			return _ParseString(ctx);
			//
		} else if (uc == '{') {
			//
			// object
			//
			return _ParseObject(ctx);
			//
		} else if (uc == '[') {
			//
			// array
			//
			return _ParseArray(ctx);
			//
		} else if (uc == '-' || (uc >= '0' && uc <= '9')) {
			//
			// number
			//
			return _ParseNumber(ctx);
			//
		} else if (uc == 't') {
			//
			// true
			//
			const char * ptr = _ShouldReadPtr(ctx, 3);
			if (ptr && ptr[0] == 'r' && ptr[1] == 'u' && ptr[2] == 'e') {
				_SkipCount(ctx, 3);
				return JSONValue(true);
			} else {
				_SetParserError(ctx, "'true' token is expected");
			}
			//
		} else if (uc == 'f') {
			//
			// false
			//
			const char * ptr = _ShouldReadPtr(ctx, 4);
			if (ptr && ptr[0] == 'a' && ptr[1] == 'l' && ptr[2] == 's' && ptr[3] == 'e') {
				_SkipCount(ctx, 4);
				return JSONValue(false);
			} else {
				_SetParserError(ctx, "'false' token is expected");
			}
		} else if (uc == 'n') {
			//
			// null
			//
			const char * ptr = _ShouldReadPtr(ctx, 3);
			if (ptr && ptr[0] == 'u' && ptr[1] == 'l' && ptr[2] == 'l') {
				_SkipCount(ctx, 3);
				return JSONValue(JSONValue::Null);
			} else {
				_SetParserError(ctx, "'null' token is expected");
			}
		} else {
			if (allowedSeparators) {
				const char * separatorPtr = allowedSeparators;
				while (*separatorPtr) {
					if (*separatorPtr++ == uc) {
						// returns NaT but without error
						ctx->consumedSeparator = uc;
						return JSONValue();
					}
				}
			}
			_SetParserError(ctx, "Unexpected character in value");
		}
		
		return JSONValue();
	}
	
	
	//
	// Parse array
	//
	
	static JSONValue _ParseArray(JSONParserContext * ctx)
	{
		if (!_PushStack(ctx)) {
			return JSONValue();
		}
		
		bool error = false;
		JSONValue array(JSONValue::Array);
		auto & result = array.asMutableArray();
		
		// Process values in array
		static const char separator[3] = { ',', ']', 0 };
		char uc;
		while (1)
		{
			JSONValue value = _ParseValue(ctx, separator + 1);
			if (value.isValid()) {
				result.push_back(value);
			} else {
				if (ctx->consumedSeparator != ']') {
					// Consumed separator must be ']'. This is error, clear result and break loop.
					error = true;
				}
				break;
			}
			
			uc = _SkipWhitespace(ctx);
			if (uc == ',') {
				// Move to next value
				continue;
				
			} else if (uc == ']') {
				// End of array, break loop
				break;
				
			} else {
				if (uc != 0) {
					_SetParserError(ctx, "Wrong character in array. Characters ']' or ',' are expected");
				} else {
					_SetParserError(ctx, "Unexpected end of array");
				}
				error = true;
			}
			break;
			
		}
		
		_PopStack(ctx);
		
		if (!error) {
			return array;
		}
		return JSONValue();
	}
	
	//
	// Parse object
	//
	static JSONValue _ParseObject (JSONParserContext * ctx)
	{
		if (!_PushStack(ctx)) {
			return JSONValue();
		}
		
		bool error = false;
		JSONValue object(JSONValue::Object);
		auto & result = object.asMutableObject();
		
		char uc;
		while (1)
		{
			uc = _SkipWhitespace(ctx);
			// '"' or '}' is expected
			if (uc == '"') {
				// Read key
				JSONValue key = _ParseString(ctx);
				if (!key.isType(JSONValue::String)) {
					error = true;
					break;
				}
				// Look for colon
				uc = _SkipWhitespace(ctx);
				if (uc != ':') {
					if (uc != 0) {
						_SetParserError(ctx, "The colon ':' is expected as key-value separator");
					} else {
						_SetParserError(ctx, "Unexpected end of object");
					}
					error = true;
					break;
				}
				// Read value
				JSONValue value = _ParseValue(ctx, NULL);
				if (value.isValid()) {
					// store key - value pair
					result[key.asString()] = value;
				} else {
					// something is wrong, break loop.
					// error is already set
					error = true;
					break;
				}
				// Look for ',' or '}'
				uc = _SkipWhitespace(ctx);
				if (uc == ',') {
					// next item in array
					continue;
					
				}
			}
			
			if (uc == '}') {
				// Success, end of object
				break;
			}
			
			if (uc != 0) {
				_SetParserError(ctx, "Unknown character in object");
			} else {
				_SetParserError(ctx, "Unexpected end of object");
			}
			error = true;
			break;
		}
		
		_PopStack(ctx);
		
		if (!error) {
			return object;
		}
		return JSONValue();
	}
	
	//
	// Parse string
	//
	
	static JSONValue _ParseString (JSONParserContext * ctx)
	{
		if (_IsEnd(ctx)) {
			_SetParserError(ctx, "Unexpected end of string");
			return JSONValue();
		}
		
		bool error = false;
		JSONValue result(JSONValue::String);
		auto & result_str = result.asMutableString();
		
		char uc;
		size_t range_location = ctx->offset;
		size_t range_length   = 0;
		while (ctx->offset < ctx->length)
		{
			uc = ctx->ptr[ctx->offset++];
			
			if (uc == '"') {
				//
				// end of string
				//
				if (range_length > 0) {
					// flush previously captured string fragment
					result_str.append(ctx->ptr + range_location, range_length);
					range_length = 0;
				}
				break;
				
			} else if (uc == '\\') {
				//
				// escaped character
				//
				if (range_length > 0) {
					// flush previously captured string fragment
					result_str.append(ctx->ptr + range_location, range_length);
					range_length = 0;
				}
				error = _ParseEscapedCharacter(ctx, result_str);
				if (error) {
					break;
				}
				// Valid escaped character. Keep start for new fragment
				range_location = ctx->offset;
				continue;
				
				
			} else if (uc < 32) {
				_SetParserError(ctx, "Unexpected control character in string");
				error = true;
				break;
			}
			
			//
			// valid characted, just increase length in capturing range
			//
			range_length++;
		}
		
		if (range_length > 0) {
			_SetParserError(ctx, "Unexpected end of string");
			error = true;
		}
		if (!error) {
			return result;
		}
		return JSONValue();
	}
	
	static inline bool _Hex2Char(const char * p, char & out)
	{
		int b1, b2;
		if (p[0] >= '0' && p[0] <= '9') {
			b1 = p[0] - '0';
		} else if (p[0] >= 'A' && p[0] <= 'F') {
			b1 = p[0] - 'A' + 16;
		} else if (p[0] >= 'a' && p[0] <= 'f') {
			b1 = p[0] - 'a' + 16;
		} else {
			return false;
		}
		if (p[1] >= '0' && p[1] <= '9') {
			b2 = p[1] - '0';
		} else if (p[1] >= 'A' && p[1] <= 'F') {
			b2 = p[1] - 'A' + 16;
		} else if (p[1] >= 'a' && p[1] <= 'f') {
			b2 = p[1] - 'a' + 16;
		} else {
			return false;
		}
		out = (b1 << 4) | b2;
		return true;
	}
	
	static bool _ParseEscapedCharacter(JSONParserContext * ctx, std::string & result)
	{
		//
		// offset points after backslash
		//
		const char * ucptr = _ShouldReadPtr(ctx, 1);
		if (!ucptr) {
			_SetParserError(ctx, "Unexpected end of string");
			return true;
		}
		
		static const char escaped[5] =
		{
			'\n', '\r', '\t', '\b', '\f'
		};
		
		size_t consumed = 1;
		switch (ucptr[0])
		{
			case '"':
			case '/':
			case '\\':
				// quote, slash or backslash
				result.append(ucptr, 1);
				break;
			case 'n':
				// newline
				result.append(escaped + 0, 1);
				break;
			case 'r':
				// carriage return
				result.append(escaped + 1, 1);
				break;
			case 't':
				// horizontal tab
				result.append(escaped + 2, 1);
				break;
			case 'b':
				// backspace
				result.append(escaped + 3, 1);
				break;
			case 'f':
				// formfeed
				result.append(escaped + 4, 1);
				break;
			case 'u':
				// unicode character
				if (!_ShouldReadPtr(ctx, 5)) {
					return true;
				}
				// read 4 hexadecimal numbers
				char uc_bytes[2];
				if (!_Hex2Char(ucptr + 1, uc_bytes[1]) || !_Hex2Char(ucptr + 3, uc_bytes[0])) {
					_SetParserError(ctx, "Wrong hexadecimal value in escaped unicode character");
					return true;
				}
				_SetParserError(ctx, "Unicode escaping in string is not supported yet.");
				return true;
//				consumed = 5;
//				break;
				
			default:
				_SetParserError(ctx, "Wrong escaped character in string");
				return true;
		}
		// success
		ctx->offset += consumed;
		return false;
	}
	
	//
	// Parse number
	//
	
	static JSONValue _ParseNumber(JSONParserContext * ctx)
	{
		_SetParserError(ctx, "JSON numbers are not supported yet.");
		return JSONValue();
	}
	
	
	//
	// MARK: Reader implementation
	//
	
	bool JSON_ReadString(const std::string & str, JSONValue & out_value, std::string * out_error)
	{
		JSONParserContext ctx(str);
		out_value = _ParseValue(&ctx, nullptr);
		if (out_value.isValid() && ctx.error.empty()) {
			return true;
		}
		if (out_error) {
			out_error->assign(ctx.error);
		}
		return false;
	}
	

	
} // cc7::tests
} // cc7
