set breakpoint pending on
set pagination off
set logging file log.txt
set logging on

define ac_print
  set $c = (char*)$arg0
  if {int}$c == 0
    printf "NULL"
  else
    if ({int}$c & 0xFFFF0000) == 0
      printf "0x%X", {int}$c
    else
      printf "'%s'", $c
    end
  end
end

define wc_print
  set $c = (short*)$arg0
  if {int}$c == 0
    printf "NULL"
  else
    if ({int}$c & 0xFFFF0000) == 0
      printf "0x%X", {int}$c
    else
      printf "'"
      while (*$c)
        if (*$c > 0x7f)
          printf "[%x]", *$c
        else
          if (*$c < 0x20)
            printf "[%x]", *$c
          else
            printf "%c", *$c
          end
        end
        set $c++
      end
      printf "'"
    end
  end
end

break keybd_event
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: keybd_event("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break UpdateWindow
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: UpdateWindow("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break UnloadKeyboardLayout
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: UnloadKeyboardLayout("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ToUnicode
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: ToUnicode("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break SystemParametersInfoW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: SystemParametersInfoW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ShowWindow
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ShowWindow("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break SetWindowPos
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  set $p6 = {int}($sp+0x20)
  printf "log: SetWindowPos("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ", "
  printf "0x%X", $p6
  printf ")\n"
  cont
end

break SendMessageTimeoutW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  set $p6 = {int}($sp+0x20)
  printf "log: SendMessageTimeoutW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ", "
  printf "0x%X", $p6
  printf ")\n"
  cont
end

break SendMessageA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: SendMessageA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ScreenToClient
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ScreenToClient("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break PostMessageW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: PostMessageW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break PostMessageA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: PostMessageA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break MonitorFromWindow
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: MonitorFromWindow("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break MessageBeep
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: MessageBeep("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break MapVirtualKeyW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: MapVirtualKeyW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break LoadKeyboardLayoutW
commands
  silent
  set $p0 = {short*}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: LoadKeyboardLayoutW("
  wc_print $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmLockImeDpi
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmLockImeDpi("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmReSizeIMCC
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmReSizeIMCC("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmGetCandidateListCountW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmGetCandidateListCountW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break CreateWindowExW
commands
  silent
  printf "log: CreateWindowExW(..."
  printf ")\n"
  cont
end

break ImmSetStatusWindowPos
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmSetStatusWindowPos("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break CtfImmIsCiceroEnabled
commands
  silent
  printf "log: CtfImmIsCiceroEnabled("
  printf ")\n"
  cont
end

break ImmSetActiveContextConsoleIME
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmSetActiveContextConsoleIME("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break DestroyWindow
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: DestroyWindow("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break GetWindow
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: GetWindow("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmRequestMessageA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmRequestMessageA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break User32InitializeImmEntryTable
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: User32InitializeImmEntryTable("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break CharNextA
commands
  silent
  set $p0 = {char*}($sp+0x8)
  printf "log: CharNextA("
  ac_print $p0
  printf ")\n"
  cont
end

break ClientToScreen
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ClientToScreen("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmSetCompositionFontW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmSetCompositionFontW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmGetDescriptionW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmGetDescriptionW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmTranslateMessage
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmTranslateMessage("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmGetIMEFileNameW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmGetIMEFileNameW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmGetStatusWindowPos
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmGetStatusWindowPos("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break LoadBitmapW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {short*}($sp+0xC)
  printf "log: LoadBitmapW("
  printf "0x%X", $p0
  printf ", "
  wc_print $p1
  printf ")\n"
  cont
end

break ImmGetIMCCLockCount
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmGetIMCCLockCount("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetGuideLineA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmGetGuideLineA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break GetClassInfoExW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {short*}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: GetClassInfoExW("
  printf "0x%X", $p0
  printf ", "
  wc_print $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmGetHotKey
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmGetHotKey("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmGetDefaultIMEWnd
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmGetDefaultIMEWnd("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetCompositionStringW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmGetCompositionStringW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmEscapeA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmEscapeA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmRequestMessageW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmRequestMessageW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmGetIMCCSize
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmGetIMCCSize("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetGuideLineW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmGetGuideLineW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmDestroyIMCC
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmDestroyIMCC("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmEnumRegisterWordW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {short*}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {short*}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: ImmEnumRegisterWordW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  wc_print $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  wc_print $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break ImmDestroySoftKeyboard
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmDestroySoftKeyboard("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetRegisterWordStyleW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmGetRegisterWordStyleW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmGetConversionListW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {short*}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: ImmGetConversionListW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  wc_print $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break IsWindow
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: IsWindow("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmDisableIME
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmDisableIME("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmConfigureIMEA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmConfigureIMEA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmDisableLegacyIME
commands
  silent
  printf "log: ImmDisableLegacyIME("
  printf ")\n"
  cont
end

break ImmGetCompositionWindow
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmGetCompositionWindow("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmAssociateContext
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmAssociateContext("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break GetKeyboardLayoutList
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: GetKeyboardLayoutList("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmDisableTextFrameService
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmDisableTextFrameService("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetCandidateListW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmGetCandidateListW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmLockIMCC
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmLockIMCC("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetCandidateWindow
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmGetCandidateWindow("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmRegisterClient
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmRegisterClient("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmGetIMCLockCount
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmGetIMCLockCount("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break GetClassInfoW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {short*}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: GetClassInfoW("
  printf "0x%X", $p0
  printf ", "
  wc_print $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break SetCursor
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: SetCursor("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetVirtualKey
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmGetVirtualKey("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break SetCapture
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: SetCapture("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break GetFocus
commands
  silent
  printf "log: GetFocus("
  printf ")\n"
  cont
end

break ImmEnumInputContext
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmEnumInputContext("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmGetRegisterWordStyleA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmGetRegisterWordStyleA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmGetIMEFileNameA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmGetIMEFileNameA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmGetContext
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmGetContext("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmEnumRegisterWordA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {char*}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {char*}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: ImmEnumRegisterWordA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  ac_print $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  ac_print $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break ImmDisableIme
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmDisableIme("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetImeMenuItemsA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: ImmGetImeMenuItemsA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break ImmGetConversionListA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {char*}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: ImmGetConversionListA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  ac_print $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break ImmSetOpenStatus
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmSetOpenStatus("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmGenerateMessage
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmGenerateMessage("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmIsUIMessageW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmIsUIMessageW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmGetCompositionFontW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmGetCompositionFontW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmConfigureIMEW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmConfigureIMEW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break CharUpperW
commands
  silent
  printf "log: CharUpperW(..."
  printf ")\n"
  cont
end

break ImmSimulateHotKey
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmSimulateHotKey("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmSetCompositionStringA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: ImmSetCompositionStringA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break ImmSetConversionStatus
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmSetConversionStatus("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmCreateContext
commands
  silent
  printf "log: ImmCreateContext("
  printf ")\n"
  cont
end

break ImmUnlockImeDpi
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmUnlockImeDpi("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmCreateIMCC
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmCreateIMCC("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetProperty
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmGetProperty("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmNotifyIME
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmNotifyIME("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmProcessKey
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  printf "log: ImmProcessKey("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ")\n"
  cont
end

break ImmSetActiveContext
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmSetActiveContext("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmGetImeMenuItemsW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: ImmGetImeMenuItemsW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break ImmSetCompositionFontA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmSetCompositionFontA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break DrawEdge
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: DrawEdge("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ToAsciiEx
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: ToAsciiEx("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break ImmRegisterWordW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {short*}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {short*}($sp+0x14)
  printf "log: ImmRegisterWordW("
  printf "0x%X", $p0
  printf ", "
  wc_print $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  wc_print $p3
  printf ")\n"
  cont
end

break ImmAssociateContextEx
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmAssociateContextEx("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmSetCompositionWindow
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmSetCompositionWindow("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmRegisterWordA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {char*}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {char*}($sp+0x14)
  printf "log: ImmRegisterWordA("
  printf "0x%X", $p0
  printf ", "
  ac_print $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  ac_print $p3
  printf ")\n"
  cont
end

break ImmReleaseContext
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmReleaseContext("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmShowSoftKeyboard
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmShowSoftKeyboard("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmGetOpenStatus
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmGetOpenStatus("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetDescriptionA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmGetDescriptionA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break SetWindowLongW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: SetWindowLongW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmUnlockClientImc
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmUnlockClientImc("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmCreateSoftKeyboard
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmCreateSoftKeyboard("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmGetCandidateListCountA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmGetCandidateListCountA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmUnlockIMC
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmUnlockIMC("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmGetCompositionFontA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmGetCompositionFontA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmGetConversionStatus
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmGetConversionStatus("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break BeginPaint
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: BeginPaint("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break GetMonitorInfoW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: GetMonitorInfoW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break RegisterClassExW
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: RegisterClassExW("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmIsIME
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmIsIME("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break SendMessageW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: SendMessageW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break DrawTextExW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {short*}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: DrawTextExW("
  printf "0x%X", $p0
  printf ", "
  wc_print $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break ImmSetCompositionStringW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: ImmSetCompositionStringW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break EndPaint
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: EndPaint("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmEscapeW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmEscapeW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmGetImeInfoEx
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: ImmGetImeInfoEx("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmInstallIMEW
commands
  silent
  set $p0 = {short*}($sp+0x8)
  set $p1 = {short*}($sp+0xC)
  printf "log: ImmInstallIMEW("
  wc_print $p0
  printf ", "
  wc_print $p1
  printf ")\n"
  cont
end

break ImmIsUIMessageA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmIsUIMessageA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break GetActiveWindow
commands
  silent
  printf "log: GetActiveWindow("
  printf ")\n"
  cont
end

break ReleaseDC
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ReleaseDC("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break GetCapture
commands
  silent
  printf "log: GetCapture("
  printf ")\n"
  cont
end

break GetClientRect
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: GetClientRect("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmUnregisterWordA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {char*}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {char*}($sp+0x14)
  printf "log: ImmUnregisterWordA("
  printf "0x%X", $p0
  printf ", "
  ac_print $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  ac_print $p3
  printf ")\n"
  cont
end

break ReleaseCapture
commands
  silent
  printf "log: ReleaseCapture("
  printf ")\n"
  cont
end

break GetCursorPos
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: GetCursorPos("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break GetDC
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: GetDC("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmUnlockIMCC
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmUnlockIMCC("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break GetDesktopWindow
commands
  silent
  printf "log: GetDesktopWindow("
  printf ")\n"
  cont
end

break ImmUnregisterWordW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {short*}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {short*}($sp+0x14)
  printf "log: ImmUnregisterWordW("
  printf "0x%X", $p0
  printf ", "
  wc_print $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  wc_print $p3
  printf ")\n"
  cont
end

break GetSystemMetrics
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: GetSystemMetrics("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break GetForegroundWindow
commands
  silent
  printf "log: GetForegroundWindow("
  printf ")\n"
  cont
end

break GetKeyboardLayout
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: GetKeyboardLayout("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break WCSToMBEx
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {short*}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {char*}($sp+0x14)
  set $p4 = {int}($sp+0x18)
  set $p5 = {int}($sp+0x1C)
  printf "log: WCSToMBEx("
  printf "0x%X", $p0
  printf ", "
  wc_print $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  ac_print $p3
  printf ", "
  printf "0x%X", $p4
  printf ", "
  printf "0x%X", $p5
  printf ")\n"
  cont
end

break GetKeyboardState
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: GetKeyboardState("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmDestroyContext
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmDestroyContext("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break ImmInstallIMEA
commands
  silent
  set $p0 = {char*}($sp+0x8)
  set $p1 = {char*}($sp+0xC)
  printf "log: ImmInstallIMEA("
  ac_print $p0
  printf ", "
  ac_print $p1
  printf ")\n"
  cont
end

break ImmLockIMC
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmLockIMC("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break DefWindowProcW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: DefWindowProcW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break ImmGetCompositionStringA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmGetCompositionStringA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break GetParent
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: GetParent("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break LoadIconW
commands
  silent
  printf "log: LoadIconW(..."
  printf ")\n"
  cont
end

break ImmSetCandidateWindow
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: ImmSetCandidateWindow("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break LoadCursorW
commands
  silent
  printf "log: LoadCursorW(..."
  printf ")\n"
  cont
end

break MapWindowPoints
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: MapWindowPoints("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break CharNextW
commands
  silent
  set $p0 = {short*}($sp+0x8)
  printf "log: CharNextW("
  wc_print $p0
  printf ")\n"
  cont
end

break GetWindowLongW
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: GetWindowLongW("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break GetWindowRect
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: GetWindowRect("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break GetWindowThreadProcessId
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  printf "log: GetWindowThreadProcessId("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ")\n"
  cont
end

break ImmGetCandidateListA
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  set $p3 = {int}($sp+0x14)
  printf "log: ImmGetCandidateListA("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ", "
  printf "0x%X", $p3
  printf ")\n"
  cont
end

break InvalidateRect
commands
  silent
  set $p0 = {int}($sp+0x8)
  set $p1 = {int}($sp+0xC)
  set $p2 = {int}($sp+0x10)
  printf "log: InvalidateRect("
  printf "0x%X", $p0
  printf ", "
  printf "0x%X", $p1
  printf ", "
  printf "0x%X", $p2
  printf ")\n"
  cont
end

break ImmLockClientImc
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: ImmLockClientImc("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

break IsWindowUnicode
commands
  silent
  set $p0 = {int}($sp+0x8)
  printf "log: IsWindowUnicode("
  printf "0x%X", $p0
  printf ")\n"
  cont
end

run
quit
