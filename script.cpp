#include "script.h"
#include "keyboard.h"
#include <string>
#include <vector>

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#pragma warning(disable : 4244 4305) // double <-> float conversions

void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7) {
	GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, A_4, A_5, A_6, A_7);
}

void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true) {
	// default values
	int text_col[4] = {255, 255, 255, 255},
		rect_col[4] = {70, 95, 95, 255};
	float text_scale = 0.35;
	int font = 0;

	// correcting values for active line
	if (active) {
		text_col[0] = 0;
		text_col[1] = 0;
		text_col[2] = 0;

		rect_col[0] = 218;
		rect_col[1] = 242;
		rect_col[2] = 216;

		if (rescaleText) text_scale = 0.40;
	}

	if (title) {
		rect_col[0] = 0;
		rect_col[1] = 0;
		rect_col[2] = 0;

		if (rescaleText) text_scale = 0.50;
		font = 1;
	}

	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	textLeft += lineLeft;

	float lineWidthScaled = lineWidth / (float)screen_w; // line width
	float lineTopScaled = lineTop / (float)screen_h; // line top offset
	float textLeftScaled = textLeft / (float)screen_w; // text left offset
	float lineHeightScaled = lineHeight / (float)screen_h; // line height

	float lineLeftScaled = lineLeft / (float)screen_w;

	// this is how it's done in original scripts

	// text upper part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	UI::_DRAW_TEXT(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// text lower part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_GXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	int num25 = UI::_0x9040DFB09BE75706(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// rect
	draw_rect(lineLeftScaled, lineTopScaled + (0.00278f),
		lineWidthScaled, ((((float)(num25)* UI::_0xDB88A37483346780(text_scale, 0)) + (lineHeightScaled * 2.0f)) + 0.005f),
		rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
}

bool menu_switch_pressed() {
	return IsKeyJustUp(VK_F4);
}

void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r) {
	if (a) *a = IsKeyDown(VK_NUMPAD5);
	if (b) *b = IsKeyDown(VK_NUMPAD0) || menu_switch_pressed() || IsKeyDown(VK_BACK);
	if (up) *up = IsKeyDown(VK_NUMPAD8);
	if (down) *down = IsKeyDown(VK_NUMPAD2);
	if (r) *r = IsKeyDown(VK_NUMPAD6);
	if (l) *l = IsKeyDown(VK_NUMPAD4);
}

void menu_beep() {
	AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}

std::string statusText;
DWORD statusTextDrawTicksMax;
bool statusTextGxtEntry;

void update_status_text() {
	if (GetTickCount() < statusTextDrawTicksMax) {
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
		if (statusTextGxtEntry) {
			UI::_SET_TEXT_ENTRY((char *)statusText.c_str());
		}
		else {
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((char *)statusText.c_str());
		}
		UI::_DRAW_TEXT(0.5, 0.5);
	}
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false) {
	statusText = str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}

// FROM SHOW COORDS MOD
void draw_text(char *text, float x, float y, float scale) {
	UI::SET_TEXT_FONT(0);
	UI::SET_TEXT_SCALE(scale, scale);
	UI::SET_TEXT_COLOUR(0, 255, 0, 245);
	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(2, 2, 0, 0, 0);
	UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}

