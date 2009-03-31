/*!
	@file platform_itouch
 
 */

#import <Foundation/Foundation.h>


@class platform_itouch_view;

@interface platform_itouch : NSObject<UIApplicationDelegate> {

	UIWindow* window;
	platform_itouch_view* view;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet platform_itouch_view *view;

@end
