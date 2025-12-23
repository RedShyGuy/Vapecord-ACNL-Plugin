#include "House/House.hpp"
#include "Address/Address.hpp"

namespace CTRPluginFramework {
    namespace House {
        /*
        I really want to improve this, but as it does work flawlessly, I will keep it for now like this
        */
        namespace Barrier {
            void UpgradeMainRoomBarrier0State(int playerIndex) {
                RoomData* roomData = GetRoomData(MAIN_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[674] = 0x4079; // base: 0x400F
                    roomData->collision[679] = 0x4079; // base: 0x400F
                    roomData->collision[684] = 0x4079; // base: 0x400F
                    roomData->collision[689] = 0x4079; // base: 0x400F
                    roomData->collision[749] = 0x407A; // base: 0x400F
                    roomData->collision[754] = 0x400E; // base: 0x402D
                    roomData->collision[759] = 0x400E; // base: 0x402D
                    roomData->collision[764] = 0x400E; // base: 0x402D
                    roomData->collision[769] = 0x400E; // base: 0x402D
                    roomData->collision[774] = 0x407B; // base: 0x400F
                    roomData->collision[829] = 0x407A; // base: 0x400F
                    roomData->collision[834] = 0x400E; // base: 0x402D
                    roomData->collision[839] = 0x400E; // base: 0x402D
                    roomData->collision[844] = 0x400E; // base: 0x402D
                    roomData->collision[849] = 0x400E; // base: 0x402D
                    roomData->collision[854] = 0x407B; // base: 0x400F
                    roomData->collision[909] = 0x407A; // base: 0x400F
                    roomData->collision[914] = 0x400E; // base: 0x402D
                    roomData->collision[919] = 0x400E; // base: 0x402D
                    roomData->collision[924] = 0x400E; // base: 0x402D
                    roomData->collision[929] = 0x400E; // base: 0x402D
                    roomData->collision[934] = 0x407B; // base: 0x400F
                    roomData->collision[989] = 0x407A; // base: 0x400F
                    roomData->collision[994] = 0x400E; // base: 0x402D
                    roomData->collision[999] = 0x400E; // base: 0x402D
                    roomData->collision[1004] = 0x400E; // base: 0x402D
                    roomData->collision[1009] = 0x400E; // base: 0x402D
                    roomData->collision[1014] = 0x407B; // base: 0x400F
                    roomData->collision[1074] = 0x4078; // base: 0x400F
                    roomData->collision[1079] = 0x407C; // base: 0x40F5
                    roomData->collision[1084] = 0x407C; // base: 0x40F5
                    roomData->collision[1089] = 0x4078; // base: 0x400F
                    roomData->collision[1159] = 0x40F4; // base: 0x40F5
                    roomData->collision[1164] = 0x40F4; // base: 0x40F5
                }
            }

            void UpgradeBarrierTo1State(u8 roomIdToUpgrade) {
                RoomData* roomData = GetRoomData(roomIdToUpgrade-1);
                if (roomData) {
                    roomData->collision[509] = 0x4079; // base: 0x400F
                    roomData->collision[514] = 0x4079; // base: 0x400F
                    roomData->collision[519] = 0x4079; // base: 0x400F
                    roomData->collision[524] = 0x4079; // base: 0x400F
                    roomData->collision[529] = 0x4079; // base: 0x400F
                    roomData->collision[534] = 0x4079; // base: 0x400F
                    roomData->collision[584] = 0x407A; // base: 0x400F
                    roomData->collision[585] = 0x0000; // base: 0x4210
                    roomData->collision[586] = 0x0000; // base: 0x4210
                    roomData->collision[587] = 0x0000; // base: 0x4210
                    roomData->collision[588] = 0x0000; // base: 0x4210
                    roomData->collision[589] = 0x400E; // base: 0x400F
                    roomData->collision[590] = 0x0000; // base: 0x4210
                    roomData->collision[591] = 0x0000; // base: 0x4210
                    roomData->collision[592] = 0x0000; // base: 0x4210
                    roomData->collision[593] = 0x0000; // base: 0x4210
                    roomData->collision[594] = 0x400E; // base: 0x400F
                    roomData->collision[595] = 0x0000; // base: 0x4210
                    roomData->collision[596] = 0x0000; // base: 0x4210
                    roomData->collision[597] = 0x0000; // base: 0x4210
                    roomData->collision[598] = 0x0000; // base: 0x4210
                    roomData->collision[599] = 0x400E; // base: 0x400F
                    roomData->collision[600] = 0x0000; // base: 0x4210
                    roomData->collision[601] = 0x0000; // base: 0x4210
                    roomData->collision[602] = 0x0000; // base: 0x4210
                    roomData->collision[603] = 0x0000; // base: 0x4210
                    roomData->collision[604] = 0x400E; // base: 0x400F
                    roomData->collision[605] = 0x0000; // base: 0x4210
                    roomData->collision[606] = 0x0000; // base: 0x4210
                    roomData->collision[607] = 0x0000; // base: 0x4210
                    roomData->collision[608] = 0x0000; // base: 0x4210
                    roomData->collision[609] = 0x400E; // base: 0x400F
                    roomData->collision[610] = 0x0000; // base: 0x4210
                    roomData->collision[611] = 0x0000; // base: 0x4210
                    roomData->collision[612] = 0x0000; // base: 0x4210
                    roomData->collision[613] = 0x0000; // base: 0x4210
                    roomData->collision[614] = 0x400E; // base: 0x400F
                    roomData->collision[619] = 0x407B; // base: 0x400F
                    roomData->collision[664] = 0x407A; // base: 0x400F
                    roomData->collision[665] = 0x0000; // base: 0x4210
                    roomData->collision[666] = 0x0000; // base: 0x4210
                    roomData->collision[667] = 0x0000; // base: 0x4210
                    roomData->collision[668] = 0x0000; // base: 0x4210
                    roomData->collision[669] = 0x400E; // base: 0x400F
                    roomData->collision[670] = 0x0000; // base: 0x4210
                    roomData->collision[671] = 0x0000; // base: 0x4210
                    roomData->collision[672] = 0x0000; // base: 0x4210
                    roomData->collision[673] = 0x0000; // base: 0x4210
                    roomData->collision[674] = 0x400E; // base: 0x4079
                    roomData->collision[675] = 0x0000; // base: 0x4210
                    roomData->collision[676] = 0x0000; // base: 0x4210
                    roomData->collision[677] = 0x0000; // base: 0x4210
                    roomData->collision[678] = 0x0000; // base: 0x4210
                    roomData->collision[679] = 0x400E; // base: 0x4079
                    roomData->collision[680] = 0x0000; // base: 0x4210
                    roomData->collision[681] = 0x0000; // base: 0x4210
                    roomData->collision[682] = 0x0000; // base: 0x4210
                    roomData->collision[683] = 0x0000; // base: 0x4210
                    roomData->collision[684] = 0x400E; // base: 0x4079
                    roomData->collision[685] = 0x0000; // base: 0x4210
                    roomData->collision[686] = 0x0000; // base: 0x4210
                    roomData->collision[687] = 0x0000; // base: 0x4210
                    roomData->collision[688] = 0x0000; // base: 0x4210
                    roomData->collision[689] = 0x400E; // base: 0x4079
                    roomData->collision[690] = 0x0000; // base: 0x4210
                    roomData->collision[691] = 0x0000; // base: 0x4210
                    roomData->collision[692] = 0x0000; // base: 0x4210
                    roomData->collision[693] = 0x0000; // base: 0x4210
                    roomData->collision[694] = 0x400E; // base: 0x400F
                    roomData->collision[699] = 0x407B; // base: 0x400F
                    roomData->collision[744] = 0x407A; // base: 0x400F
                    roomData->collision[745] = 0x0000; // base: 0x4210
                    roomData->collision[746] = 0x0000; // base: 0x4210
                    roomData->collision[747] = 0x0000; // base: 0x4210
                    roomData->collision[748] = 0x0000; // base: 0x4210
                    roomData->collision[749] = 0x400E; // base: 0x407A
                    roomData->collision[770] = 0x0000; // base: 0x4210
                    roomData->collision[771] = 0x0000; // base: 0x4210
                    roomData->collision[772] = 0x0000; // base: 0x4210
                    roomData->collision[773] = 0x0000; // base: 0x4210
                    roomData->collision[774] = 0x400E; // base: 0x407B
                    roomData->collision[779] = 0x407B; // base: 0x400F
                    roomData->collision[824] = 0x407A; // base: 0x400F
                    roomData->collision[825] = 0x0000; // base: 0x4210
                    roomData->collision[826] = 0x0000; // base: 0x4210
                    roomData->collision[827] = 0x0000; // base: 0x4210
                    roomData->collision[828] = 0x0000; // base: 0x4210
                    roomData->collision[829] = 0x400E; // base: 0x407A
                    roomData->collision[850] = 0x0000; // base: 0x4210
                    roomData->collision[851] = 0x0000; // base: 0x4210
                    roomData->collision[852] = 0x0000; // base: 0x4210
                    roomData->collision[853] = 0x0000; // base: 0x4210
                    roomData->collision[854] = 0x400E; // base: 0x407B
                    roomData->collision[859] = 0x407B; // base: 0x400F
                    roomData->collision[904] = 0x407A; // base: 0x400F
                    roomData->collision[905] = 0x0000; // base: 0x4210
                    roomData->collision[906] = 0x0000; // base: 0x4210
                    roomData->collision[907] = 0x0000; // base: 0x4210
                    roomData->collision[908] = 0x0000; // base: 0x4210
                    roomData->collision[909] = 0x400E; // base: 0x407A
                    roomData->collision[930] = 0x0000; // base: 0x4210
                    roomData->collision[931] = 0x0000; // base: 0x4210
                    roomData->collision[932] = 0x0000; // base: 0x4210
                    roomData->collision[933] = 0x0000; // base: 0x4210
                    roomData->collision[934] = 0x400E; // base: 0x407B
                    roomData->collision[939] = 0x407B; // base: 0x400F
                    roomData->collision[984] = 0x407A; // base: 0x400F
                    roomData->collision[985] = 0x0000; // base: 0x4210
                    roomData->collision[986] = 0x0000; // base: 0x4210
                    roomData->collision[987] = 0x0000; // base: 0x4210
                    roomData->collision[988] = 0x0000; // base: 0x4210
                    roomData->collision[989] = 0x400E; // base: 0x407A
                    roomData->collision[1010] = 0x0000; // base: 0x4210
                    roomData->collision[1011] = 0x0000; // base: 0x4210
                    roomData->collision[1012] = 0x0000; // base: 0x4210
                    roomData->collision[1013] = 0x0000; // base: 0x4210
                    roomData->collision[1014] = 0x400E; // base: 0x407B
                    roomData->collision[1019] = 0x407B; // base: 0x400F
                    roomData->collision[1069] = 0x4078; // base: 0x400F
                    roomData->collision[1094] = 0x4078; // base: 0x400F
                }
            }

