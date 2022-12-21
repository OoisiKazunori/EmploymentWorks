#pragma once
#include"../DirectXCommon/Base.h"
#include"../Pipeline/Shader.h"
#include"../Helper/RegisterNameWithData.h"
#include"../Helper/ISinglton.h"
#include"../Pipeline/GraphicsRootSignature.h"

struct InputLayOutData
{
	D3D12_INPUT_ELEMENT_DESC *inputLayOut;
	UINT size;
};

enum PipeLineDataNames
{
	PIPELINE_DATA_BACKCARING_ALPHABLEND,
	PIPELINE_DATA_BACKCARING_ALPHABLEND_RNEDERTARGET_SECOND,
	PIPELINE_DATA_NOCARING_ALPHABLEND_RNEDERTARGET_SECOND,
	PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME,
	PIPELINE_DATA_BACKCARING_NOBLEND_WIREFLAME,
	PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME_MUTITEX,
	PIPELINE_DATA_NOCARING_NOBLEND,
	PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
	PIPELINE_DATA_NOCARING_NOBLEND_LINE,
	PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_Z_ALWAYS,
	PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_MULTITEX,
	PIPELINE_DATA_NOCARING_BLENDALPHA,
	PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_LESSEQUAL,
	PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_ALWAYS,
	PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_OFF,
	PIPELINE_DATA_NOCARING_BLENDALPHA_MULTIPASS_TWO,
	PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTHOFF,
	PIPELINE_DATA_NOCARING_BLENDALPHA_CUT,
	PIPELINE_DATA_NOCARING_BLENDALPHA_CUT_MULTITEX,
	PIPELINE_DATA_NOCARING_ADDBLEND_CUTALPHA,
	PIPELINE_DATA_NOCARING_ALPHABLEND_WIREFLAME,
	PIPELINE_DATA_NOCARING_ADDBLEND,
	PIPELINE_DATA_NOCARING_NOBLEND_WIREFLAME,
	PIPELINE_DATA_NOCARING_NOBLEND_WIREFLAME_MULTIPASS_TWO,
	PIPELINE_DATA_EXPANTION_VERTEX,					// 頂点拡張用に前面カリング設定で追加しました。
	PIPELINE_DATA_NOCARING_CUTALPHA_DEPTH_ALWAYS,
	PIPELINE_DATA_MAX
};

enum ComputePipeLineDataNames
{
	PIPELINE_COMPUTE_DATA_NONE = -1,
	PIPELINE_COMPUTE_DATA_TEST
};

enum InputLayOutNames
{
	LAYOUT_POS_NORMAL_TEX,
	LAYOUT_POS_NORMAL,
	LAYOUT_POS_TEX,
	LAYOUT_POS,
	LAYOUT_POS_NORMAL_COLOR_TEX,
	LAYOUT_POS_NORMAL_TEX_BONE_WEIGHT,
	LAYOUT_POS_TICK,
	LAYOUT_MAX
};

enum VertexShaderNames
{
	SHADER_VERTEX_COLOR,
	SHADER_VERTEX_TEXTURE,
	SHADER_VERTEX_SPRITE,
	SHADER_VERTEX_OBJ,
	SHADER_VERTEX_OBJ_PORTAL,
	SHADER_VERTEX_LIGHT,
	SHADER_VERTEX_LUMINAS,
	SHADER_VERTEX_FBX,
	SHADER_VERTEX_FBX_FOG,
	SHADER_VERTEX_MULTIPASS,
	SHADER_VERTEX_FOG_COLOR,
	SHADER_VERTEX_FOG_OBJ,
	SHADER_VERTEX_LINE,
	SHADER_VERTEX_OBJ_LIGHT,
	SHADER_VERTEX_INSTANCE_OBJ,
	SHADER_VERTEX_INSTANCE_COLOR,
	SHADER_VERTEX_INSTANCE_OBJ_DEPTH,
	SHADER_VERTEX_INSTANCE_COLOR_SHADING,
	SHADER_VERTEX_INSTANCE_GET_SHADOWMAP,
	SHADER_VERTEX_INSTANCE_COLOR_GET_SHADOWMAP,
	SHADER_VERTEX_GPUPARTICLE,
	SHADER_VERTEX_GPUPARTICLE_POSUV,
	SHADER_VERTEX_PORTAL,
	SHADER_VERTEX_MAX
};

