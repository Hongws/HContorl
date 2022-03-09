#ifndef CROSSPLATFORMDEF_H
#define CROSSPLATFORMDEF_H

#ifdef __APPLE__
#  include <TargetConditionals.h>
#  if (defined(TARGET_OS_OSX) && TARGET_OS_OSX == 1) || \
      (defined(TARGET_OS_MAC) && TARGET_OS_MAC == 1)
#    define CP_PLATFORM_MAC
#  elif (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE == 1)
#    define CP_PLATFORM_IPHONE
#  endif

#elif defined(linux) || defined(__linux) || defined(__linux__)
#  define CP_PLATFORM_LINUX

#elif defined(WIN32) || defined(__WIN32__) || defined(_WIN32) || defined(_MSC_VER) || defined(__MINGW32__)
#  define CP_PLATFORM_WINDOWS
#endif

#define CROSS_PLATFORM_LIBRARY_EXPORTS
#ifdef _MSC_VER 
	#define CROSS_PLATFORM_HIDDEN_API
	#ifdef CROSS_PLATFORM_LIBRARY_EXPORTS
	#define CROSS_PLATFORM_API __declspec(dllexport)
	#else
	#define CROSS_PLATFORM_API __declspec(dllimport)
	#endif
#else 
	#define CROSS_PLATFORM_API __attribute((visibility("default"))) 
	#define CROSS_PLATFORM_HIDDEN_API __attribute((visibility("hidden"))) 
#endif

#ifdef _WIN32
	//#include <WinSock2.h>
	#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
	#include <Windows.h>
	#include <tchar.h>
	#include <Shlwapi.h>
	#pragma comment(lib,"shlwapi.lib")
#else

	  #ifndef UNICODECP
	  	#define UNICODECP
	  #endif
	#include <string.h>
	#include <dlfcn.h>
	#include <unistd.h>
	#include <stdbool.h>
	#ifndef DWORD
		typedef unsigned int DWORD;
	#endif
	#ifndef WCHAR
		#ifndef _MAC
			typedef wchar_t WCHAR;    
		#else
			typedef unsigned short WCHAR;    
		#endif
	#endif

	#ifndef LPSTR
		typedef char *LPSTR;
	#endif
	#ifndef LPCSTR
		typedef const char* LPCSTR;
	#endif
	#ifndef LPWSTR
		typedef wchar_t* LPWSTR;
	#endif
	#ifndef LPCWSTR
		typedef const wchar_t* LPCWSTR;
	#endif

	#ifndef LPCTSTR
		#ifdef UNICODECP
			#define LPCTSTR LPCWSTR
			//typedef const wchar_t* LPCTSTR;
		#else
			#define LPCTSTR LPCSTR
			//typedef const char* LPCTSTR;
		#endif	
	#endif

	#ifndef MAX_PATH
		#define MAX_PATH 260
	#endif

	#ifndef TCHAR
		#ifdef UNICODECP
		typedef wchar_t TCHAR;
		#else
		typedef char TCHAR;
		#endif
	#endif

	#ifndef _tcscpy
		#ifdef UNICODECP
		#define _tcscpy wcscpy
		#else
		#define _tcscpy strcpy
		#endif
	#endif
	#ifndef _tccmp
		#ifdef UNICODECP
		#define _tccmp wcscmp
		#else
		#define _tccmp strcmp
		#endif
	#endif

	#ifndef BYTE
		typedef unsigned char BYTE;
	#endif
	#ifndef LPBYTE
		typedef BYTE *LPBYTE;
	#endif
	#ifndef BOOL
		typedef bool BOOL;
	#endif
	#ifndef FALSE
		#define FALSE 0
	#endif
	#ifndef TRUE
		#define TRUE 1
	#endif
	#ifndef UINT
		#define UINT unsigned int 
	#endif

	#ifndef strcpy_s
		#define strcpy_s strcpy
	#endif
	#ifndef strcat_s
		#define strcat_s strcat
	#endif

	#ifndef Sleep
		#define Sleep sleep
	#endif	


#endif // _WIN32


#include <string>
#include <cassert>
#include <functional>

#include <typeinfo>
#include <iostream>
namespace CrossPlatFormDef
{
	class lib_warpper
	{
	public:
		lib_warpper(std::string str_lib)
		{
#ifdef _WIN32
			char szPath[1024] = { 0 };
			GetModuleFileNameA(NULL, szPath, 1024);
			PathRemoveFileSpecA(szPath);
			std::string strDll = "\\" + str_lib + ".dll";
			strDll = szPath + strDll;
			win_handle_ = LoadLibraryA(strDll.c_str());
			int n = GetLastError();
			if (win_handle_ == NULL)			
				printf("LoadLibraryA: %s. failed, error:%d.\n", strDll.c_str(), GetLastError());
			//assert(win_handle_);
#else
			char szTemp[1024] = { 0 };
			getcwd(szTemp, 1024);
			std::string strDir = szTemp;
			std::string strSo = strDir +"/"+ str_lib + ".so";
			//std::string strSo = str_lib + ".so";
			handle_ = dlopen(strSo.c_str(), RTLD_LAZY| RTLD_GLOBAL);
			if(handle_ == NULL)
				printf("dlopen: %s. failed, error:%sn.\n", std::string(str_lib + ".so").c_str(), dlerror());
			assert(handle_);
#endif // _WIN32
		}

// 		lib_warpper(std::string str_lib)
// 		{
// #ifdef _WIN32
// 			win_handle_ = LoadLibraryA(str_lib.c_str());
// 			int n = GetLastError();
// 			if (win_handle_ == NULL)			
// 				printf("LoadLibraryA: %s. failed, error:%sn.\n", str_lib.c_str(), GetLastError());
// 			//assert(win_handle_);
// #else
// 			handle_ = dlopen(str_lib.c_str(), RTLD_LAZY| RTLD_GLOBAL);
// 			if(handle_ == NULL)
// 				printf("dlopen: %s. failed, error:%sn.\n", str_lib.c_str(), dlerror());
// 			assert(handle_);
// #endif // _WIN32
// 		}

