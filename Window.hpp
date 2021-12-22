#include <cmath>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

using namespace std;

enum Icon { FOLDER, NOTE, HUFFMAN };
enum Menu { NEW_FOLDER = 1, NEW_NOTE = 2, RENAME = 4, DELETE = 8, ENCODE = 16, DECODE = 32, SELECT_ALL = 64 };

class Window {
public:
	Window();
	~Window();

	static void drawIcon(const Icon& icon, const int& x, const int& y, const bool& selected = false);
	static void drawString(const string& text, const int& x, const int& y);
	static int getHeight();
	static int getIconHeight();
	static int getIconWidth();
	static int getStringWidth(const string& text);
	static int getWidth();
	static bool open(void (*OnInit)(), void(*OnMenuClick)(const unsigned int&), void(*OnRefresh)(), void(*OnWindowClick)(const int&, const int&, const bool&, const bool&), void (*OnQuit)());
	static void setTitle(const string& title);
	static void showMenu(const int& x, const int& y, const unsigned int& enables = 0);
	static string showTextField(const string& text = "");

    //J'ai créer cette méthode de l'objet Window afin d'éviter de créer une variable globale contenant un titre, sachant que GetWindowTitle le faisait déjà pour moi.
    static string getTitle() { return SDL_GetWindowTitle(instance->sdlWindow); }

private:
	static TTF_Font* font, * textFont;
	static unsigned int enableItems;
	static Window* instance;
	static bool isMenu;
	static SDL_Texture* icons, * menu, * menuItems[7], * textField, * textBox, * textTexture;
	static SDL_Event sdlEvent;
	static SDL_Rect dstRect, itemRect, menuRect, srcRect, textRect, textFieldRect, textBoxRect;
	static void(*OnMenuClick)(const unsigned int&);
	static void(*OnRefresh)();
	static void(*OnWindowClick)(const int&, const int&, const bool&, const bool&);

	SDL_Renderer* sdlRenderer;
	SDL_Window* sdlWindow;
};
