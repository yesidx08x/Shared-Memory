#include "GUI.h"
unsigned int GUI::memorySize;
MString GUI::window;
MString GUI::pathField;
MStringArray GUI::path;
DataHandler GUI::dataHandler;
DataHandler::Choices GUI::choices;

GUI::GUI()
{
	if (DEBUG == false)
	{
		_defaultMemSize = MString("50");
		memorySize = _defaultMemSize.asUnsigned();
		MString imagePath = "C:/Users/stahl/Visual Studio 2015/Team Service/Sender/x64/Debug/MatrixScaled.jpg";

		int result = 0;
		MGlobal::executeCommand("windowPref -exists ""SharedMemoryGUI"";", result);
		if (result > 0)
		{
			MGlobal::executeCommand("windowPref -remove ""SharedMemoryGUI"";");
		}
		MGlobal::executeCommand("window -wh 215 105 -s true -title \"Shared Memory\" ""SharedMemoryGUI"";", window);
		MGlobal::executeCommand("rowColumnLayout - nc 2 - cw 1 100 - cw 2 100 -rs 1 5 -cs 1 5 -co 2 left 5 -ro 1 top 5 -ro 5 bottom 5;");

		// Buttons
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Add All\" -command ""AddAllClass"";");
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Add Selected\" -command ""AddSelectedClass"";");
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Remove Selected\" -command ""RemoveSelectedClass"";");
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Export Selected\" -command ""ExportSelectedClass"";");
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Send Camera\" -command ""SendCameraClass"";");
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Stop Send Camera\" -command ""StopSendCameraClass"";");

		// Numbers
		MGlobal::executeCommand("text -label \"Memory Allocation (MB)\" -align \"left\"; intField -value " + _defaultMemSize + " -changeCommand ""MemorySizeClass"" \"MemoryAllocation\";");

		// Execute buttons
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Start Sharing\" -command ""StartSharingClass"";");
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Stop Sharing\" -command ""StopSharingClass"";");
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Restart\" -command ""RestartClass"";");
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Close\" -command ""CloseClass"";");
		MGlobal::executeCommand("button -w 100 -h 25 -label \"Browse\" -command ""BrowseClass"";");

		// Textfield
		MGlobal::executeCommand("textField;", pathField);

		MGlobal::executeCommand("showWindow " + window + ";");
	}
	else
	{
		GUI::memorySize = 50;
		GUI::dataHandler.StartSharing(GUI::memorySize);
	}
}

GUI::~GUI()
{
	MStatus status;
	status = MGlobal::executeCommand("deleteUI \"SharedMemoryGUI\";");
	Check_Error(status);
}

void GUI::InitGUI(){}

void AddAllClass::AddAll()
{
	if (GUI::dataHandler.IsMemoryOpen())
	{
		MDagPath path;
		MItDag itTransform(MItDag::kDepthFirst, MFn::kTransform);
		for (; !itTransform.isDone(); itTransform.next())
		{
			itTransform.getPath(path);
			GUI::dataHandler.GetSceneData(path);
		}
		MItDag itLight(MItDag::kDepthFirst, MFn::kLight);
		for (; !itLight.isDone(); itLight.next())
		{
			itLight.getPath(path);
			GUI::dataHandler.GetSceneData(path);
		}
		MItDag itMesh(MItDag::kDepthFirst, MFn::kMesh);
		for (; !itMesh.isDone(); itMesh.next())
		{
			itMesh.getPath(path);
			GUI::dataHandler.GetSceneData(path);
		}
		Print("Send All");
	}
	else
	{
		Print("Error: Shared Memory not open");
	}
}

void AddSelectedClass::AddSelected()
{
	if (GUI::dataHandler.IsMemoryOpen())
	{
		MSelectionList activeList;
		MGlobal::getActiveSelectionList(activeList);
		MItSelectionList itSelection(activeList);
		MDagPath item;
		MObject component;
		unsigned int shapeCount;
		for (; !itSelection.isDone(); itSelection.next())
		{
			itSelection.getDagPath(item, component);
			GUI::dataHandler.GetSceneData(item);

			item.numberOfShapesDirectlyBelow(shapeCount);
			for (size_t i = 0; i < shapeCount; i++)
			{
				item.extendToShapeDirectlyBelow(i);
				GUI::dataHandler.GetSceneData(item);
			}
		}
		Print("Add Selected");
	}
	else
	{
		Print("Error: Shared Memory not open");
	}
}

void RemoveSelectedClass::RemoveSelected()
{
	if (GUI::dataHandler.IsMemoryOpen())
	{
		MSelectionList activeList;
		MGlobal::getActiveSelectionList(activeList);
		MItSelectionList itSelection(activeList);
		MDagPath item;
		MObject component;
		unsigned int shapeCount;
		for (; !itSelection.isDone(); itSelection.next())
		{
			itSelection.getDagPath(item, component);
			GUI::dataHandler.Remove(item);

			item.numberOfShapesDirectlyBelow(shapeCount);
			for (size_t i = 0; i < shapeCount; i++)
			{
				item.extendToShapeDirectlyBelow(i);
				GUI::dataHandler.Remove(item);
			}
		}
		Print("Remove Selected");
	}
	else
	{
		Print("Error: Shared Memory not open");
	}
}

void UpdateWholeSceneClass::UpdateWholeScene()
{
	if (GUI::choices.updateWholeScene)
	{
		GUI::choices.updateWholeScene = false;
		Print("Update Whole Scene False");
	}
	else
	{
		GUI::choices.updateWholeScene = true;
		Print("Update Whole Scene True");
	}
	GUI::dataHandler.SetChoices(GUI::choices);
}

void UpdateAnimationClass::UpdateAnimation()
{
	if (GUI::choices.updateAnimation)
	{
		GUI::choices.updateAnimation = false;
		Print("Update Animation False");
	}
	else
	{
		GUI::choices.updateAnimation = true;
		Print("Update Animation True");
	}
	GUI::dataHandler.SetChoices(GUI::choices);
}

void MemorySizeClass::MemorySize()
{
	int value;
	MGlobal::executeCommand("intField -q -value \"MemoryAllocation\";", value);
	GUI::memorySize = value;
	Print("Memory Size", value);
}

void StartSharingClass::StartSharing()
{
	GUI::dataHandler.StartSharing(GUI::memorySize);
	Print("Start Sharing");
}

void StopSharingClass::StopSharing()
{
	GUI::dataHandler.StopSharing();
	Print("Stop Sharing");
}

void RestartClass::Restart()
{
	GUI::dataHandler.StopSharing();
	GUI::dataHandler.StartSharing(GUI::memorySize);
	Print("Restart");
}

void CloseClass::Close()
{
	GUI::dataHandler.StopSharing();
	MGlobal::executeCommand("deleteUI -window " + GUI::window + ";");
	Print("Close");
}

void ExportSelectedClass::ExportSelected()
{
	GUI::dataHandler.ExportSelected();
	Print("Export Selected");
}

void SendCameraClass::SendCamera()
{
	GUI::dataHandler.SendCamera(true);
	Print("Send Camera");
}

void StopSendCameraClass::StopSendCamera()
{
	GUI::dataHandler.SendCamera(false);
	Print("Stop Send Camera");
}

void BrowseClass::Browse()
{
	MGlobal::executeCommand("fileDialog2 -fileMode 3 -dialogStyle 2 -okCaption \"Choose\";", GUI::path);
	GUI::path[0] += "/";
	GUI::dataHandler.SetPath(GUI::path[0]);
	MGlobal::executeCommand("textField -edit -text \"" + GUI::path[0] + "\" " + GUI::pathField + ";");
	Print("Browse");
}