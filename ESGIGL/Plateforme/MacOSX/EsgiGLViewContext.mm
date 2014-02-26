// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

// --- Includes --------------------------------------------------------------

#import "EsgiGLViewContext.h"
#import <OpenGL/OpenGL.h>
#include "EsgiGL.h"

extern void esgiCleanFunc();
extern void esgiIdleFunc();
extern void esgiDisplayFunc();

// --- Implementation --------------------------------------------------------

@interface EsgiGLViewContext (PrivateMethods)
-(void) initGL;
@end

@implementation EsgiGLViewContext

//
// --- Initialisation -----------
//

// --- fenetre

-(bool) createWindow:(const char *)name : (int)originex : (int)originey : (int)longueur : (int)hauteur : (unsigned int) flags
{
	int width = longueur;
	int height = hauteur;
	
    char buffer[1024];
    getcwd(buffer, 1024);
    
	__app = [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    
    NSRect screenBounds = [[NSScreen mainScreen] frame];
    NSRect viewBounds = NSMakeRect(0, 0, width, height);
    
    __title = name;
    
    NSRect withOrigin = NSMakeRect(originex,
                                 originey,
                                 viewBounds.size.width,
                                 viewBounds.size.height);

    NSRect centered = NSMakeRect(NSMidX(screenBounds) - NSMidX(viewBounds),
                                 NSMidY(screenBounds) - NSMidY(viewBounds),
                                 viewBounds.size.width,
                                 viewBounds.size.height);
    __window = [[NSWindow alloc]
                initWithContentRect:(flags&0x80000000 ? centered : withOrigin)
              styleMask:NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask
              backing:NSBackingStoreBuffered
              defer:NO];

    if (__window == NULL)
    {
        return false;
    }
    
    __width = width;
    __height = height;
    
    [__window setTitle: [NSString stringWithUTF8String: __title ? __title : ""]];

    // Menu
    
    id menubar = [NSMenu new];
    id appMenuItem = [NSMenuItem new];
    [menubar addItem:appMenuItem];
    [NSApp setMainMenu:menubar];    
    id appMenu = [NSMenu new];
    id appName = [[NSProcessInfo processInfo] processName];
    id quitTitle = [@"Quitter " stringByAppendingString:appName];
    id quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
                                                  action:@selector(terminate:) keyEquivalent:@"q"];
    [appMenu addItem:quitMenuItem];
    [appMenuItem setSubmenu:appMenu];    
    
    return true;
}

bool EsgiContext::CreateWindow(const char *applicationName, int originx, int originy, int width, int height, unsigned int flags)
{
	NSRect viewBounds = NSMakeRect(0, 0, width, height);
    
    self = (__bridge_retained void *)[EsgiGLViewContext alloc];
    ((__bridge EsgiGLViewContext*)self)->__renderflags = flags;
    self = (__bridge void *)[(__bridge EsgiGLViewContext*)self initWithFrame:viewBounds];
    if (self == NULL)
        return false;   
    
    return [(__bridge EsgiGLViewContext*)self createWindow:applicationName :originx :originy :width :height :flags];
}

// --- context

-(bool)createContext
{
    [self initGL];
    return true;
}

bool EsgiContext::CreateContext()
{    
    return [(__bridge EsgiGLViewContext*)self createContext];
}


-(void) initGL
{
    // Make all the OpenGL calls to setup rendering and build the necessary rendering objects
    [[self openGLContext] makeCurrentContext];
}

//
// --- Finalisation ---------
//

-(void) destroyContext
{ 
    [self setOpenGLContext:nil];
}

void EsgiContext::DestroyContext()
{
    return [(__bridge EsgiGLViewContext*)self destroyContext];
}

-(void) closeWindow
{
    [[self window] close];
}

void EsgiContext::CloseWindow()
{
    return [(__bridge EsgiGLViewContext*)self closeWindow];
}

//
// --- Boucle principale -------
//

