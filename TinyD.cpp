#include "TinyD.hpp"
#include "Utils.hpp"
#include "MR.hpp"
#include "mstr.hpp"
#include <psapi.h>
#include <shlwapi.h>
#include <map>
#include <set>
#include <cstdio>
#include <cassert>
#include <clocale>

#define EFLAGS_SINGLESTEP 0x00000100
#define MAX_ARGS 16

#ifdef _WIN64
    #define REG_IP(ctx) (ctx).Rip
#else
    #define REG_IP(ctx) (ctx).Eip
#endif

struct DataEntryEx : DataEntry
{
    DWORD_PTR ret_addr = 0;
    BYTE byte1 = 0, byte2 = 0;
    LPVOID fn = NULL;
    INT refcount = 0;

    DataEntryEx()
    {
    }

    DataEntryEx(const DataEntry& super);
    DataEntryEx& operator=(const DataEntry& super);
};

DataEntryEx::DataEntryEx(const DataEntry& super)
{
    *reinterpret_cast<DataEntry*>(this) = super;
}

DataEntryEx& DataEntryEx::operator=(const DataEntry& super)
{
    *reinterpret_cast<DataEntry*>(this) = super;
    return *this;
}

struct TinyDImpl
{
    PROCESS_INFORMATION m_pi;
    HANDLE& m_hProcess = m_pi.hProcess;
    HANDLE& m_hThread = m_pi.hThread;
    DWORD& m_dwPID = m_pi.dwProcessId;
    DWORD& m_dwTID = m_pi.dwThreadId;
    std::vector<DataEntryEx> m_entries;
    std::map<DWORD, HANDLE> m_id_to_thread;
    std::set<std::string> m_forbidden;

    TinyDImpl()
    {
        ZeroMemory(&m_pi, sizeof(m_pi));
    }

    ~TinyDImpl()
    {
        if (m_hProcess)
            ::CloseHandle(m_hProcess);
        if (m_hThread)
            ::CloseHandle(m_hThread);
    }
};

void TinyD::AddForbidden(const char *func_name)
{
    impl().m_forbidden.insert(func_name);
}

static void
DoReadArgs(HANDLE hProcess, DataEntryEx& entry, const CONTEXT& ctx, DWORD_PTR *pdw)
{
#ifdef _WIN64
    std::string arg;
    ::DoReadWriteProcessMemory(hProcess, (LPVOID)ctx.Rsp, pdw, 1 * sizeof(DWORD_PTR), FALSE);
    size_t i = 0;
    if (i++ < entry.args.size())
    {
        if (entry.args[i - 1] == "double")
            pdw[i] = *(DWORD_PTR*)&ctx.Xmm0;
        else
            pdw[i] = ctx.Rcx;
    }
    if (i++ < entry.args.size())
    {
        if (entry.args[i - 1] == "double")
            pdw[i] = *(DWORD_PTR*)&ctx.Xmm1;
        else
            pdw[i] = ctx.Rdx;
    }
    if (i++ < entry.args.size())
    {
        if (entry.args[i - 1] == "double")
            pdw[i] = *(DWORD_PTR*)&ctx.Xmm2;
        else
            pdw[i] = ctx.R8;
    }
    if (i++ < entry.args.size())
    {
        if (entry.args[i - 1] == "double")
            pdw[i] = *(DWORD_PTR*)&ctx.Xmm3;
        else
            pdw[i] = ctx.R9;
    }
    if (i < entry.args.size())
        ::DoReadWriteProcessMemory(hProcess, (LPVOID)ctx.Rsp, &pdw[i], (entry.args.size() - i) * sizeof(DWORD_PTR), FALSE);
#else
    ::DoReadWriteProcessMemory(hProcess, (LPVOID)ctx.Esp, pdw, (entry.args.size() + 1) * sizeof(DWORD_PTR), FALSE);
#endif
}

