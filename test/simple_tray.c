#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRAY_ICON_SIZE 16 // Size of the systray icon

// Function to send a message to the system tray
void send_message(Display *display, Window window, Atom message, long data) {
    XClientMessageEvent ev = {0};

    ev.type = ClientMessage;
    ev.window = window;
    ev.message_type = message;
    ev.format = 32;
    ev.data.l[0] = CurrentTime;
    ev.data.l[1] = data;

    XSendEvent(display, window, False, NoEventMask, (XEvent *)&ev);
    XFlush(display);
}

int main() {
    Display *display;
    Window root;
    Window tray_window;
    XEvent event;
    Atom tray_atom, tray_selection, tray_message;
    int screen;

    // Open connection to X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    root = RootWindow(display, screen);

    // Create a simple window to act as the systray icon
    tray_window = XCreateSimpleWindow(display, root, 0, 0, TRAY_ICON_SIZE, TRAY_ICON_SIZE, 0,
                                      BlackPixel(display, screen), WhitePixel(display, screen));

    // Set the window title (optional)
    XStoreName(display, tray_window, "SimpleTray");

    // Set window properties to behave like a systray icon
    tray_atom = XInternAtom(display, "_NET_SYSTEM_TRAY_S0", False);
    tray_selection = XGetSelectionOwner(display, tray_atom);
    XSetSelectionOwner(display, tray_atom, tray_window, CurrentTime);

    if (XGetSelectionOwner(display, tray_atom) != tray_window) {
        fprintf(stderr, "Unable to set selection owner\n");
        XDestroyWindow(display, tray_window);
        XCloseDisplay(display);
        exit(1);
    }

    // Inform the system tray of the new icon
    tray_message = XInternAtom(display, "_NET_SYSTEM_TRAY_OPCODE", False);
    send_message(display, root, tray_message, 0);

    // Map the window (make it visible)
    XMapWindow(display, tray_window);
    XFlush(display);

    // Event loop to handle clicks
    while (1) {
        XNextEvent(display, &event);
        if (event.type == ButtonPress) {
            if (event.xbutton.button == Button1) {
                printf("Systray icon clicked!\n");
            }
        }
    }

    // Clean up
    XDestroyWindow(display, tray_window);
    XCloseDisplay(display);
    return 0;
}
