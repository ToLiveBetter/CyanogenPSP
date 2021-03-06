#include "appDrawer.h"
#include "clock.h"
#include "fileManager.h"
#include "gameLauncher.h"
#include "homeMenu.h"
#include "include/systemctrl_se.h"  
#include "include/utils.h"
#include "lockScreen.h"
#include "powerMenu.h"
#include "recentsMenu.h"
#include "screenshot.h"
#include "settingsMenu.h"

struct gameFontColor fontColor;

int launchEbootMs0(char path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;

	// Clear Memory
	memset(&param, 0, sizeof(param));

	// Configure Parameters
	param.size = sizeof(param);
	param.args = strlen(path) + 1;
	param.argp = (void *)path;
	param.key = "game";

	// Trigger Reboot
	return sctrlKernelLoadExecVSHWithApitype(0x141, path, &param);
}

int launchEbootEf0(char path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;

	// Clear Memory
	memset(&param, 0, sizeof(param));

	// Configure Parameters
	param.size = sizeof(param);
	param.args = strlen(path) + 1;
	param.argp = (void *)path;
	param.key = "game";

	// Trigger Reboot
	return sctrlKernelLoadExecVSHWithApitype(0x152, path, &param);
}

int launchPOPsMs0(char path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;

	// Clear Memory
	memset(&param, 0, sizeof(param));

	// Configure Parameters
	param.size = sizeof(param);
	param.args = strlen(path) + 1;
    param.argp = (void *)path;
    param.key = "pops";

	// Trigger Reboot
	return sctrlKernelLoadExecVSHWithApitype(0x144, path, &param);
}

int launchPOPsEf0(char path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;

	// Clear Memory
	memset(&param, 0, sizeof(param));

	// Configure Parameters
	param.size = sizeof(param);
	param.args = strlen(path) + 1;
    param.argp = (void *)path;
    param.key = "pops";

	// Trigger Reboot
	return sctrlKernelLoadExecVSHWithApitype(0x155, path, &param);
}

int launchISOMs0(char path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;
	
	// Clear Memory
	memset(&param, 0, sizeof(param));
	
	// Set Common Parameters
	param.size = sizeof(param);
	
	// EBOOT Path
    char * ebootpath = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN";
               
    // Prepare ISO Reboot
    param.args = strlen(ebootpath) + 1;
    param.argp = ebootpath;
    param.key = "umdemu";
               
	// Enable Galaxy ISO Emulator Patch
    sctrlSESetBootConfFileIndex(MODE_INFERNO);
    sctrlSESetUmdFile(path);
	
	return sctrlKernelLoadExecVSHWithApitype(0x123, path, &param);
}

int launchISOEf0(char path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;
	
	// Clear Memory
	memset(&param, 0, sizeof(param));
	
	// Set Common Parameters
	param.size = sizeof(param);
	
	// EBOOT Path
    char * ebootpath = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN";
               
    // Prepare ISO Reboot
    param.args = strlen(ebootpath) + 1;
    param.argp = ebootpath;
    param.key = "umdemu";
               
	// Enable Galaxy ISO Emulator Patch
    sctrlSESetBootConfFileIndex(MODE_INFERNO);
    sctrlSESetUmdFile(path);
	
	return sctrlKernelLoadExecVSHWithApitype(0x125, path, &param);
}

int launchUMD(char path[])
{
	// Load Execute Parameter
	struct SceKernelLoadExecVSHParam param;
	
	// Clear Memory
	memset(&param, 0, sizeof(param));
	
	// Set Common Parameters
	param.size = sizeof(param);
	
	// EBOOT Path
    char * ebootpath = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN";
               
    // Prepare ISO Reboot
    param.args = strlen(ebootpath) + 1;
    param.argp = ebootpath;
    param.key = "game";

	return sctrlKernelLoadExecVSHWithApitype(0x120, path, &param);
}

