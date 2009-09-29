//
//  Platform_IphoneAppDelegate.h
//  Platform_Iphone
//
//  Created by admin on 9/28/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLView;

@interface Platform_IphoneAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *m_window;
    EAGLView *m_glView;
	NSTimer* m_mainLoop;
}

@property (nonatomic, retain) UIWindow *m_window;
@property (nonatomic, retain) EAGLView *m_glView;

@end

