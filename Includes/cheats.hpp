#ifndef CHEATS_H
#define CHEATS_H

#include <3ds.h>
#include <CTRPluginFramework.hpp>
#include "Helpers/Game.hpp"
#include "Helpers/AnimData.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/PlayerPTR.hpp"
#include "Helpers/ItemSequence.hpp"
#include "Helpers/Region.hpp"
#include "Helpers/ACSystem.hpp"
#include "Helpers/ACMessageBox.hpp"
#include "Helpers/GameKeyboard.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Town.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/NPC.hpp"
#include "NonHacker.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"
#include "MenuPointers.hpp"
#include "Helpers/QRCodeGen.hpp"
#include "Helpers/CustomButton.hpp"
#include "Helpers/KeySequence.hpp"

#define PATH_PRESET		"Vapecord/PreSet"
#define PATH_LOGO		"Vapecord/logo.bin"
#define PATH_ACRE		"Vapecord/Acre/acre%X.bin"
#define PATH_PICON		"Vapecord/PlayerIcon/Player%d.bin"

#define PATH_BULLETIN	"Vapecord/Bulletin"
#define PATH_SAVE		"Vapecord/Save"
#define PATH_TPC		"Vapecord/TPC"
#define PATH_ITEMSET	"Vapecord/Set"
#define PATH_PLAYER		"Vapecord/Player"
#define PATH_CUSTOM		"Vapecord/Custom"
#define PATH_DESIGN		"Vapecord/Design"
#define PATH_CRASH		"Vapecord/CrashDump"

#define PATH_COLOR		"Vapecord/color.bin"
#define PATH_CBOARD		"Vapecord/clipboard.txt"
#define ITEMLIST		"Vapecord/item.txt"
#define CONFIGNAME		"Vapecord/plugin.bin"

#define DISCORDINV		"discord.gg/QwqdBpKWf3"

#define SAVE_GARDENPLUS 0x89B00  //0x95CF94
#define SAVE_MAIL 		0x1C208  //0x983050
#define SAVE_EXHIBITION 0x17BE10 //0x9B4AC0
#define SAVE_FRIEND 	0x29608  //0x983038
#define SAVE_AMIIBO 	0x128 	 //0x95CF88
#define SAVE_DESIGN 	0x25F90  //0x983088
#define SAVE_PLAYER		0xC8B8	 //0x983010

#define TID_USA 		0x0004000000086300
#define TID_USAWA 		0x0004000000198E00
#define TID_EUR 		0x0004000000086400
#define TID_EURWA 		0x0004000000198F00
#define TID_JPN 		0x0004000000086200
#define TID_JPNWA 		0x0004000000198D00
#define TID_KOR 		0x0004000000086500
#define TID_KORWA 		0x0004000000199000
#define TID_EURWL 		0x00040000004C5700

extern bool IsIndoorsBool;
extern u32 PickupSeederItemID;

namespace CTRPluginFramework {
	extern const u32 majorV;
	extern const u32 minorV;
	extern const u32 revisV;
	extern const std::string fullV;

	enum class CONFIG : s64 {
		Color = 0,
		FWKColor,
		Language,
		Info,
		DevMode,
		Version
	};

	extern Result retcode;
	extern int res;

	extern const IDS1 Buildings[205];
	extern const IDS1 Countrys[134];
	extern const char* const amiiboSpecies[36];
	extern const SPAmiiboInfo amiiboSPVillagers[55];
    extern const AmiiboInfo amiiboVillagers[399];

	extern bool DropPatternON;
	extern bool turbo;
	extern bool MapEditorActive;
	extern bool bypassing;
	extern bool animExecuting;
	extern bool fovbool;
	extern bool save;
	extern bool IfForceAllowed;
	
//Animation Related
	extern u32 emotion;
	extern u32 soundID;
	extern u32 itemslotid;
	extern u16 snake;
	extern u8 appearance[3];
	extern u8 anim1;
	extern u8 waitAnim;
	extern u8 animToForce;
	extern u32 animItem;
	
//Dropper Related
	extern u8 DropType;
	extern u8 DropRoom;
	extern u32 ItemIDToReplace;
	extern u32 dropitem;
	extern u32 selectedX; 
	extern u32 selectedY; 
	extern u32 selectedItem; 
	extern u8 dropAnims[];

	extern const std::string g_Empty;
	
//Colors
	extern const Color pBlue;
	extern const Color pRed;
	extern const Color pGreen;
	extern const Color pYellow;
	extern const Color PressedButton;
	extern const Color SelectedText;
	extern const Color SpecialRed;
	
	extern const Color pColor[];

	struct Item {
		std::vector<std::string> Name;
		std::vector<u16> ID;
	};
	
