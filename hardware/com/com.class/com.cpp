#include "./Com.h"

#include <stdio.h>              /* perror, printf, puts, fprintf, fputs */
#ifdef LINUX
#include <stdlib.h>
#include <termios.h>            /* tcgetattr, tcsetattr */
#include <unistd.h>             /* read, write, close */
#include <fcntl.h>              /* open */
#include <sys/signal.h>
#include <sys/types.h>
#include <string.h>             /* bzero, memcpy */
#include <limits.h>             /* CHAR_MAX */
#endif

CCom::CCom(void):nHandle(0)
{
}

CCom::~CCom(void)
{
	Close();
}

CCom::CCom(int nCom, int nBaudRate, int nDataBit, char* nStopBit,
		   char nParity, int nFlowCtrl):nHandle(0)
{
	Open(nCom, nBaudRate, nDataBit, nStopBit, nParity, nFlowCtrl);
}

int CCom::Open(int nCom, int nBaudRate, int nDataBit, char* nStopBit, 
			   char nParity, int nFlowCtrl)
{
#ifdef WIN32
	memset(&m_dcb, 0x00 , sizeof(DCB) );

	m_dcb.DCBlength = sizeof(DCB);
	m_dcb.BaudRate = nBaudRate; 
	m_dcb.ByteSize = nDataBit;

	m_dcb.fBinary = 1;
	m_dcb.fParity = 0;
	m_dcb.fOutxCtsFlow = 0; //
	m_dcb.fOutxDsrFlow = 0; //
	m_dcb.fDtrControl = 0;
	m_dcb.fDsrSensitivity = 0;
	m_dcb.fTXContinueOnXoff = 0;
	m_dcb.fOutX = 0;
	m_dcb.fInX = 0;
	m_dcb.fErrorChar = 0;
	m_dcb.fNull = 0;
	m_dcb.fRtsControl = 0;
	m_dcb.fAbortOnError = 0;
	m_dcb.fDummy2 = 0;
	m_dcb.wReserved = 0;
	m_dcb.XonLim = 0;
	m_dcb.XoffLim = 0;

	switch(nParity)
	{
	case 'N':
		m_dcb.Parity = 0;
		break;
	case 'O':
		m_dcb.Parity = 1;
		break;
	case 'E':
		m_dcb.Parity = 2;
		break;
	default:
		m_dcb.Parity = 0;
		break;
	}

	if(strcmp(nStopBit, "1") == 0)
		m_dcb.StopBits = 0;
	if(strcmp(nStopBit, "1.5") == 0)
		m_dcb.StopBits = 1;
	if(strcmp(nStopBit, "2") == 0)
		m_dcb.StopBits = 2;

	m_dcb.XonChar = 0;
	m_dcb.XoffChar = 0;
	m_dcb.ErrorChar = 0;
	m_dcb.EofChar = 0;
	m_dcb.EvtChar = 0;
	m_dcb.wReserved1 = 0;

	WCHAR comPort[10];
	if(nCom < 10)
	{
		//swprintf(comPort, ("COM%d"), nCom);
		comPort[0] = 'C';
		comPort[1] = 'O';
		comPort[2] = 'M';
		comPort[3] = '1' + nCom;
		comPort[4] = '\0';
		nHandle = CreateFile(comPort, 
			GENERIC_READ|GENERIC_WRITE,
			0,
			NULL, 
			OPEN_EXISTING,
			0,
			NULL);
		if(!nHandle)
			return false;
		SetCommState(nHandle, &m_dcb);
		SetupComm(nHandle , 1600, 1600 ); //a single Ethernet frame.

		COMMTIMEOUTS CommtimeOuts ;
		CommtimeOuts.ReadIntervalTimeout = 100;
		CommtimeOuts.ReadTotalTimeoutConstant =100;
		CommtimeOuts.ReadTotalTimeoutMultiplier=100;
		CommtimeOuts.WriteTotalTimeoutConstant=100;
		CommtimeOuts.WriteTotalTimeoutMultiplier=100;

		SetCommTimeouts(nHandle, &CommtimeOuts);

		PurgeComm(nHandle , PURGE_TXCLEAR );
		PurgeComm(nHandle , PURGE_RXCLEAR );
		return true;
	}
	return false;
#else
#if 1
	if(nHandle == 0)
	{
		char comPort[20];
		if(nCom < 10)
		{
			sprintf(comPort, "/dev/ttyS%d", nCom);
			nHandle = open(comPort, O_RDWR | O_NOCTTY | O_NONBLOCK);
			if(nHandle == -1)
				return false;
			tcgetattr (nHandle, &termios_old);       /* save old termios value */
			if(SetComAttr(nBaudRate, nDataBit, nStopBit, nParity, nFlowCtrl) == -1)
				return false;
			return true;
		}
	}
	return true;
#endif
#endif
}

