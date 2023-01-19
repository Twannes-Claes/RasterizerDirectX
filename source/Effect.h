#pragma once

namespace dae
{

	class Texture;

	class Effect final
	{
	public:

		enum class SamplerStates
		{
			point,
			linear,
			anisotropic
		};

		Effect(ID3D11Device* pDevice, const std::wstring& assetFile);
		~Effect();

		Effect(const Effect&) = delete;
		Effect(Effect&&) noexcept = delete;
		Effect& operator=(const Effect&) = delete;
		Effect& operator=(Effect&&) noexcept = delete;

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

		ID3DX11Effect* GetEffect() const
		{
			return m_pEffect;
		}
		ID3DX11EffectTechnique* GetTechnique() const
		{
			return m_pTechnique;
		}

		void SetProjectionMatrix(const Matrix& matrix);

		void SetDiffuseMap(const Texture* pTexture);

		void SetNormalMap(const Texture* pTexture);

		void SetSpecularMap(const Texture* pTexture);

		void SetGlossinessMap(const Texture* pTexture);

		void SetWorldMatrix(const Matrix& worldMatrix);

		void SetInvViewMatrix(const Matrix& invViewMatrix);

		void ToggleSamplerState(ID3D11Device* pDevice, const bool changeState = true)
		{

			if (changeState)
			{
				m_currentSampleState = static_cast<SamplerStates>((static_cast<int>(m_currentSampleState) + 1) % 3);
			}

			switch (m_currentSampleState)
			{
				case SamplerStates::point:
					{
						std::cout << "POINT\n";
						m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
					}
					break;
				case SamplerStates::linear:
					{
						std::cout << "LINEAR\n";
						m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
					}
					break;
				case SamplerStates::anisotropic:
					{
						std::cout << "ANISOTROPIC\n";
						m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
					}
					break;
				default:
					break;
			}

			if (m_pSamplerState) m_pSamplerState->Release();

			HRESULT result{ pDevice->CreateSamplerState(&m_SamplerDesc, &m_pSamplerState) };

			if (FAILED(result)) return;

			m_pEffectSamplerVariable->SetSampler(0, m_pSamplerState);
		}

	private:

		SamplerStates m_currentSampleState{ SamplerStates::point };

		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};

		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectMatrixVariable* m_pMatWorldMatrixVariable{};
		ID3DX11EffectMatrixVariable* m_pMatInverseViewMatrixVariable{};

		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pGlossinessMapVariable{};


		ID3DX11EffectSamplerVariable* m_pEffectSamplerVariable{};
		ID3D11SamplerState* m_pSamplerState{};
		D3D11_SAMPLER_DESC m_SamplerDesc{};

	};

}
