#include "GUI.h"
#include <maya/MFnPlugin.h>
GUI* _gui;

EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus _status = MS::kSuccess;

	MFnPlugin plugin(obj, "Shared Memory", "1.0", "any", &_status);
	if (MFAIL(_status))
		CHECK_MSTATUS(_status);

	MStatus test;
	test = plugin.registerCommand("AddAllClass", AddAllClass::creator);
	plugin.registerCommand("AddSelectedClass", AddSelectedClass::creator);
	plugin.registerCommand("RemoveSelectedClass", RemoveSelectedClass::creator);
	plugin.registerCommand("UpdateWholeSceneClass", UpdateWholeSceneClass::creator);
	plugin.registerCommand("UpdateAnimationClass", UpdateAnimationClass::creator);
	plugin.registerCommand("MemorySizeClass", MemorySizeClass::creator);
	plugin.registerCommand("StartSharingClass", StartSharingClass::creator);
	plugin.registerCommand("StopSharingClass", StopSharingClass::creator);
	plugin.registerCommand("RestartClass", RestartClass::creator);
	plugin.registerCommand("CloseClass", CloseClass::creator);
	plugin.registerCommand("ExportSelectedClass", ExportSelectedClass::creator);
	plugin.registerCommand("SendCameraClass", SendCameraClass::creator);
	plugin.registerCommand("StopSendCameraClass", StopSendCameraClass::creator);
	plugin.registerCommand("BrowseClass", BrowseClass::creator);
	Check_Error(test);

	_gui = new GUI();

	if (_status == MS::kSuccess)
		Print("Shared Memory loaded!");
	else
		Print("Shared Memory failed to load!");

	return _status;
}

EXPORT MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterCommand("AddAllClass");
	plugin.deregisterCommand("AddSelectedClass");
	plugin.deregisterCommand("RemoveSelectedClass");
	plugin.deregisterCommand("UpdateWholeSceneClass");
	plugin.deregisterCommand("UpdateAnimationClass");
	plugin.deregisterCommand("MemorySizeClass");
	plugin.deregisterCommand("StartSharingClass");
	plugin.deregisterCommand("StopSharingClass");
	plugin.deregisterCommand("RestartClass");
	plugin.deregisterCommand("CloseClass");
	plugin.deregisterCommand("ExportSelectedClass");
	plugin.deregisterCommand("SendCameraClass");
	plugin.deregisterCommand("StopSendCameraClass");
	plugin.deregisterCommand("BrowseClass");

	delete _gui;
	Print("Shared Memory unloaded!");
	return MS::kSuccess;
}