void TinyD::OnEnterProc(DataEntryEx& entry, CONTEXT& ctx, DWORD_PTR *pdw)
{
    const size_t MAX_STRING = 128;
    Printf("%s(", entry.func_name.c_str());
    for (size_t i = 0; i < entry.args.size(); ++i)
    {
        if (i > 0)
            Printf(", ");

        auto type = entry.args[i].c_str();
        DWORD_PTR arg = pdw[i + 1];
        if (lstrcmpiA(type, "ptr") == 0)
        {
            Printf("%p", (void *)arg);
        }
        else if (lstrcmpiA(type, "str") == 0)
        {
            if (arg != 0)
            {
                if (HIWORD(arg) == 0)
                {
                    Printf("0x%p", arg);
                }
                else
                {
                    char buf[MAX_STRING] = "";
                    DoReadWriteProcessMemory(impl().m_hProcess, (LPVOID)arg, buf, sizeof(buf), FALSE);
                    buf[_countof(buf) - 1] = 0;
                    Printf("\"%s\"", mstr_escape(buf).c_str());
                }
            }
            else
            {
                Printf("NULL");
            }
        }
        else if (lstrcmpiA(type, "wstr") == 0)
        {
            if (arg != 0)
            {
                if (HIWORD(arg) == 0)
                {
                    Printf("0x%p", arg);
                }
                else
                {
                    wchar_t buf[MAX_STRING] = L"";
                    DoReadWriteProcessMemory(impl().m_hProcess, (LPVOID)arg, buf, sizeof(buf), FALSE);
                    buf[_countof(buf) - 1] = 0;
                    Printf("\"%ls\"", mstr_escape(buf).c_str());
                }
            }
            else
            {
                Printf("NULL");
            }
        }
        else if (lstrcmpiA(type, "long") == 0)
        {
            Printf("0x%08lX", arg);
        }
        else if (lstrcmpiA(type, "double") == 0)
        {
            Printf("0x%08lX", *(double*)&arg);
        }
        else
        {
            assert(0);
        }
    }

    Printf("): enter\n");
}

void TinyD::OnLeaveProc(DataEntryEx& entry, CONTEXT& ctx)
{
#ifdef _WIN64
    Printf("%s: leave: %p\n", entry.func_name.c_str(), ctx.Rax);
#else
    Printf("%s: leave: %p\n", entry.func_name.c_str(), ctx.Eax);
#endif
}

TinyDImpl& TinyD::impl()
{
    return *m_pimpl;
}

TinyD::TinyD()
{
    m_pimpl = new TinyDImpl();
}

TinyD::~TinyD()
{
    delete m_pimpl;
}

void TinyD::Print(const std::string& str)
{
    Printf("%s", str.c_str());
}

void TinyD::Printf(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    VPrintf(fmt, va);
    va_end(va);
}

void TinyD::VPrintf(const char *fmt, va_list va)
{
    std::vprintf(fmt, va);
    std::fflush(stdout);
}

void TinyD::AddDebugEntry(const DataEntry& src)
{
    impl().m_entries.emplace_back(src);
}

void TinyD::SetDebugEntries(size_t count, const DataEntry *src)
{
    impl().m_entries.clear();
    for (size_t i = 0; i < count; ++i)
    {
        AddDebugEntry(src[i]);
    }
}

bool TinyD::StartDebugProcess(const char *cmdline)
{
    STARTUPINFOA si = { sizeof(si) };
    DWORD dwFlags = DEBUG_PROCESS | CREATE_NEW_CONSOLE | DEBUG_ONLY_THIS_PROCESS;

    std::string strCmdLine = cmdline;
    if (!CreateProcessA(NULL, &strCmdLine[0], NULL, NULL, TRUE, dwFlags,
                        NULL, NULL, &si, &impl().m_pi))
    {
        return false;
    }
    return true;
}

