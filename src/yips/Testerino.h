#pragma once
// yips' Testerino bridge (kazilib testerino/tcore.h): on with the launch flag
// testerino=<slot> (1..9), only in a source checkout (src\yips.sln beside deploy\).
// Endpoint <checkout>\testerino\client_<slot>\.
namespace testerino
{
	void Start();
	void Pump();							// once per frame, before the scene's tick
	void AfterDraw(sf::RenderWindow & wnd);	// drawn, not presented yet: screenshots
	void Stop();
	bool Active();
}
