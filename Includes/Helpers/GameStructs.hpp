#pragma once

#include <CTRPluginFramework.hpp>
#include "Item/Item.hpp"

#pragma pack(push, 1)

typedef char16_t wchar; ///< Wide character type

namespace CTRPluginFramework {

    enum class PlayerStatus : u8 {
        Town_00 = 0,
        Town_01 = 1,
        Town_02 = 2,
        Town_03 = 3,
        Isl_00 = 4,
        Isl_01 = 5,
        Isl_02 = 6,
        Isl_03 = 7,
        Empty = 8
    };

    enum Item_Category : u32 {
        MiiHead = 0, //(0x2000)
        NPCBuildingItems, //(0x2061 -> 0x2074)
        Bells, //(0x20AC -> 0x2117)
        Wallpaper, //(0x234C -> 0x23EA)
        Carpets, //(0x23EB -> 0x2492)
        Furniture, //(0x29DF -> 0x30CB)
        Shirts, //(0x2495 -> 0x2681)
        Dresses, //(0x2682 -> 0x26EC)
        Wetsuits, //(0x26ED -> 0x26F5)
        Trousers, //(0x26F8 -> 0x2776)
        Socks, //(0x2777 -> 0x279E)
        Shoes, //(0x279F -> 0x27E5)
        Hats, //(0x280B -> 0x28F4)
        Accesories, //flower accesories not included(0x28F5 -> 0x292E)
        Axes, //(0x334C -> 0x334F)
        Nets, //(0x3350 -> 0x3353)
        Rods, //(0x3354 -> 0x3357)
        Shovels, //(0x3358 -> 0x335B)
        Slingshots, //(0x3360 -> 0x3363)
        Cans, //(0x335C -> 0x335F)
        ToyHammer, //(0x3364 -> 0x3365)
        Megaphone, //(0x3366)
        Timer, //(0x3367)
        HandheldFireworks, //(0x3368 -> 0x3369) 
        FountainFirework, //(0x339F)
        Umbrellas, //(0x27E6 -> 0x280A) 
        PartyPopper, //(0x336A)
        BubbleWands, //(0x336B)
        Balloons, //(0x336C -> 0x3383) 
        Pinwheels, //(0x3384 -> 0x338B)
        Beans, //(0x338E)
        Good_LuckRoll, //(0x338F)
        Soft_Serves, //(0x3390 -> 0x3393)
        IceCream, //(0x3394 -> 0x3397)
        Wands, //(0x3398 -> 0x339D)
        Tweeter, //(0x339E)
        Bugs, //(0x228E -> 0x22D5)
        Feathers, //(0x22D7 -> 0x22DE)
        Snowflake, //(0x22D6)
        Fish, //(0x22E1 -> 0x2328)
        SeaCreatures, //(0x232D -> 0x234A)
        MailPapers, //(0x223F -> 0x2281)
        MelodyPaper, //(0x2282)
        Fossil, //(0x202A)
        AnalyzedFossils, //(0x3130 -> 0x3172)
        FossilModels, //(0x3173 -> 0x3186)
        Gyroids, //(0x295C -> 0x29DE)
        Songs, //(0x212B -> 0x2185)
        Unknown1, //(0x2186 -> 0x2188)
        SongsWall, //(0x2189 -> 0x21E3)
        SongsMusicBox, //(0x21E4 -> 0x223E)
        Fruits, //(0x2001 -> 0x200D)
        PerfectFruits, //(0x200E -> 0x2012)
        RottenFruits, //(0x2013 -> 0x2017)
        FlowerBags, //(0x2038 -> 0x2060)
        TreeSaplings, //(0x202C -> 0x202D)
        BushShoots, //(0x202E -> 0x2035)
        Turnips, //(0x2283 -> 0x228C)
        SpoiledTurnips, //(0x228D)
        Sold_OutSigns1, //Nooklings and Gracie(0x2083 -> 0x2084)
        Seashells, //(0x208C -> 0x2095)
        Garbage, //(0x2329 -> 0x232B)
        Mushrooms, //(0x2098 -> 0x209D)
        MushroomFurniture, //(0x211E -> 0x212A)
        FlowerAccesories, //(0x292F -> 0x295B)
        PitfallSeeds, //(0x209F -> 0x20A0)
        SpecialOres, //gold and silver nugget(0x20A1 -> 0x20A2)
        Ores, //(0x20A3 -> 0x20A6)
        Ingredients, //butter, milk, etc(0x20A7 -> 0x20AB)
        Beehive, //(0x209E)
        Fertilizer, //(0x202B)
        ProPatternShirt, //(0x33A7)
        PatternItems, //like knit hat, horned hat, etc(0x33A8 -> 0x33BB)
        WrappingPaper, //(0x2089)
        Medicine, //(0x208A)
        SignatureSheet, //(0x33BC)
        PetitionForm, //(0x33BD)
        PaperScrap, //(0x33BE)
        UnknownLostItem, //lost item not checked yet(0x33BF -> 0x33C2)
        KnownLostItem, //checked lost item(0x33C3 -> 0x33C6)
        SantaBag, //(0x33A1)
        MysteryBag, //(0x33A2)
        Candy, //(0x2096)
        Lollipop, //(0x2097)
        RealPaintings, //(0x30D2 -> 0x30EA)
        FakePaintings, //(0x30F3 -> 0x3100)
        RealPaintingsWall, //(0x3109 -> 0x3121)
        FakePaintingsWall, //(0x3122 -> 0x312F)
        RealStatues, //(0x30EB -> 0x30F2)
        FakeStatues, //(0x3101 -> 0x3108)
        FortuneCookie, //(0x33C8)
        FortuneCookieSlips, //(0x33C9 -> 0x3401)
        BadgesSprite, //(0x340C -> 0x340E)
        HouseMailBoxs, //(0x3426 -> 0x343B)
        HouseArchedDoors, //(0x343C -> 0x3451)
        HouseDoors, //(0x3452 -> 0x3466)
        HouseRoofs, //(0x3467 -> 0x3489)
        HouseFences, //(0x348A -> 0x34A0)
        HouseExteriors, //(0x34A1 -> 0x34C0)
        HousePavements, //(0x34C1 -> 0x34C9)
        HouseArchitectures, //(0x34CA -> 0x34CD)
        WaterEgg, //(0x232C)
        Deep_SeaEgg, //(0x234B)
        EarthEgg, //(0x2118)
        TreeEgg, //(0x2119)
        StoneEgg, //(0x211A)
        SkyEgg, //(0x211B)
        WinningTicket, //(0x211C)
        Grand_PrizeTicket, //(0x211D)
        Present, //(0x2036)
        Map, //(0x340F)
        Trophies, //(0x3417 -> 0x3419)
        FireworkUsed, //(0x33A0)
        CoffeeBeans, //(0x33A3 -> 0x33A5)
        ChocolateCoin, //(0x33A6)
        Coffee, //(0x338C)
        FizzyAppleJuice, //(0x338D)
        FruitBaskets, //(0x2018 -> 0x2024)
        PerfectFruitBaskets, //(0x2025 -> 0x2029)
        MovingBoxes, //(0x207E -> 0x2080)
        BingoCard, //(0x341A)
        BingoCardExpired, //(0x341B)
        TimeCapsule, //(0x341C)
        Mannequins, //(0x30CC -> 0x30CF)
        Sold_OutSigns2, //Retail(0x2085)
        TPC, //(0x3410)
        Lockers, //Train Station and Museum(0x2081 -> 0x2082)
        Chairs, //Retail, Island Hut and Harvey(0x207B -> 0x207D)
        FlowerSprites, //(0x341F -> 0x3425)
        Towel, //(0x33C7)
        Vests, //Male and Female(0x2493 -> 0x2494)
        Shorts, //Male and Female (0x26F6 -> 0x26F7)
        Photos, //(0x3187 -> 0x334B)
        Sold_OutSigns1Wall, //Nooklings and Gracie(0x2086 -> 0x2087)
        LostBook, //(0x3411)
        ChocolateHeart, //(0x208B)
        Scavenger_HuntList, //(0x341D)
        Mail, //(0x3412)
        VillagerPresent, //(0x2037)
        Sold_OutSigns3, //Redd(0x2088)
        ShopItemHangups, //1x1 and 1x2(0x2079 -> 0x207A)
        ReddsCookie, //(0x3402)
        Tickets, //(0x3403 -> 0x340B)
        MuseumItems, //Podium, Stands and Wallpaper(0x2075 -> 0x2078)
        ClubTortimerForm, //(0x341E)
        GracieMannequins, //(0x30D0 -> 0x30D1)
        Seeds, //Flower seed, sapling(0x3413 -> 0x3416)
        ShirtsWall, //(0x34CE -> 0x36BA)
        DressesWall, //(0x36BB -> 0x3725)
        MagicLamp, //(0x3726)
        RetailGiantItem, //giant item sign and box(0x3727 -> 0x3728)
        FryingPan, //(0x3729)
        Good_LuckCharm, //(0x22DF)
        IkadaTrophy, //(0x372A)
        Unknown2, //(0x22E0)
        Invalid //Game internally actually has this for many checks
    };

    /*All Credits go to https://github.com/Slattz/ACNL_Research/blob/master/010%20Templates/garden_plus.dat.bt*/

    struct Emoticons {
        u8 emoticons[40];
    };

    struct Mannequin {
        Item Hat; //Item ID < 0xXXXX
        Item Accessory; //Item ID < 0xXXXX
        Item TopWear; //Item ID < 0xXXXX
        Item BottomWear; //Item ID < 0xXXXX
        Item Socks; //Item ID < 0xXXXX
        Item Shoes; //Item ID < 0xXXXX
    };

    struct TownID { //0x16
        u16 TID; //Default is 0
        wchar DataTownName[9]; //Default is 0
        u8 Unknown01; //Default is 0xA
        u8 Unknown02;

        bool operator==(const TownID& townID) const {
            return TID == townID.TID && 
                    std::equal(std::begin(DataTownName), std::end(DataTownName), std::begin(townID.DataTownName)) && 
                    Unknown01 == townID.Unknown01 && 
                    Unknown02 == townID.Unknown02;
        }

        std::string GetName() const {
            std::string out;
            Utils::ConvertUTF16ToUTF8(
                out,
                DataTownName
            );
            return out;
        }
    };

    struct PlayerID { //0x16
        u16 PID;
        wchar PlayerName[9];
        u8 Gender;
        u8 ZeroPad;

        bool operator==(const PlayerID& playerID) const {
            return PID == playerID.PID && 
                    std::equal(std::begin(PlayerName), std::end(PlayerName), std::begin(playerID.PlayerName)) && 
                    Gender == playerID.Gender && 
                    ZeroPad == playerID.ZeroPad;
        }

        std::string GetName() const {
            std::string out;
            Utils::ConvertUTF16ToUTF8(
                out,
                PlayerName
            );
            return out;
        }
    };

    struct PersonalID { //Size: 0x2E
        PlayerID PlayerData;
        TownID TownData;
        u8 TPC_Country;
        u8 TPC_County;

        bool operator==(const PersonalID& personalID) const {
            return PlayerData == personalID.PlayerData && 
                    TownData == personalID.TownData && 
                    TPC_Country == personalID.TPC_Country && 
                    TPC_County == personalID.TPC_County;
        }
    };

    struct ACNL_Pattern { //Size: 0x870
        wchar Title[21];
        PersonalID CreatorData;
        u8 Palette[15];
        u8 UnusedChecksum = 1; //changing seems to have no effect //Default: 1, Set at 0x1B4F74 EUR 1.5
        u8 TenConstant; //seems to always be 0x0A
        u8 PatternType;
        u16 ZeroPad_2; //Zero Padding; Always 0x0000
        u8 PatternData1[512]; //mandatory

    //only used when PatternType < 0x09
        u8 PatternData2[512]; //optional
        u8 PatternData3[512]; //optional
        u8 PatternData4[512]; //optional
        u32 ZeroPad_3; //Zero Padding; Optional*/
    };

    /*struct BDGValues {
        u64 Badge_Fishes_Value; //Encrypted Value
        u64 Badge_Bugs_Value; //Encrypted Value
        u64 Badge_Marine_Value; //Encrypted Value
        u64 Badge_FishColl_Value; //Encrypted Value
        u64 Badge_BugColl_Value; //Encrypted Value
        u64 Badge_MarineColl_Value; //Encrypted Value
        u64 Badge_Balloons_Value; //Encrypted Value
        u64 Badge_Visiting_Value; //Encrypted Value
        u64 Badge_Hosting_Value; //Encrypted Value
        u64 Badge_Gardening_Value; //Encrypted Value
        u64 Badge_Bank_Value; //Encrypted Value
        u64 Badge_Turnips_Value; //Encrypted Value
        u64 Badge_Medals_Value; //Encrypted Value
        u64 Badge_Streetpass_Value; //Encrypted Value
        u64 Badge_Weeding_Valu; //Encrypted Value
        u64 Badge_Shopping_Value; //Encrypted Value
        u64 Badge_LetterWriter_Value; //Encrypted Value
        u64 Badge_Refurbish_Value; //Encrypted Value
        u64 Badge_Catalog_Value; //Encrypted Value
        u64 Badge_KK_Value; //Encrypted Value
        u64 Badge_HHAScore_Value; //Encrypted Value
        u64 Badge_PlayTime_Value; //Encrypted Value
        u64 Badge_Helper_Value; //Encrypted Value
        u64 Badge_Dream_Value; //Encrypted Value      
    };

    struct BDG {
        u8 Badge_Fishes;
        u8 Badge_Bugs;
        u8 Badge_Marine;
        u8 Badge_FishColl;
        u8 Badge_BugColl;
        u8 Badge_MarineColl;
        u8 Badge_Balloons;
        u8 Badge_Visiting;
        u8 Badge_Hosting;
        u8 Badge_Gardening;
        u8 Badge_Bank;
        u8 Badge_Turnips;
        u8 Badge_Medals;
        u8 Badge_Streetpass;
        u8 Badge_Weeding;
        u8 Badge_Shopping;
        u8 Badge_LetterWriter;
        u8 Badge_Refurbish;
        u8 Badge_Catalog;
        u8 Badge_KK;
        u8 Badge_HHAScore;
        u8 Badge_PlayTime;
        u8 Badge_Helper;

        u8 Badge_Dream;   
    };*/

