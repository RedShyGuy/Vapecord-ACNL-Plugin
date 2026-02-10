#pragma once

#include <3ds.h>
#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"
#include "Address/Address.hpp"
#include "Language.hpp"
#include "TextID.hpp"
#include "LibCtrpfExtras/OSDExtras.hpp"

extern CTRPluginFramework::Item PickupSeederItemID;

namespace CTRPluginFramework {
	extern const std::string fullV;

//Save Codes Folder
	void townnamechanger(MenuEntry* entry);
	void savebackup(MenuEntry *entry);
	void bullboard(MenuEntry *entry);
	void TreeSizeChanger(MenuEntry *entry);
	void ChangeNativeFruit(MenuEntry *entry);
	void PWPUnlock(MenuEntry *entry);
	void BuildingMod(MenuEntry *entry);
	void GrassChanger(MenuEntry *entry);
	void HouseChanger(MenuEntry *entry);
	void caravanchange(MenuEntry *entry);
	void SetCampingVillager(MenuEntry *entry);
	void CompleteMuseum(MenuEntry *entry);
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
	void shovelknockback(MenuEntry *entry);
	
//Inventory Codes Folder
	void itemsearch(MenuEntry *entry);
	void t2i(MenuEntry *entry);
	void duplication(MenuEntry *entry);
	void catalog(MenuEntry *entry);
	void itemsettings(MenuEntry *entry);
	void MenuChanger(MenuEntry *entry);
	void ClearInventory(MenuEntry *entry);
	void getset(MenuEntry *entry);
	void SettingsButton(MenuEntry *entry);
	void chatt2i(MenuEntry *entry);
	
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
	void unlockqrmachine(MenuEntry *entry);
	
	void neckentry(MenuEntry *entry);
	void debug(MenuEntry *entry);
	void hatz(MenuEntry *entry);
	void Faint(MenuEntry *entry);
	void map(MenuEntry *entry);
	void NeverBedHead(MenuEntry *entry);
	void SaveColor(MenuEntry *entry);
	void App_ColorMod(MenuEntry *entry);
	
//Animation Codes Folder
	void playerSelector(MenuEntry *entry);
	void execAnim(MenuEntry *entry);
	void inf_expression(MenuEntry *entry);
	void idle(MenuEntry *entry);
	void slmoanms(MenuEntry *entry);
	void doonall(MenuEntry *entry);
	void anticheat(MenuEntry *entry);
	
//Seeding Codes
	bool SetSeederInfos(void);
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
	void IslandShop(MenuEntry *entry);
	void IslandSettings(MenuEntry *entry);
	void alltour(MenuEntry *entry);
	void acreMod(MenuEntry *entry);
	void menuAcreMod(MenuEntry *entry);
	void buildingMod(MenuEntry *entry);
	void menuBuildingMod(MenuEntry *entry);
	void FreeKappn(MenuEntry *entry);
	void IslandSaver(MenuEntry *entry);
	
//Fun Codes
	void sizecodes(MenuEntry *entry);
	void tposeentry(MenuEntry *entry);
	void freezeframe(MenuEntry *entry);
	void maxturbo(MenuEntry *entry);
	void asmpresses(MenuEntry *entry);
	void partypopper(MenuEntry *entry);
	void cameramod(MenuEntry *entry);
	void SetFacialExpression(MenuEntry *entry);
	void FacialExpressionMod(MenuEntry *entry);
	void wandability(MenuEntry *entry);

//Environment Codes
	//Fish Codes
	void FishAlwaysBiteRightAway(MenuEntry *entry);
	void FishCantBeScared(MenuEntry *entry);

	//Insect Codes
	void SetInsectIdEntry(MenuEntry *entry);
	void SpawnInsectEntry(MenuEntry *entry);
	void InsectsCantBeScared(MenuEntry *entry);

	void Daytime(MenuEntry *entry);
	void auroralights(MenuEntry *entry);
	void unbreakableflower(MenuEntry *entry);
	void Weathermod(MenuEntry *entry);
	void WaterAllFlowers(MenuEntry *entry);
	void weedremover(MenuEntry *entry);
	void grasscomplete(MenuEntry *entry);
	void grasseditor(MenuEntry *entry);
	void KeepGrassState(MenuEntry *entry);
	void BuriedInspector(MenuEntry *entry);
	void ItemsDontDissappearOnInvalidPositions(MenuEntry *entry);
	
//Extra Codes
	//Chat Codes
	void bubblesDisappear(MenuEntry *entry);
	void Forcesendchat(MenuEntry *entry);
	void chatCommands(MenuEntry *entry);
	void ChatCopyPaste(MenuEntry *entry);
	void ChatButton(MenuEntry *entry);