            void UpgradeBarrierTo2State(u8 roomIdToUpgrade) {
                RoomData* roomData = GetRoomData(roomIdToUpgrade-1);
                if (roomData) {
                    roomData->collision[344] = 0x4079; // base: 0x400F
                    roomData->collision[349] = 0x4079; // base: 0x400F
                    roomData->collision[354] = 0x4079; // base: 0x400F
                    roomData->collision[359] = 0x4079; // base: 0x400F
                    roomData->collision[364] = 0x4079; // base: 0x400F
                    roomData->collision[369] = 0x4079; // base: 0x400F
                    roomData->collision[374] = 0x4079; // base: 0x400F
                    roomData->collision[379] = 0x4079; // base: 0x400F
                    roomData->collision[419] = 0x407A; // base: 0x400F
                    roomData->collision[420] = 0x0000; // base: 0x4210
                    roomData->collision[421] = 0x0000; // base: 0x4210
                    roomData->collision[422] = 0x0000; // base: 0x4210
                    roomData->collision[423] = 0x0000; // base: 0x4210
                    roomData->collision[424] = 0x400E; // base: 0x400F
                    roomData->collision[425] = 0x0000; // base: 0x4210
                    roomData->collision[426] = 0x0000; // base: 0x4210
                    roomData->collision[427] = 0x0000; // base: 0x4210
                    roomData->collision[428] = 0x0000; // base: 0x4210
                    roomData->collision[429] = 0x400E; // base: 0x400F
                    roomData->collision[430] = 0x0000; // base: 0x4210
                    roomData->collision[431] = 0x0000; // base: 0x4210
                    roomData->collision[432] = 0x0000; // base: 0x4210
                    roomData->collision[433] = 0x0000; // base: 0x4210
                    roomData->collision[434] = 0x400E; // base: 0x400F
                    roomData->collision[435] = 0x0000; // base: 0x4210
                    roomData->collision[436] = 0x0000; // base: 0x4210
                    roomData->collision[437] = 0x0000; // base: 0x4210
                    roomData->collision[438] = 0x0000; // base: 0x4210
                    roomData->collision[439] = 0x400E; // base: 0x400F
                    roomData->collision[440] = 0x0000; // base: 0x4210
                    roomData->collision[441] = 0x0000; // base: 0x4210
                    roomData->collision[442] = 0x0000; // base: 0x4210
                    roomData->collision[443] = 0x0000; // base: 0x4210
                    roomData->collision[444] = 0x400E; // base: 0x400F
                    roomData->collision[445] = 0x0000; // base: 0x4210
                    roomData->collision[446] = 0x0000; // base: 0x4210
                    roomData->collision[447] = 0x0000; // base: 0x4210
                    roomData->collision[448] = 0x0000; // base: 0x4210
                    roomData->collision[449] = 0x400E; // base: 0x400F
                    roomData->collision[450] = 0x0000; // base: 0x4210
                    roomData->collision[451] = 0x0000; // base: 0x4210
                    roomData->collision[452] = 0x0000; // base: 0x4210
                    roomData->collision[453] = 0x0000; // base: 0x4210
                    roomData->collision[454] = 0x400E; // base: 0x400F
                    roomData->collision[455] = 0x0000; // base: 0x4210
                    roomData->collision[456] = 0x0000; // base: 0x4210
                    roomData->collision[457] = 0x0000; // base: 0x4210
                    roomData->collision[458] = 0x0000; // base: 0x4210
                    roomData->collision[459] = 0x400E; // base: 0x400F
                    roomData->collision[464] = 0x407B; // base: 0x400F
                    roomData->collision[499] = 0x407A; // base: 0x400F
                    roomData->collision[500] = 0x0000; // base: 0x4210
                    roomData->collision[501] = 0x0000; // base: 0x4210
                    roomData->collision[502] = 0x0000; // base: 0x4210
                    roomData->collision[503] = 0x0000; // base: 0x4210
                    roomData->collision[504] = 0x400E; // base: 0x400F
                    roomData->collision[505] = 0x0000; // base: 0x4210
                    roomData->collision[506] = 0x0000; // base: 0x4210
                    roomData->collision[507] = 0x0000; // base: 0x4210
                    roomData->collision[508] = 0x0000; // base: 0x4210
                    roomData->collision[509] = 0x400E; // base: 0x4079
                    roomData->collision[510] = 0x0000; // base: 0x4210
                    roomData->collision[511] = 0x0000; // base: 0x4210
                    roomData->collision[512] = 0x0000; // base: 0x4210
                    roomData->collision[513] = 0x0000; // base: 0x4210
                    roomData->collision[514] = 0x400E; // base: 0x4079
                    roomData->collision[515] = 0x0000; // base: 0x4210
                    roomData->collision[516] = 0x0000; // base: 0x4210
                    roomData->collision[517] = 0x0000; // base: 0x4210
                    roomData->collision[518] = 0x0000; // base: 0x4210
                    roomData->collision[519] = 0x400E; // base: 0x4079
                    roomData->collision[520] = 0x0000; // base: 0x4210
                    roomData->collision[521] = 0x0000; // base: 0x4210
                    roomData->collision[522] = 0x0000; // base: 0x4210
                    roomData->collision[523] = 0x0000; // base: 0x4210
                    roomData->collision[524] = 0x400E; // base: 0x4079
                    roomData->collision[525] = 0x0000; // base: 0x4210
                    roomData->collision[526] = 0x0000; // base: 0x4210
                    roomData->collision[527] = 0x0000; // base: 0x4210
                    roomData->collision[528] = 0x0000; // base: 0x4210
                    roomData->collision[529] = 0x400E; // base: 0x4079
                    roomData->collision[530] = 0x0000; // base: 0x4210
                    roomData->collision[531] = 0x0000; // base: 0x4210
                    roomData->collision[532] = 0x0000; // base: 0x4210
                    roomData->collision[533] = 0x0000; // base: 0x4210
                    roomData->collision[534] = 0x400E; // base: 0x4079
                    roomData->collision[535] = 0x0000; // base: 0x4210
                    roomData->collision[536] = 0x0000; // base: 0x4210
                    roomData->collision[537] = 0x0000; // base: 0x4210
                    roomData->collision[538] = 0x0000; // base: 0x4210
                    roomData->collision[539] = 0x400E; // base: 0x400F
                    roomData->collision[544] = 0x407B; // base: 0x400F
                    roomData->collision[579] = 0x407A; // base: 0x400F
                    roomData->collision[580] = 0x0000; // base: 0x4210
                    roomData->collision[581] = 0x0000; // base: 0x4210
                    roomData->collision[582] = 0x0000; // base: 0x4210
                    roomData->collision[583] = 0x0000; // base: 0x4210
                    roomData->collision[584] = 0x400E; // base: 0x407A
                    roomData->collision[615] = 0x0000; // base: 0x4210
                    roomData->collision[616] = 0x0000; // base: 0x4210
                    roomData->collision[617] = 0x0000; // base: 0x4210
                    roomData->collision[618] = 0x0000; // base: 0x4210
                    roomData->collision[619] = 0x400E; // base: 0x407B
                    roomData->collision[624] = 0x407B; // base: 0x400F
                    roomData->collision[659] = 0x407A; // base: 0x400F
                    roomData->collision[660] = 0x0000; // base: 0x4210
                    roomData->collision[661] = 0x0000; // base: 0x4210
                    roomData->collision[662] = 0x0000; // base: 0x4210
                    roomData->collision[663] = 0x0000; // base: 0x4210
                    roomData->collision[664] = 0x400E; // base: 0x407A
                    roomData->collision[695] = 0x0000; // base: 0x4210
                    roomData->collision[696] = 0x0000; // base: 0x4210
                    roomData->collision[697] = 0x0000; // base: 0x4210
                    roomData->collision[698] = 0x0000; // base: 0x4210
                    roomData->collision[699] = 0x400E; // base: 0x407B
                    roomData->collision[704] = 0x407B; // base: 0x400F
                    roomData->collision[739] = 0x407A; // base: 0x400F
                    roomData->collision[740] = 0x0000; // base: 0x4210
                    roomData->collision[741] = 0x0000; // base: 0x4210
                    roomData->collision[742] = 0x0000; // base: 0x4210
                    roomData->collision[743] = 0x0000; // base: 0x4210
                    roomData->collision[744] = 0x400E; // base: 0x407A
                    roomData->collision[775] = 0x0000; // base: 0x4210
                    roomData->collision[776] = 0x0000; // base: 0x4210
                    roomData->collision[777] = 0x0000; // base: 0x4210
                    roomData->collision[778] = 0x0000; // base: 0x4210
                    roomData->collision[779] = 0x400E; // base: 0x407B
                    roomData->collision[784] = 0x407B; // base: 0x400F
                    roomData->collision[819] = 0x407A; // base: 0x400F
                    roomData->collision[820] = 0x0000; // base: 0x4210
                    roomData->collision[821] = 0x0000; // base: 0x4210
                    roomData->collision[822] = 0x0000; // base: 0x4210
                    roomData->collision[823] = 0x0000; // base: 0x4210
                    roomData->collision[824] = 0x400E; // base: 0x407A
                    roomData->collision[855] = 0x0000; // base: 0x4210
                    roomData->collision[856] = 0x0000; // base: 0x4210
                    roomData->collision[857] = 0x0000; // base: 0x4210
                    roomData->collision[858] = 0x0000; // base: 0x4210
                    roomData->collision[859] = 0x400E; // base: 0x407B
                    roomData->collision[864] = 0x407B; // base: 0x400F
                    roomData->collision[899] = 0x407A; // base: 0x400F
                    roomData->collision[900] = 0x0000; // base: 0x4210
                    roomData->collision[901] = 0x0000; // base: 0x4210
                    roomData->collision[902] = 0x0000; // base: 0x4210
                    roomData->collision[903] = 0x0000; // base: 0x4210
                    roomData->collision[904] = 0x400E; // base: 0x407A
                    roomData->collision[935] = 0x0000; // base: 0x4210
                    roomData->collision[936] = 0x0000; // base: 0x4210
                    roomData->collision[937] = 0x0000; // base: 0x4210
                    roomData->collision[938] = 0x0000; // base: 0x4210
                    roomData->collision[939] = 0x400E; // base: 0x407B
                    roomData->collision[944] = 0x407B; // base: 0x400F
                    roomData->collision[979] = 0x407A; // base: 0x400F
                    roomData->collision[980] = 0x0000; // base: 0x4210
                    roomData->collision[981] = 0x0000; // base: 0x4210
                    roomData->collision[982] = 0x0000; // base: 0x4210
                    roomData->collision[983] = 0x0000; // base: 0x4210
                    roomData->collision[984] = 0x400E; // base: 0x407A
                    roomData->collision[1015] = 0x0000; // base: 0x4210
                    roomData->collision[1016] = 0x0000; // base: 0x4210
                    roomData->collision[1017] = 0x0000; // base: 0x4210
                    roomData->collision[1018] = 0x0000; // base: 0x4210
                    roomData->collision[1019] = 0x400E; // base: 0x407B
                    roomData->collision[1024] = 0x407B; // base: 0x400F
                    roomData->collision[1064] = 0x4078; // base: 0x400F
                    roomData->collision[1099] = 0x4078; // base: 0x400F
                }
            }

