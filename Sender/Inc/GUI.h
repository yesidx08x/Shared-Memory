#pragma once
#include "DataHandler.h"

class GUI
{
public:
	GUI();
	~GUI();

	void InitGUI();
	static unsigned int memorySize;
	static MString window;
	static MString pathField;
	static MStringArray path;
	static DataHandler dataHandler;
	static DataHandler::Choices choices;

private:
	MString _defaultMemSize;
};

class AddAllClass : public MPxCommand
{
private:
	void AddAll();

public:
	AddAllClass() {};
	~AddAllClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("SendAll Called\n");
		AddAll();
		return MS::kSuccess;
	}

	static void* creator(){return new AddAllClass;}
};

class AddSelectedClass : public MPxCommand
{
private:
	void AddSelected();

public:
	AddSelectedClass() {};
	~AddSelectedClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("AddSelected Called\n");
		AddSelected();
		return MS::kSuccess;
	}

	static void* creator(){return new AddSelectedClass;}
};

class RemoveSelectedClass : public MPxCommand
{
private:
	void RemoveSelected();

public:
	RemoveSelectedClass() {};
	~RemoveSelectedClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("AddSelected Called\n");
		RemoveSelected();
		return MS::kSuccess;
	}

	static void* creator() { return new RemoveSelectedClass; }
};

class UpdateWholeSceneClass : public MPxCommand
{
private:
	void UpdateWholeScene();

public:
	UpdateWholeSceneClass() {};
	~UpdateWholeSceneClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("UpdateWholeScene Called\n");
		UpdateWholeScene();
		return MS::kSuccess;
	}

	static void* creator() { return new UpdateWholeSceneClass; }
};

class UpdateAnimationClass : public MPxCommand
{
private:
	void UpdateAnimation();

public:
	UpdateAnimationClass() {};
	~UpdateAnimationClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("UpdateWholeScene Called\n");
		UpdateAnimation();
		return MS::kSuccess;
	}

	static void* creator() { return new UpdateAnimationClass; }
};

class MemorySizeClass : public MPxCommand
{
private:
	void MemorySize();

public:
	MemorySizeClass() {};
	~MemorySizeClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("UpdateWholeScene Called\n");
		MemorySize();
		return MS::kSuccess;
	}

	static void* creator() { return new MemorySizeClass; }
};

class StartSharingClass : public MPxCommand
{
private:
	void StartSharing();

public:
	StartSharingClass() {};
	~StartSharingClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("UpdateWholeScene Called\n");
		StartSharing();
		return MS::kSuccess;
	}

	static void* creator() { return new StartSharingClass; }
};

class StopSharingClass : public MPxCommand
{
private:
	void StopSharing();

public:
	StopSharingClass() {};
	~StopSharingClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("UpdateWholeScene Called\n");
		StopSharing();
		return MS::kSuccess;
	}

	static void* creator() { return new StopSharingClass; }
};

class RestartClass : public MPxCommand
{
private:
	void Restart();

public:
	RestartClass() {};
	~RestartClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("UpdateWholeScene Called\n");
		Restart();
		return MS::kSuccess;
	}

	static void* creator() { return new RestartClass; }
};

class CloseClass : public MPxCommand
{
private:
	void Close();

public:
	CloseClass() {};
	~CloseClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("UpdateWholeScene Called\n");
		Close();
		return MS::kSuccess;
	}

	static void* creator() { return new CloseClass; }
};

class ExportSelectedClass : public MPxCommand
{
private:
	void ExportSelected();

public:
	ExportSelectedClass() {};
	~ExportSelectedClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("Export Selected Called\n");
		ExportSelected();
		return MS::kSuccess;
	}

	static void* creator() { return new ExportSelectedClass; }
};

class SendCameraClass : public MPxCommand
{
private:
	void SendCamera();

public:
	SendCameraClass() {};
	~SendCameraClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("Send Camera Called\n");
		SendCamera();
		return MS::kSuccess;
	}

	static void* creator() { return new SendCameraClass; }
};

class StopSendCameraClass : public MPxCommand
{
private:
	void StopSendCamera();

public:
	StopSendCameraClass() {};
	~StopSendCameraClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("Send Camera Called\n");
		StopSendCamera();
		return MS::kSuccess;
	}

	static void* creator() { return new StopSendCameraClass; }
};

class BrowseClass : public MPxCommand
{
private:
	void Browse();

public:
	BrowseClass() {};
	~BrowseClass() {};

	virtual MStatus doIt(const MArgList&)
	{
		setResult("Export Selected Called\n");
		Browse();
		return MS::kSuccess;
	}

	static void* creator() { return new BrowseClass; }
};