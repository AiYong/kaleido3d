#include "ShaderCompiler.h"

#if K3DPLATFORM_OS_WIN
#include "DXCompiler.h"
#include <d3dcompiler.h>
#include <wrl/client.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace k3d {

	using namespace shaderbinding;

	EDataType D3DSigTypeConvert(const D3D12_SIGNATURE_PARAMETER_DESC& desc)
	{
		EDataType result = EDataType::EUnknown;
		switch (desc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_SINT32:
		{
			if (desc.Mask == 1)
			{
				result = EDataType::EInt; break;
			}
			else if (desc.Mask <= 3)
			{
				result = EDataType::EInt2; break;
			}
			else if (desc.Mask <= 7)
			{
				result = EDataType::EInt3; break;
			}
			else if (desc.Mask <= 15)
			{
				result = EDataType::EInt4; break;
			}
		}
		break;

		case D3D_REGISTER_COMPONENT_UINT32:
		{
			if (desc.Mask == 1)
			{
				result = EDataType::EUInt; break;
			}
			else if (desc.Mask <= 3)
			{
				result = EDataType::EUInt2; break;
			}
			else if (desc.Mask <= 7)
			{
				result = EDataType::EUInt3; break;
			}
			else if (desc.Mask <= 15)
			{
				result = EDataType::EUInt4; break;
			}
		}
		break;

		case D3D_REGISTER_COMPONENT_FLOAT32:
		{
			if (desc.Mask == 1)
			{
				result = EDataType::EFloat; break;
			}
			else if (desc.Mask <= 3)
			{
				result = EDataType::EFloat2; break;
			}
			else if (desc.Mask <= 7)
			{
				result = EDataType::EFloat3; break;
			}
			else if (desc.Mask <= 15)
			{
				result = EDataType::EFloat4; break;
			}
		}
		break;

		}
		return result;
	}


	IShaderCompilerOutput * DXCompiler::Compile(ShaderCompilerOption const& option, const char * source)
	{
		using PtrBlob = Microsoft::WRL::ComPtr<ID3DBlob>;
		PtrBlob ShaderBlob, ErrorBlob;
#if defined(_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		UINT dwShaderFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT dwShaderFlags = 0;
#endif
		HRESULT Hr = ::D3DCompile(
			source, strlen(source), /**SrcData, SrcDataLen***/
			nullptr, nullptr, nullptr, /**SrcName, Defines, Includes**/
			option.EntryFunction.c_str(), option.ShaderModel.c_str(),
			dwShaderFlags, 0,
			ShaderBlob.GetAddressOf(), ErrorBlob.GetAddressOf());

		DXCompilerOutput * output = new DXCompilerOutput;
		
		if (FAILED(Hr))
		{
			output->m_ErrorMsg = { (const char*)ErrorBlob->GetBufferPointer(), ErrorBlob->GetBufferSize() };
			output->m_Result = EShaderCompileResult::Fail;
			return output;
		} 
		else 
		{
			output->m_Data = { (const char*)ShaderBlob->GetBufferPointer(), (uint32)ShaderBlob->GetBufferSize() };
			output = Reflect(output);
			return output;
		}
	}

	DXCompilerOutput * DXCompiler::Reflect(DXCompilerOutput * input)
	{
		Microsoft::WRL::ComPtr<ID3D12ShaderReflection> reflection;
		HRESULT refHr = D3DReflect(input->GetShaderBytes(), input->GetByteCount(), IID_PPV_ARGS(reflection.GetAddressOf()));
		D3D12_SHADER_DESC desc;
		if (SUCCEEDED(refHr))
		{
			refHr = reflection->GetDesc(&desc);
			printf("Shader Comipler : (%s)\n", desc.Creator);
			uint32 constBufferCount = desc.ConstantBuffers;
			for (uint32 i = 0; i < constBufferCount; i++) 
			{
				auto cb = reflection->GetConstantBufferByIndex(i);
				D3D12_SHADER_BUFFER_DESC sbd = {};
				cb->GetDesc(&sbd);
				printf("ConstBuffer -- VarName:(%s) VarSize:(%d) VarNum:(%d)\n", sbd.Name, sbd.Size, sbd.Variables);
				for (uint32 vid = 0; vid < sbd.Variables; vid++)
				{
//					auto var = reflection->Get
				}
			}

			uint32 bindResCount = desc.BoundResources;
			for (uint32 i = 0; i < bindResCount; i++)
			{
				D3D12_SHADER_INPUT_BIND_DESC sibd = {};
				reflection->GetResourceBindingDesc(i, &sibd);
				printf("BindResource -- VarName:(%s) VarBindPoint:(%d) VarBindCount:(%d) VarReg:(%d)\n", sibd.Name, sibd.BindPoint, sibd.BindCount, sibd.Type);
			}

			uint32 inputParamCount = desc.InputParameters;
			for (uint32 i = 0; i < inputParamCount; i++)
			{
				D3D12_SIGNATURE_PARAMETER_DESC spd = {};
				reflection->GetInputParameterDesc(i, &spd);
				auto dt = D3DSigTypeConvert(spd);
				printf("Input Param -- SemName:(%s) SemId:(%d) Type:(%d) Size:(%d)\n", spd.SemanticName, spd.SemanticIndex, spd.ComponentType);
			}

			input->m_Result = EShaderCompileResult::Success;
		}
		else {
			input->m_Result = EShaderCompileResult::Fail;
		}
		return input;
	}

}
#endif