/* REMOVED VEHICLES:
	BOATTRAILER BUS ARMYTANKER ARMYTRAILER
	ARMYTRAILER2 SUNTRAP COACH AIRBUS
	AIRTUG BARRACKS BARRACKS2 BENSON
	BIFF BOXVILLE BOXVILLE2 BOXVILLE3
	BULLDOZER BLIMP CABLECAR CAMPER
	CUTTER DUMP RUBBLE DOCKTUG
	FORKLIFT HAULER HANDLER JOURNEY
	MIXER MIXER2 MOWER MULE
	MULE2 PACKER PBUS PHANTOM
	POUNDER PROPTRAILER RENTALBUS RHINO
	RIOT RIPLEY SCRAP STOCKADE
	STOCKADE3 TACO TRASH TRACTOR
	TRACTOR2 GRAINTRAILER BALETRAILER TIPTRUCK
	TIPTRUCK2 UTILLITRUCK UTILLITRUCK2 UTILLITRUCK3
	SCORCHER TRIBIKE TRIBIKE2 TRIBIKE3
	FIXTER CRUISER BMX FAGGIO2
	ANNIHILATOR BUZZARD BUZZARD2 CARGOBOB
	CARGOBOB2 CARGOBOB3 SKYLIFT POLMAV
	MAVERICK FROGGER FROGGER2 CUBAN800
	DUSTER STUNT MAMMATUS JET
	SHAMAL LUXOR TITAN LAZER
	CARGOPLANE SQUALO MARQUIS DINGHY
	DINGHY2 JETMAX PREDATOR TROPIC
	SEASHARK SEASHARK2 SUBMERSIBLE TRAILERS
	TRAILERS2 TRAILERS3 TVTRAILER RAKETRAILER
	TANKER TRAILERLOGS TR2 TR3
	TR4 TRFLAT TRAILERSMALL VELUM
	SPEEDER VESTRA MILJET DODO
	SUBMERSIBLE2 HYDRA SAVAGE VALKYRIE
	VELUM2 SWIFT2 LUXOR2 BESRA
	SWIFT BLIMP2 TORO CARGOBOB4
	DINGHY4 SEASHARK3 SPEEDER2 SUPERVOLITO
	SUPERVOLITO2 TORO2 TROPIC2 VALKYRIE2
*/
std::vector<std::string> vehicleModels = {
	"NINEF", "NINEF2", "BLISTA", "ASEA", "ASEA2", "ASTEROPE", "AMBULANCE", "BALLER", "BALLER2",
	"BJXL", "BANSHEE", "BFINJECTION", "BLAZER", "BLAZER2", "BLAZER3", "BISON", "BISON2",
	"BISON3", "BOBCATXL", "BODHI2", "BUCCANEER", "BUFFALO", "BUFFALO2",
	"BULLET", "BURRITO", "BURRITO2", "BURRITO3", "BURRITO4", "BURRITO5", "CAVALCADE", "CAVALCADE2", "POLICET",
	"GBURRITO", "CADDY", "CADDY2", "CARBONIZZARE", "CHEETAH", "COMET2", "COGCABRIO", "COQUETTE",
	"GRESLEY", "DILETTANTE", "DILETTANTE2", "DUNE", "DUNE2", "HOTKNIFE", "DLOADER", "DUBSTA", "DUBSTA2",
	"DOMINATOR", "EMPEROR", "EMPEROR2", "EMPEROR3", "ENTITYXF", "EXEMPLAR", "ELEGY2",
	"F620", "FBI", "FBI2", "FELON", "FELON2", "FELTZER2", "FIRETRUK", "FQ2",
	"FUSILADE", "FUGITIVE", "FUTO", "GRANGER", "GAUNTLET", "HABANERO", "INFERNUS", "INGOT",
	"INTRUDER", "ISSI2", "JACKAL", "JB700", "KHAMELION", "LANDSTALKER", "LGUARD", "MANANA", "MESA",
	"MESA2", "MESA3", "CRUSADER", "MINIVAN", "MONROE",
	"ORACLE", "ORACLE2", "PATRIOT", "PENUMBRA", "PEYOTE", "PHOENIX", "PICADOR",
	"POLICE", "POLICE4", "POLICE2", "POLICE3", "POLICEOLD1", "POLICEOLD2", "PONY", "PONY2", "PRAIRIE",
	"PRANGER", "PREMIER", "PRIMO", "RANCHERXL", "RANCHERXL2", "RAPIDGT", "RAPIDGT2", "RADI", "RATLOADER",
	"REBEL", "REGINA", "REBEL2", "RUINER", "RUMPO", "RUMPO2",
	"ROCOTO", "ROMERO", "SABREGT", "SADLER", "SADLER2", "SANDKING", "SANDKING2", "SCHAFTER2", "SCHWARZER",
	"SEMINOLE", "SENTINEL", "SENTINEL2", "ZION", "ZION2", "SERRANO", "SHERIFF", "SHERIFF2", "SPEEDO", "SPEEDO2",
	"STANIER", "STINGER", "STINGERGT", "STRATUM", "SULTAN", "SUPERD", "SURANO", "SURFER",
	"SURFER2", "SURGE", "TAILGATER", "TAXI", "TORNADO", "TORNADO2", "TORNADO3", "TORNADO4", "TOWTRUCK2",
	"VOODOO2", "WASHINGTON", "STRETCH", "YOUGA", "ZTYPE", "SANCHEZ", "SANCHEZ2", "POLICEB", "AKUMA", "CARBONRS",
	"BAGGER", "BATI", "BATI2", "RUFFIAN", "DAEMON", "DOUBLE", "PCJ", "VADER", "VIGERO", "HEXER", "NEMESIS",
	"ADDER", "VOLTIC", "VACCA", "BIFTA",
	"PARADISE", "KALAHARI", "JESTER", "TURISMOR", "ALPHA", "HUNTLEY", "THRUST", "MASSACRO",
	"MASSACRO2", "ZENTORNO", "BLADE", "GLENDALE", "PANTO", "PIGALLE", "WARRENER", "RHAPSODY", "DUBSTA3", "MONSTER",
	"SOVEREIGN", "INNOVATION", "HAKUCHOU", "FUROREGT", "COQUETTE2", "BTYPE", "BUFFALO3", "DOMINATOR2", "GAUNTLET2",
	"MARSHALL", "DUKES", "DUKES2", "STALION", "STALION2", "BLISTA2", "BLISTA3",
	"INSURGENT", "INSURGENT2", "TECHNICAL", "KURUMA", "KURUMA2", "JESTER2", "CASCO",
	"GUARDIAN", "ENDURO", "LECTRO", "SLAMVAN", "SLAMVAN2", "RATLOADER2", "FELTZER3", "OSIRIS",
	"VIRGO", "WINDSOR", "VINDICATOR", "T20", "COQUETTE3", "CHINO",
	"BRAWLER", "BUCCANEER2", "CHINO2", "FACTION", "FACTION2", "MOONBEAM", "MOONBEAM2", "PRIMO2", "VOODOO", "LURCHER",
	"BTYPE2", "BALLER3", "BALLER4", "BALLER5", "BALLER6", "COG55", "COG552", "COGNOSCENTI", "COGNOSCENTI2",
	"LIMO2", "MAMBA", "NIGHTSHADE", "SCHAFTER3", "SCHAFTER4", "SCHAFTER5", "SCHAFTER6",
	"VERLIERER2", "TAMPA", "BANSHEE2", "SULTANRS", "BTYPE3"
};