    struct PlayerBadges {
        u64 BadgeValues[24]; //0x567C: 24 badges (encrypted values)
        u8 Badges[24]; //0x573C: 24 badges
        u64 Unknown1; //0x5754 -> 0x575B: Encrypted Value
        u64 Unknown1_1; //0x575C -> 0x5763: Encrypted Value
    };

    struct HHAHouseInfo {
        s32 HHAHousePoints; //0x5764 -> 0x5767
        u16 HHAItem1; //0x5768 -> 0x5777 //Only used if no theme
        u16 HHAItem2; //0x5768 -> 0x5777 //Only used if no theme
        u16 HHAItem3; //0x5768 -> 0x5777 //Only used if no theme
        u16 HHAItem4; //0x5768 -> 0x5777 //Only used if no theme
        u16 HHAItem5; //0x5768 -> 0x5777 //Only used if no theme
        u16 Item_Exterior; //0x5768 -> 0x5777 //Only used if you have a theme
        u16 Item_Interior; //0x5768 -> 0x5777 //Only used if you have a theme
        u16 HHAItem7; //0x5768 -> 0x5777 //Only used if you have a theme
        u8 CurrentHouseTheme; //0x5778 -> 0x5778
        u8 EvaluationType; //0x5779 -> 0x5779
        u8 HouseUnk0; //0x577A -> 0x577A //Valid values: 0, 1, 2, 3
        u8 HouseUnk1; //0x577B -> 0x577B
        u8 HouseUnk2; //0x577C -> 0x577C //Read if HouseUnk0==3
        u8 HouseUnk3; //0x577D -> 0x577D
        u8 HouseUnk4; //0x577E -> 0x577E
        u8 HouseUnk5; //0x577F -> 0x577F
        u8 HouseUnk6; //0x5780 -> 0x5780
        u8 HouseUnk7; //0x5781 -> 0x5781
        u8 HouseUnk8; //0x577E -> 0x577E
        u8 HouseUnk9; //0x577E -> 0x577E
        u8 HouseUnk10; //0x577E -> 0x577E
        u8 HouseExteriorObeyingTheme; //0x577E -> 0x577E //Not Verified; 5 is max, 0 = Not obeying(?); higher better(?)
        u8 HouseInteriorObeyingTheme; //0x577E -> 0x577E //Not Verified; 5 is max, 0 = Not obeying(?); higher better(?)
        u8 WhichFloorWasImpressive; //0x577E -> 0x577E //Not Verified; 5 is max
        u8 HouseUnk14; //0x577E -> 0x577E
        u8 HouseUnk15; //0x577E -> 0x577E
        u8 FutureAdvice; //0x577E -> 0x577E //Values: 0 - 10
        u8 HHAAwardsUnlocked; //0x577E -> 0x577E //Not Verified; 8 is max
        u8 HHAAwardsRecieved; //0x577E -> 0x577E //Not Verified; 8 is max; 0 = None
        u8 GoldExteriorsUnlocked; //0x577E -> 0x577E //5 is max
        u8 GoldExteriorsApplied; //0x577E -> 0x577E //Not Verified; 5 is max
        u8 HouseUnk21; //0x577E -> 0x577E
    };

    struct ACNL_DreamAddress {
        u32 DCPart1;
        u32 DCPart2; //Code checks it's less than 1, aka 0
        bool HasDreamAddress;
        u8 DCPart3;
        u16 Padding;
    };

    //0x57A0 -> 0x57D3
    struct Player_Flags {
        u8 Unknown1 : 1;
        u8 Unknown2 : 1;
        u8 Unknown3 : 1;
        u8 Unknown4 : 1;
        u8 PlayerSetNameAndTownName : 1; //most likely player set their name and townname
        u8 ReddIntroduced : 1;
        u8 Unknown7 : 1;
        u8 Unknown8 : 1;
        u8 Unknown9 : 1;
        u8 FinishedFirstDay : 1; //Introduction day finished
        u8 Unknown11 : 1; //Meet Tommy or Timmy in shop for first time
        u8 Unknown12 : 1;
        u8 Unknown13 : 1;
        u8 Unknown14 : 1;
        u8 Unknown15 : 1;
        u8 Unknown16 : 1;
        u8 Unknown17 : 1;
        u8 Unknown18 : 1;
        u8 Unknown19 : 1;
        u8 Unknown20 : 1;
        u8 Unknown21 : 1;
        u8 Unknown22 : 1;
        u8 Unknown23 : 1;
        u8 Unknown24 : 1;
        u8 Unknown25 : 1;
        u8 Unknown26 : 1;
        u8 Unknown27 : 1;
        u8 TomNookIntroduced : 1;
        u8 BlathersIntroduced : 1; //somehow also set tom nook introduced to 1???
        u8 Unknown30 : 1;
        u8 CelesteIntroduced : 1;
        u8 Unknown32 : 1;
        u8 Unknown33 : 1;
        u8 Unknown34 : 1; //Reese and Tommy conversation (ReTail)
        u8 Unknown35 : 1;
        u8 Unknown36 : 1;
        u8 TomNookDownFirstPayment : 1;
        u8 ChooseHouseLocation : 1; //Tom nook first time meeting -> finding place to build house
        u8 MainStreetUnlocked : 1; //go to tom nook after talking to isabelle in town hall at start of game
        u8 HouseLocationChosen : 1; //Build House (Tom Nook placed down tent)
        u8 KnowIsabelleName : 1;
        u8 KnowKappn : 1;
        u8 Unknown43 : 1; //after repaying loan in post office
        u8 Unknown44 : 1; //set birthday in town hall / received TPC
        u8 Unknown45 : 1;
        u8 Unknown46 : 1;
        u8 Unknown47 : 1;
        u8 Unknown48 : 1;
        u8 Unknown49 : 1; //Met Resetti for the first time
        u8 Unknown50 : 1;
        u8 Unknown51 : 1;
        u8 Unknown52 : 1;
        u8 Unknown53 : 1;
        u8 Unknown54 : 1;
        u8 Unknown55 : 1;
        u8 Unknown56 : 1;
        u8 Unknown57 : 1;
        u8 Unknown58 : 1;
        u8 Unknown59 : 1;
        u8 Unknown60 : 1;
        u8 Unknown61 : 1; //Meet Tommy or Timmy in shop for first time
        u8 Unknown62 : 1;
        u8 Unknown63 : 1;
        u8 Unknown64 : 1;
        u8 Unknown65 : 1;
        u8 Unknown66 : 1;
        u8 Unknown67 : 1;
        u8 Unknown68 : 1; //Porter Introduction
        u8 Unknown69 : 1; //i think got map from isabelle at start of game
        u8 Unknown70 : 1;
        u8 Unknown71 : 1;
        u8 Unknown72 : 1;
        u8 Unknown73 : 1;
        u8 Unknown74 : 1;
        u8 Unknown75 : 1;
        u8 Unknown76 : 1;
        u8 Unknown77 : 1;
        u8 Unknown78 : 1;
        u8 Unknown79 : 1;
        u8 Unknown80 : 1;
        u8 Unknown81 : 1;
        u8 ResetPending : 1;
        u8 Unknown83 : 1; //Villager related
        u8 Unknown84 : 1; //Villager related
        u8 Unknown85 : 1; //Villager related
        u8 Unknown86 : 1; 
        u8 Unknown87 : 1; //isabelle welcomed you in your tent (actually also gets set when placing any item in house for first time, if never been in tent)
        u8 PeteIntroduction : 1;
        u8 Unknown89 : 1; //Reese and Tommy conversation (ReTail)
        u8 Unknown90 : 1;
        u8 Unknown91 : 1;
        u8 ExteriorRenovationsUnlocked : 1; //If house is max size
        u8 HouseLoanRepayed : 1; //after repaying loan in post office = 1 | //Talking to tom nook to let him know you payed your loan = 0
        u8 Unknown94 : 1;
        u8 Unknown95 : 1; //Gets set when you talk to tom nook after your house was built = 0
        u8 HouseUpgradeFinished : 1; //next day when house upgrade is finished = 1 | when you talk to tom nook after your house was built = 0
        u8 Unknown97 : 1;
        u8 Unknown98 : 1; 
        u8 Unknown99 : 1;
        u8 Unknown100 : 1;
        u8 Unknown101 : 1;
        u8 MuseumExhibitExplained : 1;
        u8 Unknown103 : 1;
        u8 Unknown104 : 1;
        u8 PermitApprovalArrived : 1; //day of arrival of permit
        u8 MayorJobIntroduction : 1; //after permit
        u8 PermitApproval : 1; //if approval arrived
        u8 PermitIntroduction : 1; //Isabelle told you about the mayor permit
        u8 CitizenSatisfactionExplained : 1;
        u8 TownFlagExplained : 1;
        u8 TownTuneExplained : 1;
        u8 PWPExplained : 1;
        u8 OrdinanceExplained : 1;
        u8 Unknown114 : 1;
        u8 Unknown115 : 1;
        u8 PermitFinished : 1;
        u8 Unknown117 : 1;
        u8 Unknown118 : 1;
        u8 HasBeeSting : 1;
        u8 Unknown120 : 1;
        u8 EditDesignIntro : 1;
        u8 Unknown122 : 1;
        u8 Unknown123 : 1;
        u8 CanTravel : 1; //i.e. can use train
        u8 HasTPCPicture : 1;
        u8 Unknown126 : 1; //after first time asking porter to travel with no TPC
        u8 Unknown127 : 1; //First time going in able sisters (Sable and mable)
        u8 Unknown128 : 1;
        u8 Unknown129 : 1;
        u8 Unknown130 : 1;
        u8 Unknown131 : 1;
        u8 Unknown132 : 1;
        u8 BefriendSable1 : 1;
        u8 Unknown134 : 1;
        u8 BefriendSable2 : 1;
        u8 Unknown136 : 1;
        u8 BefriendSable3 : 1;
        u8 Unknown138 : 1;
        u8 Unknown139 : 1;
        u8 Unknown140 : 1; //Gets set when you talk to tom nook after your house was built
        u8 Unknown141 : 1;
        u8 Unknown142 : 1;
        u8 Unknown143 : 1;
        u8 Unknown144 : 1; 
        u8 Unknown145 : 1;
        u8 Unknown146 : 1;
        u8 TalkToLyleForTheFirstTime : 1;
        u8 Unknown148 : 1;
        u8 Unknown149 : 1; //Gets set when you talk to tom nook after your house was built
        u8 Unknown150 : 1;
        u8 Unknown151 : 1;
        u8 UnlockedKappn : 1; //unlocked kappn boat
        u8 LyleIntroduction : 1;
        u8 Unknown154 : 1;
        u8 RecievedHHSIntro : 1;
        u8 Unknown156 : 1; //after repaying loan in post office
        u8 Unknown157 : 1;
        u8 Unknown158 : 1; //Talked to tortimer
        u8 Unknown159 : 1;
        u8 Unknown160 : 1;
        u8 Unknown161 : 1;
        u8 Unknown162 : 1;
        u8 FinishedShrunkSignatures : 1;
        u8 Unknown164 : 1;
        u8 Unknown165 : 1;
        u8 Unknown166 : 1;
        u8 Unknown167 : 1;
        u8 Unknown168 : 1;
        u8 Unknown169 : 1;
        u8 Unknown170 : 1;
        u8 Unknown171 : 1;
        u8 Unknown172 : 1;
        u8 IsabelleWallpaperGiftForGettingHouse : 1;
        u8 KickIntroduced : 1;
        u8 Unknown175 : 1;
        u8 Unknown176 : 1;
        u8 Unknown177 : 1;
        u8 Unknown178 : 1;
        u8 Unknown179 : 1;
        u8 Unknown180 : 1;
        u8 Unknown181 : 1;
        u8 Unknown182 : 1;
        u8 Unknown183 : 1;
        u8 Unknown184 : 1;
        u8 Unknown185 : 1;
        u8 Unknown186 : 1;
        u8 Unknown187 : 1;
        u8 Unknown188 : 1;
        u8 Unknown189 : 1;
        u8 Unknown190 : 1;
        u8 Unknown191 : 1;
        u8 Unknown192 : 1; //Gets set and unset at a new day
        u8 Unknown193 : 1; //Gets set and unset at a new day
        u8 Unknown194 : 1; //Gets set and unset at a new day
        u8 Unknown195 : 1;
        u8 Unknown196 : 1;
        u8 Unknown197 : 1;
        u8 Unknown198 : 1;
        u8 Unknown199 : 1;
        u8 Unknown200 : 1;
        u8 Unknown201 : 1;
        u8 Unknown202 : 1;
        u8 Unknown203 : 1;
        u8 Unknown204 : 1;
        u8 Unknown205 : 1;
        u8 Unknown206 : 1;
        u8 Unknown207 : 1;
        u8 KnowsPermitRequirements : 1; //Isabelle explained how to obtain mayor permit
        u8 TomNookToldYouFirstPayment : 1;
        u8 Permit_Points1 : 1; //written to bulletin board? (1 points)
        u8 Permit_Points2 : 1; //mayor permit (2 points)
        u8 Permit_Points3 : 1; //written to bulletin board? (4 points)
        u8 Permit_Points4 : 1; //mayor permit (8 points)
        u8 Permit_Points5 : 1; //mayor permit (16 points)
        u8 Permit_Points6 : 1; //mayor permit (32 points)
        u8 Permit_Points7 : 1; //mayor permit (64 points)
        u8 Permit_Points8 : 1; //mayor permit? (128 points?)
        u8 Permit_Points9 : 1; //mayor permit? (256 points?)
        u8 Permit_Points10 : 1; //mayor permit? (512 points?)
        u8 Unknown220 : 1; 
        u8 Unknown221 : 1;
        u8 Unknown222 : 1;
        u8 Unknown223 : 1;
        u8 Unknown224 : 1;
        u8 Unknown225 : 1;
        u8 Unknown226 : 1;
        u8 Unknown227 : 1;
        u8 Unknown228 : 1;
        u8 Unknown229 : 1;
        u8 Unknown230 : 1;
        u8 Unknown231 : 1;
        u8 Unknown232 : 1;
        u8 Unknown233 : 1;
        u8 Unknown234 : 1;
        u8 Unknown235 : 1;
        u8 Unknown236 : 1;
        u8 Unknown237 : 1;
        u8 Unknown238 : 1;
        u8 Unknown239 : 1;
        u8 Unknown240 : 1;
        u8 Unknown241 : 1;
        u8 Unknown242 : 1; //Gets set and unset at a new day
        u8 Unknown243 : 1; //Gets set and unset at a new day
        u8 Unknown244 : 1; //Gets set and unset at a new day
        u8 Unknown245 : 1; //Gets set and unset at a new day
        u8 Unknown246 : 1; //Gets set and unset at a new day
        u8 Unknown247 : 1; //Gets set and unset at a new day
        u8 Unknown248 : 1; //Gets set and unset at a new day
        u8 Unknown249 : 1; //Gets set and unset at a new day
        u8 Unknown250 : 1;
        u8 HasClubTortimerMembership : 1;
        u8 ClubTortimerFirstAsked : 1; //Kappn asks user for the first time 
        u8 ClubTortimerRulesExplained : 1;
        u8 Unknown254 : 1;
        u8 Unknown255 : 1;
        u8 Unknown256 : 1;
        u8 Unknown257 : 1;
        u8 Unknown258 : 1; //lyle talked to you at your house for becoming VIP
        u8 Unknown259 : 1; //maybe after first day??
        u8 Unknown260 : 1;
        u8 Unknown261 : 1;
        u8 Unknown262 : 1;
        u8 Unknown263 : 1;
        u8 Unknown264 : 1;
        u8 Unknown265 : 1;
        u8 Unknown266 : 1;
        u8 Unknown267 : 1; //maybe donated fossil?
        u8 Unknown268 : 1;
        u8 Unknown269 : 1;
        u8 Unknown270 : 1;
        u8 Unknown271 : 1;
        u8 Unknown272 : 1;
        u8 Unknown273 : 1; //Island People Here Tutorial
        u8 Unknown274 : 1;
        u8 Unknown275 : 1;
        u8 Unknown276 : 1;
        u8 Unknown277 : 1;
        u8 Unknown278 : 1;
        u8 Unknown279 : 1;
        u8 Unknown280 : 1;
        u8 Unknown281 : 1; //Tommy/Timmy first time selling item
        u8 Unknown282 : 1;
        u8 Unknown283 : 1;
        u8 Unknown284 : 1; //Isabelle second advice? (Letters)
        u8 Unknown285 : 1; //Isabelle second advice? (Letters)
        u8 Unknown286 : 1; //Isabelle third advice (Beach)
        u8 Unknown287 : 1; //Isabelle third advice (Beach)
        u8 Unknown288 : 1; //Isabelle fourth advice (Timmy and Tommy)
        u8 Unknown289 : 1; //Isabelle fifth advice (bury with shovel)
        u8 Unknown290 : 1; //Isabelle fifth advice (bury with shovel)
        u8 Unknown291 : 1; //Isabelle fifth advice (bury with shovel)
        u8 Unknown292 : 1;
        u8 Unknown293 : 1; //Isabelle finished fishing/catching advice (got watering can)
        u8 Unknown294 : 1; //Isabelle finished fishing/catching advice (got watering can)
        u8 Unknown295 : 1; //Isabelle finished fishing/catching advice (got watering can)
        u8 Unknown296 : 1; //Isabelle fifth advice (bury with shovel)
        u8 Unknown297 : 1;
        u8 Unknown298 : 1; //Isabelle fourth advice? (Timmy and Tommy)
        u8 Unknown299 : 1;
        u8 Unknown300 : 1; //Isabelle ask first advice (Talk to new villager)
        u8 Unknown301 : 1;
        u8 Unknown302 : 1;
        u8 Unknown303 : 1;
        u8 Unknown304 : 1;
        u8 Unknown305 : 1;
        u8 Unknown306 : 1;
        u8 Unknown307 : 1;
        u8 Unknown308 : 1;
        u8 Unknown309 : 1;
        u8 Unknown310 : 1;
        u8 Unknown311 : 1;
        u8 Unknown312 : 1;
        u8 Unknown313 : 1;
        u8 Unknown314 : 1; //Nintendo 3DS Image Share used for the first time
        u8 SecretStorageExplained : 1; //Tom nook tells you what the secret storage is
        u8 Unknown316 : 1; //Found magic lamp for the first time
        u8 Unknown317 : 1; //Talked to wisp for the first time
        u8 Unknown318 : 1; //isabelle wants to hold ceremony for becomign major | received tpc?
        u8 BuildingSecretStorage : 1; //Next day secret storage will be built (1 = will be built -> afterwards 0 when built)
        u8 UnlockedSecretStorage : 1;
        u8 Unknown321 : 1;
        u8 Unknown322 : 1; //Talking to harvey for the first time
        u8 Unknown323 : 1;
        u8 Unknown324 : 1;
        u8 Unknown325 : 1;
        u8 Unknown326 : 1;
        u8 Unknown327 : 1;
        u8 Unknown328 : 1;
        u8 Unknown329 : 1;
        u8 Unknown330 : 1;
        u8 Unknown331 : 1;
        u8 Unknown332 : 1;
        u8 Unknown333 : 1;
        u8 Unknown334 : 1;
        u8 Unknown335 : 1;
        u8 Unknown336 : 1;
        u8 Unknown337 : 1;
        u8 Unknown338 : 1;
        u8 Unknown339 : 1;
        u8 Unknown340 : 1;
        u8 Unknown341 : 1;
        u8 Unknown342 : 1;
        u8 Unknown343 : 1;
        u8 Unknown344 : 1;
        u8 Unknown345 : 1;
        u8 Unknown346 : 1;
        u8 Unknown347 : 1;
        u8 Unknown348 : 1; //Related to scanning amiibo at wisp lamp
        u8 Unknown349 : 1;
        u8 Unknown350 : 1; //Related to finishing first day
        u8 Unknown351 : 1; //after talking to cat in train at beginning of game
        u8 Unknown352 : 1;
        u8 Unknown353 : 1;
        u8 Unknown354 : 1;
        u8 Unknown355 : 1;
        u8 UnlockedHouseEditor : 1;
        u8 Unknown357 : 1;
        u8 Unknown358 : 1; //talking to harvery for the first time
        u8 Unknown359 : 1;
        u8 RecievedCATIntro : 1; //at the townhall station
        u8 Unknown361 : 1;
        u8 Unknown362 : 1;
        u8 Unknown363 : 1;
        u8 Unknown364 : 1;
        u8 Unknown365 : 1;
        u8 Unknown366 : 1;
        u8 Unknown367 : 1;
        u8 Unknown368 : 1;
        u8 Unknown369 : 1;
        u8 Unknown370 : 1;
        u8 Unknown371 : 1;
        u8 Unknown372 : 1;
        u8 Unknown373 : 1;
        u8 Unknown374 : 1;
        u8 Unknown375 : 1;
        u8 Unknown376 : 1;
        u8 Unknown377 : 1;
        u8 Unknown378 : 1;
        u8 Unknown379 : 1;
        u8 Unknown380 : 1;
        u8 Unknown381 : 1;
        u8 Unknown382 : 1;
        u8 CanUseCensusMenu : 1;
        u8 Unknown384 : 1;
        u8 Unknown385 : 1; //finished cat entry (First intro one?)
        u8 Unknown386 : 1; //finished cat entry (Second Intro one)
        u8 Unknown387 : 1; //finished all intro cat entries?
        u8 Unknown388 : 1;
        u8 Unknown389 : 1; //TPC explained (Intro)
        u8 Unknown390 : 1;
        u8 Unknown391 : 1;
        u8 Unknown392 : 1;
        u8 Unknown393 : 1;
        u8 Unknown394 : 1;
        u8 Unknown395 : 1;
        u8 Unknown396 : 1;
        u8 Unknown397 : 1;
        u8 Unknown398 : 1; //Related to finishing first day
        u8 Unknown399 : 1;
        u8 Unknown400 : 1;
        u8 Unknown401 : 1;
        u8 Unknown402 : 1; //CAT explained in TPC (Intro)
        u8 Unknown403 : 1;
        u8 Unknown404 : 1;
        u8 Unknown405 : 1;
        u8 Unknown406 : 1;
        u8 Unknown407 : 1;
        u8 Unknown408 : 1;
        u8 Unknown409 : 1;
        u8 Unknown410 : 1;
        u8 Unknown411 : 1;
        u8 Unknown412 : 1;
        u8 Unknown413 : 1;
        u8 Unknown414 : 1;
        u8 Unknown415 : 1;
        u8 Unknown416 : 1;
    };

