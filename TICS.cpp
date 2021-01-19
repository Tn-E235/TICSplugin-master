#include "stdafx.h"
#include "atsplugin.h"
#include "Tics.h"
// -----------------------------------------------------------------------
void TICS::init() {
	deltaT = 0;
	time = 0;
	speed = 0.0;
	doorOpen = false;
	stopFlash.interval = 0;
}
// -----------------------------------------------------------------------
void TICS::run(ATS_VEHICLESTATE vehicleState, int* panel, int* sound) {
	deltaT = vehicleState.Time - time;
	time = vehicleState.Time;
	speed = vehicleState.Speed;
	if (isUpdate()) {
		// 即時更新またはプッシュアップ更新
		if (doorOpen || uInfo.type > 1) {
			pushUp(panel);
		}
	}
	flashNextStop(vehicleState, panel);
	if (oSet.stopNotice) flashStopDisp(vehicleState, panel, sound);
	if (doorOpen) runDis = 0.0;
	runDis += (speed / 3.6) * deltaT;
	panel[73] = time / 1000 / 3600;		// 現在時刻H
	panel[74] = time / 1000 / 60 % 60;	// 現在時刻M
	panel[75] = time / 1000 % 60;		// 現在時刻S
	panel[76] = oSet.conductorSet[0];	// 現在時刻下の表示枠1
	panel[77] = oSet.conductorSet[1];	// 現在時刻下の表示枠2
	panel[78] = oSet.conductorSet[2];	// 現在時刻下の表示枠3
	panel[79] = oSet.conductorSet[3];	// 現在時刻下の表示枠4
	panel[80] = oSet.conductorSet[4];	// 現在時刻下の表示枠5
	panel[81] = oSet.conductorSet[5];	// 現在時刻下の表示枠6
	panel[84] = (int)runDis / 1000 / 1000;
	panel[85] = (int)runDis / 1000 / 100 % 10;
	panel[88] = reverser;
}
// -----------------------------------------------------------------------
void TICS::setDoorState(bool b) { doorOpen = b; }
// -----------------------------------------------------------------------
void TICS::setUpdate(int num) {
	//               1/011/094
	int type = num / 1000000;
	int pushNum = num / 1000 % 1000;
	int option = num % 1000;
	if (type < 0 && type > 3) return;
	if (pushNum > ARRAY_MAX_NUM) return;
	uInfo.update = true;
	uInfo.type = type;
	uInfo.pushNum = pushNum;
	uInfo.option = option;
}
// -----------------------------------------------------------------------
void TICS::setTrainInfo(int num) {
	int n = num / 1000000;
	if (n > 3) return;
	int type = num / 10000 % 100;
	int distination = num / 100 % 100;
	int carNumber = num % 100;
	tInfo[n - 1].type = type;
	tInfo[n - 1].distination = distination;
	tInfo[n - 1].carNumber = carNumber;
}
// -----------------------------------------------------------------------
void TICS::setTrainNumber(int num) {
	           // 10123405
	int n = num / 10000000;
	if (n > 3) return;
	int num0 = num / 1000000;		// 未使用
	int num1 = num / 100000 % 10;
	int num2 = num / 10000 % 10;
	int num3 = num / 1000 % 10;
	int num4 = num / 100 % 10;
	int num5 = num % 100;			// アルファベット対応
	// 桁取得
	int d = 0;
	int number = num % 10000000 / 10;
	for (int i = 0; i < 10; ++i) {
		if (number / pow(10, i) < 1) {
			d = i;
			break;
		}
	}
	tInfo[n - 1].trainNumber[0] = 0;
	tInfo[n - 1].trainNumber[1] = (d >= 5) ? num1:40;
	tInfo[n - 1].trainNumber[2] = (d >= 4) ? num2:40;
	tInfo[n - 1].trainNumber[3] = (d >= 3) ? num3:40;
	tInfo[n - 1].trainNumber[4] = (d >= 2) ? num4:40;
	tInfo[n - 1].trainNumber[5] = (d >= 1) ? num5:40;
}
// -----------------------------------------------------------------------
void TICS::setRunningSection(int num) {
	int n = num / 1000;
	if (n > 3) return;
	tInfo[n - 1].runnigSection = num % 1000;
}
// -----------------------------------------------------------------------
void TICS::setStaName(int num) {
	int n = num / 1000000;
	if (n > ARRAY_MAX_NUM || n < 1) return;
	// tTp[n - 1]->staName = num % 1000;
	tTable[n - 1].staName = num % 1000;
}
// -----------------------------------------------------------------------
void TICS::setArrTime(int num) {
	int n = num / 1000000;
	if (n > ARRAY_MAX_NUM || n < 1) return;
	// tTp[n - 1]->arrTime = num % 1000000;
	tTable[n - 1].arrTime = num % 1000000;
}
// -----------------------------------------------------------------------
void TICS::setLevTime(int num) {
	int n = num / 1000000;
	if (n > ARRAY_MAX_NUM || n < 1) return;
	// tTp[n - 1]->levTime = num % 1000000;
	tTable[n - 1].levTime = num % 1000000;
}
// -----------------------------------------------------------------------
void TICS::setStaWork(int num) {
	int n = num / 1000000;
	if (n > ARRAY_MAX_NUM || n < 1) return;
	tTable[n - 1].isWaitPass = (num / 1000 % 1000 == 1) ? true : false;
	tTable[n - 1].isDissolution = (num % 1000 == 1) ? true : false;
}
// -----------------------------------------------------------------------
// void TICS::setWaitPass(int num) {
// 	int n = num / 1000;
// 	if (n > 20) return;
// 	tTable[n - 1].isWaitPass = (num % 1000 != 1) ? false : true;
// }
// -----------------------------------------------------------------------
// void TICS::setDissolution(int num) {
// 	int n = num / 1000;
// 	if (n > 20) return;
// 	tTable[n - 1].isDissolution = (num % 1000 != 1) ? false : true;
// }
// -----------------------------------------------------------------------
void TICS::setExtraStop(int num) {
	int n = num / 1000;
	if (n > 3) return;
	tInfo[n - 1].extraStop = num % 1000;
}
// -----------------------------------------------------------------------
void TICS::setStopFlash(int num) {
	// 500/01/05/
	stopFlash.flashDis = num / 10000;		// 距離
	// stopFlash.count[0] = num / 100 % 100;	// 画像先頭番号
	// stopFlash.count[1] = num % 100;			// 画像末尾番号
	stopFlash.state = true;
}
// -----------------------------------------------------------------------
void TICS::setNextStop(int num) {
	// 500/01/02
	nextStop.flashDis = num / 10000;		// 距離
	nextStop.count[0] = num / 100 % 100;	// 次駅
	nextStop.count[1] = num % 100;			// 次々駅
	nextStop.state = true;
}
// -----------------------------------------------------------------------
void TICS::setReverser(int num) {
	switch (num) {
		case 1:		// F
			reverser = 1;
			break;
		case 0:		// N
			reverser = 0;
			break;
		case -1:	// B
			reverser = 2;
			break;
	}
}
// -----------------------------------------------------------------------
void TICS::setFlashStopSetting(int num, int type) {
	if (type == 1) stopFlash.interval = num;
	if (type == 2) nextStop.interval = num;
	if (type == 3) stopFlash.count[1] = num;
}
// -----------------------------------------------------------------------
void TICS::setOperateSetting(int num) {
	if (num < 0) return;
	oSet.stopNotice = (num >= 10000000) ? true : false;
	oSet.day = num/1000000%10;
	oSet.conductorSet[0] = num/100000%10;
	oSet.conductorSet[1] = num/10000%10;
	oSet.conductorSet[2] = num/1000%10;
	oSet.conductorSet[3] = num/100%10;
	oSet.conductorSet[4] = num/10%10;
	oSet.conductorSet[5] = num%10;
}
// -----------------------------------------------------------------------
bool TICS::isUpdate() { return uInfo.update; }
// -----------------------------------------------------------------------
void TICS::pushUp(int* panel) {
	int opNum = 0;
	int bina = uInfo.option;
	bool push = (uInfo.type < 3) ? true : false;
	for (int i = 0; bina > 0; i++) {
		opNum = opNum + (bina % 2) * pow(10, i);
		bina = bina / 2;
	}
	int option[7];
	//                  1000000
	option[0] = opNum / 1000000;	// 走行区間
	option[1] = opNum / 100000 % 10;// テーブル
	option[2] = opNum / 10000 % 10;	// 種別
	option[3] = opNum / 1000 % 10;	// 行先
	option[4] = opNum / 100 % 10;	// 列車番号
	option[5] = opNum / 10 % 10;	// 両数
	option[6] = opNum % 10;			// 臨時停車駅

	// 走行区間更新
	if (option[0] == 1) {
		if (push) {
			tInfo[0].runnigSection = tInfo[1].runnigSection;
			tInfo[1].runnigSection = tInfo[2].runnigSection;
			tInfo[2].runnigSection = 0;
		}
		// パネル反映
		panel[82] = tInfo[0].runnigSection;
		panel[83] = tInfo[1].runnigSection;
		// MessageBox(NULL, TEXT("走行区間更新"), TEXT("pushUp"), MB_OK);
	}
	// テーブル更新
	if (option[1] == 1) {
		int pushNum = uInfo.pushNum;
		if (push && pushNum > 0) {
			for (int i = 0; i + pushNum < ARRAY_MAX_NUM; ++i) {
				tTable[i].arrTime = tTable[i + pushNum].arrTime;
				tTable[i].levTime = tTable[i + pushNum].levTime;
				tTable[i].isWaitPass = tTable[i + pushNum].isWaitPass;
				tTable[i].isDissolution = tTable[i + pushNum].isDissolution;
			}
			for (int i = ARRAY_MAX_NUM - pushNum; i < ARRAY_MAX_NUM; ++i) {
				tTable[i].staName = 0;
				tTable[i].arrTime = 0;
				tTable[i].levTime = 0;
				tTable[i].isWaitPass = 0;
				tTable[i].isDissolution = 0;
			}
		}
		// パネル反映(10段まで)
		for (int i = 0; i < 10; ++i) {
			panel[218 + i] = tTable[i].staName;
			panel[136 + i * 3] = getDigitOfTime(tTable[i].arrTime, 3);
			panel[137 + i * 3] = getDigitOfTime(tTable[i].arrTime, 2);
			panel[138 + i * 3] = getDigitOfTime(tTable[i].arrTime, 1);
			panel[166 + i * 3] = getDigitOfTime(tTable[i].levTime, 3);
			panel[167 + i * 3] = getDigitOfTime(tTable[i].levTime, 2);
			panel[168 + i * 3] = getDigitOfTime(tTable[i].levTime, 1);
			panel[228 + i] = tTable[i].isWaitPass;
		}
		// 11段目
		panel[51] = tTable[10].staName;
		panel[52] = getDigitOfTime(tTable[10].arrTime, 3);
		panel[53] = getDigitOfTime(tTable[10].arrTime, 2);
		panel[54] = getDigitOfTime(tTable[10].arrTime, 1);
		panel[55] = getDigitOfTime(tTable[10].levTime, 3);
		panel[56] = getDigitOfTime(tTable[10].levTime, 2);
		panel[57] = getDigitOfTime(tTable[10].levTime, 1);
		panel[58] = tTable[10].isWaitPass;
		// MessageBox(NULL, TEXT("テーブル更新"), TEXT("pushUp"), MB_OK);
		
	}
	// 種別更新
	if (option[2] == 1) {
		if (push) {
			tInfo[0].type = tInfo[1].type;
			tInfo[1].type = tInfo[2].type;
			tInfo[2].type = 0;
		}
		// パネル反映
		panel[70] = tInfo[0].type;
		// MessageBox(NULL, TEXT("種別更新"), TEXT("pushUp"), MB_OK);
	}
	// 行先更新
	if (option[3] == 1) {
		if (push) {
			tInfo[0].distination = tInfo[1].distination;
			tInfo[1].distination = tInfo[2].distination;
			tInfo[2].distination = 0;
		}
		// パネル反映
		panel[71] = tInfo[0].distination;
		// MessageBox(NULL, TEXT("行先更新"), TEXT("pushUp"), MB_OK);
	}
	// 列車番号更新
	if (option[4] == 1) {
		if (push) {
			for (int i = 0; i < 3 - 1; ++i)
				for (int j = 0; j < 6; ++j)
					tInfo[i].trainNumber[j] = tInfo[i + 1].trainNumber[j];
			for (int i = 0; i < 6; ++i)
				tInfo[2].trainNumber[i] = 0;
		}
		// パネル反映
		panel[65] = tInfo[0].trainNumber[1];
		panel[66] = tInfo[0].trainNumber[2];
		panel[67] = tInfo[0].trainNumber[3];
		panel[68] = tInfo[0].trainNumber[4];
		panel[69] = tInfo[0].trainNumber[5];
		// MessageBox(NULL, TEXT("列車番号更新"), TEXT("pushUp"), MB_OK);
	}
	// 両数更新
	if (option[5] == 1) {
		// 廃止
		// if (push) {
		// 	tInfo[0].carNumber = tInfo[1].carNumber;
		// 	tInfo[1].carNumber = tInfo[2].carNumber;
		// 	tInfo[2].carNumber = 0;
		// }
		// パネル反映
		// panel[72] = tInfo[0].carNumber;
		// MessageBox(NULL, TEXT("両数更新"), TEXT("pushUp"), MB_OK);
	}
	// 臨時停車駅更新
	if (option[6] == 1) {
		if (push) {
			tInfo[0].extraStop = tInfo[1].extraStop;
			tInfo[1].extraStop = tInfo[2].extraStop;
			tInfo[2].extraStop = 0;
		}
		// パネル反映
		panel[238] = tInfo[0].extraStop;
		panel[239] = tInfo[1].extraStop;
		// MessageBox(NULL, TEXT("臨時停車駅更新"), TEXT("pushUp"), MB_OK);
	}
	// 更新設定リセット
	uInfo.option = 0;
	uInfo.pushNum = 0;
	uInfo.type = 0;
	uInfo.update = false;
	// for (int i = 0; i < 255; ++i)panel[i] = 1;
}
// --------------------------------------------------------
void TICS::flashStopDisp(ATS_VEHICLESTATE vehicleState, int* panel, int* sound) {
	if (doorOpen || !stopFlash.state) {
		panel[197] = 0;
		stopFlash.state = false;
		stopFlash.distance = 0.0;
		stopFlash.timeCounter = 0;
		stopFlash.soundState = 0;
		sound[200] = ATS_SOUND_STOP;
		return;
	}
	if (sound[200] == ATS_SOUND_PLAY) sound[200] = ATS_SOUND_CONTINUE;
	if (stopFlash.distance < stopFlash.flashDis*1000) {
		if (stopFlash.soundState == 0) {
			stopFlash.soundState = 1;
			sound[200] = ATS_SOUND_PLAY;
		}
		float distancePerFrame = (speed * 1000 / 3600) * deltaT;	// 1フレームで進んだ距離
		stopFlash.distance = stopFlash.distance + distancePerFrame;	// 累積距離(1/1000でm)
		panel[197] = stopFlash.count[0];
		return;
	}
	if (stopFlash.soundState == 1) {
		stopFlash.soundState = 2;
		sound[200] = ATS_SOUND_PLAY;
	}
	if (stopFlash.timeCounter > stopFlash.interval) {
		stopFlash.timeCounter = 0;
		int pOut = panel[197];
		pOut++;
		// MessageBox(NULL, TEXT("停車点滅"), TEXT("切り替え"), MB_OK);
		if (pOut > stopFlash.count[1]) pOut = stopFlash.count[0];
		panel[197] = pOut;
	}
	stopFlash.timeCounter = stopFlash.timeCounter + deltaT;
}
// --------------------------------------------------------
void TICS::flashNextStop(ATS_VEHICLESTATE vehicleState, int* panel) {
	if (doorOpen || !nextStop.state) {
		panel[196] = nextStop.count[1];
		nextStop.state = false;
		nextStop.distance = 0.0;
		nextStop.timeCounter = 0;
		return;
	}
	if (nextStop.distance < nextStop.flashDis *1000) {
		float distancePerFrame = (speed * 1000 / 3600) * deltaT;
		nextStop.distance = nextStop.distance + distancePerFrame;
		panel[196] = nextStop.count[0];
		return;
	}
	if (nextStop.timeCounter > nextStop.interval) {
		// MessageBox(NULL, TEXT("次駅点滅"), TEXT("切り替え"), MB_OK);
		nextStop.timeCounter = 0;
		int pOut = panel[196];
		(pOut == 0) ? pOut = nextStop.count[0] : pOut = 0;
		panel[196] = pOut;
	}
	nextStop.timeCounter = nextStop.timeCounter + deltaT;
}
// --------------------------------------------------------
// 指定桁の数字を返す(hh/mm/ss)
int TICS::getDigitOfTime(int num, int digit) {
	if (num == 0) return 60;
	if (digit == 3) { return num / 10000; }
	else if (digit == 2) { return num / 100 % 100; }
	else if (digit == 1) { return num % 100; }
	else { return 0; }
}
// --------------------------------------------------------