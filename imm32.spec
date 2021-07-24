@ stdcall CtfImmIsCiceroEnabled()
@ stdcall -stub ImmActivateLayout(long)
@ stdcall ImmAssociateContext(ptr ptr)
@ stdcall ImmAssociateContextEx(ptr ptr long)
@ stdcall ImmConfigureIMEA(ptr ptr long ptr)
@ stdcall ImmConfigureIMEW(ptr ptr long ptr)
@ stdcall ImmCreateContext()
@ stdcall ImmCreateIMCC(long)
@ stdcall ImmCreateSoftKeyboard(long long long long)
@ stdcall ImmDestroyContext(ptr)
@ stdcall ImmDestroyIMCC(ptr)
@ stdcall ImmDestroySoftKeyboard(ptr)
@ stdcall ImmDisableIME(long)
@ stdcall ImmDisableIme(long) ImmDisableIME
@ stdcall ImmDisableLegacyIME()
@ stdcall ImmDisableTextFrameService(long)
@ stdcall ImmEnumInputContext(long ptr long)
@ stdcall ImmEnumRegisterWordA(long ptr str long str ptr)
@ stdcall ImmEnumRegisterWordW(long ptr wstr long wstr ptr)
@ stdcall ImmEscapeA(long ptr long ptr)
@ stdcall ImmEscapeW(long ptr long ptr)
@ stdcall -stub ImmFreeLayout(long)
@ stdcall ImmGenerateMessage(ptr)
@ stdcall ImmGetCandidateListA(long long ptr long)
@ stdcall ImmGetCandidateListCountA(long ptr)
@ stdcall ImmGetCandidateListCountW(long ptr)
@ stdcall ImmGetCandidateListW(long long ptr long)
@ stdcall ImmGetCandidateWindow(long long ptr)
@ stdcall ImmGetCompositionFontA(ptr ptr)
@ stdcall ImmGetCompositionFontW(ptr ptr)
@ stdcall ImmGetCompositionString(ptr long ptr long) ImmGetCompositionStringA
@ stdcall ImmGetCompositionStringA(ptr long ptr long)
@ stdcall ImmGetCompositionStringW(ptr long ptr long)
@ stdcall ImmGetCompositionWindow(ptr ptr)
@ stdcall ImmGetContext(ptr)
@ stdcall ImmGetConversionListA(long ptr str ptr long long)
@ stdcall ImmGetConversionListW(long ptr wstr ptr long long)
@ stdcall ImmGetConversionStatus(ptr ptr ptr)
@ stdcall ImmGetDefaultIMEWnd(ptr)
@ stdcall ImmGetDescriptionA(long ptr long)
@ stdcall ImmGetDescriptionW(long ptr long)
@ stdcall ImmGetGuideLineA(ptr long ptr long)
@ stdcall ImmGetGuideLineW(ptr long ptr long)
@ stdcall ImmGetHotKey(long ptr ptr ptr)
@ stdcall ImmGetIMCCLockCount(ptr)
@ stdcall ImmGetIMCCSize(ptr)
@ stdcall ImmGetIMCLockCount(ptr)
@ stdcall ImmGetIMEFileNameA(long ptr long)
@ stdcall ImmGetIMEFileNameW(long ptr long)
@ stdcall ImmGetImeInfoEx(ptr long ptr)
@ stdcall ImmGetImeMenuItemsA(ptr long long ptr ptr long)
@ stdcall ImmGetImeMenuItemsW(ptr long long ptr ptr long)
@ stdcall ImmGetOpenStatus(ptr)
@ stdcall ImmGetProperty(long long)
@ stdcall ImmGetRegisterWordStyleA(long long ptr)
@ stdcall ImmGetRegisterWordStyleW(long long ptr)
@ stdcall ImmGetStatusWindowPos(ptr ptr)
@ stdcall ImmGetVirtualKey(ptr)
@ stdcall -stub ImmIMPGetIMEA(ptr ptr)
@ stdcall -stub ImmIMPGetIMEW(ptr ptr)
@ stdcall -stub ImmIMPQueryIMEA(ptr)
@ stdcall -stub ImmIMPQueryIMEW(ptr)
@ stdcall -stub ImmIMPSetIMEA(ptr ptr)
@ stdcall -stub ImmIMPSetIMEW(ptr ptr)
@ stdcall ImmInstallIMEA(str str)
@ stdcall ImmInstallIMEW(wstr wstr)
@ stdcall ImmIsIME(long)
@ stdcall ImmIsUIMessageA(long long long long)
@ stdcall ImmIsUIMessageW(long long long long)
@ stdcall -stub ImmLoadIME(long)
@ stdcall -stub ImmLoadLayout(long ptr)
@ stdcall ImmLockClientImc(ptr)
@ stdcall ImmLockIMC(ptr)
@ stdcall ImmLockIMCC(ptr)
@ stdcall ImmLockImeDpi(ptr)
@ stdcall ImmNotifyIME(ptr long long long)
@ stub ImmPenAuxInput
@ stdcall ImmProcessKey(ptr long long long long)
@ stdcall -stub ImmPutImeMenuItemsIntoMappedFile(ptr)
@ stdcall ImmReSizeIMCC(ptr long)
@ stdcall ImmRegisterClient(ptr ptr)
@ stdcall ImmRegisterWordA(long str long str)
@ stdcall ImmRegisterWordW(long wstr long wstr)
@ stdcall ImmReleaseContext(ptr ptr)
@ stdcall ImmRequestMessageA(ptr long long)
@ stdcall ImmRequestMessageW(ptr long long)
@ stdcall -stub ImmSendIMEMessageExA(ptr long)
@ stdcall -stub ImmSendIMEMessageExW(ptr long)
@ stub ImmSendMessageToActiveDefImeWndW
@ stdcall ImmSetActiveContext(ptr ptr long)
@ stdcall ImmSetActiveContextConsoleIME(ptr long)
@ stdcall ImmSetCandidateWindow(ptr ptr)
@ stdcall ImmSetCompositionFontA(ptr ptr)
@ stdcall ImmSetCompositionFontW(ptr ptr)
@ stdcall ImmSetCompositionStringA(ptr long ptr long ptr long)
@ stdcall ImmSetCompositionStringW(ptr long ptr long ptr long)
@ stdcall ImmSetCompositionWindow(ptr ptr)
@ stdcall ImmSetConversionStatus(ptr long long)
#@ stdcall ImmSetHotKey(long long long ptr) user32.CliImmSetHotKey
@ stdcall ImmSetOpenStatus(ptr long)
@ stdcall ImmSetStatusWindowPos(ptr ptr)
@ stdcall ImmShowSoftKeyboard(ptr long)
@ stdcall ImmSimulateHotKey(ptr long)
@ stdcall -stub ImmSystemHandler(ptr long long)
@ stdcall ImmTranslateMessage(ptr long long long)
@ stdcall ImmUnlockClientImc(ptr)
@ stdcall ImmUnlockIMC(ptr)
@ stdcall ImmUnlockIMCC(ptr)
@ stdcall ImmUnlockImeDpi(ptr)
@ stdcall ImmUnregisterWordA(long str long str)
@ stdcall ImmUnregisterWordW(long wstr long wstr)
@ stdcall -stub ImmWINNLSEnableIME(ptr long)
@ stdcall -stub ImmWINNLSGetEnableStatus(ptr)
@ stub ImmWINNLSGetIMEHotkey

################################################################
# Wine internal extensions
@ stdcall __wine_get_ui_window(ptr)
@ stdcall __wine_register_window(long)
@ stdcall __wine_unregister_window(long)