	extern Item* ItemList;
	extern int ItemFileLenght;
	extern bool ItemFileExists;

//Save Codes Folder
	void townnamechanger(MenuEntry* entry);
	void savebackup(MenuEntry *entry);
	void bullboard(MenuEntry *entry);
	void TreeSizeChanger(MenuEntry *entry);
	void ChangeNativeFruit(MenuEntry *entry);
	void PWPUnlock(MenuEntry *entry);
	void unlockqrmachine(MenuEntry *entry);
	void GrassChanger(MenuEntry *entry);
	void HouseChanger(MenuEntry *entry);
	void caravanchange(MenuEntry *entry);
	void SetCampingVillager(MenuEntry *entry);
	void Permit100(MenuEntry *entry);	
	void MapEditor(MenuEntry *entry);
	
//Movement Codes Folder
	void noPush(MenuEntry *entry);
	void coordspeed(MenuEntry *entry);
	void coordinate(MenuEntry *entry);
	void moonjump(MenuEntry *entry);
	void tch_warp(MenuEntry *entry);
	void walkOver(MenuEntry *entry);
	void MovementChanger(MenuEntry *entry);
	void Walkparticle(MenuEntry *entry);
	void stalk(MenuEntry *entry);
	void onlineplayermod(MenuEntry *entry);
	void speedMod(MenuEntry *entry);
	void menuSpeedMod(MenuEntry *entry);
	void roomWarp(MenuEntry *entry);
	
//Inventory Codes Folder
	void t2i(MenuEntry *entry);
	void duplication(MenuEntry *entry);
	void catalog(MenuEntry *entry);
	void itemsettings(MenuEntry *entry);
	void MenuChanger(MenuEntry *entry);
	void getset(MenuEntry *entry);
	
//Player Codes Folder
	//Player Save Codes
	void NameChanger(MenuEntry* entry);
	void tpcmessage(MenuEntry* entry);
	void tpc(MenuEntry *entry);
	void DesignDumper(MenuEntry *entry);
	void emotelist(MenuEntry *entry);
	void enzyklopedia(MenuEntry *entry);
	void comodifier(MenuEntry *entry);
	void debug1(MenuEntry *entry);
	void FillSongs(MenuEntry *entry);
	void FillCatalog(MenuEntry *entry);
	void playermod(MenuEntry *entry);
	void randomoutfit(MenuEntry *entry);
	void playerbackup(MenuEntry *entry);
	
	void neckentry(MenuEntry *entry);
	void debug(MenuEntry *entry);
	void hatz(MenuEntry *entry);
	void pLoaderEntry(MenuEntry *entry);
	void Faint(MenuEntry *entry);
	void map(MenuEntry *entry);
	
//Animation Codes Folder
	void playerSelector(MenuEntry *entry);
	void execAnim(MenuEntry *entry);
	void inf_expression(MenuEntry *entry);
	void idle(MenuEntry *entry);
	void slmoanms(MenuEntry *entry);
	void doonall(MenuEntry *entry);
	
//Seeding Codes
	//Seed Codes
	void pickseeder(MenuEntry *entry);
	void Walkseeder(MenuEntry *entry);
	void fireworkentry(MenuEntry *entry);
	void tileSelector(MenuEntry *entry);
	void E_Seeder_KB(MenuEntry *entry);
	void everythingseeder(MenuEntry *entry);
	//Drop Codes
	void Entry_itemsequence(MenuEntry *entry);
	void dropMod(MenuEntry *entry);
	void instantDrop(MenuEntry *entry);
	void autoDrop(MenuEntry *entry);
	void touchDrop(MenuEntry *entry);
	void ShowInvSlotID(MenuEntry *entry);	
	//Tree Codes
	void fruitStays(MenuEntry *entry);
	void shakechop(MenuEntry *entry);
	void fruititemmod(MenuEntry *entry);
	void instantchop(MenuEntry *entry);
	
//Money Codes
	void wallet(MenuEntry *entry);
	void bank(MenuEntry *entry);
	void coupon(MenuEntry *entry);
	void badges(MenuEntry *entry);
	void medals(MenuEntry *entry);
	void turnips(MenuEntry *entry);
	
//Island Codes
	void UnlockIsland(MenuEntry *entry);
	void bonusOre(MenuEntry *entry);
	void instantFruit(MenuEntry *entry);
	void Hackerisland(MenuEntry *entry);
	void Countryspoof(MenuEntry *entry);
	void IslandShop(MenuEntry *entry);
	void IslandSettings(MenuEntry *entry);
	void alltour(MenuEntry *entry);
	void acreMod(MenuEntry *entry);
	void menuAcreMod(MenuEntry *entry);
	void buildingMod(MenuEntry *entry);
	void menuBuildingMod(MenuEntry *entry);
	void FreeKappn(MenuEntry *entry);
	
//Fun Codes
	void sizecodes(MenuEntry *entry);
	void tposeentry(MenuEntry *entry);
	void freezeframe(MenuEntry *entry);
	void maxturbo(MenuEntry *entry);
	void asmpresses(MenuEntry *entry);
	void partypopper(MenuEntry *entry);
	void cameramod(MenuEntry *entry);
	void wandability(MenuEntry *entry);
	
//Extra Codes
	//Fish Codes
	void FishAlwaysBiteRightAway(MenuEntry *entry);
	void FishCantBeScared(MenuEntry *entry);
	//Chat Codes
	void bubblesDisappear(MenuEntry *entry);
	void Forcesendchat(MenuEntry *entry);
	void ChatCopyPaste(MenuEntry *entry);

