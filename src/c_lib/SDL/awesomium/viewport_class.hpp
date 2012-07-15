#pragma once

namespace Awesomium
{

int getWebKeyFromSDLKey(SDLKey key);
void injectSDLKeyEvent(awe_webview* webView, const SDL_Event& event);
///linux only
//#include <unistd.h>

#define URL "http://www.google.com"


/*
struct chromeDisplay {
    int x;
    int y;
    int width;
    int height;
    int tex_id;
    void* webView;
};
*/

class ChromeViewport
{
	public:	
		int xoff;
		int yoff;
		int width;
		int height;

		bool inFocus;

		int texture_id;

		awe_webview* webView;
		unsigned int TEX0;

		ChromeViewport()
		{
			inFocus = false;

			xoff = 128;
			yoff = 128;
			width = 512;
			height = 512;

            TEX0 = 0;
			init_webview();
		}

		~ChromeViewport()
		{
			awe_webview_destroy(webView);
            //free textures etc..
		}

		void init_webview()
		{
			webView = awe_webcore_create_webview(width, height, false);
		    //default loading

            if(webView == NULL)
                printf("ERROR: webView is null!\n");
		    awe_webview_set_transparent(webView, 1); ///preserve transpanency of window
		    //Sets whether or not pages should be rendered with transparency preserved.
		    //(ex, for pages with style="background-color:transparent")

		#if 1
		    awe_string* url_str = awe_string_create_from_ascii(URL, strlen(URL));
		    awe_webview_load_url(webView, url_str, awe_string_empty(),awe_string_empty(), awe_string_empty());
		    awe_string_destroy(url_str);
		#endif

		}

        void init_render_surface()
        {
            const awe_renderbuffer* renderBuffer = awe_webview_render(webView);

            if(renderBuffer == NULL)
            {
                printf("chrome_viewport: init_render_surface, error renderBuffer is null\n"); 
                return;
            }
           
            glEnable(GL_TEXTURE_2D);
            glGenTextures(1, &TEX0);

            glBindTexture(GL_TEXTURE_2D, TEX0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        /*
            display->surface = SDL_CreateRGBSurfaceFrom((void *) awe_renderbuffer_get_buffer(renderBuffer),
                                    awe_renderbuffer_get_width(renderBuffer),
                                    awe_renderbuffer_get_height(renderBuffer),
                                    32,
                                    awe_renderbuffer_get_rowspan(renderBuffer),
                                    0x0000FF00, 0x00FF0000,
                                    0xFF000000, 0x000000FF);
        */


            //awe_renderbuffer_get_buffer(renderBuffer),
            //awe_renderbuffer_get_width(renderBuffer),
            //awe_renderbuffer_get_height(renderBuffer),
            //awe_renderbuffer_get_rowspan(renderBuffer)

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) awe_renderbuffer_get_buffer(renderBuffer) );

            glDisable(GL_TEXTURE_2D);

        }

		void update_webview()
		{
		    if( !awe_webview_is_dirty(webView) ) return;
	        //awe_rect rect = awe_webview_get_dirty_bounds(webView);
            if(TEX0 == 0) init_render_surface();

	        const awe_renderbuffer* renderBuffer = awe_webview_render(webView);

	        if (renderBuffer == NULL) 
	        {
	        	printf("Chrome Crashed!\n");
	        	return;
	    	}

	        glEnable(GL_TEXTURE_2D);
	        glBindTexture( GL_TEXTURE_2D, TEX0 );
	        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) awe_renderbuffer_get_buffer(renderBuffer) );
	        glDisable(GL_TEXTURE_2D);
		}

		void draw_webview()
		{
            if(TEX0 == 0) return;

		    glEnable(GL_TEXTURE_2D);
		    glBindTexture( GL_TEXTURE_2D, TEX0 );

		    float z = -0.5f;
		    float x0,y0,x1,y1;

		    x0 = xoff;
		    y0 = yoff;
		    x1 = xoff + width;
		    y1 = yoff + height;
		    //printf("(x0,y0,x1,t1= %f,%f,%f,%f \n", x0, y0, x1, y1);

		    glBegin( GL_QUADS );
		        glTexCoord2f( 0.0f, 1.0f);
		        glVertex3f( x0, y0, z );

		        glTexCoord2f( 1.0f, 1.0f );
		        glVertex3f( x1, y0, z );

		        glVertex3f( x1, y1, z );

		        glTexCoord2f( 0.0f, 0.0f );
		        glVertex3f( x0, y1, z );
		    glEnd();

		    glDisable(GL_TEXTURE_2D);
		}

		void focus()
		{
			awe_webview_focus(webView);
		}

		void unfocus()
		{
			awe_webview_unfocus(webView);
		}


		void set_html(char* html)
		{
   			awe_string* html_str = awe_string_create_from_ascii(html, strlen(html));
    		awe_webview_load_html(webView, html_str,awe_string_empty());
    		awe_string_destroy(html_str);
		}

		void set_url(char* url) 
		{
    		awe_string* url_str = awe_string_create_from_ascii(url, strlen(url));
    		awe_webview_load_url(webView, url_str, awe_string_empty(), awe_string_empty(), awe_string_empty()); 
    		awe_string_destroy(url_str);
		}


		void processKeyEvent(SDL_Event keyEvent) 
		{
		    if(inFocus == false)  return;
		    injectSDLKeyEvent(webView, keyEvent);
		}


};

