#ifndef __V8Engine_h__
#define __V8Engine_h__
#include <unordered_map>
#include "v8.h"
#include "libplatform/libplatform.h"

namespace v8
{
	class BaseObjectWrap
	{
	private:
		using ExternalMap = std::unordered_map<int, Persistent<External>*>;
		ExternalMap		externals_;
		Isolate *		isolate_;
	
	public:
		BaseObjectWrap()
		{}

		~BaseObjectWrap()
		{}

		template <class T>
		T * Unwrap(const FunctionCallbackInfo<Value>& args, int fieldId)
		{
			auto self = args.Holder();
			auto i = Local<External>::Cast(self->GetInternalField(fieldId));
			T* ptr = reinterpret_cast<T*>( i->Value() );
			return ptr;
		}

		template <class T>
		void Wrap(const FunctionCallbackInfo<Value>& args, int fieldId, T * ptr) {
			Local<Object> object = args.This();
			auto ext = External::New(isolate_, ptr);
			externals_[fieldId] = new Persistent<External>;
			externals_[fieldId]->Reset(isolate_, ext);
			//external_.SetWeak(point, )
			object->SetInternalField(fieldId, ext);
		}
	};

	class ScriptEngine
	{
	private:
		Isolate*                isolate_;
		Global<Context>         context_;
		Local<ObjectTemplate>   global_template_;
		bool                    initialized_;

	public:
		ScriptEngine();
		~ScriptEngine();

		Isolate* getIsoloate() const { return isolate_; }

	private:
		void initializeWithExtData();

		void initialize();
		void destroy();

		v8::Context::Scope *            context_scope_;
		v8::Persistent<v8::Context> *   primary_context_;
	};
}

#endif