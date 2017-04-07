/*
* Author: LowBoyTeam (https://github.com/LowBoyTeam)
* License: Code Project Open License
* Disclaimer: The software is provided "as-is". No claim of suitability, guarantee, or any warranty whatsoever is provided.
* Copyright (c) 2016-2017.
*/

#ifndef _STRING_HELPER_HPP_INCLUDED_
#define _STRING_HELPER_HPP_INCLUDED_

#include <stdarg.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>

namespace string_helper
{
	inline std::string format(const char *fmt, ...)
	{
		std::string strResult = "";
		if (NULL != fmt)
		{
			va_list marker = NULL;
			va_start(marker, fmt);

			size_t nLength = _vscprintf(fmt, marker) + 1;
			std::vector<char> vBuffer(nLength, '\0');

			int nRet = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
			if (nRet > 0)
			{
				strResult = &vBuffer[0];

			}

			va_end(marker);
		}
		return strResult;
	}

	inline std::wstring format(const wchar_t *fmt, ...)
	{
		std::wstring strResult = L"";
		if (NULL != fmt)
		{
			va_list marker = NULL;
			va_start(marker, fmt);

			size_t nLength = _vscwprintf(fmt, marker) + 1;
			std::vector<wchar_t> vBuffer(nLength, L'\0');

			int nWritten = _vsnwprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
			if (nWritten > 0)
			{
				strResult = &vBuffer[0];
			}

			va_end(marker);
		}
		return strResult;
	}

	inline std::vector<std::string> compact(const std::vector<std::string> &tokens)
	{
		std::vector<std::string> compacted;
		for (size_t i = 0; i < tokens.size(); ++i) {
			if (!tokens[i].empty()) {
				compacted.push_back(tokens[i]);
			}
		}
		return compacted;
	}

	inline std::vector<std::wstring> compact(const std::vector<std::wstring> &tokens)
	{
		std::vector<std::wstring> compacted;
		for (size_t i = 0; i < tokens.size(); ++i) {
			if (!tokens[i].empty()) {
				compacted.push_back(tokens[i]);
			}
		}
		return compacted;
	}
	
	inline std::vector<std::string> split(const std::string& str, const std::string& delim, const bool trim_empty = false)
	{
		size_t pos, last_pos = 0, len;
		std::vector<std::string> tokens;
		while (true) {
			pos = str.find(delim, last_pos);
			if (pos == std::string::npos) {
				pos = str.size();
			}
			len = pos - last_pos;
			if (!trim_empty || len != 0) {
				tokens.push_back(str.substr(last_pos, len));
			}
			if (pos == str.size()) {
				break;
			}
			else {
				last_pos = pos + delim.size();
			}
		}
		return tokens;
	}

	inline std::vector<std::wstring> split(const std::wstring& str, const std::wstring& delim, const bool trim_empty = false)
	{
		size_t pos, last_pos = 0, len;
		std::vector<std::wstring> tokens;
		while (true) {
			pos = str.find(delim, last_pos);
			if (pos == std::wstring::npos) {
				pos = str.size();
			}
			len = pos - last_pos;
			if (!trim_empty || len != 0) {
				tokens.push_back(str.substr(last_pos, len));
			}
			if (pos == str.size()) {
				break;
			}
			else {
				last_pos = pos + delim.size();
			}
		}
		return tokens;
	}

	inline std::string join(const std::vector<std::string> &tokens,const std::string& delim, const bool trim_empty = false)
	{
		if (trim_empty) {
			return join(compact(tokens), delim, false);
		}
		else {
			std::stringstream ss;
			for (size_t i = 0; i < tokens.size() - 1; ++i) {
				ss << tokens[i] << delim;
			}
			ss << tokens[tokens.size() - 1];
			return ss.str();
		}
	}

