#pragma once

template<class Interface>
inline void
SafeRelease(
	Interface** ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif



#define VK__A 0x41
#define VK__B 0x42
#define VK__C 0x43
#define VK__D 0x44
#define VK__E 0x45
#define VK__F 0x46
#define VK__G 0x47
#define VK__H 0x48
#define VK__I 0x49
#define VK__J 0x4A
#define VK__K 0x4B
#define VK__L 0x4C
#define VK__M 0x4D
#define VK__N 0x4E
#define VK__O 0x4F
#define VK__P 0x50
#define VK__Q 0x51
#define VK__R 0x52
#define VK__S 0x53
#define VK__T 0x54
#define VK__U 0x55
#define VK__V 0x56
#define VK__W 0x57
#define VK__X 0x58
#define VK__Y 0x59
#define VK__Z 0x5A