-(void) mainLoop
{
    [__window setAcceptsMouseMovedEvents:YES];
    [__window setContentView:self];
    [__window setDelegate:self];
    [NSApp activateIgnoringOtherApps:YES];
    [__app run];    
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    esgiCleanFunc();
}

void EsgiContext::MainLoop()
{ 
   return [(__bridge EsgiGLViewContext*)self mainLoop]; 
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void) keyDown: (NSEvent*) event
{
    if ([event isARepeat] == NO)
    {
        unichar c = [[event charactersIgnoringModifiers] characterAtIndex:0];
        
        bool __shiftDown = ([event modifierFlags] & NSShiftKeyMask);
        unsigned int caps = (__shiftDown ? 1 : 0) ^ (([event modifierFlags] & NSAlphaShiftKeyMask) ? 1 : 0);
        unsigned int key = [event keyCode];
        switch(key)        
        {
            case 0x35: // ESCAPE
            {
                [self closeWindow];
                break;
            }
            default:
                break;
        }
        
        [gameLock lock];
        [gameLock unlock];
    }
    
    // Allow other character to be handled (or not and beep)
    [super keyDown:event];
}

// --- rafraichissement

-(void) setVsync:(bool)enable
{
    //__vsync = enable;
    GLint swapInt = enable ? 1 : 0;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
}

-(void) swapBuffers
{
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

void EsgiContext::SwapBuffers()
{
   return [(__bridge EsgiGLViewContext*)self swapBuffers]; 
}

//
// --- Fonctions systeme ---------
//

// --- OpenGL initialisation

- (id) initWithFrame: (NSRect) frame
{  
    unsigned int samples = __renderflags & ESGI_WINDOW_MULTISAMPLE ? 2 : 0;
    
    // Note: Keep multisampling attributes at the start of the attribute lists since code below
    // assumes they are array elements 0 through 4.
    NSOpenGLPixelFormatAttribute windowedAttrs[] =
    {
        NSOpenGLPFAMultisample,
        NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)(samples ? 1 : 0),
        NSOpenGLPFASamples, samples,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, __renderflags & ESGI_WINDOW_RGBA ? 32 : 24,
        NSOpenGLPFAAlphaSize, __renderflags & ESGI_WINDOW_RGBA ? 8 : 0,
        NSOpenGLPFADepthSize, __renderflags & ESGI_WINDOW_DEPTH ? 24 : 0,
        NSOpenGLPFAStencilSize, __renderflags & ESGI_WINDOW_STENCIL, 
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,// NSOpenGLProfileVersion3_2Core,        
#endif
        0
    };
    /*NSOpenGLPixelFormatAttribute fullscreenAttrs[] =
     {
     NSOpenGLPFAMultisample,
     NSOpenGLPFASampleBuffers, (unsigned int)(samples ? 1 : 0),
     NSOpenGLPFASamples, samples,
     NSOpenGLPFADoubleBuffer,
     NSOpenGLPFAScreenMask, (NSOpenGLPixelFormatAttribute)CGDisplayIDToOpenGLDisplayMask(CGMainDisplayID()),
     NSOpenGLPFAFullScreen,
     NSOpenGLPFAColorSize, 32,
     NSOpenGLPFADepthSize, 24,
     NSOpenGLPFAAlphaSize, 8,
     NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
     0
     };*/
    NSOpenGLPixelFormatAttribute* attrs = windowedAttrs;//renderer->__fullscreen ? fullscreenAttrs : windowedAttrs;
    
    //renderer->__multiSampling = samples > 0;
	
    // Try to choose a supported pixel format
    NSOpenGLPixelFormat* pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    if (!pf)
    {
        bool valid = false;
        while (!pf && samples > 0)
        {
            samples /= 2;
            attrs[2] = samples ? 1 : 0;
            attrs[4] = samples;
            pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
            if (pf)
            {
                valid = true;
                break;
            }
        }
		
        //renderer->__multiSampling = samples > 0;
        
        if (!valid)
        {
            NSLog(@"OpenGL pixel format not supported.");
            return nil;
        }
    }
    
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
    
    if ((self = [super initWithFrame:frame pixelFormat:pf]))
    {
        gameLock = [[NSRecursiveLock alloc] init];
        //[self wantsBestResolutionOpenGLSurface];
        [self setOpenGLContext:context];
    }
    
    return self;
}

