#ifndef _CINTERFACE_H_
    #define _CINTERFACE_H_
 
typedef struct {
    int X;
    int Y;
    char text[255];
    SDL_Color clr;
    SDL_Surface *surf;
} Text;

class CInterface {
    TTF_Font *Font;
    std::vector< std::vector<Text> > HUD;   
public: 
    static CInterface InterfaceControl;
    CInterface();
    bool OnLoad(const char *);
    void OnLoop();
    void OnRender(SDL_Surface *);
    void OnCleanup();
};

#endif