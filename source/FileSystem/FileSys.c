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

//static char name[255] = "";

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

void ReadDir(DIR* dir, FILINFO* fileinf, char* dirname);


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/*
void InitFileSys()
{

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
	//name = "";
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

void ReadDir(DIR* dir, FILINFO* fileinf, char* dirname)
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
				//char newname[255] = strcat(strcat(name, '/'), fileinf -> fname);
				char* newname = strcat(strcat(newnamedir, "/"), fileinf -> fname);
				ReadDir(&newdir, &newfileinf, newname);
				//volver el name a nada?
			}
			else if(!(ok = strcmp(ext = strchr(fileinf -> fname, ch), ".mp3")))		//identicas=0. Izq contenido en der>0. Der contenido en izq<0
			{
				//guardar el path
				int j = 0;
				char* filename = strcat(strcat(namefile, "/"), fileinf -> fname);
				while(*filename)
				{
					path[i][j] = *filename;
					filename++;
					j++;
				}
				i++;
			}
			//else ya no queda nada y hago break
		}
	}
	
}
