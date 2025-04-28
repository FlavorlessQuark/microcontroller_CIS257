#include "includes/SDLX/SDLX.h"
#include <sys/inotify.h>
    #include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (10 * (EVENT_SIZE + 16))

int pos_x = DEFAULT_WIN_W /2;
int pos_y = DEFAULT_WIN_H /2;

int file_watch(int fd, int wd) {

    int length, i = 0;
    char buffer[BUF_LEN];


    length = read(fd, buffer, BUF_LEN);

    if (length < 0) {
        // perror("read");
        return 0;
    }

    printf("Reading events\n");
    while (i < length) {
        struct inotify_event *event =
            (struct inotify_event *) &buffer[i];
        if (event->len) {
            if (event->mask & IN_CREATE) {
                printf("The file %s was created.\n", event->name);
            } else if (event->mask & IN_DELETE) {
                printf("The file %s was deleted.\n", event->name);
            } else if (event->mask & IN_MODIFY) {
                printf("The file %s was modified.\n", event->name);
            }
        }
        i += EVENT_SIZE + event->len;
    }

    printf("returning at end\n");

    return 0;
}

int main()
{
    int fd;
    int wd;
	SDLX_RectContainer *root;
	SDLX_Display *display;

	SDLX_InitDefault();

    fd = inotify_init();

    if (fd < 0) {
        perror("inotify_init");
    }

    wd = inotify_add_watch(fd, ".",
        IN_MODIFY | IN_CREATE | IN_DELETE);
	// cleanupUIConfig(root);
	display = SDLX_DisplayGet();
    SDL_Rect rect = {.x = DEFAULT_WIN_W /2, .y = DEFAULT_WIN_H / 2, .w = 4, .h = 4};

	while (1)
	{
        SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 0xFF);
		SDLX_RenderReset(display);
		SDLX_InputLoop();
        file_watch(fd, wd);
        SDL_SetRenderDrawColor(display->renderer, 0xFF, 0, 0, 0xFF);
        SDL_RenderDrawRect(display->renderer, &rect);
		SDL_RenderPresent(display->renderer);
	}
	SDLX_RenderQueuesCleanup();

    inotify_rm_watch(fd, wd);
    close(fd);
}