    struct ACNL_Letter { //Size: 0x280
        PersonalID Reciever; //0
        u16 ZeroPad_1; //0x2E
        u16 ReceiverID; //0x30
        u8 ZeroPad_2[50]; //0x32
        u16 Unknown2; //0x64 //Some form of ID? 
        u16 ZeroPad_3; //0x66
        wchar Header[32]; //0x68 //Max amount is 32 UTF-16 characters
        u16 ZeroPad_4; //0xA8
        wchar Body[192]; //0xAA //Max amount is 192 UTF-16 characters
        u16 ZeroPad_5; //0x22A
        wchar Signature[32]; //0x22C //Max amount is 32 UTF-16 characters
        u16 ZeroPad_6; //0x26C
        u8 RecieverNameIndent; //0x26E
        u8 PaperID; //0x26F
        u8 LetterFlag; //0x270 //9 closed
        u8 StringIDOfSender; //0x271
        u8 LetterType; //0x272 //0x88 time capsule
        u8 Unknown3; //0x273
        Item AttachedItem; //0x274
        u64 Unknown4; //0x278
    };

    enum GulliverLetterType : u8 {
        None = 0,
        SouthKorea = 1,
        EasterIslands = 2,
        Scotland = 3,
        Russia = 4,
        Singapore = 5,
        Denmark = 6,
        Belgium = 7,
        Spain = 8,
        France = 9,
        Rome = 0xA,
        Japan = 0xB,
        Ireland = 0xC,
        HowAreYou = 0xD, //he asks how you are and all
        Mexico = 0xE,
        Germany = 0xF,
        Netherlands = 0x10,
        DoYouMissMe = 0x11, //he says he is becoming a movie star
        Kenya = 0x12,
        Egypt = 0x13,
        China = 0x14,
        Greece = 0x15,
        England = 0x16,
        Australia = 0x17,
        Vietnam = 0x18,
        Sweden = 0x19,
        Thailand = 0x1A,
        India = 0x1B,
        Peru = 0x1C,
        Hawaii = 0x1D,
        Portugal = 0x1E,
        Empty = 0x1F, //Probably unused
        Empty2 = 0x20, //Probably unused
        Empty3 = 0x21, //Probably unused
        Empty4 = 0x22, //Probably unused
        Empty5 = 0x23, //Probably unused
        Empty6 = 0x24, //Probably unused
        Empty7 = 0x25, //Probably unused
        Empty8 = 0x26, //Probably unused
        Empty9 = 0x27, //Probably unused
        Empty10 = 0x28, //Probably unused
        Empty11 = 0x29, //Probably unused
        Empty12 = 0x2A, //Probably unused
        Empty13 = 0x2B, //Probably unused
        Empty14 = 0x2C, //Probably unused
        Empty15 = 0x2D, //Probably unused
        WrongGuess1 = 0x2E,
        WrongGuess2 = 0x2F,
        WrongGuess3 = 0x30
    };

    struct PrizeLetterType {
        u8 First : 1;
        u8 Second : 1;
        u8 Third : 1;
        u8 _pad : 5;
    };

    struct SnowmanLetterType {
        u8 Snowtyke_Sleigh : 1; //when only building snowtyke
        u8 Snowtyke_SnowBunny : 1; //when building 3 of his family members
        u8 Snowtyke_SmallIgloo : 1; //when building 2 of his family members
        u8 Snowtyke_SnowmanMatryoshka : 1; //when getting whole family together
        u8 Snowboy_Gift : 1; 
        u8 _pad : 3;
    };