enum PixcelShaderNames
{
	SHADER_PIXCEL_COLOR,
	SHADER_PIXCEL_COLOR_MULTITEX,
	SHADER_PIXCEL_INSTANCE_COLOR_MULTITEX,
	SHADER_PIXCEL_TEXTURE,
	SHADER_PIXCEL_SPRITE,
	SHADER_PIXCEL_SPRITE_GRADATION,
	SHADER_PIXCEL_OBJ_GRADATION,
	SHADER_PIXCEL_SPRITE_MULTITEX,
	SHADER_PIXCEL_SPRITE_GOAL_EFFECT,
	SHADER_PIXCEL_OBJ,
	SHADER_PIXCEL_MONITOR_MULTITEX,
	SHADER_PIXCEL_OBJ_MULTITEX,
	SHADER_PIXCEL_EXPANTION_OBJ,
	SHADER_PIXCEL_LIGHT,
	SHADER_PIXCEL_LUMINAS,
	SHADER_PIXCEL_LUMINANCE_MULTI,
	SHADER_PIXCEL_GAUSSIAN_UPBLUR,
	SHADER_PIXCEL_GAUSSIAN_SIDEBLUR,
	SHADER_PIXCEL_FBX,
	SHADER_PIXCEL_BLOOM,
	SHADER_PIXCEL_GOALLIGHT,
	SHADER_PIXCEL_GOALLIGHT_MULTITEX,
	SHADER_PIXCEL_FOG_COLOR,
	SHADER_PIXCEL_FOG_COLOR_MULTITEX,
	SHADER_PIXCEL_FOG_MULTITEX,
	SHADER_PIXCEL_FOG_FBX,
	SHADER_PIXCEL_FOG_OBJ,
	SHADER_PIXCEL_LINE,
	SHADER_PIXCEL_TICK_LINE_FOG,
	SHADER_PIXCEL_FOG_SPRITE,
	SHADER_PIXCEL_LINE_FOG,
	SHADER_PIXCEL_LINE_FOG_FORBG,
	SHADER_PIXEL_LUMINANCE,
	SHADER_PIXCEL_OBJ_LIGHT,
	SHADER_PIXCEL_FOG_OBJ_MULTIPASS_BILL,
	SHADER_PIXCEL_OBJ_MULTIPASS_LIGHT,
	SHADER_PIXCEL_INSTANCE_OBJ,
	SHADER_PIXCEL_INSTANCE_COLOR,
	SHADER_PIXCEL_INSTANCE_COLOR_SHADING,
	SHADER_PIXCEL_INSTANCE_COLOR_GET_SHADOW,
	SHADER_PIXCEL_LINE_UV,
	SHADER_PIXCEL_LINE_UV_MULTITEX,
	SHADER_PIXCEL_LINE_FLASHEFFECT,
	SHADER_PIXCEL_INSTANCE_GET_SHADOWMAP,
	SHADER_PIXCEL_INSTANCE_OBJ_EXPANTION,
	SHADER_PIXCEL_WIHITENOISE,
	SHADER_PIXCEL_FBX_RENDER_TWO_LIGHT,
	SHADER_PIXCEL_FBX_RENDER_TWO,
	SHADER_PIXEL_GPUPARTICLE,
	SHADER_PIXEL_GPUPARTICLE_TEX,
	SHADER_PIXCEL_SPRITE_NOISE,
	SHADER_PIXCEL_SPRITE_WINDOW,
	SHADER_PIXEL_PORTAL,
	SHADER_PIXEL_SPRITE_COLOR,
	SHADER_PIXCEL_MAX
};

enum GeometoryShaderNames
{
	SHADER_GEOMETORY_NONE = -1,
	SHADER_GEOMETORY_LINE
};

