
#pragma once

#ifdef DOWNLOAD_SUPPORT

#include "../includes.h"
#include "../event.h"

#define DOWNLOADS DownloadManager::CurrentDownloadManager

class DownloadInformation
{
	public:
		std::string URL;
		unsigned long TotalDLSize;
		unsigned long CurrentDLSize;

		DownloadInformation( std::string SourceURL )
		{
			URL = SourceURL;
			TotalDLSize = 0;
			CurrentDLSize = 0;
		};
};

typedef struct DOWNLOAD_THREAD_DATA
{
	std::string* URL;
	DownloadInformation* Information;
} DOWNLOAD_THREAD_DATA;

class DownloadManager
{
	private:
		std::vector<DownloadInformation*> urlDownloading;
		std::list<std::string> urlList;
		int urlDownloads;

	public:
		static DownloadManager* CurrentDownloadManager;
		static bool AbortDownloads;

		DownloadManager( int AllowedConcurrents );
		~DownloadManager();
		void RequestURL( std::string URL );
		void EventOccured( Event* e );
		void Update();
};

#endif