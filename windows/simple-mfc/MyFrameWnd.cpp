#include "MyFrameWnd.h"
#include "Resource.h"


BEGIN_MESSAGE_MAP(MyFrameWnd, CFrameWnd)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()

    ON_COMMAND(ID_FILE_EXIT, OnFileExit)
    ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
END_MESSAGE_MAP()


MyFrameWnd::MyFrameWnd(std::wstring const& name) {
    Create(nullptr, name.c_str());

    m_menu.LoadMenuW(IDR_MENU);
    SetMenu(&m_menu);
}


void MyFrameWnd::OnLButtonDown(UINT nFlags, CPoint point) {
    base::OnLButtonDown(nFlags, point);

    m_hold = true;
    m_lastPoint = point;
}


void MyFrameWnd::OnMouseMove(UINT nFlags, CPoint point) {
    base::OnMouseMove(nFlags, point);

    if (m_hold) {
        CClientDC dc(this);
        dc.MoveTo(m_lastPoint);
        dc.LineTo(point);
        m_lastPoint = point;
    }
}


void MyFrameWnd::OnLButtonUp(UINT nFlags, CPoint point) {
    base::OnLButtonUp(nFlags, point);

    m_hold = false;
}


void MyFrameWnd::OnFileExit() {
    AfxGetMainWnd()->SendMessage(WM_CLOSE);
}


void MyFrameWnd::OnHelpAbout() {
    AfxMessageBox(L"MFC example.", MB_ICONINFORMATION);
}
