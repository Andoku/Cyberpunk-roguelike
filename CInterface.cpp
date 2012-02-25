#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <vector>
#include "CSurface.h"
#include "CInterface.h"

CInterface CInterface::InterfaceControl;

CInterface::CInterface() {
}

bool CInterface::OnLoad(const char *font_file) {
    if (TTF_Init() < 0) return false;
    Font = TTF_OpenFont(font_file, 14);
    
    FILE *FileHandle = fopen("interface/1.interface", "r");
    if(FileHandle == NULL) return false;
    
    int n1;
    fscanf(FileHandle, "%d\n", &n1);
    HUD.resize(n1);
    for(int i = 0; i < n1; i++) {
        int n2;
        fscanf(FileHandle, "%d\n", &n2);
        for(int j = 0; j < n2; j++) {
            HUD[i].resize(n2);
            int r, g, b;
            fscanf(FileHandle, "%s %d %d %d:%d:%d\n", HUD[i][j].text, &HUD[i][j].X, &HUD[i][j].Y, &r, &g, &b);
            for(int k = 0; HUD[i][j].text[k] != '\0'; k++) // так себе
                if (HUD[i][j].text[k] == '_') HUD[i][j].text[k] = ' '; 
            HUD[i][j].clr.r = static_cast<Uint8>(r);
            HUD[i][j].clr.g = static_cast<Uint8>(g);
            HUD[i][j].clr.b = static_cast<Uint8>(b);
            if(HUD[i][j].text[0] != '0') {
                HUD[i][j].surf = TTF_RenderText_Solid(Font, HUD[i][j].text, HUD[i][j].clr);
            } else {
                HUD[i][j].surf = NULL;
            }
        }
    }
    fclose(FileHandle);
    return true;
}

void CInterface::OnLoop() {
    
}

void CInterface::OnRender(SDL_Surface *Surf_Display) {
    for(int i = 0; i < HUD.size(); i++) {
        for(int j = 0; j < HUD[i].size(); j++) {
            CSurface::OnDraw(Surf_Display, HUD[i][j].surf, HUD[i][j].X, HUD[i][j].Y);
        }
    }
    
}

void CInterface::OnCleanup() {
    for(int i = 0; i < HUD.size(); i++) {
        for(int j = 0; j < HUD[i].size(); j++) {
            SDL_FreeSurface(HUD[i][j].surf);
        }
    }
    TTF_CloseFont(Font);
    TTF_Quit();
}