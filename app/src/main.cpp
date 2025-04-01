#include "Engine.h"
#include <exception>

#ifdef _CONSOLE
int main() {
#else
#ifdef _WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLin, int nCmdShow){
#else
int main(int argc, char** argv){
#endif //_WIN32
#endif // Console
	Engine engine;
	engine.Run();
}