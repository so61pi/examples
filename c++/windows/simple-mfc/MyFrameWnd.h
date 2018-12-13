#ifndef MY_FRAME_WND_H
#define MY_FRAME_WND_H


#include <string>
#include <afxwin.h>


class MyFrameWnd : public CFrameWnd
{
    using base = CFrameWnd;

public:
    MyFrameWnd(std::wstring const& name);

    DECLARE_MESSAGE_MAP()
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnMouseMove(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);

    void OnFileExit();
    void OnHelpAbout();

private:
    bool m_hold{ false };
    CPoint m_lastPoint{ 0 , 0 };

    CMenu m_menu;
};


#endif // MY_FRAME_WND_H
