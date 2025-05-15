#include "includes/SDLX/SDLX.h"
#include "includes/decoder.h"



void init (int *fd, struct termios *oldtio, struct termios *newtio) {
	#ifndef _DEBUG_OUT
        SDLX_InitDefault();
    #endif


    *fd = open(SERIALSOURCE, O_RDWR);
    if (*fd < 0) {
        printf("Error %d opening port: %s\n", errno, strerror(errno));
    }
    else
        printf("Success!\n");

    tcgetattr(*fd, oldtio); /* save current port settings */

    bzero(newtio, sizeof(newtio));
    newtio->c_cflag = BAUDRATE | CRTSCTS| CS8 | CLOCAL | CREAD;
    newtio->c_iflag = IGNPAR;
    newtio->c_oflag = 0;
    newtio->c_lflag = 0;/* set input mode (non-canonical, no echo,...) */
    newtio->c_cc[VTIME]    = 1;   /*timer 0.1s*/
    newtio->c_cc[VMIN]     = CHAR_COUNT;   /* blocking read until 3 chars received */

    tcflush(*fd, TCIFLUSH); /* Flush data */
    tcsetattr(*fd,TCSANOW, newtio);/* Set new port settings */
}

void parse_input(char buffer[3], SDL_Rect * rect, int *speed) {
    static int bl_state = 0;
    static int br_state = 0;
    Packet packet = {
        .data.raw = buffer[0],
        .horizontal = buffer[1],
        .vertical = buffer[2]
    };
    int x = packet.horizontal;
    int y = packet.vertical;

    if (packet.data.bits.x_sign)
        x *= -1;
    if (packet.data.bits.y_sign)
        y *= -1;

    #ifdef _DEBUG_OUT
        printf(" x move %d, y move %d speed %d\n ", x, y, *speed);
        printf("button state L prv %d curr %d \n", bl_state, packet.data.bits.button_left);
        printf("button state R prv %d curr %d \n", br_state, packet.data.bits.button_right);
    #endif

    if (br_state == 0 && packet.data.bits.button_right == 1) {
        *speed = ((*speed + 5) % 21) + 1;
    }
    br_state = packet.data.bits.button_right;
    rect->x += *speed * x;
    rect->y += *speed * y;
    if (rect->x > DEFAULT_WIN_W)
        rect->x -= DEFAULT_WIN_W;
    if (rect->y > DEFAULT_WIN_H)
        rect->y -= DEFAULT_WIN_H;
    if (rect->x < 0)
        rect->x += DEFAULT_WIN_W;
    if (rect->y < 0)
        rect->y += DEFAULT_WIN_H;
}

int main()
{
	SDLX_Display *display;
    SDL_Rect rect = {.x = DEFAULT_WIN_W, .y = DEFAULT_WIN_H, .w = 10, .h = 10};

    int fd;
    int speed = 5;
    int res;
    char buff[CHAR_COUNT + 1];
    struct termios oldtio, newtio;

    init(&fd, &oldtio, &newtio);

    #ifndef _DEBUG_OUT
        display = SDLX_DisplayGet();
    #endif
    while (1) {       /* loop for input */
        #ifndef _DEBUG_OUT
            SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 0xFF);
            SDLX_RenderReset(display);
            SDLX_InputLoop();
            SDL_SetRenderDrawColor(display->renderer, 0xFF, 0, 0, 0xFF);
            SDL_RenderFillRect(display->renderer, &rect);
            SDL_RenderPresent(display->renderer);
        #endif
        if ((res = read(fd,buff,sizeof(char) * CHAR_COUNT) < 0))
            printf("No data (this shouldn't happen)\n");
        else {
            parse_input(buff, &rect, &speed);
        }
        #ifdef _DEBUG_OUT
            print_bits(buff[0]);
            print_bits(buff[1]);
            print_bits(buff[2]);
            print_data(buff);
            printf(" DONE \n");
        #endif
    }
    tcsetattr(fd,TCSANOW,&oldtio);



	// while (1)
	// {
    //     SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 0xFF);
	// 	SDLX_RenderReset(display);
	// 	SDLX_InputLoop();
    //     SDL_SetRenderDrawColor(display->renderer, 0xFF, 0, 0, 0xFF);
    //     SDL_RenderDrawRect(display->renderer, &rect);
	// 	SDL_RenderPresent(display->renderer);
	// }

}
