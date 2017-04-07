/*
* Author: LowBoyTeam (https://github.com/LowBoyTeam)
* License: Code Project Open License
* Disclaimer: The software is provided "as-is". No claim of suitability, guarantee, or any warranty whatsoever is provided.
* Copyright (c) 2016-2017.
*/

#ifndef _ENCODER_HPP_INCLUDED_
#define _ENCODER_HPP_INCLUDED_

#include <windows.h>
#include <vector>

////////////////////////////////////////////////////////
// Classes and functions (typedefs) for text conversions
//
//  This section defines the following text conversions:
//  A2BSTR		ANSI  to BSTR
//  A2OLE		ANSI  to OLE
//	A2T			ANSI  to TCHAR
//	A2W			ANSI  to WCHAR
//  OLE2A		OLE   to ANSI
//  OLE2T		OLE   to TCHAR
//  OLE2W		OLE   to WCHAR
//  T2A			TCHAR to ANSI
//  T2BSTR		TCHAR to BSTR
//  T2OLE       TCHAR to OLE
//  T2W			TCHAR to WCHAR
//  W2A			WCHAR to ANSI
//  W2BSTR		WCHAR to BSTR
//  W2OLE		WCHAR to OLE
//  W2T			WCHAR to TCHAR

// About different character and string types:
// ------------------------------------------
// char (or CHAR) character types are ANSI (8 bits).
// wchar_t (or WCHAR) character types are Unicode (16 bits).
// TCHAR characters are Unicode if the _UNICODE macro is defined, otherwise they are ANSI.
// BSTR (Basic String) is a type of string used in Visual Basic and COM programming.
// OLE is the same as WCHAR. It is used in Visual Basic and COM programming.

namespace textconv_helper
{
	// Forward declarations of our classes. They are defined later.
	class CA2A;
	class CA2W;
	class CW2A;
	class CW2W;
	class CA2BSTR;
	class CW2BSTR;

	// typedefs for the well known text conversions
	typedef CA2W A2W;
	typedef CW2A W2A;
	typedef CW2BSTR W2BSTR;
	typedef CA2BSTR A2BSTR;
	typedef CW2A BSTR2A;
	typedef CW2W BSTR2W;

#ifdef _UNICODE
	typedef CA2W A2T;
	typedef CW2A T2A;
	typedef CW2W T2W;
	typedef CW2W W2T;
	typedef CW2BSTR T2BSTR;
	typedef BSTR2W BSTR2T;
#else
	typedef CA2A A2T;
	typedef CA2A T2A;
	typedef CA2W T2W;
	typedef CW2A W2T;
	typedef CA2BSTR T2BSTR;
	typedef BSTR2A BSTR2T;
#endif

	typedef A2W  A2OLE;
	typedef T2W  T2OLE;
	typedef CW2W W2OLE;
	typedef W2A  OLE2A;
	typedef W2T  OLE2T;
	typedef CW2W OLE2W;

	class CA2W
	{
	public:
		CA2W(LPCSTR pStr, UINT codePage = CP_ACP) : m_pStr(pStr)
		{
			if (pStr)
			{
				// Resize the vector and assign null WCHAR to each element
				int length = MultiByteToWideChar(codePage, 0, pStr, -1, NULL, 0) + 1;
				m_vWideArray.assign(length, L'\0');

				// Fill our vector with the converted WCHAR array
				MultiByteToWideChar(codePage, 0, pStr, -1, &m_vWideArray[0], length);
			}
		}
		~CA2W() {}
		operator LPCWSTR() { return m_pStr ? &m_vWideArray[0] : NULL; }
		operator LPOLESTR() { return m_pStr ? (LPOLESTR)&m_vWideArray[0] : (LPOLESTR)NULL; }

	private:
		CA2W(const CA2W&);
		CA2W& operator= (const CA2W&);
		std::vector<wchar_t> m_vWideArray;
		LPCSTR m_pStr;
	};

	class CW2A
	{
	public:
		CW2A(LPCWSTR pWStr, UINT codePage = CP_ACP) : m_pWStr(pWStr)
			// Usage:
			//   CW2A ansiString(L"Some Text");
			//   CW2A utf8String(L"Some Text", CP_UTF8);
			//
			// or
			//   SetWindowTextA( W2A(L"Some Text") ); The ANSI version of SetWindowText
		{
			// Resize the vector and assign null char to each element
			int length = WideCharToMultiByte(codePage, 0, pWStr, -1, NULL, 0, NULL, NULL) + 1;
			m_vAnsiArray.assign(length, '\0');

			// Fill our vector with the converted char array
			WideCharToMultiByte(codePage, 0, pWStr, -1, &m_vAnsiArray[0], length, NULL, NULL);
		}

		~CW2A()
		{
			m_pWStr = 0;
		}
		operator LPCSTR() { return m_pWStr ? &m_vAnsiArray[0] : NULL; }

	private:
		CW2A(const CW2A&);
		CW2A& operator= (const CW2A&);
		std::vector<char> m_vAnsiArray;
		LPCWSTR m_pWStr;
	};

	class CW2W
	{
	public:
		CW2W(LPCWSTR pWStr) : m_pWStr(pWStr) {}
		operator LPCWSTR() { return const_cast<LPWSTR>(m_pWStr); }
		operator LPOLESTR() { return const_cast<LPOLESTR>(m_pWStr); }

	private:
		CW2W(const CW2W&);
		CW2W& operator= (const CW2W&);

		LPCWSTR m_pWStr;
	};

	class CA2A
	{
	public:
		CA2A(LPCSTR pStr) : m_pStr(pStr) {}
		operator LPCSTR() { return (LPSTR)m_pStr; }

	private:
		CA2A(const CA2A&);
		CA2A& operator= (const CA2A&);

		LPCSTR m_pStr;
	};

	class CW2BSTR
	{
	public:
		CW2BSTR(LPCWSTR pWStr) { m_bstrString = ::SysAllocString(pWStr); }
		~CW2BSTR() { ::SysFreeString(m_bstrString); }
		operator BSTR() { return m_bstrString; }

	private:
		CW2BSTR(const CW2BSTR&);
		CW2BSTR& operator= (const CW2BSTR&);
		BSTR m_bstrString;
	};

	class CA2BSTR
	{
	public:
		CA2BSTR(LPCSTR pStr) { m_bstrString = ::SysAllocString(A2W(pStr)); }
		~CA2BSTR() { ::SysFreeString(m_bstrString); }
		operator BSTR() { return m_bstrString; }

	private:
		CA2BSTR(const CA2BSTR&);
		CA2BSTR& operator= (const CA2BSTR&);
		BSTR m_bstrString;
	};
}

#endif // _ENCODER_HPP_INCLUDED_