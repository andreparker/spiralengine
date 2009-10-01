#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glExt.h>

/*
	This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
	The view content is basically an EAGL surface you render your OpenGL scene into.
	Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
*/
@interface EAGLView : UIView {
    
@private
    /* The pixel dimensions of the backbuffer */
    GLint m_bufferWidth;
    GLint m_bufferHeight;
    
    EAGLContext *m_eaglContext;
    
    /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
    GLuint m_renderBuffer, m_frameBuffer;
    
    /* OpenGL name for the depth buffer that is attached to m_frameBuffer, if it exists (0 if it does not exist) */
    GLuint m_depthBuffer;
}

- (EAGLContext*) GetRenderContext;
- (void)drawView;

@end