    /*
    Starts at 0x8ADC in player struct
    This handles letters and their gifts from special npcs
    needs a lot of research!
    */
    struct UnknownStruct1 { 
        u32 Unk1; //0 //Set to 0 in Player ctor
        Item GulliverGift; //4
        GulliverLetterType GulliverLetter; //8 //received the next day
        u8 Unk4; //9 //Set to 0x80 in Player ctor
        u16 Unk5; //0xA //Set to 0 in Player ctor
        u8 Unk6; //0xC //Set to 0xFF in Player ctor
        u32 Unk7; //0xD //Set to 0 in Player ctor (Set later, before 1st loop)
        u8 Unk8; //0x11 //Set to 0 in Player ctor
        u8 Unk9; //0x12 //Set to 0 in Player ctor (Set in 1st loop)
        u8 Padding1; //0x13 //Padding: Not set in Player ctor
        u16 Unk11; //0x14 //Set to 0 in Player ctor (Set in 1st loop)
        u8 Unk12; //0x16 //Set to 0 in Player ctor (Set in 1st loop)
        u8 Padding2; //0x17 //Padding: Not set in Player ctor
        u16 Unk14; //0x18 //Set to 0 in Player ctor (Set in 1st loop)
        u8 Unk15; //0x1A //Set to 0 in Player ctor (Set in 1st loop)
        u8 Padding3; //0x1B //Padding: Not set in Player ctor
        u16 Unk17; //0x1C //Set to 0 in Player ctor (Set in 1st loop)
        u8 Unk18; //0x1E //Set to 0 in Player ctor (Set in 1st loop)
        u8 Padding4; //0x1F //Padding: Not set in Player ctor
        u16 Unk20; //0x20 //Set to 0 in Player ctor (Set in 1st loop)
        u8 Unk21; //0x22 //Set to 0 in Player ctor (Set in 1st loop)
        u8 Padding5; //0x23 //Padding: Not set in Player ctor
        u16 Unk23; //0x24 //Set to 0 in Player ctor (Set in 1st loop)
        u8 Unk24; //0x26 //Set to 0 in Player ctor (Set in 1st loop)
        u8 Padding6; //0x27 //Padding: Not set in Player ctor
        u16 Unk26; //0x28 //Set to 0 in Player ctor (Set in 1st loop)
        u8 Unk27; //0x2A //Set to 0 in Player ctor
        u8 Padding7; //0x2B //Padding: Not set in Player ctor
        u16 Unk29; //0x2C //Set to 0 in Player ctor (Set in 1st loop)
        u8 Unk30; //0x2E //Set to 0 in Player ctor
        u8 Unk31; //0x2F //Set to 0 in Player ctor
        u16 CurrentYear; //0x30 //not sure for what
        u16 UnknownYear2; //0x32 //not sure for what
        u32 Unk34; //0x34 //Set to 0 in Player ctor
        u8 JingleLetter; //0x38 //received the next day //1 for receiving, 0 for not receiving
        u8 Unk36; //0x39 //Set to 0 in Player ctor
        u16 Unk37; //0x3A //Set to 0 in Player ctor
        u8 Unk38; //0x3C //Set to 0 in Player ctor
        u8 Unk39; //0x3D //Set to 0 in Player ctor
        u16 Unk40; //0x3E //Set to 0 in Player ctor
        PrizeLetterType ChipLetter; //0x40 //received the next day
        u8 Unk42; //0x41 //Set to 1 in Player ctor
        u16 Unk43; //0x42 //Set to 0 in Player ctor
        PrizeLetterType NatLetter; //0x44 //received the next day
        SnowmanLetterType SnowmanLetter; //0x45 //received the next day
        Item SnowmanGift; //0x46 //only used if the item is dynamic (like snowboy gifts)
        u16 UnknownYear3; //0x4A //Set to 0 in Player ctor
        u32 Unk49; //0x4C //Set to 0 in Player ctor
        u8 Unk50; //0x50 //Set to 0 in Player ctor
        u8 Unk51; //0x51 //Set to 0 in Player ctor
        u16 Unk52; //0x52 //Set to 0 in Player ctor
        u8 Unk53; //0x54 //Set to 0 in Player ctor
        u8 Unk54; //0x55 //Set to 0 in Player ctor
        u8 Unk55; //0x56 //Set to 0 in Player ctor
        u8 Unk56; //0x57 //Set to 0 in Player ctor
        u8 Unk57; //0x58 //Set to 5 in Player ctor
        u8 Unk58; //0x59 //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk59; //0x5A //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk60; //0x5B //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk61; //0x5C //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk62; //0x5D //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk63; //0x5E //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk64; //0x5F //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk65; //0x60 //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk66; //0x61 //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk67; //0x62 //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk68; //0x63 //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk69; //0x64 //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk70; //0x65 //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 Unk71; //0x66 //Set to 0 in Player ctor (Set in 2nd loop, BIC R2 R2 0xF)
        u8 KatieLetter; //0x67 //if 9 letter will be received the next day
        u8 Unk73; //0x68 //Set to 0 in Player ctor
        u8 Unk74; //0x69 //Set to 0 in Player ctor
        TownID KatieTraveledToTownData; //0x6A -> 0x7F //Will be shown in katies letter
    };

    /*
    Monday is ID 1
    */
    struct Daily_Initiative { //Size: 0x7
        u8 MondayInitiative; //0x0
        u8 TuesdayInitiative; //0x1
        u8 WednesdayInitiative; //0x2
        u8 ThursdayInitiative; //0x3
        u8 FridayInitiative; //0x4
        u8 SaturdayInitiative; //0x5
        u8 SundayInitiative; //0x6
    };

    struct ACNL_InitiativeList {
        //there are 102 initiatives in total
    };

    struct ACNL_Initiative { //Initiatives
        u64 Unknown5; //0x8B80 //???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
        u8 WeeklyInitiatives[2]; //0x8B88 //Set to 0 in player ctor
        Daily_Initiative DailyInitiatives[2]; //0x8B8A //Set to 0 in player ctor
        u16 Unknown10; //0x8B8E //???; Set to 0 in player ctor
        u8 Unknown11; //0x8B90 //???; Set to 0 in player ctor
        u32 Unknown12; //0x8B91 //???; Set to 0 in player ctor
        u16 Unknown13; //0x8B95 //???; Set to 0 in player ctor
        u8 Unknown14; //0x8B97 //???; Set to 0 in player ctor
        u8 Unknown15; //0x8B98 //???; Set to 0x65 in player ctor
        u8 Unknown16; //0x8B99 //???; Set to 0 in player ctor
        u8 Unknown17; //0x8B9A //???; Set to 0 in player ctor
        u8 Padding1; //0x8B9B //Padding: Not set in player ctor
        u8 Unknown19[0x198]; //0x8B9C //???; 0x198 buffer size set to 0 in player ctor
        u8 Unknown20[0x66]; //0x8D34 //???; 0x66 buffer size set to 0 in player ctor
        u8 Padding2; //0x8B9A //Padding: Not set in player ctor
        u8 Padding3; //0x8B9B //Padding: Not set in player ctor
        u32 Unknown23; //0x8D9C //???; Set to 0 in player ctor
        u32 Unknown24; //0x8DA0 //???; Set to 0 in player ctor
        u32 Unknown25; //0x8DA4 //???; Set to 0 in player ctor
        u32 Unknown26; //0x8DA8 //???; Set to 0 in player ctor
        u32 Unknown27; //0x8DAC //???; Set to 0 in player ctor
        u32 Unknown28; //0x8DB0 //???; Set to 0 in player ctor
        u32 Unknown29; //0x8DB4 //???; Set to 0 in player ctor
        u32 Unknown30; //0x8DBC //???; Set to 0 in player ctor
    };

    struct UnknownStruct3 {
        s64 Unknown1; //0x8DEC //???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
        u8 Unknown2; //0x8DF4 //???; Set to 0x21 in player ctor
        u8 Unknown3; //0x8DF5 //???; Set to 0xFF in player ctor
        u8 Unknown4; //0x8DF6 //???; Set to 0x9 in player ctor
        u8 Padding1; //0x8DF7 //Padding: Not set in ctor
        u16 Unknown6; //0x8DF8 //???; Set to 0x0000 in player ctor
        u8 Unknown7; //0x8DFA //???; Set to 0x6 in player ctor
        u8 Unknown8; //0x8DFB //???; Set to 0xFF in player ctor
    };

    struct UnknownStruct4 { //Probably exact same struct as UnknownStruct3 but is set differently
        s64 Unknown1; //0x8DEC //???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
        u8 Unknown2; //0x8DF4 //???; Set to 0x21 in player ctor
        u8 Unknown3; //0x8DF5 //???; Set to 0xFF in player ctor
        u8 Unknown4; //0x8DF6 //???; Set to 0x9 in player ctor
        u8 Padding1; //0x8DF7 //Padding: Not set in ctor
        u16 Unknown6; //0x8DF8 //???; Set to 0x7ffe in player ctor
        u8 Unknown7; //0x8DFA //???; Set to 0x4 in player ctor
        u8 Unknown8; //0x8DFB //???; Set to 0xFF in player ctor
    };

    struct UnknownStruct5 {//Similar to UnknownStruct3
        s64 Unknown1; //0x8DEC //???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
        u8 Unknown2; //0x8DF4 //???; Set to 0x21 in player ctor
        u8 Unknown3; //0x8DF5 //???; Set to 0xFF in player ctor
        u8 Unknown4; //0x8DF6 //???; Set to 0x9 in player ctor
        u8 Padding1; //0x8DF7 //Padding: Not set in ctor
        u32 Unknown6; //0x8DF8 //???; Set to 0x00007ffe in player ctor
        u32 Unknown7; //0x8DFC //???; Set to 0x00007ffe in player ctor
        u8 Unknown8; //0x8E00 //???; Set to 0xFF in player ctor
        u8 Unknown9; //0x8E01 //???; Set to 0x7 in player ctor
        u8 Unknown10; //0x8E02 //???; Set to 0xFF in player ctor
        u8 Padding2; //Padding: Not set in ctor;
    };

    struct VillagerType { //Size: 0x30
        TownID TownData1; //0x0
        TownID TownData2; //0x16
        u16 VillagerID; //0x2C //Set to 0xFFFF in player ctor
        u8 VillagerPersonality; //0x2E //Set to 0x8 in player ctor 
        u8 Padding; //0x2F //Padding: Not set in ctor
    };

    struct UnknownStruct6 {
        s64 Unknown1; //???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
        u8 Unknown2; //???; Set to 0x21 in player ctor
        u8 Unknown3; //???; Set to 0xFF in player ctor
        u8 Unknown4; //???; Set to 0x9 in player ctor
        u8 Padding1; //Padding: Not set in ctor
        VillagerType Mini1;
        VillagerType Mini2;
        u32 Unknown6; //???; Set to 0x00007ffe in player ctor
        u32 Unknown7; //???; Set to 0x00007ffe in player ctor
        u8 Unknown8; //???; Set to 0 in player ctor
        u8 Unknown9; //???; Set to 0 in player ctor
    };

    struct UnknownStruct7 { //Probably exact same struct as UnknownStruct4 but is set differently
        s64 Unknown1; //???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
        u8 Unknown2; //???; Set to 0x21 in player ctor
        u8 Unknown3; //???; Set to 0xFF in player ctor
        u8 Unknown4; //???; Set to 0x9 in player ctor
        u8 Unknown5; //???; Set to 0xFF in player ctor
        u8 Unknown6; //???; Set to 0xFF in player ctor
        u8 Padding1; //Padding: Not set in ctor
    };

    struct UnknownStruct8 { //Similar to UnknownStruct3
        s64 Unknown1; //???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
        u8 Unknown2; //???; Set to 0x21 in player ctor
        u8 Unknown3; //???; Set to 0xFF in player ctor
        u8 Unknown4; //???; Set to 0x9 in player ctor
        u8 Unknown7; //Padding: Not set in ctor
    };

    struct UnknownStruct9 {
        u16 Unknown1; //???; Set to 0 in player ctor
        u8 Unknown2; //???; Set to 0 in player ctor
        u8 Unknown3; //???; Set to 0 in player ctor
        u32 Unknown4; //???; Set to 0 in player ctor
        u32 Unknown5; //???; Set to 0 in player ctor
        u32 Unknown6; //???; Set to 0 in player ctor
        u32 Unknown7; //???; Set to 0 in player ctor
        u32 Unknown8; //???; Set to 0 in player ctor
        u16 Unknown9; //???; Set to 0 in player ctor
        u32 Unknown10; //???; Set to 0 in player ctor
        u32 Unknown11; //???; Set to 0 in player ctor
        u32 Unknown12; //???; Set to 0 in player ctor
        u16 Unknown13; //???; Set to 0 in player ctor
        u8 Unknown14; //???; Set to 0 in player ctor
        u8 Padding1; //Padding: Not set in ctor
    };

    struct UnknownStruct10 {
        Item UnkItem1; //Some Item; Set to 0x00007ffe in player ctor
        u16 Unknown2; //???; Set to 0 in player ctor
    };

    struct LyleFlags {
        u8 HasHeardEvaluation : 1; //Set when asking about home evaluation
        u8 HasAskedWhatsNew : 1;
        u8 HasHeardFirstEvaluation: 1; //Set when asking about home evaluation
        u8 LyleWhatsNew : 5;
    };

    struct MiiData {
        u8 MiiFace[92]; //0x5538 -> 0x5595 //Based on https://3dbrew.org/wiki/Mii#Mii_format
        u16 ZeroPad_1 = 0;  //0x5594 //U16 Zero Padding; Always 0x0000
        u16 Mii_CRC16;
        u32 AES_CCM_MAC[4];
        u8 Unknown2[24]; //0x5596 -> 0x55BF: Gets written to when getting a Mii form Harriet. Never read(?)
        u8 ZeroPad_3[30]; //0x55C0 -> 0x55DF
        u16 ZeroPad_2;
    };

    struct Encyclopedia_Sizes {
        u16 Insects[0x48]; //range 1 to 0x3FFF
        u16 Fish[0x48]; //range 1 to 0x3FFF
        u16 SeaCreatures[0x1E]; //range 1 to 0x3FFF
    };

    struct Player_Features {
        u8 HairStyle; //0xA4
        u8 HairColor; //0xA5 //Values: 0x0 -> 0xF
        u8 Face; //0xA6 //Values: 0x0 -> 0xB
        u8 EyeColor; //0xA7 //Values: 0x0 -> 0x5
        u16 Tan; //0xA8 //Values: 0x0 -> 0xF
    };

    struct Player_Outfit { //sizeof = 0x21
        Item Hat; //0xAA //Item ID < 0xXXXX
        Item Accessory; //0xAE //Item ID < 0xXXXX
        Item TopWear; //0xB2 //Item ID < 0xXXXX
        Item UnderTopWear; //0xB6 //Item ID < 0xXXXX
        Item BottomWear; //0xBA //Item ID < 0xXXXX
        Item Socks; //0xBE //Item ID < 0xXXXX
        Item Shoes; //0xC2 //Item ID < 0xXXXX
        Item HeldItem; //0xC6 //Item ID < 0xXXXX
        u8 Unknown0; //0xCA //Inverted gender(?): 1 for male, 0 for female. Default = 1 in PlayerConstructor (EUR 1.5 0x20D27C)
    };

    struct Player_Appearance { // sizeof = 0x27
        Player_Features PlayerFeatures;
        Player_Outfit PlayerOutfit;
    };

