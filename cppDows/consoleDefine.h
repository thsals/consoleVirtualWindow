#define LOCALE	_T("kor")

#define	SAFE_DELETE(data)			if( data != NULL ){ delete data; data = NULL; }
#define	SAFE_DELETE_ARRAY(data)		if( data != NULL ){ delete [] data; data = NULL; }

	// �÷� ���̺�
	typedef enum eColorList
	{
		BLACK,
		D_BLUE,
		D_GREEN,
		D_SKYBLUE,
		D_RED,
		D_VIOLET,
		D_YELLOW,
		GRAY,
		D_GRAY,
		BLUE,
		GREEN,
		SKYBLUE,
		RED,
		VIOLET,
		YELLOW,
		WHITE,
		MAX_NUM_OF_COLOR,
	}COLOR_LIST;

	typedef enum eScreenList
	{
		SCREEN_01,
		SCREEN_02, // ���� ���۸�
		// SCREEN_03, // ���� ���۸�
		MAX_NUM_OF_SCREEN,
	}SCREEN_LIST;

	CONST SHORT Width = 700;
	CONST SHORT Height = 188;
