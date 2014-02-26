// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

// --- Includes --------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import "EsgiMacOSX.h"

// --- Interface -------------------------------------------------------------

@interface EsgiGLViewContext : NSOpenGLView<NSWindowDelegate>
{
@protected
    CVDisplayLinkRef displayLink;
    NSRecursiveLock* gameLock;
	
    NSWindow * __window;
    NSApplication * __app;

@public
    const char *__title;
    unsigned int __width;
    unsigned int __height;
    unsigned int __renderflags;
    bool __vsync;
    bool __fullscreen;
}

-(bool) createContext;
-(void) destroyContext;
-(bool) createWindow:(const char *)name : (int)originex : (int)originey : (int)longueur : (int)hauteur : (unsigned int) flags;
-(void) closeWindow;
-(void) setVsync:(bool) enable;
-(void) swapBuffers;

@end