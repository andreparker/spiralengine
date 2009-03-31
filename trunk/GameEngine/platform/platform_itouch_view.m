//
//  platform_itouch_view.m
//  xCode
//
//  Created by admin on 3/24/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import "platform_itouch_view.h"


@interface platform_itouch_view ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *applicationTimer;

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;

- (void) oglBeginRender;
- (void) oglEndRender;
- (void) oglPresent;


- (void) appMain;

@end

@implementation platform_itouch_view

@synthesize context;
@synthesize applicationTimer;
@synthesize applicationInterval;

// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}

//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder 
{
    
    if ((self = [super initWithCoder:coder])) 
	{
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = NO;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context]) {
            [self release];
            return nil;
        }
        
        applicationInterval = 1.0 / 60.0;
    }
    return self;
}

- (void) oglBeginRender
{
	// bind the context and set the draw buffer
	[EAGLContext setCurrentContext:context];
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
}

- (void) oglEndRender
{
	// set the render buffer to present out put
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
}

- (void) oglPresent
{
	// present results
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void)layoutSubviews 
{
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
}

- (void) runApplicationTimer: (NSTimeInterval) interval
{
	applicationInterval = interval;
	self.applicationTimer = nil;
	self.applicationTimer = [NSTimer scheduledTimerWithTimeInterval:applicationInterval target:self selector:@selector(appMain) userInfo:nil repeats:YES];
}

- (void)setAnimationTimer:(NSTimer *)newTimer {
	int i = 0;
  //  [animationTimer invalidate];
  //  animationTimer = newTimer;
}


- (void)setAnimationInterval:(NSTimeInterval)interval {
    
	int i = 0;
   // animationInterval = interval;
   // if (animationTimer) {
   //     [self stopAnimation];
   //     [self startAnimation];
   // }
}

- (BOOL) appInit
{
	// initialization code
	return YES;
}

- (void) appUnInit
{
}

- (void) appMain
{
	[self oglBeginRender];
	// application main code goes here
	[self oglEndRender];
	[self oglPresent];
}

- (BOOL)createFramebuffer 
{
    
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);
    
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
	glGenRenderbuffersOES(1, &depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
    
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }
    
    return YES;
}


- (void)destroyFramebuffer {
    
	if( viewFramebuffer )
	{
		glDeleteFramebuffersOES(1, &viewFramebuffer);
		viewFramebuffer = 0;
	}
	
	if( viewRenderbuffer )
	{
        glDeleteRenderbuffersOES(1, &viewRenderbuffer);
		viewRenderbuffer = 0;
	}
    
    if(depthRenderbuffer) 
	{
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}

- (void)dealloc {
	
	self.applicationTimer = nil;
	
	if ([EAGLContext currentContext] == context) 
	{
        [EAGLContext setCurrentContext:nil];
    }
    
    [context release];
    [super dealloc];
}


@end