	inline std::wstring join(const std::vector<std::wstring> &tokens, const std::wstring& delim, const bool trim_empty = false)
	{
		if (trim_empty) {
			return join(compact(tokens), delim, false);
		}
		else {
			std::wstringstream ss;
			for (size_t i = 0; i < tokens.size() - 1; ++i) {
				ss << tokens[i] << delim;
			}
			ss << tokens[tokens.size() - 1];
			return ss.str();
		}
	}
	
	inline std::string trim(const std::string& str)
	{
		if (str.empty()) {
			return str;
		}
		std::string sstr(str);
		std::string::iterator c;
		// Erase whitespace before the string  
		for (c = sstr.begin(); c != sstr.end() && iswspace(*c++);); sstr.erase(sstr.begin(), --c);
		// Erase whitespace after the string  
		for (c = sstr.end(); c != sstr.begin() && iswspace(*--c);); sstr.erase(++c, sstr.end());
		return sstr;
	}

	inline std::wstring trim(const std::wstring& str)
	{
		if (str.empty()) {
			return str;
		}
		std::wstring sstr(str);
		std::wstring::iterator c;
		// Erase whitespace before the string  
		for (c = sstr.begin(); c != sstr.end() && iswspace(*c++);); sstr.erase(sstr.begin(), --c);
		// Erase whitespace after the string  
		for (c = sstr.end(); c != sstr.begin() && iswspace(*--c);); sstr.erase(++c, sstr.end());
		return sstr;
	}
	
	inline std::string repeat(const std::string& str, unsigned int times)
	{
		std::stringstream ss;
		for (unsigned int i = 0; i < times; ++i) {
			ss << str;
		}
		return ss.str();
	}

	inline std::wstring repeat(const std::wstring& str, unsigned int times)
	{
		std::wstringstream ss;
		for (unsigned int i = 0; i < times; ++i) {
			ss << str;
		}
		return ss.str();
	}
	
	inline std::string Toupper(const std::string& str)
	{
		if (str.empty()) {
			return str;
		}
		std::string s(str);
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		return s;
	}

	inline std::wstring Toupper(const std::wstring& str)
	{
		if (str.empty()) {
			return str;
		}
		std::wstring s(str);
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		return s;
	}

	inline std::string Tolower(const std::string& str)
	{
		if (str.empty()) {
			return str;
		}
		std::string s(str);
		std::transform(s.begin(), s.end(), s.begin(), tolower);
		return s;
	}

	inline std::wstring Tolower(const std::wstring& str)
	{
		if (str.empty()) {
			return str;
		}
		std::wstring s(str);
		std::transform(s.begin(), s.end(), s.begin(), tolower);
		return s;
	}
	
	inline std::string replace(const std::string& source,const std::string& target,const std::string& replacement)
	{
		std::string s(source);
		std::string::size_type pos = 0;
		std::string::size_type srclen = target.size();
		std::string::size_type dstlen = replacement.size();
		while ((pos = s.find(target, pos)) != std::string::npos)
		{
			s.replace(pos, srclen, replacement);
			pos += dstlen;
		}
		return s;
	}

	inline std::wstring replace(const std::wstring& source, const std::wstring& target, const std::wstring& replacement)
	{
		std::wstring s(source);
		std::wstring::size_type pos = 0;
		std::wstring::size_type srclen = target.size();
		std::wstring::size_type dstlen = replacement.size();
		while ((pos = s.find(target, pos)) != std::wstring::npos)
		{
			s.replace(pos, srclen, replacement);
			pos += dstlen;
		}
		return s;
	}
	
	inline std::string between(const std::string& str, const std::string& left, const std::string& right) {
		size_t left_pos, right_pos, last_pos = 0;
		left_pos = str.find(left, last_pos);
		if (left_pos == std::string::npos)
			return std::string();
		last_pos = left_pos + left.size();
		right_pos = str.find(right, last_pos);
		if (right_pos == std::string::npos)
			return std::string();
		return str.substr(left_pos + left.size(), right_pos - left_pos - left.size());
	}

