/*
* Author: LowBoyTeam (https://github.com/LowBoyTeam)
* License: Code Project Open License
* Disclaimer: The software is provided "as-is". No claim of suitability, guarantee, or any warranty whatsoever is provided.
* Copyright (c) 2016-2017.
*/

#ifndef filefinder_helper_h__
#define filefinder_helper_h__

#include <Windows.h>
#include <assert.h>

class filefinder_helper
{
public:
	// Data members
	WIN32_FIND_DATAW m_fd;
	WCHAR m_lpszRoot[MAX_PATH];
	WCHAR m_chDirSeparator;
	HANDLE m_hFind;
	BOOL m_bFound;

	// Constructor/destructor
	filefinder_helper() : m_hFind(NULL), m_chDirSeparator(L'\\'), m_bFound(FALSE) {};

	~filefinder_helper() { Close(); };

	BOOL GetFileName(LPWSTR lpstrFileName, int cchLength) const 
	{
		assert(m_hFind != NULL);
		if (lstrlenW(m_fd.cFileName) >= cchLength)
			return FALSE;

		if (m_bFound)
			lstrcpynW(lpstrFileName, m_fd.cFileName, cchLength);

		return m_bFound;
	}

	BOOL GetFilePath(LPWSTR lpstrFilePath, int cchLength) const
	{
		assert(m_hFind != NULL);

		int nLen = lstrlenW(m_lpszRoot);

		assert(nLen > 0);
		if (nLen == 0)
			return FALSE;

		bool bAddSep = (m_lpszRoot[nLen - 1] != L'\\' && m_lpszRoot[nLen - 1] != L'/');

		if ((lstrlenW(m_lpszRoot) + (bAddSep ? 1 : 0)) >= cchLength)
			return FALSE;

		lstrcpyW(lpstrFilePath, const_cast<WCHAR*>(m_lpszRoot));

		if (bAddSep)
		{
			WCHAR szSeparator[2] = { m_chDirSeparator, 0 };
			lstrcatW(lpstrFilePath, szSeparator);
		}

		lstrcatW(lpstrFilePath, const_cast<WCHAR*>(m_fd.cFileName));

		return TRUE;
	}
	BOOL MatchesMask(DWORD dwMask) const
	{
		assert(m_hFind != NULL);

		if (m_bFound)
			return ((m_fd.dwFileAttributes & dwMask) != 0);

		return FALSE;
	}

	BOOL IsDots() const
	{
		assert(m_hFind != NULL);

		// return TRUE if the file name is "." or ".." and
		// the file is a directory

		BOOL bResult = FALSE;
		if (m_bFound && IsDirectory())
		{
			if (m_fd.cFileName[0] == L'.' && (m_fd.cFileName[1] == L'\0' || (m_fd.cFileName[1] == L'.' && m_fd.cFileName[2] == L'\0')))
				bResult = TRUE;
		}

		return bResult;
	}

	BOOL IsReadOnly() const
	{
		return MatchesMask(FILE_ATTRIBUTE_READONLY);
	}

	BOOL IsDirectory() const
	{
		return MatchesMask(FILE_ATTRIBUTE_DIRECTORY);
	}

	BOOL IsCompressed() const
	{
		return MatchesMask(FILE_ATTRIBUTE_COMPRESSED);
	}

	BOOL IsSystem() const
	{
		return MatchesMask(FILE_ATTRIBUTE_SYSTEM);
	}

	BOOL IsHidden() const
	{
		return MatchesMask(FILE_ATTRIBUTE_HIDDEN);
	}

	BOOL IsTemporary() const
	{
		return MatchesMask(FILE_ATTRIBUTE_TEMPORARY);
	}

	BOOL IsNormal() const
	{
		return MatchesMask(FILE_ATTRIBUTE_NORMAL);
	}

	BOOL IsArchived() const
	{
		return MatchesMask(FILE_ATTRIBUTE_ARCHIVE);
	}

	// Operations
	BOOL FindFile(LPCWSTR pstrName = NULL)
	{
		Close();

		if (pstrName == NULL)
		{
			pstrName = L"*.*";
		}
		else if (lstrlenW(pstrName) >= MAX_PATH)
		{
			assert(FALSE);
			return FALSE;
		}

		lstrcpynW(m_fd.cFileName, pstrName, sizeof(m_fd.cFileName) / sizeof(m_fd.cFileName[0]));

		m_hFind = FindFirstFileW(pstrName, &m_fd);

		if (m_hFind == INVALID_HANDLE_VALUE)
			return FALSE;

		bool bFullPath = (GetFullPathNameW(pstrName, MAX_PATH, m_lpszRoot, NULL) != 0);


		// passed name isn't a valid path but was found by the API
		assert(bFullPath);
		if (!bFullPath)
		{
			Close();
			SetLastError(ERROR_INVALID_NAME);
			return FALSE;
		}
		else
		{
			// find the last forward or backward whack
			LPWSTR pstrBack = wcsrchr(m_lpszRoot, L'\\');
			LPWSTR pstrFront = wcsrchr(m_lpszRoot, L'/');

			if (pstrFront != NULL || pstrBack != NULL)
			{
				if (pstrFront == NULL)
					pstrFront = m_lpszRoot;
				if (pstrBack == NULL)
					pstrBack = m_lpszRoot;

				// from the start to the last whack is the root

				if (pstrFront >= pstrBack)
					*pstrFront = L'\0';
				else
					*pstrBack = L'\0';
			}
		}

		m_bFound = TRUE;

		return TRUE;
	}

	BOOL FindNextFile()
	{
		assert(m_hFind != NULL);

		if (m_hFind == NULL)
			return FALSE;

		if (!m_bFound)
			return FALSE;

		m_bFound = ::FindNextFileW(m_hFind, &m_fd);
		return m_bFound;
	}

	void Close()
	{
		m_bFound = FALSE;

		if (m_hFind != NULL && m_hFind != INVALID_HANDLE_VALUE)
		{
			FindClose(m_hFind);
			m_hFind = NULL;
		}
	}
};


#endif // filefinder_helper_h__

