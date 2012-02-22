#ifndef _CINTERFACE_H_
	#define _CINTERFACE_H_
 
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>


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
	bool OnLoad(char *, int, char *);
	void OnLoop();
	void OnRender(SDL_Surface *);
	void OnCleanup();
};

CInterface CInterface::InterfaceControl;

CInterface::CInterface() {
}

bool CInterface::OnLoad(char *font_file, int ptsize = 14, char *HUD_file = "interface/1.interface") {
	if (TTF_Init() < 0) {
		return false;
	}
	Font = TTF_OpenFont(font_file, ptsize);
	
	FILE *FileHandle = fopen(HUD_file, "r");
	if(FileHandle == NULL) {
		return false;
	}
	
	int n1;
    fscanf(FileHandle, "%d\n", &n1);
	HUD.resize(n1);
	for(int i = 0; i < n1; i++) {
		int n2;
		fscanf(FileHandle, "%d\n", &n2);
		for(int j = 0; j < n2; j++) {
			HUD[i].resize(n2);
			fscanf(FileHandle, "%s %d %d %d:%d:%d\n", HUD[i][j].text, &HUD[i][j].X, &HUD[i][j].Y, &HUD[i][j].clr.r, &HUD[i][j].clr.g, &HUD[i][j].clr.b);
			if(HUD[i][j].text[0] != '0') {
				HUD[i][j].surf = TTF_RenderText_Solid(Font, HUD[i][j].text, HUD[i][j].clr);
			} else {
				HUD[i][j].surf = NULL;
			}
		}
	}
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


#endif
