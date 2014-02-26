//
//  main.m
//  OpenGLtest
//
//  Created by Malek Bengougam on 28/10/2013.
//
//

#import "EsgiGL.h"

View* __view = NULL;

int dllmain(int argc, char *argv[])
{
	//NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];
   
	int width = renderer->__width;
	int height = renderer->__height;
	if (renderer->__fullscreen && width == 0 && height == 0)
	{
		CGRect mainMonitor = CGDisplayBounds(CGMainDisplayID());
		renderer->__width = CGRectGetWidth(mainMonitor);
		renderer->__height = CGRectGetHeight(mainMonitor);
	}
	
	NSApplication* app = [NSApplication sharedApplication];
    NSRect screenBounds = [[NSScreen mainScreen] frame];
    NSRect viewBounds = NSMakeRect(0, 0, renderer->__width, renderer->__height);
    
    __view = [[View alloc] initWithFrame:viewBounds];
    if (__view == NULL)
    {
        return -1;
    }
	
    NSRect centered = NSMakeRect(NSMidX(screenBounds) - NSMidX(viewBounds),
                                 NSMidY(screenBounds) - NSMidY(viewBounds),
                                 viewBounds.size.width,
                                 viewBounds.size.height);
    
    NSWindow* window = NULL;
    /*if (__fullscreen)
	 {
	 window = [[FullscreenWindow alloc]
	 initWithContentRect:screenBounds
	 styleMask:NSBorderlessWindowMask
	 backing:NSBackingStoreBuffered
	 defer:NO];
	 }
	 else*/
    {
        window = [[NSWindow alloc]
				  initWithContentRect:centered
                  styleMask:NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask
				  backing:NSBackingStoreBuffered
				  defer:NO];
    }
    
    [window setAcceptsMouseMovedEvents:YES];
    [window setContentView:__view];
    [window setDelegate:__view];
    //[__view release];
    
    [app run];
	
	//[p release];
	return 0;
}
