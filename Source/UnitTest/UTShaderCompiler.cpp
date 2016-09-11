#include <Kaleido3D.h>
#include <Core/File.h>
#include <Tools/ShaderGen/ShaderCompiler.h>
#if _WIN32
#pragma comment(linker,"/subsystem:console")
#endif

using namespace k3d;

int main(int argc, char**argv)
{
	auto compiler = CreateShaderCompiler(EShaderLang::GLSL);
	ShaderCompilerOption option = {
		rhi::ES_Vertex,
		"",
		"main"
	};

	MemMapFile _vertexShader;
	_vertexShader.Open("../../Data/Test/triangle.vert", IOFlag::IORead);
	auto output = compiler->Compile(option, (const char*)_vertexShader.FileData());
	output->GetAttributes();

	File _output("triangle.vert.spv");
	_output.Open(IOWrite);
	_output.Write(output->GetShaderBytes(), output->GetByteCount());
	_output.Close();

	_vertexShader.Close();

	compiler = CreateShaderCompiler(EShaderLang::HLSL);
	ShaderCompilerOption option_hlsl = {
		rhi::ES_Vertex,
		"vs_5_1",
		"RenderSceneVS"
	};
	_vertexShader.Open("../../Data/Test/TestMaterial.hlsl", IOFlag::IORead);
	output = compiler->Compile(option_hlsl, (const char*)_vertexShader.FileData());
	_output.Open("TestMaterial.dxbc",IOWrite);
	_output.Write(output->GetShaderBytes(), output->GetByteCount());
	_output.Close();

	_vertexShader.Close();

	return 0;
}