- (void) prepareOpenGL
{
    [super prepareOpenGL];
    
    //_game->run();
    
    /*if (__fullscreen)
     {
     [[self window] setLevel: NSMainMenuWindowLevel+1];
     [[self window] setHidesOnDeactivate:YES];
     }
     else*/
    {
        [[self window] setLevel: NSNormalWindowLevel];
    }
    [[self window] makeKeyAndOrderFront: self];
    
    //[self initGL];
    [self setVsync:false];
    
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, (__bridge void *)self);
    
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    GLint interval = 0;
    CGLSetParameter(cglContext, kCGLCPSwapInterval, &interval);
    //GLint dim[2] = {__width, __height};
    //CGLSetParameter(cglContext, kCGLCPSurfaceBackingSize, dim);
    //CGLEnable(cglContext, kCGLCESurfaceBackingSize);
    
    // Activate the display link
    CVDisplayLinkStart(displayLink);
    
    // register to be notified when the window closes so we can stop the displayLink
    //[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowWillClose:) name:NSWindowWillCloseNotification object:[self window]];
}


// --- display link callback

- (void)resumeDisplayRenderer
{
    [gameLock lock];
    CVDisplayLinkStop(displayLink);
    [gameLock unlock];
}

- (void)haltDisplayRenderer
{
    [gameLock lock];
    CVDisplayLinkStop(displayLink);
    [gameLock unlock];
}

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
    //NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    [self drawView];
    
    //[pool release];
    
    return kCVReturnSuccess;
}

static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime,
                                      CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(__bridge EsgiGLViewContext*)displayLinkContext getFrameForTime:outputTime];
    return result;
}


-(void) drawRect:(NSRect)dirtyRect
{
    // avoid flickering during resize
    [self drawView];
}

-(void) drawView
{
    [gameLock lock];
	
    [[self openGLContext] makeCurrentContext];
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

    // draw
    esgiIdleFunc();
    esgiDisplayFunc();
    
    //CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
	
    [gameLock unlock];
}

// --- gestion de la fenetre

-(void)windowWillClose:(NSNotification*)note
{
    [gameLock lock];
    if (displayLink)
    {
        CVDisplayLinkStop(displayLink);
        CVDisplayLinkRelease(displayLink);
        displayLink = nil;
        esgiCleanFunc();
    }
    [gameLock unlock];
    [[NSApplication sharedApplication] terminate:self];
}

- (void)windowDidResize:(NSNotification*)notification
{
    [gameLock lock];
    //NSSize size = [ [ _window contentView ] frame ].size;
    [gameLock unlock];
}

// --- window content update

- (void) reshape
{
    [super reshape];
    
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    
    //NSRect viewRectPoints = [self bounds];
    
    //NSRect viewRectPixels = viewRectPoints;
    
    //[m_renderer resizeWithWidth:viewRectPixels.size.width
    //                  AndHeight:viewRectPixels.size.height);
    
    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
    
}

-(void) renewGState
{
    [[self window] disableScreenUpdatesUntilFlush];
    [super renewGState];
}

/*
// --- Termine View, Window & App.

- (void) dealloc
{
    [gameLock lock];
    
    // Release the display link
    CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);
    
    [gameLock unlock];
}
*/

@end


@interface FullscreenWindow : NSWindow
{
}
@end

@implementation FullscreenWindow
- (BOOL)canBecomeKeyWindow
{
    return YES;
}
@end
