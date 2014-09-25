
#include "download.h"
#include "../framework.h"

#ifdef DOWNLOAD_SUPPORT

void* get_http_page_threaded( ALLEGRO_THREAD* thread, void* ThreadData );
void http_event_destructor(ALLEGRO_USER_EVENT* e);
size_t write_data(void *ptr, size_t size, size_t nmemb, void *user);
int download_progress(void *user, double dltotal, double dlnow, double ultotal, double ulnow);


DownloadManager* DownloadManager::CurrentDownloadManager = nullptr;
bool DownloadManager::AbortDownloads = false;

DownloadManager::DownloadManager( int AllowedConcurrents )
{
#ifdef WRITE_LOG
	printf( "DownloadManager: Init\n" );
#endif
	urlDownloads = AllowedConcurrents;
	curl_global_init( CURL_GLOBAL_ALL );
}

DownloadManager::~DownloadManager()
{
#ifdef WRITE_LOG
	printf( "DownloadManager: Shutdown\n" );
#endif
	curl_global_cleanup();
}

void DownloadManager::RequestURL( std::string URL )
{
#ifdef WRITE_LOG
	printf( "DownloadManager: Request '%s' (Queue Length: %d, Download Count: %d)\n", URL.c_str(), urlList.size(), urlDownloading.size() );
#endif
	urlList.push_back( URL );
}

void DownloadManager::EventOccured( Event* e )
{
	if( e->Type == EVENT_DOWNLOAD_COMPLETE )
	{
		for( std::vector<DownloadInformation*>::iterator q = urlDownloading.begin(); q != urlDownloading.end(); q++ )
		{
			DownloadInformation* dlI = (*q);
			if( e->Data.Download.URL->compare( dlI->URL ) == 0 )
			{
				urlDownloading.erase( q );
				delete dlI;
				break;
			}
		}
#ifdef WRITE_LOG
		printf( "DownloadManager: Completed '%s' (Queue Length: %d, Download Count: %d)\n", e->Data.Download.URL.c_str(), urlList.size(), urlDownloading.size() );
#endif
	}
}

void DownloadManager::Update()
{
  ALLEGRO_THREAD* t;
  DOWNLOAD_THREAD_DATA* tD;

	while( urlDownloading.size() < urlDownloads && urlList.size() > 0 )
	{
	  DownloadInformation* dlI = new DownloadInformation( urlList.front() );
		urlDownloading.push_back( dlI );
		tD = (DOWNLOAD_THREAD_DATA*)malloc( sizeof(DOWNLOAD_THREAD_DATA) );
		tD->URL = new std::string( urlList.front() );
		tD->Information = dlI;
    t = al_create_thread( get_http_page_threaded, (void*)tD );
    al_start_thread( t );
		urlList.pop_front();
#ifdef WRITE_LOG
		printf( "DownloadManager: Downloading '%s' (Queue Length: %d, Download Count: %d)\n", urlDownloading.back()->URL.c_str(), urlList.size(), urlDownloading.size() );
#endif
	}
}

void* get_http_page_threaded( ALLEGRO_THREAD* thread, void* ThreadData )
{
  DOWNLOAD_THREAD_DATA* threadData = (DOWNLOAD_THREAD_DATA*)ThreadData;
	CURL* curl_handle;
	Memory* buffer = new Memory( 0 );
	int cTO = 60;

	curl_handle = curl_easy_init();
	curl_easy_setopt( curl_handle, CURLOPT_URL, threadData->URL->c_str() );
	curl_easy_setopt( curl_handle, CURLOPT_FOLLOWLOCATION, 1L );
  curl_easy_setopt( curl_handle, CURLOPT_NOPROGRESS, 0L );
  curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, write_data );
	curl_easy_setopt( curl_handle, CURLOPT_WRITEDATA, (void*)buffer );
  curl_easy_setopt( curl_handle, CURLOPT_PROGRESSFUNCTION, download_progress );
	curl_easy_setopt( curl_handle, CURLOPT_PROGRESSDATA, threadData->Information );

	cTO = FRAMEWORK->Settings->GetQuickIntegerValue( "Downloads.TimeOut", 60 ); // Default timeout is 60, but get settings
  curl_easy_setopt( curl_handle, CURLOPT_TIMEOUT, (long)cTO );
	curl_easy_setopt( curl_handle, CURLOPT_NOSIGNAL, 1L );

	if( FRAMEWORK->Settings->GetQuickBooleanValue( "Downloads.ProxyRequired", false ) )
	{
		curl_easy_setopt( curl_handle, CURLOPT_PROXY, FRAMEWORK->Settings->GetQuickStringValue( "Downloads.ProxyServer", "" )->c_str() );
		curl_easy_setopt( curl_handle, CURLOPT_PROXYPORT, FRAMEWORK->Settings->GetQuickInteger64Value( "Downloads.ProxyPort", 8080 ) );
	}

	if( curl_easy_perform( curl_handle ) != 0 )
	{
		buffer->Clear();
	}
	curl_easy_cleanup( curl_handle );

	if( !DownloadManager::AbortDownloads )
  {
    Event* fwEvent = new Event();
    fwEvent->Type = EVENT_DOWNLOAD_COMPLETE;
    fwEvent->Data.Download.URL = new std::string( threadData->URL->c_str() );
    fwEvent->Data.Download.Contents = buffer;
    FRAMEWORK->PushEvent( fwEvent );
  }

	if( thread != 0 )
	{
		al_destroy_thread( thread );
	}

	return 0;
}

void http_event_destructor(ALLEGRO_USER_EVENT* e)
{
	if( e->data2 != 0 )
	{
		delete (Memory*)e->data2;
	}
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *user)
{
	Memory* mem = (Memory*)user;
	mem->AppendData( ptr, size * nmemb );
	return size * nmemb;
}

int download_progress(void *user, double dltotal, double dlnow, double ultotal, double ulnow)
{
  DownloadInformation* dlI = (DownloadInformation*)user;
  dlI->TotalDLSize = (unsigned long)dltotal;
  dlI->CurrentDLSize = (unsigned long)dlnow;

  return 0;
}

#endif