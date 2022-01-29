#ifndef GAMESTRUCTS_HPP
#define GAMESTRUCTS_HPP

#include <CTRPluginFramework.hpp>
#include <optional>

#pragma pack(1)

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

    /*All Credits go to https://github.com/Slattz/ACNL_Research/blob/master/010%20Templates/garden_plus.dat.bt*/

    struct Item {
        u16 ID;
        u16 Flags;
    };

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

    struct TownID {
        u16 TID; //Default is 0
        u16/*wchar*/ DataTownName[9]; //Default is 0
        u8 Unknown01; //Default is 0xA
        u8 Unknown02;
    };

    struct PersonalID {
        u16 PID;
        u16/*wchar*/ PlayerName[9];
        u8 Gender;
        u8 ZeroPad;
        TownID TownData;
        u8 TPC_Country;
        u8 TPC_County;
    };

    struct ACNL_Pattern {
        u16/*wchar*/ Title[21];
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

    struct BDGValues {
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
    };

    struct PlayerBadges {
        BDGValues BadgeValues; //0x567C: 24 badges (encrypted values)
        BDG Badges; //0x573C: 24 badges
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
        u8  DCPart3;
        u16 Padding;
    };

    struct Player_Flags {
        u8 Unknown1 : 1;
        u8 Unknown2 : 1;
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
        u8 BlathersIntroduced : 1;
        u8 Unknown30 : 1;
        u8 Unknown31 : 1;
        u8 Unknown32 : 1;
        u8 Unknown33 : 1;
        u8 Unknown34 : 1;
        u8 Unknown35 : 1;
        u8 Unknown36 : 1;
        u8 Unknown37 : 1;
        u8 Unknown38 : 1;
        u8 Unknown39 : 1;
        u8 Unknown40 : 1;
        u8 KnowIsabelleName : 1;
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
        u8 Unknown56 : 1;
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
        u8 Unknown83 : 1;
        u8 Unknown84 : 1;
        u8 Unknown85 : 1;
        u8 Unknown86 : 1;
        u8 Unknown87 : 1;
        u8 Unknown88 : 1;
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
        u8 Unknown105 : 1;
        u8 Unknown106 : 1;
        u8 Unknown107 : 1;
        u8 Unknown108 : 1;
        u8 Unknown109 : 1;
        u8 Unknown110 : 1;
        u8 Unknown111 : 1;
        u8 Unknown112 : 1;
        u8 Unknown113 : 1;
        u8 Unknown114 : 1;
        u8 Unknown115 : 1;
        u8 Unknown116 : 1;
        u8 Unknown117 : 1;
        u8 Unknown118 : 1;
        u8 HasBeeSting : 1;
        u8 Unknown120 : 1;
        u8 Unknown121 : 1;
        u8 Unknown122 : 1;
        u8 Unknown123 : 1;
        u8 CanTravel : 1; //i.e. can use train
        u8 Unknown125 : 1;
        u8 Unknown126 : 1;
        u8 Unknown127 : 1;
        u8 Unknown128 : 1;
        u8 Unknown129 : 1;
        u8 Unknown130 : 1;
        u8 Unknown131 : 1;
        u8 Unknown132 : 1;
        u8 Unknown133 : 1;
        u8 Unknown134 : 1;
        u8 Unknown135 : 1;
        u8 Unknown136 : 1;
        u8 Unknown137 : 1;
        u8 Unknown138 : 1;
        u8 Unknown139 : 1;
        u8 Unknown140 : 1;
        u8 Unknown141 : 1;
        u8 Unknown142 : 1;
        u8 Unknown143 : 1;
        u8 Unknown144 : 1;
        u8 Unknown145 : 1;
        u8 Unknown146 : 1;
        u8 Unknown147 : 1;
        u8 Unknown148 : 1;
        u8 Unknown149 : 1;
        u8 Unknown150 : 1;
        u8 Unknown151 : 1;
        u8 Unknown152 : 1;
        u8 Unknown153 : 1;
        u8 Unknown154 : 1;
        u8 RecievedHHSIntro : 1;
        u8 Unknown156 : 1;
        u8 Unknown157 : 1;
        u8 Unknown158 : 1;
        u8 Unknown159 : 1;
        u8 Unknown160 : 1;
        u8 Unknown161 : 1;
        u8 Unknown162 : 1;
        u8 Unknown163 : 1;
        u8 Unknown164 : 1;
        u8 Unknown165 : 1;
        u8 Unknown166 : 1;
        u8 Unknown167 : 1;
        u8 Unknown168 : 1;
        u8 Unknown169 : 1;
        u8 Unknown170 : 1;
        u8 Unknown171 : 1;
        u8 Unknown172 : 1;
        u8 Unknown173 : 1;
        u8 Unknown174 : 1;
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
        u8 Unknown192 : 1;
        u8 Unknown193 : 1;
        u8 Unknown194 : 1;
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
        u8 Unknown208 : 1;
        u8 Unknown209 : 1;
        u8 Unknown210 : 1;
        u8 Unknown211 : 1;
        u8 Unknown212 : 1;
        u8 Unknown213 : 1;
        u8 Unknown214 : 1;
        u8 Unknown215 : 1;
        u8 Unknown216 : 1;
        u8 Unknown217 : 1;
        u8 Unknown218 : 1;
        u8 Unknown219 : 1;
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
        u8 Unknown242 : 1;
        u8 Unknown243 : 1;
        u8 Unknown244 : 1;
        u8 Unknown245 : 1;
        u8 Unknown246 : 1;
        u8 Unknown247 : 1;
        u8 Unknown248 : 1;
        u8 Unknown249 : 1;
        u8 Unknown250 : 1;
        u8 HasClubTortimerMembership : 1;
        u8 Unknown252 : 1;
        u8 Unknown253 : 1;
        u8 Unknown254 : 1;
        u8 Unknown255 : 1;
        u8 Unknown256 : 1;
        u8 Unknown257 : 1;
        u8 Unknown258 : 1;
        u8 Unknown259 : 1;
        u8 Unknown260 : 1;
        u8 Unknown261 : 1;
        u8 Unknown262 : 1;
        u8 Unknown263 : 1;
        u8 Unknown264 : 1;
        u8 Unknown265 : 1;
        u8 Unknown266 : 1;
        u8 Unknown267 : 1;
        u8 Unknown268 : 1;
        u8 Unknown269 : 1;
        u8 Unknown270 : 1;
        u8 Unknown271 : 1;
        u8 Unknown272 : 1;
        u8 Unknown273 : 1;
        u8 Unknown274 : 1;
        u8 Unknown275 : 1;
        u8 Unknown276 : 1;
        u8 Unknown277 : 1;
        u8 Unknown278 : 1;
        u8 Unknown279 : 1;
        u8 Unknown280 : 1;
        u8 Unknown281 : 1;
        u8 Unknown282 : 1;
        u8 Unknown283 : 1;
        u8 Unknown284 : 1;
        u8 Unknown285 : 1;
        u8 Unknown286 : 1;
        u8 Unknown287 : 1;
        u8 Unknown288 : 1;
        u8 Unknown289 : 1;
        u8 Unknown290 : 1;
        u8 Unknown291 : 1;
        u8 Unknown292 : 1;
        u8 Unknown293 : 1;
        u8 Unknown294 : 1;
        u8 Unknown295 : 1;
        u8 Unknown296 : 1;
        u8 Unknown297 : 1;
        u8 Unknown298 : 1;
        u8 Unknown299 : 1;
        u8 Unknown300 : 1;
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
        u8 Unknown314 : 1;
        u8 Unknown315 : 1;
        u8 Unknown316 : 1;
        u8 Unknown317 : 1;
        u8 Unknown318 : 1;
        u8 Unknown319 : 1;
        u8 Unknown320 : 1;
        u8 Unknown321 : 1;
        u8 Unknown322 : 1;
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
        u8 Unknown348 : 1;
        u8 Unknown349 : 1;
        u8 Unknown350 : 1;
        u8 Unknown351 : 1;
        u8 Unknown352 : 1;
        u8 Unknown353 : 1;
        u8 Unknown354 : 1;
        u8 Unknown355 : 1;
        u8 Unknown356 : 1;
        u8 Unknown357 : 1;
        u8 Unknown358 : 1;
        u8 Unknown359 : 1;
        u8 RecievedCATIntro : 1;
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
        u8 Unknown385 : 1;
        u8 Unknown386 : 1;
        u8 Unknown387 : 1;
        u8 Unknown388 : 1;
        u8 Unknown389 : 1;
        u8 Unknown390 : 1;
        u8 Unknown391 : 1;
        u8 Unknown392 : 1;
        u8 Unknown393 : 1;
        u8 Unknown394 : 1;
        u8 Unknown395 : 1;
        u8 Unknown396 : 1;
        u8 Unknown397 : 1;
        u8 Unknown398 : 1;
        u8 Unknown399 : 1;
        u8 Unknown400 : 1;
        u8 Unknown401 : 1;
        u8 Unknown402 : 1;
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

    struct ACNL_Letter {
        PersonalID Reciever;
        u16 ZeroPad_1;
        u16 RecieverID;
        u8 ZeroPad_2[50];
        u16 Unknown2; //Some form of ID?
        u16 ZeroPad_3;
        u16/*wchar*/ Header[32]; //Max amount is 32 UTF-16 characters
        u16 ZeroPad_4;
        u16/*wchar*/ Body[192]; //Max amount is 192 UTF-16 characters
        u16 ZeroPad_5;
        u16/*wchar*/ Signature[32]; //Max amount is 32 UTF-16 characters
        u16 ZeroPad_6;
        u8 RecieverNameIndent;
        u8 PaperID;
        u8 LetterFlag;
        u8 StringIDOfSender;
        u8 LetterType;
        u8 Unknown3;
        Item AttachedItem;
        u64  Unknown4;
    };

    struct Player_Letters {
        ACNL_Letter Letter1;
        ACNL_Letter Letter2;
        ACNL_Letter Letter3;
        ACNL_Letter Letter4;
        ACNL_Letter Letter5;
        ACNL_Letter Letter6;
        ACNL_Letter Letter7;
        ACNL_Letter Letter8;
        ACNL_Letter Letter9;
        ACNL_Letter Letter10;
    };

    struct UnknownStruct1 {
        u32 Unk1; //0 //Set to 0 in Player ctor
        u32 Unk2; //4 //Set to 0 in Player ctor
        u8 Unk3; //8 //Set to 0 in Player ctor
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
        u16 Unk32; //0x30 //Set to 0 in Player ctor
        u16 Unk33; //0x32 //Set to 0 in Player ctor
        u32 Unk34; //0x34 //Set to 0 in Player ctor
        u8 Unk35; //0x38 //Set to 0 in Player ctor
        u8 Unk36; //0x39 //Set to 0 in Player ctor
        u16 Unk37; //0x3A //Set to 0 in Player ctor
        u8 Unk38; //0x3C //Set to 0 in Player ctor
        u8 Unk39; //0x3D //Set to 0 in Player ctor
        u16 Unk40; //0x3E //Set to 0 in Player ctor
        u8 Unk41; //0x40 //Set to 0 in Player ctor
        u8 Unk42; //0x41 //Set to 1 in Player ctor
        u16 Unk43; //0x42 //Set to 0 in Player ctor
        u8 Unk44; //0x44 //Set to 0 in Player ctor
        u8 Unk45; //0x45 //Set to 0 in Player ctor
        u16 Unk46; //0x46 //Set to 0 in Player ctor
        u16 Unk47; //0x48 //Set to 0 in Player ctor
        u16 Unk48; //0x4A //Set to 0 in Player ctor
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
        u8 Unk72; //0x67 //Set to 0 in Player ctor
        u8 Unk73 ; //0x68 //Set to 0 in Player ctor
        u8 Unk74; //0x69 //Set to 0 in Player ctor
        TownID TownData; //0x6A -> 0x7F
    };

   struct UnknownStruct2 {
        u32 Unknown5; //0x8B80 //???; Set to 0, then 0xFFFFFFFF in player ctor
        u32 Unknown6; //0x8B84 //???; Set to 0, then 0x7FFFFFFF in player ctor
        u8 Unknown7; //0x8B88 //???; Set to 0 in player ctor
        u8 Unknown8; //0x8B89 //???; Set to 0 in player ctor
        u32 Unknown9; //0x8B8A //???; Set to 0 in player ctor
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

    struct UnknownStruct6Mini {
        TownID TownData1;
        TownID TownData2;
        u16 VillagerID; //This struct used in Villager ctor, and this is VID in it; Set to 0xFFFF in player ctor
        u8 VillagerPersonality; //This struct used in Villager ctor, and this is VP in it; Set to 0x8 in player ctor
        u8 Padding; //Padding: Not set in ctor
    };

    struct UnknownStruct6 {
        s64 Unknown1; //???; Set to 0, then 0x7FFFFFFFFFFFFFFF in player ctor
        u8 Unknown2; //???; Set to 0x21 in player ctor
        u8 Unknown3; //???; Set to 0xFF in player ctor
        u8 Unknown4; //???; Set to 0x9 in player ctor
        u8 Padding1; //Padding: Not set in ctor
        UnknownStruct6Mini Mini1;
        UnknownStruct6Mini Mini2;
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

    struct Player_Dressers {
        Item DrawerA[60];
        Item DrawerB[60];
        Item DrawerC[60];
    };

    struct MiiData1 {
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

    struct ACNL_Player { //UnknownNotSetYetX = Not set by player ctor
        u32 Checksum1; //0xA0 //Checksum of the first 0x6b84 of player data
        u8 HairStyle; //0xA4
        u8 HairColor; //0xA5 //Values: 0x0 -> 0xF
        u8 Face; //0xA6 //Values: 0x0 -> 0xB
        u8 EyeColor; //0xA7 //Values: 0x0 -> 0x5
        u16 Tan; //0xA8 //Values: 0x0 -> 0xF
        Item Hat; //0xAA //Item ID < 0xXXXX
        Item Accessory; //0xAE //Item ID < 0xXXXX
        Item TopWear; //0xB2 //Item ID < 0xXXXX
        Item UnderTopWear; //0xB6 //Item ID < 0xXXXX
        Item BottomWear; //0xBA //Item ID < 0xXXXX
        Item Socks; //0xBE //Item ID < 0xXXXX
        Item Shoes; //0xC2 //Item ID < 0xXXXX
        Item HeldItem; //0xC6 //Item ID < 0xXXXX
        u8 Unknown0; //0xCA //Inverted gender(?): 1 for male, 0 for female. Default = 1 in PlayerConstructor (EUR 1.5 0x20D27C)
        u8 Padding_0 = 0; //0xCB
        ACNL_Pattern Patterns[10]; //0xCC //10 Patterns
        u8 PatternOrder[10]; //0x552C -> 0x5535 //Order of patterns from 0x0 - 0x9
        u16 Padding_1; //0x5536 //U16 Zero Padding; Always 0x0000
        MiiData1 PlayerMii;
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
        u16/*wchar*/ TPCText[33]; //0x6BD8 -> 0x6C19
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
        Player_Letters Letters; //0x70A8 -> 0x89A7
        u16/*wchar*/ LetterHeader[0x20]; //0x89A8 -> 0x89E7
        u16 Padding_11; //0x89E8 -> 0x89E9
        u16/*wchar*/ FutureLetterHeader[0x20]; //0x89EA -> 0x8A29
        u16 Padding_12; //0x8A2A -> 0x8A2B    
        u16/*wchar*/ LetterSignature[0x20]; //0x8A2C -> 0x8A6B
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
        u8 GoldExteriorsUnlockedDupe; //0x8B60
        u8 HHAUnk1; //0x8B62
        u8 HHAUnk2; //0x8B63 //READU8(CurrentHouseTheme+1) | 0x80; Only When Exterior/Theme???
        u8 HHAUnk3[0x1C]; //0x8B63 -> //0x8B7F //Come back to later
        UnknownStruct2 UnkBuffer2; //0x8B80 -> 0x8DBB
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
        Player_Dressers Dressers; //Each dresser is 60 long
        u16/*wchar*/ BDayWish[0x22]; //0x9660 -> 0x96A4
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
}
#endif