/*
_OSMExport bool awe_webview_is_dirty  (   awe_webview *   webview )
_OSMExport const awe_renderbuffer* awe_webview_render (   awe_webview *   webview )
_OSMExport awe_rect awe_webview_get_dirty_bounds  (   awe_webview *   webview )
Returns the bounds of the area that has changed since the last call to awe_webview_render.
*/

/*
_OSMExport int  awe_renderbuffer_get_width (const awe_renderbuffer *renderbuffer)
_OSMExport int  awe_renderbuffer_get_height (const awe_renderbuffer *renderbuffer)
_OSMExport int  awe_renderbuffer_get_rowspan (const awe_renderbuffer *renderbuffer)
_OSMExport const unsigned char *    awe_renderbuffer_get_buffer (const awe_renderbuffer *renderbuffer)
*/


/*
    // Destroy our WebView instance
    awe_webview_destroy(webView);

    // Destroy our WebCore instance
    awe_webcore_shutdown();
*/

/*        awe_string* filename_str = awe_string_create_from_ascii("./result.jpg",
                                                        strlen("./result.jpg"));

        // Save our RenderBuffer directly to a JPEG image
        awe_renderbuffer_save_to_jpeg(renderBuffer, filename_str, 90);

        // Destroy our filename string
        awe_string_destroy(filename_str);
*/


///input handling crap

// Forward declaration, defined below
//int getWebKeyFromSDLKey(SDLKey key);

/**
* This utility function generates a WebKeyboardEvent directly from an SDL
* Key Event and injects it into a certain WebView.
*
* @param    webView The WebView that will receive the event.
* @param    event   The SDL Event (of type SDL_KEYDOWN or SDL_KEYUP).
*/

//void injectSDLKeyEvent(Awesomium::WebView* webView, const SDL_Event& event)
//_awe_webkeyboardevent convertSDLKeyEvent(const SDL_Event& event)

//int getWebKeyFromSDLKey(SDLKey key);

awe_webkeyboardevent convert_key_event(Awesomium::WebKeyboardEvent keyEvent) 
{
    awe_webkeyboardevent ke;

/*
    awe_webkey_type     type
awe_webkey_modifiers    modifiers
int     virtual_key_code
int     native_key_code
wchar16     text [4]
wchar16     unmodified_text [4]
bool    is_system_keyURL
*/
    ke.type = (awe_webkey_type)keyEvent.type;
    ke.modifiers = (awe_webkey_modifiers)keyEvent.modifiers;
    ke.virtual_key_code = keyEvent.virtualKeyCode;
    ke.native_key_code = keyEvent.nativeKeyCode;
    int i;
    //for(i=0; i<=19;i++) {        ke.text[i] = keyEvent.keyIdentifier; }
    for(i=0; i<=3;i++) {
        ke.text[i] = keyEvent.text[i];
        ke.unmodified_text[i] = keyEvent.unmodifiedText[i];
    }
    ke.is_system_key = keyEvent.isSystemKey;
    return ke;
}