bool TinyD::StartDebugProcess(const wchar_t *cmdline)
{
    STARTUPINFOW si = { sizeof(si) };
    DWORD dwFlags = DEBUG_PROCESS | CREATE_NEW_CONSOLE | DEBUG_ONLY_THIS_PROCESS;

    std::wstring strCmdLine = cmdline;
    if (!CreateProcessW(NULL, &strCmdLine[0], NULL, NULL, TRUE, dwFlags,
                        NULL, NULL, &si, &impl().m_pi))
    {
        return false;
    }
    return true;
}

void TinyD::OnCreateProcess(DEBUG_EVENT& de, CREATE_PROCESS_DEBUG_INFO& CreateProcessInfo)
{
    impl().m_id_to_thread[de.dwThreadId] = CreateProcessInfo.hThread;
}

void TinyD::OnCreateThread(DEBUG_EVENT& de, CREATE_THREAD_DEBUG_INFO& CreateThread)
{
    impl().m_id_to_thread[de.dwThreadId] = CreateThread.hThread;
}

void TinyD::OnExitThread(DEBUG_EVENT& de, EXIT_THREAD_DEBUG_INFO& ExitThread)
{
    impl().m_id_to_thread.erase(de.dwThreadId);
}

DWORD TinyD::OnExitProcess(DEBUG_EVENT& de, EXIT_PROCESS_DEBUG_INFO& ExitProcess)
{
    impl().m_id_to_thread.erase(de.dwThreadId);
    return ExitProcess.dwExitCode;
}

void TinyD::OnBreakPoint(DEBUG_EVENT& de)
{
    BYTE byte;

    auto it = impl().m_id_to_thread.find(de.dwThreadId);
    if (it == impl().m_id_to_thread.end())
        return;

    HANDLE hThread = it->second;

    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;
    ::GetThreadContext(hThread, &ctx);

    for (auto& entry : impl().m_entries)
    {
        if (REG_IP(ctx) - 1 == (DWORD_PTR)entry.fn)
        {
            if (impl().m_forbidden.count(entry.func_name) > 0)
            {
                Printf("%s(...)\n", entry.func_name.c_str());

                REG_IP(ctx)--;
                DoReadWriteProcessMemory(impl().m_hProcess, (LPVOID)REG_IP(ctx), &entry.byte1, sizeof(entry.byte1), TRUE);
                FlushInstructionCache(impl().m_hProcess, NULL, 0);

                ctx.EFlags |= EFLAGS_SINGLESTEP;
                SetThreadContext(hThread, &ctx);
            }
            else
            {
                DWORD_PTR pdw[MAX_ARGS] = { 0 };
                DoReadArgs(impl().m_hProcess, entry, ctx, pdw);
                OnEnterProc(entry, ctx, pdw);

                entry.ret_addr = pdw[0];
                DoReadWriteProcessMemory(impl().m_hProcess, (LPVOID)entry.ret_addr, &byte, sizeof(byte), FALSE);
                entry.byte2 = byte;

                byte = 0xCC; // INT3
                DoReadWriteProcessMemory(impl().m_hProcess, (LPVOID)entry.ret_addr, &byte, sizeof(byte), TRUE);

                REG_IP(ctx)--;
                DoReadWriteProcessMemory(impl().m_hProcess, (LPVOID)REG_IP(ctx), &entry.byte1, sizeof(entry.byte1), TRUE);
                FlushInstructionCache(impl().m_hProcess, NULL, 0);

                ctx.EFlags |= EFLAGS_SINGLESTEP;
                SetThreadContext(hThread, &ctx);

                entry.refcount += 1;
            }
            break;
        }
        else if (REG_IP(ctx) - 1 == (DWORD_PTR)entry.ret_addr)
        {
            entry.refcount -= 1;
            if (entry.refcount == 0)
                DoReadWriteProcessMemory(impl().m_hProcess, (LPVOID)entry.ret_addr, &entry.byte2, sizeof(entry.byte2), TRUE);

            OnLeaveProc(entry, ctx);
            REG_IP(ctx)--;

            FlushInstructionCache(impl().m_hProcess, NULL, 0);

            SetThreadContext(hThread, &ctx);
            break;
        }
    }
}

