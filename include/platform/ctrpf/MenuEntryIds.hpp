#pragma once

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	namespace MenuEntryId {
		// ID policy:
		// - IDs are append-only. Never renumber existing IDs.
		// - Menu order is independent from ID order.
		// - For new entries, just append a new ID at the end of the relevant group.
		// - Keep old IDs reserved when removing entries.
		constexpr u32 GroupSave = 0x01;
		constexpr u32 GroupMovement = 0x02;
		constexpr u32 GroupInventory = 0x03;
		constexpr u32 GroupPlayerSave = 0x04;
		constexpr u32 GroupPlayer = 0x05;
		constexpr u32 GroupAnimation = 0x06;
		constexpr u32 GroupSeed = 0x07;
		constexpr u32 GroupDrop = 0x08;
		constexpr u32 GroupTree = 0x09;
		constexpr u32 GroupMoney = 0x0A;
		constexpr u32 GroupIsland = 0x0B;
		constexpr u32 GroupNpc = 0x0C;
		constexpr u32 GroupEnvFish = 0x0D;
		constexpr u32 GroupEnvInsect = 0x0E;
		constexpr u32 GroupEnvironment = 0x0F;
		constexpr u32 GroupChat = 0x10;
		constexpr u32 GroupFun = 0x11;
		constexpr u32 GroupExtra = 0x12;
		constexpr u32 GroupMisc = 0x13;
		constexpr u32 GroupDefault = 0x14;
		constexpr u32 GroupDev = 0x15;
		constexpr u32 GroupPlugin = 0x16;
		constexpr u32 GroupPretendo = 0x17;

		constexpr u32 Make(u32 group, u32 index) {
			return (group << 16) | (index & 0xFFFF);
		}

		// Save folder
		constexpr u32 SaveTownNameChanger = Make(GroupSave, 0x0001);
		constexpr u32 SaveBackup = Make(GroupSave, 0x0002);
		constexpr u32 SaveBullBoardDumper = Make(GroupSave, 0x0003);
		constexpr u32 SaveTreeSizeChanger = Make(GroupSave, 0x0004);
		constexpr u32 SaveChangeNativeFruit = Make(GroupSave, 0x0005);
		constexpr u32 SavePwpUnlock = Make(GroupSave, 0x0006);
		constexpr u32 SaveGrassChanger = Make(GroupSave, 0x0007);
		constexpr u32 SaveCaravanSet = Make(GroupSave, 0x0008);
		constexpr u32 SaveCampingSet = Make(GroupSave, 0x0009);
		constexpr u32 SaveShopUpgrade = Make(GroupSave, 0x000A);
		constexpr u32 SaveHouseEditor = Make(GroupSave, 0x000B);
		constexpr u32 SaveBuildingMod = Make(GroupSave, 0x000C);
		constexpr u32 SaveFillMuseum = Make(GroupSave, 0x000D);
		constexpr u32 SaveMayorPermit = Make(GroupSave, 0x000E);
		constexpr u32 SaveRealTimeAcre = Make(GroupSave, 0x000F);

		// Movement folder
		constexpr u32 MovementCantPush = Make(GroupMovement, 0x0001);
		constexpr u32 MovementCoordMod = Make(GroupMovement, 0x0002);
		constexpr u32 MovementMoonJump = Make(GroupMovement, 0x0003);
		constexpr u32 MovementTouchWarp = Make(GroupMovement, 0x0004);
		constexpr u32 MovementWalkOver = Make(GroupMovement, 0x0005);
		constexpr u32 MovementChange = Make(GroupMovement, 0x0006);
		constexpr u32 MovementWalkParticleChange = Make(GroupMovement, 0x0007);
		constexpr u32 MovementPlayerTeleport = Make(GroupMovement, 0x0008);
		constexpr u32 MovementVisibilityMod = Make(GroupMovement, 0x0009);
		constexpr u32 MovementSpeedMod = Make(GroupMovement, 0x000A);
		constexpr u32 MovementRoomWarping = Make(GroupMovement, 0x000B);
		constexpr u32 MovementShovelKnockback = Make(GroupMovement, 0x000C);
		constexpr u32 MovementCstickCoordMod = Make(GroupMovement, 0x000D);

		// Inventory folder
		constexpr u32 InventoryText2Item = Make(GroupInventory, 0x0001);
		constexpr u32 InventoryDupeItems = Make(GroupInventory, 0x0002);
		constexpr u32 InventoryCatalogToPocket = Make(GroupInventory, 0x0003);
		constexpr u32 InventoryChatT2I = Make(GroupInventory, 0x0004);
		constexpr u32 InventoryClearInventory = Make(GroupInventory, 0x0005);
		constexpr u32 InventoryItemSettings = Make(GroupInventory, 0x0006);
		constexpr u32 InventorySaveMenuChanger = Make(GroupInventory, 0x0007);
		constexpr u32 InventoryGetSet = Make(GroupInventory, 0x0008);
		constexpr u32 InventoryCustomButton = Make(GroupInventory, 0x0009);
		constexpr u32 InventorySearchItem = Make(GroupInventory, 0x000A);
		constexpr u32 InventoryHideDesignOutfits = Make(GroupInventory, 0x000B);

		// PlayerSave folder
		constexpr u32 PlayerSaveNameChanger = Make(GroupPlayerSave, 0x0001);
		constexpr u32 PlayerSaveAppearance = Make(GroupPlayerSave, 0x0002);
		constexpr u32 PlayerSaveRandomPlayer = Make(GroupPlayerSave, 0x0003);
		constexpr u32 PlayerSaveBackup = Make(GroupPlayerSave, 0x0004);
		constexpr u32 PlayerSaveTPCMessage = Make(GroupPlayerSave, 0x0005);
		constexpr u32 PlayerSaveTPCSettings = Make(GroupPlayerSave, 0x0006);
		constexpr u32 PlayerSaveDesignDump = Make(GroupPlayerSave, 0x0007);
		constexpr u32 PlayerSaveEmotionList = Make(GroupPlayerSave, 0x0008);
		constexpr u32 PlayerSaveEncyList = Make(GroupPlayerSave, 0x0009);
		constexpr u32 PlayerSaveDreamCode = Make(GroupPlayerSave, 0x000A);
		constexpr u32 PlayerSaveDebugMenu = Make(GroupPlayerSave, 0x000B);
		constexpr u32 PlayerSaveSongList = Make(GroupPlayerSave, 0x000C);
		constexpr u32 PlayerSaveFillCatalog = Make(GroupPlayerSave, 0x000D);
		constexpr u32 PlayerSaveQRMachine = Make(GroupPlayerSave, 0x000E);

		// Player folder
		constexpr u32 PlayerInfo = Make(GroupPlayer, 0x0001);
		//constexpr u32 PlayerNeckPosition = Make(GroupPlayer, 0x0002); Removed!
		constexpr u32 PlayerCustomHairEye = Make(GroupPlayer, 0x0003);
		constexpr u32 PlayerWearHelmet = Make(GroupPlayer, 0x0004);
		constexpr u32 PlayerFaint = Make(GroupPlayer, 0x0005);
		constexpr u32 PlayerFishThrower = Make(GroupPlayer, 0x0006);
		constexpr u32 PlayerOnMap = Make(GroupPlayer, 0x0007);
		constexpr u32 PlayerNeverBedhead = Make(GroupPlayer, 0x0008);

		// Animation folder
		constexpr u32 AnimationPlayerSelector = Make(GroupAnimation, 0x0001);
		constexpr u32 AnimationAntiAnim = Make(GroupAnimation, 0x0002);
		constexpr u32 AnimationMod = Make(GroupAnimation, 0x0003);
		constexpr u32 AnimationEmotionLoop = Make(GroupAnimation, 0x0004);
		constexpr u32 AnimationIdle = Make(GroupAnimation, 0x0005);
		//constexpr u32 AnimationSlowMo = Make(GroupAnimation, 0x0006); Removed!
		constexpr u32 AnimationOnAll = Make(GroupAnimation, 0x0007);

		// Seed subfolder
		constexpr u32 SeedPickSeeder = Make(GroupSeed, 0x0001);
		constexpr u32 SeedWalkSeeder = Make(GroupSeed, 0x0002);
		constexpr u32 SeedFireworkSeeder = Make(GroupSeed, 0x0003);
		constexpr u32 SeedMapEditor = Make(GroupSeed, 0x0004);
		constexpr u32 SeedAllSeeder = Make(GroupSeed, 0x0005);

		// Drop subfolder
		constexpr u32 DropItemSequencer = Make(GroupDrop, 0x0001);
		constexpr u32 DropMods = Make(GroupDrop, 0x0002);
		constexpr u32 DropItems = Make(GroupDrop, 0x0003);
		constexpr u32 DropAutoDrop = Make(GroupDrop, 0x0004);
		constexpr u32 DropTouchDrop = Make(GroupDrop, 0x0005);
		constexpr u32 DropSlotDrop = Make(GroupDrop, 0x0006);

		// Tree subfolder
		constexpr u32 TreeInfFruit = Make(GroupTree, 0x0001);
		constexpr u32 TreeAxeShake = Make(GroupTree, 0x0002);
		constexpr u32 TreeFruitMod = Make(GroupTree, 0x0003);
		constexpr u32 TreeInstChop = Make(GroupTree, 0x0004);

		// Money folder
		constexpr u32 MoneyWallet = Make(GroupMoney, 0x0001);
		constexpr u32 MoneyBank = Make(GroupMoney, 0x0002);
		constexpr u32 MoneyMeowCoupons = Make(GroupMoney, 0x0003);
		constexpr u32 MoneyBadges = Make(GroupMoney, 0x0004);
		constexpr u32 MoneyMedals = Make(GroupMoney, 0x0005);
		constexpr u32 MoneyTurnips = Make(GroupMoney, 0x0006);

		// Island folder
		constexpr u32 IslandUnlock = Make(GroupIsland, 0x0001);
		constexpr u32 IslandFillInvOre = Make(GroupIsland, 0x0002);
		constexpr u32 IslandFillInvFruit = Make(GroupIsland, 0x0003);
		constexpr u32 IslandFreeKappn = Make(GroupIsland, 0x0004);
		constexpr u32 IslandShopMod = Make(GroupIsland, 0x0005);
		constexpr u32 IslandAllTours = Make(GroupIsland, 0x0006);
		constexpr u32 IslandAcre = Make(GroupIsland, 0x0007);
		constexpr u32 IslandBuilding = Make(GroupIsland, 0x0008);
		constexpr u32 IslandSaver = Make(GroupIsland, 0x0009);
		constexpr u32 IslandOnlineOptions = Make(GroupIsland, 0x000A);

		// NPC folder
		constexpr u32 NpcSelector = Make(GroupNpc, 0x0001);
		constexpr u32 NpcAnimation = Make(GroupNpc, 0x0002);
		constexpr u32 NpcCoordinate = Make(GroupNpc, 0x0003);
		constexpr u32 NpcTeleport = Make(GroupNpc, 0x0004);
		constexpr u32 NpcRotation = Make(GroupNpc, 0x0005);

		// Environment subfolders
		constexpr u32 EnvFishAlwaysBite = Make(GroupEnvFish, 0x0001);
		constexpr u32 EnvFishCantScare = Make(GroupEnvFish, 0x0002);
		constexpr u32 EnvInsectSpawn = Make(GroupEnvInsect, 0x0001);
		constexpr u32 EnvInsectCantScare = Make(GroupEnvInsect, 0x0002);

		// Environment folder
		constexpr u32 EnvironmentDaytime = Make(GroupEnvironment, 0x0001);
		constexpr u32 EnvironmentAlwaysAurora = Make(GroupEnvironment, 0x0002);
		constexpr u32 EnvironmentUnbreakFlower = Make(GroupEnvironment, 0x0003);
		constexpr u32 EnvironmentWeatherMod = Make(GroupEnvironment, 0x0004);
		constexpr u32 EnvironmentWaterFlowers = Make(GroupEnvironment, 0x0005);
		constexpr u32 EnvironmentWeedRemover = Make(GroupEnvironment, 0x0006);
		constexpr u32 EnvironmentGrassEditor = Make(GroupEnvironment, 0x0007);
		constexpr u32 EnvironmentKeepGrassState = Make(GroupEnvironment, 0x0008);
		constexpr u32 EnvironmentBuriedInspector = Make(GroupEnvironment, 0x0009);
		constexpr u32 EnvironmentItemsDontDissapear = Make(GroupEnvironment, 0x000A);
		constexpr u32 EnvironmentSetFoliageSeason = Make(GroupEnvironment, 0x000B);

		// Chat subfolder
		constexpr u32 ChatDontDissapear = Make(GroupChat, 0x0001);
		//constexpr u32 ChatCopyPaste = Make(GroupChat, 0x0002); Removed!
		constexpr u32 ChatForce = Make(GroupChat, 0x0003);
		constexpr u32 ChatCommands = Make(GroupChat, 0x0004);
		constexpr u32 ChatButton = Make(GroupChat, 0x0005);
		constexpr u32 ChatTranslationOptions = Make(GroupChat, 0x0006);
		constexpr u32 ChatKeyExtend = Make(GroupChat, 0x0007);
		constexpr u32 ChatCustomKey = Make(GroupChat, 0x0008);
		constexpr u32 ChatMoreNumbers = Make(GroupChat, 0x0009);

		// Fun subfolder
		constexpr u32 FunSizeCodes = Make(GroupFun, 0x0001);
		constexpr u32 FunTPose = Make(GroupFun, 0x0002);
		constexpr u32 FunTakeTpcPic = Make(GroupFun, 0x0003);
		constexpr u32 FunMaxTurbo = Make(GroupFun, 0x0004);
		constexpr u32 FunMultiPress = Make(GroupFun, 0x0005);
		constexpr u32 FunUltimatePopper = Make(GroupFun, 0x0006);
		constexpr u32 FunCameraMod = Make(GroupFun, 0x0007);

		// Extra folder
		constexpr u32 ExtraShopsAlwaysOpen = Make(GroupExtra, 0x0001);
		constexpr u32 ExtraDisableSave = Make(GroupExtra, 0x0002);
		constexpr u32 ExtraDisableItemLocks = Make(GroupExtra, 0x0003);
		constexpr u32 ExtraCantFallHole = Make(GroupExtra, 0x0004);
		constexpr u32 ExtraSetSpotState = Make(GroupExtra, 0x0005);
		constexpr u32 ExtraSearchReplace = Make(GroupExtra, 0x0006);
		constexpr u32 ExtraRemoveMapItems = Make(GroupExtra, 0x0007);
		constexpr u32 ExtraEditPattern = Make(GroupExtra, 0x0008);
		constexpr u32 ExtraAmiiboSpoofer = Make(GroupExtra, 0x0009);
		constexpr u32 ExtraTimeTravel = Make(GroupExtra, 0x000A);

		// Misc folder
		constexpr u32 MiscToolAnim = Make(GroupMisc, 0x0001);
		constexpr u32 MiscGameType = Make(GroupMisc, 0x0002);
		constexpr u32 MiscRadioPlayer = Make(GroupMisc, 0x0003);
		constexpr u32 MiscReloadRoom = Make(GroupMisc, 0x0004);
		constexpr u32 MiscLargeFov = Make(GroupMisc, 0x0005);
		constexpr u32 MiscMoveFurn = Make(GroupMisc, 0x0006);
		constexpr u32 MiscWalkTalk = Make(GroupMisc, 0x0007);
		constexpr u32 MiscBeansParticle = Make(GroupMisc, 0x0008);
		constexpr u32 MiscFastMode = Make(GroupMisc, 0x0009);
		constexpr u32 MiscFastTextSpeed = Make(GroupMisc, 0x000A);
		constexpr u32 MiscFastGameSpeed = Make(GroupMisc, 0x000B);
		constexpr u32 MiscFastIsabelle = Make(GroupMisc, 0x000C);
		constexpr u32 MiscKeepConnection = Make(GroupMisc, 0x000D);

		// Default folder
		constexpr u32 DefaultSeedItemLegit = Make(GroupDefault, 0x0001);
		constexpr u32 DefaultDropLagRemove = Make(GroupDefault, 0x0002);
		constexpr u32 DefaultChangeRockbreakParticle = Make(GroupDefault, 0x0003);
		constexpr u32 DefaultDropItemsEverywhere = Make(GroupDefault, 0x0004);
		constexpr u32 DefaultIdleAfterTreeShakeOrCut = Make(GroupDefault, 0x0005);
		constexpr u32 DefaultReplaceDropFunctions = Make(GroupDefault, 0x0006);
		constexpr u32 DefaultPreventParticleCrash = Make(GroupDefault, 0x0007);
		constexpr u32 DefaultBypassGameChecks = Make(GroupDefault, 0x0008);
		constexpr u32 DefaultDisableNonSeedItemCheck = Make(GroupDefault, 0x0009);
		constexpr u32 DefaultPatchDropFunction = Make(GroupDefault, 0x000A);
		constexpr u32 DefaultCheckInvalidBadge = Make(GroupDefault, 0x000B);
		constexpr u32 DefaultDisableOpenSaveMenuWithStart = Make(GroupDefault, 0x000C);
		constexpr u32 DefaultDisableCatalogSearch = Make(GroupDefault, 0x000D);
		constexpr u32 DefaultFixInvalidPickupCrash = Make(GroupDefault, 0x000E);
		constexpr u32 DefaultFixInvalidDropPlantCrash = Make(GroupDefault, 0x000F);
		constexpr u32 DefaultFixInvalidSpriteCrash = Make(GroupDefault, 0x0010);
		constexpr u32 DefaultFixInvalidGiveItemCrash = Make(GroupDefault, 0x0011);
		constexpr u32 DefaultFixInvalidHoleCrash = Make(GroupDefault, 0x0012);
		constexpr u32 DefaultFixInvalidItemCrash = Make(GroupDefault, 0x0013);
		constexpr u32 DefaultConvertFlowerSeedToNormal = Make(GroupDefault, 0x0014);
		constexpr u32 DefaultSetSeedItemNames = Make(GroupDefault, 0x0015);
		constexpr u32 DefaultSetItemReplacementRules = Make(GroupDefault, 0x0016);
		constexpr u32 DefaultSetDropRules = Make(GroupDefault, 0x0017);
		constexpr u32 DefaultSetPlantRules = Make(GroupDefault, 0x0018);
		constexpr u32 DefaultFixParticlesPuzzleLeague = Make(GroupDefault, 0x0019);
		constexpr u32 DefaultSetCustomSpritesProDesigns = Make(GroupDefault, 0x001A);
		constexpr u32 DefaultFixPretendoOnlineIslandSession = Make(GroupDefault, 0x001B);
		constexpr u32 DefaultFixPretendoFindSessionByOwnerCall = Make(GroupDefault, 0x001C);

		// Dev folder
		constexpr u32 DevCustomDump = Make(GroupDev, 0x0001);
		constexpr u32 DevCallFunction = Make(GroupDev, 0x0002);
		constexpr u32 DevDisplaySaveFlagDifference = Make(GroupDev, 0x0003);
		constexpr u32 DevUnusedFallDown = Make(GroupDev, 0x0004);
		constexpr u32 DevUnlockCroRegion = Make(GroupDev, 0x0005);
		constexpr u32 DevAcnhCheat = Make(GroupDev, 0x0006);
		constexpr u32 DevPlayerDumper = Make(GroupDev, 0x0007);
		constexpr u32 DevWandAbility = Make(GroupDev, 0x0008);
		constexpr u32 DevLightSwitchToggler = Make(GroupDev, 0x0009);
		constexpr u32 DevExpressionMod = Make(GroupDev, 0x000A);
		constexpr u32 DevPrivateIsland = Make(GroupDev, 0x000B);

		// Plugin entries
		constexpr u32 PluginSettings = Make(GroupPlugin, 0x0001);

		// Pretendo folder
		constexpr u32 PretendoPlayersInSession = Make(GroupPretendo, 0x0001);
		constexpr u32 PretendoPIALoggerOptions = Make(GroupPretendo, 0x0002);
	}
}