void injectSDLKeyEvent(awe_webview* webView, const SDL_Event& event)
{
    if(!(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP))
        return;

    Awesomium::WebKeyboardEvent keyEvent;

    keyEvent.type = event.type == SDL_KEYDOWN?
        Awesomium::WebKeyboardEvent::TYPE_KEY_DOWN :
        Awesomium::WebKeyboardEvent::TYPE_KEY_UP;

    char* buf = new char[20];
    keyEvent.virtualKeyCode = getWebKeyFromSDLKey(event.key.keysym.sym);
    Awesomium::getKeyIdentifierFromVirtualKeyCode(keyEvent.virtualKeyCode,
                                                  &buf);
    strcpy(keyEvent.keyIdentifier, buf);
    delete[] buf;

    keyEvent.modifiers = 0;

    if(event.key.keysym.mod & KMOD_LALT || event.key.keysym.mod & KMOD_RALT)
        keyEvent.modifiers |= Awesomium::WebKeyboardEvent::MOD_ALT_KEY;
    if(event.key.keysym.mod & KMOD_LCTRL || event.key.keysym.mod & KMOD_RCTRL)
        keyEvent.modifiers |= Awesomium::WebKeyboardEvent::MOD_CONTROL_KEY;
    if(event.key.keysym.mod & KMOD_LMETA || event.key.keysym.mod & KMOD_RMETA)
        keyEvent.modifiers |= Awesomium::WebKeyboardEvent::MOD_META_KEY;
    if(event.key.keysym.mod & KMOD_LSHIFT || event.key.keysym.mod & KMOD_RSHIFT)
        keyEvent.modifiers |= Awesomium::WebKeyboardEvent::MOD_SHIFT_KEY;
    if(event.key.keysym.mod & KMOD_NUM)
        keyEvent.modifiers |= Awesomium::WebKeyboardEvent::MOD_IS_KEYPAD;

    keyEvent.nativeKeyCode = event.key.keysym.scancode;

    if(event.type == SDL_KEYUP)
    {
        ///webView->injectKeyboardEvent(keyEvent);
        awe_webview_inject_keyboard_event(webView, convert_key_event(keyEvent));
    }
    else
    {
        unsigned int chr;
        if((event.key.keysym.unicode & 0xFF80) == 0)
            chr = event.key.keysym.unicode & 0x7F;
        else
            chr = event.key.keysym.unicode;

        keyEvent.text[0] = chr;
        keyEvent.unmodifiedText[0] = chr;

        //webView->injectKeyboardEvent(keyEvent);
        awe_webview_inject_keyboard_event(webView, convert_key_event(keyEvent));
        if(chr)
        {
            keyEvent.type = Awesomium::WebKeyboardEvent::TYPE_CHAR;
            keyEvent.virtualKeyCode = chr;
            keyEvent.nativeKeyCode = chr;
            //webView->injectKeyboardEvent(keyEvent);
            awe_webview_inject_keyboard_event(webView, convert_key_event(keyEvent));
        }
    }
}



// A helper macro, used in 'getWebKeyFromSDLKey'
#define mapKey(a, b) case SDLK_##a: return Awesomium::KeyCodes::AK_##b;