void gameDisplay()
{	
	//char gPath[400] = "ms0:/PSP/GAME/";

	oslDrawImageXY(gamebg, 0, 0);
	oslDrawImageXY(gameSelection,-2,(current - curScroll)*63+GAME_CURR_DISPLAY_Y);

	battery(370,2,1);
	digitaltime(420,4,0,hrTime);

	/*if (IsNextDir())
	{
		strcat(gPath, GetNextFileName());
		ChangeDir(gPath);
		if (fileExists("EBOOT.PBP"))
		{
			getIcon0("EBOOT.PBP");
			if(icon0!=NULL)
			{	
				oslDrawImageXY(icon0, 326,118);
			}
		}
	}*/
		
	/*
	char * ext = strrchr(folderIcons[current].filePath, '.');
	
	if(((ext) != NULL) && (strcmp(ext ,".PBP") == 0))
	{
		icon0 = processPBP(folderIcons[current].filePath);
		oslDrawImageXY(icon0, 326,118);
	}	
	*/

	/*if (folderIcons[i].active == 1) 
	{
		getIcon0(folderIcons[current].filePath);
		if(icon0!=NULL)
		{	
			oslDrawImageXY(icon0, 326,118);
		}
	}	
	else if (folderIcons[i].active == 0)
	{
		oslDeleteImage(icon0);
	}*/

	// Displays the directories, while also incorporating the scrolling
	for(i=curScroll;i<MAX_GAME_DISPLAY+curScroll;i++) 
	{
		// Handles the cursor and the display to not move past the MAX_GAME_DISPLAY.
		// For moving down
		//if ((folderIcons[i].active == 0) && (current >= i-1)) {
	
		if ((folderIcons[i].active == 0) && (current >= i-1)) 
		{
			current = i-1;
			break;
		}
		// For moving up
		if (current <= curScroll-1) {
			current = curScroll-1;
			break;
		}                    
			
		// If the currently selected item is active, then display the name
		if (folderIcons[i].active == 1) 
		{
			oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
			oslDrawStringf(GAME_DISPLAY_X, (i - curScroll)*63+GAME_DISPLAY_Y, "%.41s", folderIcons[i].name);	// change the X & Y value accordingly if you want to move it
		}
	}
}

