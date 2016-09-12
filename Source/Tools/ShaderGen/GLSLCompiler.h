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
		IShaderCompilerOutput*		Compile(ShaderCompilerOption const& option, const char * source) override;
	};


	struct GLSLOutput : public IShaderCompilerOutput
	{
		GLSLOutput() {}
		explicit GLSLOutput(SPIRV_T && in) : m_ByteCode( std::move(in) ) {}

		const char*					GetErrorMsg() const { return m_ErrorMsg.c_str(); }
		const void*					Bytes() const { return m_ByteCode.data(); }
		uint64						Length() const { return m_ByteCode.size()*sizeof(uint32); }
		shaderbinding::BindingTable GetBindingTable() const override { return m_BindingTable; }
		const Attributes &			GetAttributes() const { return m_Attributes; }

	private:
		friend GLSLOutput*	glslToSpv(	const rhi::EShaderType shader_type, const EShaderModel shader_model,
										const char *pshader);

		GLSLOutput*			reflect(GLSLOutput* input);

		SPIRV_T									m_ByteCode;
		Attributes								m_Attributes;
		shaderbinding::BindingTable				m_BindingTable;
		std::string								m_ErrorMsg;
		EShaderCompileResult					m_Result;
	};

	extern GLSLOutput*		glslToSpv(	const rhi::EShaderType shader_type,
										const char *pshader);
}

#endif