	void AmiiboSpoofer(MenuEntry *entry);
	void ShopsAlwaysOpen(MenuEntry *entry);
	void shopunlocks(MenuEntry *entry);
	void nonesave(MenuEntry *entry);	
	void bypass(MenuEntry *entry);
	void noTrap(MenuEntry *entry);	
	void mailtext(MenuEntry *entry);
	void WaterAllFlowers(MenuEntry *entry);
	void weedremover(MenuEntry *entry);
	void editpattern(MenuEntry *entry);	
	void grasscomplete(MenuEntry *entry);	
	void grasseditor(MenuEntry *entry);
	void AmiiboSpoof(MenuEntry *entry);
	void TTKeyboard(MenuEntry *entry);
	void TimeTravel(MenuEntry *entry);
	
//Misc Codes
	void disablecommands(MenuEntry *entry);
	void tooltype(MenuEntry *entry);
	void mgtype(MenuEntry *entry);
	void unbreakableflower(MenuEntry *entry);
	void Weathermod(MenuEntry *entry);
	void Musicchange(MenuEntry *entry);
	void miscFunctions(MenuEntry *entry);
	void morenumberisland(MenuEntry *entry);
	void fovlarge(MenuEntry *entry);
	void roomSeeder(MenuEntry *entry);
	void walktalkentry(MenuEntry *entry);
	void key_limit(MenuEntry* entry);
	void CustomKeyboard(MenuEntry* entry);
	void Beans(MenuEntry *entry);
	void Daytime(MenuEntry *entry);
	void fast(MenuEntry *entry);
	void fasttalk(MenuEntry *entry);
	void speedentry(MenuEntry *entry);

//Dev Codes	
	void customdump(MenuEntry *entry);
	void FunctionsCaller(MenuEntry *entry);
	void FunctionsCallerSettings(MenuEntry *entry);
	void callfunction(MenuEntry *entry);
	void chatt2i(MenuEntry *entry);
	void npcMenu(MenuEntry *entry);
	void npcCoordinates(MenuEntry *entry);
	void npcAnimation(MenuEntry *entry);
	void msgboxtest(MenuEntry *entry);
	void falldownfishing(MenuEntry *entry);
	void anticheat(MenuEntry *entry);
	void Analyzer(MenuEntry *entry);
	void islanditems(MenuEntry *entry);
	void acnh(MenuEntry *entry);
	void PlayerLoader(MenuEntry *entry);
	void player_dumper(MenuEntry *entry);
	void valuedisplayer(MenuEntry *entry);
	void keymap(MenuEntry *entry);
	void SaveColor(MenuEntry *entry);
	void App_ColorMod(MenuEntry *entry);
	//void RestoreAll(MenuEntry *entry);
	void FishThrower(MenuEntry *entry);
	void lightswitch(MenuEntry *entry);
	void SettingsButton(MenuEntry *entry);
	void CustomSaveScreen(MenuEntry *entry);

//NPC Codes
	void NPCFunction(MenuEntry *entry);
	void NPCSetAnim(MenuEntry *entry);
	void NPCAnimation(MenuEntry *entry);
	void NPCCoordinates(MenuEntry *entry);
	void NPCTeleportToYou(MenuEntry *entry);
	void NPCRotate(MenuEntry *entry);
	
//Helper functions
	void ReserveItemData(Item* out);
  //Located in cheats.cpp
  	void RestoreDropPattern(void);
	u32 Rainbow();
	bool CallbackRainbow(void);
	void rainbow(MenuEntry *entry);
  //Located in Plugin_Settings.cpp
	void pluginsettings(MenuEntry *entry);
	void gamesettings(MenuEntry *entry);
  //Located in Plugin_Patches.cpp
	void PatchProcess(FwkSettings &settings);
	void RCO(void);
  //Located in MiscCodes.cpp
	void GetPlayerInfoData(void);
  //Located in MenuCreate.cpp
	void InitMenu(PluginMenu *menu);

	bool IsDevModeUsable(void);
	void StartingMsg(void);
	void CustomFWK(bool SetInMenu);
	void SetupColors(bool SetInMenu);
	void SetupLanguage(bool SetInMenu);
	void CheckForCONFIG(void);
	void Check_Color_Language(void);
	
//Callbacks
	void OnNewFrameCallback(Time ttime);
  //Located in NonHacker.cpp
	void NonHackerCallBack(void);
  //Located in Plugin_Setting.cpp
	void devcallback(void);
  //Located in Plugin_Checks.cpp
	void IndoorsSeedItemCheck(void);
	void AccidentalCrashCheck(void);

	int ItemSearch(const std::string& match, Item& out);
	std::string ItemIDSearch(u16 ItemID);
}

#endif
