#include <Imlib2.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PATH_SIZE 500

int wBackgroundImage(Display *dpy, Window window, const char* FILEPATH) {
    Imlib_Image img;
    Pixmap pix;

    if (FILEPATH == NULL || strlen(FILEPATH) == 0) {
        fprintf(stderr, "Invalid file path\n");
        return 1;
    }

    DIR *dir = opendir(FILEPATH);
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory");
        return 1;
    }

    int file_count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG)
            file_count++;
    }

    // Select a random file
	srand(time(NULL));
    int r = rand() % file_count;
    rewinddir(dir);
    file_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            if (file_count == r)
                break;
            file_count++;
        }
    }
    closedir(dir);

	// construct path
    char path[PATH_SIZE];
    snprintf(path, PATH_SIZE, "%s/%s", FILEPATH, entry->d_name);

    if (!(img = imlib_load_image(path))) {
        fprintf(stderr, "Error loading image: %s\n", path);
        return 1;
    }

    int width, height;
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