	inline std::wstring between(const std::wstring& str, const std::wstring& left, const std::wstring& right) {
		size_t left_pos, right_pos, last_pos = 0;
		left_pos = str.find(left, last_pos);
		if (left_pos == std::wstring::npos)
			return std::wstring();
		last_pos = left_pos + left.size();
		right_pos = str.find(right, last_pos);
		if (right_pos == std::string::npos)
			return std::wstring();
		return str.substr(left_pos + left.size(), right_pos - left_pos - left.size());
	}
	
	inline std::vector<std::string> between_array(const std::string& str, const std::string& left, const std::string&right, const bool trim_empty = false)
	{
		size_t left_pos, right_pos, last_pos = 0, len;
		std::vector<std::string> result;
		while (true) {
			left_pos = str.find(left, last_pos);
			if (left_pos == std::string::npos) {
				break;
			}
			else
			{
				last_pos = left_pos + left.size();
				right_pos = str.find(right, last_pos);
				if (right_pos == std::string::npos) {
					break;
				}
				len = right_pos - left_pos - left.size();
				if (len != 0 || !trim_empty)
				{
					result.push_back(str.substr(last_pos, len));
				}
			}
			last_pos = right_pos + right.size();
		}
		return result;
	}

	inline std::vector<std::wstring> between_array(const std::wstring& str, const std::wstring& left, const std::wstring&right, const bool trim_empty = false)
	{
		size_t left_pos, right_pos, last_pos = 0, len;
		std::vector<std::wstring> result;
		while (true) {
			left_pos = str.find(left, last_pos);
			if (left_pos == std::wstring::npos) {
				break;
			}
			else
			{
				last_pos = left_pos + left.size();
				right_pos = str.find(right, last_pos);
				if (right_pos == std::wstring::npos) {
					break;
				}
				len = right_pos - left_pos - left.size();
				if (len != 0 || !trim_empty)
				{
					result.push_back(str.substr(last_pos, len));
				}
			}
			last_pos = right_pos + right.size();
		}
		return result;
	}
	
	inline std::string left(const std::string& str, const std::string& left)
	{
		std::string s(str);
		size_t pos, last_pos = 0;
		pos = s.find(left, last_pos);
		if (pos != std::string::npos)
			return s.substr(0, pos);
		else
			return std::string();
	}

	inline std::wstring left(const std::wstring& str, const std::wstring& left)
	{
		std::wstring s(str);
		size_t pos, last_pos = 0;
		pos = s.find(left, last_pos);
		if (pos != std::wstring::npos)
			return s.substr(0, pos);
		else
			return std::wstring();
	}
	
	inline std::string right(const std::string& str, const std::string& right)
	{
		std::string s(str);
		size_t pos, last_pos = 0;
		pos = s.find(right, last_pos);
		if (pos != std::string::npos)
		{
			pos += right.length();
			return s.substr(pos, str.length() - pos);
		}
		else
			return std::string();
	}

	inline std::wstring right(const std::wstring& str, const std::wstring& right)
	{
		std::wstring s(str);
		size_t pos, last_pos = 0;
		pos = s.find(right, last_pos);
		if (pos != std::wstring::npos)
		{
			pos += right.length();
			return s.substr(pos, str.length() - pos);
		}
		else
			return std::wstring();
	}

	inline bool is_start_with(const std::string& str, const std::string& src)
	{
		return str.compare(0, src.length(), src) == 0;
	}

	inline bool is_start_with(const std::wstring& str, const std::wstring& src)
	{
		return str.compare(0, src.length(), src) == 0;
	}

	inline bool is_end_with(const std::string& str, const std::string& src)
	{
		return str.compare(str.length() - src.length(), src.length(), src) == 0;
	}

	inline bool is_end_with(const std::wstring& str, const std::wstring& src)
	{
		return str.compare(str.length() - src.length(), src.length(), src) == 0;
	}
}

#endif // _STRING_HELPER_HPP_INCLUDED_
