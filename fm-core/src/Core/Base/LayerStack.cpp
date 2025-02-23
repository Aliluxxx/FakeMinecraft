#include "fmpch.h"

#include "LayerStack.h"

namespace fm {

	LayerStack::LayerStack() {}

	LayerStack::~LayerStack() {

		Clear();
	}

	void LayerStack::PushLayer(Ref<Layer> layer) {

		const auto& iterator = m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Ref<Layer> overlay) {

		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Ref<Layer> layer) {

		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end()) {

			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Ref<Layer> overlay) {

		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}

	void LayerStack::Clear() {

		for (Ref<Layer> layer : m_Layers)
			layer->OnDetach();

		m_Layers.clear();

		m_LayerInsertIndex = 0;
	}
}
