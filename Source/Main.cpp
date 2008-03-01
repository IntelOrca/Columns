/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "GameApp.h"
#include "PopPakUnpack.h"

using namespace Sexy;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	gHInstance = hInstance;

	//UnpackFiles("C:\\Program Files\\PopCap Games\\Peggle Nights Deluxe\\main.pak", "C:\\Program Files\\SexyApp Framework\\tools\\Peggle Nights");

	GameApp* anApp = new GameApp();
	anApp->Init();
	anApp->Start();

	delete anApp;

	return 0;
}