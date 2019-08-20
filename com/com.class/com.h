#ifndef _COM_H_
#define _COM_H_

#ifdef WIN32
#include <Windows.h>
#else
#include <termios.h>            /* tcgetattr, tcsetattr */
#include <fcntl.h>
#endif

class CCom
{
public:
	CCom(void);
	CCom(int nCom, int nBaudRate=115200, int nDataBit=8, char* nStopBit="1", 
		char nParity='N', int nFlowCtrl=0);

	int Open(int nCom, int nBaudRate=115200, int nDataBit=8, char* nStopBit="1", 
		char nParity='N', int nFlowCtrl=0);
	int Open(char* strCom, int nBaudRate=115200, int nDataBit=8, char* nStopBit="1", 
		char nParity='N', int nFlowCtrl=0);
	int Close();

	int Write(const char* buf, int& size);
	int Read(char* buf, int size);

	~CCom(void);

private:
	int SetComAttr(int nBaudRate, int nDataBit, char* nStopBit, char nParity,
		int nFlowCtrl);
	int SetBaudRate(int nBaudRate);
	int SetDataBit(int nDataBit);
	int SetParity(char nParity);
	int SetStopBit(const char* nStopBit);

	int GetBaudRate();
	int BAUDRATE(int nBaudRate);
	int _BAUDRATE(int nBaudRate);

#ifdef WIN32
	HANDLE nHandle;
	DCB m_dcb; 
#else
	int nHandle;
	struct termios termios_old, termios_new;
	fd_set   fs_read, fs_write;
	struct timeval tv_timeout;
	int nBaudRate;
#endif
};
#endif