// Translates an SDLKey virtual key code to an Awesomium key code
int getWebKeyFromSDLKey(SDLKey key)
{
    switch(key)
    {
    mapKey(BACKSPACE, BACK)
    mapKey(TAB, TAB)
    mapKey(CLEAR, CLEAR)
    mapKey(RETURN, RETURN)
    mapKey(PAUSE, PAUSE)
    mapKey(ESCAPE, ESCAPE)
    mapKey(SPACE, SPACE)
    mapKey(EXCLAIM, 1)
    mapKey(QUOTEDBL, 2)
    mapKey(HASH, 3)
    mapKey(DOLLAR, 4)
    mapKey(AMPERSAND, 7)
    mapKey(QUOTE, OEM_7)
    mapKey(LEFTPAREN, 9)
    mapKey(RIGHTPAREN, 0)
    mapKey(ASTERISK, 8)
    mapKey(PLUS, OEM_PLUS)
    mapKey(COMMA, OEM_COMMA)
    mapKey(MINUS, OEM_MINUS)
    mapKey(PERIOD, OEM_PERIOD)
    mapKey(SLASH, OEM_2)
    mapKey(0, 0)
    mapKey(1, 1)
    mapKey(2, 2)
    mapKey(3, 3)
    mapKey(4, 4)
    mapKey(5, 5)
    mapKey(6, 6)
    mapKey(7, 7)
    mapKey(8, 8)
    mapKey(9, 9)
    mapKey(COLON, OEM_1)
    mapKey(SEMICOLON, OEM_1)
    mapKey(LESS, OEM_COMMA)
    mapKey(EQUALS, OEM_PLUS)
    mapKey(GREATER, OEM_PERIOD)
    mapKey(QUESTION, OEM_2)
    mapKey(AT, 2)
    mapKey(LEFTBRACKET, OEM_4)
    mapKey(BACKSLASH, OEM_5)
    mapKey(RIGHTBRACKET, OEM_6)
    mapKey(CARET, 6)
    mapKey(UNDERSCORE, OEM_MINUS)
    mapKey(BACKQUOTE, OEM_3)
    mapKey(a, A)
    mapKey(b, B)
    mapKey(c, C)
    mapKey(d, D)
    mapKey(e, E)
    mapKey(f, F)
    mapKey(g, G)
    mapKey(h, H)
    mapKey(i, I)
    mapKey(j, J)
    mapKey(k, K)
    mapKey(l, L)
    mapKey(m, M)
    mapKey(n, N)
    mapKey(o, O)
    mapKey(p, P)
    mapKey(q, Q)
    mapKey(r, R)
    mapKey(s, S)
    mapKey(t, T)
    mapKey(u, U)
    mapKey(v, V)
    mapKey(w, W)
    mapKey(x, X)
    mapKey(y, Y)
    mapKey(z, Z)
    mapKey(DELETE, DELETE)
    mapKey(KP0, NUMPAD0)
    mapKey(KP1, NUMPAD1)
    mapKey(KP2, NUMPAD2)
    mapKey(KP3, NUMPAD3)
    mapKey(KP4, NUMPAD4)
    mapKey(KP5, NUMPAD5)
    mapKey(KP6, NUMPAD6)
    mapKey(KP7, NUMPAD7)
    mapKey(KP8, NUMPAD8)
    mapKey(KP9, NUMPAD9)
    mapKey(KP_PERIOD, DECIMAL)
    mapKey(KP_DIVIDE, DIVIDE)
    mapKey(KP_MULTIPLY, MULTIPLY)
    mapKey(KP_MINUS, SUBTRACT)
    mapKey(KP_PLUS, ADD)
    mapKey(KP_ENTER, SEPARATOR)
    mapKey(KP_EQUALS, UNKNOWN)
    mapKey(UP, UP)
    mapKey(DOWN, DOWN)
    mapKey(RIGHT, RIGHT)
    mapKey(LEFT, LEFT)
    mapKey(INSERT, INSERT)
    mapKey(HOME, HOME)
    mapKey(END, END)
    mapKey(PAGEUP, PRIOR)
    mapKey(PAGEDOWN, NEXT)
    mapKey(F1, F1)
    mapKey(F2, F2)
    mapKey(F3, F3)
    mapKey(F4, F4)
    mapKey(F5, F5)
    mapKey(F6, F6)
    mapKey(F7, F7)
    mapKey(F8, F8)
    mapKey(F9, F9)
    mapKey(F10, F10)
    mapKey(F11, F11)
    mapKey(F12, F12)
    mapKey(F13, F13)
    mapKey(F14, F14)
    mapKey(F15, F15)
    mapKey(NUMLOCK, NUMLOCK)
    mapKey(CAPSLOCK, CAPITAL)
    mapKey(SCROLLOCK, SCROLL)
    mapKey(RSHIFT, RSHIFT)
    mapKey(LSHIFT, LSHIFT)
    mapKey(RCTRL, RCONTROL)
    mapKey(LCTRL, LCONTROL)
    mapKey(RALT, RMENU)
    mapKey(LALT, LMENU)
    mapKey(RMETA, LWIN)
    mapKey(LMETA, RWIN)
    mapKey(LSUPER, LWIN)
    mapKey(RSUPER, RWIN)
    mapKey(MODE, MODECHANGE)
    mapKey(COMPOSE, ACCEPT)
    mapKey(HELP, HELP)
    mapKey(PRINT, SNAPSHOT)
    mapKey(SYSREQ, EXECUTE)
    default: return Awesomium::KeyCodes::AK_UNKNOWN;
    }
}


}