            void UpgradeRightRoomBarrier1State(int playerIndex) {
                RoomData* roomData = GetRoomData(RIGHT_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[504] = 0x4079; // base: 0x400F
                    roomData->collision[509] = 0x4079; // base: 0x400F
                    roomData->collision[514] = 0x4079; // base: 0x400F
                    roomData->collision[519] = 0x4079; // base: 0x400F
                    roomData->collision[524] = 0x4079; // base: 0x400F
                    roomData->collision[529] = 0x4079; // base: 0x400F
                    roomData->collision[579] = 0x407A; // base: 0x400F
                    roomData->collision[580] = 0x0000; // base: 0x4210
                    roomData->collision[581] = 0x0000; // base: 0x4210
                    roomData->collision[582] = 0x0000; // base: 0x4210
                    roomData->collision[583] = 0x0000; // base: 0x4210
                    roomData->collision[584] = 0x400E; // base: 0x400F
                    roomData->collision[585] = 0x0000; // base: 0x4210
                    roomData->collision[586] = 0x0000; // base: 0x4210
                    roomData->collision[587] = 0x0000; // base: 0x4210
                    roomData->collision[588] = 0x0000; // base: 0x4210
                    roomData->collision[589] = 0x400E; // base: 0x400F
                    roomData->collision[590] = 0x0000; // base: 0x4210
                    roomData->collision[591] = 0x0000; // base: 0x4210
                    roomData->collision[592] = 0x0000; // base: 0x4210
                    roomData->collision[593] = 0x0000; // base: 0x4210
                    roomData->collision[594] = 0x400E; // base: 0x400F
                    roomData->collision[595] = 0x0000; // base: 0x4210
                    roomData->collision[596] = 0x0000; // base: 0x4210
                    roomData->collision[597] = 0x0000; // base: 0x4210
                    roomData->collision[598] = 0x0000; // base: 0x4210
                    roomData->collision[599] = 0x400E; // base: 0x400F
                    roomData->collision[600] = 0x0000; // base: 0x4210
                    roomData->collision[601] = 0x0000; // base: 0x4210
                    roomData->collision[602] = 0x0000; // base: 0x4210
                    roomData->collision[603] = 0x0000; // base: 0x4210
                    roomData->collision[604] = 0x400E; // base: 0x400F
                    roomData->collision[605] = 0x0000; // base: 0x4210
                    roomData->collision[606] = 0x0000; // base: 0x4210
                    roomData->collision[607] = 0x0000; // base: 0x4210
                    roomData->collision[608] = 0x0000; // base: 0x4210
                    roomData->collision[609] = 0x400E; // base: 0x400F
                    roomData->collision[614] = 0x407B; // base: 0x400F
                    roomData->collision[659] = 0x407A; // base: 0x400F
                    roomData->collision[660] = 0x0000; // base: 0x4210
                    roomData->collision[661] = 0x0000; // base: 0x4210
                    roomData->collision[662] = 0x0000; // base: 0x4210
                    roomData->collision[663] = 0x0000; // base: 0x4210
                    roomData->collision[664] = 0x400E; // base: 0x4079
                    roomData->collision[665] = 0x0000; // base: 0x4210
                    roomData->collision[666] = 0x0000; // base: 0x4210
                    roomData->collision[667] = 0x0000; // base: 0x4210
                    roomData->collision[668] = 0x0000; // base: 0x4210
                    roomData->collision[669] = 0x400E; // base: 0x4079
                    roomData->collision[670] = 0x0000; // base: 0x4210
                    roomData->collision[671] = 0x0000; // base: 0x4210
                    roomData->collision[672] = 0x0000; // base: 0x4210
                    roomData->collision[673] = 0x0000; // base: 0x4210
                    roomData->collision[674] = 0x400E; // base: 0x4079
                    roomData->collision[675] = 0x0000; // base: 0x4210
                    roomData->collision[676] = 0x0000; // base: 0x4210
                    roomData->collision[677] = 0x0000; // base: 0x4210
                    roomData->collision[678] = 0x0000; // base: 0x4210
                    roomData->collision[679] = 0x400E; // base: 0x4079
                    roomData->collision[680] = 0x0000; // base: 0x4210
                    roomData->collision[681] = 0x0000; // base: 0x4210
                    roomData->collision[682] = 0x0000; // base: 0x4210
                    roomData->collision[683] = 0x0000; // base: 0x4210
                    roomData->collision[684] = 0x400E; // base: 0x400F
                    roomData->collision[685] = 0x0000; // base: 0x4210
                    roomData->collision[686] = 0x0000; // base: 0x4210
                    roomData->collision[687] = 0x0000; // base: 0x4210
                    roomData->collision[688] = 0x0000; // base: 0x4210
                    roomData->collision[689] = 0x400E; // base: 0x400F
                    roomData->collision[694] = 0x407B; // base: 0x400F
                    roomData->collision[760] = 0x0000; // base: 0x4210
                    roomData->collision[761] = 0x0000; // base: 0x4210
                    roomData->collision[762] = 0x0000; // base: 0x4210
                    roomData->collision[763] = 0x0000; // base: 0x4210
                    roomData->collision[764] = 0x400E; // base: 0x407B
                    roomData->collision[765] = 0x0000; // base: 0x4210
                    roomData->collision[766] = 0x0000; // base: 0x4210
                    roomData->collision[767] = 0x0000; // base: 0x4210
                    roomData->collision[768] = 0x0000; // base: 0x4210
                    roomData->collision[769] = 0x400E; // base: 0x400F
                    roomData->collision[774] = 0x407B; // base: 0x400F
                    roomData->collision[840] = 0x0000; // base: 0x4210
                    roomData->collision[841] = 0x0000; // base: 0x4210
                    roomData->collision[842] = 0x0000; // base: 0x4210
                    roomData->collision[843] = 0x0000; // base: 0x4210
                    roomData->collision[844] = 0x400E; // base: 0x407B
                    roomData->collision[845] = 0x0000; // base: 0x4210
                    roomData->collision[846] = 0x0000; // base: 0x4210
                    roomData->collision[847] = 0x0000; // base: 0x4210
                    roomData->collision[848] = 0x0000; // base: 0x4210
                    roomData->collision[849] = 0x400E; // base: 0x400F
                    roomData->collision[854] = 0x407B; // base: 0x400F
                    roomData->collision[920] = 0x0000; // base: 0x4210
                    roomData->collision[921] = 0x0000; // base: 0x4210
                    roomData->collision[922] = 0x0000; // base: 0x4210
                    roomData->collision[923] = 0x0000; // base: 0x4210
                    roomData->collision[924] = 0x400E; // base: 0x407B
                    roomData->collision[925] = 0x0000; // base: 0x4210
                    roomData->collision[926] = 0x0000; // base: 0x4210
                    roomData->collision[927] = 0x0000; // base: 0x4210
                    roomData->collision[928] = 0x0000; // base: 0x4210
                    roomData->collision[929] = 0x400E; // base: 0x400F
                    roomData->collision[934] = 0x407B; // base: 0x400F
                    roomData->collision[1000] = 0x0000; // base: 0x4210
                    roomData->collision[1001] = 0x0000; // base: 0x4210
                    roomData->collision[1002] = 0x0000; // base: 0x4210
                    roomData->collision[1003] = 0x0000; // base: 0x4210
                    roomData->collision[1004] = 0x400E; // base: 0x407B
                    roomData->collision[1005] = 0x0000; // base: 0x4210
                    roomData->collision[1006] = 0x0000; // base: 0x4210
                    roomData->collision[1007] = 0x0000; // base: 0x4210
                    roomData->collision[1008] = 0x0000; // base: 0x4210
                    roomData->collision[1009] = 0x400E; // base: 0x400F
                    roomData->collision[1014] = 0x407B; // base: 0x400F
                    roomData->collision[1084] = 0x4078; // base: 0x400F
                    roomData->collision[1089] = 0x4078; // base: 0x400F
                }
            }

