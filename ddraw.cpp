////////////////////////////////////////////////////////////////////
// =================================================================
// ddraw.cpp
// =================================================================
#include "ddraw.h"
 
#include <detours.h> // detours 3.0
#pragma comment(lib, "detours.lib")
 
#include <tchar.h>
#include <ShlObj.h>
#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>
 
BOOL DetourFunction(BOOL fStatus, LPVOID* lppvFunction, LPVOID lpvRedirection)
{
        if (DetourTransactionBegin() != NO_ERROR)
                return FALSE;
 
        if (DetourUpdateThread(GetCurrentThread()) == NO_ERROR)
                if ((fStatus ? DetourAttach : DetourDetach)(lppvFunction, lpvRedirection) == NO_ERROR)
                        if (DetourTransactionCommit() == NO_ERROR)
                                return TRUE;
 
        DetourTransactionAbort();
        return FALSE;
}
 
namespace ddraw
{
        AcquireDDThreadLock_t                   _AcquireDDThreadLock;
        CompleteCreateSysmemSurface_t   _CompleteCreateSysmemSurface;
        D3DParseUnknownCommand_t                _D3DParseUnknownCommand;
        DDGetAttachedSurfaceLcl_t               _DDGetAttachedSurfaceLcl;
        DDInternalLock_t                                _DDInternalLock;
        DDInternalUnlock_t                              _DDInternalUnlock;
        DSoundHelp_t                                    _DSoundHelp;
        DirectDrawCreate_t                              _DirectDrawCreate;
        DirectDrawCreateClipper_t               _DirectDrawCreateClipper;
        DirectDrawCreateEx_t                    _DirectDrawCreateEx;
        DirectDrawEnumerateA_t                  _DirectDrawEnumerateA;
        DirectDrawEnumerateExA_t                _DirectDrawEnumerateExA;
        DirectDrawEnumerateExW_t                _DirectDrawEnumerateExW;
        DirectDrawEnumerateW_t                  _DirectDrawEnumerateW;
        DllCanUnloadNow_t                               _DllCanUnloadNow;
        DllGetClassObject_t                             _DllGetClassObject;
        GetDDSurfaceLocal_t                             _GetDDSurfaceLocal;
        GetOLEThunkData_t                               _GetOLEThunkData;
        GetSurfaceFromDC_t                              _GetSurfaceFromDC;
        RegisterSpecialCase_t                   _RegisterSpecialCase;
        ReleaseDDThreadLock_t                   _ReleaseDDThreadLock;
        SetAppCompatData_t                              _SetAppCompatData;
 
        HRESULT WINAPI AcquireDDThreadLock(void)
        {
                return _AcquireDDThreadLock();
        }
 
        HRESULT WINAPI CompleteCreateSysmemSurface(void)
        {
                return _CompleteCreateSysmemSurface();
        }
 
        HRESULT WINAPI D3DParseUnknownCommand(void)
        {
                return _D3DParseUnknownCommand();
        }
 
        HRESULT WINAPI DDGetAttachedSurfaceLcl(void)
        {
                return _DDGetAttachedSurfaceLcl();
        }
 
        HRESULT WINAPI DDInternalLock(void)
        {
                return _DDInternalLock();
        }
 
        HRESULT WINAPI DDInternalUnlock(void)
        {
                return _DDInternalUnlock();
        }
 
        HRESULT WINAPI DSoundHelp(void)
        {
                return _DSoundHelp();
        }
 
        HRESULT WINAPI DirectDrawCreate(
                GUID FAR                                *lpGUID,
                LPDIRECTDRAW                    *lplpDD,
                IUnknown FAR                    *pUnkOuter)
        {
                return _DirectDrawCreate(lpGUID, lplpDD, pUnkOuter);
        }
 
        HRESULT WINAPI DirectDrawCreateClipper(
                DWORD                                   dwFlags,
                LPDIRECTDRAWCLIPPER FAR *lplpDDClipper,
                IUnknown FAR            *pUnkOuter)
        {
                return _DirectDrawCreateClipper(dwFlags, lplpDDClipper, pUnkOuter);
        }
 
        HRESULT WINAPI DirectDrawCreateEx(
                GUID FAR     *lpGUID,
                LPVOID       *lplpDD,
                REFIID       iid,
                IUnknown FAR *pUnkOuter)
        {
                return _DirectDrawCreateEx(lpGUID, lplpDD, iid, pUnkOuter);
        }
 
