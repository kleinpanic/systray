#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

Display *dpy;
Window root, tray_icon_window;
GC gc;

void create_tray_icon_window(int width, int height) {
    tray_icon_window = XCreateSimpleWindow(dpy, root, 0, 0, width, height, 0, 0, BlackPixel(dpy, DefaultScreen(dpy)));
    XSelectInput(dpy, tray_icon_window, ExposureMask | ButtonPressMask);
    XMapWindow(dpy, tray_icon_window);
}

void draw_tray_icon() {
    XClearWindow(dpy, tray_icon_window);
    // Draw a simple filled rectangle as the icon (black box)
    XFillRectangle(dpy, tray_icon_window, gc, 0, 0, 24, 24);
}

int main() {
    XEvent ev;

    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    root = DefaultRootWindow(dpy);
    gc = DefaultGC(dpy, DefaultScreen(dpy));

    // Create the tray icon window with the specified dimensions
    create_tray_icon_window(24, 24);

    // Main event loop
    while (1) {
        XNextEvent(dpy, &ev);

        if (ev.type == Expose) {
            if (ev.xexpose.window == tray_icon_window) {
                draw_tray_icon();
            }
        } else if (ev.type == ButtonPress) {
            if (ev.xbutton.window == tray_icon_window) {
                printf("Tray icon clicked!\n");
            }
        }
    }

    XCloseDisplay(dpy);
    return 0;
}
