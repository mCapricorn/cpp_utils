/*
* Author: LowBoyTeam (https://github.com/LowBoyTeam)
* License: Code Project Open License
* Disclaimer: The software is provided "as-is". No claim of suitability, guarantee, or any warranty whatsoever is provided.
* Copyright (c) 2016-2017.
*/

#ifndef _URLENCODER_HPP_INCLUDED_
#define _URLENCODER_HPP_INCLUDED_

#include <string>
#include <vector>

namespace url_helper
{
	int _htoi(char *s)
	{
		int value;
		int c;

		c = ((unsigned char *)s)[0];
		if (isupper(c))
			c = tolower(c);
		value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

		c = ((unsigned char *)s)[1];
		if (isupper(c))
			c = tolower(c);
		value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

		return (value);
	}

	std::string url_decode(const std::string &str_source)
	{
		char const *in_str = str_source.c_str();
		int in_str_len = strlen(in_str);
		int out_str_len = 0;
		std::string out_str;
		char *str;

		str = _strdup(in_str);
		char *dest = str;
		char *data = str;

		while (in_str_len--) {
			if (*data == '+') {
				*dest = ' ';
			}
			else if (*data == '%' && in_str_len >= 2 && isxdigit((int) *(data + 1))
				&& isxdigit((int) *(data + 2))) {
				*dest = (char)_htoi(data + 1);
				data += 2;
				in_str_len -= 2;
			}
			else {
				*dest = *data;
			}
			data++;
			dest++;
		}
		*dest = '\0';
		out_str_len = dest - str;
		out_str = str;
		free(str);
		return out_str;
	}

	std::string url_encode(const std::string &str_source)
	{
		char const *in_str = str_source.c_str();
		int in_str_len = strlen(in_str);
		int out_str_len = 0;
		std::string out_str;
		register unsigned char c;
		unsigned char *to, *start;
		unsigned char const *from, *end;
		unsigned char hexchars[] = "0123456789ABCDEF";

		from = (unsigned char *)in_str;
		end = (unsigned char *)in_str + in_str_len;
		start = to = (unsigned char *)malloc(3 * in_str_len + 1);

		while (from < end) {
			c = *from++;

			if (c == ' ') {
				*to++ = '+';
			}
			else if ((c < '0' && c != '-' && c != '.') ||
				(c < 'A' && c > '9') ||
				(c > 'Z' && c < 'a' && c != '_') ||
				(c > 'z')) {
				to[0] = '%';
				to[1] = hexchars[c >> 4];
				to[2] = hexchars[c & 15];
				to += 3;
			}
			else {
				*to++ = c;
			}
		}
		*to = 0;

		out_str_len = to - start;
		out_str = (char *)start;
		free(start);
		return out_str;
	}
}

#endif // _URLENCODER_HPP_INCLUDED_