	void AmiiboSpoofer(MenuEntry *entry);
	void ShopsAlwaysOpen(MenuEntry *entry);
	void shopunlocks(MenuEntry *entry);
	void nonesave(MenuEntry *entry);	
	void bypass(MenuEntry *entry);
	void noTrap(MenuEntry *entry);	
	void SetSpotState(MenuEntry *entry);
	void SearchReplace(MenuEntry *entry);
	void RemoveItemsCheat(MenuEntry *entry);
	void editpattern(MenuEntry *entry);	
	void AmiiboSpoof(MenuEntry *entry);
	void TTKeyboard(MenuEntry *entry);
	void TimeTravel(MenuEntry *entry);
	
//Misc Codes
	void tooltype(MenuEntry *entry);
	void mgtype(MenuEntry *entry);
	void radioPlayer(MenuEntry *entry);
	void ReloadRoomCheat(MenuEntry *entry);
	void morenumberisland(MenuEntry *entry);
	void fovlarge(MenuEntry *entry);
	void roomSeeder(MenuEntry *entry);
	void walktalkentry(MenuEntry *entry);
	void key_limit(MenuEntry* entry);
	void CustomKeyboard(MenuEntry* entry);
	void BeansParticleChanger(MenuEntry *entry);
	void fast(MenuEntry *entry);
	void fasttalk(MenuEntry *entry);
	void speedentry(MenuEntry *entry);
	void fastisabelle(MenuEntry *entry);

//Dev Codes	
	void customdump(MenuEntry *entry);
	void FunctionsCaller(MenuEntry *entry);
	void FunctionsCallerSettings(MenuEntry *entry);
	
	void displaySaveFlagDifference(MenuEntry *entry);
	void falldownfishing(MenuEntry *entry);
	void unlockCroRegion(MenuEntry *entry);
	void acnh(MenuEntry *entry);
	void player_dumper(MenuEntry *entry);
	void FishThrower(MenuEntry *entry);
	void lightswitch(MenuEntry *entry);

//NPC Codes
	void NPCFunction(MenuEntry *entry);
	void NPCSetAnim(MenuEntry *entry);
	void NPCAnimation(MenuEntry *entry);
	void NPCCoordinates(MenuEntry *entry);
	void NPCTeleportToYou(MenuEntry *entry);
	void NPCRotate(MenuEntry *entry);

//Default Codes
	void EnableAllChecks(void);
	void EnableAllPatches(void);
	void DisableAllChecks(void);
	void DisableAllPatches(void);

	void CheckInvalidBadgeEntry(MenuEntry *entry);
	void SeedItemLegitimacyEntry(MenuEntry *entry);
	void OnlineDropLagRemoverEntry(MenuEntry *entry);
    void ChangeRockbreakParticleEntry(MenuEntry *entry);
    void DropItemsEverywhereEntry(MenuEntry *entry);
    void IdleAfterTreeShakeOrCutEntry(MenuEntry *entry);
    void DontMoveNPCBackToOriginalPositionEntry(MenuEntry *entry);
	void ReplaceDropFunctionsEntry(MenuEntry *entry);
	void PreventParticleCrashEntry(MenuEntry *entry);
	void BypassGameChecksEntry(MenuEntry *entry);
	void DisableNonSeedItemCheckEntry(MenuEntry *entry);
	void PatchDropFunctionEntry(MenuEntry *entry);

	void DisableOpenSaveMenuWithStartButton(MenuEntry *entry);
    void DisableCatalogSearchFunction(MenuEntry *entry);
    void FixInvalidPickupCrash(MenuEntry *entry);
    void FixInvalidDropPlantCrash(MenuEntry *entry);
    void FixInvalidSpriteCrash(MenuEntry *entry);
    void FixInvalidGiveItemCrash(MenuEntry *entry);
    void FixInvalidHoleCrash(MenuEntry *entry);
    void FixInvalidItemCrash(MenuEntry *entry);
    void ConvertFlowerFromSeedItemToNormalItem(MenuEntry *entry);
    void SetSeedItemNames(MenuEntry *entry);
    void SetItemReplacementRules(MenuEntry *entry);
    void SetDropRules(MenuEntry *entry);
    void SetPlantRules(MenuEntry *entry);
    void FixParticlesInPuzzleLeague(MenuEntry *entry);
    void SetCustomSpritesForProDesigns(MenuEntry *entry);
	void FruitStackingCheck(MenuEntry *entry);

	void pluginSettingsEntry(MenuEntry *entry);
	
//Helper functions
  //Located in cheats.cpp
	void SendPlayerData(Time time);
}