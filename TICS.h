#define SET_TICS_UPDATE        325  // �X�V�p
#define SET_TICS_TRAIN_INFO    326  // �^�]��ԏ��P
#define SET_TICS_TRAIN_NUMBER  327  // ��Ԕԍ�
#define SET_TICS_SECTION       328  // ���s���
#define SET_TICS_STA           329  // �w��
#define SET_TICS_ARR_TIME      330  // ��������
#define SET_TICS_LEV_TIME      331  // �o������
#define SET_TICS_STA_WORK      332  // �ޔ�/�����ݒ�
#define SET_TICS_EXTRA_STOP    333  // �Վ���ԉw

#define SET_TICS_NEXT_STA      335  // ���w�ݒ�
#define SET_TICS_DISP_STOP     336  // �u��ԁv�\���ݒ�
#define SET_TICS_ARBITRARY     337  // ���j�^�E��̘g

#define ARRAY_MAX_NUM 99			// �f�[�^�i�[�p�v�f��

struct ticsTable {
	int staName;		// �w��
	int arrTime;		// ��������
	int levTime;		// ���Ԏ���
	bool isWaitPass;	// �Ҕ�
	bool isDissolution;	// ����
};
struct trainInfo {
	int type;			// ���
	int distination;	// �s��
	int carNumber;		// ����
	int extraStop;		// �Վ���ԉw
	int trainNumber[6];	// ��Ԕԍ�
	int runnigSection;	// ���s���
};
struct updateInfo {
	bool update;
	int type;
	int pushNum;
	int option;
};
struct flashDisp {
	int interval;		// �؂�ւ�����[ms]
	int count[2];		// �摜�C���f�b�N�X�w��[�J�n,�I��]�@�J�n0:0�ƏI���l�̓_��
	int time;			// �O�t���[������
	int timeCounter;	// �ݐώ���
	int flashDis;		// �_�ŊJ�n�܂ł̋���
	float distance;		// �ݐϋ���
	bool state;			// ���[����/�L��]
	int soundState;		// �u��Ԃł��v�Đ�
};
struct OperationSetting {
	int conductorSet[6];// ���ݎ������̕\���g
	int day;			// ����/�x��
	bool notification;	// �ʒm
	bool stopNotice;	// ��ʖh�~
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
		// void setWaitPass(int);	// setStaWork�ɓ���
		// void setDissolution(int);// setStaWork�ɓ���
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
			750,	// �؂�ւ�����[ms]
		  {1,2},	// �摜�C���f�b�N�X		
			  0,	// �O�t���[������
			  0,	// �ݐώ���
			  0,	// �_�ŊJ�n�܂ł̋�
			0.0,	// �ݐϋ���
		   false,	// ���[����/�L��]
			  0		// 
		};
		flashDisp nextStop = {
			750,	// �؂�ւ�����[ms]
		  {0,0},	// �摜�C���f�b�N�X		
			  0,	// �O�t���[������
			  0,	// �ݐώ���
			  0,	// �_�ŊJ�n�܂ł̋�
			0.0,	// �ݐϋ���
		   false,	// ���[����/�L��]
			  0		// ���g�p
		};
		ticsTable* tTp[22];
		OperationSetting oSet = { {0,0,0,0,0,0},0,false,false };
};