    struct ACNL_Player { //Size: 0xA480 //UnknownNotSetYetX = Not set by player ctor
        u32 Checksum1; //0xA0 //Checksum of the first 0x6b84 of player data
        Player_Appearance PlayerAppearance; //0xA4
        u8 Padding_0 = 0; //0xCB
        ACNL_Pattern Patterns[10]; //0xCC //10 Patterns
        u8 PatternOrder[10]; //0x552C -> 0x5535 //Order of patterns from 0x0 - 0x9
        u16 Padding_1; //0x5536 //U16 Zero Padding; Always 0x0000
        MiiData PlayerMii;
        u8 HasMii; //0x55E0 //Values: 0 = No Mii, 1 = Has Mii, <1 = Has Mii, face doesn't show
        u8 Padding_2; //0x55E1 //Not Verified: U8 Zero Padding; Always 0x00
        u16 Padding_3; //0x55E2 -> 0x55E3 //Not Verified: U16 Zero Padding; Always 0x0000
        Mannequin Mannequin1;
        Mannequin Mannequin2;
        Mannequin Mannequin3;
        Mannequin Mannequin4;
        u16 Padding_4; //0x5644
        PersonalID PlayerInfo; //0x5646 -> 0x5671
        u8 BirthMonth; //0x5674
        u8 BirthDay; //0x5675
        u16 YearRegistered; //0x5676
        u8 MonthRegistered; //0x5678
        u8 DayRegistered; //0x5679
        u16 Padding_5; //0x567A: Zero Padding; Always 0x0000  
        PlayerBadges Badges; //0x567C -> 0x5763
        HHAHouseInfo HHAHouse; //0x5764 -> 0x5789
        ACNL_DreamAddress DreamCode;  //0x5790 -> 0x579B
        u32 Padding_6; //0x579C -> 0x579F
        Player_Flags PlayerFlags; //0x57A0 -> 0x57D3
        u32 HasTPCPic; //0x57D4 -> 0x57D7
        u8 TPCPic[0x1400]; //0x57D8 -> 0x6BD7
        wchar TPCText[33]; //0x6BD8 -> 0x6C19
        u8 Unknown1; //0x6C1A //Unknown: Was 1 on a save, 2 on another
        u8 Unknown2; //0x6C1B
        u32 Unknown3; //0x6C1C -> 0x6C19
        u32 Unknown4; //0x6C20 -> 0x6C23
        u32 Padding_7; //0x6C24 -> 0x6C27
        u32 Checksum2; //0x6C28 -> 0x6C2B
        u64 BankAmount; //0x6C2C -> 0x6C33: Encrypted Value
        u64 DebtAmount; //0x6C34 -> 0x6C3B: Encrypted Value
        u64 MedalAmount; //0x6C3C -> 0x6C43: Encrypted Value
        u64 BellsFromReeseAmount; //0x6C44 -> 0x6C4B: Encrypted Value
        u32 Padding_8; //0x6C4C -> 0x6C4F
        s64 Playtime; //0x6C50 -> 0x6C57
        TownID TownData2; //0x6C68 -> 0x6C5D
        u16 Padding_10; //0x6C6E -> 0x6C6F
        Item Inventory[16]; //0x6C70 -> 0x6CAF
        u8 InventoryItemLocks[16]; //0x6CB0 -> 0x6CBF
        u32 UnlockedItems[0xBA]; //0x6CC0 -> 0x6FA7 //Game uses one big bitfield for items 'unlocked'. Bits correspond to item ids. Used for catalog, encyclopedia, etc
        u64 PocketMoney; //0x6FA8 -> 0x6FAF: Encrypted Value
        Item IslandBox[40]; //0x6FB0 -> 0x704F
        Item IslandInventory[16]; //0x7050 -> 0x708F
        u8 IslandInventoryItemLocks[16]; //0x7090 -> 0x709F: may be Padding also, needs testing
        Item UnknownItem1; //0x70A0 -> 0x70A3
        Item UnknownItem2; //0x70A4 -> 0x70A7
        ACNL_Letter Letters[10]; //0x70A8 -> 0x89A7
        wchar LetterHeader[0x20]; //0x89A8 -> 0x89E7
        u16 Padding_11; //0x89E8 -> 0x89E9
        wchar FutureLetterHeader[0x20]; //0x89EA -> 0x8A29
        u16 Padding_12; //0x8A2A -> 0x8A2B
        wchar LetterSignature[0x20]; //0x8A2C -> 0x8A6B
        u16 Padding_13; //0x8A6C -> 0x8A6D
        u8 DefaultLtrRecieverNameIndent; //0x8A6E -> 0x8A6E
        u8 DefaultFutureLtrRecieverNameIndent; //0x8A6F -> 0x8A6F //Cannot be >= 0x20
        Emoticons Emotes; //0x8A70 -> 0x8A97 //Players Emotes (40 slots)
        s8 EmotePage; //0x8A98 -> 0x8A98 //0xFF = Page 1, 0x00 = Page 2
        u8 Padding_14; //0x8A99 -> 0x8A99
        u16 SpotpassDLCRecievedIds[32]; //0x8A9A -> 0x8AD9 //Not Verified; 0xFFFF is default, then 0xXXXX is ID of DLC recieved
        u16 Padding_15; //0x8ADA -> 0x8ADB
        UnknownStruct1 UnkBuffer1; //0x8ADC -> 0x8B5B
        u16 Padding_16; //0x8B5C -> 0x8B5D
        LyleFlags LyleFlag; //0x8B5E
        u8 HasDeductions__; //0x8B5F //Not 100% sure; something to do with items facing a wall, therefore deductions; Reads HouseUnk9 later in code
        u8 HHAAwardsUnlockedDupe; //0x8B60
        u8 GoldExteriorsUnlockedDupe; //0x8B61
        u8 HHAUnk1; //0x8B62
        u8 HHAUnk2; //0x8B63 //READU8(CurrentHouseTheme+1) | 0x80; Only When Exterior/Theme???
        u8 HHAUnk3[0x1C]; //0x8B63 -> //0x8B7F //Come back to later
        ACNL_Initiative Initiative; //0x8B80 -> 0x8DBB
        u64 MeowCoupons; //0x8DBC -> 0x8DC3: Encrypted Value
        u64 Unk2; //0x8DC4 -> 0x8DCB: Encrypted Value
        u64 Unk3; //0x8DCC -> 0x8DD3: Encrypted Value
        u64 Unk4; //0x8DD4 -> 0x8DDB: Encrypted Value
        u64 Unk5; //0x8DDC -> 0x8DE3: Encrypted Value
        u64 Unk6; //0x8DE4 -> 0x8DEB: Encrypted Value
        UnknownStruct3 UnkBuffer3;
        UnknownStruct4 UnkBuffer4;
        UnknownStruct5 UnkBuffer5;
        UnknownStruct6 UnkBuffer6;
        UnknownStruct7 UnkBuffer7;
        UnknownStruct8 UnkBuffer8;
        u8 UnknownNotSetYet[0xAA];
        u8 PlayerFFFF_Filler[0x16]; //Always contains 0x16 0xFF bytes. ctor sets this.
        u8 UnknownNotSetYet2[0xC];
        UnknownStruct9 UnkBuffer9;
        Item UnkItem1;
        Item UnkItem2;
        u8 UnknownNotSetYet3[0xE];
        u8 UnknownNotSetYet4[0xA];
        u16 Unk_u16_1; //Set to 0x7DB in player ctor
        u16 Unk_u16_2; //Set to 0x7DB in player ctor
        u16 Unk_u16_3; //Set to 0 in player ctor
        u8 Unk_u8_1; //Set to 0 in player ctor
        u8 Unk_u8_2; //Set to 0 in player ctor
        u8 UnknownNotSetYet5[0x45];
        u8 UnkBuffer10[0x21]; //ctor unsets every bit except in last byte in buffer, it only unsets bits 0 to 5
        u8 Padding_17;
        u8 Unk_u8_3; //Set to 0 in player ctor
        u16 Padding_18;
        u32 AddedSongs[3]; //Bitfield for added songs
        Item SantaBagInv[10]; //0x9048 -> 0x906F
        u8 PlayerZero_Filler[0x320]; //Always 0?? Game just memclr's in player ctor
        Item Dressers[180]; //Each dresser is 60 long
        wchar BDayWish[0x22]; //0x9660 -> 0x96A4
        /*
        ACNL_Letter Letter1; 
        ACNL_Letter Letter2;
        ACNL_Letter Letter3;
        ACNL_Letter Letter4;
        ACNL_Letter Letter5;
        u8 UnknownNotSetYet7[0xC0];
        u8 UnkBuffer11[0x40];
        u8 UnkBuffer12[0x40];
        u8 UnknownNotSetYet8[0xA4];
        */
        u8 UnknownBuffer[0xC84]; 
        Encyclopedia_Sizes EncyclopediaSizes; //0xA328 -> 0xA484
        u8 UnkBuffer11[0x84]; //0xA485 -> 0xA507
        UnknownStruct10 UnkStruct10_1; //0xA508
        UnknownStruct10 UnkStruct10_2;
        UnknownStruct10 UnkStruct10_3;
        Item UnkItem3; //Some Item; Set to 0x00007ffe in player ctor
        u16 Padding_19;
    };

    struct ACNL_DateTime {
        u16 Year;
        u16 Unk0; //Maybe padding
        u8 Month;
        u8 Day;
        u8 Unk1; //some sort of flag probably
        u8 Hour;
        u8 Minute;
        u8 Second;
        u16 Millisecond;
    };

    struct ACNL_Date { //Size: 0x4
        u16 Year; //0x0
        u8 Month; //0x2
        u8 Day; //0x3
    };

    struct VillagerFutureHome { //Size: 0xF2
        PersonalID unknownPID; //0x0
        u8 Zerod[0x22]; //0x2E //Related to class script::WordPtrSv, technically same buffer as UnkName
        wchar UnkName[9]; //0x50 //Related to class script::WordPtrSv, technically same buffer as Zerod
        Item UnkItem1; //0x62 //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem2; //0x66 //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem3; //0x6A //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem4; //0x6E //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem5; //0x72 //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem6; //0x76 //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem7; //0x7A //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem8; //0x7E //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem9; //0x82 //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem10; //0x86 //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem11; //0x8A //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem12; //0x8E //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem13; //0x92 //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem14; //0x96 //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem15; //0x9A //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem16; //0x9E //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem17; //0xA2 //Group 3; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem18; //0xA6 //Group 4; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem19; //0xAA //Group 5; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem20; //0xAE //Group 6; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem21; //0xB2 //Group 7; Some Item; Set to 0x00007ffe in player ctor
        u8 UnkBitfield[8]; //0xB6
        u16 unk; //0xBE
        TownID UnkTownID; //0xC0
        u64 Unknown22; //0xD6 //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        ACNL_Date Date1; //0xDE
        ACNL_Date Date2; //0xE2
        ACNL_Date Date3; //0xE6
        ACNL_Date Date4; //0xEA
        u16 Unknown23; //0xEE //ctor sets to 0xFFFF
        u8 Unk24; //0xF0 //ctor sets to 0x31
        u8 Padding; //0xF1
    };

    struct Villager_Status { //Size: 0x4
        u8 IsBoxed : 1;
        u8 HasMoved : 1;
        u8 Unknown3 : 1;
        u8 Unknown4 : 1;
        u8 Unknown5 : 1;
        u8 Unknown6 : 1;
        u8 Unknown7 : 1;
        u8 Unknown8 : 1;
        u8 Unknown9 : 1;
        u8 Unknown10 : 1;
        u8 Unknown11 : 1;
        u8 Unknown12 : 1; 
        u8 Unknown13 : 1;
        u8 RelatedToBelowButUnk : 1;
        u8 MovingToAnotherTown : 1;
        u8 Unknown16 : 1;
        u8 Unknown17 : 1;
        u8 Unknown18 : 1;
        u8 Unknown19 : 1;
        u8 Unknown20 : 1;
        u8 Unknown21 : 1;
        u8 Unknown22 : 1;
        u8 Unknown23 : 1;
        u8 Unknown24 : 1;
        u8 Unknown25 : 1;
        u8 Unknown26 : 1;
        u8 Unknown27 : 1;
        u8 Unknown28 : 1;
        u8 Unknown29 : 1;
        u8 Unknown30 : 1;
        u8 Unknown31 : 1;
        u8 Unknown32 : 1;
    };

    struct ACNL_Villager { //Size: 0x2518
        VillagerType villagerType; //0x0 //Which villager
        ACNL_Pattern Pattern; //0x30
        TownID TownID1; //0x8A0
        u32 Unknown1[2]; //0x8B6 //Game copies value from 0x6E29A, otherwise gens random number, gets u32 value then does something to it (EUR 1.5 IDA: 0x6BBBD4)
        u64 Unknown2; //0x8BE //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        Item UnkItem1; //0x8C6 //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem2; //0x8CA //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem3; //0x8CE //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem4; //0x8D2 //Group 1; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem5; //0x8D6 //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem6; //0x8DA //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem7; //0x8DE //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem8; //0x8E2 //Group 2; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem9; //0x8E6 //Group 3; Some Item; Set to 0x00007ffe in player ctor
        Item UnkItem10; //0x8EA //Group 4; Some Item; Set to 0x00007ffe in player ctor
        //This is the places the Villager has passed through, which is cool
        VillagerFutureHome Home[16]; //0x8EE //If moving: 0 = next home, 1 = current home | If not moving: 0 = current home, 1 = last home
        u16 Padding; //0x180E
        ACNL_Letter Letter[5]; //0x1810
        u16 UnkNum1; //0x2490 //Single; Set to 2011 in ctor
        u16 UnkNum2; //0x2492 //Group 1; Set to 2011 in ctor
        u16 UnkNum3; //0x2494 //Group 1; Set to 2011 in ctor
        u16 UnkNum4; //0x2496 //Group 1; Set to 2011 in ctor
        u16 UnkNum5; //0x2498 //Group 1; Set to 2011 in ctor
        Item Shirt; //0x249A //246E - 2471
        Item Song; //0x249E //2472 - 2475
        Item Wallpaper; //0x24A2 //2476 - 2479
        Item Carpet; //0x24A6 //247A - 247D
        Item Umbrella; //0x24AA //247E - 2481
        Item Furniture[16]; //0x24AE //2482 - 24C1
        ACNL_Date Date1; //0x24EE //24C2 - 24C5
        wchar Catchphrase[11]; //0x24F2 //Last character is null terminator | 24C6 - 24DB
        u8 Unknown3; //0x2508 //ctor sets to 7
        u8 Padding2; //0x2509
        ACNL_Date Date2; //0x250A //This seems to be a date, maybe date last talked??
        u8 Unknown4; //0x250E //ctor sets to 2 //Also general flags? | 24E4
        u8 Unknown5; //0x250F //ctor sets to 0x18; 0x2 means they're at home
        Villager_Status Status; //0x2510 //ctor sets to 0; bit 1 set: moving out, bit 2 set: moving in; bit 3 removed from all villagers when loading game, unk
        u8 Unknown6; //0x2514 //ctor sets to 0xFF
        u8 Unknown7; //0x2515 //ctor sets to 0xFF
        u8 Unknown8; //0x2516 //ctor sets to 0xFF
        u8 Unknown9; //0x2517 //ctor sets to 0
    };