void set_player_current_vehicle(std::string modelName) {
	DWORD model = GAMEPLAY::GET_HASH_KEY((char *)modelName.c_str());
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model)) {
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);

		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, 0.0, 1, 1);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);

		float prevVehicleSpeed = 0.0f;

		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) {
			Vehicle currentVehicle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
			prevVehicleSpeed = ENTITY::GET_ENTITY_SPEED(currentVehicle);

			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(currentVehicle, true, true);
			VEHICLE::DELETE_VEHICLE(&currentVehicle);
		}

		ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
		if (prevVehicleSpeed > 0.0f) {
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, prevVehicleSpeed);
		}
		AUDIO::SET_VEHICLE_RADIO_ENABLED(veh, false);
		PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);

		WAIT(0);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
	}
}

void load_vehicle_collection(std::string collectionPath) {

	char status[128];
	sprintf(status, "Loading vehicle collection: VRVC/%s", collectionPath.c_str());
	set_status_text(status);

	std::string line;
	std::ifstream collectionFile("VRVC/" + collectionPath);
	if (collectionFile.is_open()) {
		vehicleModels.clear();
		while (std::getline(collectionFile, line)) {
			vehicleModels.push_back(line);
		}
		sprintf(status, "Loaded vehicle collection: VRVC/%s (%zu cars)", collectionPath.c_str(), vehicleModels.size());
		set_status_text(status, 4500UL);
	}
	else {
		sprintf(status, "Unable to load vehicle collection: VRVC/%s", collectionPath.c_str());
		set_status_text(status);
	}

	collectionFile.close();
}