void TinyD::OnSingleStep(DEBUG_EVENT& de)
{
    auto it = impl().m_id_to_thread.find(de.dwThreadId);
    if (it == impl().m_id_to_thread.end())
        return;
    auto hThread = it->second;

    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;
    ::GetThreadContext(hThread, &ctx);

    for (DWORD i = 0; i < impl().m_entries.size(); ++i)
    {
        auto& entry = impl().m_entries[i];
        //Printf("%p, %p\n", REG_IP(ctx), entry.fn);
        if (REG_IP(ctx) - 1 == (DWORD_PTR)entry.fn ||
            REG_IP(ctx) - 2 == (DWORD_PTR)entry.fn ||
            REG_IP(ctx) - 3 == (DWORD_PTR)entry.fn ||
            REG_IP(ctx) - 4 == (DWORD_PTR)entry.fn ||
            REG_IP(ctx) - 5 == (DWORD_PTR)entry.fn ||
            REG_IP(ctx) - 6 == (DWORD_PTR)entry.fn)
        {
            BYTE byte = 0xCC; // INT3
            ::DoReadWriteProcessMemory(impl().m_hProcess, (LPVOID)entry.fn, &byte, sizeof(byte), TRUE);
            ::FlushInstructionCache(impl().m_hProcess, NULL, 0);

            ctx.EFlags &= ~EFLAGS_SINGLESTEP;
            ::SetThreadContext(hThread, &ctx);
            break;
        }
    }
}

void TinyD::DumpContext(DWORD dwThreadId)
{
    if (dwThreadId == 0)
        dwThreadId = impl().m_dwTID;

    auto it = impl().m_id_to_thread.find(dwThreadId);
    if (it == impl().m_id_to_thread.end())
        return;
    auto hThread = it->second;

    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;
    ::GetThreadContext(hThread, &ctx);

#ifdef _WIN64
    Printf(
        "RDI:%p RSI:%p RBX:%p RDX:%p RCX:%p\n"
        "RAX:%p RBP:%p RIP:%p RSP:%p EFlags:%p\n",
        ctx.Rdi, ctx.Rsi, ctx.Rbx, ctx.Rdx, ctx.Rcx,
        ctx.Rax, ctx.Rbp, ctx.Rip, ctx.Rsp, ctx.EFlags);
#else
    Printf(
        "EDI:%p ESI:%p EBX:%p EDX:%p ECX:%p\n"
        "EAX:%p EBP:%p EIP:%p ESP:%p EFlags:%p\n",
        ctx.Edi, ctx.Esi, ctx.Ebx, ctx.Edx, ctx.Ecx,
        ctx.Eax, ctx.Ebp, ctx.Eip, ctx.Esp, ctx.EFlags);
#endif
}