        HRESULT WINAPI DirectDrawEnumerateA(void)
        {
                return _DirectDrawEnumerateA();
        }
 
        HRESULT WINAPI DirectDrawEnumerateExA(
                LPDDENUMCALLBACKEXA     lpCallback,
                LPVOID                          lpContext,
                DWORD               dwFlags)
        {
                return _DirectDrawEnumerateExA(lpCallback, lpContext, dwFlags);
        }
 
        HRESULT WINAPI DirectDrawEnumerateExW(
                LPDDENUMCALLBACKEXW     lpCallback,
                LPVOID                          lpContext,
                DWORD               dwFlags)
        {
                return _DirectDrawEnumerateExW(lpCallback, lpContext, dwFlags);
        }
 
        HRESULT WINAPI DirectDrawEnumerateW(
                LPDDENUMCALLBACKEXW lpCallback,
                LPVOID                          lpContext)
        {
                return _DirectDrawEnumerateW(lpCallback, lpContext);
        }
 
        HRESULT WINAPI DllCanUnloadNow(void)
        {
                return _DllCanUnloadNow();
        }
 
        HRESULT WINAPI DllGetClassObject(
                const CLSID & rclsid,
                const IID & riid,
                void ** ppv)
        {
                return _DllGetClassObject(rclsid, riid, ppv);
        }
 
        HRESULT WINAPI GetDDSurfaceLocal(void)
        {
                return _GetDDSurfaceLocal();
        }
 
        HRESULT WINAPI GetOLEThunkData(void)
        {
                return _GetOLEThunkData();
        }
 
        HRESULT WINAPI GetSurfaceFromDC(void)
        {
                return _GetSurfaceFromDC();
        }
 
        HRESULT WINAPI RegisterSpecialCase(void)
        {
                return _RegisterSpecialCase();
        }
 
        HRESULT WINAPI ReleaseDDThreadLock(void)
        {
                return _ReleaseDDThreadLock();
        }
 
        HRESULT WINAPI SetAppCompatData(void)
        {
                return _SetAppCompatData();
        }
 
        BOOL Detour__FindFirstFileA(void)
        {
                static decltype(&FindFirstFileA) _FindFirstFileA = FindFirstFileA;
 
                decltype(&FindFirstFileA) FindFirstFileA__Hook = [](
                        _In_ LPCSTR lpFileName,
                        _Out_ LPWIN32_FIND_DATAA lpFindFileData) -> HANDLE
                {
                        for (int i = 0; lpFileName[i] != '\0'; i++)
                                if (lpFileName[i] == '*')
                                        return INVALID_HANDLE_VALUE;
 
                        return _FindFirstFileA(lpFileName, lpFindFileData);
                };
 
                return DetourFunction(TRUE, reinterpret_cast<void**>(&_FindFirstFileA), FindFirstFileA__Hook);
        }
 
