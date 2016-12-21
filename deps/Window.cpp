//------------------------------------------------------------------------------
//--- MugenEngine
//------------------------------------------------------------------------------
#include "MugenEngineZero.h"

//------------------------------------------------------------------------------ Setup

Window::Window()
{
    log(0,"- Window Created \n");
}
Window::~Window()
{
    log(0,"- Window Deleted \n");
}
int Window::init(const char* name,
                 int screenW, int screenH,
                 int scaleWin, int scaleFull,
                 bool fullScreen)
{
    _name = name;
    _screenW = screenW;
    _screenH = screenH;
    _scaleWin = scaleWin;
    _scaleFull = scaleFull;
    _isMaxScale = false;
    _isFullScreen = fullScreen;
    _currentMonitor = 0;

    _windowDisplay = al_create_display(_screenW*_scaleWin, _screenH*_scaleWin);
    _windowBuffer = al_create_bitmap(_screenW, _screenH);

    //al_set_app_name(_name);
    al_set_window_title(_windowDisplay,_name);
    pollMonitor(_currentMonitor);

    int scale(0);
    _isFullScreen ? scale = scaleFull : scale = scaleWin;

    setWindow(_windowDisplay, _currentMonitor, _isFullScreen, scale);

    return log(0,"- init Window OK !\n");
}
int Window::done()
{
    if (_windowBuffer) al_destroy_bitmap(_windowBuffer);
    if (_windowDisplay) al_destroy_display(_windowDisplay);
    return log(0,"- done Window OK \n");
}

//------------------------------------------------------------------------------ Render

void Window::beginRender(ALLEGRO_COLOR color)
{
    al_set_target_bitmap(_windowBuffer);
    al_clear_to_color(color);
// debug
    al_get_window_position(_windowDisplay, &_x, &_y);
}
void Window::endRender(ALLEGRO_COLOR color)
{
    al_set_target_backbuffer(_windowDisplay);
    al_clear_to_color(color);

    if (_isFullScreen)
        al_draw_scaled_bitmap(_windowBuffer,
                              0, 0, _screenW, _screenH,
                              _viewX, _viewY, _viewW, _viewH,
                              0);
    else
        al_draw_scaled_bitmap(_windowBuffer,
                              0, 0, _screenW, _screenH,
                              0, 0, _screenW*_scaleWin, _screenH*_scaleWin,
                              0);
    al_flip_display();
}

//------------------------------------------------------------------------------ Manager

void Window::setMonitor(int monitor, int scale)
{
    // if prev Monitor is on maxScale then new Monitor go maxScale too !
    if (!_isFullScreen)
        _scaleWin == getMaxScale()-1 ? _isMaxScale = true : _isMaxScale = false;
    else
        _scaleFull == getMaxScale() ? _isMaxScale = true : _isMaxScale = false;

    _currentMonitor = monitor;
    pollMonitor(_currentMonitor); // peek Monitor infos !
    if (_isFullScreen)
    {
        scale = _scaleFull;
        // if prev Monitor Scale is different than Max scale Monitor possible !
        if (scale > getMaxScale() || _isMaxScale || scale == 0)
            scale = getMaxScale();
    }
    else
    {
        scale = _scaleWin;
        // if prev Monitor Scale is different than Max scale Monitor possible !
        if (scale > getMaxScale()-1 || _isMaxScale || scale == 0)
            scale = getMaxScale()-1;
    }
    setWindow(_windowDisplay, _currentMonitor, _isFullScreen, scale);
}

void Window::switchMonitor(int scale)
{
    _currentMonitor++;
    if (_currentMonitor > al_get_num_video_adapters()-1) _currentMonitor = 0;
    setMonitor(_currentMonitor, scale); // -1 scale default , 0 scale Max !
}

void Window::toggleFullScreen(int scale)
{
    _isFullScreen = !_isFullScreen;
    setScale(scale);
}

void Window::setScale(int scale)
{
    pollMonitor(_currentMonitor);
    if (_isFullScreen)
    {
        if (scale > getMaxScale() || scale == 0)
            scale = getMaxScale();

        if (scale == -1)
            _scaleFull > getMaxScale() ? scale = getMaxScale() : scale = _scaleFull;
    }
    else
    {
        if (scale > getMaxScale()-1 || scale == 0)
            scale = getMaxScale()-1;

        if (scale == -1)
            _scaleWin > getMaxScale()-1 ? scale = getMaxScale()-1 : scale = _scaleWin;
    }

    setWindow(_windowDisplay,
              currentMonitor(_windowDisplay), _isFullScreen, scale);

}

