/*
 * FilseSys.c
 *
 *  Created on: July 25, 2024
 *  Author: Grupo 2
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <string.h>

#include "FileSys.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CANTSONGS	50

char path[CANTSONGS][255];
static int i = 0;

static FRESULT res;

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

static FATFS fsobj; 		//file system object
static FILINFO fileinf; 	//file information system
static DIR directory;

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void ReadDir(DIR* dir, FILINFO* fileinf, char* name);


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/*
bool InitFileSys()
{
	if(f_mount(&fsobj, "", 0))
	{
		return false;
	}

	return true;
}


bool OpenDir(TCHAR * path)
{
	if(!OpenFile(path))
	{
		return false;
	}
	return true;
}
*/

/*
bool GetPath()
{
	bool ok = false;
	if(SD_isSDcard() )
	{
		if(ok==false){
			ok = SD_initializationProcess();
		}
		bool isElement = true;
		f_mount(&fsobj,"",0);
		FILINFO archivos[100] = {0};
		uint8_t i = 1;
		fr = f_findfirst(&dj, &(archivos[0]), "\0ir", "*.*");
		do{
			if(archivos[i].fname[0] == 0)
				isElement = false;
			else
				fr = f_findnext(&dj, &(archivos[i++]));

		}while(fr == FR_OK && isElement);
		while(1);

	}
	else{
		SD_reset();
		ok = false;
	}
	return ok;

}

*/

bool mountSD()
{
	if(SD_isSDcard())
	{
		if(SD_initializationProcess())
		{
			if(f_mount(&fsobj,"",0) == FR_OK)
			{
				return true;
			}
		}
	}
	return false;
}

void readSD ()
{
	ReadDir(&directory, &fileinf, "");
	//i = 0;
}

char * getPath(int i)
{
	return &(path[i][0]);
}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
// entro a la sd -> leo la extension del archivo -> si es mp3, guardo el path -> si es dir entro y vuelvo a leer -> si es con . lo ignoro

void ReadDir(DIR* dir, FILINFO* fileinf, char* name)
{
	if(!f_opendir(dir, name))
	{
		for( ; ; )
		{
			f_readdir(dir, fileinf);
			if(fileinf->fname[0] == '.')
			{
				continue;
			}
			else if(fileinf -> fattrib == AM_DIR)
			{
				DIR newdir;
				FILINFO newfileinf;
				char* newname = strcat(strcat(name, '/'), fileinf -> fname);
				ReadDir(&newdir, &newfileinf, newname);
			}
			else if(strcmp(fileinf -> fname, ".mp3"))
			{
				//guardar el path
				int j = 0;
				char* filename = strcat(strcat(name, '/'), fileinf -> fname);
				while(*filename)
				{
					path[i][j] = *filename;
					filename++;
					j++;
				}
				i++;
			}
			else if(fileinf -> fname == 0)
			{
				break;
			}
			//else ya no queda nada y hago break
		}
	}
	
}