            void UpgradeRightRoomBarrier2State(int playerIndex) {
                RoomData* roomData = GetRoomData(RIGHT_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[344] = 0x4079; // base: 0x400F
                    roomData->collision[349] = 0x4079; // base: 0x400F
                    roomData->collision[354] = 0x4079; // base: 0x400F
                    roomData->collision[359] = 0x4079; // base: 0x400F
                    roomData->collision[364] = 0x4079; // base: 0x400F
                    roomData->collision[369] = 0x4079; // base: 0x400F
                    roomData->collision[374] = 0x4079; // base: 0x400F
                    roomData->collision[379] = 0x4079; // base: 0x400F
                    roomData->collision[419] = 0x407A; // base: 0x400F
                    roomData->collision[420] = 0x0000; // base: 0x4210
                    roomData->collision[421] = 0x0000; // base: 0x4210
                    roomData->collision[422] = 0x0000; // base: 0x4210
                    roomData->collision[423] = 0x0000; // base: 0x4210
                    roomData->collision[424] = 0x400E; // base: 0x400F
                    roomData->collision[425] = 0x0000; // base: 0x4210
                    roomData->collision[426] = 0x0000; // base: 0x4210
                    roomData->collision[427] = 0x0000; // base: 0x4210
                    roomData->collision[428] = 0x0000; // base: 0x4210
                    roomData->collision[429] = 0x400E; // base: 0x400F
                    roomData->collision[430] = 0x0000; // base: 0x4210
                    roomData->collision[431] = 0x0000; // base: 0x4210
                    roomData->collision[432] = 0x0000; // base: 0x4210
                    roomData->collision[433] = 0x0000; // base: 0x4210
                    roomData->collision[434] = 0x400E; // base: 0x400F
                    roomData->collision[435] = 0x0000; // base: 0x4210
                    roomData->collision[436] = 0x0000; // base: 0x4210
                    roomData->collision[437] = 0x0000; // base: 0x4210
                    roomData->collision[438] = 0x0000; // base: 0x4210
                    roomData->collision[439] = 0x400E; // base: 0x400F
                    roomData->collision[440] = 0x0000; // base: 0x4210
                    roomData->collision[441] = 0x0000; // base: 0x4210
                    roomData->collision[442] = 0x0000; // base: 0x4210
                    roomData->collision[443] = 0x0000; // base: 0x4210
                    roomData->collision[444] = 0x400E; // base: 0x400F
                    roomData->collision[445] = 0x0000; // base: 0x4210
                    roomData->collision[446] = 0x0000; // base: 0x4210
                    roomData->collision[447] = 0x0000; // base: 0x4210
                    roomData->collision[448] = 0x0000; // base: 0x4210
                    roomData->collision[449] = 0x400E; // base: 0x400F
                    roomData->collision[450] = 0x0000; // base: 0x4210
                    roomData->collision[451] = 0x0000; // base: 0x4210
                    roomData->collision[452] = 0x0000; // base: 0x4210
                    roomData->collision[453] = 0x0000; // base: 0x4210
                    roomData->collision[454] = 0x400E; // base: 0x400F
                    roomData->collision[455] = 0x0000; // base: 0x4210
                    roomData->collision[456] = 0x0000; // base: 0x4210
                    roomData->collision[457] = 0x0000; // base: 0x4210
                    roomData->collision[458] = 0x0000; // base: 0x4210
                    roomData->collision[459] = 0x400E; // base: 0x400F
                    roomData->collision[464] = 0x407B; // base: 0x400F
                    roomData->collision[499] = 0x407A; // base: 0x400F
                    roomData->collision[500] = 0x0000; // base: 0x4210
                    roomData->collision[501] = 0x0000; // base: 0x4210
                    roomData->collision[502] = 0x0000; // base: 0x4210
                    roomData->collision[503] = 0x0000; // base: 0x4210
                    roomData->collision[504] = 0x400E; // base: 0x4079
                    roomData->collision[505] = 0x0000; // base: 0x4210
                    roomData->collision[506] = 0x0000; // base: 0x4210
                    roomData->collision[507] = 0x0000; // base: 0x4210
                    roomData->collision[508] = 0x0000; // base: 0x4210
                    roomData->collision[509] = 0x400E; // base: 0x4079
                    roomData->collision[510] = 0x0000; // base: 0x4210
                    roomData->collision[511] = 0x0000; // base: 0x4210
                    roomData->collision[512] = 0x0000; // base: 0x4210
                    roomData->collision[513] = 0x0000; // base: 0x4210
                    roomData->collision[514] = 0x400E; // base: 0x4079
                    roomData->collision[515] = 0x0000; // base: 0x4210
                    roomData->collision[516] = 0x0000; // base: 0x4210
                    roomData->collision[517] = 0x0000; // base: 0x4210
                    roomData->collision[518] = 0x0000; // base: 0x4210
                    roomData->collision[519] = 0x400E; // base: 0x4079
                    roomData->collision[520] = 0x0000; // base: 0x4210
                    roomData->collision[521] = 0x0000; // base: 0x4210
                    roomData->collision[522] = 0x0000; // base: 0x4210
                    roomData->collision[523] = 0x0000; // base: 0x4210
                    roomData->collision[524] = 0x400E; // base: 0x4079
                    roomData->collision[525] = 0x0000; // base: 0x4210
                    roomData->collision[526] = 0x0000; // base: 0x4210
                    roomData->collision[527] = 0x0000; // base: 0x4210
                    roomData->collision[528] = 0x0000; // base: 0x4210
                    roomData->collision[529] = 0x400E; // base: 0x4079
                    roomData->collision[530] = 0x0000; // base: 0x4210
                    roomData->collision[531] = 0x0000; // base: 0x4210
                    roomData->collision[532] = 0x0000; // base: 0x4210
                    roomData->collision[533] = 0x0000; // base: 0x4210
                    roomData->collision[534] = 0x400E; // base: 0x400F
                    roomData->collision[535] = 0x0000; // base: 0x4210
                    roomData->collision[536] = 0x0000; // base: 0x4210
                    roomData->collision[537] = 0x0000; // base: 0x4210
                    roomData->collision[538] = 0x0000; // base: 0x4210
                    roomData->collision[539] = 0x400E; // base: 0x400F
                    roomData->collision[544] = 0x407B; // base: 0x400F
                    roomData->collision[610] = 0x0000; // base: 0x4210
                    roomData->collision[611] = 0x0000; // base: 0x4210
                    roomData->collision[612] = 0x0000; // base: 0x4210
                    roomData->collision[613] = 0x0000; // base: 0x4210
                    roomData->collision[614] = 0x400E; // base: 0x407B
                    roomData->collision[615] = 0x0000; // base: 0x4210
                    roomData->collision[616] = 0x0000; // base: 0x4210
                    roomData->collision[617] = 0x0000; // base: 0x4210
                    roomData->collision[618] = 0x0000; // base: 0x4210
                    roomData->collision[619] = 0x400E; // base: 0x400F
                    roomData->collision[624] = 0x407B; // base: 0x400F
                    roomData->collision[690] = 0x0000; // base: 0x4210
                    roomData->collision[691] = 0x0000; // base: 0x4210
                    roomData->collision[692] = 0x0000; // base: 0x4210
                    roomData->collision[693] = 0x0000; // base: 0x4210
                    roomData->collision[694] = 0x400E; // base: 0x407B
                    roomData->collision[695] = 0x0000; // base: 0x4210
                    roomData->collision[696] = 0x0000; // base: 0x4210
                    roomData->collision[697] = 0x0000; // base: 0x4210
                    roomData->collision[698] = 0x0000; // base: 0x4210
                    roomData->collision[699] = 0x400E; // base: 0x400F
                    roomData->collision[704] = 0x407B; // base: 0x400F
                    roomData->collision[770] = 0x0000; // base: 0x4210
                    roomData->collision[771] = 0x0000; // base: 0x4210
                    roomData->collision[772] = 0x0000; // base: 0x4210
                    roomData->collision[773] = 0x0000; // base: 0x4210
                    roomData->collision[774] = 0x400E; // base: 0x407B
                    roomData->collision[775] = 0x0000; // base: 0x4210
                    roomData->collision[776] = 0x0000; // base: 0x4210
                    roomData->collision[777] = 0x0000; // base: 0x4210
                    roomData->collision[778] = 0x0000; // base: 0x4210
                    roomData->collision[779] = 0x400E; // base: 0x400F
                    roomData->collision[784] = 0x407B; // base: 0x400F
                    roomData->collision[850] = 0x0000; // base: 0x4210
                    roomData->collision[851] = 0x0000; // base: 0x4210
                    roomData->collision[852] = 0x0000; // base: 0x4210
                    roomData->collision[853] = 0x0000; // base: 0x4210
                    roomData->collision[854] = 0x400E; // base: 0x407B
                    roomData->collision[855] = 0x0000; // base: 0x4210
                    roomData->collision[856] = 0x0000; // base: 0x4210
                    roomData->collision[857] = 0x0000; // base: 0x4210
                    roomData->collision[858] = 0x0000; // base: 0x4210
                    roomData->collision[859] = 0x400E; // base: 0x400F
                    roomData->collision[864] = 0x407B; // base: 0x400F
                    roomData->collision[930] = 0x0000; // base: 0x4210
                    roomData->collision[931] = 0x0000; // base: 0x4210
                    roomData->collision[932] = 0x0000; // base: 0x4210
                    roomData->collision[933] = 0x0000; // base: 0x4210
                    roomData->collision[934] = 0x400E; // base: 0x407B
                    roomData->collision[935] = 0x0000; // base: 0x4210
                    roomData->collision[936] = 0x0000; // base: 0x4210
                    roomData->collision[937] = 0x0000; // base: 0x4210
                    roomData->collision[938] = 0x0000; // base: 0x4210
                    roomData->collision[939] = 0x400E; // base: 0x400F
                    roomData->collision[944] = 0x407B; // base: 0x400F
                    roomData->collision[1010] = 0x0000; // base: 0x4210
                    roomData->collision[1011] = 0x0000; // base: 0x4210
                    roomData->collision[1012] = 0x0000; // base: 0x4210
                    roomData->collision[1013] = 0x0000; // base: 0x4210
                    roomData->collision[1014] = 0x400E; // base: 0x407B
                    roomData->collision[1015] = 0x0000; // base: 0x4210
                    roomData->collision[1016] = 0x0000; // base: 0x4210
                    roomData->collision[1017] = 0x0000; // base: 0x4210
                    roomData->collision[1018] = 0x0000; // base: 0x4210
                    roomData->collision[1019] = 0x400E; // base: 0x400F
                    roomData->collision[1024] = 0x407B; // base: 0x400F
                    roomData->collision[1094] = 0x4078; // base: 0x400F
                    roomData->collision[1099] = 0x4078; // base: 0x400F
                }
            }

