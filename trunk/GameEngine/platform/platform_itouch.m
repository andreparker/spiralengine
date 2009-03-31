#import "platform_itouch.h"
#import "platform_itouch_view.h"

@implementation platform_itouch


@synthesize window;
@synthesize view;

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
	[view appInit];
	[view runApplicationTimer: 1/60];
}


- (void)applicationWillResignActive:(UIApplication *)application
{
	view.applicationInterval = 1/5;
}


- (void)applicationDidBecomeActive:(UIApplication *)application
{
	view.applicationInterval = 1/60;
}


- (void)dealloc {
	[view appUnInit];
	[window release];
	[view release];
	[super dealloc];
}

@end
