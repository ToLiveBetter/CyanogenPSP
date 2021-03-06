#include <oslib/oslib.h>

#include <pspctrl.h>
#include <psploadexec.h>
#include <psploadexec_kernel.h>
#include <systemctrl.h>

#define MAX_GAME_DISPLAY	3 // max amount of files displayed on-screen.
#define GAME_DISPLAY_X		20 // X value of where the filebrowser is displayed.
#define GAME_DISPLAY_Y		87 // Y value of the filebrowser is displayed.
#define GAME_CURR_DISPLAY_Y     66 

#define game_xSelection 0 //The distance between the X axis of the two selections
#define game_ySelection 63 //The distance between the Y axis of the two selections

OSL_IMAGE *gamebg, *gameSelection, *icon0, *gameAnim[20];
OSL_FONT *Roboto;

char gameFontColorPath[250];

struct gameFontColor 
{
   int	r;
   int	g;
   int	b;
};

typedef struct
{
	char signature[4];

	int version;

	int offset[8];

} HEADER;

int launchEbootMs0(char path[]);
int launchEbootEf0(char path[]);
int launchPOPsMs0(char path[]);
int launchPOPsEf0(char path[]);
int launchISOMs0(char path[]);
int launchISOEf0(char path[]);
int launchUMD(char path[]);
void gameDisplay();
void gameControls(int n);
char * gameBrowse(const char * path);
char * popsBrowse(const char * path);
void gameUnloadAssets();
void getIcon0(char* filename);
OSL_IMAGE * processPBP(const char * path);
int gameView(char * browseDirectory, int type);
void checkGBootActivation();
int gameBoot();
int gameApp();
