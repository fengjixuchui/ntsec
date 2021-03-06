#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <sddl.h>
#include "include\utils.h"

PVOID safe_alloc(SIZE_T dwBytes)
{
   PVOID pRes = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytes);
   if (pRes == NULL)
      ExitProcess(ERROR_OUTOFMEMORY);
   return pRes;
}

PVOID safe_realloc(PVOID pBuffer, SIZE_T dwBytes)
{
   PVOID pRes = NULL;
   if (pBuffer == NULL)
      return safe_alloc(dwBytes);
   pRes = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, pBuffer, dwBytes);
   if (pRes == NULL)
      ExitProcess(ERROR_OUTOFMEMORY);
   return pRes;
}

VOID safe_free(PVOID pBuffer)
{
   if (!HeapFree(GetProcessHeap(), 0, pBuffer))
   {
      _ftprintf(stderr, TEXT("Heap corrupted\n"));
      ExitProcess(ERROR_OUTOFMEMORY);
   }
}

PVOID safe_dup(const VOID *pBuffer, SIZE_T dwBytes)
{
   PVOID pRes = safe_alloc(dwBytes);
   memcpy(pRes, pBuffer, dwBytes);
   return pRes;
}

PTSTR safe_strdup(PCTSTR swzIn)
{
   SIZE_T dwBytes = (_tcslen(swzIn) + 1) * sizeof(TCHAR);
   PTSTR swzRes = safe_alloc(dwBytes);
   memcpy(swzRes, swzIn, dwBytes);
   return swzRes;
}

BOOL str_starts_with(PCTSTR swzFull, PCTSTR swzPrefix)
{
   return (_tcsnicmp(swzFull, swzPrefix, _tcslen(swzPrefix)) == 0);
}

UNICODE_STRING* string_to_unicode(PCTSTR swzIn)
{
   PUNICODE_STRING pUS = safe_alloc(sizeof(UNICODE_STRING) + (_tcslen(swzIn) + 1) * sizeof(WCHAR));
   pUS->Length = pUS->MaximumLength = (USHORT)(_tcslen(swzIn) * sizeof(TCHAR));
   pUS->Buffer = (PWCHAR)(((PBYTE)pUS) + sizeof(UNICODE_STRING));
#ifdef UNICODE
   memcpy(pUS->Buffer, swzIn, wcslen(swzIn) * sizeof(WCHAR));
#else
   swprintf_s(pUS->Buffer, strlen(swzIn), L"%hs", swzIn);
#endif
   return pUS;
}

PTSTR unicode_to_string(PUNICODE_STRING pIn)
{
   PTSTR swzOut = NULL;

   if (pIn == NULL)
      return safe_alloc(sizeof(TCHAR));

   swzOut = safe_alloc(pIn->Length + sizeof(TCHAR));
   memcpy(swzOut, pIn->Buffer, pIn->Length);
   return swzOut;
}

VOID basedir(PTSTR swzPath)
{
   PTSTR swzStart = swzPath;
   while (*swzPath != TEXT('\0'))
      swzPath++;
   while (swzPath != swzStart && *swzPath != TEXT('\\'))
   {
      *swzPath = TEXT('\0');
      swzPath--;
   }
}

PWSTR string_to_wide(PCTSTR swzIn)
{
#ifdef UNICODE
   return safe_dup(swzIn, (wcslen(swzIn) + 1) * sizeof(WCHAR));
#else
#error Not implemented.
#endif
}

SIZE_T count_bits_set(SIZE_T dwBitField)
{
   SIZE_T res = 0;
   while (dwBitField != 0)
   {
      if ((dwBitField & 1) != 0)
         res++;
      dwBitField >>= 1;
   }
   return res;
}
