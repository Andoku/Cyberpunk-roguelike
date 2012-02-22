#ifndef _CAPP_H_
	#define _CAPP_H_

#include "SDL/SDL.h"
#include "CSurface.h"
#include "CEvent.h"
#include "CInterface.h"
#include "CMap.h"
#include "Define.h"
#include <iostream>

class CApp : public CEvent {
private:
	bool Running;
	SDL_Surface *Surf_Display;
	SDL_Surface *Surf_Background;
	
public:
	CApp();
	int OnExecute();
	bool OnInit();
	void OnEvent(SDL_Event *Event);
	void OnExit();
	void OnLoop();
	void OnRender();
	void OnCleanup();
};

CApp theApp;

CApp::CApp() {
	Surf_Display = NULL;
	Surf_Background = NULL;
	Running = true;
}

int CApp::OnExecute() {
	if(OnInit() == false) {
		return -1;
	}
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
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;
	if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) return false;
	if((Surf_Background = CSurface::OnLoad("./media/hud.png")) == NULL) return false;
	if(!CMap::MapControl.OnLoad("./maps/1.map")) return false;
	if(!CInterface::InterfaceControl.OnLoad("./fonts/terminus.ttf")) return false;
	
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL*3); //Не очень круто получается

	return true;
}

void CApp::OnEvent(SDL_Event *Event) {
	CEvent::OnEvent(Event);
}

void CApp::OnExit() {
	Running = false;
}

void CApp::OnLoop() {
	
}

void CApp::OnRender() {
	CSurface::OnDraw(Surf_Display, Surf_Background, 0, 0);
	CMap::MapControl.OnRender(Surf_Display, MAP_X, MAP_Y);
	CInterface::InterfaceControl.OnRender(Surf_Display);
	SDL_Flip(Surf_Display);
}

void CApp::OnCleanup() {
	CMap::MapControl.OnCleanup();
	CInterface::InterfaceControl.OnCleanup();
	SDL_FreeSurface(Surf_Display);
	SDL_FreeSurface(Surf_Background);
	SDL_Quit();
}

#endif
