#ifndef MUGENENGINEZERO_H_INCLUDED
#define MUGENENGINEZERO_H_INCLUDED

//------------------------------------------------------------------------------  Major dependances
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_windows.h>

#include <cstdio>
#include <memory>
#include <vector>

#include <iostream>
#include <functional>
#include <fstream>
#include <cstdlib>
#include <ctime>

//------------------------------------------------------------------------------  Minor dependances
#define WINSYSTEM // for Windows specific platform

template <class E, class M> E log(E error, M msg);

//------------------------------------------------------------------------------ Class Window
class Window
{
    public:
    // Setup
        Window();
        virtual ~Window();
        int init(const char *name, int screenW, int screenH, int scaleWin, int scaleFull, bool fullScreen);
        int done();

    // Render
        void beginRender(ALLEGRO_COLOR color);
        void endRender(ALLEGRO_COLOR color);

    // Manager
        void toggleFullScreen(int scale); // Toggle windowed to FullScreen : 0 = Max Zoom , -1 = Default Zoom
        void switchMonitor(int scale); // Switch monitor : 0 = Max Zoom, -1 Default Zoom
        void setMonitor(int monitor, int scale); // Select the Monitor : O = Max Zoom, -1 Default Zoom
        void setScale(int scale); // set Scale : 0 = Max Zoom, -1 Default Zoom
        void setWindow(ALLEGRO_DISPLAY *display, int adapter,bool isFullScreen, int scale);

        void setViewAtCenter(int scale); // Place the View in the center of Monitor by scale !
        int getMaxScale(); // Calculate max scaling factor !
        void pollMonitor(int adapter);

    // Accessor
        bool isFullScreen() const;
        ALLEGRO_DISPLAY *display();
        ALLEGRO_BITMAP *buffer();


        int screenW() const;
        int screenH() const;
        int centerX() const;
        int centerY() const;

        int scaleWin() const;
        int scaleFull() const;
        int viewX() const;
        int viewY() const;
        int viewW() const;
        int viewH() const;

        int currentMonitor(ALLEGRO_DISPLAY *display); // Find the current monitor where the window is !
        int currentMonitor() const; // return the current Monitor selected !
        int currentMonitorX() const;
        int currentMonitorY() const;
        int currentMonitorW() const;
        int currentMonitorH() const;

        // Mouse
        void getMouse(ALLEGRO_MOUSE_STATE *mouseState, float &xMouse, float &yMouse);


    // Debug
        int x() const;
        int y() const;

    protected:
        const char* _name;

        // Screen of Game
        int _screenW;
        int _screenH;

        // Windowed
        int _scaleWin;
        int _x;
        int _y;

        // FullScreen
        int _scaleFull;
        int _viewW;
        int _viewH;
        int _viewX;
        int _viewY;

        // Monitor
        int _currentMonitor;
        int _currentMonitorX;
        int _currentMonitorY;
        int _currentMonitorW;
        int _currentMonitorH;

        // View state
        bool _isFullScreen;
        bool _isMaxScale;

    private:
        ALLEGRO_DISPLAY *_windowDisplay = NULL;
        ALLEGRO_BITMAP *_windowBuffer = NULL;
};

//------------------------------------------------------------------------------ Class MugenEngine
class MugenEngineZero
{
    public:
//------------------------------------------------------------------------------ Method declarations
        MugenEngineZero();
        virtual ~MugenEngineZero();

        virtual int init() = 0; // Init all here !
        virtual int done() = 0; // Done all here !

        int run(); // Main loop entry !

        virtual void update() = 0; // Update in main loop !
        virtual void render() = 0; // Render in main loop !

        int initTemplate();
        int doneTemplate();
        int initWindow(const char* name, int screenW, int screenH, bool fullscreen, int scaleWin, int scaleFull, int framerate, bool vsync, bool mouse);

        void controlFramerate(bool control);

        // Inputs
        void pollKeyboard();
        void pollMouse();

        bool keyDown(int key);


    protected:
//------------------------------------------------------------------------------ Globals Var
        bool _quit = false;
        bool _controlFramerate = true;

        float _xMouse = 0;
        float _yMouse = 0;

        ALLEGRO_COLOR _black = {0,0,0};

        ALLEGRO_KEYBOARD_STATE _keyState;
        ALLEGRO_MOUSE_STATE _mouseState;

        ALLEGRO_EVENT _event;
        ALLEGRO_EVENT_QUEUE * _eventQueue = NULL;
        ALLEGRO_TIMER * _timer = NULL;

        std::shared_ptr<Window> _window;

    private:
        int setFramerate(int fps);

};

#include "MugenEngineZero.inl"


#endif // MUGENENGINEZERO_H_INCLUDED