enum ComputeShaderNames
{
	SHADER_COMPUTE_NONE = -1,
	SHADER_COMPUTE_TEST,
	SHADER_COMPUTE_FLOORPARTICLE,
	SHADER_COMPUTE_FLOORPARTICLE_MOVE,
	SHADER_COMPUTE_BLOCKPARTICLE,
	SHADER_COMPUTE_PORTALLINE,
	SHADER_COMPUTE_PORTALLINE_MOVE,
	SHADER_COMPUTE_BLOCKPARTICLE_MOVE,
	SHADER_COMPUTE_MESHPARTICLE_INIT,
	SHADER_COMPUTE_MESHPARTICLE_UPDATE,
	SHADER_COMPUTE_DEADPARTICLE_INIT,
	SHADER_COMPUTE_DEADPARTICLE_UPDATE,
	SHADER_COMPUTE_TEXTUREPARTICLE_INIT,
	SHADER_COMPUTE_TEXTUREPARTICLE_UPDATE,
	SHADER_COMPUTE_TEXTUREPARTICLE_FLASH_UPDATE,
	SHADER_COMPUTE_SPLINEPARTICLE_INIT,
	SHADER_COMPUTE_SPLINEPARTICLE_UPDATE,
	SHADER_COMPUTE_MESHPARTICLE,
	SHADER_COMPUTE_HITBOX_TRIANGLE_INIT,
	SHADER_COMPUTE_HITBOX_UPDATE,
	SHADER_COMPUTE_PARTICLEWALL_HITBOX,
	SHADER_COMPUTE_HITBOX_MESH,
	SHADER_COMPUTE_HITBOX_BB,
	SHADER_COMPUTE_HITBOX_SETHITBOX_IN_BB,
	SHADER_COMPUTE_HITBOX_CALUMAT
};

enum PipeLineNames
{
	PIPELINE_NAME_COLOR,
	PIPELINE_NAME_COLOR_LINE,
	PIPELINE_NAME_COLOR_NOCARING,
	PIPELINE_NAME_COLOR_MULTITEX,
	PIPELINE_NAME_INSTANCE_COLOR_MULTITEX,
	PIPELINE_NAME_INSTANCE_COLOR_MULTITEX_SHADING,
	PIPELINE_NAME_INSTANCE_COLOR,
	PIPELINE_NAME_INSTANCE_COLOR_WIREFLAME,
	PIPELINE_NAME_INSTANCE_COLOR_LINE,
	PIPELINE_NAME_COLOR_WIREFLAME,
	PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX,
	PIPELINE_NAME_TEXTURE,
	PIPELINE_NAME_SPRITE,
	PIPELINE_NAME_SPRITE_WINODW,
	PIPELINE_NAME_SPRITE_COLOR,
	PIPELINE_NAME_SPRITE_COLOR_FOG,
	PIPELINE_NAME_SPRITE_DEPTH_NOTEQUAL,
	PIPELINE_NAME_SPRITE_NOISE,
	PIPELINE_NAME_SPRITE_LUMI,
	PIPELINE_NAME_SPRITE_MULTITEX,
	PIPELINE_NAME_SPRITE_Z_ALWAYS,
	PIPELINE_NAME_SPRITE_Z_ALWAYS_CUTALPHA,
	PIPELINE_NAME_SPRITE_Z_OFF,
	PIPELINE_NAME_SPRITE_NOBLEND,
	PIPELINE_NAME_SPRITE_GOAL_EFFECT,
	PIPELINE_NAME_SPRITE_GOAL_EFFECT_MULTITEX,
	PIPELINE_NAME_SPRITE_GOAL_EFFECT_DEPTHOFF,
	PIPELINE_NAME_OBJ,
	PIPELINE_NAME_PCMONITOR_MULTITEX,
	PIPELINE_NAME_OBJ_MULTITEX,
	PIPELINE_NAME_BILL,
	PIPELINE_NAME_OBJ_MULTITEX_LIGHT,
	PIPELINE_NAME_OBJ_WIREFLAME,
	PIPELINE_NAME_OBJ_WIREFLAME_FOG,
	PIPELINE_NAME_OBJ_FOG_GRADATION,
	PIPELINE_NAME_OBJ_LIGHT,
	PIPELINE_NAME_LINE,
	PIPELINE_NAME_LINE_MULTITEX,
	PIPELINE_NAME_LINE_TICK,
	PIPELINE_NAME_LUMINANCE,
	PIPELINE_NAME_GAUSSIAN_UPBLUR,
	PIPELINE_NAME_GAUSSIAN_SIDEBLUR,
	PIPELINE_NAME_FBX,
	PIPELINE_NAME_FBX_RENDERTARGET_TWO,
	PIPELINE_NAME_FBX_RENDERTARGET_TWO_LIGHT,
	PIPELINE_NAME_ADDBLEND,
	PIPELINE_NAME_BLOOM,
	PIPELINE_NAME_MULTIPASS,
	PIPELINE_NAME_GOALLIGHT,
	PIPELINE_NAME_COLOR_LINE_Z_ALWAYS,
	PIPELINE_NAME_FOG_COLOR,
	PIPELINE_NAME_FOG_COLOR_MULTITEX,
	PIPELINE_NAME_FOG_FBX,
	PIPELINE_NAME_FOG_FBX_WIREFLAME,
	PIPELINE_NAME_FOG_FBX_WIREFLAME_MULTI_TWO,
	PIPELINE_NAME_FOG_OBJ,
	PIPELINE_NAME_FOG_LINE,
	PIPELINE_NAME_FOG_LINE_MULTIPASS,
	PIPELINE_NAME_FOG_LINE_FORBG,
	PIPELINE_NAME_FOG_SPRITE,
	PIPELINE_NAME_SPRITE_CUTALPHA,
	PIPELINE_NAME_SPRITE_CUTALPHA_MULTITEX,
	PIPELINE_NAME_SPRITE_VHS,
	PIPELINE_NAME_SPRITE_GRADATION,
	PIPELINE_NAME_INSTANCE_OBJ,
	PIPELINE_NAME_LINE_UV,
	PIPELINE_NAME_LINE_FLASHEFFECT,
	PIPELINE_NAME_LINE_UV_MULTITEX,
	PIPELINE_NAME_GPUPARTICLE,
	PIPELINE_NAME_GPUPARTICLE_TEX,
	PIPELINE_NAME_GPUPARTICLE_PORTAL,
	PIPELINE_NAME_PORTAL,
	PIPELINE_MAX
};

