#pragma comment(lib, "420C35JOTP3Lib.lib")

#ifdef _DEBUG
  #undef _DEBUG
  #include "Window.hpp"
  #define _DEBUG
#else
  #include "Window.hpp"
#endif

#include "TP3.hpp"

int main(int argc, char* argv[]) {
	return Window::open(onInit, onMenuClick, onRefresh, onWindowClick, onQuit);
}