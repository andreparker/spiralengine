#import "Platform_IphoneAppDelegate.hpp"
#import "EAGLView.h"

@interface Platform_IphoneAppDelegate ()

@property (nonatomic, retain) UIWindow *m_window;
@property (nonatomic, retain) EAGLView *m_glView;
@property (nonatomic, assign) NSTimer *m_mainLoop;
@property (nonatomic, assign) NSTimeInterval m_tickInterval;

- (void) Tick;
- (void) SetUpMain;
- (void) SetTickInterval: (NSTimeIntervel)interval;
- (void) StopTick;

@end

@implementation Platform_IphoneAppDelegate

@synthesize m_window;
@synthesize m_glView;
@synthesize m_mainLoop;
@synthesize m_tickInterval;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
	CGRect rect = [[UIScreen mainScreen] bounds];
	m_glView = [[EAGLView alloc] initWithFrame: (CGRect)rect ];
	m_window = [[UIWindow alloc] initWithFrame: (CGRect)rect ];
	
	[ m_window addSubview: (UIView*)m_glView ];
	[ m_window makeKeyAndVisible];
	
	[ self SetUpMain ];
}


- (void)applicationWillResignActive:(UIApplication *)application {
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
}

- (void) Tick
{
	if( m_glView )
	{
		[ m_glView drawView ];
	}
}

- (void) SetUpMain
{
	[self SetTickInterval: (1.0f/60.0f)];
	m_mainLoop = [NSTimer scheduledTimerWithTimeInterval:m_tickInterval target:self selector:@selector(Tick) userInfo:nil repeats:YES];
}

- (void) SetTickInterval: (NSTimeInterval)interval
{
	m_tickInterval = interval;
}

- (void) StopTick
{
	m_mainLoop = nil;
}

- (void)dealloc {

	[self StopTick];
	[m_window release];
	[m_glView release];
	[super dealloc];
}

@end