    /*
    When time capsule gets removed:
    0
    1
    4
    5
    6

    Valid bits:
    0
    1
    2 => Please give time capsule back
    3
    4 => Time Capsule ready, talk to villager
    5 => Villager told you to dig the time capsule back up
    6
    7 => Villager dug the time capsule up themselves V1
    8 => Villager dug the time capsule up themselves V2
    9 => Might be default
    0xA
    0xB => Villager finished telling you they dug the time capsule up themselves
    
    */
    struct Unknown_VillagerData1_Bitfield {
        u8 Unknown0 : 1;
        u8 Unknown1 : 1;
        u8 Unknown2 : 1;
        u8 Unknown3 : 1;
        u8 Unknown4 : 1;
        u8 Unknown5 : 1;
        u8 Unknown6 : 1;
        u8 Unknown7 : 1;
    };

    struct Unknown_VillagerData1 { //Size: 0x40
        u64 Unknown0; //0x0 //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u8 Unknown1; //0x8
        Unknown_VillagerData1_Bitfield Unknown2; //0x9 //Time capsule? //2 = Return Time Capsule | 9 = Time Capsule Returned
        u8 unk1[0x36]; //0xA
    };

    struct Unknown_VillagerData2 { //Size: 0x80
        Unknown_VillagerData1 UnkVillagerData1[2];
    };

    struct ACNL_VillagerData { //0x292A0 //Size: 0x22BE0
        u32 Checksum; //0x292A0 //Checksum of the 0x22BC8 of this data
        ACNL_Villager Villager[10]; //0x292A4
        u32 Unk1; //0x40594
        u8 Unknown1[0x154]; //0x40598
        Unknown_VillagerData2 UnkVillagerData1[4]; //0x406EC //for each player one struct
        u8 Unknown3[0x2E]; //0x408EC
        TownID townID1; //has something to do with the campsite //0x4091A
        TownID townID2; //has something to do with the campsite //0x40930
        u8 Unknown4[0x20C6]; //0x40946
        ACNL_Villager UnkVillager[4]; //0x42A0C
        u8 Unknown5[0x14]; //0x4BE6C
    };

    struct ACNL_Building { //Size: 0x4
        u16 ID; //0xFC is none, only below 0xFC considered valid
        u8 XCoord;
        u8 YCoord;
    };

    struct Town_Buildings { //Size: 0xE8
        ACNL_Building Building[56];
        ACNL_Building EventPWP[2];
    };

    struct Unlocked_PWPs {
        u8 DefaultPWPs : 1; //All unlocked at default (cobblestone bridge, suspension bridge, yellow bench, water well, fountain, park clock, street lamp, campsite, fence, fire hydrant, custom-design sign, face-cutout standee, do-not-enter sign)
        u8 Topiaries : 1; //round, square, tulip
        u8 DreamSuite : 1;
        u8 MuseumRenovation : 1;
        u8 Cafe : 1;
        u8 ResetCenter : 1;
        u8 PerfectTownPWPs : 1; //town-hall renovations & flower clock
        u8 StationReconstruction : 1;
        u8 DrinkingFountain : 1;
        u8 GarbageCan : 1;
        u8 FlowerBed : 1;
        u8 OutdoorChair : 1;
        u8 FlowerArch : 1;
        u8 FairyTaleClock : 1;
        u8 FairyTaleBench : 1;
        u8 FairyTaleStreetlight : 1;
        u8 FairyTaleBridge : 1;
        u8 MetalBench : 1;
        u8 RoundStreetlight : 1;
        u8 IlluminatedHeart : 1;
        u8 IlluminatedClock : 1;
        u8 IlluminatedTree : 1;
        u8 Bell : 1;
        u8 ArchwaySculpture : 1;
        u8 StatueFountain : 1;
        u8 HotSpring : 1;
        u8 Streetlight : 1;
        u8 IlluminatedArch : 1;
        u8 Tower : 1;
        u8 ModernClock : 1;
        u8 ModernBench : 1;
        u8 ModernStreetlight : 1;
        u8 Scarecrow : 1;
        u8 Geyser : 1;
        u8 Windmill : 1;
        u8 WoodBench : 1;
        u8 WisteriaTrellis : 1;
        u8 LogBench : 1;
        u8 BusStop : 1;
        u8 PicnicBlanket : 1;
        u8 BalloonArch : 1;
        u8 TireToy : 1;
        u8 PileOfPipes : 1;
        u8 CampingCot : 1;
        u8 JungleGym : 1;
        u8 Sandbox : 1;
        u8 Hammock : 1;
        u8 WaterPump : 1;
        u8 InstrumentShelter : 1;
        u8 Torch : 1;
        u8 FirePit : 1;
        u8 SolarPanel : 1;
        u8 BlueBench : 1;
        u8 TrafficSignal : 1;
        u8 StadiumLight : 1;
        u8 VideoScreen : 1;
        u8 WoodenBridge : 1;
        u8 ZenGarden : 1;
        u8 ZenBell : 1;
        u8 RackOfRice : 1;
        u8 DrillingRig : 1;
        u8 ZenClock : 1;
        u8 ZenBench : 1;
        u8 ZenStreetlight : 1;
        u8 Sphinx : 1;
        u8 TotemPole : 1;
        u8 ParabolicAntenna : 1;
        u8 MoaiStatue : 1;
        u8 Stonehenge : 1;
        u8 Pyramid : 1;
        u8 CubeSculpture : 1;
        u8 ChairSculpture : 1;
        u8 PoliceStations : 1; //modern & classic
        u8 Lighthouse : 1;
        u8 BrickBridge : 1;
        u8 ModernBridge : 1;
        u8 StoneTablet : 1;
        u8 WindTurbine : 1;
        u8 CautionSign : 1;
        u8 YieldSign : 1;
        u8 FortuneTellersShop : 1;
        //All unused flags
        u8 Unused1 : 1;
        u8 Unused2 : 1;
        u8 Unused3 : 1;
        u8 Unused4 : 1;
        u8 Unused5 : 1;
        u8 Unused6 : 1;
        u8 Unused7 : 1;
        u8 Unused8 : 1;
        u8 Unused9 : 1;
        u8 Unused10 : 1;
        u8 Unused11 : 1;
        u8 Unused12 : 1;
        u8 Unused13 : 1;
        u8 Unused14 : 1;
        u8 Unused15 : 1;
    };

    struct DesignStand {
        ACNL_Pattern Pattern;
        u32 xCoord; //0xFFFFFFFF if none placed (idk why 32bit was needed, world coords are 8bit..)
        u32 yCoord; //0xFFFFFFFF if none placed (idk why 32bit was needed, world coords are 8bit..)
    };

    struct ACNL_BuildingData { //0x4BE80
        u32 Checksum; //0x4BE80 //Checksum of the 0x44B8 of this data
        u8 NormalPWPsAmount; //0x4BE84
        u8 EventPWPsAmount; //0x4BE85
        u8 TownTreeSize; //0x4BE86 //1 <-> 7
        u8 Padding1; //0x4BE87
        Town_Buildings Buildings; //0x4BE88
        DesignStand Stands[8]; //0x4BF70
        Unlocked_PWPs UnlockedPWPs; //0x50330
    };

    struct ACNL_MinigameData {
        u32 Checksum; //Checksum of the 0x28F0 of this data
        u8 Unknown[0x28F0];
    };

    struct ACNL_UnknownData {
        u32 Checksum; //Checksum of the 0x7F0 of this data
        u8 Unknown[0x7F0];
    };

    /*
    If furniture is "active", meaning if the lamp is turned on, if the tv is turned on, etc.
    By default every spot in the room is active
    Its a u16 : 1 bitflag
    */
    struct ACNL_Room_Active_Struct { //size=0x20
        u16 ActiveStates[16]; //default active
    };

    //0x5D904 and 0x5D90D
    struct PlayerHouseExterior { //size = 0x9
        u8 HouseSize; //0x5D904 and 0x5D90D
        u8 HouseStyle; //0x5D905 and 0x5D90E
        u8 HouseDoorShape; //0x5D906 and 0x5D90F
        u8 HouseBrick; //0x5D907 and 0x5D910
        u8 HouseRoof; //0x5D908 and 0x5D911
        u8 HouseDoor; //0x5D909 and 0x5D912
        u8 HouseFence; //0x5D90A and 0x5D913
        u8 HousePavement; //0x5D90B and 0x5D914
        u8 HouseMailBox; //0x5D90C and 0x5D915
    };

    struct RoomFlags {
        u8 BrightLight; //0x5D918 //or white light?
        u8 Unknown2; //0x5D919
        u8 RegularLight; //0x5D91A //or yellow light?
        u8 Unknown4; //0x5D91B
        u8 Unknown5; //0x5D91C //lighting related
        u8 Unknown6; //0x5D91D
        u8 Unknown7; //0x5D91E
        u8 Unknown8; //0x5D91F
        u8 Unknown9; //0x5D920
        u8 Unknown10; //0x5D921
        u8 Unknown11; //0x5D922
        u8 Unknown12; //0x5D923
        u8 Unknown13; //0x5D924
        u8 Unknown14; //0x5D925
        u8 LowLight; //0x5D926 //dim light?
        u8 Unknown16; //0x5D927
        u8 Unknown17; //0x5D928 //lighting related
        u8 Unknown18; //0x5D929
        u8 Unknown19; //0x5D92A
        u8 Unknown20; //0x5D92B
        u8 Unknown21; //0x5D92C
        u8 Unknown22; //0x5D92D
        u8 LightSwitchState; //0x5D92E //0 = Light OFF; 1 = Light ON
        u8 Unknown24; //0x5D92F
        u8 Unknown25; //0x5D930 //How often you went into the middle room???
        u8 Unknown26; //0x5D931
        u8 Unknown27; //0x5D932 //lighting related
        u8 Unknown28; //0x5D933 //lighting related
        u8 Unknown29; //0x5D934 //lighting related
        u8 Unknown30; //0x5D935
        u8 RoomSize; //0x5D936
        bool IsRoomUpgrading; //0x5D937
        u8 Unknown33; //0x5D938
        u8 Unknown34; //0x5D939
    };

    struct ACNL_Room { //size = 0x302
        RoomFlags flags; //0x5D918
        ACNL_Room_Active_Struct RoomItemsActiveStates; //0x5D93A
        ACNL_Room_Active_Struct RoomItemsPlacedOnOtherRoomItemsActiveStates; //0x5D95A
        Item RoomItems[0x64]; //0x5D97A
        Item RoomItemsPlacedOnOtherRoomItems[0x40]; //0x5DB0A
        Item Wallpaper;
        Item Flooring;
        Item PlayingSong;
        Item UnkItem4;
    };

    struct ACNL_CockroachData {
        u16 CockroachAmount;
        u8 Unk;
        u8 Padding;
    };

    struct ACNL_PlayerHouse {
        u32 ZeroPad1; //0x5D900
        PlayerHouseExterior exterior1; //0x5D904
        PlayerHouseExterior exterior2; //maybe if you are upgrading? //0x5D90D
        u16 Padding1; //0x5D916
        ACNL_Room MiddleRoom; //0x5D918
        ACNL_Room SecondRoom; //0x5DC1A
        ACNL_Room BasementRoom; //0x5DF1C
        ACNL_Room RightRoom; //0x5E21E
        ACNL_Room LeftRoom; //0x5E520
        ACNL_Room BackRoom; //0x5E822
        ACNL_CockroachData CockroachData; //0x5EB24
    };

    struct ACNL_MuseumExhibit { //size = 0xB98
        u8 PlayerIndex; //0 //ctor sets to 0xFF
        u8 Padding; //1
        ACNL_Room MuseumRoom; //2
        ACNL_Pattern ExhibitPoster; //0x304
        wchar ExhibitTitle[0x11]; //0xB74
        u16 Padding2; //0xB96
    };

    struct ACNL_NewPWP {//size = 0x10
        u8 Unk1; //ctor sets to 0
        u8 Unk2; //ctor sets to 0
        u8 Unk3; //ctor sets to 0
        u8 Padding;
        u32 Unk4; //Money accumulated???
        u32 Unk5; //Total cost amount???
        ACNL_Building Building;
    };

    struct ACNL_BulletinBoardMessage { //size = 0x1AC
        wchar PlayerName[9]; //Only set when player writes the message
        wchar TownName[9]; //Only set when player from other town writes the message
        wchar Message[0xC1];
        ACNL_Date MessageDate;
        u16 UsedFlag; //if message is written
    };

