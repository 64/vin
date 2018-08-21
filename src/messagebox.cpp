// TODO: Fiz 80 issues and possibly convert to C++
// TODO: OSX?
#ifdef linux

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#elif defined(_WIN32) || defined(WIN32)

#include <windows.h>

#endif

#include <string>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32)
void MessageBoxWindows(const std::string& title, const std::string& message)
{
    MessageBox(NULL, message.c_str(), title.c_str(), MB_OK);
}
#endif

#ifdef linux
typedef struct
{
    int x, y;
    unsigned int width, height;
    int textx, texty;
    int mouseover;
    int clicked;
    const char* text;
}
button;

static void draw_button( button* b, unsigned long fg, unsigned long bg,
                         Display* dpy, Window w, GC gc )
{
    if( b->mouseover )
    {
        XFillRectangle( dpy, w, gc, b->clicked+b->x, b->clicked+b->y,
                                    b->width, b->height );
        XSetForeground( dpy, gc, bg );
        XSetBackground( dpy, gc, fg );
    }
    else
    {
        XSetForeground( dpy, gc, fg );
        XSetBackground( dpy, gc, bg );
        XDrawRectangle( dpy, w, gc, b->x, b->y, b->width, b->height );
    }

    XDrawString( dpy, w, gc, b->clicked+b->textx, b->clicked+b->texty,
                 b->text, static_cast<int>(strlen(b->text)) );
    XSetForeground( dpy, gc, fg );
    XSetBackground( dpy, gc, bg );
}

static int is_point_inside( button* b, int px, int py )
{
    return px>=b->x && px<=(b->x+(int)b->width-1) &&
           py>=b->y && py<=(b->y+(int)b->height-1);
}

void MessageBoxX11( const char* title, const char* text )
{
    const char* wmDeleteWindow = "WM_DELETE_WINDOW";
    int black, white, height = 0, direction, ascent, descent, X, Y, W=0, H;
    size_t i, lines = 0;
    char *atom;
    const char *end, *temp;
    button ok;
    Display* dpy;
    Window w;
    Atom wmDelete;
    GC gc;
    XFontStruct* font;
    XCharStruct overall;
    XSizeHints hints;
    XEvent e;

    /* Open a display */
    if( !(dpy = XOpenDisplay( 0 )) )
        return;

    /* Get us a white and black color */
    black = BlackPixel( dpy, DefaultScreen(dpy) );
    white = WhitePixel( dpy, DefaultScreen(dpy) );

    /* Create a window with the specified title */
    w = XCreateSimpleWindow( dpy, DefaultRootWindow(dpy), 0, 0, 100, 100,
                             0, black, black );

    XSelectInput( dpy, w, ExposureMask | StructureNotifyMask |
                          KeyReleaseMask | PointerMotionMask |
                          ButtonPressMask | ButtonReleaseMask );

    XMapWindow( dpy, w );
    XStoreName( dpy, w, title );

    wmDelete = XInternAtom( dpy, wmDeleteWindow, True );
    XSetWMProtocols( dpy, w, &wmDelete, 1 );

    /* Create a graphics context for the window */
    gc = XCreateGC( dpy, w, 0, 0 );

    XSetForeground( dpy, gc, white );
    XSetBackground( dpy, gc, black );

    /* Compute the printed width and height of the text */
    if( !(font = XQueryFont( dpy, XGContextFromGC(gc) )) )
        goto cleanup;

    for( temp=text; temp; temp = end ? (end+1) : NULL, ++lines )
    {
        end = strchr( temp, '\n' );

        XTextExtents( font, temp, end ? (unsigned int)(end-temp):strlen(temp),
                      &direction, &ascent, &descent, &overall );

        W = overall.width>W ? overall.width : W;
        height = (ascent+descent)>height ? (ascent+descent) : height;
    }

    /* Compute the shape of the window and adjust the window accordingly */
    W += 20;
    H = lines*height + height + 40;
    X = DisplayWidth ( dpy, DefaultScreen(dpy) )/2 - W/2;
    Y = DisplayHeight( dpy, DefaultScreen(dpy) )/2 - H/2;

    XMoveResizeWindow( dpy, w, X, Y, W, H );

    /* Compute the shape of the OK button */
    XTextExtents( font, "OK", 2, &direction, &ascent, &descent, &overall );

    ok.width = overall.width + 30;
    ok.height = ascent + descent + 5;
    ok.x = W/2 - ok.width/2;
    ok.y = H   - height - 15;
    ok.textx = ok.x + 15;
    ok.texty = ok.y + ok.height - 3;
    ok.mouseover = 0;
    ok.clicked = 0;
    ok.text = "OK";

    XFreeFontInfo( NULL, font, 1 ); /* We don't need that anymore */

    /* Make the window non resizeable */
    XUnmapWindow( dpy, w );

    hints.flags      = PSize | PMinSize | PMaxSize;
    hints.min_width  = hints.max_width  = hints.base_width  = W;
    hints.min_height = hints.max_height = hints.base_height = H;

    XSetWMNormalHints( dpy, w, &hints );
    XMapRaised( dpy, w );
    XFlush( dpy );

    /* Event loop */
    while( 1 )
    {
        XNextEvent( dpy, &e );
        ok.clicked = 0;

        if( e.type == MotionNotify )
        {
            if( is_point_inside( &ok, e.xmotion.x, e.xmotion.y ) )
            {
                if( !ok.mouseover )
                    e.type = Expose;

                ok.mouseover = 1;
            }
            else
            {
                if( ok.mouseover )
                    e.type = Expose;

                ok.mouseover = 0;
                ok.clicked = 0;
            }
        }

        switch( e.type )
        {
        case ButtonPress:
        case ButtonRelease:
            if( e.xbutton.button!=Button1 )
                break;

            if( ok.mouseover )
            {
                ok.clicked = e.type==ButtonPress ? 1 : 0;

                if( !ok.clicked )
                    goto cleanup;
            }
            else
            {
                ok.clicked = 0;
            }

        case Expose:
        case MapNotify:
            XClearWindow( dpy, w );

            /* Draw text lines */
            for( i=0, temp=text; temp; temp=end ? (end+1) : NULL, i+=height )
            {
                end = strchr( temp, '\n' );

                XDrawString( dpy, w, gc, 10, 10+height+i, temp,
                             end ? (unsigned int)(end-temp) : strlen(temp) );
            }

            /* Draw OK button */
            draw_button( &ok, white, black, dpy, w, gc );
            XFlush( dpy );
            break;

        case KeyRelease:
            if( XLookupKeysym( &e.xkey, 0 ) == XK_Escape )
                goto cleanup;
            break;

        case ClientMessage:
            atom = XGetAtomName( dpy, e.xclient.message_type );

            if( *atom == *wmDeleteWindow )
            {
                XFree( atom );
                goto cleanup;
            }

            XFree( atom );
            break;
        }
    }

cleanup:
    XFreeGC( dpy, gc );
    XDestroyWindow( dpy, w );
    XCloseDisplay( dpy );
}
#endif

void message_box(const std::string& title, const std::string& msg, bool err)
{
#ifdef linux
    MessageBoxX11(title.c_str(), msg.c_str());
#elif defined(_WIN32) || defined(WIN32)
    // This is untested
    // If someone confirms this works on windows
    // then remove this comment
    MessageBoxWindows(title, msg);
#endif

    if (err)
        std::exit(EXIT_FAILURE);
}
