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

static char path[CANTSONGS_MAX][255];
static int i;

static int cantsongs;

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

static void ReadDir(DIR* dir, FILINFO* fileinf, char* dirname);


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void InitFileSys()
{
	i = 0;
	cantsongs = 0;
	SD_init();
	
	//"Boca_Yo_Te_Amo.mp3""himno1.mp3""El Sensei (64).mp3"

/*
	strcpy(path[0], "/Boca_Yo_Te_Amo.mp3");
	strcpy(path[1], "/himno1.mp3");
	strcpy(path[2], "/El Sensei (64).mp3");
	strcpy(path[3], "/HelloWorld-1.mp3");
	strcpy(path[4], "/himno2.mp3");
	strcpy(path[5], "/El Sensei (120).mp3");


	cantsongs = 6;

*/



}




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
	//name = "";
	ReadDir(&directory, &fileinf, "");
	i = 0;
}

char * getPath(int _i)
{
	return &(path[_i][0]);
}

int getCantSongs()
{
	return cantsongs;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
// entro a la sd -> leo la extension del archivo -> si es mp3, guardo el path -> si es dir entro y vuelvo a leer -> si es con . lo ignoro

static void ReadDir(DIR* dir, FILINFO* fileinf, char* dirname)
{
	char namedir[255] = {0};
	char * cpy = strcpy(namedir, dirname);
	int ok = 0;
	int ch = '.';
	char * ext;
	if(!f_opendir(dir, &namedir))
	{
		for( ; ; )
		{
			char namefile[255] = {0};
			f_readdir(dir, fileinf);
			if(fileinf->fname[0] == '.')
			{
				continue;
			}
			else if(fileinf -> fname[0] == 0)
			{
				break;
			}
			else if(fileinf -> fattrib == AM_DIR)
			{
				DIR newdir;
				FILINFO newfileinf;
				char* newnamedir[255] = {0};
				char* newname = strcat(strcat(newnamedir, "/"), fileinf -> fname);
				ReadDir(&newdir, &newfileinf, newname);
			}
			else if(!(ok = strcmp(ext = strchr(fileinf -> fname, ch), ".mp3")))		//identicas=0. Izq contenido en der>0. Der contenido en izq<0
			{
				if(i == CANTSONGS_MAX)
				{
					break;
				}
				else
				{
					cantsongs++;
					int j = 0;
					char* filename = strcat(strcat(namefile, "/"), fileinf -> fname);
					strncpy(path[i], filename, strlen(filename)+1);	//+1 por el terminador
					i++;
				}
				//guardar el path
			}
			//else ya no queda nada y hago break
		}
	}
	
}
