//==============================================================================
// ALLEGRO 5 template : By Silver TES 2016
//==============================================================================
#include "Game.h"
//------------------------------------------------------------------------------ Init
int Game::init()
{
    initWindow(
               "- My Game -", // Title
               640,           // Width
               360,           // Height
               false,         // FullScreen
               2,             // Scale in Window mode
               0,             // scale in Fullscreen  mode
               60,            // Framerate
               false,         // Vsync
               false          // Show Mouse
               );

    // ... INIT HERE ...

    return log(0,"- init OK !\n");
}
//------------------------------------------------------------------------------ Done
int Game::done()
{
    // ... DONE HERE ...

    return log(0,"- done OK !\n");
}
//------------------------------------------------------------------------------ Update
void Game::update()
{
    // ... UPDATE HERE ...

    if (keyDown(ALLEGRO_KEY_ESCAPE))
    {
        _quit = true;
    }

    if (!keyDown(ALLEGRO_KEY_SPACE)) keyFull = false;
    if (!keyDown(ALLEGRO_KEY_TAB)) keySwitch = false;

    if (keyDown(ALLEGRO_KEY_SPACE) && !keyFull)
    {
        keyFull = true;
        _window->toggleFullScreen(0); // Toggle FullScreen : 0 = max size
    }
    if (keyDown(ALLEGRO_KEY_TAB) && !keySwitch)
    {
        keySwitch = true;
        _window->switchMonitor(-1); // Switch Monitor : -1 = default size
    }

    x += 1;
    if (x>_window->screenW()) x = 0;
}
//------------------------------------------------------------------------------ Render
void Game::render()
{
    _window->beginRender(al_map_rgb(20,60,80));
    // ... RENDER HERE ...


    float cx = _window->centerX();
    float cy = _window->centerY();

    // Draw a circle !
    al_draw_circle(cx,cy,
                   32,
                   al_map_rgb(255,0,255),
                   2);

    // Draw a filled rectangle !
    al_draw_rounded_rectangle(cx-40,cy-40,
                              cx+40,cy+40,
                              8,8,
                             al_map_rgba(100,150,50,20), 2);
    // Draw a rectangle !
    al_draw_rectangle(0.5,0.5,100.5,100.5,
                      al_map_rgba(0,250,250,50),
                      0);

    // Draw a filled circle !
    al_draw_filled_circle(x,_window->screenH()-40,
                          20,
                          al_map_rgb(255,200,50));

    // Draw a circle alpha at 50/255!
    al_draw_circle(_xMouse, _yMouse,
                   8,
                   al_map_rgba(55,255,55,25),
                   4);

    // Draw a line alpha at 150/255!
    al_draw_line(_window->centerX(),_window->centerY(),
                 _xMouse,_yMouse,
                 al_map_rgba(50,250,200,150),
                 0);


    _window->endRender(_black);
}