            void UpgradeLeftRoomBarrier1State(int playerIndex) {
                RoomData* roomData = GetRoomData(LEFT_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[514] = 0x4079; // base: 0x400F
                    roomData->collision[519] = 0x4079; // base: 0x400F
                    roomData->collision[524] = 0x4079; // base: 0x400F
                    roomData->collision[529] = 0x4079; // base: 0x400F
                    roomData->collision[534] = 0x4079; // base: 0x400F
                    roomData->collision[539] = 0x4079; // base: 0x400F
                    roomData->collision[589] = 0x407A; // base: 0x400F
                    roomData->collision[590] = 0x0000; // base: 0x4210
                    roomData->collision[591] = 0x0000; // base: 0x4210
                    roomData->collision[592] = 0x0000; // base: 0x4210
                    roomData->collision[593] = 0x0000; // base: 0x4210
                    roomData->collision[594] = 0x400E; // base: 0x400F
                    roomData->collision[595] = 0x0000; // base: 0x4210
                    roomData->collision[596] = 0x0000; // base: 0x4210
                    roomData->collision[597] = 0x0000; // base: 0x4210
                    roomData->collision[598] = 0x0000; // base: 0x4210
                    roomData->collision[599] = 0x400E; // base: 0x400F
                    roomData->collision[600] = 0x0000; // base: 0x4210
                    roomData->collision[601] = 0x0000; // base: 0x4210
                    roomData->collision[602] = 0x0000; // base: 0x4210
                    roomData->collision[603] = 0x0000; // base: 0x4210
                    roomData->collision[604] = 0x400E; // base: 0x400F
                    roomData->collision[605] = 0x0000; // base: 0x4210
                    roomData->collision[606] = 0x0000; // base: 0x4210
                    roomData->collision[607] = 0x0000; // base: 0x4210
                    roomData->collision[608] = 0x0000; // base: 0x4210
                    roomData->collision[609] = 0x400E; // base: 0x400F
                    roomData->collision[610] = 0x0000; // base: 0x4210
                    roomData->collision[611] = 0x0000; // base: 0x4210
                    roomData->collision[612] = 0x0000; // base: 0x4210
                    roomData->collision[613] = 0x0000; // base: 0x4210
                    roomData->collision[614] = 0x400E; // base: 0x400F
                    roomData->collision[615] = 0x0000; // base: 0x4210
                    roomData->collision[616] = 0x0000; // base: 0x4210
                    roomData->collision[617] = 0x0000; // base: 0x4210
                    roomData->collision[618] = 0x0000; // base: 0x4210
                    roomData->collision[619] = 0x400E; // base: 0x400F
                    roomData->collision[624] = 0x407B; // base: 0x400F
                    roomData->collision[669] = 0x407A; // base: 0x400F
                    roomData->collision[670] = 0x0000; // base: 0x4210
                    roomData->collision[671] = 0x0000; // base: 0x4210
                    roomData->collision[672] = 0x0000; // base: 0x4210
                    roomData->collision[673] = 0x0000; // base: 0x4210
                    roomData->collision[674] = 0x400E; // base: 0x400F
                    roomData->collision[675] = 0x0000; // base: 0x4210
                    roomData->collision[676] = 0x0000; // base: 0x4210
                    roomData->collision[677] = 0x0000; // base: 0x4210
                    roomData->collision[678] = 0x0000; // base: 0x4210
                    roomData->collision[679] = 0x400E; // base: 0x400F
                    roomData->collision[680] = 0x0000; // base: 0x4210
                    roomData->collision[681] = 0x0000; // base: 0x4210
                    roomData->collision[682] = 0x0000; // base: 0x4210
                    roomData->collision[683] = 0x0000; // base: 0x4210
                    roomData->collision[684] = 0x400E; // base: 0x4079
                    roomData->collision[685] = 0x0000; // base: 0x4210
                    roomData->collision[686] = 0x0000; // base: 0x4210
                    roomData->collision[687] = 0x0000; // base: 0x4210
                    roomData->collision[688] = 0x0000; // base: 0x4210
                    roomData->collision[689] = 0x400E; // base: 0x4079
                    roomData->collision[690] = 0x0000; // base: 0x4210
                    roomData->collision[691] = 0x0000; // base: 0x4210
                    roomData->collision[692] = 0x0000; // base: 0x4210
                    roomData->collision[693] = 0x0000; // base: 0x4210
                    roomData->collision[694] = 0x400E; // base: 0x4079
                    roomData->collision[695] = 0x0000; // base: 0x4210
                    roomData->collision[696] = 0x0000; // base: 0x4210
                    roomData->collision[697] = 0x0000; // base: 0x4210
                    roomData->collision[698] = 0x0000; // base: 0x4210
                    roomData->collision[699] = 0x400E; // base: 0x4079
                    roomData->collision[704] = 0x407B; // base: 0x400F
                    roomData->collision[749] = 0x407A; // base: 0x400F
                    roomData->collision[750] = 0x0000; // base: 0x4210
                    roomData->collision[751] = 0x0000; // base: 0x4210
                    roomData->collision[752] = 0x0000; // base: 0x4210
                    roomData->collision[753] = 0x0000; // base: 0x4210
                    roomData->collision[754] = 0x400E; // base: 0x400F
                    roomData->collision[755] = 0x0000; // base: 0x4210
                    roomData->collision[756] = 0x0000; // base: 0x4210
                    roomData->collision[757] = 0x0000; // base: 0x4210
                    roomData->collision[758] = 0x0000; // base: 0x4210
                    roomData->collision[759] = 0x400E; // base: 0x407A
                    roomData->collision[829] = 0x407A; // base: 0x400F
                    roomData->collision[830] = 0x0000; // base: 0x4210
                    roomData->collision[831] = 0x0000; // base: 0x4210
                    roomData->collision[832] = 0x0000; // base: 0x4210
                    roomData->collision[833] = 0x0000; // base: 0x4210
                    roomData->collision[834] = 0x400E; // base: 0x400F
                    roomData->collision[835] = 0x0000; // base: 0x4210
                    roomData->collision[836] = 0x0000; // base: 0x4210
                    roomData->collision[837] = 0x0000; // base: 0x4210
                    roomData->collision[838] = 0x0000; // base: 0x4210
                    roomData->collision[839] = 0x400E; // base: 0x407A
                    roomData->collision[909] = 0x407A; // base: 0x400F
                    roomData->collision[910] = 0x0000; // base: 0x4210
                    roomData->collision[911] = 0x0000; // base: 0x4210
                    roomData->collision[912] = 0x0000; // base: 0x4210
                    roomData->collision[913] = 0x0000; // base: 0x4210
                    roomData->collision[914] = 0x400E; // base: 0x400F
                    roomData->collision[915] = 0x0000; // base: 0x4210
                    roomData->collision[916] = 0x0000; // base: 0x4210
                    roomData->collision[917] = 0x0000; // base: 0x4210
                    roomData->collision[918] = 0x0000; // base: 0x4210
                    roomData->collision[919] = 0x400E; // base: 0x407A
                    roomData->collision[989] = 0x407A; // base: 0x400F
                    roomData->collision[990] = 0x0000; // base: 0x4210
                    roomData->collision[991] = 0x0000; // base: 0x4210
                    roomData->collision[992] = 0x0000; // base: 0x4210
                    roomData->collision[993] = 0x0000; // base: 0x4210
                    roomData->collision[994] = 0x400E; // base: 0x400F
                    roomData->collision[995] = 0x0000; // base: 0x4210
                    roomData->collision[996] = 0x0000; // base: 0x4210
                    roomData->collision[997] = 0x0000; // base: 0x4210
                    roomData->collision[998] = 0x0000; // base: 0x4210
                    roomData->collision[999] = 0x400E; // base: 0x407A
                    roomData->collision[1074] = 0x4078; // base: 0x400F
                    roomData->collision[1079] = 0x4078; // base: 0x400F
                }
            }
        