        BOOL Initialize(void)
        {
                TCHAR tszPath[MAX_PATH];
 
                if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, 0, tszPath)))
                        return FALSE;
 
                if (!SUCCEEDED(StringCchPrintf(tszPath, MAX_PATH, _T("%s%s"), tszPath, _T("\\ddraw.dll"))))
                        return FALSE;
 
                HMODULE hModule = LoadLibrary(tszPath);
 
                if (!hModule)
                        return FALSE;
 
                _AcquireDDThreadLock = reinterpret_cast<AcquireDDThreadLock_t>(GetProcAddress(hModule, "AcquireDDThreadLock"));
                _CompleteCreateSysmemSurface = reinterpret_cast<CompleteCreateSysmemSurface_t>(GetProcAddress(hModule, "CompleteCreateSysmemSurface"));
                _D3DParseUnknownCommand = reinterpret_cast<D3DParseUnknownCommand_t>(GetProcAddress(hModule, "D3DParseUnknownCommand"));
                _DDGetAttachedSurfaceLcl = reinterpret_cast<DDGetAttachedSurfaceLcl_t>(GetProcAddress(hModule, "DDGetAttachedSurfaceLcl"));
                _DDInternalLock = reinterpret_cast<DDInternalLock_t>(GetProcAddress(hModule, "DDInternalLock"));
                _DDInternalUnlock = reinterpret_cast<DDInternalUnlock_t>(GetProcAddress(hModule, "DDInternalUnlock"));
                _DSoundHelp = reinterpret_cast<DSoundHelp_t>(GetProcAddress(hModule, "DSoundHelp"));
                _DirectDrawCreate = reinterpret_cast<DirectDrawCreate_t>(GetProcAddress(hModule, "DirectDrawCreate"));
                _DirectDrawCreateClipper = reinterpret_cast<DirectDrawCreateClipper_t>(GetProcAddress(hModule, "DirectDrawCreateClipper"));
                _DirectDrawCreateEx = reinterpret_cast<DirectDrawCreateEx_t>(GetProcAddress(hModule, "DirectDrawCreateEx"));
                _DirectDrawEnumerateA = reinterpret_cast<DirectDrawEnumerateA_t>(GetProcAddress(hModule, "DirectDrawEnumerateA"));
                _DirectDrawEnumerateExA = reinterpret_cast<DirectDrawEnumerateExA_t>(GetProcAddress(hModule, "DirectDrawEnumerateExA"));
                _DirectDrawEnumerateExW = reinterpret_cast<DirectDrawEnumerateExW_t>(GetProcAddress(hModule, "DirectDrawEnumerateExW"));
                _DirectDrawEnumerateW = reinterpret_cast<DirectDrawEnumerateW_t>(GetProcAddress(hModule, "DirectDrawEnumerateW"));
                _DllCanUnloadNow = reinterpret_cast<DllCanUnloadNow_t>(GetProcAddress(hModule, "DllCanUnloadNow"));
                _DllGetClassObject = reinterpret_cast<DllGetClassObject_t>(GetProcAddress(hModule, "DllGetClassObject"));
                _GetDDSurfaceLocal = reinterpret_cast<GetDDSurfaceLocal_t>(GetProcAddress(hModule, "GetDDSurfaceLocal"));
                _GetOLEThunkData = reinterpret_cast<GetOLEThunkData_t>(GetProcAddress(hModule, "GetOLEThunkData"));
                _GetSurfaceFromDC = reinterpret_cast<GetSurfaceFromDC_t>(GetProcAddress(hModule, "GetSurfaceFromDC"));
                _RegisterSpecialCase = reinterpret_cast<RegisterSpecialCase_t>(GetProcAddress(hModule, "RegisterSpecialCase"));
                _ReleaseDDThreadLock = reinterpret_cast<ReleaseDDThreadLock_t>(GetProcAddress(hModule, "ReleaseDDThreadLock"));
                _SetAppCompatData = reinterpret_cast<SetAppCompatData_t>(GetProcAddress(hModule, "SetAppCompatData"));
 
                Detour__FindFirstFileA();
 
                return TRUE;
        }
}
 
////////////////////////////////////////////////////////////////////
// =================================================================
// ddraw.def
// =================================================================
 
LIBRARY "ddraw"
EXPORTS
 
AcquireDDThreadLock                     PRIVATE
CompleteCreateSysmemSurface     PRIVATE
D3DParseUnknownCommand          PRIVATE
DDGetAttachedSurfaceLcl         PRIVATE
DDInternalLock                          PRIVATE
DDInternalUnlock                        PRIVATE
DSoundHelp                                      PRIVATE
DirectDrawCreate                        PRIVATE
DirectDrawCreateClipper         PRIVATE
DirectDrawCreateEx                      PRIVATE
DirectDrawEnumerateA            PRIVATE
DirectDrawEnumerateExA          PRIVATE
DirectDrawEnumerateExW          PRIVATE
DirectDrawEnumerateW            PRIVATE
DllCanUnloadNow                         PRIVATE
DllGetClassObject                       PRIVATE
GetDDSurfaceLocal                       PRIVATE
GetOLEThunkData                         PRIVATE
GetSurfaceFromDC                        PRIVATE
RegisterSpecialCase                     PRIVATE
ReleaseDDThreadLock                     PRIVATE
SetAppCompatData                        PRIVATE
 
 
 
////////////////////////////////////////////////////////////////////
// =================================================================
// ddraw.hpp
// =================================================================
 
#pragma once
 
#include <Windows.h>
#include <d3d.h>
 
namespace ddraw
{
        typedef HRESULT (WINAPI * AcquireDDThreadLock_t)(void);
 
        typedef HRESULT (WINAPI * CompleteCreateSysmemSurface_t)(void);
       
        typedef HRESULT (WINAPI * D3DParseUnknownCommand_t)(void);
       
        typedef HRESULT (WINAPI * DDGetAttachedSurfaceLcl_t)(void);
       
        typedef HRESULT (WINAPI * DDInternalLock_t)(void);
       