int CCom::Open(char* strCom, int nBaudRate, int nDataBit, char* nStopBit, 
			   char nParity, int nFlowCtrl)
{
#ifdef WIN32
	memset(&m_dcb, 0x00 , sizeof(DCB) );

	m_dcb.DCBlength = sizeof(DCB);
	m_dcb.BaudRate = nBaudRate; 
	m_dcb.ByteSize = nDataBit;

	m_dcb.fBinary = 1;
	m_dcb.fParity = 0;
	m_dcb.fOutxCtsFlow = 0; //
	m_dcb.fOutxDsrFlow = 0; //
	m_dcb.fDtrControl = 0;
	m_dcb.fDsrSensitivity = 0;
	m_dcb.fTXContinueOnXoff = 0;
	m_dcb.fOutX = 0;
	m_dcb.fInX = 0;
	m_dcb.fErrorChar = 0;
	m_dcb.fNull = 0;
	m_dcb.fRtsControl = 0;
	m_dcb.fAbortOnError = 0;
	m_dcb.fDummy2 = 0;
	m_dcb.wReserved = 0;
	m_dcb.XonLim = 0;
	m_dcb.XoffLim = 0;

	switch(nParity)
	{
	case 'N':
		m_dcb.Parity = 0;
		break;
	case 'O':
		m_dcb.Parity = 1;
		break;
	case 'E':
		m_dcb.Parity = 2;
		break;
	default:
		m_dcb.Parity = 0;
		break;
	}

	if(strcmp(nStopBit, "1") == 0)
		m_dcb.StopBits = 0;
	if(strcmp(nStopBit, "1.5") == 0)
		m_dcb.StopBits = 1;
	if(strcmp(nStopBit, "2") == 0)
		m_dcb.StopBits = 2;

	m_dcb.XonChar = 0;
	m_dcb.XoffChar = 0;
	m_dcb.ErrorChar = 0;
	m_dcb.EofChar = 0;
	m_dcb.EvtChar = 0;
	m_dcb.wReserved1 = 0;

	if(strCom != NULL)
	{
		//swprintf(comPort, ("COM%d"), nCom);
		nHandle = CreateFileA((LPCSTR)(strCom), 
			GENERIC_READ|GENERIC_WRITE,
			0,
			NULL, 
			OPEN_EXISTING,
			0,
			NULL);
		if(!nHandle)
			return false;
		SetCommState(nHandle, &m_dcb);
		SetupComm(nHandle , 1600, 1600 ); //a single Ethernet frame.

		COMMTIMEOUTS CommtimeOuts ;
		CommtimeOuts.ReadIntervalTimeout = 10;
		CommtimeOuts.ReadTotalTimeoutConstant =10;
		CommtimeOuts.ReadTotalTimeoutMultiplier=10;
		CommtimeOuts.WriteTotalTimeoutConstant=10;
		CommtimeOuts.WriteTotalTimeoutMultiplier=10;

		SetCommTimeouts(nHandle, &CommtimeOuts);

		PurgeComm(nHandle , PURGE_TXCLEAR );
		PurgeComm(nHandle , PURGE_RXCLEAR );
		return true;
	}
	return false;
#endif
}

int CCom::Close()
{
#ifdef WIN32
	if( nHandle ) { CloseHandle(nHandle); nHandle=NULL ; } ;
#else
	if(nHandle > 0)
	{
		tcsetattr (nHandle, TCSADRAIN, &termios_old);
		close (nHandle);
		nHandle = 0;
	}
#endif
	return true;
}

int CCom::GetBaudRate()
{
#ifdef LINUX
	return (_BAUDRATE(cfgetospeed(&termios_new)));
#else
	return 9600;
#endif
}

int CCom::SetBaudRate(int nBaudRate)
{
#ifdef LINUX
	termios_new.c_cflag = BAUDRATE(nBaudRate);  /* set baudrate */
	return true;
#else
	return true;
#endif
}

int CCom::SetDataBit(int nDataBit)
{
#ifdef LINUX
	termios_new.c_cflag &= ~CSIZE;
	switch (nDataBit) {
	case 8:
		termios_new.c_cflag |= CS8;
		break;
	case 7:
		termios_new.c_cflag |= CS7;
		break;
	case 6:
		termios_new.c_cflag |= CS6;
		break;
	case 5:
		termios_new.c_cflag |= CS5;
		break;
	default:
		termios_new.c_cflag |= CS8;
		break;
	}
	return true;
#else
	return true;
#endif
}

int CCom::SetStopBit(const char* nStopBit)
{
#ifdef LINUX
	if (0 == strcmp (nStopBit, "1")) {
		termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */
	}
	else if (0 == strcmp (nStopBit, "1.5")) {
		termios_new.c_cflag &= ~CSTOPB; /* 1.5 stop bits */
	}
	else if (0 == strcmp (nStopBit, "2")) {
		termios_new.c_cflag |= CSTOPB;  /* 2 stop bits */
	}
	else {
		termios_new.c_cflag &= ~CSTOPB; /* 1 stop bit */
	}
	return true;
#else
	return true;
#endif
}