            void UpgradeLeftRoomBarrier2State(int playerIndex) {
                RoomData* roomData = GetRoomData(LEFT_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[344] = 0x4079; // base: 0x400F
                    roomData->collision[349] = 0x4079; // base: 0x400F
                    roomData->collision[354] = 0x4079; // base: 0x400F
                    roomData->collision[359] = 0x4079; // base: 0x400F
                    roomData->collision[364] = 0x4079; // base: 0x400F
                    roomData->collision[369] = 0x4079; // base: 0x400F
                    roomData->collision[374] = 0x4079; // base: 0x400F
                    roomData->collision[379] = 0x4079; // base: 0x400F
                    roomData->collision[419] = 0x407A; // base: 0x400F
                    roomData->collision[420] = 0x0000; // base: 0x4210
                    roomData->collision[421] = 0x0000; // base: 0x4210
                    roomData->collision[422] = 0x0000; // base: 0x4210
                    roomData->collision[423] = 0x0000; // base: 0x4210
                    roomData->collision[424] = 0x400E; // base: 0x400F
                    roomData->collision[425] = 0x0000; // base: 0x4210
                    roomData->collision[426] = 0x0000; // base: 0x4210
                    roomData->collision[427] = 0x0000; // base: 0x4210
                    roomData->collision[428] = 0x0000; // base: 0x4210
                    roomData->collision[429] = 0x400E; // base: 0x400F
                    roomData->collision[430] = 0x0000; // base: 0x4210
                    roomData->collision[431] = 0x0000; // base: 0x4210
                    roomData->collision[432] = 0x0000; // base: 0x4210
                    roomData->collision[433] = 0x0000; // base: 0x4210
                    roomData->collision[434] = 0x400E; // base: 0x400F
                    roomData->collision[435] = 0x0000; // base: 0x4210
                    roomData->collision[436] = 0x0000; // base: 0x4210
                    roomData->collision[437] = 0x0000; // base: 0x4210
                    roomData->collision[438] = 0x0000; // base: 0x4210
                    roomData->collision[439] = 0x400E; // base: 0x400F
                    roomData->collision[440] = 0x0000; // base: 0x4210
                    roomData->collision[441] = 0x0000; // base: 0x4210
                    roomData->collision[442] = 0x0000; // base: 0x4210
                    roomData->collision[443] = 0x0000; // base: 0x4210
                    roomData->collision[444] = 0x400E; // base: 0x400F
                    roomData->collision[445] = 0x0000; // base: 0x4210
                    roomData->collision[446] = 0x0000; // base: 0x4210
                    roomData->collision[447] = 0x0000; // base: 0x4210
                    roomData->collision[448] = 0x0000; // base: 0x4210
                    roomData->collision[449] = 0x400E; // base: 0x400F
                    roomData->collision[450] = 0x0000; // base: 0x4210
                    roomData->collision[451] = 0x0000; // base: 0x4210
                    roomData->collision[452] = 0x0000; // base: 0x4210
                    roomData->collision[453] = 0x0000; // base: 0x4210
                    roomData->collision[454] = 0x400E; // base: 0x400F
                    roomData->collision[455] = 0x0000; // base: 0x4210
                    roomData->collision[456] = 0x0000; // base: 0x4210
                    roomData->collision[457] = 0x0000; // base: 0x4210
                    roomData->collision[458] = 0x0000; // base: 0x4210
                    roomData->collision[459] = 0x400E; // base: 0x400F
                    roomData->collision[464] = 0x407B; // base: 0x400F
                    roomData->collision[499] = 0x407A; // base: 0x400F
                    roomData->collision[500] = 0x0000; // base: 0x4210
                    roomData->collision[501] = 0x0000; // base: 0x4210
                    roomData->collision[502] = 0x0000; // base: 0x4210
                    roomData->collision[503] = 0x0000; // base: 0x4210
                    roomData->collision[504] = 0x400E; // base: 0x400F
                    roomData->collision[505] = 0x0000; // base: 0x4210
                    roomData->collision[506] = 0x0000; // base: 0x4210
                    roomData->collision[507] = 0x0000; // base: 0x4210
                    roomData->collision[508] = 0x0000; // base: 0x4210
                    roomData->collision[509] = 0x400E; // base: 0x400F
                    roomData->collision[510] = 0x0000; // base: 0x4210
                    roomData->collision[511] = 0x0000; // base: 0x4210
                    roomData->collision[512] = 0x0000; // base: 0x4210
                    roomData->collision[513] = 0x0000; // base: 0x4210
                    roomData->collision[514] = 0x400E; // base: 0x4079
                    roomData->collision[515] = 0x0000; // base: 0x4210
                    roomData->collision[516] = 0x0000; // base: 0x4210
                    roomData->collision[517] = 0x0000; // base: 0x4210
                    roomData->collision[518] = 0x0000; // base: 0x4210
                    roomData->collision[519] = 0x400E; // base: 0x4079
                    roomData->collision[520] = 0x0000; // base: 0x4210
                    roomData->collision[521] = 0x0000; // base: 0x4210
                    roomData->collision[522] = 0x0000; // base: 0x4210
                    roomData->collision[523] = 0x0000; // base: 0x4210
                    roomData->collision[524] = 0x400E; // base: 0x4079
                    roomData->collision[525] = 0x0000; // base: 0x4210
                    roomData->collision[526] = 0x0000; // base: 0x4210
                    roomData->collision[527] = 0x0000; // base: 0x4210
                    roomData->collision[528] = 0x0000; // base: 0x4210
                    roomData->collision[529] = 0x400E; // base: 0x4079
                    roomData->collision[530] = 0x0000; // base: 0x4210
                    roomData->collision[531] = 0x0000; // base: 0x4210
                    roomData->collision[532] = 0x0000; // base: 0x4210
                    roomData->collision[533] = 0x0000; // base: 0x4210
                    roomData->collision[534] = 0x400E; // base: 0x4079
                    roomData->collision[535] = 0x0000; // base: 0x4210
                    roomData->collision[536] = 0x0000; // base: 0x4210
                    roomData->collision[537] = 0x0000; // base: 0x4210
                    roomData->collision[538] = 0x0000; // base: 0x4210
                    roomData->collision[539] = 0x400E; // base: 0x4079
                    roomData->collision[544] = 0x407B; // base: 0x400F
                    roomData->collision[579] = 0x407A; // base: 0x400F
                    roomData->collision[580] = 0x0000; // base: 0x4210
                    roomData->collision[581] = 0x0000; // base: 0x4210
                    roomData->collision[582] = 0x0000; // base: 0x4210
                    roomData->collision[583] = 0x0000; // base: 0x4210
                    roomData->collision[584] = 0x400E; // base: 0x400F
                    roomData->collision[585] = 0x0000; // base: 0x4210
                    roomData->collision[586] = 0x0000; // base: 0x4210
                    roomData->collision[587] = 0x0000; // base: 0x4210
                    roomData->collision[588] = 0x0000; // base: 0x4210
                    roomData->collision[589] = 0x400E; // base: 0x407A
                    roomData->collision[659] = 0x407A; // base: 0x400F
                    roomData->collision[660] = 0x0000; // base: 0x4210
                    roomData->collision[661] = 0x0000; // base: 0x4210
                    roomData->collision[662] = 0x0000; // base: 0x4210
                    roomData->collision[663] = 0x0000; // base: 0x4210
                    roomData->collision[664] = 0x400E; // base: 0x400F
                    roomData->collision[665] = 0x0000; // base: 0x4210
                    roomData->collision[666] = 0x0000; // base: 0x4210
                    roomData->collision[667] = 0x0000; // base: 0x4210
                    roomData->collision[668] = 0x0000; // base: 0x4210
                    roomData->collision[669] = 0x400E; // base: 0x407A
                    roomData->collision[739] = 0x407A; // base: 0x400F
                    roomData->collision[740] = 0x0000; // base: 0x4210
                    roomData->collision[741] = 0x0000; // base: 0x4210
                    roomData->collision[742] = 0x0000; // base: 0x4210
                    roomData->collision[743] = 0x0000; // base: 0x4210
                    roomData->collision[744] = 0x400E; // base: 0x400F
                    roomData->collision[745] = 0x0000; // base: 0x4210
                    roomData->collision[746] = 0x0000; // base: 0x4210
                    roomData->collision[747] = 0x0000; // base: 0x4210
                    roomData->collision[748] = 0x0000; // base: 0x4210
                    roomData->collision[749] = 0x400E; // base: 0x407A
                    roomData->collision[819] = 0x407A; // base: 0x400F
                    roomData->collision[820] = 0x0000; // base: 0x4210
                    roomData->collision[821] = 0x0000; // base: 0x4210
                    roomData->collision[822] = 0x0000; // base: 0x4210
                    roomData->collision[823] = 0x0000; // base: 0x4210
                    roomData->collision[824] = 0x400E; // base: 0x400F
                    roomData->collision[825] = 0x0000; // base: 0x4210
                    roomData->collision[826] = 0x0000; // base: 0x4210
                    roomData->collision[827] = 0x0000; // base: 0x4210
                    roomData->collision[828] = 0x0000; // base: 0x4210
                    roomData->collision[829] = 0x400E; // base: 0x407A
                    roomData->collision[899] = 0x407A; // base: 0x400F
                    roomData->collision[900] = 0x0000; // base: 0x4210
                    roomData->collision[901] = 0x0000; // base: 0x4210
                    roomData->collision[902] = 0x0000; // base: 0x4210
                    roomData->collision[903] = 0x0000; // base: 0x4210
                    roomData->collision[904] = 0x400E; // base: 0x400F
                    roomData->collision[905] = 0x0000; // base: 0x4210
                    roomData->collision[906] = 0x0000; // base: 0x4210
                    roomData->collision[907] = 0x0000; // base: 0x4210
                    roomData->collision[908] = 0x0000; // base: 0x4210
                    roomData->collision[909] = 0x400E; // base: 0x407A
                    roomData->collision[979] = 0x407A; // base: 0x400F
                    roomData->collision[980] = 0x0000; // base: 0x4210
                    roomData->collision[981] = 0x0000; // base: 0x4210
                    roomData->collision[982] = 0x0000; // base: 0x4210
                    roomData->collision[983] = 0x0000; // base: 0x4210
                    roomData->collision[984] = 0x400E; // base: 0x400F
                    roomData->collision[985] = 0x0000; // base: 0x4210
                    roomData->collision[986] = 0x0000; // base: 0x4210
                    roomData->collision[987] = 0x0000; // base: 0x4210
                    roomData->collision[988] = 0x0000; // base: 0x4210
                    roomData->collision[989] = 0x400E; // base: 0x407A
                    roomData->collision[1064] = 0x4078; // base: 0x400F
                    roomData->collision[1069] = 0x4078; // base: 0x400F
                }
            }

