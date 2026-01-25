/* ======================================================================================
 *  SystemWindow.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

/*
 * TODO
 *		-	Add error management.
 *		
 *		-	??? Remove SDL initialization and quit logic and transfer to Application class
 *			or even main function. ???
 * 
 *		-	Modify implementation of SystemWindow class to allow creation and management
 *			of multiple windows.
*/

#pragma once

#include <Core/OptimEngine.h>

/*
 * @brief 
 * This class represent an OS managed window.
*/
class CORE_API SystemWindow final
{
public:
	SystemWindow();
	~SystemWindow();

	/*
	 * @brief 
	 * Initializes SDL and creates a simple window that can be resized.
	 * For now the window does manage any message other than the QUIT message.
	 * 
	 * @param windowTitle 
	 *		>>> DOES NOT APPLY FOR NOW <<< The title of the window
	*/
	void initialize(const char* windowTitle);

	/*
	 * Runs the window loop.
	 * 
	 * @return 
	 * Returns true if the window should continue to run.
	 * Returns false if the window should close.
	*/
	bool loop();

	/*
	 * @brief
	 * Set the title of the system window.
	 * 
	 * @param title
	 * The new title of the window.
	*/
	void setWindowTitle(const char* title);

	/*
	 * @brief 
	 * Get the window's pointer as managed by the OS.
	 * 
	 * @return
	 * Pointer to window managed by the OS as a void*. Must be cast to appropriate
	 * pointer type when used in context.
	*/
	void* getSystemPointer();

	/*
	 * @brief
	 * Destroys the current window and unitializes SDL.
	*/
	void quit();
};