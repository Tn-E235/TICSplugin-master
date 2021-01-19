#include "stdafx.h"
#include "atsplugin.h"
#include "ini.h"
#include "Tics.h"
#include "Ats.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	char filePath[_MAX_PATH + 1] = _T("");						//ファイルパス格納
	char* posIni;												//検索文字列へのポインタ
	bool loadCheck;												//INIファイルのロードに成功したかどうか
	::GetModuleFileName((HMODULE)hModule, filePath, _MAX_PATH);	//Ats.dllのファイルパスを取得
	posIni = strstr(filePath, ".dll");							//パスから.dllの位置を検索
	memmove(posIni, ".ini", 4);									//.dllを.iniに置換
	loadCheck = g_ini.load(filePath);							//INIファイルをロードして結果を取得
	tics.setFlashStopSetting(g_ini.FLASH.time1, 1);
	tics.setFlashStopSetting(g_ini.FLASH.time2, 2);
	tics.setFlashStopSetting(g_ini.FLASH.maxIdx, 3);
	return TRUE; 
}
ATS_API int WINAPI GetPluginVersion() { return ATS_VERSION; }
ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC vehicleSpec) {
	g_svcBrake = vehicleSpec.BrakeNotches;
	g_emgBrake = g_svcBrake + 1;
}
ATS_API void WINAPI Initialize(int brake) { tics.init(); }
ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE vehicleState, int* panel, int* sound) {
	g_deltaT = vehicleState.Time - g_time;
	g_time = vehicleState.Time;
	g_speed = vehicleState.Speed;

	if (g_pilotlamp) { g_output.Reverser = g_reverser; }
	else { g_output.Reverser = 0; }
	g_output.Brake = g_brakeNotch;
	g_output.Power = g_powerNotch;
	g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;

	tics.run(vehicleState, panel, sound);

	return g_output;
}
ATS_API void WINAPI SetPower(int notch) { g_powerNotch = notch; }
ATS_API void WINAPI SetBrake(int notch) { g_brakeNotch = notch; }
ATS_API void WINAPI SetReverser(int pos) { 
	g_reverser = pos; 
	tics.setReverser(pos);
}
ATS_API void WINAPI KeyDown(int atsKeyCode) {}
ATS_API void WINAPI KeyUp(int hornType) {}
ATS_API void WINAPI HornBlow(int atsHornBlowIndex) {}
ATS_API void WINAPI DoorOpen() {
	g_pilotlamp = false;
	tics.setDoorState(true);
}
ATS_API void WINAPI DoorClose() {
	g_pilotlamp = true;
	tics.setDoorState(false);
}
ATS_API void WINAPI SetSignal(int signal) {}
ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA beaconData) {
	int sendData = beaconData.Optional;
	switch (beaconData.Type) {
		case SET_TICS_UPDATE:
			tics.setUpdate(sendData);
			break;
		case SET_TICS_TRAIN_INFO:
			tics.setTrainInfo(sendData);
			break;
		case SET_TICS_TRAIN_NUMBER:
			tics.setTrainNumber(sendData);
			break;
		case SET_TICS_SECTION:
			tics.setRunningSection(sendData);
			break;
		case SET_TICS_STA:
			tics.setStaName(sendData);
			break;
		case SET_TICS_ARR_TIME:
			tics.setArrTime(sendData);
			break;
		case SET_TICS_LEV_TIME:
			tics.setLevTime(sendData);
			break;
		case SET_TICS_STA_WORK:
			tics.setStaWork(sendData);
			break;
		case SET_TICS_EXTRA_STOP:
			tics.setExtraStop(sendData);
			break;
		case SET_TICS_NEXT_STA:
			tics.setNextStop(sendData);
			break;
		case SET_TICS_DISP_STOP:
			tics.setStopFlash(sendData);
			break;
		case 337:
			tics.setOperateSetting(sendData);
			break;
		default:
			break;
	}
}
ATS_API void WINAPI Load() {}
ATS_API void WINAPI Dispose() {}