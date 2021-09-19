#include "cheats.hpp"

namespace CTRPluginFramework {
    const IDS1 Buildings[205] = {
		{ "Player 1 House", 0 }, 
		{ "Player 2 House", 1 },
		{ "Player 3 House", 2 },
		{ "Player 4 House", 3 },
		{ "Player 1 Mailbox", 4 },
		{ "Player 2 Mailbox ", 5 },
		{ "Player 3 Mailbox ", 6 },
		{ "Player 4 Mailbox ", 7 },
		{ "Villager 1 House", 8 },
		{ "Villager 2 House", 9 },
		{ "Villager 3 House", 0xA },
		{ "Villager 4 House", 0xB },
		{ "Villager 5 House", 0xC },
		{ "Villager 6 House", 0xD },
		{ "Villager 7 House", 0xE },
		{ "Villager 8 House", 0xF },
		{ "Villager 9 House", 0x10 },
		{ "Villager 10 House", 0x11 },
		{ "Invalid Villager 1 House", 0x42 },
		{ "Invalid Villager 2 House", 0x43 },
		{ "Invalid Villager 3 House", 0x44 },
		{ "Invalid Villager 4 House", 0x45 },
		{ "Invalid Villager 5 House", 0x46 },
		{ "Invalid Villager 6 House", 0x47 },
		{ "Invalid Villager 7 House", 0x48 },
		{ "Invalid Villager 8 House", 0x49 },
		{ "Invalid Villager 9 House", 0x4A },
		{ "Invalid Villager 10 House", 0x4B },
		{ "Modern Police Station", 0x4C },
		{ "Classic Police Station", 0x4D },
		{ "Cafe", 0x4E },
		{ "Reset Center", 0x4F },
		{ "Classic Town Hall", 0x50 },
		{ "Zen Town Hall", 0x51 },
		{ "Fairy-Tale Town Hall", 0x52 },
		{ "Modern Town Hall", 0x53 },
		{ "Classic Train Station", 0x54 },
		{ "Zen Train Station", 0x55 },
		{ "Fairy-Tale Train Station", 0x56 },
		{ "Modern Train Station", 0x57 },
		{ "Recycle Shop", 0x58 },
		{ "Town Plaza", 0x59 },
		{ "Railroad Crossing Arms", 0x5A },
		{ "Bulletin Board", 0x5B },
		{ "Island Storage Bin", 0x5C },
		{ "Kappn's Boat", 0x5D },
		{ "Katrina's Tent", 0x5E },
		{ "Camper's Tent", 0x5F },
		{ "Redd's Tent", 0x60 },
		{ "Chip's Tent", 0x61 },
		{ "Nat's Tent", 0x62 },
		{ "Franklin's Table", 0x63 },
		{ "Pavé's Dance Stage", 0x64 },
		{ "Countdown Board", 0x65 },
		{ "Redd's Stand", 0x66 },
		{ "Dream World Bed", 0x67 },
		{ "Dock", 0x68 },
		{ "Island Hut", 0x69 },
		{ "Lloid", 0x6A },
		{ "FCS New Years Day", 0x6B },
		{ "FCS Lunar New Year", 0x6C },
		{ "FCS Great Full Moon", 0x6D },
		{ "FCS Groundhog Day ", 0x6E },
		{ "FCS Bean Day", 0x6F },
		{ "FCS Girl's Festival", 0x70 },
		{ "FCS Shamrock Day", 0x71 },
		{ "FCS Arbor Day", 0x72 },
		{ "FCS Nature Day", 0x73 },
		{ "FCS Boy's Festival", 0x74 },
		{ "FCS Teachers' Day", 0x75 },
		{ "FCS Summer Solstice", 0x76 },
		{ "FCS Starcrossed Day", 0x77 },
		{ "FCS Obon", 0x78 },
		{ "FCS Labor Day", 0x79 },
		{ "FCS Autumn Moon JPN&KOR", 0x7A },
		{ "FCS Autumn Moon USA&EUR", 0x7B },
		{ "FCS Explorer's Day", 0x7C },
		{ "FCS Naughty-or-Nice Day", 0x7D },
		{ "FCS Winter Solstice", 0x7E },
		{ "Camping Car 1", 0x7F },
		{ "Camping Car 2", 0x80 },
		{ "Fortune Shop", 0x81 },
		{ "Dream Suite", 0x82 },
		{ "Club LOL", 0x83 },
		{ "Museum", 0x84 },
		{ "Nookling General Stores", 0x85 },
		{ "Gardening Store", 0x86 },
		{ "Kicks", 0x87 },
		{ "Shampoodle", 0x88 },
		{ "Able Sisters (Mabel&Sable Part)", 0x89 },
		{ "Able Sisters (Labelle Part)", 0x8A },
		{ "Nook's Homes", 0x8B },
		{ "Post Office", 0x8C },
		{ "Photo Booth", 0x8D },
		{ "Main Street lamp", 0x8E },
		{ "Main Street Bench", 0x8F },
		{ "Cobblestone Bridge (N-S)", 0x90 },
		{ "Cobblestone Bridge (SW-NE)", 0x91 },
		{ "Cobblestone Bridge (NW-SE)", 0x92 },
		{ "Cobblestone Bridge (W-E)", 0x93 },
		{ "Suspension Bridge (N-S)", 0x94 },
		{ "Suspension Bridge (SW-NE)", 0x95 },
		{ "Suspension Bridge (NW-SE)", 0x96 },
		{ "Suspension Bridge (W-E)", 0x97 },
		{ "Brick Bridge (N-S)", 0x98 },
		{ "Brick Bridge (SW-NE)", 0x99 },
		{ "Brick Bridge (NW-SE)", 0x9A },
		{ "Brick Bridge (W-E)", 0x9B },
		{ "Modern Bridge (N-S)", 0x9C },
		{ "Modern Bridge (SW-NE)", 0x9D },
		{ "Modern Bridge (NW-SE)", 0x9E },
		{ "Modern Bridge (W-E)", 0x9F },
		{ "Zen Bridge (N-S)", 0xA0 },
		{ "Zen Bridge (SW-NE)", 0xA1 },
		{ "Zen Bridge (NW-SE)", 0xA2 },
		{ "Zen Bridge (W-E)", 0xA3 },
		{ "Fairy Tale Bridge (N-S)", 0xA4 },
		{ "Fairy Tale Bridge (SW-NE)", 0xA5 },
		{ "Fairy Tale Bridge (NW-SE)", 0xA6 },
		{ "Fairy Tale Bridge (W-E)", 0xA7 },
		{ "Yellow Bench", 0xA8 },
		{ "Blue Bench", 0xA9 },
		{ "Wood Bench", 0xAA },
		{ "Metal Bench", 0xAB },
		{ "Log Bench", 0xAC },
		{ "Modern Bench", 0xAD },
		{ "Fairy-Tale Bench", 0xAE },
		{ "Zen Bench", 0xAF },
		{ "Flower Bed", 0xB0 },
		{ "Water Fountain", 0xB1 },
		{ "Instrument Shelter", 0xB2 },
		{ "Sandbox", 0xB3 },
		{ "Garbage Can", 0xB4 },
		{ "Pile of Pipes", 0xB5 },
		{ "Water Well", 0xB6 },
		{ "Fountain", 0xB7 },
		{ "Tire Toy", 0xB8 },
		{ "Jungle Gym", 0xB9 },
		{ "Park Clock", 0xBA },
		{ "Modern Clock", 0xBB },
		{ "Fairy-Tale Clock", 0xBC },
		{ "Zen Clock", 0xBD },
		{ "Street Lamp", 0xBE },
		{ "Round Streetlight", 0xBF },
		{ "Streetlight", 0xC0 },
		{ "Modern Streetlight", 0xC1 },
		{ "Fairy-Tale Streetlight", 0xC2 },
		{ "Zen Streetlight", 0xC3 },
		{ "Balloon Arch", 0xC4 },
		{ "Flower Arch", 0xC5 },
		{ "Campsite", 0xC6 },
		{ "Picnic Blanket", 0xC7 },
		{ "Hammock", 0xC8 },
		{ "Fire Pit", 0xC9 },
		{ "Camping Cot", 0xCA },
		{ "Outdoor Chair", 0xCB },
		{ "Torch", 0xCC },
		{ "Zen Garden", 0xCD },
		{ "Hot Spring", 0xCE },
		{ "Geyser", 0xCF },
		{ "Statue Fountain", 0xD0 },
		{ "Stone Tablet", 0xD1 },
		{ "Water Pump", 0xD2 },
		{ "Wisteria Trellis", 0xD3 },
		{ "Bell", 0xD4 },
		{ "Zen Bell", 0xD5 },
		{ "Scarecrow", 0xD6 },
		{ "Race of Rice", 0xD7 },
		{ "Fence", 0xD8 },
		{ "Bus Stop", 0xD9 },
		{ "Fire Hydrant", 0xDA },
		{ "Traffic Signal", 0xDB },
		{ "Custom-Design Sign", 0xDC },
		{ "Face-Cutout Standee", 0xDD },
		{ "Caution Sign", 0xDE },
		{ "Do-Not-Enter Sign", 0xDF },
		{ "Yield Sign", 0xE0 },
		{ "Cube Sculpture", 0xE1 },
		{ "Archway Sculpture", 0xE2 },
		{ "Chair Sculpture", 0xE3 },
		{ "Illuminated Heart", 0xE4 },
		{ "Illuminated Arch", 0xE5 },
		{ "Illuminated Clock", 0xE6 },
		{ "Illuminated Tree", 0xE7 },
		{ "Stadium Light", 0xE8 },
		{ "Video Screen", 0xE9 },
		{ "Drilling Rig", 0xEA },
		{ "Parabolic Antenna", 0xEB },
		{ "Solar Panels", 0xEC },
		{ "Wind Turbine", 0xED },
		{ "Windmill", 0xEE },
		{ "Lighthouse", 0xEF },
		{ "Tower", 0xF0 },
		{ "Stonehenge", 0xF1 },
		{ "Totem Pole", 0xF2 },
		{ "Moai Statue", 0xF3 },
		{ "Pyramid", 0xF4 },
		{ "Sphinx", 0xF5 },
		{ "Circle Topiary", 0xF6 },
		{ "Square Topiary", 0xF7 },
		{ "Tulip Topiary", 0xF8 },
		{ "Flower Clock", 0xF9 },
		{ "Campground Fence Lock 1", 0xFA },
		{ "Campground Fence Lock 2", 0xFB },
		{ "Empty Space", 0xFC }
	};