int CCom::SetParity(char nParity)
{
#ifdef LINUX
	switch (nParity) {
	case 'N':                  /* no parity check */
		termios_new.c_cflag &= ~PARENB;
		break;
	case 'E':                  /* even */
		termios_new.c_cflag |= PARENB;
		termios_new.c_cflag &= ~PARODD;
		break;
	case 'O':                  /* odd */
		termios_new.c_cflag |= PARENB;
		termios_new.c_cflag |= ~PARODD;
		break;
	default:                   /* no parity check */
		termios_new.c_cflag &= ~PARENB;
		break;
	}
	return true;
#else
	return true;
#endif
}

int CCom::SetComAttr(int nBaudRate, int nDataBit, char* nStopBit, char nParity, int nFlowCtrl)
{
#ifdef LINUX
	bzero (&termios_new, sizeof (termios_new));
	cfmakeraw (&termios_new);
	SetBaudRate (nBaudRate);
	termios_new.c_cflag |= CLOCAL | CREAD;      /* | CRTSCTS */
	SetDataBit (nDataBit);
	SetParity (nParity);
	SetStopBit (nStopBit);
	termios_new.c_oflag = 0;
	termios_new.c_lflag |= 0;
	termios_new.c_oflag &= ~OPOST;
	termios_new.c_cc[VTIME] = 1;        /* unit: 1/10 second. */
	termios_new.c_cc[VMIN] = 1; /* minimal characters for reading */
	tcflush (nHandle, TCIFLUSH);
	return (tcsetattr (nHandle, TCSANOW, &termios_new));
#else
	return 0;
#endif
}

int CCom::BAUDRATE(int nBaudRate)
{
#ifdef LINUX
		switch (nBaudRate) {
		case 0:
			return (B0);
		case 50:
			return (B50);
		case 75:
			return (B75);
		case 110:
			return (B110);
		case 134:
			return (B134);
		case 150:
			return (B150);
		case 200:
			return (B200);
		case 300:
			return (B300);
		case 600:
			return (B600);
		case 1200:
			return (B1200);
		case 2400:
			return (B2400);
		case 9600:
			return (B9600);
		case 19200:
			return (B19200);
		case 38400:
			return (B38400);
		case 57600:
			return (B57600);
		case 115200:
			return (B115200);
		case 230400:
			return (B230400);
		case 460800:
			return (B460800);
		default:
			return (B9600);
		}
#endif
	return 0;
}

int CCom::_BAUDRATE (int nBaudRate)
{
	/* reverse baudrate */
#ifdef LINUX
		switch (nBaudRate) {
		case B0:
			return (0);
		case B50:
			return (50);
		case B75:
			return (75);
		case B110:
			return (110);
		case B134:
			return (134);
		case B150:
			return (150);
		case B200:
			return (200);
		case B300:
			return (300);
		case B600:
			return (600);
		case B1200:
			return (1200);
		case B2400:
			return (2400);
		case B9600:
			return (9600);
		case B19200:
			return (19200);
		case B38400:
			return (38400);
		case B57600:
			return (57600);
		case B115200:
			return (115200);
		case B230400:
			return (230400);
		case B460800:
			return (460800);
		default:
			return (9600);
		}
#endif
	return 0;
}

#define TIMEOUT_SEC(buflen,baud) (0)
//(buflen*5/baud+2)
#define TIMEOUT_USEC (100)
//0

int CCom::Read(char* buf, int size)
{
#ifdef WIN32
	DWORD dwBytesRead =0; 

	if( !ReadFile( nHandle,
		buf,
		size,
		&dwBytesRead,
		NULL ) ) return 0;

	return dwBytesRead;
#else
	int retval = 0;
	if(nHandle > 0)
	{
		FD_ZERO (&fs_read);
		FD_SET (nHandle, &fs_read);
		tv_timeout.tv_sec = TIMEOUT_SEC (size, nBaudRate);
		tv_timeout.tv_usec = TIMEOUT_USEC;
		retval = select (nHandle + 1, &fs_read, NULL, NULL, &tv_timeout);
		if (retval) 
		{
			size = read (nHandle, buf, size);
			return size;
		}
	}
	return 0;
#endif
}

int CCom::Write(const char* buf, int& size)
{
#ifdef WIN32
	DWORD dwBytesWritten =0;

	WriteFile(nHandle,
		buf,
		size,
		&dwBytesWritten,
		NULL);

	return dwBytesWritten ;
#else
	int retval, len = 0, total_len = 0;

	if(nHandle > 0)
	{
		FD_ZERO (&fs_write);
		FD_SET (nHandle, &fs_write);
		tv_timeout.tv_sec = TIMEOUT_SEC (size, nBaudRate);
		tv_timeout.tv_usec = TIMEOUT_USEC;

		for (total_len = 0, len = 0; total_len < size;) {
			retval = select (nHandle + 1, NULL, &fs_write, NULL, &tv_timeout);
			if (retval) {
				len = write (nHandle, &buf[total_len], size - total_len);
				if (len > 0) {
					total_len += len;
				}
			}
			else {
				tcflush (nHandle, TCOFLUSH);     /* flush all output data */
				break;
			}
		}
	}
	return (total_len);
#endif
	return 0;
}
