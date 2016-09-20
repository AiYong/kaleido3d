#pragma once
#ifndef __GLSLCompiler_h__
#define __GLSLCompiler_h__

#include "ShaderCompiler.h"
#include <cassert>
#include <vector>

namespace k3d 
{
	typedef std::vector<uint32> SPIRV_T;

	class GLSLCompiler : public IShaderCompiler
	{
	public:
									GLSLCompiler();
									~GLSLCompiler() override;
		const char *				GetVersion() override;
		IShaderCompilerOutput*		Compile(ShaderCompilerOption const& option, const char * source, size_t length) override;
	};


	struct GLSLOutput : public IShaderCompilerOutput
	{
		GLSLOutput() {}
		explicit GLSLOutput(SPIRV_T && in, const rhi::EShaderType & shT) : m_ByteCode( std::move(in) ), m_ShaderType(shT) {}

		const char*					GetErrorMsg() const { return m_ErrorMsg.c_str(); }
		const void*					Bytes() const { return m_ByteCode.data(); }
		uint64						Length() const { return m_ByteCode.size()*sizeof(uint32); }
		shaderbinding::BindingTable GetBindingTable() const override { return m_BindingTable; }
		const Attributes &			GetAttributes() const { return m_Attributes; }

	private:
		friend class GLSLCompiler;
		friend GLSLOutput*	glslToSpv(	const rhi::EShaderType shader_type, const EShaderModel shader_model,
										const char *pshader, size_t length);

		void									reflect();
		rhi::EShaderType						m_ShaderType;
		SPIRV_T									m_ByteCode;
		Attributes								m_Attributes;
		shaderbinding::BindingTable				m_BindingTable;
		std::string								m_ErrorMsg;
		EShaderCompileResult					m_Result;
	};
}

#endif