#include "RenderState.h"

#include "DXTrace.h"
ComPtr<ID3D11RasterizerState> RenderState::RSNoCull = nullptr;
ComPtr<ID3D11RasterizerState> RenderState::RSWireframe = nullptr;
ComPtr<ID3D11RasterizerState> RenderState::RSCullClockWise = nullptr;
ComPtr<ID3D11RasterizerState> RenderState::RSCullFront = nullptr;
ComPtr<ID3D11RasterizerState> RenderState::RSShadow = nullptr;

ComPtr<ID3D11SamplerState> RenderState::SSPointClamp = nullptr;
ComPtr<ID3D11SamplerState> RenderState::SSLinearWrap = nullptr;
ComPtr<ID3D11SamplerState> RenderState::SSLinearClamp = nullptr;
/*
ComPtr<ID3D11SamplerState> RenderState::SSAnisotropicWrap16x = nullptr;
ComPtr<ID3D11SamplerState> RenderState::SSAnisotropicClamp2x = nullptr;
ComPtr<ID3D11SamplerState> RenderState::SSAnisotropicClamp4x = nullptr;
ComPtr<ID3D11SamplerState> RenderState::SSAnisotropicClamp8x = nullptr;
ComPtr<ID3D11SamplerState> RenderState::SSAnisotropicClamp16x = nullptr;
*/
ComPtr<ID3D11SamplerState> RenderState::SSShadowPCF = nullptr;

ComPtr<ID3D11BlendState> RenderState::BSAlphaToCoverage = nullptr;
ComPtr<ID3D11BlendState> RenderState::BSTransparent = nullptr;
ComPtr<ID3D11BlendState> RenderState::BSAdditive = nullptr;
ComPtr<ID3D11BlendState> RenderState::BSAlphaWeightedAdditive = nullptr;
ComPtr<ID3D11BlendState> RenderState::BSColorDodge = nullptr;
ComPtr<ID3D11BlendState> RenderState::BSMulti = nullptr;

ComPtr<ID3D11DepthStencilState> RenderState::DSSEqual = nullptr;
ComPtr<ID3D11DepthStencilState> RenderState::DSSLessEqual = nullptr;
ComPtr<ID3D11DepthStencilState> RenderState::DSSGreaterEqual = nullptr;
ComPtr<ID3D11DepthStencilState> RenderState::DSSNoDepthWrite = nullptr;
ComPtr<ID3D11DepthStencilState> RenderState::DSSNoDepthTest = nullptr;
ComPtr<ID3D11DepthStencilState> RenderState::DSSWriteStencil = nullptr;
ComPtr<ID3D11DepthStencilState> RenderState::DSSEqualStencil = nullptr;

bool RenderState::IsInit()
{
	//初期化できたどうかの判断
	return RSWireframe != nullptr;
}

