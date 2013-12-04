#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
	DeviceInfo* dev = new DeviceInfo("test", "win32", "0", "computer");
	app.setDeviceInfo(dev);

    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("PokerGame");
    eglView->setFrameSize(1280, 720);
    return CCApplication::sharedApplication()->run();
}
