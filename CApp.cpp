#include "SDL/SDL.h"
#include "CTile.h"
#include "CSurface.h"
#include "CEvent.h"
#include <vector>
#include <SDL/SDL_ttf.h>
#include "CInterface.h"
#include "CUnit.h"
#include "CMap.h"
#include "Define.h"
#include "CApp.h"

CApp::CApp() {
    Surf_Display = NULL;
    Surf_Background = NULL;
    Running = true;
}

int CApp::OnExecute() {
    if(OnInit() == false) return -1;
    SDL_Event Event;
    while(Running) {
        SDL_WaitEvent(&Event);
        OnEvent(&Event);
        OnLoop();
        OnRender();
    }
    OnCleanup();
    return 0;
}

bool CApp::OnInit() {
    CUnit unit;
    unit.posx = 1;
    unit.posy = 1;
    unit.facing = RIGHT;
    CUnit::UnitList.push_back(unit);
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;
    if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) return false;
    if((Surf_Background = CSurface::OnLoad("./media/hud.png")) == NULL) return false;
    if(!CMap::MapControl.OnLoad("./maps/1.map")) return false;
    if(!CInterface::InterfaceControl.OnLoad("./fonts/terminus.ttf")) return false;
    for(int i = 0; i < CUnit::UnitList.size(); i++)
        if(!CUnit::UnitList[i].OnLoad("tilesets/2.png")) return false;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL*3);
    return true;
}

void CApp::OnEvent(SDL_Event *Event) {
    CEvent::OnEvent(Event);
}

void CEvent::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch (sym)
    {
    case SDLK_UP:
        CUnit::UnitList[0].Move(UP);
        break;
    case SDLK_DOWN:
        CUnit::UnitList[0].Move(DOWN);
        break;
    case SDLK_LEFT:
        CUnit::UnitList[0].Move(LEFT);
        break;
    case SDLK_RIGHT:
        CUnit::UnitList[0].Move(RIGHT);
        break;
    case SDLK_k:
        CUnit::UnitList[0].Move(UP);
        break;
    case SDLK_j:
        CUnit::UnitList[0].Move(DOWN);
        break;
    case SDLK_h:
        CUnit::UnitList[0].Move(LEFT);
        break;
    case SDLK_l:
        CUnit::UnitList[0].Move(RIGHT);
        break;
    case SDLK_ESCAPE:
        OnExit();
        break;
    default:
        break;
    }
}

void CApp::OnExit() {
    Running = false;
}

void CApp::OnLoop() {
    
}

void CApp::OnRender() {
    CSurface::OnDraw(Surf_Display, Surf_Background, 0, 0);
    CMap::MapControl.OnRender(Surf_Display, CUnit::UnitList[0].posx, CUnit::UnitList[0].posy);
    for(int i = 0; i < CUnit::UnitList.size(); i++)
        CUnit::UnitList[i].OnRender(Surf_Display);
    CUnit::UnitList[0].DrawFog(Surf_Display);
    CInterface::InterfaceControl.OnRender(Surf_Display);
    SDL_Flip(Surf_Display);
}

void CApp::OnCleanup() {
    for(int i = 0; i < CUnit::UnitList.size(); i++)
        CUnit::UnitList[i].OnCleanup();
    CMap::MapControl.OnCleanup();
    CInterface::InterfaceControl.OnCleanup();
    CUnit::UnitList.clear();
    SDL_FreeSurface(Surf_Display);
    SDL_FreeSurface(Surf_Background);
    SDL_Quit();
}

CApp theApp;

int main(int argv, char **argc) {
    return theApp.OnExecute();
}