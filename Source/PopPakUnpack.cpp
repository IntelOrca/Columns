#include "PakLib/pakinterface.h"
#include "SexyAppFramework/Common.h"
using namespace Sexy;

// popunpak [source.pak] [dest dir]
int UnpackFiles(char *filename, char *directory)
{
	if(!gPakInterface->AddPakFile(filename))
	{
		//printf("\nInvalid or corrupt Pak File: %s\n", argv[1]);
	}

	std::string aDestDir = "";

	if(aDestDir != "") MkDir(aDestDir);

	PakRecordMap::iterator map_itr = gPakInterface->mPakRecordMap.begin();
	for( ; map_itr != gPakInterface->mPakRecordMap.end(); map_itr++)
	{
		if(Upper(map_itr->second.mFileName) == Upper(directory))
			continue;

		std::string aFileDest = aDestDir + map_itr->second.mFileName;
		MkDir(GetFileDir(aFileDest));

		FILE* anOutputFile = fopen(aFileDest.c_str(), "wb");
		if(anOutputFile)
		{
			PFILE aPFP;
			aPFP.mRecord = &map_itr->second;
			aPFP.mPos = 0;
			aPFP.mFP = NULL;

			BYTE aReadByte;
			while(aPFP.mPos < map_itr->second.mSize && gPakInterface->FRead(&aReadByte, 1,1, &aPFP))
			{
				fwrite(&aReadByte, 1, 1, anOutputFile);
			}

			fclose(anOutputFile);
		}
	}

	return 0;
}