#include <Imlib2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PATH_SIZE 500
int wBackgroundImage(Display *dpy, Window window, const char* FILEPATH) {
    Imlib_Image img;
    Pixmap pix;
	FILE *command;
	char path[PATH_SIZE];
	char args[PATH_SIZE];
	// random file command
	snprintf(args, PATH_SIZE, "ls %s | sort -R | tail -%d | while read file; do echo $file; done", FILEPATH, 10);
	command = popen(args,"r");
	if(command == NULL){
		printf("Failed to run command\n");
		exit(1);
	}
	// get one file
	while (fgets(path, sizeof(path), command) != NULL) {
		break;
	}
	fclose(command);

	int width, height;
	char realpath[PATH_SIZE];
	// weird const conversion
    snprintf(realpath, PATH_SIZE, "%s%s", FILEPATH, path);
	// omitting \n 
	char *c = strtok(realpath,"\n");
	// set bg image
    if(!(img = imlib_load_image(c))) return 1;

	imlib_context_set_image(img);
	width = imlib_image_get_width();
	height = imlib_image_get_height();
	Screen *s = XScreenOfDisplay(dpy, DefaultScreen(dpy));
	pix = XCreatePixmap(dpy, window, width, height, DefaultDepthOfScreen(s));

	imlib_context_set_display(dpy);
	imlib_context_set_visual(DefaultVisualOfScreen(s));
	imlib_context_set_colormap(DefaultColormapOfScreen(s));
	imlib_context_set_drawable(pix);

	imlib_render_image_on_drawable(0, 0);
	XSetWindowBackgroundPixmap(dpy, window, pix);
	XClearWindow(dpy, window);
	return 0;
}
