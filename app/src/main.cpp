#include "Engine.h"
#include <exception>

//#ifdef _CONSOLE
//int main() {
//#else
#ifdef _WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLin, int nCmdShow){
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		ShowWindow(hwnd, SW_HIDE); // Hide the console
}

#else
int main(){
#endif
//int main(int argc, char** argv){
//#endif //_WIN32
//#endif // Console
	Engine engine;
	engine.Run();

	return 0;
}