	const IDS1 Countrys[134] = {
		{ "Japan", 1 },
		{ "Anguilla", 8 },
		{ "Antigua and Barbuda", 9 },
		{ "Argentina", 0xA },
		{ "Aruba", 0xB },
		{ "Bahamas", 0xC },
		{ "Barbados", 0xD },
		{ "Belize", 0xE },
		{ "Bolivia", 0xF },
		{ "Brazil", 0x10 },
		{ "British Virgin Islands", 0x11 },
		{ "Canada", 0x12 },
		{ "Cayman Islands", 0x13 },
		{ "Chile", 0x14 },
		{ "Colombia", 0x15 },
		{ "Costa Rica", 0x16 },
		{ "Dominica", 0x17 },
		{ "Dominican Republic", 0x18 },
		{ "Ecuador", 0x19 },
		{ "El Salvador", 0x1A },
		{ "French Guiana", 0x1B },
		{ "Grenada", 0x1C },
		{ "Guadeloupe", 0x1D },
		{ "Guatemala", 0x1E },
		{ "Guyana", 0x1F },
		{ "Haiti", 0x20 },
		{ "Honduras", 0x21 },
		{ "Jamaica", 0x22 },
		{ "Martinique", 0x23 },
		{ "Mexico", 0x24 },
		{ "Montserrat", 0x25 },
		{ "Netherlands Antilles", 0x26 },
		{ "Nicaragua", 0x27 },
		{ "Panama", 0x28 },
		{ "Paraguay", 0x29 },
		{ "Peru", 0x2A },
		{ "Saint Kitts and Nevis", 0x2B },
		{ "Saint Lucia", 0x2C },
		{ "Saint Vincent and the Grenadines", 0x2D },
		{ "Suriname", 0x2E },
		{ "Trinidad and Tobago", 0x2F },
		{ "Turks and Caicos Islands", 0x30 },
		{ "United States", 0x31 },
		{ "Uruguay", 0x32 },
		{ "US Virgin Islands", 0x33 },
		{ "Venezuela", 0x34 },
		{ "Albania", 0x40 },
		{ "Australia", 0x41 },
		{ "Austria", 0x42 },
		{ "Belgium", 0x43 },
		{ "Bosnia and Herzegovina", 0x44 },
		{ "Botswana", 0x45 },
		{ "Bulgaria", 0x46 },
		{ "Croatia", 0x47 },
		{ "Cyprus", 0x48 },
		{ "Czech Republic", 0x49 },
		{ "Denmark", 0x4A },
		{ "Estonia", 0x4B },
		{ "Finland", 0x4C },
		{ "France", 0x4D },
		{ "Germany", 0x4E },
		{ "Greece", 0x4F },
		{ "Hungary", 0x50 },
		{ "Iceland", 0x51 },
		{ "Ireland", 0x52 },
		{ "Italy", 0x53 },
		{ "Latvia", 0x54 },
		{ "Lesotho", 0x55 },
		{ "Liechtenstein", 0x56 },
		{ "Lithuania", 0x57 },
		{ "Luxembourg", 0x58 },
		{ "Macedonia", 0x59 },
		{ "Malta", 0x5A },
		{ "Montenegro", 0x5B },
		{ "Mozambique", 0x5C },
		{ "Namibia", 0x5D },
		{ "Netherlands", 0x5E },
		{ "New Zealand", 0x5F },
		{ "Norway", 0x60 },
		{ "Poland", 0x61 },
		{ "Portugal", 0x62 },
		{ "Romania", 0x63 },
		{ "Russia", 0x64 },
		{ "Serbia", 0x65 },
		{ "Slovakia", 0x66 },
		{ "Slovenia", 0x67 },
		{ "South Africa", 0x68 },
		{ "Spain", 0x69 },
		{ "Swaziland", 0x6A },
		{ "Sweden", 0x6B },
		{ "Switzerland", 0x6C },
		{ "Turkey", 0x6D },
		{ "United Kingdom", 0x6E },
		{ "Zambia", 0x6F },
		{ "Zimbabwe", 0x70 },
		{ "Azerbaijan", 0x71 },
		{ "Mauritania", 0x72 },
		{ "Mali", 0x73 },
		{ "Niger", 0x74 },
		{ "Chad", 0x75 },
		{ "Sudan", 0x76 },
		{ "Eritrea", 0x77 },
		{ "Djibouti", 0x78 },
		{ "Somalia", 0x79 },
		{ "Andorra", 0x7A },
		{ "Gibraltar", 0x7B },
		{ "Guernsey", 0x7C },
		{ "Isle of Man", 0x7D },
		{ "Jersey", 0x7E },
		{ "Monaco", 0x7F },
		{ "Taiwan", 0x80 },
		{ "South Korea", 0x88 },
		{ "Hong Kong", 0x90 },
		{ "Macau", 0x91 },
		{ "Indonesia", 0x98 },
		{ "Singapore", 0x99 },
		{ "Thailand", 0x9A },
		{ "Philippines", 0x9B },
		{ "Malaysia", 0x9C },
		{ "China", 0xA0 },
		{ "United Arab Emirates", 0xA8 },
		{ "India", 0xA9 },
		{ "Egypt", 0xAA },
		{ "Oman", 0xAB },
		{ "Qatar", 0xAC },
		{ "Kuwait", 0xAD },
		{ "Saudi Arabia", 0xAE },
		{ "Syria", 0xAF },
		{ "Bahrain", 0xB0 },
		{ "Jordan", 0xB1 },
		{ "San Marino", 0xB8 },
		{ "Vatican City", 0xB9 },
		{ "Bermuda", 0xBA },
		{ "Hacker Island", 0xFF }
	};

//Based on: https://docs.google.com/spreadsheets/d/19E7pMhKN6x583uB6bWVBeaTMyBPtEAC-Bk59Y6cfgxA/edit#gid=95881524
    const AmiiboInfo amiiboVillagers[399] = {
        {0x02700001, 0x00FF0502, 0x003C, SpecieID::Cat}, //Filename: cat19
        {0x025D0001, 0x00550502, 0x0029, SpecieID::Cat}, //Filename: cat00
        {0x026E0001, 0x00BA0502, 0x003A, SpecieID::Cat}, //Filename: cat17
        {0x35010000, 0x02E30F02, 0x003F, SpecieID::Cat}, //Filename: cat22
        {0x02660001, 0x00680502, 0x0032, SpecieID::Cat}, //Filename: cat09
        {0x02720001, 0x01860502, 0x003E, SpecieID::Cat}, //Filename: cat21
        {0x02670001, 0x01080502, 0x0033, SpecieID::Cat}, //Filename: cat10
        {0x02610001, 0x00650502, 0x002D, SpecieID::Cat}, //Filename: cat04
        {0x026B0001, 0x00E90502, 0x0037, SpecieID::Cat}, //Filename: cat14
        {0x026F0001, 0x01900502, 0x003B, SpecieID::Cat}, //Filename: cat18
        {0x026D0001, 0x013F0502, 0x0039, SpecieID::Cat}, //Filename: cat16
        {0x025E0001, 0x01250502, 0x002A, SpecieID::Cat}, //Filename: cat01
        {0x02650001, 0x01540502, 0x0031, SpecieID::Cat}, //Filename: cat08
        {0x02680001, 0x007D0502, 0x0034, SpecieID::Cat}, //Filename: cat11
        {0x02600001, 0x00D20502, 0x002C, SpecieID::Cat}, //Filename: cat03
        {0x02630001, 0x00750502, 0x002F, SpecieID::Cat}, //Filename: cat06
        {0x02640001, 0x01AC0502, 0x0030, SpecieID::Cat}, //Filename: cat07
        {0x025F0001, 0x01D70502, 0x002B, SpecieID::Cat}, //Filename: cat02
        {0x02710001, 0x019B0502, 0x003D, SpecieID::Cat}, //Filename: cat20
        {0x026A0001, 0x01460502, 0x0036, SpecieID::Cat}, //Filename: cat13
        {0x02690001, 0x011F0502, 0x0035, SpecieID::Cat}, //Filename: cat12
        {0x02620001, 0x01370502, 0x002E, SpecieID::Cat}, //Filename: cat05
        {0x026C0001, 0x00C30502, 0x0038, SpecieID::Cat}, //Filename: cat15

        {0x03290001, 0x009D0502, 0x0096, SpecieID::Elephant}, //Filename: elp06
        {0x03250001, 0x010A0502, 0x0092, SpecieID::Elephant}, //Filename: elp02
        {0x032E0101, 0x031C0502, 0x009A, SpecieID::Elephant}, //Filename: elp11
        {0x03240001, 0x01890502, 0x0091, SpecieID::Elephant}, //Filename: elp01
        {0x032A0001, 0x03070502, 0x0097, SpecieID::Elephant}, //Filename: elp07
        {0x03260001, 0x01390502, 0x0093, SpecieID::Elephant}, //Filename: elp03
        {0x03270001, 0x01C30502, 0x0094, SpecieID::Elephant}, //Filename: elp04
        {0x03230001, 0x00760502, 0x0090, SpecieID::Elephant}, //Filename: elp00
        {0x03280001, 0x02EB0502, 0x0095, SpecieID::Elephant}, //Filename: elp05
        {0x032D0001, 0x00BC0502, 0x0099, SpecieID::Elephant}, //Filename: elp10
        {0x032C0001, 0x01480502, 0x0098, SpecieID::Elephant}, //Filename: elp09

        {0x04C60001, 0x01670502, 0x015D, SpecieID::Sheep}, //Filename: shp01
        {0x04C90001, 0x030D0502, 0x0160, SpecieID::Sheep}, //Filename: shp04
        {0x04CD0001, 0x01520502, 0x0162, SpecieID::Sheep}, //Filename: shp08
        {0x04D30101, 0x031B0502, 0x0168, SpecieID::Sheep}, //Filename: shp14
        {0x04C70001, 0x00940502, 0x015E, SpecieID::Sheep}, //Filename: shp02
        {0x04D00001, 0x01960502, 0x0165, SpecieID::Sheep}, //Filename: shp11
        {0x04D10001, 0x009E0502, 0x0166, SpecieID::Sheep}, //Filename: shp12
        {0x04D20001, 0x01A70502, 0x0167, SpecieID::Sheep}, //Filename: shp13
        {0x04C80001, 0x02ED0502, 0x015F, SpecieID::Sheep}, //Filename: shp03
        {0x04CF0001, 0x00E10502, 0x0164, SpecieID::Sheep}, //Filename: shp10
        {0x04C50001, 0x01010502, 0x015C, SpecieID::Sheep}, //Filename: shp00
        {0x04CE0001, 0x00DB0502, 0x0163, SpecieID::Sheep}, //Filename: shp09
        {0x04CC0001, 0x00A40502, 0x0161, SpecieID::Sheep}, //Filename: shp07

        {0x02210001, 0x013C0502, 0x0013, SpecieID::Bear}, //Filename: bea13
        {0x02200001, 0x00FD0502, 0x0012, SpecieID::Bear}, //Filename: bea12
        {0x02170001, 0x01B30502, 0x000A, SpecieID::Bear}, //Filename: bea03
        {0x02160001, 0x00570502, 0x0009, SpecieID::Bear}, //Filename: bea02
        {0x021D0001, 0x01CD0502, 0x000F, SpecieID::Bear}, //Filename: bea09
        {0x021A0001, 0x00DA0502, 0x000C, SpecieID::Bear}, //Filename: bea06
        {0x021F0001, 0x03170502, 0x0011, SpecieID::Bear}, //Filename: bea11
        {0x02220001, 0x01440502, 0x0014, SpecieID::Bear}, //Filename: bea14
        {0x02190001, 0x007E0502, 0x000B, SpecieID::Bear}, //Filename: bea05
        {0x021E0001, 0x01230502, 0x0010, SpecieID::Bear}, //Filename: bea10
        {0x02150001, 0x01820502, 0x0008, SpecieID::Bear}, //Filename: bea01
        {0x02140001, 0x00E40502, 0x0007, SpecieID::Bear}, //Filename: bea00
        {0x021B0001, 0x00800502, 0x000D, SpecieID::Bear}, //Filename: bea07
        {0x021C0001, 0x02F70502, 0x000E, SpecieID::Bear}, //Filename: bea08

        {0x02F40001, 0x03050502, 0x0079, SpecieID::Dog}, //Filename: dog10
        {0x02FA0001, 0x00970502, 0x007C, SpecieID::Dog}, //Filename: dog16
        {0x02ED0001, 0x015A0502, 0x0072, SpecieID::Dog}, //Filename: dog03
        {0x02EE0001, 0x01990502, 0x0073, SpecieID::Dog}, //Filename: dog04
        {0x02EB0001, 0x00DE0502, 0x0070, SpecieID::Dog}, //Filename: dog01
        {0x02FB0001, 0x00900502, 0x007D, SpecieID::Dog}, //Filename: dog17
        {0x02F20001, 0x00CC0502, 0x0077, SpecieID::Dog}, //Filename: dog08
        {0x02F10001, 0x01450502, 0x0076, SpecieID::Dog}, //Filename: dog07
        {0x02EA0001, 0x01D50502, 0x006F, SpecieID::Dog}, //Filename: dog00
        {0x02EC0001, 0x01C40502, 0x0071, SpecieID::Dog}, //Filename: dog02
        {0x02F80001, 0x01380502, 0x007A, SpecieID::Dog}, //Filename: dog14
        {0x02F30001, 0x02F90502, 0x0078, SpecieID::Dog}, //Filename: dog09
        {0x02F90001, 0x01020502, 0x007B, SpecieID::Dog}, //Filename: dog15
        {0x02EF0001, 0x00580502, 0x0074, SpecieID::Dog}, //Filename: dog05
        {0x02FC0001, 0x018F0502, 0x007E, SpecieID::Dog}, //Filename: dog18
        {0x02F00001, 0x00A70502, 0x0075, SpecieID::Dog}, //Filename: dog06

        {0x04E20001, 0x01090502, 0x016E, SpecieID::Squirrel}, //Filename: squ05
        {0x04DE0001, 0x00CE0502, 0x016A, SpecieID::Squirrel}, //Filename: squ01
        {0x04E80001, 0x01CE0502, 0x0174, SpecieID::Squirrel}, //Filename: squ11
        {0x04E30001, 0x01650502, 0x016F, SpecieID::Squirrel}, //Filename: squ06
        {0x08010000, 0x025D0402, 0x017B, SpecieID::Squirrel}, //Filename: squ19
        {0x04DF0001, 0x00E80502, 0x016B, SpecieID::Squirrel}, //Filename: squ02
        {0x04EF0001, 0x013B0502, 0x017A, SpecieID::Squirrel}, //Filename: squ18
        {0x04EE0001, 0x014B0502, 0x0179, SpecieID::Squirrel}, //Filename: squ17
        {0x04E60001, 0x00820502, 0x0172, SpecieID::Squirrel}, //Filename: squ09
        {0x04E10001, 0x01BE0502, 0x016D, SpecieID::Squirrel}, //Filename: squ04
        {0x04DD0001, 0x00A20502, 0x0169, SpecieID::Squirrel}, //Filename: squ00
        {0x04E00001, 0x00F70502, 0x016C, SpecieID::Squirrel}, //Filename: squ03
        {0x04EC0001, 0x00770502, 0x0177, SpecieID::Squirrel}, //Filename: squ15
        {0x04E70001, 0x01320502, 0x0173, SpecieID::Squirrel}, //Filename: squ10
        {0x04E40001, 0x01B60502, 0x0170, SpecieID::Squirrel}, //Filename: squ07
        {0x04ED0001, 0x00620502, 0x0178, SpecieID::Squirrel}, //Filename: squ16
        {0x04E50001, 0x01AD0502, 0x0171, SpecieID::Squirrel}, //Filename: squ08
        {0x04EB0001, 0x02F00502, 0x0176, SpecieID::Squirrel}, //Filename: squ14
        {0x04EA0001, 0x03180502, 0x0175, SpecieID::Squirrel}, //Filename: squ13
        {0x08020000, 0x025E0402, 0x017C, SpecieID::Squirrel}, //Filename: squ20

        {0x04A50001, 0x00740502, 0x0152, SpecieID::Rabbit}, //Filename: rbt17
        {0x04940001, 0x009A0502, 0x0141, SpecieID::Rabbit}, //Filename: rbt00
        {0x04A40001, 0x00D40502, 0x0151, SpecieID::Rabbit}, //Filename: rbt16
        {0x04A10001, 0x016F0502, 0x014E, SpecieID::Rabbit}, //Filename: rbt13
        {0x049F0001, 0x03010502, 0x014C, SpecieID::Rabbit}, //Filename: rbt11
        {0x04960001, 0x00D90502, 0x0143, SpecieID::Rabbit}, //Filename: rbt02
        {0x04A60001, 0x00A30502, 0x0153, SpecieID::Rabbit}, //Filename: rbt18
        {0x049E0001, 0x01B70502, 0x014B, SpecieID::Rabbit}, //Filename: rbt10
        {0x04950001, 0x01920502, 0x0142, SpecieID::Rabbit}, //Filename: rbt01
        {0x04A00001, 0x016E0502, 0x014D, SpecieID::Rabbit}, //Filename: rbt12
        {0x04990001, 0x00DF0502, 0x0146, SpecieID::Rabbit}, //Filename: rbt05
        {0x04980001, 0x014A0502, 0x0145, SpecieID::Rabbit}, //Filename: rbt04
        {0x049C0001, 0x01400502, 0x0149, SpecieID::Rabbit}, //Filename: rbt08
        {0x04A20001, 0x02E80502, 0x014F, SpecieID::Rabbit}, //Filename: rbt14
        {0x04A70001, 0x01A60502, 0x0154, SpecieID::Rabbit}, //Filename: rbt19
        {0x04A30001, 0x01C90502, 0x0150, SpecieID::Rabbit}, //Filename: rbt15
        {0x049A0001, 0x014E0502, 0x0147, SpecieID::Rabbit}, //Filename: rbt06
        {0x049D0001, 0x00ED0502, 0x014A, SpecieID::Rabbit}, //Filename: rbt09
        {0x04970001, 0x007A0502, 0x0144, SpecieID::Rabbit}, //Filename: rbt03
        {0x049B0001, 0x00610502, 0x0148, SpecieID::Rabbit}, //Filename: rbt07
        {0x04A80101, 0x031E0502, 0x0155, SpecieID::Rabbit}, //Filename: rbt20

        {0x03070001, 0x00640502, 0x007F, SpecieID::Duck}, //Filename: duk00
        {0x030B0001, 0x00790502, 0x0083, SpecieID::Duck}, //Filename: duk04
        {0x030F0001, 0x016D0502, 0x0087, SpecieID::Duck}, //Filename: duk08
        {0x03100001, 0x00F80502, 0x0088, SpecieID::Duck}, //Filename: duk09
        {0x030E0001, 0x012F0502, 0x0086, SpecieID::Duck}, //Filename: duk07
        {0x03160001, 0x01C00502, 0x008D, SpecieID::Duck}, //Filename: duk15
        {0x03080001, 0x014D0502, 0x0080, SpecieID::Duck}, //Filename: duk01
        {0x03140001, 0x02F40502, 0x008C, SpecieID::Duck}, //Filename: duk13
        {0x030A0001, 0x01C70502, 0x0082, SpecieID::Duck}, //Filename: duk03
        {0x030D0001, 0x01840502, 0x0085, SpecieID::Duck}, //Filename: duk06
        {0x03130001, 0x01210502, 0x008B, SpecieID::Duck}, //Filename: duk12
        {0x03170001, 0x00A60502, 0x008E, SpecieID::Duck}, //Filename: duk16
        {0x03090001, 0x00C60502, 0x0081, SpecieID::Duck}, //Filename: duk02
        {0x030C0001, 0x01B80502, 0x0084, SpecieID::Duck}, //Filename: duk05
        {0x03180001, 0x006C0502, 0x008F, SpecieID::Duck}, //Filename: duk17
        {0x03110001, 0x00D60502, 0x0089, SpecieID::Duck}, //Filename: duk10
        {0x03120001, 0x03090502, 0x008A, SpecieID::Duck}, //Filename: duk11

        {0x03930001, 0x00A00502, 0x00C9, SpecieID::Hippo}, //Filename: hip03
        {0x03940001, 0x00890502, 0x00CA, SpecieID::Hippo}, //Filename: hip04
        {0x03950001, 0x02FC0502, 0x00CB, SpecieID::Hippo}, //Filename: hip05
        {0x03920001, 0x01270502, 0x00C8, SpecieID::Hippo}, //Filename: hip02
        {0x03980001, 0x00BF0502, 0x00CC, SpecieID::Hippo}, //Filename: hip08
        {0x03990001, 0x01C20502, 0x00CD, SpecieID::Hippo}, //Filename: hip09
        {0x03900001, 0x01850502, 0x00C7, SpecieID::Hippo}, //Filename: hip00

        {0x050B0001, 0x00990502, 0x0184, SpecieID::Wolf}, //Filename: wol00
        {0x050F0001, 0x03140502, 0x0188, SpecieID::Wolf}, //Filename: wol04
        {0x05110001, 0x01950502, 0x018A, SpecieID::Wolf}, //Filename: wol06
        {0x05100001, 0x01070502, 0x0189, SpecieID::Wolf}, //Filename: wol05
        {0x05150001, 0x005B0502, 0x018D, SpecieID::Wolf}, //Filename: wol10
        {0x050C0001, 0x01C10502, 0x0185, SpecieID::Wolf}, //Filename: wol01
        {0x05140001, 0x01530502, 0x018C, SpecieID::Wolf}, //Filename: wol09
        {0x05130001, 0x02E70502, 0x018B, SpecieID::Wolf}, //Filename: wol08
        {0x050E0001, 0x00D70502, 0x0187, SpecieID::Wolf}, //Filename: wol03
        {0x050D0001, 0x01420502, 0x0186, SpecieID::Wolf}, //Filename: wol02
        {0x01030000, 0x024F0902, 0x018E, SpecieID::Wolf}, //Filename: wol11

        {0x04160001, 0x00FB0502, 0x0106, SpecieID::Mouse}, //Filename: mus10
        {0x040E0001, 0x00880502, 0x0100, SpecieID::Mouse}, //Filename: mus02
        {0x041B0001, 0x00F10502, 0x0109, SpecieID::Mouse}, //Filename: mus15
        {0x040F0001, 0x01500502, 0x0101, SpecieID::Mouse}, //Filename: mus03
        {0x04180001, 0x00D80502, 0x0107, SpecieID::Mouse}, //Filename: mus12
        {0x04140001, 0x030A0502, 0x0104, SpecieID::Mouse}, //Filename: mus08
        {0x041E0001, 0x015F0502, 0x010C, SpecieID::Mouse}, //Filename: mus18
        {0x040C0001, 0x01590502, 0x00FE, SpecieID::Mouse}, //Filename: mus00
        {0x041C0001, 0x01410502, 0x010A, SpecieID::Mouse}, //Filename: mus16
        {0x040D0001, 0x00780502, 0x00FF, SpecieID::Mouse}, //Filename: mus01
        {0x041A0001, 0x00E00502, 0x0108, SpecieID::Mouse}, //Filename: mus14
        {0x041D0001, 0x018A0502, 0x010B, SpecieID::Mouse}, //Filename: mus17
        {0x04150001, 0x01BB0502, 0x0105, SpecieID::Mouse}, //Filename: mus09
        {0x04110001, 0x01AB0502, 0x0103, SpecieID::Mouse}, //Filename: mus05
        {0x04100001, 0x007F0502, 0x0102, SpecieID::Mouse}, //Filename: mus04

        {0x04890001, 0x00EF0502, 0x013F, SpecieID::Pig}, //Filename: pig17
        {0x04810001, 0x02F10502, 0x0138, SpecieID::Pig}, //Filename: pig09
        {0x04860001, 0x00FC0502, 0x013C, SpecieID::Pig}, //Filename: pig14
        {0x04800001, 0x008D0502, 0x0137, SpecieID::Pig}, //Filename: pig08
        {0x04780001, 0x01630502, 0x0131, SpecieID::Pig}, //Filename: pig00
        {0x047D0001, 0x012E0502, 0x0136, SpecieID::Pig}, //Filename: pig05
        {0x04850001, 0x014C0502, 0x013B, SpecieID::Pig}, //Filename: pig13
        {0x01020100, 0x001B0002, 0x0140, SpecieID::Pig}, //Filename: pig18
        {0x047B0001, 0x00F50502, 0x0134, SpecieID::Pig}, //Filename: pig03
        {0x04870001, 0x01BF0502, 0x013D, SpecieID::Pig}, //Filename: pig15
        {0x047C0001, 0x01A00502, 0x0135, SpecieID::Pig}, //Filename: pig04
        {0x04820001, 0x02FD0502, 0x0139, SpecieID::Pig}, //Filename: pig10
        {0x04880001, 0x00980502, 0x013E, SpecieID::Pig}, //Filename: pig16
        {0x04830001, 0x01B00502, 0x013A, SpecieID::Pig}, //Filename: pig11
        {0x047A0001, 0x00600502, 0x0133, SpecieID::Pig}, //Filename: pig02
        {0x04790001, 0x00920502, 0x0132, SpecieID::Pig}, //Filename: pig01

        {0x029E0001, 0x013D0502, 0x0053, SpecieID::Chicken}, //Filename: chn05
        {0x02A20001, 0x01BA0502, 0x0054, SpecieID::Chicken}, //Filename: chn09
        {0x029A0001, 0x00EE0502, 0x0051, SpecieID::Chicken}, //Filename: chn01
        {0x02A50001, 0x018C0502, 0x0057, SpecieID::Chicken}, //Filename: chn12
        {0x029B0001, 0x00CB0502, 0x0052, SpecieID::Chicken}, //Filename: chn02
        {0x02990001, 0x00950502, 0x0050, SpecieID::Chicken}, //Filename: chn00
        {0x02A60001, 0x01240502, 0x0058, SpecieID::Chicken}, //Filename: chn13
        {0x02A40001, 0x00720502, 0x0056, SpecieID::Chicken}, //Filename: chn11
        {0x02A30001, 0x02FF0502, 0x0055, SpecieID::Chicken}, //Filename: chn10

        {0x024A0001, 0x01D10502, 0x0023, SpecieID::Bull}, //Filename: bul00
        {0x02510001, 0x00C10502, 0x0027, SpecieID::Bull}, //Filename: bul07
        {0x024B0001, 0x01260502, 0x0024, SpecieID::Bull}, //Filename: bul01
        {0x024D0001, 0x02F60502, 0x0025, SpecieID::Bull}, //Filename: bul03
        {0x024F0001, 0x00810502, 0x0026, SpecieID::Bull}, //Filename: bul05
        {0x02520001, 0x00FE0502, 0x0028, SpecieID::Bull}, //Filename: bul08

        {0x02B80001, 0x019C0502, 0x005C, SpecieID::Cow}, //Filename: cow07
        {0x02B70001, 0x030F0502, 0x005B, SpecieID::Cow}, //Filename: cow06
        {0x02B10001, 0x00690502, 0x0059, SpecieID::Cow}, //Filename: cow00
        {0x02B20001, 0x00C40502, 0x005A, SpecieID::Cow}, //Filename: cow01

        {0x02330001, 0x03060502, 0x001B, SpecieID::Bird}, //Filename: brd06
        {0x022F0001, 0x011E0502, 0x0017, SpecieID::Bird}, //Filename: brd02
        {0x023D0001, 0x01B50502, 0x001F, SpecieID::Bird}, //Filename: brd16
        {0x02380001, 0x02F80502, 0x001D, SpecieID::Bird}, //Filename: brd11
        {0x022D0001, 0x00F20502, 0x0015, SpecieID::Bird}, //Filename: brd00
        {0x02310001, 0x006A0502, 0x0019, SpecieID::Bird}, //Filename: brd04
        {0x023C0001, 0x00BD0502, 0x001E, SpecieID::Bird}, //Filename: brd15
        {0x01000100, 0x03500902, 0x0022, SpecieID::Bird}, //Filename: brd19
        {0x02350001, 0x00840502, 0x001C, SpecieID::Bird}, //Filename: brd08
        {0x023E0001, 0x00D10502, 0x0020, SpecieID::Bird}, //Filename: brd17
        {0x02320001, 0x02EA0502, 0x001A, SpecieID::Bird}, //Filename: brd05
        {0x022E0001, 0x01D30502, 0x0016, SpecieID::Bird}, //Filename: brd01
        {0x023F0001, 0x01660502, 0x0021, SpecieID::Bird}, //Filename: brd18
        {0x02300001, 0x01D20502, 0x0018, SpecieID::Bird}, //Filename: brd03

        {0x033B0001, 0x00FA0502, 0x009E, SpecieID::Frog}, //Filename: flg03
        {0x03420001, 0x01280502, 0x00A4, SpecieID::Frog}, //Filename: flg10
        {0x03490001, 0x018D0502, 0x00AA, SpecieID::Frog}, //Filename: flg17
        {0x034A0001, 0x01430502, 0x00AB, SpecieID::Frog}, //Filename: flg18
        {0x033C0001, 0x01000502, 0x009F, SpecieID::Frog}, //Filename: flg04
        {0x033A0001, 0x01CC0502, 0x009D, SpecieID::Frog}, //Filename: flg02
        {0x03480001, 0x006B0502, 0x00A9, SpecieID::Frog}, //Filename: flg16
        {0x034B0001, 0x009F0502, 0x00AC, SpecieID::Frog}, //Filename: flg19
        {0x03430001, 0x02EF0502, 0x00A5, SpecieID::Frog}, //Filename: flg11
        {0x03450001, 0x005F0502, 0x00A7, SpecieID::Frog}, //Filename: flg13
        {0x033F0001, 0x008F0502, 0x00A2, SpecieID::Frog}, //Filename: flg07
        {0x03380001, 0x011D0502, 0x009B, SpecieID::Frog}, //Filename: flg00
        {0x03440001, 0x00C50502, 0x00A6, SpecieID::Frog}, //Filename: flg12
        {0x033E0001, 0x01A20502, 0x00A1, SpecieID::Frog}, //Filename: flg06
        {0x03470001, 0x03020502, 0x00A8, SpecieID::Frog}, //Filename: flg15
        {0x03390001, 0x01B10502, 0x009C, SpecieID::Frog}, //Filename: flg01
        {0x03410001, 0x030E0502, 0x00A3, SpecieID::Frog}, //Filename: flg09
        {0x033D0001, 0x013A0502, 0x00A0, SpecieID::Frog}, //Filename: flg05

        {0x02C30001, 0x00DC0502, 0x005D, SpecieID::Alligator}, //Filename: crd00
        {0x02C40001, 0x00670502, 0x005E, SpecieID::Alligator}, //Filename: crd01
        {0x02C50001, 0x03080502, 0x005F, SpecieID::Alligator}, //Filename: crd02
        {0x02C70001, 0x01220502, 0x0060, SpecieID::Alligator}, //Filename: crd04
        {0x02CB0001, 0x01360502, 0x0063, SpecieID::Alligator}, //Filename: crd08
        {0x02CA0001, 0x01CA0502, 0x0062, SpecieID::Alligator}, //Filename: crd07
        {0x02C90001, 0x00CD0502, 0x0061, SpecieID::Alligator}, //Filename: crd06

        {0x03580001, 0x02FA0502, 0x00AF, SpecieID::Goat}, //Filename: goa02
        {0x03560001, 0x01350502, 0x00AD, SpecieID::Goat}, //Filename: goa00
        {0x03580001, 0x02FA0502, 0x00B0, SpecieID::Goat}, //Filename: goa04
        {0x035D0001, 0x00C90502, 0x00B2, SpecieID::Goat}, //Filename: goa07
        {0x03570001, 0x00EB0502, 0x00AE, SpecieID::Goat}, //Filename: goa01
        {0x035E0001, 0x018E0502, 0x00B3, SpecieID::Goat}, //Filename: goa08
        {0x035C0001, 0x01290502, 0x00B1, SpecieID::Goat}, //Filename: goa06

        {0x04FD0001, 0x007B0502, 0x0180, SpecieID::Tiger}, //Filename: tig03
        {0x05000001, 0x00E70502, 0x0183, SpecieID::Tiger}, //Filename: tig06
        {0x04FF0001, 0x01620502, 0x0182, SpecieID::Tiger}, //Filename: tig05
        {0x04FE0001, 0x00590502, 0x0181, SpecieID::Tiger}, //Filename: tig04
        {0x04FA0001, 0x01680502, 0x017D, SpecieID::Tiger}, //Filename: tig00
        {0x04FB0001, 0x01C60502, 0x017E, SpecieID::Tiger}, //Filename: tig01
        {0x04FC0001, 0x02EE0502, 0x017F, SpecieID::Tiger}, //Filename: tig02

        {0x02030001, 0x019A0502, 0x0003, SpecieID::Anteater}, //Filename: ant03
        {0x02080001, 0x00960502, 0x0005, SpecieID::Anteater}, //Filename: ant08
        {0x02010001, 0x016A0502, 0x0001, SpecieID::Anteater}, //Filename: ant01
        {0x02000001, 0x00A10502, 0x0000, SpecieID::Anteater}, //Filename: ant00
        {0x02090001, 0x019F0502, 0x0006, SpecieID::Anteater}, //Filename: ant09
        {0x02020001, 0x01030502, 0x0002, SpecieID::Anteater}, //Filename: ant02
        {0x02060001, 0x03120502, 0x0004, SpecieID::Anteater}, //Filename: ant06

        {0x03BD0001, 0x00F90502, 0x00DF, SpecieID::Koala}, //Filename: kal01
        {0x03C40001, 0x012B0502, 0x00E4, SpecieID::Koala}, //Filename: kal08
        {0x03C60001, 0x00930502, 0x00E6, SpecieID::Koala}, //Filename: kal10
        {0x03C00001, 0x03100502, 0x00E2, SpecieID::Koala}, //Filename: kal04
        {0x03C50001, 0x015C0502, 0x00E5, SpecieID::Koala}, //Filename: kal09
        {0x03BE0001, 0x01980502, 0x00E0, SpecieID::Koala}, //Filename: kal02
        {0x03C10001, 0x00BB0502, 0x00E3, SpecieID::Koala}, //Filename: kal05
        {0x03BF0001, 0x01BC0502, 0x00E1, SpecieID::Koala}, //Filename: kal03
        {0x03BC0001, 0x008A0502, 0x00DE, SpecieID::Koala}, //Filename: kal00

        {0x03AD0001, 0x01B20502, 0x00D7, SpecieID::Horse}, //Filename: hrs09
        {0x03A40001, 0x014F0502, 0x00CE, SpecieID::Horse}, //Filename: hrs00
        {0x03AB0001, 0x03160502, 0x00D5, SpecieID::Horse}, //Filename: hrs07
        {0x03AE0001, 0x00870502, 0x00D8, SpecieID::Horse}, //Filename: hrs10
        {0x03AF0001, 0x012C0502, 0x00D9, SpecieID::Horse}, //Filename: hrs11
        {0x03AA0001, 0x00E60502, 0x00D4, SpecieID::Horse}, //Filename: hrs06
        {0x03A70001, 0x01A10502, 0x00D1, SpecieID::Horse}, //Filename: hrs03
        {0x01010100, 0x00170002, 0x00DD, SpecieID::Horse}, //Filename: hrs15
    /*Non Amiibo*/ {0x00000000, 0x00000000, 0x00DC, SpecieID::Horse}, //Filename: hrs14
        {0x03B10001, 0x00F00502, 0x00DB, SpecieID::Horse}, //Filename: hrs13
        {0x03B00001, 0x01A90502, 0x00DA, SpecieID::Horse}, //Filename: hrs12
        {0x03AC0001, 0x01880502, 0x00D6, SpecieID::Horse}, //Filename: hrs08
        {0x03A80001, 0x00910502, 0x00D2, SpecieID::Horse}, //Filename: hrs04
        {0x03A60001, 0x00C80502, 0x00D0, SpecieID::Horse}, //Filename: hrs02
        {0x03A50001, 0x015B0502, 0x00CF, SpecieID::Horse}, //Filename: hrs01
        {0x03A90001, 0x00710502, 0x00D3, SpecieID::Horse}, //Filename: hrs05

        {0x08000300, 0x00400402, 0x0110, SpecieID::Octopus}, //Filename: oct03
        {0x042A0001, 0x012D0502, 0x010E, SpecieID::Octopus}, //Filename: oct01
        {0x04290001, 0x00700502, 0x010D, SpecieID::Octopus}, //Filename: oct00
        {0x042B0001, 0x01AF0502, 0x010F, SpecieID::Octopus}, //Filename: oct02

        {0x03D60001, 0x01570502, 0x00EF, SpecieID::Lion}, //Filename: lon00
        {0x03E70001, 0x012A0502, 0x00F0, SpecieID::Lion}, //Filename: lon01
        {0x03EA0001, 0x030B0502, 0x00F2, SpecieID::Lion}, //Filename: lon04
        {0x03EE0001, 0x008B0502, 0x00F5, SpecieID::Lion}, //Filename: lon08
        {0x03EC0001, 0x01830502, 0x00F3, SpecieID::Lion}, //Filename: lon06
        {0x03E80001, 0x02F50502, 0x00F1, SpecieID::Lion}, //Filename: lon02
        {0x03ED0001, 0x01A30502, 0x00F4, SpecieID::Lion}, //Filename: lon07    

        {0x028D0001, 0x01BD0502, 0x004D, SpecieID::Bearcub}, //Filename: cbr16
        {0x027D0001, 0x00630502, 0x0040, SpecieID::Bearcub}, //Filename: cbr00
        {0x02870001, 0x005A0502, 0x0049, SpecieID::Bearcub}, //Filename: cbr10
        {0x028C0001, 0x013E0502, 0x004C, SpecieID::Bearcub}, //Filename: cbr15
        {0x028A0001, 0x02E90502, 0x004A, SpecieID::Bearcub}, //Filename: cbr13
        {0x02810001, 0x01200502, 0x0044, SpecieID::Bearcub}, //Filename: cbr04
        {0x027E0001, 0x01690502, 0x0041, SpecieID::Bearcub}, //Filename: cbr01
        {0x028F0101, 0x031A0502, 0x004F, SpecieID::Bearcub}, //Filename: cbr18
        {0x02840001, 0x02FE0502, 0x0047, SpecieID::Bearcub}, //Filename: cbr07
        {0x02860001, 0x03130502, 0x0048, SpecieID::Bearcub}, //Filename: cbr09
        {0x028B0001, 0x00E30502, 0x004B, SpecieID::Bearcub}, //Filename: cbr14
        {0x027F0001, 0x00B90502, 0x0042, SpecieID::Bearcub}, //Filename: cbr02
        {0x02800001, 0x00830502, 0x0043, SpecieID::Bearcub}, //Filename: cbr03
        {0x02820001, 0x01D60502, 0x0045, SpecieID::Bearcub}, //Filename: cbr05
        {0x028E0001, 0x019E0502, 0x004E, SpecieID::Bearcub}, //Filename: cbr17
        {0x02830001, 0x00C70502, 0x0046, SpecieID::Bearcub}, //Filename: cbr06

        {0x04B60001, 0x02EC0502, 0x0159, SpecieID::Rhinocero}, //Filename: rhn04
        {0x04B90001, 0x01600502, 0x015A, SpecieID::Rhinocero}, //Filename: rhn07
        {0x04BA0001, 0x005D0502, 0x015B, SpecieID::Rhinocero}, //Filename: rhn08
        {0x04B30001, 0x00DD0502, 0x0157, SpecieID::Rhinocero}, //Filename: rhn01
        {0x04B40001, 0x030C0502, 0x0158, SpecieID::Rhinocero}, //Filename: rhn02
        {0x04B20001, 0x01B90502, 0x0156, SpecieID::Rhinocero}, //Filename: rhn00

        {0x03710001, 0x005C0502, 0x00BA, SpecieID::Gorilla}, //Filename: gor08
        {0x036B0001, 0x018B0502, 0x00B6, SpecieID::Gorilla}, //Filename: gor02
        {0x036E0001, 0x02FB0502, 0x00B8, SpecieID::Gorilla}, //Filename: gor05
        {0x03690001, 0x00D30502, 0x00B4, SpecieID::Gorilla}, //Filename: gor00
        {0x03730001, 0x01340502, 0x00BC, SpecieID::Gorilla}, //Filename: gor10
        {0x036D0001, 0x03040502, 0x00B7, SpecieID::Gorilla}, //Filename: gor04
        {0x036A0001, 0x019D0502, 0x00B5, SpecieID::Gorilla}, //Filename: gor01
        {0x03740101, 0x03190502, 0x00BD, SpecieID::Gorilla}, //Filename: gor11
        {0x03720001, 0x010B0502, 0x00BB, SpecieID::Gorilla}, //Filename: gor09
        {0x03700001, 0x015D0502, 0x00B9, SpecieID::Gorilla}, //Filename: gor07

        {0x043E0001, 0x01490502, 0x0118, SpecieID::Ostrich}, //Filename: ost08
        {0x043C0001, 0x01CB0502, 0x0116, SpecieID::Ostrich}, //Filename: ost06
        {0x043F0001, 0x01550502, 0x0119, SpecieID::Ostrich}, //Filename: ost09
        {0x04370001, 0x01050502, 0x0112, SpecieID::Ostrich}, //Filename: ost01
        {0x043B0001, 0x03030502, 0x0115, SpecieID::Ostrich}, //Filename: ost05
        {0x043D0001, 0x007C0502, 0x0117, SpecieID::Ostrich}, //Filename: ost07
        {0x04400001, 0x00CA0502, 0x011A, SpecieID::Ostrich}, //Filename: ost10
        {0x04360001, 0x01940502, 0x0111, SpecieID::Ostrich}, //Filename: ost00
        {0x04380001, 0x03000502, 0x0113, SpecieID::Ostrich}, //Filename: ost02
        {0x04390001, 0x03110502, 0x0114, SpecieID::Ostrich}, //Filename: ost03

        {0x03D60001, 0x01570502, 0x00EA, SpecieID::Kangaroo}, //Filename: kgr05
        {0x03D30001, 0x02F30502, 0x00E9, SpecieID::Kangaroo}, //Filename: kgr02
        {0x03D10001, 0x00C20502, 0x00E7, SpecieID::Kangaroo}, //Filename: kgr00
        {0x03DB0001, 0x006D0502, 0x00EE, SpecieID::Kangaroo}, //Filename: kgr10
        {0x03D20001, 0x00E50502, 0x00E8, SpecieID::Kangaroo}, //Filename: kgr01
        {0x03DA0001, 0x01510502, 0x00ED, SpecieID::Kangaroo}, //Filename: kgr09
        {0x03D70001, 0x01B40502, 0x00EB, SpecieID::Kangaroo}, //Filename: kgr06
        {0x03D90001, 0x01A50502, 0x00EC, SpecieID::Kangaroo}, //Filename: kgr08

        {0x044C0001, 0x008E0502, 0x011C, SpecieID::Eagle}, //Filename: pbr01
        {0x044B0001, 0x016C0502, 0x011B, SpecieID::Eagle}, //Filename: pbr00
        {0x04500001, 0x00CF0502, 0x011F, SpecieID::Eagle}, //Filename: pbr05
        {0x044E0001, 0x03150502, 0x011E, SpecieID::Eagle}, //Filename: pbr03
        {0x04540001, 0x01AE0502, 0x0123, SpecieID::Eagle}, //Filename: pbr09
        {0x04510001, 0x015E0502, 0x0120, SpecieID::Eagle}, //Filename: pbr06
        {0x04530001, 0x01040502, 0x0122, SpecieID::Eagle}, //Filename: pbr08
        {0x044D0001, 0x01930502, 0x011D, SpecieID::Eagle}, //Filename: pbr02
        {0x04520001, 0x00730502, 0x0121, SpecieID::Eagle}, //Filename: pbr07

        {0x045F0001, 0x01A80502, 0x0124, SpecieID::Penguin}, //Filename: pgn00
        {0x04690001, 0x01640502, 0x012C, SpecieID::Penguin}, //Filename: pgn10
        {0x04610001, 0x01610502, 0x0126, SpecieID::Penguin}, //Filename: pgn02
        {0x046C0001, 0x008C0502, 0x012F, SpecieID::Penguin}, //Filename: pgn13
        {0x04630001, 0x01310502, 0x0128, SpecieID::Penguin}, //Filename: pgn04
        {0x04640001, 0x00C00502, 0x0129, SpecieID::Penguin}, //Filename: pgn05
        {0x04620001, 0x00F60502, 0x0127, SpecieID::Penguin}, //Filename: pgn03
        {0x046A0001, 0x01D00502, 0x012D, SpecieID::Penguin}, //Filename: pgn11
        {0x04650001, 0x006E0502, 0x012A, SpecieID::Penguin}, //Filename: pgn06
        {0x04600001, 0x00A50502, 0x0125, SpecieID::Penguin}, //Filename: pgn01
        {0x046D0001, 0x00F30502, 0x0130, SpecieID::Penguin}, //Filename: pgn14
        {0x046B0001, 0x01970502, 0x012E, SpecieID::Penguin}, //Filename: pgn12
        {0x04680001, 0x02F20502, 0x012B, SpecieID::Penguin}, //Filename: pgn09

        {0x04010001, 0x00660502, 0x00FD, SpecieID::Monkey}, //Filename: mnk08
        {0x03FE0001, 0x01A40502, 0x00FA, SpecieID::Monkey}, //Filename: mnk05
        {0x03FF0001, 0x00F40502, 0x00FB, SpecieID::Monkey}, //Filename: mnk06
        {0x03FD0001, 0x01580502, 0x00F9, SpecieID::Monkey}, //Filename: mnk04
        {0x03FA0001, 0x00D00502, 0x00F6, SpecieID::Monkey}, //Filename: mnk01
        {0x04000001, 0x006F0502, 0x00Fc, SpecieID::Monkey}, //Filename: mnk07
        {0x03FB0001, 0x01CF0502, 0x00F7, SpecieID::Monkey}, //Filename: mnk02
        {0x03FC0001, 0x01470502, 0x00F8, SpecieID::Monkey}, //Filename: mnk03

        {0x037F0001, 0x01AA0502, 0x00BF, SpecieID::Hamster}, //Filename: ham01
        {0x03830001, 0x009B0502, 0x00C3, SpecieID::Hamster}, //Filename: ham05
        {0x03840001, 0x00860502, 0x00C4, SpecieID::Hamster}, //Filename: ham06
        {0x03800001, 0x01870502, 0x00C0, SpecieID::Hamster}, //Filename: ham02
        {0x037E0001, 0x01560502, 0x00BE, SpecieID::Hamster}, //Filename: ham00
        {0x03850001, 0x01060502, 0x00C5, SpecieID::Hamster}, //Filename: ham07
    /*Non Amiibo*/ {0x00000000, 0x00000000, 0x00C6, SpecieID::Hamster}, //Filename: ham08
        {0x03810001, 0x00D50502, 0x00C1, SpecieID::Hamster}, //Filename: ham03
        {0x03820001, 0x016B0502, 0x00C2, SpecieID::Hamster}, //Filename: ham04

        {0x02D70001, 0x01300502, 0x0065, SpecieID::Deer}, //Filename: der01
        {0x02DD0001, 0x00EA0502, 0x006B, SpecieID::Deer}, //Filename: der07
        {0x02D90001, 0x01C80502, 0x0067, SpecieID::Deer}, //Filename: der03
        {0x02E00101, 0x031D0502, 0x006E, SpecieID::Deer}, //Filename: der10
        {0x02DA0001, 0x01330502, 0x0068, SpecieID::Deer}, //Filename: der04
        {0x02DE0001, 0x009C0502, 0x006C, SpecieID::Deer}, //Filename: der08
        {0x02DF0001, 0x01910502, 0x006D, SpecieID::Deer}, //Filename: der09
        {0x02D60001, 0x00560502, 0x0064, SpecieID::Deer}, //Filename: der00
        {0x02DC0001, 0x00BE0502, 0x006A, SpecieID::Deer}, //Filename: der06
        {0x02DB0001, 0x005E0502, 0x0069, SpecieID::Deer}, //Filename: der05
        {0x02D80001, 0x00E20502, 0x0066, SpecieID::Deer}, //Filename: der02
    };

