      #include <sys/types.h>
      #include <sys/stat.h>
      #include <fcntl.h>
      #include <termios.h>
      #include <stdio.h>
      #include <errno.h>

      #define BAUDRATE B9600
      #define MODEMDEVICE "/dev/ttyUSB0"
      #define _POSIX_SOURCE 1 /* POSIX compliant source */
      #define FALSE 0
      #define TRUE 1
      # define CRTSCTS 020000000000

    int STOP=FALSE;

      main()
      {
        int fd,c, res;
        struct termios oldtio,newtio;
        char buf[255] = {0};

        fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
        if (fd <0) {perror(MODEMDEVICE); exit(-1); }

        // tcgetattr(fd,&oldtio); /* save current port settings */

        // bzero(&newtio, sizeof(newtio));
        // newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
        // newtio.c_iflag = IGNPAR;
        // newtio.c_oflag = 0;

        // /* set input mode (non-canonical, no echo,...) */
        // newtio.c_lflag = 0;
        // newtio.c_cflag &= ~CRTSCTS;

        // newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
        // newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */

        // tcflush(fd, TCIFLUSH);
        // tcsetattr(fd,TCSANOW,&newtio);


        // // while (STOP==FALSE) {       /* loop for input */
        // //   res = read(fd,buf,3);   /* returns after 5 chars have been input */
        // //   if (res < 0) {
        // //     perror("no read");
        // //     // printf("erro %d : %s", errno, strerror(errno));
        // //   }
        // //   buf[res]="\0";
        // //   printf("RES %d\n", res);
        // //   if (buf[0]=='z') STOP=TRUE;
        // // }
        // tcsetattr(fd,TCSANOW,&oldtio);
      }
