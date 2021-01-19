#define SET_TICS_UPDATE        325  // 更新用
#define SET_TICS_TRAIN_INFO    326  // 運転列車情報１
#define SET_TICS_TRAIN_NUMBER  327  // 列車番号
#define SET_TICS_SECTION       328  // 走行区間
#define SET_TICS_STA           329  // 駅名
#define SET_TICS_ARR_TIME      330  // 到着時刻
#define SET_TICS_LEV_TIME      331  // 出発時刻
#define SET_TICS_STA_WORK      332  // 退避/解結設定
#define SET_TICS_EXTRA_STOP    333  // 臨時停車駅

#define SET_TICS_NEXT_STA      335  // 次駅設定
#define SET_TICS_DISP_STOP     336  // 「停車」表示設定
#define SET_TICS_ARBITRARY     337  // モニタ右上の枠

#define ARRAY_MAX_NUM 99			// データ格納用要素数

struct ticsTable {
	int staName;		// 駅名
	int arrTime;		// 到着時刻
	int levTime;		// 発車時刻
	bool isWaitPass;	// 待避
	bool isDissolution;	// 解結
};
struct trainInfo {
	int type;			// 種別
	int distination;	// 行先
	int carNumber;		// 両数
	int extraStop;		// 臨時停車駅
	int trainNumber[6];	// 列車番号
	int runnigSection;	// 走行区間
};
struct updateInfo {
	bool update;
	int type;
	int pushNum;
	int option;
};
struct flashDisp {
	int interval;		// 切り替え時間[ms]
	int count[2];		// 画像インデックス指定[開始,終了]　開始0:0と終了値の点滅
	int time;			// 前フレーム時間
	int timeCounter;	// 累積時間
	int flashDis;		// 点滅開始までの距離
	float distance;		// 累積距離
	bool state;			// 状態[無効/有効]
	int soundState;		// 「停車です」再生
};
struct OperationSetting {
	int conductorSet[6];// 現在時刻下の表示枠
	int day;			// 平日/休日
	bool notification;	// 通知
	bool stopNotice;	// 停通防止
};
class TICS {
	public:
		void init();
		void run(ATS_VEHICLESTATE, int*, int*);
		void setDoorState(bool);
		void setUpdate(int);
		void setTrainInfo(int);
		void setTrainNumber(int);
		void setRunningSection(int);
		void setStaName(int);
		void setArrTime(int);
		void setLevTime(int);
		void setStaWork(int);
		// void setWaitPass(int);	// setStaWorkに統合
		// void setDissolution(int);// setStaWorkに統合
		void setExtraStop(int);
		void setStopFlash(int);
		void setNextStop(int);
		void setReverser(int);
		void setFlashStopSetting(int,int);
		void setOperateSetting(int);
		bool isUpdate();
		void pushUp(int* panel);
		void flashStopDisp(ATS_VEHICLESTATE, int*, int*);
		void flashNextStop(ATS_VEHICLESTATE, int*);
		int getDigitOfTime(int, int);
	private:
		float speed;
		int time;
		int deltaT;
		bool update;
		bool doorOpen;
		int reverser = 0;
		float runDis = 0.0;
		ticsTable tTable[99] = {
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false},
			{0,0,0,false,false}
		};
		trainInfo tInfo[3] = {
			{0,0,0,0,{0,0,0,0,0,0},0},
			{0,0,0,0,{0,0,0,0,0,0},0},
			{0,0,0,0,{0,0,0,0,0,0},0}
		};
		updateInfo uInfo = { false,0,0,0 };
		flashDisp stopFlash = {
			750,	// 切り替え時間[ms]
		  {1,2},	// 画像インデックス		
			  0,	// 前フレーム時間
			  0,	// 累積時間
			  0,	// 点滅開始までの距
			0.0,	// 累積距離
		   false,	// 状態[無効/有効]
			  0		// 
		};
		flashDisp nextStop = {
			750,	// 切り替え時間[ms]
		  {0,0},	// 画像インデックス		
			  0,	// 前フレーム時間
			  0,	// 累積時間
			  0,	// 点滅開始までの距
			0.0,	// 累積距離
		   false,	// 状態[無効/有効]
			  0		// 未使用
		};
		ticsTable* tTp[22];
		OperationSetting oSet = { {0,0,0,0,0,0},0,false,false };
};