enum ComputePipeLineNames
{
	PIPELINE_COMPUTE_NAME_NONE = -1,
	PIPELINE_COMPUTE_NAME_FLOORPARTICLE,
	PIPELINE_COMPUTE_NAME_FLOORPARTICLE_MOVE,
	PIPELINE_COMPUTE_NAME_BLOCKPARTICLE,
	PIPELINE_COMPUTE_NAME_MESHPARTICLE,
	PIPELINE_COMPUTE_NAME_BLOCKPARTICLE_MOVE,
	PIPELINE_COMPUTE_NAME_TEXTUREPARTICLE_INIT,
	PIPELINE_COMPUTE_NAME_TEXTUREPARTICLE_UPDATE,
	PIPELINE_COMPUTE_NAME_TEXTUREPARTICLE_FLASH_UPDATE,
	PIPELINE_COMPUTE_NAME_PORTALLINE,
	PIPELINE_COMPUTE_NAME_PORTALLINE_MOVE,
	PIPELINE_COMPUTE_NAME_MESHPARTICLE_INIT,
	PIPELINE_COMPUTE_NAME_MESHPARTICLE_UPDATE,
	PIPELINE_COMPUTE_NAME_DEADPARTICLE_INIT,
	PIPELINE_COMPUTE_NAME_DEADPARTICLE_UPDATE,
	PIPELINE_COMPUTE_NAME_SPLINEPARTICLE_INIT,
	PIPELINE_COMPUTE_NAME_SPLINEPARTICLE_UPDATE,
	PIPELINE_COMPUTE_NAME_HITBOX_TRIANGLE_INIT,
	PIPELINE_COMPUTE_NAME_HITBOX_UPDATE,
	PIPELINE_COMPUTE_NAME_PARTICLEWALL_HITBOX,
	PIPELINE_COMPUTE_NAME_HITBOX_MESH,
	PIPELINE_COMPUTE_NAME_HITBOX_BB,
	PIPELINE_COMPUTE_NAME_HITBOX_SETCIRCLE_IN_BB,
	PIPELINE_COMPUTE_NAME_HITBOX_MESHCIRCLE_PARTICLE,
	PIPELINE_COMPUTE_NAME_HITBOX_CALUMAT,
	PIPELINE_COMPUTE_NAME_INIT_MESHPARTICLE,
	PIPELINE_COMPUTE_NAME_UPDATE_MESHPARTICLE
};

struct GraphicsPipelineData
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	ShaderData shaderOption;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc;
	RootSignatureDataTest rootsignatureData;
	PipeLineNames pipelineName;
};

