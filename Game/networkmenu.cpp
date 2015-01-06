
#include "networkmenu.h"


NetworkMenu::NetworkMenu(Fighter::FighterController Controller)
{
	localController = Controller;
}

void NetworkMenu::Begin()
{
}

void NetworkMenu::Pause()
{
}

void NetworkMenu::Resume()
{
}

void NetworkMenu::Finish()
{
}

void NetworkMenu::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		delete FRAMEWORK->ProgramStages->Pop();
		return;
	}
}

void NetworkMenu::Update()
{
	FRAMEWORK->ProgramStages->Previous()->Update();
}

void NetworkMenu::Render()
{
	FRAMEWORK->ProgramStages->Previous()->Render();
	al_draw_filled_rounded_rectangle( 10, 10, 300, 300, 10, 10, al_map_rgb(128, 192, 128) );
}

bool NetworkMenu::IsTransition()
{
	return false;
}

