#import "Platform_IphoneAppDelegate.hpp"
#import "EAGLView.h"

@implementation Platform_IphoneAppDelegate

@synthesize m_window;
@synthesize m_glView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
	CGRect rect = [[UIScreen mainScreen] bounds];
	m_glView = [[EAGLView alloc] initWithFrame: (CGRect)rect ];
	m_window = [[UIWindow alloc] initWithFrame: (CGRect)rect ];
	
	
	[ m_window addSubview: (UIView*)m_glView ];
	[ m_window makeKeyAndVisible];
	
	m_glView.animationInterval = 1.0 / 60.0;
	
	[m_glView startAnimation];
}


- (void)applicationWillResignActive:(UIApplication *)application {
	m_glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	m_glView.animationInterval = 1.0 / 60.0;
}


- (void)dealloc {
	[m_window release];
	[m_glView release];
	[super dealloc];
}

@end
