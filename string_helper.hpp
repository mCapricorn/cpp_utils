/*
* Author: LowBoyTeam (https://github.com/LowBoyTeam)
* License: Code Project Open License
* Disclaimer: The software is provided "as-is". No claim of suitability, guarantee, or any warranty whatsoever is provided.
* Copyright (c) 2016-2017.
*/

#ifndef __STRINGEX_HPP_INCLUDED_
#define __STRINGEX_HPP_INCLUDED_

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

	inline std::vector<std::string> split(const std::string &str, const std::string &delim, const bool trim_empty = false)
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

	inline std::string join(const std::vector<std::string> &tokens, const std::string &delim, const bool trim_empty = false)
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

	inline std::string trim(const std::string &str)
	{
		std::string blank = "\r\n\t ";
		size_t begin = str.size(), end = 0;
		for (size_t i = 0; i < str.size(); ++i) {
			if (blank.find(str[i]) == std::string::npos) {
				begin = i;
				break;
			}
		}
		for (size_t i = str.size(); i > 0; --i) {
			if (blank.find(str[i - 1]) == std::string::npos) {
				end = i - 1;
				break;
			}
		}
		if (begin >= end) {
			return "";
		}
		else {
			return str.substr(begin, end - begin + 1);
		}
	}

	inline std::string repeat(const std::string &str, unsigned int times)
	{
		std::stringstream ss;
		for (unsigned int i = 0; i < times; ++i) {
			ss << str;
		}
		return ss.str();
	}

	inline std::string Toupper(const std::string &str)
	{
		std::string s(str);
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		return s;
	}

	inline std::string Tolower(const std::string &str)
	{
		std::string s(str);
		std::transform(s.begin(), s.end(), s.begin(), tolower);
		return s;
	}

	inline std::string readfile(const std::string &filepath)
	{
		std::ifstream ifs(filepath.c_str());
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		ifs.close();
		return content;
	}

	inline bool writefile(const std::string &filepath, const std::string &content) {
		std::ofstream examplefile(filepath.c_str());
		if (examplefile.is_open()) {
			examplefile << content;
			examplefile.close();
			return true;
		}
		return false;
	}

	inline std::string replace(const std::string &str, const std::string &strsrc, const std::string &strdst)
	{
		std::string s(str);
		std::string::size_type pos = 0;
		std::string::size_type srclen = strsrc.size();
		std::string::size_type dstlen = strdst.size();
		while ((pos = s.find(strsrc, pos)) != std::string::npos)
		{
			s.replace(pos, srclen, strdst);
			pos += dstlen;
		}
		return s;
	}

	inline std::string replace_all(const std::string &source, const std::string &target, const std::string &replacement) {
		return join(split(source, target, false), replacement, false);
	}

	inline std::string between(const std::string &str, const std::string &left, const std::string &right) {
		size_t left_pos, right_pos, last_pos = 0;
		left_pos = str.find(left, last_pos);
		if (left_pos == std::string::npos)
			return "";
		last_pos = left_pos + left.size();
		right_pos = str.find(right, last_pos);
		if (right_pos == std::string::npos)
			return "";
		return str.substr(left_pos + left.size(), right_pos - left_pos - left.size());
	}

	inline std::vector<std::string> between_array(const std::string &str, const std::string &left, const std::string &right, const bool trim_empty = false)
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

	inline std::string to_hex(const std::string &str)
	{
		std::ostringstream oss;
		oss << std::hex;
		oss << std::setfill('0');
		oss << std::uppercase;
		for (size_t i = 0; i < str.length(); i++)
		{
			unsigned char c = str[i];
			oss << "0x" << std::setw(2) << (unsigned int)c;
			if (i < (str.length() - 1))
				oss << ' ';
		}
		return oss.str();
	}

	inline std::string left(const std::string &str, const std::string &left)
	{
		std::string s(str);
		size_t pos, last_pos = 0;
		pos = s.find(left, last_pos);
		if (pos != std::string::npos)
			return s.substr(0, pos);
		else
			return "";
	}

	inline std::string right(const std::string &str, const std::string &right)
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
			return "";
	}

	inline bool is_start_with(const std::string &str, const std::string &src)
	{
		return str.compare(0, src.length(), src) == 0;
	}

	inline bool is_end_with(const std::string &str, const std::string &src)
	{
		return str.compare(str.length() - src.length(), src.length(), src) == 0;
	}
}

#endif // __STRINGEX_HPP_INCLUDED_