    struct Town_Flags {
        u8 Unknown1 : 1;
        u8 EarlyBirdOrdinance : 1;
        u8 NightOwlOrdinance  : 1;
        u8 BellBoomOrdinance  : 1;
        u8 BeautifulOrdinance : 1;
        u8 Unknown6 : 1;
        u8 Unknown7 : 1;
        u8 Unknown8 : 1;
        u8 Unknown9 : 1;
        u8 Unknown10 : 1;
        u8 Unknown11 : 1; //gulliver woken up?
        u8 Unknown12 : 1; //second player created?
        u8 Unknown13 : 1;
        u8 Unknown14 : 1;
        u8 Unknown15 : 1;
        u8 Unknown16 : 1;
        u8 Unknown17 : 1;
        u8 Unknown18 : 1;
        u8 Unknown19 : 1;
        u8 Unknown20 : 1;
        u8 Unknown21 : 1;
        u8 Unknown22 : 1;
        u8 Unknown23 : 1;
        u8 Unknown24 : 1;
        u8 Unknown25 : 1;
        u8 Unknown26 : 1;
        u8 Unknown27 : 1;
        u8 Unknown28 : 1;
        u8 Unknown29 : 1;
        u8 Unknown30 : 1;
        u8 Unknown31 : 1;
        u8 Unknown32 : 1;
        u8 Unknown33 : 1;
        u8 Unknown34 : 1;
        u8 Unknown35 : 1;
        u8 Unknown36 : 1;
        u8 Unknown37 : 1;
        u8 Unknown38 : 1;
        u8 Unknown39 : 1; //ctor sets this bit
        u8 Unknown40 : 1;
        u8 Unknown41 : 1;
        u8 Unknown42 : 1;
        u8 Unknown43 : 1;
        u8 Unknown44 : 1;
        u8 Unknown45 : 1;
        u8 Unknown46 : 1;
        u8 Unknown47 : 1;
        u8 Unknown48 : 1;
        u8 Unknown49 : 1;
        u8 Unknown50 : 1;
        u8 Unknown51 : 1;
        u8 Unknown52 : 1;
        u8 Unknown53 : 1;
        u8 Unknown54 : 1;
        u8 Unknown55 : 1;
        u8 QRMachineUnlocked : 1;
        u8 Unknown57 : 1;
        u8 Unknown58 : 1;
        u8 Unknown59 : 1;
        u8 Unknown60 : 1;
        u8 Unknown61 : 1;
        u8 Unknown62 : 1;
        u8 Unknown63 : 1;
        u8 Unknown64 : 1;
        u8 Unknown65 : 1;
        u8 Unknown66 : 1;
        u8 Unknown67 : 1;
        u8 Unknown68 : 1;
        u8 Unknown69 : 1;
        u8 Unknown70 : 1;
        u8 Unknown71 : 1;
        u8 Unknown72 : 1; //gets set when you talked to isabelle after she wants to build the dream suite
        u8 Unknown73 : 1; //ctor sets this bit
        u8 Unknown74 : 1; //ctor sets this bit
        u8 Unknown75 : 1; //ctor sets this bit
        u8 Unknown76 : 1;
        u8 Unknown77 : 1;
        u8 Unknown78 : 1;
        u8 Unknown79 : 1;
        u8 Unknown80 : 1;
        u8 Unknown81 : 1;
        u8 Unknown82 : 1;
        u8 Unknown83 : 1;
        u8 Unknown84 : 1;
        u8 Unknown85 : 1;
        u8 Unknown86 : 1;
        u8 Unknown87 : 1; //someone new moved in, isabelle mentions it, then its set to 0 again
        u8 OpenPublicWorkProject : 1;
        u8 Unknown89 : 1;
        u8 Unknown90 : 1;
        u8 Unknown91 : 1;
        u8 Unknown92 : 1;
        u8 Unknown93 : 1;
        u8 Unknown94 : 1;
        u8 Unknown95 : 1;
        u8 Unknown96 : 1;
        u8 Unknown97 : 1;
        u8 Unknown98 : 1;
        u8 Unknown99 : 1;
        u8 Unknown100 : 1;
        u8 Unknown101 : 1;
        u8 Unknown102 : 1;
        u8 Unknown103 : 1;
        u8 Unknown104 : 1;
        u8 HHSStandUnlocked : 1; //lyles stand in tom nooks shop
        u8 Unknown106 : 1;
        u8 Unknown107 : 1;
        u8 Unknown108 : 1;
        u8 Unknown109 : 1;
        u8 Unknown110 : 1;
        u8 Unknown111 : 1; //found magic lamp?
        u8 Unknown112 : 1;
        u8 Unknown113 : 1;
        u8 Unknown114 : 1;
        u8 Unknown115 : 1;
        u8 Unknown116 : 1;
        u8 Unknown117 : 1;
        u8 Unknown118 : 1;
        u8 Unknown119 : 1;
        u8 Unknown120 : 1; //this might be related to the last census menu entry
        u8 Unknown121 : 1; //this might be related to the last census menu entry
        u8 Unknown122 : 1;
        u8 Unknown123 : 1;
        u8 Unknown124 : 1;
        u8 Unknown125 : 1; //its snowing/raining?
        u8 Unknown126 : 1;
        u8 Unknown127 : 1;
        u8 Unknown128 : 1;
        u8 Unknown129 : 1;
        u8 Unknown130 : 1;
        u8 Unknown131 : 1;
        u8 Unused1 : 1;
        u8 Unused2 : 1;
        u8 Unused3 : 1;
        u8 Unused4 : 1;
    };

    struct Future_Letter { //Size: 0x288
        ACNL_Letter LetterForFutureSelf;
        u64 ReceiveDateInNanoSeconds; //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
    };

    struct Player_Letters { //Size: 0x1B88
        ACNL_Letter letters[10]; //0x73958 MailBox
        Future_Letter futureLetter; //0x75258
    };

    struct Player_Secret_Storage { //Size: 0x5A0
        Item StorageItems[360];
    };

    struct ACNL_TownData {
        u32 Checksum; //0x53424; Checksum of the 0x1E4D8 of this data
        u8 Unknown1[88];
        u8 OceanSide; //[0-1]; 0: Left, 1: Right; Museum and Camping Ground and the starting position of the train driving by are all on the opposite side to this.; Ctor sets to 3
        u8 TownGrassType; //[0-2]; 0: Triangle / Square (Winter) | 1: Circle / Star (Winter) | 2: Square / Circle (Winter); Ctor sets to 2
        u8 CliffType; //[0-2]; This is unused/scrapped. All 3 cliff texture in the ROM are the exact same.; Ctor sets to 2
        u8 Padding1;
        u16 TownAcres[7*6]; //42 acres in total; 7 colunms, 6 rows. Game reads Acre IDs as u16; ctor sets each to 265 (0x109)
        Item TownItems[(16*16)*(5*4)]; //16*16 items per acre; Items only cover map acres (5*4); 0x1400 of items
        u8 MapGrassToday[(16*16)*(5*4)]; //0x584d8 //16*16 slots per acre; Grass deterioration only affects map acres (5*4); 0x1400 of grass
        u8 Unused2[40]; //Town data ctor never initializes this, so likely not used. Where the code would branch to do so, there is a NOP (both in WA and Orig).
        u8 MapGrass[(16*16)*(8*6)]; //0x59900
        u8 Unused3[0x1000]; //Town data actually includes it with MapGrass (0x8000 in total), despite this portion not being used
        ACNL_PlayerHouse PlayerHouse[4]; //0x5D900
        s64 CurrentTime; //0x621A0
        s64 Unknown2;
        s64 Playtime; //0x621B0
        TownID TownData1;
        u8 Unknown3;
        Town_Flags TownFlags; //0x621CF
        u16 Unknown4; //Likely padding+ //0x621E0
        Item LostAndFoundItems[16]; //0x621E2
        u8 Unknown5[16]; //0x62222; likely padding
        u64 Unknown6; //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        Item TownFruit; //Yes, the game uses an item id to represent the town fruit lol
        u16 DaysPlayed; //0x6223E
        u8 Unknown7[0xC]; //0x62240
        u8 Unknown8; //0x6224C //Group 1
        u8 Unknown9; //0x6224D //Group 1
        u8 Unknown10; //0x6224E //Group 1
        u8 Padding2; //0x6224F
        u64 Unknown11; //0x62250 //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u64 Unknown12; //0x62258 //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u8 Unknown13[4]; //0x62260
        u8 NooklingState; //level of nookling shop
        u8 NooklingStateUnknown;
        u8 Unknown14[2];
        u64 NooklingBellsSpent; //encrypted value
        Item NooklingsItems[24];
        u8 Unknown15[28]; //likely padding
        Item AblesItems[5];
        Item AblesPatternItems[8];
        u64 Unknown16; //encrypted value
        u8 Unknown17[0x10]; //likely padding
        ACNL_Pattern AbleDisplayPattern[8];
        Item LabellesItems[7]; //Accessories in right of shop
        u8 Unknown18[8]; //likely padding
        u64 Unknown19; //encrypted value
        wchar ScrappedString[4];
        u64 Unknown20; //encrypted value
        u8 LeifUnlockStatus; //0 = locked; 1 = Being Built; X = Levels
        u8 Unknown21; 
        Item LeifItems[11];
        u8 Unknown22[14]; //likely padding
        Item ReddItems[4];
        u8 Unknown23[4]; //likely padding
        PersonalID UnknownPID1[4]; //unused?
        u64 Unknown24; //encrypted value
        u64 Unknown25; //encrypted value
        u64 Unknown26; //encrypted value
        u64 Unknown27; //encrypted value
        u8 Unknown28[8];
        u64 Unknown29; //encrypted value
        u8 KickUnlockStatus; //0 = locked; 1 = Being Built; 2 = Built/Unlocked
        u8 Padding4;
        Item KicksItems[6];
        u8 Unknown31[6];
        Item UnkItems1[4]; //ctor does this and below seperately, 4 at a time
        Item UnkItems2[4];
        u32 Padding5;
        u64 Unknown32; //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u64 Unknown33; //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u64 Unknown34; //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u64 Unknown35; //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u8 Unknown36[0x10]; //likely padding
        Item UnkItems3[5];
        Item UnkItems4[8];
        u8 Unknown37[8];
        u64 Unknown38; //encrypted value
        u64 Unknown39; //encrypted value
        u64 Unknown40; //encrypted value
        u64 Unknown41; //encrypted value
        u64 Unknown42; //encrypted value
        u64 Unknown43; //encrypted value
        u64 Unknown44; //encrypted value
        u64 Unknown45; //encrypted value
        ACNL_Pattern UnknownPattern[8];
        u64 Unknown46; //encrypted value
        u64 Unknown47; //encrypted value
        u64 Unknown48; //encrypted value
        u64 Unknown49; //encrypted value
        u8 MuseumShopUnlockState; //0x6ACBC
        u8 Unknown50;
        Item MuseumItems[3]; //0x6ACBE
        u8 Unknown51[4]; //likely padding
        Item NooksHomeItems[8];
        u8 Unknown52[8]; //likely padding
        u8 GracieUnlockStatus; //0 = locked??; 1 = Unlocked??
        u8 Padding6;
        Item GracieItems[18]; //0x6ACF8
        u8 Unknown53[18]; //likely padding
        Mannequin GracieMannequin1;
        Mannequin GracieMannequin2;
        u8 ClubLOLUnlockState; //0x6AD82
        u8 Unknown54[0xF];
        Item ClubLOLGyroids[4]; //0x6AD92
        u8 DreamSuiteUnlockStatus; //0x6ADA2
        u8 Unknown55; //0x6ADA3
        u8 FortuneTellerUnlockStatus; //0x6ADA4
        u8 Unknown56[15]; //0x6ADA5
        u8 ShampoodleUnlockStatus; //0x6ADB4
        u8 Unknown57[3];
        Item IslandShopItems[4];
        u64 Unknown58; //encrypted value
        u64 Unknown59[2]; //encrypted value
        u64 TurnipPrices[12]; //0x6ADE0; encrypted value; first 6 are AM, second 6 are PM
        u8 Unknown60[8]; 
        Item UnkItem6;
        Item UnkItems7[2]; //new year hats?
        Item CampgroundShopItems[2];
        u8 Unknown61[4];
        u16 CampgroundCaravan[2]; //left and right caravan (uses NPC VID); 0xFFFF if empty
        u8 Unknown62[8];
        //This is likely town tree stuff
        ACNL_Date Unknown63;
        ACNL_Date Unknown64; 
        u8 Unknown65[0x44];
    /*
        ACNL_Date FossilDonationDate[0x43]; //0x6AEB8
        ACNL_Date FishDonationDate[0x48]; //0x6AFC4
        ACNL_Date SeaCreatureDonationDate[0x1E]; //0x6B0E4
        ACNL_Date InsectDonationDate[0x48]; //0x6B15C
        ACNL_Date PaintingDonationDate[0x19]; //0x6B27C
        ACNL_Date StatueDonationDate[0x08]; //0x6B2E0
    */
        ACNL_Date MuseumDonationDates[0x112]; //0x6AEB8
        u8 MuseumDonations[0x112]; //0x6B300
        u8 Padding8[2]; //0x6B412
        ACNL_MuseumExhibit Exhibit[4]; //0x6B414
        u64 Unknown66; //0x6E274 //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u8 Unknown67[7]; //0x6E27C
        u64 Unknown68; //0x6E283 //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u8 Unknown69[7]; //0x6E28B
        u64 Unknown70; //0x6E292 //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u8 Unknown71[10]; //0x6E29A
        ACNL_NewPWP NextPWPToBuild; //0x6E2A4
        ACNL_Building UnknownBuilding1; //0x6E2B4
        ACNL_BulletinBoardMessage BBoardMessages[0xF]; //0x6E2B8
        u64 Unknown72; //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u8 Unknown73[0x14];
        u16 Unknown74;
        Item UnkItem9;
        u64 Unknown75; //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u64 Unknown76; //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        u64 Unknown77; //ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
        PersonalID UnknownPID2[3];
        u8 Unknown78[0x96];
        Item UnkItems10[3];
        u8 Unknown79[0x184]; //0x6FD32 //start of unknown
        u8 IslandGrassType; //[0-2]; 0: Triangle | 1: Circle | 2: Square;
        u8 Padding7;
        u16 IslandAcres[4*4]; //0x6FEB8; 16 acres in total; 4 colunms, 4 rows. Game reads Acre IDs as u16;
        Item IslandItems[(16*16)*(2*2)]; //0x6FED8; 16*16 items per acre; Items only cover map acres (2*2); 0x400 of items
        ACNL_Building IslandBuildings[2]; //Island Hut and Lloid
        ACNL_Pattern TownFlag; //0x70F1C
        u8 Unknown80[0x174]; //0x7178C
    };

    struct ACNL_Cenus_Data_Type { //Size: 0x14 
        u32 TotalPlayerStat; //All player stats combined
        u32 PlayerStats[4]; //For each player
    };

