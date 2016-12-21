#include "MugenEngineZero.h"

MugenEngineZero::MugenEngineZero()
{
    log(0,"- MugenEngineZero Created \n");
}

MugenEngineZero::~MugenEngineZero()
{
    log(0,"- MugenEngineZero Deleted \n");
}

//------------------------------------------------------------------------------ Main loop

int MugenEngineZero::run()
{
    if (initTemplate())
        _quit = true;
    if (init())
        _quit = true;

    while(!_quit)
    {
        if (_controlFramerate)
            al_wait_for_event(_eventQueue, &_event);

        pollKeyboard();
        pollMouse();
        update();
        render();
    }
    if (done())
        return log(1,"- done Program error !\n");

    if (doneTemplate())
        return log(1,"- done Template error !\n");

    return log(0,"- Main loop terminated OK !\n");
}

//------------------------------------------------------------------------------ Setup Methods

int MugenEngineZero::initWindow(const char* name,
                                int screenW ,
                                int screenH,
                                bool fullscreen,
                                int scaleWin,
                                int scaleFull,
                                int framerate,
                                bool vsync,
                                bool mouse)
{

    if (vsync)
        al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);

    _window = std::make_shared<Window>();
    _window->init(name, screenW, screenH, scaleWin, scaleFull, fullscreen);

    if (setFramerate(framerate))
        return log(1,"- Unable to set Framerate !\n");

    if (mouse)
        al_show_mouse_cursor(_window->display());
    else
        al_hide_mouse_cursor(_window->display());

    return log(0,"- Init Window OK !\n");
}

int MugenEngineZero::initTemplate()
{
    // --- Init Allegro 5
    if (!al_init())
        return log(1,"- Unable to init Allegro 5 !\n");

    // --- Install All devices
    if (!al_install_audio())
        return log(1,"- Unable to install Audio !\n");
    if (!al_install_keyboard())
        return log(1,"- Unable to install Keyboard !\n");
    if (!al_install_mouse())
        return log(1,"- Unable to install Mouse ! \n");

    // --- Init All addons
    if (!al_init_primitives_addon())
        return log(1,"- Unable to init Primitives Addon !\n");
    if (!al_init_image_addon())
        return log(1,"- Unable to init Image Addon !\n");
    if (!al_init_font_addon())
        return log(1,"- Unable to init Font Addon !\n");
    if (!al_init_ttf_addon())
        return log(1,"- Unable to init TTF Addon !\n");
    if (!al_init_acodec_addon())
        return log(1,"- Unable to init ACODEC Addon !\n");

    if (!al_reserve_samples(32))
        return log(1,"- Unable to reserve samples");

    al_set_new_display_flags(ALLEGRO_OPENGL);

    return log(0,"- init Template OK !\n");
}

int MugenEngineZero::doneTemplate()
{
    _window->done();
    al_destroy_timer(_timer);
    al_destroy_event_queue(_eventQueue);

    return log(0,"- done Template OK !\n");
}

int MugenEngineZero::setFramerate(int fps)
{
    _timer = al_create_timer(1.0/fps);
    if (!_timer)
        return log(1,"- Unable to create timer !\n");

    _eventQueue = al_create_event_queue();
    if (!_eventQueue)
        return log(1,"- Unable to create eventQueue !\n");

    //al_register_event_source(_eventQueue, al_get_display_event_source(_window->display()));
    al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));
    al_start_timer(_timer);

    return log(0,"- setFramerate OK !\n");
}
void MugenEngineZero::controlFramerate(bool control)
{
    _controlFramerate = control;
}

//------------------------------------------------------------------------------ Accessor

void MugenEngineZero::pollKeyboard()
{
    al_get_keyboard_state(&_keyState);
}

void MugenEngineZero::pollMouse()
{
    al_get_mouse_state(&_mouseState);
    _window->getMouse(&_mouseState, _xMouse, _yMouse);
}

bool MugenEngineZero::keyDown(int key)
{
    return al_key_down(&_keyState, key);
}