int previousRandVehicleIndex = -1;
std::string get_random_vehicle_model() {
	int randVehicleIndex = (rand() % vehicleModels.size());
	while (randVehicleIndex == previousRandVehicleIndex) {
		randVehicleIndex = (rand() % vehicleModels.size());
	}
	std::string randVehicleModel = vehicleModels[randVehicleIndex];
	previousRandVehicleIndex = randVehicleIndex;

	return randVehicleModel;
}

int activeLineIndexMain = 0;
int activeLineIndexCollections = 0;

void process_collections_menu() {
	const float lineWidth = 250.0;

	std::string caption = "VEHICLE COLLECTIONS";

	std::vector<fs::path> lineCaption;
	for (const auto &entry : fs::directory_iterator("VRVC")) {
		if (entry.is_regular_file()) {
			fs::path file_path = fs::path(entry.path()).filename();

			lineCaption.push_back(file_path);
		}
	}

	DWORD waitTime = 150;
	while (true) {
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do {
			// draw menu
			draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < lineCaption.size(); i++) {
				if (i != activeLineIndexCollections) {
					std::string collectionName = lineCaption[i].string();
					collectionName = collectionName.substr(0, collectionName.find("."));
					draw_menu_line(collectionName, lineWidth, 9.0, 60.0 + i * 36.0, 0.0, 9.0, false, false);
				}
			}
			std::string collectionName = lineCaption[activeLineIndexCollections].string();
			collectionName = collectionName.substr(0, collectionName.find("."));
			draw_menu_line(collectionName, lineWidth + 1.0, 11.0, 56.0 + activeLineIndexCollections * 36.0, 0.0, 7.0, true, false);

			update_status_text();
			WAIT(0);
		}
		while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect) {
			menu_beep();
			std::string collectionPath = lineCaption[activeLineIndexCollections].string();
			load_vehicle_collection(collectionPath);
			waitTime = 200;
			break;
		}
		else
			if (bBack || menu_switch_pressed()) {
				menu_beep();
				break;
			}
			else if (bUp) {
				menu_beep();
				if (activeLineIndexCollections == 0)
					activeLineIndexCollections = lineCaption.size();
				activeLineIndexCollections--;
				waitTime = 150;
			}
			else if (bDown) {
				menu_beep();
				activeLineIndexCollections++;
				if (activeLineIndexCollections == lineCaption.size())
					activeLineIndexCollections = 0;
				waitTime = 150;
			}
	}
}

