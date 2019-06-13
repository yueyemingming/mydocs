# 串口代码函数版

* [代码](com.cpp)

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <iostream>

using namespace std ;

bool write_data( char* dev, char* buffer, const int& size )
{
    int fd = open( dev, O_RDWR | O_NOCTTY | O_NONBLOCK);         //| O_NOCTTY | O_NDELAY
    if ( fd < 0 ) {
        perror("open");
        return false ;
    }

    struct termios option = {};
    bzero (&option, sizeof (option));
    cfmakeraw (&option);
    option.c_cflag |= B115200 | CLOCAL | CREAD;      /* | CRTSCTS */
    option.c_cflag |= CS8;
    option.c_cflag &= ~PARENB;
    option.c_cflag &= ~CSTOPB ;
    option.c_oflag = 0;
    option.c_lflag |= 0;
    option.c_oflag &= ~OPOST;
    option.c_cc[VTIME] = 1;        /* unit: 1/10 second. */
    option.c_cc[VMIN] = 1; /* minimal characters for reading */
    tcflush (fd, TCIFLUSH);
    if ( tcsetattr(fd,TCSANOW,&option) < 0 ) {
        perror("tcsetattr");
        return false ;
    }

    if ( write( fd, buffer, size ) < 0 ) {
        perror("write");
        return false ;
    }

    close( fd ) ;

    return true ;
}

bool read_data( char* dev, char* buffer, int& size )
{
    int fd = open( dev, O_RDWR | O_NOCTTY | O_NONBLOCK );         //| O_NOCTTY | O_NDELAY
    if ( fd < 0 ) {
        perror("open");
        return false ;
    }

    struct termios option = {};
    bzero (&option, sizeof (option));
    cfmakeraw (&option);
    option.c_cflag |= B115200 | CLOCAL | CREAD;
    option.c_cflag |= CS8;
    option.c_cflag &= ~PARENB;
    option.c_cflag &= ~CSTOPB ;
    option.c_oflag = 0;
    option.c_lflag |= 0;
    option.c_oflag &= ~OPOST;
    option.c_cc[VTIME] = 1;
    option.c_cc[VMIN] = 1;
    tcflush (fd, TCIFLUSH);
    if ( tcsetattr(fd,TCSANOW,&option) < 0 ) {
        perror("tcsetattr");
        return false ;
    }

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET( fd, &rfds);

    while (1) //循环读取从串口受到的数据
    {
        // select 是最重要的部分。如果没有select。则read函数永远读不到任何东西，这就是为什么linux串口程序发送能成功，但始终接收不到任何数据的原因
        int retval = select( fd+1, &rfds, NULL, NULL, NULL );

        if (retval == -1) {
            perror("select");
            continue ;
        }

        char buf[80] = { 0 } ;
        int size = -1 ;

        if ( ( size=read( fd, buf, 80 ) ) < 0 ) {
            perror("read");
            continue ;
        }

        cout << "length : " << size << endl ;
        for ( int i = 0 ; i < size ; i++ )
            printf( "0x%02x, ", buf[i] ) ;
        cout << endl ;
    }

    close( fd ) ;

    return true ;
}


int main(int argc, char **argv)
{
    /**
     * 写入测试
     */
    /*
    char write_buffer[100] = {} ;
    strcpy( write_buffer, "AT+CGMI\r\n" ) ;
    int write_size = strlen( write_buffer ) ;
    if ( !write_data( argv[1], write_buffer, write_size ) ) {
        cout << "write_data error ." << endl ;
        return -1 ;
    }
    */

    /**
     * 读取测试
     */
    while(1)
    {
        char read_buffer[100] = {} ;
        int read_size = 0 ;
        if ( !read_data( argv[1], read_buffer, read_size ) ) {
            cout << "read_data error ." << endl ;
            return -1 ;
        }

        cout << read_buffer << endl ;
    }
    return 0 ;
}
```
