#include <Kaleido3D.h>
#include <Core/File.h>
#include <Tools/ShaderGen/ShaderCompiler.h>
#include <iostream>

using namespace std;

#if _WIN32
#pragma comment(linker,"/subsystem:console")
#endif

using namespace k3d;

int main(int argc, char**argv)
{
	auto compiler = CreateShaderCompiler(EShaderLang::GLSL);
	ShaderCompilerOption option = {
		rhi::ES_Vertex,
		k3d::EShaderModel::GL_4_3,
		"main",
		k3d::ShaderCompilerOption::CompileAndReflect
	};

	MemMapFile _vertexShader;
	_vertexShader.Open("../../Data/Test/triangle.vert", IOFlag::IORead);
	auto output = compiler->Compile(option, (const char*)_vertexShader.FileData());
	auto attr = output->GetAttributes();

	cout << compiler->GetVersion() << endl;

	File _output("triangle.vert.spv");
	_output.Open(IOWrite);
	_output.Write(output->Bytes(), output->Length());
	_output.Close();

	_vertexShader.Close();


	MemMapFile bytecode;
	bytecode.Open("triangle.vert.spv", IORead);

	ShaderCompilerOption glreflectOption = {
		rhi::ES_Vertex,
		k3d::EShaderModel::GL_4_3,
		"main",
		k3d::ShaderCompilerOption::ReflectByteCode
	};
	output = compiler->Compile(glreflectOption, (const char*)bytecode.FileData(), bytecode.GetSize());
	attr = output->GetAttributes();
	bytecode.Close();


	compiler = CreateShaderCompiler(EShaderLang::HLSL);
	cout << compiler->GetVersion() << endl;

	ShaderCompilerOption option_hlsl = {
		rhi::ES_Vertex,
		k3d::EShaderModel::SM_5_1,
		"RenderSceneVS",
		k3d::ShaderCompilerOption::CompileAndReflect
	};
	_vertexShader.Open("../../Data/Test/TestMaterial.hlsl", IOFlag::IORead);
	output = compiler->Compile(option_hlsl, (const char*)_vertexShader.FileData());
	_output.Open("TestMaterial.dxbc",IOWrite);
	_output.Write(output->Bytes(), output->Length());
	_output.Close();

	//_vertexShader.Close();

	// unavailable currently
	compiler = CreateShaderCompiler(EShaderLang::GLSL);
	output = compiler->Compile(option_hlsl, (const char*)_vertexShader.FileData());
	
	cout << compiler->GetVersion() << endl;

	_vertexShader.Close();

	return 0;
}