		lib_warpper(std::string str_dir,std::string str_lib)
		{
#ifdef _WIN32
			std::string strDll = "\\" + str_lib + ".dll";
			strDll = str_dir + strDll;
			win_handle_ = LoadLibraryA(strDll.c_str());
			int n = GetLastError();
			if (win_handle_ == NULL)			
				printf("LoadLibraryA: %s. failed, error:%sn.\n", strDll.c_str(), GetLastError());
			//assert(win_handle_);
#else
			std::string strSo = str_dir +"/"+ str_lib + ".so";
			//std::string strSo = str_lib + ".so";
			handle_ = dlopen(strSo.c_str(), RTLD_LAZY| RTLD_GLOBAL);
			if(handle_ == NULL)
				printf("dlopen: %s. failed, error:%sn.\n", std::string(str_lib + ".so").c_str(), dlerror());
			assert(handle_);

#endif // _WIN32
		}

		virtual ~lib_warpper()
		{
#ifdef _WIN32
			if (nullptr != win_handle_)
				FreeLibrary(win_handle_);
#else
			if (nullptr != handle_)
				dlclose(handle_);
#endif // _WIN32	
		}

		template <class ret_type>
		ret_type* get_function(std::string str_func)
		{
#ifdef _WIN32
			return (reinterpret_cast<ret_type*>(GetProcAddress(win_handle_, str_func.c_str())));
#else
			return (reinterpret_cast<ret_type*>(dlsym(handle_, str_func.c_str())));
#endif // _WIN32	
		}

	private:
#ifdef _WIN32
		HMODULE win_handle_ = nullptr;
#else
		void* handle_ = nullptr;
#endif // _WIN32
	};
}

#ifdef _WIN32
	#include "../others/PykMgr.h"
#else
	#include <iostream>       // std::cout, std::hex
	#include <string>         // std::string, std::u32string
	#include <locale>         // std::wstring_convert
	#include <codecvt>        // std::codecvt_utf8
	#include <cstdint>        // std::uint_least32_t
	
	inline std::wstring s2ws(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;	
		return converterX.from_bytes(str);
	}
	inline std::string ws2s(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;	
		return converterX.to_bytes(wstr);
	}

	#include "Uuid4.h"
	static char *random_uuid( char buf[37] )
	{
		const char *c = "89ab";
		char *p = buf;
		int n;
		for( n = 0; n < 16; ++n )
		{
			int b = rand()%255;
			switch( n )
			{
				case 6:
					sprintf(p, "4%x", b%15 );
				break;
				case 8:
					sprintf(p, "%c%x", c[rand()%strlen(c)], b%15 );
				break;
				default:
					sprintf(p, "%02x", b);
				break;
			}
			p += 2;
			switch( n )
			{
				case 3:
				case 5:
				case 7:
				case 9:
					*p++ = '-';
					break;
			}
		}
		*p = 0;
		return buf;
	}

	static char* str2upper(char *str)  
	{  
		if(str == NULL)
			return NULL;
		char *p = str;  
		while(*str)
		{
			*str = toupper(*str);  
			str++;  
		}  
		return p;  
	}

	static wchar_t* str2upper(wchar_t *str)  
	{  
		if(str == NULL)
			return NULL;
		wchar_t *p = str;  
		while(*str)
		{
			*str = towupper(*str);  
			str++;  
		}  
		return p;  
	}

	static char* str2lower(char *str)  
	{  
		if(str == NULL)
			return NULL;
		char *p = str;  
		while(*str)
		{  
			*str = tolower(*str);  
			str++;  
		}  
		return p;  
	}

	static wchar_t* str2lower(wchar_t *str)  
	{  
		if(str == NULL)
			return NULL;
		wchar_t *p = str;  
		while(*str)
		{  
			*str = towlower(*str);  
			str++;  
		}  
		return p;  
	}
	static int CopyFile(const char* src, const char* des)
	{
		int nRet = 0;
		FILE* pSrc = NULL, *pDes = NULL;
		pSrc = fopen(src, "r");
		pDes = fopen(des, "w+");	
		if (pSrc && pDes)
		{
			int nLen = 0;
			char szBuf[1024] = {0};
			while((nLen = fread(szBuf, 1, sizeof szBuf, pSrc)) > 0)
				fwrite(szBuf, 1, nLen, pDes);
		}
		else
			nRet = -1;
		if (pSrc)
			fclose(pSrc), pSrc = NULL;
		if (pDes)
			fclose(pDes), pDes = NULL;
		return nRet;
	}

	static int CopyFile(const wchar_t* srcw, const wchar_t* desw)
	{
		char src[1024]={0};
		char des[1024]={0};
		strcpy(src,ws2s(srcw).c_str());
		strcpy(des,ws2s(desw).c_str());
		return CopyFile(src,des);
	}

	#include <sys/stat.h>
    // 判断是否是一个文件
    static bool is_file(std::string filename) 
	{
        struct stat   buffer;
        return (stat (filename.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode));
    }
	static bool is_file(std::wstring filename) 
	{
		 return is_file(ws2s(filename));
	}

    // 判断是否是一个文件夹,
    static bool is_dir(std::string filefodler) 
	{
        struct stat   buffer;
        return (stat (filefodler.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
    }
	static bool is_dir(std::wstring filefodler) 
	{
        return is_dir(ws2s(filefodler));
    }
#endif


#endif // CROSSPLATFORMDEF_H