    const SPAmiiboInfo amiiboSPVillagers[55] = {
        {0x01810001, 0x00440502, 0x2000, 0x01}, //Filename: liz
        {0x018D0001, 0x010C0502, 0x200C, 0x02}, //Filename: xct
        {0x01830001, 0x00450502, 0x2004, 0x03}, //Filename: rci
        {0x01A80001, 0x004F0502, 0xFFFF, 0x04}, //Filename: fob
        {0x01910001, 0x004E0502, 0xFFFF, 0x05}, //Filename: poo
        {0x01920001, 0x010D0502, 0x200B, 0x06}, //Filename: owl
        {0x01AD0001, 0x00B80502, 0x1003, 0x07}, //Filename: pkn
        {0x01820001, 0x00A80502, 0x2007, 0x08}, //Filename: end
        {0x01870001, 0x00470502, 0xFFFF, 0x09}, //Filename: hgs
        {0x01880001, 0x01120502, 0x2006, 0x0A}, //Filename: hgh
        {0x01A00001, 0x010F0502, 0xFFFF, 0x0B}, //Filename: pga
        {0x01A10001, 0x01100502, 0xFFFF, 0x0C}, //Filename: pgb
        {0x018A0001, 0x00A90502, 0x2002, 0x0D}, //Filename: alw
        {0x018B0001, 0x01150502, 0x2003, 0x0E}, //Filename: alp
        {0x01930001, 0x01740502, 0x200A, 0x0F}, //Filename: ows
        {0x01850001, 0x004B0502, 0x200D, 0x10}, //Filename: lrc
        {0x01860301, 0x01750502, 0xFFFF, 0x11}, //Filename: lrc
        {0x018E0001, 0x00490502, 0x2009, 0x12}, //Filename: moo
        {0x018F0001, 0x00B30502, 0xFFFF, 0x13}, //Filename: moc
        {0x01950001, 0x00B00502, 0xFFFF, 0x14}, //Filename: mnk
        {0x01960001, 0x00480502, 0x200E, 0x15}, //Filename: kpp
        {0x01A20001, 0x017D0502, 0x100D, 0x16}, //Filename: seg
        {0x01A70001, 0x01140502, 0x100B, 0x17}, //Filename: wrl
        {0x01900001, 0x01710502, 0xFFFF, 0x18}, //Filename: pge
        {0x01AA0001, 0x00530502, 0xFFFF, 0x19}, //Filename: ott
        {0x01B60001, 0x00AE0502, 0xFFFF, 0x1A}, //Filename: lom
        {0x01B40001, 0x01130502, 0xFFFF, 0x1B}, //Filename: slo
        {0x01AC0001, 0x017F0502, 0x1004, 0x1C}, //Filename: pyn
        {0x01A40001, 0x004D0502, 0x100C, 0x1D}, //Filename: seo
        {0x01A60001, 0x00500502, 0x100E, 0x1E}, //Filename: cml
        {0x01AE0001, 0x011B0502, 0x1009, 0x1F}, //Filename: tuk
        {0x01A90001, 0x01760502, 0xFFFF, 0x20}, //Filename: grf
        {0x01B10001, 0x00B20502, 0xFFFF, 0x21}, //Filename: upa
        {0x01A50001, 0x01720502, 0xFFFF, 0x23}, //Filename: bpt
        {0x019F0001, 0x01110502, 0xFFFF, 0x24}, //Filename: plb
        {0x01AF0001, 0x011C0502, 0x1002, 0x25}, //Filename: snt
        {0x019A0001, 0x00B70502, 0x1007, 0x26}, //Filename: bev
        {0x018C0001, 0x004C0502, 0x2001, 0x27}, //Filename: szo
        {0x01890001, 0x00AB0502, 0xFFFF, 0x29}, //Filename: hgc
        {0x01940001, 0x00AA0502, 0x2008, 0x2A}, //Filename: skk
        {0x019B0001, 0x00B60502, 0x1008, 0x2B}, //Filename: chm
        {0x01AB0001, 0x017C0502, 0x1006, 0x2C}, //Filename: pck
        {0x01B50001, 0x00510502, 0xFFFF, 0x30}, //Filename: tap
        {0x019C0001, 0x01730502, 0xFFFF, 0x34}, //Filename: bln
        {0x01A30001, 0x004A0502, 0x100A, 0x36}, //Filename: boa
        {0x019D0001, 0x00AC0502, 0x1000, 0x37}, //Filename: plc
        {0x019E0001, 0x00AD0502, 0x1001, 0x38}, //Filename: pla
        {0x01B30001, 0x00B50502, 0x1005, 0x39}, //Filename: mka
        {0x01970001, 0x01770502, 0xFFFF, 0x3D}, //Filename: kpm
        {0x01990001, 0x01160502, 0xFFFF, 0x3E}, //Filename: kpg
        {0x01980001, 0x00B10502, 0xFFFF, 0x3F}, //Filename: kps
        {0x01B00001, 0x00520502, 0xFFFF, 0x41}, //Filename: ttl
        {0x01C10101, 0x017A0502, 0x2005, 0x48}, //Filename: 

        {0x01800000, 0x00080002, 0x200F, 0xFE}, //SPECIAL CASE: "Villager"
        {0x01840000, 0x024D0502, 0xFFFF, 0xFF}, //SPECIAL CASE: "Timmy&Tommy"
    };
}