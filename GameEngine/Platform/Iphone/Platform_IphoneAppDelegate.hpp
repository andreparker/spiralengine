#import <UIKit/UIKit.h>

@class EAGLView;

@interface Platform_IphoneAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *m_window;
    EAGLView *m_glView;
	NSTimer* m_mainLoop;
	NSTimeInterval m_tickInterval;
}

@end

