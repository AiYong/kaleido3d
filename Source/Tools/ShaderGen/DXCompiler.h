#pragma once
#ifndef __DXCompiler_h__
#define __DXCompiler_h__

#if _WIN32

#include <d3dcommon.h>
#include <wrl/client.h>
#include <sstream>

namespace k3d 
{
	using namespace Microsoft::WRL;

	class DXCompilerOutput;

	class DXCompiler : public IShaderCompiler
	{
	public:
		DXCompiler() {}
		~DXCompiler() override {}

		IShaderCompilerOutput *		Compile(ShaderCompilerOption const& option, const char * source, size_t length) override;
		const char *				GetVersion() override;
		DXCompilerOutput *			Reflect(DXCompilerOutput * input);
	};
	
	class DXCompilerOutput : public IShaderCompilerOutput
	{
		friend class DXCompiler;
	public:
		DXCompilerOutput() {}
		~DXCompilerOutput() override {}

		const char*					GetErrorMsg() const					{ return m_ErrorMsg.c_str(); }
		const void*					Bytes() const override				{ return m_pData->GetBufferPointer(); }
		uint64						Length() const override				{ return m_pData->GetBufferSize(); }
		shaderbinding::BindingTable GetBindingTable() const override	{ return m_BindingTable; }
		const Attributes&			GetAttributes() const override		{ return m_Attributes;}

	private:
		ComPtr<ID3DBlob>			m_pData;
		shaderbinding::BindingTable	m_BindingTable;
		Attributes					m_Attributes;
		std::string					m_ErrorMsg;
		EShaderCompileResult		m_Result;
	};
}
#endif
#endif