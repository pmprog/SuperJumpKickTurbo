
#include "menu.h"
#include "../Framework/framework.h"
#include "fight.h"

void Menu::Begin()
{
	ScreenScale = Framework::System->GetDisplayHeight() / 240.0;
	float Scale = 3 * ScreenScale;

	menuOption = 0;
	turboValue = 1;
	menuTimer = 0;

	titleChar = new Akuma();
	titleChar->Initialise( Scale );
	titleChar->Fighter_SetPosition( -(Framework::System->GetDisplayWidth() / 2), -(Framework::System->GetDisplayHeight() * 0.05f) );

	titleSuper = spLoadSurface( "Resource/super.png" );
	titleJumpFighter = spLoadSurface( "Resource/jumpfighter.png" );

	titleFont = spFontLoad( "Resource/visitor1.ttf", 24 * ScreenScale );
	spFontAdd( titleFont, SP_FONT_GROUP_ASCII, spGetFastRGB( 128, 128, 128 ) );
	spFontAddBorder( titleFont, spGetFastRGB( 0, 0, 0 ) );
	titleFontSel = spFontLoad( "Resource/visitor1.ttf", 24 * ScreenScale );
	spFontAdd( titleFontSel, SP_FONT_GROUP_ASCII, spGetFastRGB( 255, 255, 128 ) );
	spFontAddBorder( titleFontSel, spGetFastRGB( 255, 0, 0 ) );

}

void Menu::Pause()
{
}

void Menu::Resume()
{
}

void Menu::Finish()
{
	spFontDelete( titleFont );
	spFontDelete( titleFontSel );
	spDeleteSurface( titleJumpFighter );
	spDeleteSurface( titleSuper );
	delete titleChar;
}

void Menu::EventOccurred(Event *e)
{
	spInput* inputData;
	inputData = spGetInput();

	if( e->Type == EVENT_KEY_DOWN )
	{
		switch( e->Data.Keyboard.keysym.sym )
		{
			case SDLK_ESCAPE:
				delete Framework::System->ProgramStages->Pop();
				break;
			case SDLK_DOWN:
				menuOption = (menuOption + 1) % 4;
				break;
			case SDLK_UP:
				menuOption = (menuOption + 7) % 4;
				break;
			case SDLK_RETURN:
			case SDLK_SPACE:
				Framework::System->ProgramStages->Push( (Stage*)new Fight() );
				break;
			default:
				if( inputData->button[SP_BUTTON_DOWN] != 0 )
				{
					menuOption = (menuOption + 1) % 4;
				}
				if( inputData->button[SP_BUTTON_UP] != 0 )
				{
					menuOption = (menuOption + 7) % 4;
				}
				break;
		}

	}

/*
	if( e->Type == EVENT_JOYSTICK_AXIS || e->Type == EVENT_JOYSTICK_HAT || e->Type == EVENT_JOYSTICK_BALL )
	{
		inputData = spGetInput();
		if( inputData->axis[SP_AXIS_UPDOWN] > 0 )
		{
			menuOption = (menuOption + 1) % 4;
		}
		if( inputData->axis[SP_AXIS_UPDOWN] < 0 )
		{
			menuOption = (menuOption + 7) % 4;
		}
	}
	if( e->Type == EVENT_JOYSTICK_BUTTON_DOWN )
	{
	}
*/
}

void Menu::Update()
{
	menuTimer++;

	titleChar->Fighter_Update();
	if( titleChar->Fighter_GetPosition()->X < Framework::System->GetDisplayWidth() / 6.0 )
	{
		titleChar->Fighter_GetPosition()->X += (Framework::System->GetDisplayWidth() / (FRAMES_PER_SECOND * 6.0));
	}
}

void Menu::Render()
{
	spClearTarget( spGetFastRGB( 64, 128, 96 ) );

	spSetHorizontalOrigin( SP_CENTER );
	spSetVerticalOrigin( SP_TOP );
	spRotozoomSurface( Framework::System->GetDisplayWidth() / 2, 4, -3, titleSuper, SP_ONE * ScreenScale, SP_ONE * ScreenScale, 0 );
	if( menuTimer > FRAMES_PER_SECOND * 2.0 )
	{
		spRotozoomSurface( Framework::System->GetDisplayWidth() / 2, 4, -2, titleJumpFighter, SP_ONE * ScreenScale, SP_ONE * ScreenScale, 0 );
	}

	
	int Ypos = Framework::System->GetDisplayHeight() - ( titleFontSel->size * 5 );
	spFontDrawRight( Framework::System->GetDisplayWidth() - 4, Ypos, -1, "Single Player", ( menuOption == 0 ? titleFontSel : titleFont ) );
	Ypos += titleFontSel->size;
	spFontDrawRight( Framework::System->GetDisplayWidth() - 4, Ypos, -1, "Versus", ( menuOption == 1 ? titleFontSel : titleFont ) );
	Ypos += titleFontSel->size;
	spFontDrawRight( Framework::System->GetDisplayWidth() - 4, Ypos, -1, "Turbo", ( menuOption == 2 ? titleFontSel : titleFont ) );
	Ypos += titleFontSel->size;
	spFontDrawRight( Framework::System->GetDisplayWidth() - 4, Ypos, -1, "Quit", ( menuOption == 3 ? titleFontSel : titleFont ) );

	spSetVerticalOrigin( SP_BOTTOM );
	titleChar->Fighter_Render(0, 0);
}
