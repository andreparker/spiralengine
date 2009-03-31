#ifndef APP_C_WRAPPER_H
#define APP_C_WRAPPER_H

extern "C" {
	
	int app_Initialize();
	void app_Update( float delta );
	void app_Uninitialize();
	
};
#endif // APP_C_WRAPPER_H