        typedef HRESULT (WINAPI * DDInternalUnlock_t)(void);
       
        typedef HRESULT (WINAPI * DSoundHelp_t)(void);
 
        typedef HRESULT (WINAPI * DirectDrawCreate_t)(
                GUID FAR                        *lpGUID,
                LPDIRECTDRAW                    *lplpDD,
                IUnknown FAR                    *pUnkOuter);
 
        typedef HRESULT (WINAPI * DirectDrawCreateClipper_t)(
                DWORD                   dwFlags,
                LPDIRECTDRAWCLIPPER FAR *lplpDDClipper,
                IUnknown FAR            *pUnkOuter);
 
        typedef HRESULT (WINAPI * DirectDrawCreateEx_t)(
                GUID FAR     *lpGUID,
                LPVOID       *lplpDD,
                REFIID       iid,
                IUnknown FAR *pUnkOuter);
 
        typedef HRESULT (WINAPI * DirectDrawEnumerateA_t)(void);
 
        typedef HRESULT (WINAPI * DirectDrawEnumerateExA_t)(
                LPDDENUMCALLBACKEXA lpCallback,
                LPVOID              lpContext,
                DWORD               dwFlags);
 
        typedef HRESULT (WINAPI * DirectDrawEnumerateExW_t)(
                LPDDENUMCALLBACKEXW lpCallback,
                LPVOID              lpContext,
                DWORD               dwFlags);
 
        typedef HRESULT (WINAPI * DirectDrawEnumerateW_t)(
                LPDDENUMCALLBACKEXW lpCallback,
                LPVOID              lpContext);
 
        typedef HRESULT (WINAPI * DllCanUnloadNow_t)(void);
 
        typedef HRESULT (WINAPI * DllGetClassObject_t)(
                const CLSID & rclsid,
                const IID & riid,
                void ** ppv);
 
        typedef HRESULT (WINAPI * GetDDSurfaceLocal_t)(void);
       
        typedef HRESULT (WINAPI * GetOLEThunkData_t)(void);
       
        typedef HRESULT (WINAPI * GetSurfaceFromDC_t)(void);
       
        typedef HRESULT (WINAPI * RegisterSpecialCase_t)(void);
       
        typedef HRESULT (WINAPI * ReleaseDDThreadLock_t)(void);
 
        typedef HRESULT (WINAPI * SetAppCompatData_t)(void);
 
        BOOL Initialize(void);
}
 
 
////////////////////////////////////////////////////////////////////
// =================================================================
// dllmain.cpp
// =================================================================
 
#include <Windows.h>
 
#include <tchar.h>
#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>
 
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
 
#include "ddraw.h"
 
BOOL WINAPI LoadLibraries()
{
        TCHAR tszTemp[MAX_PATH];
 
        HANDLE hFile;
        WIN32_FIND_DATAW fData;
 
        if (GetCurrentDirectory(MAX_PATH, tszTemp) == 0)
                return FALSE;
 
        if (!SUCCEEDED(StringCchPrintf(tszTemp, MAX_PATH, _T("%s%s"), tszTemp, _T("\\Loader\\*.dll"))))
                return FALSE;
 
        hFile = FindFirstFile(tszTemp, &fData);
        if (hFile == INVALID_HANDLE_VALUE)
                return FALSE;
 
        do
        {
                if ((fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
                        if (_tcscmp(PathFindExtension(fData.cFileName), _T(".dll")) == 0)
                                if (SUCCEEDED(StringCchPrintf(tszTemp, MAX_PATH, _T("%s%s"), _T("Loader\\"), fData.cFileName)))
                                        LoadLibrary(tszTemp);
        } while (FindNextFile(hFile, &fData));
 
        return TRUE;
}
 
BOOL WINAPI OnAttachProcess(__in HINSTANCE hInstance)
{
        ddraw::Initialize();
 
        return LoadLibraries();
}
 
BOOL WINAPI DllMain(
        __in HINSTANCE hInstance,
        __in DWORD fdwReason,  
        __reserved LPVOID lpvReserved
        )
{
        UNREFERENCED_PARAMETER(lpvReserved);
 
        switch (fdwReason)
        {
        case DLL_PROCESS_ATTACH:
        {
                // AllocConsole();
 
                DisableThreadLibraryCalls(hInstance);
 
                return OnAttachProcess(hInstance);
        }
        case DLL_PROCESS_DETACH:
        {
                // FreeLibrary
        }
        }
 
        return TRUE;
}
