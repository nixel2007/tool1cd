#include "System.hpp"



namespace System {

bool operator == (const TGUID &a, const TGUID &b)
{
	return a.D1 == b.D1
		&& a.D2 == b.D2
		&& a.D3 == b.D3
		&& a.D4[0] == b.D4[0]
		&& a.D4[1] == b.D4[1]
		&& a.D4[2] == b.D4[2]
		&& a.D4[3] == b.D4[3]
		&& a.D4[4] == b.D4[4]
		&& a.D4[5] == b.D4[5]
		&& a.D4[6] == b.D4[6]
		&& a.D4[7] == b.D4[7]
	;
}

//---------------------------------------------------------------------------
// Преобразование времени FILETIME to POSIX
uint64_t FileTime_to_POSIX(LPFILETIME pft)
{
	// takes the last modified date
	LARGE_INTEGER date, adjust;
	date.HighPart = pft -> dwHighDateTime;
	date.LowPart  = pft -> dwLowDateTime;

	// 100-nanoseconds = milliseconds * 10000
	adjust.QuadPart = SEC_TO_UNIX_EPOCH * 10000;

	// removes the diff between 1970 and 1601
	date.QuadPart -= adjust.QuadPart;

	// converts back from 100-nanoseconds to seconds
	return date.QuadPart / 10000000;
}

//---------------------------------------------------------------------------
// Преобразование времени POSIX to FILETIME
void UnixTimeToFileTime(time_t t, LPFILETIME pft)
{

	uint64_t ll = Int32x32To64(t, 10000000) + SEC_TO_UNIX_EPOCH;

	pft->dwLowDateTime = (uint32_t)ll;
	pft->dwHighDateTime = ll >> 32;
}


bool SystemTimeToFileTime(const SYSTEMTIME *lpSystemTime, LPFILETIME lpFileTime)
{
	struct tm ttm;
	time_t UnixTime;

	ttm.tm_year  = lpSystemTime->wYear - 1900;
	ttm.tm_mon   = lpSystemTime->wMonth - 1;
	ttm.tm_mday  = lpSystemTime->wDay;
	ttm.tm_hour  = lpSystemTime->wHour;
	ttm.tm_min   = lpSystemTime->wMinute;
	ttm.tm_sec   = lpSystemTime->wSecond;
	ttm.tm_isdst = 0;
	ttm.tm_wday  = 0; // возможно здесь что-то не то, надо проверять; days since Sunday - [0, 6] 
	ttm.tm_yday  = 0; // возможно здесь что-то не то, надо проверять; days since January 1 - [0, 365]
	
	UnixTime = mktime(&ttm);
	UnixTimeToFileTime(UnixTime, lpFileTime);

	return true; // TODO: реализовать кроссплатформенное преобразование системного времени в файловое время
}

bool LocalFileTimeToFileTime(const FILETIME *lpLocalFileTime, LPFILETIME lpFileTime)
{
	time_t rawtime = FileTime_to_POSIX((LPFILETIME)lpLocalFileTime);
	UnixTimeToFileTime(rawtime, lpFileTime);
	
	return true; // TODO: реализовать кроссплатформенное преобразование локального системного файлового времени в универсальное (UTC) файловое время
}

bool FileTimeToLocalFileTime(const FILETIME *lpLocalFileTime, LPFILETIME lpFileTime)
{
	time_t rawtime = FileTime_to_POSIX((LPFILETIME)lpLocalFileTime);
	UnixTimeToFileTime(rawtime, lpFileTime);

	return true; // TODO: реализовать кроссплатформенное преобразование из универсального (UTC) файлового времени в локальноое системное файловое время
}

void GetSystemTime(LPSYSTEMTIME lpSystemTime)
{
	// TODO: реализовать кроссплатформенное получение времени операционной системы

	time_t CurrTime = time(NULL);
	tm *ttm = localtime(&CurrTime);

	lpSystemTime->wYear   = ttm->tm_year + 1900;
	lpSystemTime->wMonth  = ttm->tm_mon + 1;
	lpSystemTime->wDay    = ttm->tm_mday;
	lpSystemTime->wHour   = ttm->tm_hour;
	lpSystemTime->wMinute = ttm->tm_min;
	lpSystemTime->wSecond = ttm->tm_sec;

}

} // System
