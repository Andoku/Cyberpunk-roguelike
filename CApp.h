#ifndef _CAPP_H_
    #define _CAPP_H_

class CApp : public CEvent {
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

#endif
