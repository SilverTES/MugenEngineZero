//==============================================================================
// ALLEGRO 5 template : By Silver TES 2016
//==============================================================================
#ifndef GAME_H
#define GAME_H

#define HIDE_LOG // Hide log
#include "MugenEngineZero.h"

//==============================================================================
// Class Game
//==============================================================================
class Game : public MugenEngineZero
{
//------------------------------------------------------------------------------ Init
        int init() override;
//------------------------------------------------------------------------------ Done
        int done() override;
//------------------------------------------------------------------------------ Update
        void update() override;
//------------------------------------------------------------------------------ Render
        void render() override;
//------------------------------------------------------------------------------ Variables
        bool keyFull = false;   // Boolean for toggle fullscreen
        bool keySwitch = false; // Boolean for switch monitor
        float x = 0;

};


#endif // GAME_H
