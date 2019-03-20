## Open `qtcreator-4.1.0/lib/qtcreator/plugins/libCore.so` in IDA.

## Search `EditorManager::addCurrentPositionToNavigationHistory` function.

- It's at `.text:00000000000D3540`.
- There are 2 `call`s instructions and 1 `jmp` instruction:
  - The 1st `call` (`.text:00000000000D3544 call sub_CDCD`) calls to `EditorManagerPrivate::currentEditorView`.
  - The 2nd `call` (`.text:00000000000D354F call sub_E54C0`) calls to `EditorView::addCurrentPositionToNavigationHistory`.
  - The `jmp` (`.text:00000000000D3555 jmp sub_D03C0`) is just a call to `EditorManagerPrivate::updateActions`.
- Reference
  - [EditorManager::addCurrentPositionToNavigationHistory](https://github.com/qtproject/qt-creator/blob/v4.1.0/src/plugins/coreplugin/editormanager/editormanager.cpp#L2845)
  - [EditorManagerPrivate::currentEditorView](https://github.com/qtproject/qt-creator/blob/v4.1.0/src/plugins/coreplugin/editormanager/editormanager.cpp#L2262)
  - [EditorView::addCurrentPositionToNavigationHistory](https://github.com/qtproject/qt-creator/blob/v4.1.0/src/plugins/coreplugin/editormanager/editorview.cpp#L469)
  - [EditorManagerPrivate::updateActions](https://github.com/qtproject/qt-creator/blob/v4.1.0/src/plugins/coreplugin/editormanager/editormanager.cpp#L1707)

## Patching

- At `.text:00000000000E57D5`
  - From `83 FA 1D` to `83 FA 7D` (from `cmp edx, 29` to `cmp edx, 125`).
  - That means we change `while (m_navigationHistory.size() >= 30)` to `while (m_navigationHistory.size() >= 126)`.
- At `.text:00000000000E57DE`
  - From `83 BB A0 00 00 00 0F` to `83 BB A0 00 00 00 3F` (from `cmp dword ptr [rbx+0A0h], 15` to `cmp dword ptr [rbx+0A0h], 63`).
  - That means we change `if (m_currentNavigationHistoryPosition > 15)` to `if (m_currentNavigationHistoryPosition > 63)`.