/// <summary>
/// パイプラインの管理、生成の役割を持つクラスです
/// InputLayOut,Shader,D3D12_GRAPHICS_PIPELINE_STATE_DESCを予め登録し、
/// その後、それぞれの登録名を使用しパイプラインを生成します
/// </summary>
class GraphicsPipeLineMgr :public ISingleton<GraphicsPipeLineMgr>
{
public:
	GraphicsPipeLineMgr();
	~GraphicsPipeLineMgr();

	/// <summary>
	/// InputLayOutの情報と名前を登録します
	/// </summary>
	/// <param name="INPUT_LAYOUT">登録したいINPUTLAYOUTの変数</param>
	/// <param name="NAME">登録名</param>
	void RegisterInputLayOutWithData(InputLayOutData INPUT_LAYOUT, InputLayOutNames NAME);

	/// <summary>
	/// 頂点シェーダーのデータと名前を登録します
	/// </summary>
	/// <param name="VS_BLOB">登録したいShaderクラスでコンパイル済みの頂点シェーダー</param>
	/// <param name="NAME">登録名</param>
	void RegisterVertexShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, VertexShaderNames NAME);

	/// <summary>
	/// ピクセルシェーダーのデータと名前を登録します
	/// </summary>
	/// <param name="PS_BLOB">登録したいShaderクラスでコンパイル済みのピクセルシェーダー</param>
	/// <param name="NAME">登録名</param>
	void RegisterPixcelShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, PixcelShaderNames NAME);

	/// <summary>
	/// ジオメトリーシェーダーのデータと名前を登録します
	/// </summary>
	/// <param name="GS_BLOB">登録したいShaderクラスでコンパイル済みのジオメトリーシェーダー</param>
	/// <param name="NAME">登録名</param>
	void RegisterGeometoryShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, GeometoryShaderNames NAME);

	/// <summary>
	/// コンピュートシェーダーのデータと名前を登録します
	/// </summary>
	/// <param name="GS_BLOB">登録したいShaderクラスでコンパイル済みのコンピュートシェーダー</param>
	/// <param name="NAME">登録名</param>
	void RegisterComputeShaderWithData(string SHADER_FILE, LPCSTR ENTRY_POINT, LPCSTR SHADER_MODEL, ComputeShaderNames NAME);

	/// <summary>
	/// PipeLineの設定と名前を登録します
	/// </summary>
	/// <param name="PIPELINE_DATA">登録したいパイプラインの設定</param>
	/// <param name="NAME">登録名</param>
	void RegisterPipeLineDataWithData(D3D12_GRAPHICS_PIPELINE_STATE_DESC PIPELINE_DATA, PipeLineDataNames NAME);

	/// <summary>
	/// ComputePipeLineの設定と名前を登録します
	/// </summary>
	/// <param name="PIPELINE_DATA">登録したいパイプラインの設定</param>
	/// <param name="NAME">登録名</param>
	void RegisterComputePipeLineDataWithData(D3D12_COMPUTE_PIPELINE_STATE_DESC PIPELINE_DATA, ComputePipeLineDataNames NAME);

	/// <summary>
	/// 登録されたデータとルートシグネチャを組みわせて、パイプラインを生成します
	/// </summary>
	/// <param name="INPUT_LAYOUT_NAME">登録したINPUT_LAYOUTの名前</param>
	/// <param name="SHADER_NAME">登録したShaderの名前</param>
	/// <param name="PIPELINE_DATA_NAME">登録したPipeLineDataの名前</param>
	/// <param name="ROOT_SIGNATURE">ルートシグネチャの設定</param>
	/// <param name="NAME">パイプラインの登録名</param>
	void CreatePipeLine(InputLayOutNames INPUT_LAYOUT_NAME, VertexShaderNames VERTEX_SHADER_NAME, PixcelShaderNames PIXCEL_SHADER_NAME, PipeLineDataNames PIPELINE_DATA_NAME, RootSignatureMode ROOTSIGNATURE, PipeLineNames PIPELINE_NAME, GeometoryShaderNames GEOMETORY_SHADER_NAME = SHADER_GEOMETORY_NONE);

	/// <summary>
	/// 登録されたデータとルートシグネチャを組みわせて、パイプラインを生成します
	/// </summary>
	/// <param name="SHADER_NAME">登録したShaderの名前</param>
	/// <param name="PIPELINE_DATA_NAME">登録したPipeLineDataの名前</param>
	/// <param name="ROOT_SIGNATURE">ルートシグネチャの設定</param>
	/// <param name="NAME">パイプラインの登録名</param>
	void CreateComputePipeLine(ComputeShaderNames COMPUTE_SHADER_NAME, ComputePipeLineDataNames PIPELINE_DATA_NAME, RootSignatureMode ROOTSIGNATURE, ComputePipeLineNames PIPELINE_NAME);

	/// <summary>
	/// 登録されたパイプラインの中から指定のパイプラインをコマンドリストに積みます
	/// </summary>
	/// <param name="PIPELINE_NAME">登録したパイプラインの名前</param>
	bool SetPipeLineAndRootSignature(PipeLineNames PIPELINE_NAME);


	/// <summary>
	/// 登録されたコンピュートパイプラインの中から指定のパイプラインをコマンドリストに積みます
	/// </summary>
	/// <param name="PIPELINE_NAME">登録したパイプラインの名前</param>
	bool SetComputePipeLineAndRootSignature(ComputePipeLineNames PIPELINE_NAME);
	bool SetComputePipeLineAndRootSignature2(ComputePipeLineNames PIPELINE_NAME);


	/// <summary>
	/// パイプラインと関連付けしたルートシグネチャーを呼び出します
	/// </summary>
	/// <param name="PIPELINE_NAME">パイプラインの名前</param>
	/// <returns>パイプラインと関連付けしたルートシグネチャー</returns>
	RootSignatureMode GetRootSignatureName(PipeLineNames PIPELINE_NAME);



	void CreatePipeLine(
		std::vector<D3D12_INPUT_ELEMENT_DESC> INPUT_LAYOUT_NAME,
		const ShaderOptionData &VERTEX_SHADER_NAME,
		const ShaderOptionData &PIXEL_SHADER_NAME,
		const D3D12_GRAPHICS_PIPELINE_STATE_DESC &PIPELINE_DATA_NAME,
		const RootSignatureDataTest &ROOTSIGNATURE,
		PipeLineNames PIPELINE_NAME,
		const ShaderOptionData &GEOMETORY_SHADER_NAME = ShaderOptionData()
	);

	void CreateComputePipeLine(
		const ShaderOptionData &COMPUTE_SHADER,
		const D3D12_COMPUTE_PIPELINE_STATE_DESC &PIPELINE_DATA_NAME,
		const RootSignatureDataTest &ROOTSIGNATURE,
		ComputePipeLineNames PIPELINE_NAME
	);