bool TinyD::OnDebugEvent(DEBUG_EVENT& de)
{
    auto& Exception = de.u.Exception;
    auto& ExceptionRecord = Exception.ExceptionRecord;
    auto& ExceptionCode = ExceptionRecord.ExceptionCode;

    switch (ExceptionCode)
    {
    case EXCEPTION_ACCESS_VIOLATION:
        Printf("EXCEPTION_ACCESS_VIOLATION\n");
        DumpContext();
        return false;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        Printf("EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n");
        DumpContext();
        break;
    case EXCEPTION_BREAKPOINT:
        //Printf("EXCEPTION_BREAKPOINT\n");
        OnBreakPoint(de);
        break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        Printf("EXCEPTION_DATATYPE_MISALIGNMENT\n");
        DumpContext();
        break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        Printf("EXCEPTION_FLT_DENORMAL_OPERAND\n");
        DumpContext();
        break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        Printf("EXCEPTION_FLT_DIVIDE_BY_ZERO\n");
        DumpContext();
        break;
    case EXCEPTION_FLT_INEXACT_RESULT:
        Printf("EXCEPTION_FLT_INEXACT_RESULT\n");
        DumpContext();
        break;
    case EXCEPTION_FLT_INVALID_OPERATION:
        Printf("EXCEPTION_FLT_INVALID_OPERATION\n");
        DumpContext();
        break;
    case EXCEPTION_FLT_OVERFLOW:
        Printf("EXCEPTION_FLT_OVERFLOW\n");
        DumpContext();
        break;
    case EXCEPTION_FLT_STACK_CHECK:
        Printf("EXCEPTION_FLT_STACK_CHECK\n");
        DumpContext();
        break;
    case EXCEPTION_FLT_UNDERFLOW:
        Printf("EXCEPTION_FLT_UNDERFLOW\n");
        DumpContext();
        break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        Printf("EXCEPTION_ILLEGAL_INSTRUCTION\n");
        DumpContext();
        break;
    case EXCEPTION_IN_PAGE_ERROR:
        Printf("EXCEPTION_IN_PAGE_ERROR\n");
        DumpContext();
        break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        Printf("EXCEPTION_INT_DIVIDE_BY_ZERO\n");
        DumpContext();
        break;
    case EXCEPTION_INT_OVERFLOW:
        Printf("EXCEPTION_INT_OVERFLOW\n");
        DumpContext();
        break;
    case EXCEPTION_INVALID_DISPOSITION:
        Printf("EXCEPTION_INVALID_DISPOSITION\n");
        DumpContext();
        break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        Printf("EXCEPTION_NONCONTINUABLE_EXCEPTION\n");
        DumpContext();
        break;
    case EXCEPTION_PRIV_INSTRUCTION:
        Printf("EXCEPTION_PRIV_INSTRUCTION\n");
        DumpContext();
        break;
    case EXCEPTION_SINGLE_STEP:
        //Printf("EXCEPTION_SINGLE_STEP\n");
        OnSingleStep(de);
        break;
    case EXCEPTION_STACK_OVERFLOW:
        Printf("EXCEPTION_STACK_OVERFLOW\n");
        DumpContext();
        break;
    case EXCEPTION_GUARD_PAGE:
        Printf("EXCEPTION_GUARD_PAGE\n");
        DumpContext();
        break;
    case EXCEPTION_INVALID_HANDLE:
        Printf("EXCEPTION_INVALID_HANDLE\n");
        DumpContext();
        break;
    case CONTROL_C_EXIT:
        Printf("CONTROL_C_EXIT\n");
        DumpContext();
        break;
    default:
        Printf("ExceptionCode: 0x%08lX\n", ExceptionCode);
        DumpContext();
        break;
    }

    return true;
}

void TinyD::OnDebugString(OUTPUT_DEBUG_STRING_INFO& DebugString)
{
    if (DebugString.fUnicode)
    {
        std::wstring str;
        str.resize(DebugString.nDebugStringLength);

        SIZE_T got;
        ::ReadProcessMemory(impl().m_hProcess, DebugString.lpDebugStringData, &str[0],
                            DebugString.nDebugStringLength * 2, &got);
        if (str.size() && str[str.size() - 1] == 0)
            str.resize(str.size() - 1);

        Printf("OutputDebugStringW(\"%ls\")\n", mstr_escape(str).c_str());
    }
    else
    {
        std::string str;
        str.resize(DebugString.nDebugStringLength);

        SIZE_T got;
        ::ReadProcessMemory(impl().m_hProcess, DebugString.lpDebugStringData, &str[0],
                            DebugString.nDebugStringLength, &got);
        if (str.size() && str[str.size() - 1] == 0)
            str.resize(str.size() - 1);

        Printf("OutputDebugStringA(\"%s\")\n", mstr_escape(str).c_str());
    }
}

