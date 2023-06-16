#pragma once
#include "Component.h"
#include <string>

namespace CBEngine
{
	class Renderer : public Component
	{
	public:
		Renderer(std::string name);
		virtual ~Renderer();


		/// IComponentEventFunc Override
	public:
		virtual void Update() override;

		virtual void OnPreRender() override;
		virtual void OnRender() override;
		virtual void OnPostRender() override;

	};
}

