#include "pch.h"
#include "App.h"

int main()
{
	// Exception reports (kazilib): exceptions\exception_<pid>.txt / .dmp beside the
	// exe; with catchexceptions (tests) the process ends instead of a JIT dialog
	lib::exc::Install(lib::exc::CatchArg());

	App().Tick();
	return 0;
}
