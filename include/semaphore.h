#pragma once
#include <Windows.h>

int open_nt_semaphore_object(PCTSTR swzNTPath, DWORD dwRightsRequired, HANDLE *phOut);