void TinyD::OnLoadDll(DEBUG_EVENT& de, LOAD_DLL_DEBUG_INFO& LoadDll)
{
    CHAR szPath[MAX_PATH];
    if (!::GetModuleFileNameExA(GetCurrentProcess(), (HMODULE)LoadDll.lpBaseOfDll,
                                szPath, _countof(szPath)))
        return;

    ::PathRemoveExtensionA(szPath);
    char *module_name = ::PathFindFileNameA(szPath);
    ::CharLowerA(module_name);

    //Printf("module_name: %p, %s\n", LoadDll.lpBaseOfDll, module_name);
    //fflush(stdout);

    MR_EXPORTS exports;
    MR_get_exports(exports, module_name, LoadDll.lpBaseOfDll);

    for (auto& exp : exports)
    {
        if (lstrcmpiA(exp.module_name.c_str(), module_name) != 0)
            continue;

        for (DWORD i = 0; i < impl().m_entries.size(); ++i)
        {
            auto& entry = impl().m_entries[i];

            if (exp.func_name != entry.func_name)
                continue;

            LPBYTE pb = reinterpret_cast<LPBYTE>(LoadDll.lpBaseOfDll);
#ifdef _WIN64
            entry.fn = reinterpret_cast<LPVOID>(pb + exp.func_va);
#else
            entry.fn = reinterpret_cast<LPVOID>(pb + exp.func_va32);
#endif

            BYTE byte;
            DoReadWriteProcessMemory(impl().m_hProcess, (LPVOID)entry.fn, &byte, sizeof(byte), FALSE);
            entry.byte1 = byte;

            byte = 0xCC; // INT3
            DoReadWriteProcessMemory(impl().m_hProcess, (LPVOID)entry.fn, &byte, sizeof(byte), TRUE);

            FlushInstructionCache(impl().m_hProcess, NULL, 0);
        }
    }
}

void TinyD::OnUnloadDll(DEBUG_EVENT& de, UNLOAD_DLL_DEBUG_INFO& UnloadDll)
{
}

DWORD TinyD::DebugLoop(void)
{
    DWORD dwExitCode = 0;
    DEBUG_EVENT de;
    BOOL bContinue = TRUE;

    while (bContinue)
    {
        if (!::WaitForDebugEvent(&de, INFINITE))
            break;

        DWORD dwStatus = DBG_CONTINUE;

        switch (de.dwDebugEventCode)
        {
        case EXCEPTION_DEBUG_EVENT:
            //Printf("EXCEPTION_DEBUG_EVENT\n");
            if (!OnDebugEvent(de))
            {
                dwStatus = DBG_EXCEPTION_NOT_HANDLED;
                bContinue = FALSE;
            }
            break;
        case CREATE_THREAD_DEBUG_EVENT:
            //Printf("CREATE_THREAD_DEBUG_EVENT\n");
            OnCreateThread(de, de.u.CreateThread);
            break;
        case CREATE_PROCESS_DEBUG_EVENT:
            //Printf("CREATE_PROCESS_DEBUG_EVENT\n");
            OnCreateProcess(de, de.u.CreateProcessInfo);
            break;
        case EXIT_THREAD_DEBUG_EVENT:
            //Printf("EXIT_THREAD_DEBUG_EVENT\n");
            OnExitThread(de, de.u.ExitThread);
            break;
        case EXIT_PROCESS_DEBUG_EVENT:
            //Printf("EXIT_PROCESS_DEBUG_EVENT\n");
            dwExitCode = OnExitProcess(de, de.u.ExitProcess);
            bContinue = FALSE;
            break;
        case LOAD_DLL_DEBUG_EVENT:
            //Printf("LOAD_DLL_DEBUG_EVENT\n");
            OnLoadDll(de, de.u.LoadDll);
            break;
        case UNLOAD_DLL_DEBUG_EVENT:
            //Printf("UNLOAD_DLL_DEBUG_EVENT\n");
            OnUnloadDll(de, de.u.UnloadDll);
            break;
        case OUTPUT_DEBUG_STRING_EVENT:
            //Printf("OUTPUT_DEBUG_STRING_EVENT\n");
            OnDebugString(de.u.DebugString);
            break;
        default:
            Printf("dwDebugEventCode: %d\n", de.dwDebugEventCode);
            break;
        }

        ::ContinueDebugEvent(de.dwProcessId, de.dwThreadId, dwStatus);
    }

    return dwExitCode;
}
