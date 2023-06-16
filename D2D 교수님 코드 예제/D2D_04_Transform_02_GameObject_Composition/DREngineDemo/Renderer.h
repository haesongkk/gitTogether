#pragma once
#include <string>
#include "IComponentEventFunc.h"

namespace Traditional
{
	using namespace std;

	/// 모든 랜더러의 상위
	/// 
	/// [2021.01.31 LeHideHome]
	class Renderer : public IComponentEventFunc
	{
	public:
		Renderer();
		virtual ~Renderer();


		/// IComponentEventFunc Override
	public:
		virtual void Update() override;

		virtual void OnPreRender() override;
		virtual void OnRender() override;
		virtual void OnPostRender() override;


	protected:
		string m_Name;
	};
}