void process_main_menu() {
	const float lineWidth = 250.0;
	const int lineCount = 4;

	std::string caption = "VEHICLE RANDOMIZER";

	static LPCSTR lineCaption[lineCount] = {
		"RANDOM VEHICLE",
		"NEXT VEHICLE",
		"FIRST VEHICLE",
		"VEHICLE COLLECTIONS"
	};

	DWORD waitTime = 150;
	while (true) {
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do {
			// draw menu
			draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexMain)
					draw_menu_line(lineCaption[i], lineWidth, 9.0, 60.0 + i * 36.0, 0.0, 9.0, false, false);
			draw_menu_line(lineCaption[activeLineIndexMain], lineWidth + 1.0, 11.0, 56.0 + activeLineIndexMain * 36.0, 0.0, 7.0, true, false);

			update_status_text();
			WAIT(0);
		}
		while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect) {
			menu_beep();
			if (activeLineIndexMain == 0) {
				std::string randVehicleModel = get_random_vehicle_model();
				set_player_current_vehicle(randVehicleModel);
			}
			else if (activeLineIndexMain == 1) {
				int index = (previousRandVehicleIndex + 1) % vehicleModels.size();
				previousRandVehicleIndex = index;
				set_player_current_vehicle(vehicleModels[index]);
			}
			else if (activeLineIndexMain == 2) {
				previousRandVehicleIndex = 0;
				set_player_current_vehicle(vehicleModels[0]);
			}
			else if (activeLineIndexMain == 3) {
				process_collections_menu();
			}
			waitTime = 200;
		}
		else
			if (bBack || menu_switch_pressed()) {
				menu_beep();
				break;
			}
			else
				if (bUp) {
					menu_beep();
					if (activeLineIndexMain == 0)
						activeLineIndexMain = lineCount;
					activeLineIndexMain--;
					waitTime = 150;
				}
				else
					if (bDown) {
						menu_beep();
						activeLineIndexMain++;
						if (activeLineIndexMain == lineCount)
							activeLineIndexMain = 0;
						waitTime = 150;
					}
	}
}


void main() {
	//bool widescreen = GRAPHICS::GET_IS_WIDESCREEN();

	bool randomCarsEnabled = false;

	Any cumulativeStuntJumpsCompleted = GAMEPLAY::_0x6856EC3D35C81EA4();

	while (true) {
		//If you're in a taxi, triggers a random vehicle
		if(PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), true)) {
			if(ENTITY::GET_ENTITY_MODEL(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true)) == GAMEPLAY::GET_HASH_KEY("TAXI")) {
				set_player_current_vehicle(get_random_vehicle_model());
			}
		}

		// playerPed = PLAYER::PLAYER_PED_ID();
		//widescreen = GRAPHICS::GET_IS_WIDESCREEN();

		/*char statName[16] = "USJS_COMPLETED";
		Hash statHash = GAMEPLAY::GET_HASH_KEY(statName);

		int stuntJumpsCompleted;
		STATS::STAT_GET_INT(statHash, &stuntJumpsCompleted, -1);*/

		/*bool stuntJumpInProgress = GAMEPLAY::IS_STUNT_JUMP_IN_PROGRESS();
		bool stuntJumpResultsOpen = GAMEPLAY::_0x2272B0A1343129F4();*/

		//Any stuntJumpsCompleted = GAMEPLAY::_0x996DD1E1E02F1008();
		Any cmJumps = GAMEPLAY::_0x6856EC3D35C81EA4();

		if (cmJumps > cumulativeStuntJumpsCompleted) {

			if (randomCarsEnabled) {
				std::string randVehicleModel = get_random_vehicle_model();
				set_player_current_vehicle(randVehicleModel);
			}
			else {
				randomCarsEnabled = true;
			}
			cumulativeStuntJumpsCompleted = cmJumps;
		}

		if (menu_switch_pressed()) {
			menu_beep();
			process_main_menu();
		}

		//char coords[128];

		//sprintf_s(coords, "RANDOM CARS ENABLED: %i", randomCarsEnabled);
		//sprintf_s(coords, "STUNT IN PROGRESS: %i, STUNT RESULTS OPEN: %i, STUNT JUMPS COMPLETED: %lu (%lu)", stuntJumpInProgress, stuntJumpResultsOpen, stuntJumpsCompleted, cumulativeStuntJumpsCompleted);
		//widescreen ? draw_text(coords, 0.205 - 0.03, 0.9, 0.5) : draw_text(coords, 0.205, 0.9, 0.5);

		update_status_text();
		WAIT(0);
	}
}

void ScriptMain() {
	srand(GetTickCount());
	main();
}
