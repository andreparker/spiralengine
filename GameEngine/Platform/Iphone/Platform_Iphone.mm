#import <UIKit/UIKit.h>
#import <exception>
#import <iostream>

int main(int argc, char *argv[]) {
    
	int retVal;
	NSAutoreleasePool * pool;
	
	try 
	{
		pool = [[NSAutoreleasePool alloc] init];
		retVal = UIApplicationMain(argc, argv, nil, @"Platform_IphoneAppDelegate");
	}
	catch ( std::exception e ) 
	{
		std::cout << "[Exception Thrown]: " << e.what() << std::endl;
	}
	catch (...) 
	{
		std::cout << "[Exception Thrown]: unknown exception!" << std::endl;
	}
    
    [pool release];
    return retVal;
}