void Window::setWindow(ALLEGRO_DISPLAY * display, int adapter,bool isFullScreen, int scale)
{
    _isFullScreen = isFullScreen;
    _currentMonitor = adapter;

    if (_isFullScreen)
    {
        al_set_display_flag(_windowDisplay,ALLEGRO_NOFRAME, true);

        pollMonitor(adapter); // peek Monitor infos !
        scale == 0 ? _scaleFull = getMaxScale() : _scaleFull = scale;
        setViewAtCenter(_scaleFull);

        al_set_window_position(display,_currentMonitorX,_currentMonitorY);
        al_resize_display(display,_currentMonitorW,_currentMonitorH);

        // Windows System Specifique
        #ifdef WINSYSTEM
            HWND hwnd = al_get_win_window_handle(_windowDisplay);
            SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        #endif // WINSYSTEM
    }
    else
    {
        al_set_display_flag(_windowDisplay,ALLEGRO_NOFRAME, false);

        pollMonitor(adapter); // peek Monitor infos !
        scale == 0 ? _scaleWin = getMaxScale()-1 : _scaleWin = scale;
        setViewAtCenter(_scaleWin);

        al_set_window_position(display,_currentMonitorX+_viewX,_currentMonitorY+_viewY);
        al_resize_display(_windowDisplay,_screenW*_scaleWin,_screenH*_scaleWin);
    }
}
// get Monitor state POS & SIZE !
void Window::pollMonitor(int adapter)
{
    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(adapter, &info);

    _currentMonitorX = info.x1;
    _currentMonitorY = info.y1;
    _currentMonitorW = info.x2 - info.x1;
    _currentMonitorH = info.y2 - info.y1;
}

// Calculate max scaling factor !
int Window::getMaxScale()
{
    int ratioX = _currentMonitorW / _screenW;
    int ratioY = _currentMonitorH / _screenH;
    return std::min(ratioX, ratioY);
}

// Place the View in the center of Monitor by scale !
void Window::setViewAtCenter(int scale)
{
    _viewW = _screenW * scale;
    _viewH = _screenH * scale;
    _viewX = (_currentMonitorW - _viewW) / 2;
    _viewY = (_currentMonitorH - _viewH) / 2;
}


//------------------------------------------------------------------------------ Accessors
ALLEGRO_DISPLAY * Window::display()
{
    return _windowDisplay;
}
ALLEGRO_BITMAP * Window::buffer()
{
    return _windowBuffer;
}

// Find the current monitor where the window is
int Window::currentMonitor(ALLEGRO_DISPLAY * display)
{
    for (int i(0); i<al_get_num_video_adapters(); i++)
    {
        ALLEGRO_MONITOR_INFO info;
        al_get_monitor_info(i, &info);

        int xWin(0);
        int yWin(0);

        al_get_window_position(display, &xWin, &yWin);

        if (xWin>=info.x1 && yWin>=info.y1 && xWin<info.x2 && yWin<info.y2)
        {
            _currentMonitor = i;
            return i;
        }
    }
    return 0; // Main/Default monitor
}

void Window::getMouse(ALLEGRO_MOUSE_STATE * mouseState, float &xMouse, float &yMouse)
{
    if (_isFullScreen)
    {
        xMouse = (mouseState->x-_viewX)/ _scaleFull;
        yMouse = (mouseState->y-_viewY)/ _scaleFull;
    }
    else
    {
        xMouse = mouseState->x/_scaleWin;
        yMouse = mouseState->y/_scaleWin;

    }
    if (xMouse < 0)          xMouse = 0;
    if (xMouse > _screenW-1) xMouse = _screenW-1;
    if (yMouse < 0)          yMouse = 0;
    if (yMouse > _screenH-1) yMouse = _screenH-1;
}

bool Window::isFullScreen() const
{
    return _isFullScreen;
}

int Window::screenW() const
{
    return _screenW;
}
int Window::screenH() const
{
    return _screenH;
}

int Window::centerX() const
{
    return _screenW/2;
}

int Window::centerY() const
{
    return _screenH/2;
}


int Window::scaleWin() const
{
    return _scaleWin;
}
int Window::x() const
{
    return _x;
}

int Window::y() const
{
    return _y;
}

int Window::scaleFull() const
{
    return _scaleFull;
}

int Window::viewX() const
{
    return _viewX;
}

int Window::viewY() const
{
    return _viewY;
}

int Window::viewW() const
{
    return _viewW;
}

int Window::viewH() const
{
    return _viewH;
}

int Window::currentMonitor() const
{
    return _currentMonitor;
}

int Window::currentMonitorX() const
{
    return _currentMonitorX;
}

int Window::currentMonitorY() const
{
    return _currentMonitorY;
}

int Window::currentMonitorW() const
{
    return _currentMonitorW;
}

int Window::currentMonitorH() const
{
    return _currentMonitorH;
}

