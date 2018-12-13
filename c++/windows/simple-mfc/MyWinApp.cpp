#include "MyWinApp.h"
#include "MyFrameWnd.h"


// enable visual style
#pragma comment(linker, "\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
    language='*'\"")


BOOL MyWinApp::InitInstance() {
    m_pMainWnd = new MyFrameWnd{ L"Hello" };
    m_pMainWnd->ShowWindow(SW_SHOW);
    return TRUE;
}


MyWinApp myWinApp;