            void UpgradeUpstairsBarrier1State(int playerIndex) {
                UpgradeBarrierTo1State(UPSTAIRS[playerIndex]);

                RoomData* roomData = GetRoomData(UPSTAIRS[playerIndex]-1);
                if (roomData) {
                    roomData->collision[1150] = 0x0000; // base: 0x4210
                    roomData->collision[1151] = 0x0000; // base: 0x4210
                    roomData->collision[1152] = 0x0000; // base: 0x4210
                    roomData->collision[1153] = 0x0000; // base: 0x4210
                    roomData->collision[1154] = 0x4007; // base: 0x400F
                    roomData->collision[1235] = 0x4210; // base: 0x0000
                    roomData->collision[1236] = 0x4210; // base: 0x0000
                    roomData->collision[1237] = 0x4210; // base: 0x0000
                    roomData->collision[1238] = 0x4210; // base: 0x0000
                    roomData->collision[1239] = 0x400F; // base: 0x4007
                    roomData->collision[1240] = 0x4210; // base: 0x0000
                    roomData->collision[1241] = 0x4210; // base: 0x0000
                    roomData->collision[1242] = 0x4210; // base: 0x0000
                    roomData->collision[1243] = 0x4210; // base: 0x0000
                    roomData->collision[1244] = 0x400F; // base: 0x4007
                }
            }