private:

	static const int PIPELINE_MAX_NUM = 100;
	//登録データ

	std::array<InputLayOutData, PIPELINE_MAX_NUM> inputLayOutRegisterData;
	std::array<Microsoft::WRL::ComPtr<IDxcBlob>, PIPELINE_MAX_NUM> vertexShaderRegisterData;
	std::array<Microsoft::WRL::ComPtr<IDxcBlob>, PIPELINE_MAX_NUM> pixcelShaderRegisterData;
	std::array<Microsoft::WRL::ComPtr<IDxcBlob>, PIPELINE_MAX_NUM> geometoryShaderRegisterData;
	std::array<RootSignatureMode, PIPELINE_MAX_NUM> rootSignatureName;
	std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, PIPELINE_MAX_NUM> rootSignature;
	std::array<D3D12_GRAPHICS_PIPELINE_STATE_DESC, PIPELINE_MAX_NUM> pipeLineDataRegisterData;
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, PIPELINE_MAX_NUM> pipeLineRegisterData;

	std::array<Microsoft::WRL::ComPtr<IDxcBlob>, PIPELINE_MAX_NUM> computeShaderRegisterData;
	std::array<D3D12_COMPUTE_PIPELINE_STATE_DESC, PIPELINE_MAX_NUM> computePipeLineDataRegisterData;
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, PIPELINE_MAX_NUM> computePipeLineRegisterData;
	std::array<RootSignatureMode, PIPELINE_MAX_NUM> computeRootSignatureName;

	bool geo;


	template <typename T>
	bool IsitSafe(T NAME, size_t SIZE);

};