#pragma once
#include"../DirectXCommon/Base.h"

/// <summary>
/// フォルダまでのパスを纏めました
/// リソースの読み込みでパスを書く場合は、読み込みたいファイルがあるフォルダを候補から選んで先頭に書いた後、ファイル名を書いてください
/// </summary>
namespace KazFilePathName
{
	static string RelativeResourcePath = "Resource/";

	//シェーダーパス-----------------------

	static string RelativeShaderPath = "KazLibrary/ShaderFiles/";
	static string VertexShaderPath = RelativeShaderPath + "VertexShader/";
	static string PixelShaderPath = RelativeShaderPath + "PixelShader/";
	static string GeometryShaderPath = RelativeShaderPath + "GeometryShader/";
	static string ComputeShaderPath = RelativeShaderPath + "ComputeShader/";


	//画像パス-----------------------

	static string TestPath = RelativeResourcePath + "Test/";
	static string CursorPath = RelativeResourcePath + "Cursor/";


	//デバックパス-----------------------

	static string ParamPath = RelativeResourcePath + "Parameter/";
	static string StageParamPath = ParamPath + "Stage/";

}