void RenderState::InitAll(ID3D11Device* device)
{
	if (IsInit())
		return;
    //=========================
	//Rasterizer Init
	//=========================
	CD3D11_RASTERIZER_DESC rasterizerDesc(CD3D11_DEFAULT{});

    //Wire Frame
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    HR(device->CreateRasterizerState(&rasterizerDesc, RSWireframe.GetAddressOf()));

    // NoCull
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    HR(device->CreateRasterizerState(&rasterizerDesc, RSNoCull.GetAddressOf()));

    //CullClock
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = true;
    HR(device->CreateRasterizerState(&rasterizerDesc, RSCullClockWise.GetAddressOf()));

    // CullFront 
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_FRONT;  // 剔除正面
    rasterizerDesc.FrontCounterClockwise = false;  // 默认情况下，顺时针为正面
    HR(device->CreateRasterizerState(&rasterizerDesc, RSCullFront.GetAddressOf()));

    //Shadow
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.SlopeScaledDepthBias = 1.0f;
    HR(device->CreateRasterizerState(&rasterizerDesc, RSShadow.GetAddressOf()));


    //=========================
	//Sampler Init
	//=========================
	CD3D11_SAMPLER_DESC sampDesc(CD3D11_DEFAULT{});

    // Point Clamp
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    HR(device->CreateSamplerState(&sampDesc, SSPointClamp.GetAddressOf()));

    // linear Clamp
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    HR(device->CreateSamplerState(&sampDesc, SSLinearClamp.GetAddressOf()));

    /*
    sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampDesc.MaxAnisotropy = 2;
    HR(device->CreateSamplerState(&sampDesc, SSAnisotropicClamp2x.GetAddressOf()));

 
    sampDesc.MaxAnisotropy = 4;
    HR(device->CreateSamplerState(&sampDesc, SSAnisotropicClamp4x.GetAddressOf()));


    sampDesc.MaxAnisotropy = 8;
    HR(device->CreateSamplerState(&sampDesc, SSAnisotropicClamp8x.GetAddressOf()));


    sampDesc.MaxAnisotropy = 16;
    HR(device->CreateSamplerState(&sampDesc, SSAnisotropicClamp16x.GetAddressOf()));


	*/
    // linear wrap
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.MaxAnisotropy = 0;
    HR(device->CreateSamplerState(&sampDesc, SSLinearWrap.GetAddressOf()));

    /*
    sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampDesc.MaxAnisotropy = 16;
    HR(device->CreateSamplerState(&sampDesc, SSAnisotropicWrap16x.GetAddressOf()));
    */


    // Depth Border
    sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
    sampDesc.MaxAnisotropy = 1;
    sampDesc.MinLOD = 0.0f;
    sampDesc.MaxLOD = 0.0f;
    sampDesc.BorderColor[0] = 0.0f;
    sampDesc.BorderColor[1] = 0.0f;
    sampDesc.BorderColor[2] = 0.0f;
    sampDesc.BorderColor[3] = 1.0f;
    HR(device->CreateSamplerState(&sampDesc, SSShadowPCF.GetAddressOf()));

    //=========================
	//Blend State Init
	//=========================
    CD3D11_BLEND_DESC blendDesc(CD3D11_DEFAULT{});
    auto& rtDesc = blendDesc.RenderTarget[0];

    // Alpha-To-Coverage模式
    blendDesc.AlphaToCoverageEnable = true;
    HR(device->CreateBlendState(&blendDesc, BSAlphaToCoverage.GetAddressOf()));

    // alpha blend
    // Color = SrcAlpha * SrcColor + (1 - SrcAlpha) * DestColor 
    // Alpha = SrcAlpha
    blendDesc.AlphaToCoverageEnable = false;
    rtDesc.BlendEnable = true;
    rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
    rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    HR(device->CreateBlendState(&blendDesc, BSTransparent.GetAddressOf()));

    // Add
    // Color = SrcColor + DestColor
    // Alpha = SrcAlpha + DestAlpha
    rtDesc.SrcBlend = D3D11_BLEND_ONE;
    rtDesc.DestBlend = D3D11_BLEND_ONE;
    rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
    rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtDesc.DestBlendAlpha = D3D11_BLEND_ONE;
    rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    HR(device->CreateBlendState(&blendDesc, BSAdditive.GetAddressOf()));

    // alpha weight add
    // Color = SrcAlpha * SrcColor + DestColor
    // Alpha = SrcAlpha
    rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtDesc.DestBlend = D3D11_BLEND_ONE;
    rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
    rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    HR(device->CreateBlendState(&blendDesc, BSAlphaWeightedAdditive.GetAddressOf()));


    // Color Dodge
	// Color = SrcColor + SrcColor*(1.0f- DestColor)
    // Alpha = SrcAlpha
    rtDesc.SrcBlend = D3D11_BLEND_INV_DEST_COLOR;
    rtDesc.DestBlend = D3D11_BLEND_ZERO;
    rtDesc.BlendOp = D3D11_BLEND_OP_ADD;  
    rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    HR(device->CreateBlendState(&blendDesc, BSColorDodge.GetAddressOf()));

    // Multiply Blending (正片叠底)
    // Color = SrcColor * DestColor
    // Alpha = SrcAlpha
    rtDesc.SrcBlend = D3D11_BLEND_DEST_COLOR;  // 目标颜色
    rtDesc.DestBlend = D3D11_BLEND_ZERO;       // 乘以零，相当于单独保留源颜色的乘积
    rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
    rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    HR(device->CreateBlendState(&blendDesc, BSMulti.GetAddressOf()));

    //=========================
    //Init DepthStencilState
	//=========================
    CD3D11_DEPTH_STENCIL_DESC dsDesc(CD3D11_DEFAULT{});


    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = D3D11_COMPARISON_EQUAL;
    HR(device->CreateDepthStencilState(&dsDesc, DSSEqual.GetAddressOf()));

    // LESS_EQUAL TEST
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    HR(device->CreateDepthStencilState(&dsDesc, DSSLessEqual.GetAddressOf()));

    // 反向Z => GREATER_EQUAL TEST
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
    HR(device->CreateDepthStencilState(&dsDesc, DSSGreaterEqual.GetAddressOf()));

    // 进行深度测试，但不写入深度值的状态
    // 若绘制非透明物体时，应使用默认状态
    // 绘制透明物体时，使用该状态可以有效确保混合状态的进行
    // 并且确保较前的非透明物体可以阻挡较后的一切物体
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    dsDesc.StencilEnable = false;
    HR(device->CreateDepthStencilState(&dsDesc, DSSNoDepthWrite.GetAddressOf()));

    // 关闭深度测试的深度/模板状态
    // 若绘制非透明物体，务必严格按照绘制顺序
    // 绘制透明物体则不需要担心绘制顺序
    // 而默认情况下模板测试就是关闭的
    dsDesc.DepthEnable = false;
    HR(device->CreateDepthStencilState(&dsDesc, DSSNoDepthTest.GetAddressOf()));

    // 反向Z深度测试，模板值比较
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
    dsDesc.StencilEnable = true;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
    HR(device->CreateDepthStencilState(&dsDesc, DSSEqualStencil.GetAddressOf()));

    // 无深度测试，仅模板写入
    dsDesc.DepthEnable = false;
    dsDesc.StencilEnable = true;
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    HR(device->CreateDepthStencilState(&dsDesc, DSSWriteStencil.GetAddressOf()));


    //=========================
    // Set Debug Object Name
	//=========================
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    SetDebugObjectName(RSCullClockWise.Get(), "RSCullClockWise");
    SetDebugObjectName(RSNoCull.Get(), "RSNoCull");
    SetDebugObjectName(RSWireframe.Get(), "RSWireframe");
    SetDebugObjectName(RSShadow.Get(), "RSShadow");

    SetDebugObjectName(SSPointClamp.Get(), "SSPointClamp");
    SetDebugObjectName(SSLinearWrap.Get(), "SSLinearWrap");
    SetDebugObjectName(SSLinearClamp.Get(), "SSLinearClamp");
    /*
    SetDebugObjectName(SSAnistropicWrap16x.Get(), "SSAnistropicWrap16x");
    SetDebugObjectName(SSAnistropicClamp2x.Get(), "SSAnistropicClamp2x");
    SetDebugObjectName(SSAnistropicClamp4x.Get(), "SSAnistropicClamp4x");
    SetDebugObjectName(SSAnistropicClamp8x.Get(), "SSAnistropicClamp8x");
    SetDebugObjectName(SSAnistropicClamp16x.Get(), "SSAnistropicClamp16x");
	*/
    SetDebugObjectName(SSShadowPCF.Get(), "SSShadowPCF");

    SetDebugObjectName(BSAlphaToCoverage.Get(), "BSAlphaToCoverage");
    SetDebugObjectName(BSTransparent.Get(), "BSTransparent");
    SetDebugObjectName(BSAdditive.Get(), "BSAdditive");
    SetDebugObjectName(BSColorDodge.Get(), "BSColorDodge");
    SetDebugObjectName(BSMulti.Get(), "BSMulti");

    SetDebugObjectName(DSSEqual.Get(), "DSSEqual");
    SetDebugObjectName(DSSGreaterEqual.Get(), "DSSGreaterEqual");
    SetDebugObjectName(DSSLessEqual.Get(), "DSSLessEqual");
    SetDebugObjectName(DSSNoDepthWrite.Get(), "DSSNoDepthWrite");
    SetDebugObjectName(DSSNoDepthTest.Get(), "DSSNoDepthTest");
    SetDebugObjectName(DSSWriteStencil.Get(), "DSSWriteStencil");
    SetDebugObjectName(DSSEqualStencil.Get(), "DSSEqualStencil");
#endif

}