            void UpgradeUpstairsBarrier2State(int playerIndex) {
                UpgradeBarrierTo2State(UPSTAIRS[playerIndex]);
            }

            void UpgradeDownStairsBarrier1State(int playerIndex) {
                UpgradeBarrierTo1State(BASEMENT[playerIndex]);

                RoomData* roomData = GetRoomData(BASEMENT[playerIndex]-1);
                if (roomData) {
                    roomData->collision[1165] = 0x0000; // base: 0x4210
                    roomData->collision[1166] = 0x0000; // base: 0x4210
                    roomData->collision[1167] = 0x0000; // base: 0x4210
                    roomData->collision[1168] = 0x0000; // base: 0x4210
                    roomData->collision[1169] = 0x4007; // base: 0x400F
                    roomData->collision[1235] = 0x4210; // base: 0x0000
                    roomData->collision[1236] = 0x4210; // base: 0x0000
                    roomData->collision[1237] = 0x4210; // base: 0x0000
                    roomData->collision[1238] = 0x4210; // base: 0x0000
                    roomData->collision[1239] = 0x400F; // base: 0x4007
                    roomData->collision[1240] = 0x4210; // base: 0x0000
                    roomData->collision[1241] = 0x4210; // base: 0x0000
                    roomData->collision[1242] = 0x4210; // base: 0x0000
                    roomData->collision[1243] = 0x4210; // base: 0x0000
                    roomData->collision[1244] = 0x400F; // base: 0x4007
                }
            }

            void UpgradeDownStairsBarrier2State(int playerIndex) {
                UpgradeBarrierTo2State(BASEMENT[playerIndex]);
            }

            void UpgradeTopRoomBarrier1State(int playerIndex) {
                UpgradeBarrierTo1State(TOP_ROOM[playerIndex]);

                RoomData* roomData = GetRoomData(TOP_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[1159] = 0x4007; // base: 0x40F4
                    roomData->collision[1164] = 0x4007; // base: 0x40F4
                    roomData->collision[1235] = 0x4210; // base: 0x0000
                    roomData->collision[1236] = 0x4210; // base: 0x0000
                    roomData->collision[1237] = 0x4210; // base: 0x0000
                    roomData->collision[1238] = 0x4210; // base: 0x0000
                    roomData->collision[1239] = 0x400F; // base: 0x4007
                    roomData->collision[1240] = 0x4210; // base: 0x0000
                    roomData->collision[1241] = 0x4210; // base: 0x0000
                    roomData->collision[1242] = 0x4210; // base: 0x0000
                    roomData->collision[1243] = 0x4210; // base: 0x0000
                    roomData->collision[1244] = 0x400F; // base: 0x4007
                }
            }

            void UpgradeTopRoomBarrier2State(int playerIndex) {
                UpgradeBarrierTo2State(TOP_ROOM[playerIndex]);
            }

            void UpgradeMainRoomBarrier1State(int playerIndex) {
                UpgradeBarrierTo1State(MAIN_ROOM[playerIndex]);
            }

            void UpgradeMainRoomBarrier2State(int playerIndex) {
                UpgradeBarrierTo2State(MAIN_ROOM[playerIndex]);
            }

            void UpgradeBarrierUpstairs(int playerIndex) {
                RoomData* roomData = GetRoomData(MAIN_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[1150] = 0x0000; // base: 0x4210
                    roomData->collision[1151] = 0x0000; // base: 0x4210
                    roomData->collision[1152] = 0x0000; // base: 0x4210
                    roomData->collision[1153] = 0x0000; // base: 0x4210
                    roomData->collision[1154] = 0x4008; // base: 0x400F
                }
            }

            void UpgradeBarrierDownstairs(int playerIndex) {
                RoomData* roomData = GetRoomData(MAIN_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[1165] = 0x0000; // base: 0x4210
                    roomData->collision[1166] = 0x0000; // base: 0x4210
                    roomData->collision[1167] = 0x0000; // base: 0x4210
                    roomData->collision[1168] = 0x0000; // base: 0x4210
                    roomData->collision[1169] = 0x4009; // base: 0x400F
                }
            }

            void UpgradeBarrierLeftRoom(int playerIndex) {
                RoomData* roomData = GetRoomData(MAIN_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[810] = 0x0000; // base: 0x4210
                    roomData->collision[811] = 0x0000; // base: 0x4210
                    roomData->collision[812] = 0x0000; // base: 0x4210
                    roomData->collision[813] = 0x0000; // base: 0x4210
                    roomData->collision[814] = 0x400A; // base: 0x400F
                    roomData->collision[815] = 0x0000; // base: 0x4210
                    roomData->collision[816] = 0x0000; // base: 0x4210
                    roomData->collision[817] = 0x0000; // base: 0x4210
                    roomData->collision[818] = 0x0000; // base: 0x4210
                    roomData->collision[819] = 0x407E; // base: 0x407A
                    roomData->collision[890] = 0x0000; // base: 0x4210
                    roomData->collision[891] = 0x0000; // base: 0x4210
                    roomData->collision[892] = 0x0000; // base: 0x4210
                    roomData->collision[893] = 0x0000; // base: 0x4210
                    roomData->collision[894] = 0x400A; // base: 0x400F
                    roomData->collision[895] = 0x0000; // base: 0x4210
                    roomData->collision[896] = 0x0000; // base: 0x4210
                    roomData->collision[897] = 0x0000; // base: 0x4210
                    roomData->collision[898] = 0x0000; // base: 0x4210
                    roomData->collision[899] = 0x407E; // base: 0x407A
                }
            }

            void UpgradeBarrierRightRoom(int playerIndex) {
                RoomData* roomData = GetRoomData(MAIN_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[860] = 0x0000; // base: 0x4210
                    roomData->collision[861] = 0x0000; // base: 0x4210
                    roomData->collision[862] = 0x0000; // base: 0x4210
                    roomData->collision[863] = 0x0000; // base: 0x4210
                    roomData->collision[864] = 0x407F; // base: 0x407B
                    roomData->collision[865] = 0x0000; // base: 0x4210
                    roomData->collision[866] = 0x0000; // base: 0x4210
                    roomData->collision[867] = 0x0000; // base: 0x4210
                    roomData->collision[868] = 0x0000; // base: 0x4210
                    roomData->collision[869] = 0x400C; // base: 0x400F
                    roomData->collision[940] = 0x0000; // base: 0x4210
                    roomData->collision[941] = 0x0000; // base: 0x4210
                    roomData->collision[942] = 0x0000; // base: 0x4210
                    roomData->collision[943] = 0x0000; // base: 0x4210
                    roomData->collision[944] = 0x407F; // base: 0x407B
                    roomData->collision[945] = 0x0000; // base: 0x4210
                    roomData->collision[946] = 0x0000; // base: 0x4210
                    roomData->collision[947] = 0x0000; // base: 0x4210
                    roomData->collision[948] = 0x0000; // base: 0x4210
                    roomData->collision[949] = 0x400C; // base: 0x400F
                }
            }

            void UpgradeBarrierTopRoom(int playerIndex) {
                RoomData* roomData = GetRoomData(MAIN_ROOM[playerIndex]-1);
                if (roomData) {
                    roomData->collision[275] = 0x0000; // base: 0x4210
                    roomData->collision[276] = 0x0000; // base: 0x4210
                    roomData->collision[277] = 0x0000; // base: 0x4210
                    roomData->collision[278] = 0x0000; // base: 0x4210
                    roomData->collision[279] = 0x400B; // base: 0x400F
                    roomData->collision[280] = 0x0000; // base: 0x4210
                    roomData->collision[281] = 0x0000; // base: 0x4210
                    roomData->collision[282] = 0x0000; // base: 0x4210
                    roomData->collision[283] = 0x0000; // base: 0x4210
                    roomData->collision[284] = 0x400B; // base: 0x400F
                    roomData->collision[355] = 0x0000; // base: 0x4210
                    roomData->collision[356] = 0x0000; // base: 0x4210
                    roomData->collision[357] = 0x0000; // base: 0x4210
                    roomData->collision[358] = 0x0000; // base: 0x4210
                    roomData->collision[359] = 0x407D; // base: 0x4079
                    roomData->collision[360] = 0x0000; // base: 0x4210
                    roomData->collision[361] = 0x0000; // base: 0x4210
                    roomData->collision[362] = 0x0000; // base: 0x4210
                    roomData->collision[363] = 0x0000; // base: 0x4210
                    roomData->collision[364] = 0x407D; // base: 0x4079
                }
            }
        }
    }
}