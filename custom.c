#include <Imlib2.h>
int wBackgroundImage(Display *dpy, Window window, const char* FILEPATH) {
    Imlib_Image img;
    Pixmap pix;
    int width, height;

    if(!(img = imlib_load_image(FILEPATH))) return 1;

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
