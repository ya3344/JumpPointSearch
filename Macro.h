#pragma once
template <typename T>
inline void SafeDelete(T& obj)
{
	if (obj)
	{
		delete obj;
		obj = nullptr;
	}
}

template <typename T>
inline void SafeFree(T& obj)
{
	if (obj)
	{
		free(obj);
		obj = nullptr;
	}
}

#define MESSAGE_BOX(ErrorName)				\
MessageBox(gHWnd, ErrorName, nullptr, MB_OK);		


#define CONSOLE_LOG(text) wprintf(L"function:%S %S\n", __FUNCTION__, text)
#define CONSOLE_ERROR_LOG(text) wprintf(L"[ERROR] function:%S %S\n", __FUNCTION__, text)

#define NODE_INFO_VIEW