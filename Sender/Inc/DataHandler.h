#pragma once
#include "SharedMemory.h"
#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"
#include "Light.h"
#include "Material.h"

class DataHandler
{
public:
	DataHandler();
	~DataHandler();

	struct Choices
	{
		bool updateWholeScene;
		bool updateAnimation;
	};

	void StartSharing(unsigned int memorySize);
	void StopSharing();
	void SetChoices(Choices choices);
	void SetPath(MString path);
	static MStatus GetSceneData(MDagPath& dagPath);
	void ExportSelected();
	void SendCamera(bool send);
	void Remove(MDagPath& path);
	bool IsMemoryOpen();

private:
	Choices _choices;
	static string _path;

	static SharedMemory _sharedMemory;
	static SharedMemory::CircInfo _circInfo;
	static MStatus _status;
	static MCallbackIdArray _callbackIds;
	static Mesh _mesh;
	static Camera _camera;
	static Transform _transform;
	static Light _light;
	static Material _material;
	static string _identifier;
	static bool _sendCamera;

	// Callbacks
	static void CameraUpdate(const MString &str, void *clientData);
	static void MeshCreationCB(MObject& object, void* clientData);
	static void MeshChangedCB(MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug, void* clientData);
	static void TransformCreationCB(MObject& object, void* clientData);
	static void TransformChangedCB(MObject& node, MPlug& plug, void* clientData);
	static void MaterialCreationCB(MObject& object, void* clientData);
	static void MaterialChangedCB(MObject& node, MPlug& plug, void* clientData);
	static void LightCreationCB(MObject& object, void* clientData);
	static void LightChangedCB(MObject& node, MPlug& plug, void* clientData);
	static void NodeDestroyedCB(MObject& object, MDGModifier& modifier, void* clientData);
	static void FileNameChangeCB(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void *clientData);
	static void TimerFunctionCB(float elapsedTime, float lastTime, void *clientData);

	static void SendPackage(DataType type, PackageType packageType, unsigned int packageSize, unsigned int identifierSize);
	static void ExportMesh(MDagPath& dagPath);

	static MessageHeader _messageHeader;

	struct ObjectEntiy
	{
		unsigned int mesh;
		unsigned int transform;
	};
	static vector<ObjectEntiy> _objectEntities;

	struct ClientData
	{
		bool exist;
	};
	static vector<ClientData*> _clientDatas;
};