void gameControls(int n) //Controls
{
	oslReadKeys();	

	if (pad.Buttons != oldpad.Buttons) 
	{
		if (osl_keys->pressed.down) 
		{
			selectionDown(MAX_GAME_DISPLAY);
			timer = 0;
		}
		else if (osl_keys->pressed.up) 
		{
			selectionUp();
			timer = 0;
		}	
	
		if (osl_keys->pressed.right) 
		{
			selectionDownx5(MAX_GAME_DISPLAY);
			timer = 0;
		}
		else if (osl_keys->pressed.left) 
		{
			selectionUpx5();	
			timer = 0;
		}
		
		if (osl_keys->pressed.triangle) 
		{
			curScroll = 1;
			current = 1;
		}
	
		if (osl_keys->pressed.cross) 
		{
			oslPlaySound(KeypressStandard, 1);  
			openDir(folderIcons[current].filePath, folderIcons[current].fileType);
		}
	}
	
	volumeController();
	
	char * ext = strrchr(folderIcons[current].filePath, '.');
	
	if (n == 0) //For regular eboots
	{
		if (((ext) != NULL) && ((strcmp(ext ,".PBP") == 0) || (strcmp(ext ,".pbp") == 0)) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1); 
			if (kuKernelGetModel() == 4)
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchEbootEf0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchEbootEf0(folderIcons[current].filePath);
				}
			}
			else
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchEbootMs0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchEbootMs0(folderIcons[current].filePath);
				}
			}
		}
	}
	
	else if (n == 1) //For POPS
	{ 
		if (((ext) != NULL) && ((strcmp(ext ,".PBP") == 0) || (strcmp(ext ,".pbp") == 0)) && (osl_keys->pressed.cross))
		{
			oslPlaySound(KeypressStandard, 1); 
			if (kuKernelGetModel() == 4)
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchPOPsEf0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchPOPsEf0(folderIcons[current].filePath);
				}
			}
			else
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchPOPsMs0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchPOPsMs0(folderIcons[current].filePath);
				}
			}
		}
	}
	
	if (((ext) != NULL) && ((strcmp(ext ,".iso") == 0) || ((strcmp(ext ,".ISO") == 0)) || ((strcmp(ext ,".cso") == 0)) || ((strcmp(ext ,".CSO") == 0))) && (osl_keys->held.cross))
	{ 
		if (kuKernelGetModel() == 4)
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchISOEf0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchISOEf0(folderIcons[current].filePath);
				}
			}
			else
			{
				if(gBootActivator == 1)
				{
					gameBoot();
					launchISOMs0(folderIcons[current].filePath);
				}
				else if(gBootActivator == 0)
				{
					launchISOMs0(folderIcons[current].filePath);
				}
			}
	}
	
	if (osl_keys->pressed.circle)
	{	
		if((strcmp("ms0:/ISO", curDir)==0) || (strcmp("ms0:/PSP/GAME", curDir)==0) || (strcmp("ms0:/", curDir)==0))
		{
			gameUnloadAssets();
			gameApp();
		}
		else if((strcmp("ms0:/PSP/GAME/", curDir)!=0)) 
		{
			dirBack(1);
		}
		else if((strcmp("ms0:/ISO/", curDir)!=0))
		{
			dirBack(2);
		}	
	}
	
	if (osl_keys->pressed.square)
	{
		powermenu();
	}
		
	if (osl_keys->pressed.L)
	{
		oslPlaySound(Lock, 1);  
		lockscreen();
    }
	
	captureScreenshot();
	
	timer++;
	if ((timer > 30) && (pad.Buttons & PSP_CTRL_UP))
	{
		selectionUp();
		timer = 25;
	} 
	else if ((timer > 30) && (pad.Buttons & PSP_CTRL_DOWN))
	{
		selectionDown(MAX_GAME_DISPLAY);
		timer = 25;
	}

	if (current < 1) 
		current = 1;
	if (current > MAX_FILES) 
		current = MAX_FILES;
}

// Just call 'path' with whichever path you want it to start off in
char * gameBrowse(const char * path)
{
	folderScan(path);
	dirVars();

	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));	
		oldpad = pad;
		sceCtrlReadBufferPositive(&pad, 1);
		gameDisplay();
		gameControls(0);

		if (strlen(returnMe) > 4) 
			break;	
		
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}
	return returnMe;
}

char * popsBrowse(const char * path)
{
	folderScan(path);
	dirVars();
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		
		oslClearScreen(RGB(0,0,0));	
		oldpad = pad;
		sceCtrlReadBufferPositive(&pad, 1);
		gameDisplay();
		gameControls(1);
		
		if (strlen(returnMe) > 4) 
			break;		
		
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}
	return returnMe;
}

void gameUnloadAssets()
{
	oslDeleteImage(gamebg);
	oslDeleteImage(gameSelection);
}

