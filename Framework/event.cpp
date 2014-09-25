
#include "event.h"

Event::Event()
{
  Type = EVENT_UNDEFINED;
	Handled = false;
}

Event::~Event()
{
	if( Type == EVENT_FORM_INTERACTION )
	{
		if( Data.Forms.AdditionalData != nullptr )
		{
			delete Data.Forms.AdditionalData;
		}
	}

#ifdef NETWORK_SUPPORT
	if( Type == EVENT_NETWORK_PACKET_RECEIVED )
	{
		enet_packet_destroy( Data.Network.Traffic.packet );
	}
#endif

#ifdef DOWNLOAD_SUPPORT
	if( Type == EVENT_DOWNLOAD_COMPLETE || Type == EVENT_DOWNLOAD_PROGRESS )
	{
		if( Data.Download.URL != nullptr )
		{
			delete Data.Download.URL;
		}
		if( Data.Download.Contents != nullptr )
		{
			delete Data.Download.Contents;
		}
	}
#endif

}
