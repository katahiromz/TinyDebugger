#pragma once

#ifndef _INC_WINDOWS
    #include <windows.h>
#endif
#include <vector>
#include <string>
#include "dat.hpp"

struct DataEntryEx;
struct TinyDImpl;

class TinyD
{
public:
    TinyD();
    virtual ~TinyD();

    void Print(const std::string& str);
    void Printf(const char *fmt, ...);
    virtual void VPrintf(const char *fmt, va_list va);

    void AddDebugEntry(const DataEntry& src);
    void SetDebugEntries(size_t count, const DataEntry *src);

    bool StartDebugProcess(const char *cmdline);
    bool StartDebugProcess(const wchar_t *cmdline);

    virtual void OnEnterProc(DataEntryEx& entry, CONTEXT& ctx, DWORD_PTR *pdw);
    virtual void OnLeaveProc(DataEntryEx& entry, CONTEXT& ctx);
    virtual DWORD DebugLoop(void);
    void DumpContext(DWORD dwThreadId = 0);

protected:
    TinyDImpl *m_pimpl;
    TinyDImpl& impl();

    virtual void OnCreateProcess(DEBUG_EVENT& de, CREATE_PROCESS_DEBUG_INFO& CreateProcessInfo);
    virtual void OnCreateThread(DEBUG_EVENT& de, CREATE_THREAD_DEBUG_INFO& CreateThread);
    virtual void OnExitThread(DEBUG_EVENT& de, EXIT_THREAD_DEBUG_INFO& ExitThread);
    virtual DWORD OnExitProcess(DEBUG_EVENT& de, EXIT_PROCESS_DEBUG_INFO& ExitProcess);
    virtual void OnBreakPoint(DEBUG_EVENT& de);
    virtual void OnSingleStep(DEBUG_EVENT& de);
    virtual bool OnDebugEvent(DEBUG_EVENT& de);
    virtual void OnDebugString(OUTPUT_DEBUG_STRING_INFO& DebugString);
    virtual void OnLoadDll(DEBUG_EVENT& de, LOAD_DLL_DEBUG_INFO& LoadDll);
    virtual void OnUnloadDll(DEBUG_EVENT& de, UNLOAD_DLL_DEBUG_INFO& UnloadDll);
};