void getIcon0(char* filename)
{
    //unsigned char _header[40];
    int icon0Offset, icon1Offset;
    char file[256];
    sprintf(file,"%s/eboot.pbp",filename);
    SceUID fd = sceIoOpen(file, PSP_O_RDONLY, 0777);
	
    if(fd < 0)
	{
		icon0 = oslLoadImageFilePNG("system/app/game/default.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
        return;
    }
	
    //sceIoRead(fd, _header, 40);
    //printf("letto header\n");
    sceIoLseek(fd, 12, SEEK_SET);
    sceIoRead(fd, &icon0Offset, 4);
    //sceIoLseek(fd, 23, SEEK_SET);
    sceIoRead(fd, &icon1Offset, 4);
    int icon0_size = icon1Offset - icon0Offset;
    sceIoLseek(fd, icon0Offset, SEEK_SET);
    unsigned char icon[icon0_size];
	
    if(icon0_size)
	{
        sceIoRead(fd, icon, icon0_size);
        oslSetTempFileData(icon, icon0_size, &VF_MEMORY);
        icon0 = oslLoadImageFilePNG(oslGetTempFileName(), OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    }
	
	else
	{
        icon0 = oslLoadImageFilePNG("system/app/game/default.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    }
    
	sceIoClose(fd);
}

OSL_IMAGE * processPBP(const char * path) 
{ 
	char *filename[8] = {"PARAM.SFO", "ICON0.PNG", "ICON1.PMF", "UNKNOWN.PNG", "PIC1.PNG", "SND0.AT3", "UNKNOWN.PSP", "UNKNOWN.PSAR"}; 
	
	OSL_IMAGE * imgSource; 
	OSL_IMAGE * imgDefault; 
    
	char curPath[512]; 
	getcwd(curPath, 512); 
	char loadImg[512]; 
	sprintf(loadImg, "%s/icon0.png", curPath); 
	int theSize = 0; 
	
	int loop0; 
	FILE *infile, *outfile; 
	HEADER header; 

	infile = fopen(path, "rb"); 

	fseek(infile, 0, SEEK_END); fseek(infile, 0, SEEK_SET); 

	if (fread(&header, sizeof(HEADER), 1, infile) < 0) 
	{ 
		printf("ERROR: Could not read the input file header. \n"); 
		return NULL; 
	} 

	if (header.signature[0] != 0x00) 
	{ 
		printf("ERROR: Input file is not a PBP file. \n"); 
		return NULL; 
	} 
	else if (header.signature[1] != 0x50) 
	{ 
		printf("ERROR: Input file is not a PBP file. \n"); 
		return NULL; 
	} 
	else if (header.signature[2] != 0x42)
	{ 	
		printf("ERROR: Input file is not a PBP file. \n"); 
		return NULL; 
	} 
	else if (header.signature[3] != 0x50) 
	{ 
		printf("ERROR: Input file is not a PBP file. \n"); 
		return NULL; 
	} 

	#ifdef __BIG_ENDIAN__ 
	for (loop0=0;loop0<2;loop0++) 
	{ 
		header.offset[loop0] = NXSwapInt(header.offset[loop0]); 
    } 
	#endif 

	// loop0 = 0 is the param.sfo (reads the data then erases, because you have to loop or it ***** up) 
	// loop0 = 1 is the icon0.png (reads the data, spits out a file; we load, then delete it) 
	for (loop0=0; loop0<2; loop0++) 
	{ 
		void *buffer; int size; 
        
		size = header.offset[loop0 + 1] - header.offset[loop0]; 
		
		buffer = malloc(size); 
		if (buffer == NULL) 
		{ 
			printf("ERROR: Could not allocate the section data buffer. (%d)\n", size); 
			return NULL; 
		} 

		if (fread(buffer, size, 1, infile) < 0) 
		{ 
			printf("ERROR: Could not read in the section data.\n"); 
			return NULL; 
		} 

		if (loop0==1) 
		{ 
			// WTF (works great until after loading 10 homebrews!?) 
			// Mess up is happening here 
			outfile = fopen(filename[loop0], "wb");          
			if (outfile == NULL)
			{ 
				printf("ERROR: Could not open the output file. (%s)\n", filename[loop0]); 
				return NULL; 
			} 
			// Mess up is happening here 

			if (fwrite(buffer, size, 1, outfile) < 0) 
			{ 
				printf("ERROR: Could not write out the section data.\n"); 
				return NULL; 
			} 
        
			if (fclose(outfile) < 0) 
			{ 
				printf("ERROR: Could not close the output file.\n"); 
				return NULL; 
			} 
		} 
		free(buffer); 
	} 
	
	if (fclose(infile) < 0) 
	{ 
		printf("ERROR: Could not close the input file.\n"); 
		return NULL; 
	}

	theSize = getFileSize(loadImg); 
	printf("%02d \n", theSize); // Debug 

	if (theSize > 0) 
	{ 
		imgSource = oslLoadImageFilePNG(loadImg, OSL_IN_RAM, OSL_PF_8888); 
		if (!(imgSource)) 
		{ 
			sceIoRemove(loadImg); 
			imgDefault = oslLoadImageFilePNG("system/app/game/default.png", OSL_IN_RAM, OSL_PF_8888); 
			return (imgDefault); 
		} 
	} 
	else 
	{ 
		sceIoRemove(loadImg); 
		imgDefault = oslLoadImageFilePNG("system/app/game/default.png.png", OSL_IN_RAM, OSL_PF_8888);  
		return (imgDefault); 
	} 
	sceIoRemove(loadImg); 
	return (imgSource); 
}

int gameView(char * browseDirectory, int type)
{	
	gamebg = oslLoadImageFilePNG(gameBgPath, OSL_IN_RAM, OSL_PF_8888);
	gameSelection = oslLoadImageFilePNG(gameSelectorPath, OSL_IN_RAM, OSL_PF_8888);

	if (type == 0)
	{
		gameBrowse(browseDirectory); //For PSP Eboots
	}
	
	else if (type == 1)
	{
		popsBrowse(browseDirectory); //For POPS Eboots
	}
	
	while (!osl_quit)
	{		
		LowMemExit();
	
		oslStartDrawing();
		oslClearScreen(RGB(0,0,0));	
		
		centerText(480/2, 272/2, browseDirectory, 50);	// Shows the path that 'Directory' was supposed to receive from mp3Browse();
	 
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}	
	return 0;
}

void checkGBootActivation()
{
	FILE * gBootActivation;

	if (!(fileExists("system/app/game/boot/gBootActivator.bin")))
	{
		gBootActivation = fopen("system/app/game/boot/gBootActivator.bin", "w");
		fprintf(gBootActivation, "1");
		fclose(gBootActivation);
	}

	gBootActivation = fopen("system/app/game/boot/gBootActivator.bin", "r");
	fscanf(gBootActivation,"%d",&gBootActivator);
	fclose(gBootActivation);
}

int gameBoot()
{
	int currentFrame = 0, i = 0;

    gameAnim[0] = oslLoadImageFileGIF("system/app/game/boot/0.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[1] = oslLoadImageFileGIF("system/app/game/boot/1.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[2] = oslLoadImageFileGIF("system/app/game/boot/2.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[3] = oslLoadImageFileGIF("system/app/game/boot/3.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[4] = oslLoadImageFileGIF("system/app/game/boot/4.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[5] = oslLoadImageFileGIF("system/app/game/boot/5.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[6] = oslLoadImageFileGIF("system/app/game/boot/6.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[7] = oslLoadImageFileGIF("system/app/game/boot/7.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[8] = oslLoadImageFileGIF("system/app/game/boot/8.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[9] = oslLoadImageFileGIF("system/app/game/boot/9.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[10] = oslLoadImageFileGIF("system/app/game/boot/10.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[11] = oslLoadImageFileGIF("system/app/game/boot/11.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[12] = oslLoadImageFileGIF("system/app/game/boot/12.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[13] = oslLoadImageFileGIF("system/app/game/boot/13.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[14] = oslLoadImageFileGIF("system/app/game/boot/14.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[15] = oslLoadImageFileGIF("system/app/game/boot/15.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[16] = oslLoadImageFileGIF("system/app/game/boot/16.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[17] = oslLoadImageFileGIF("system/app/game/boot/17.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[18] = oslLoadImageFileGIF("system/app/game/boot/18.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	gameAnim[19] = oslLoadImageFileGIF("system/app/game/boot/19.gif", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);

	if (!gameAnim[0] || !gameAnim[1] || !gameAnim[2] || !gameAnim[3] || !gameAnim[4] || !gameAnim[5] || !gameAnim[6] || !gameAnim[7] || !gameAnim[8] || !gameAnim[9] || !gameAnim[10] || !gameAnim[11] || !gameAnim[12] || !gameAnim[13] || !gameAnim[14] || !gameAnim[15] || !gameAnim[16] || !gameAnim[17] || !gameAnim[18] || !gameAnim[19])
		debugDisplay();
	
    while(!osl_quit)
	{
		oslStartDrawing();
		oslClearScreen(RGB(255,255,255));
		oslDrawImageXY(gameAnim[currentFrame], 80, -25); 
		sceDisplayWaitVblankStart();
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		sceKernelDelayThread(10000 * 2);
   
		currentFrame++;
		if(currentFrame > 19)
		{
			currentFrame = 0;
			i++;
		}
		
		if (i > 7)
		{
			return 1;
		}
	}
	return 0;
}

int gameApp() 
{	 
	FILE * file = fopen(gameFontColorPath, "r");
	fscanf(file, "%d %d %d", &fontColor.r, &fontColor.g, &fontColor.b);
	fclose(file);
	
	gamebg = oslLoadImageFilePNG(gameBgPath, OSL_IN_RAM, OSL_PF_8888);
	gameSelection = oslLoadImageFilePNG(gameSelectorPath, OSL_IN_RAM, OSL_PF_8888);
	
	if (!gamebg || !gameSelection)
		debugDisplay();

	oslSetFont(Roboto);
		
	checkGBootActivation();
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = -2; //The x position of the first selection
	int selector_y = 3; //The y position of the first selection
	int selector_image_x; //Determines the starting x position of the selection
	int selector_image_y = 0; //Determines the starting y position of the selection
	int numMenuItems = 3; //Amount of items in the menu
	int selection = 0;

	while (!osl_quit)
	{		
		LowMemExit();
	
		selector_image_x = selector_x+(game_xSelection*MenuSelection); //Determines where the selection image is drawn for each selection
		selector_image_y = selector_y+(game_ySelection*MenuSelection); //Determines where the selection image is drawn for each selection
	
		oslStartDrawing();
		oslReadKeys();
		oslClearScreen(RGB(0,0,0));	
		oslDrawImageXY(gamebg, 0, 0);
		oslIntraFontSetStyle(Roboto, fontSize, RGBA(fontColor.r, fontColor.g, fontColor.b, 255), 0, INTRAFONT_ALIGN_LEFT);
		oslDrawImageXY(gameSelection, selector_image_x, selector_image_y);
		
		oslDrawStringf(20,87,"GAME");
		oslDrawStringf(20,150,"ISO/CSO");
		oslDrawStringf(20,213,"POPS");

		battery(370,2,1);
		digitaltime(420,4,0,hrTime);	
		volumeController();
        
        if (osl_keys->pressed.down) MenuSelection++; //Moves the selector down
        if (osl_keys->pressed.up) MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) MenuSelection = numMenuItems; //Sets the selection back to last
		
		if (MenuSelection == 1 && osl_keys->pressed.cross)
        {		
			oslPlaySound(KeypressStandard, 1);  
			gameUnloadAssets();
			gameView("ms0:/PSP/GAME", 0); //PSP Homebrew
        }
		else if (MenuSelection == 2 && osl_keys->pressed.cross)
        {		
			oslPlaySound(KeypressStandard, 1);  
			gameUnloadAssets();
			gameView("ms0:/ISO", 0); //ISO backups
        }
		else if (MenuSelection == 3 && osl_keys->pressed.cross)
        {		
			oslPlaySound(KeypressStandard, 1);  
			gameUnloadAssets();
			gameView("ms0:/PSP/GAME", 1); //POPS
        }

		if (osl_keys->pressed.circle)
		{
			gameUnloadAssets();
			appdrawer();
		}
		
		if (osl_keys->pressed.square)
		{
			powermenu();
		}
		
		if (osl_keys->pressed.L)
		{
			oslPlaySound(Lock, 1);  
			lockscreen();
		}
	
		captureScreenshot();
		
		oslEndDrawing(); 
        oslEndFrame(); 
		oslSyncFrame();	
	}	
	return selection;
}
