#include "Kaleido3D.h"
#include "V8Engine.h"
#include <Core/AssetManager.h>

using namespace k3d;

namespace v8
{
	ScriptEngine::ScriptEngine()
	{
		initializeWithExtData();
		initialize();
	}

	ScriptEngine::~ScriptEngine()
	{
		destroy();
	}

	void ScriptEngine::initializeWithExtData() {
		IAsset * natives_blob = AssetManager::Open("asset://Test/natives_blob.bin");
		uint64 natives_blobs_length = natives_blob->GetLength();
		StartupData ndata = {(const char*)natives_blob->GetBuffer(), (int) natives_blobs_length};
		V8::SetNativesDataBlob(&ndata);
		
		IAsset * snapshot_blob = AssetManager::Open("asset://Test/snapshot_blob.bin");
		uint64 snapshot_blob_length = snapshot_blob->GetLength();
		StartupData sdata = {(const char*)snapshot_blob->GetBuffer(), (int) snapshot_blob_length};
		V8::SetSnapshotDataBlob(&sdata);

		auto platform = platform::CreateDefaultPlatform();
		V8::InitializePlatform(platform);
		V8::Initialize();

	}

	void ScriptEngine::initialize() {
		Isolate::CreateParams create_params;
		create_params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
		auto isolate = Isolate::New(create_params);
		isolate_ = isolate;
		isolate_->Enter();
		HandleScope handleScope(isolate);
		global_template_ = ObjectTemplate::New(isolate);
		//global_template_->Set(String::NewFromUtf8(isolate, "print"), FunctionTemplate::New(isolate, printMessage));
		Local<Context> context = Context::New(isolate, nullptr, global_template_);
		context_.Reset(isolate, context);
	}

	void ScriptEngine::destroy() {
		//isolate_->Exit();
		//isolate_->Dispose();
		V8::Dispose();
		V8::ShutdownPlatform();
	}
}