    /*
    In this case money values are actually not encrypted like in other parts of the save

    A few of these stats are not player relevant, those will only set the Stat at TotalPlayerStat, not at the playerstats
    They are marked with NON_PLAYER_SPECIFIC

    BlueRosesCreated is not tested, cause it sucks, but at 0x11570e the check actually checks for a blue rose creation
    HelpedKatie is also not tested, but the function enables her letter where you helped her

    Census Data is got through the US 1.5 address 0x2fd00c

    0x550 ACNL_Cenus_Data_Type Unknown2; //Indoor: 0xB09000 => 0xB09EF0 (B0C930 (6b8de0))
    Related to ACNL_VillagerData => 0x40598
    u8 Unknown1[0x154]; //0x40598
    u32 Unknown2; //0x406EC
    u8 Unknown3[0x22A]; //0x406F0

    0x564 ACNL_Cenus_Data_Type Unknown3; //Set at 6b8ec8
    Related to ACNL_VillagerData => 0x406EC
    u8 Unknown1[0x154]; //0x40598
    u32 Unknown2; //0x406EC
    u8 Unknown3[0x22A]; //0x406F0

    0x62C ACNL_Cenus_Data_Type Unused; //Village: 0xBEA000 => 0xBEA5C4 (BEA988 (6b7434)) //This sets the UNUSED ONE!!!

    //this sets the unused stat
    void FUN_0001399c(int param_1) {
        int iVar1;
        undefined4 uVar2;
        undefined4 uVar3;
        
        if (*(int *)(*(int *)(param_1 + 0x30) + 0x3b0) != -1) {
            iVar1 = FUN_002B8858();
            if (iVar1 != 0) {
                uVar2 = FUN_002B8820();
                uVar3 = FUN_002FD00C();
                FUN_006B7434(uVar3,uVar2);
                uVar2 = FUN_002B8890();
                uVar3 = FUN_002FD00C();
                FUN_006B8260(uVar3,uVar2);
                FUN_002B88C8();
            }
            FUN_0000e728(param_1,*DAT_00013a00,DAT_00013a00[1]);
        }
        
        DAT_00013a00 = 0xC0920C (BFD5D0) which is FUN_000135d0
        DAT_00013a00[1] = 0xC09210 (0)
    }

    void FUN_000135d0(int param_1) {
        int iVar1;
        int iVar2;
        undefined4 *puVar3;
        
        iVar1 = FUN_0000e548(*(undefined4 *)(param_1 + 0x40));
        if ((iVar1 == 0) && (iVar1 = FUN_0000f7e0(*(undefined4 *)(param_1 + 0x30)), iVar1 == 0)) {
            if (*(int *)(param_1 + 0x40) != 0) {
                FUN_0000e4ec();
                if (*(int **)(param_1 + 0x40) != (int *)0x0) {
                    (**(code **)(**(int **)(param_1 + 0x40) + 4))();
                }
                *(undefined4 *)(param_1 + 0x40) = 0;
            }

            if (*(int *)(param_1 + 0x30) != 0) {
                FUN_0000f198();
                if (*(int **)(param_1 + 0x30) != (int *)0x0) {
                    (**(code **)(**(int **)(param_1 + 0x30) + 4))();
                }
                *(undefined4 *)(param_1 + 0x30) = 0;
            }

            iVar2 = GetGardenSaveData();
            puVar3 = DAT_000136a4;
            iVar1 = DAT_000136a0;

            if ((*(char *)(iVar2 + DAT_000136a0 + 0x8e) < '\0') && (iVar2 = GetGardenSaveData(), (*(byte *)(iVar2 + iVar1 + 0x8f) & 1) == 0)) {
                puVar3 = puVar3 + 0x1e;
            }
            FUN_0000e728(param_1,*puVar3,puVar3[1]);
        }
    }

    KOKT63???
    KOKT66???
    */
    struct ACNL_Census_Player_Stats { //Size: 0x62C probably(?)
        ACNL_Cenus_Data_Type BellsEarned; //0x72510
        ACNL_Cenus_Data_Type ABDBalance; //0x72524
        ACNL_Cenus_Data_Type BellsSpent; //0x72538
        ACNL_Cenus_Data_Type LoanPaid; //0x7254C
        ACNL_Cenus_Data_Type TurnipsBought; //0x72560
        ACNL_Cenus_Data_Type TurnipsSold; //0x72574
        ACNL_Cenus_Data_Type TurnipExpenses; //0x72588
        ACNL_Cenus_Data_Type TurnipProfits; //0x7259C
        ACNL_Cenus_Data_Type PublicWorkExpenses; //0x725B0
        ACNL_Cenus_Data_Type RecycleShopEarnings; //0x725C4
        ACNL_Cenus_Data_Type FlowersWatered; //0x725D8
        ACNL_Cenus_Data_Type FishCaught; //0x725EC
        ACNL_Cenus_Data_Type BugsCaught; //0x72600
        ACNL_Cenus_Data_Type SeaCreaturesCaught; //0x72614
        ACNL_Cenus_Data_Type PublicWorksBuilt; //0x72628 //NON_PLAYER_SPECIFIC
        ACNL_Cenus_Data_Type FruitGrown; //0x7263C //NON_PLAYER_SPECIFIC
        ACNL_Cenus_Data_Type PerfectFruitGrown; //0x72650 //NON_PLAYER_SPECIFIC
        ACNL_Cenus_Data_Type FruitsGrownOnTheBeach; //0x72664 //NON_PLAYER_SPECIFIC
        ACNL_Cenus_Data_Type FloweresPlanted; //0x72678
        ACNL_Cenus_Data_Type TreesPlanted; //0x7268C
        ACNL_Cenus_Data_Type TreesCutDown; //0x726A0
        ACNL_Cenus_Data_Type DreamTownsVisited; //0x726B4
        ACNL_Cenus_Data_Type KKSliderConcertsAttended; //0x726C8
        ACNL_Cenus_Data_Type ShrunkSketchesAttended; //0x726DC
        ACNL_Cenus_Data_Type TourVisits; //0x726F0
        ACNL_Cenus_Data_Type IslandVisits; //0x72704
        ACNL_Cenus_Data_Type LettersSent; //0x72718
        ACNL_Cenus_Data_Type FurnitureCustomized; //0x7272C
        ACNL_Cenus_Data_Type NooklingsExpenses; //0x72740
        ACNL_Cenus_Data_Type GraciesExpenses; //0x72754
        ACNL_Cenus_Data_Type GardeningShopExpenses; //0x72768
        ACNL_Cenus_Data_Type WeedsPulled; //0x7277C
        ACNL_Cenus_Data_Type AbleSistersExpenses; //0x72790
        ACNL_Cenus_Data_Type KicksExpenses; //0x727A4
        ACNL_Cenus_Data_Type KatrinaVisits; //0x727B8
        ACNL_Cenus_Data_Type StreetPassVisitors; //0x727CC //NON_PLAYER_SPECIFIC
        ACNL_Cenus_Data_Type TownsVisited; //0x727E0
        ACNL_Cenus_Data_Type TownVisitors; //0x727F4
        ACNL_Cenus_Data_Type SaharaRedecorations; //0x72808
        ACNL_Cenus_Data_Type JobsAtTheRoost; //0x7281C
        ACNL_Cenus_Data_Type FossilsAnalyzed; //0x72830
        ACNL_Cenus_Data_Type ProDesignsCreated; //0x72844
        ACNL_Cenus_Data_Type EarnedBadges; //0x72858 //This one has no total amount
        ACNL_Cenus_Data_Type HelpedGulliver; //0x7286C
        ACNL_Cenus_Data_Type ShampoodleVisits; //0x72880
        ACNL_Cenus_Data_Type BoughtArtAtRedds; //0x72894
        ACNL_Cenus_Data_Type ScallopsGivenToPascal; //0x728A8
        ACNL_Cenus_Data_Type HelpedKatieTravel; //0x728BC
        ACNL_Cenus_Data_Type BalloonsPopped; //0x728D0
        ACNL_Cenus_Data_Type TourneyFishCaught; //0x728E4 //Get cleared after event
        ACNL_Cenus_Data_Type TourneyInsectCaught; //0x728F8 //Get cleared after event
        ACNL_Cenus_Data_Type FestivalFeathersCaught; //0x7290C
        ACNL_Cenus_Data_Type EggsGivenToZipper; //0x72920
        ACNL_Cenus_Data_Type FoundImposterBlanca; //0x72934
        ACNL_Cenus_Data_Type FireworkDesignsGivenToIsabelle; //0x72948 //will be set right after the fireworks started
        ACNL_Cenus_Data_Type ReceivedCandyOnHalloween; //0x7295C
        ACNL_Cenus_Data_Type HarvestFestivalCoursesDone; //0x72970
        ACNL_Cenus_Data_Type SnowmenBuiltByYouInTown; //0x72984
        ACNL_Cenus_Data_Type GivenToyDayPresents; //0x72998
        ACNL_Cenus_Data_Type SnowflakesCaught; //0x729AC
        ACNL_Cenus_Data_Type PartyPoppersPoppedAtNewYears; //0x729C0
        ACNL_Cenus_Data_Type TotalBingoWins; //0x729D4
        ACNL_Cenus_Data_Type ShootingStarWishes; //0x729E8
        ACNL_Cenus_Data_Type NPCPicturesReceived; //0x729FC
        ACNL_Cenus_Data_Type LostItemsGivenBack; //0x72A10
        ACNL_Cenus_Data_Type YouVisitedVillagers; //0x72A24
        ACNL_Cenus_Data_Type VillagersVisitedYou; //0x72A38
        ACNL_Cenus_Data_Type PlayedHideAndSeek; //0x72A4C
        /*0x550*/ACNL_Cenus_Data_Type Unknown1; //0x72A60 //KOKT71
        /*0x564*/ACNL_Cenus_Data_Type Unknown2; //0x72A74 //KOKT72
        ACNL_Cenus_Data_Type AmiiboScanned; //0x72A88
        ACNL_Cenus_Data_Type ResetSurveillanceCenterVisits; //0x72A9C
        ACNL_Cenus_Data_Type DJKKVisits; //0x72AB0
        ACNL_Cenus_Data_Type GoldenRosesCreated; //0x72AC4 //NON_PLAYER_SPECIFIC
        ACNL_Cenus_Data_Type BlueRosesCreated; //0x72AD8 //NON_PLAYER_SPECIFIC
        ACNL_Cenus_Data_Type FamousMushroomsEaten; //0x72AEC
        ACNL_Cenus_Data_Type TownInitiativesCompleted; //0x72B00
        ACNL_Cenus_Data_Type MEOWCouponsEarned; //0x72B14
        ACNL_Cenus_Data_Type MEOWCouponsSpent; //0x72B28
        /*0x62C*/ACNL_Cenus_Data_Type Unused; //0x72B3C //NON_PLAYER_SPECIFIC //KOKT82
        
        /*0x1180*/ //0x73690 //that one seems to be set in the game, meaning there are more???
        /*0x118C*/ //0x7369C
    };

    struct UnknownSharedData {
        u16 Unk0; // 0x00
        u16 Unk1; // 0x02
        u16 Unk2; // 0x04
    };

    /*
    int sub_2B8890()
    Maybe MEOW Coupons? One method checks them (u64 MeowCoupons Count)
    */
    struct UnknownSharedData2 { //Size: 
        u8 Unknown[0xC90]; //0x88D50
        UnknownSharedData SharedData[4];
    };

    struct ACNL_Player_SharedData { //Size: 0x181FF //Starts at 0x71900
        u32 Checksum; //0x71900
        u8 Unknown0[8]; //0x71904 //structure unsure
        u16 Unknown1; //0x7190C //ctor sets to 0
        u32 Unknown2; //0x7190E //ctor sets to 0
        u32 Unknown3; //0x71912 //ctor sets to 0
        u32 Unknown4; //0x71916 //ctor sets to 0
        u32 Unknown5; //0x7191A //ctor sets to 0
        u16 Unknown6; //0x7191C //ctor sets to 0
        u8 Unknown7; //0x7191E //ctor sets to 0
        u8 Unknown8; //0x7191F //ctor sets to 0
        u8 Unknown9[2]; //0x71920 //structure unsure
        u16 Unknown10; //0x71922
    
        u8 Unknown11[0xBEE]; //0x71924 //structure unsure

        ACNL_Census_Player_Stats PlayerStats; //0x72510 (Start + 0xC10)

        u8 Unknown12[0xE08]; //0x72B3C //structure unsure

        Player_Letters PlayerMailBoxLetters[4]; //0x73958 (Start + 0x2058)
        Player_Secret_Storage PlayerSecretStorages[4]; //0x7A778 (Start + 0x8e78)
        ACNL_Letter LetterPool[80]; //0x7BDF8 (Start + 0xa4f8) //any letter that isn't in your mailbox yet (every player shares this pool)
        //Next: 0x885F8

        //u32 Unknown; //0x16CF8
        //u32 Unknown; //0x16CFC
        //0x88608 (Start + 0x16d08) ???
        //0x887A8 (Start + 0x16ea8) ??? (size: 0x68?)

        UnknownSharedData2 UnknownData2; //0x88D50 (Start + 0x17450) ???

        u8 Unknown13[0x1507];
    };

    /*
    Snowman data probably lies in UnknownStruct1 & Unknown79 in TownData

    Snowman:
    Built:
    0x31F2FAA4 0x07E9000D (Old: 0x0000000D)
    0x31F2FAAC 0x07E9003C (Old: 0x07E9004C)
    0x31F7D894 0x00007FF8 (Old: 0x00007FFE)
    0x31F96DF8 0xCED7F0C0 (Old: 0xFFFFFFFF)
    0x31F96DFC 0x0B6708EB (Old: 0x7FFFFFFF)
    0x31F96E00 0x2F09FFFF (Old: 0x00000000)
    0x31F99904 0x00000001 (Old: 0x00000000)
    0x31F99908 0x00000001 (Old: 0x00000000)

    0x31f96e00 seems to store the number of the day and if it was set already
    */

    struct SecureValueHeader {
        u64 SecureValue; //0x0 //Unused in ACNL WA
        u32 SaveInitalised; //Has to be exactly 1
        u8 HeaderPadding[0x74]; //Always 0
    };

    struct ACNL_SaveHeader {
        u32 HeaderChecksum; //0x80 Checksum of the next 0x1C of header data
        u16 SaveVerifier1; //Always 0x009E; 0x00F8 pre-WA
        u8 SaveVerifier2; //Has to be exactly 0x2; 0x2 pre-WA
        u8 HeaderPadding[0x19]; //Always 0
    };

    struct Garden_Plus { //Size: 0x89B00
        SecureValueHeader SecureValue; //0x0
        ACNL_SaveHeader Header; //0x80
        ACNL_Player Player[4]; //0xA0
        ACNL_VillagerData VillagerData; //0x292A0
        ACNL_BuildingData BuildingData; //0x4BE80
        ACNL_MinigameData MinigameData; //WA exclusive
        ACNL_UnknownData UnkData; //WA exclusive
        ACNL_TownData TownData; //0x53424
        ACNL_Player_SharedData PlayerSharedData; //0x71900
        u8 padding; //0x89AFF
    };
}

#pragma pack(pop)
