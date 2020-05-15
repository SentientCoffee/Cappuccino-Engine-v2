#include "CappPCH.h"
#include "Cappuccino/Scenes/LayerStack.h"

using namespace Capp;

LayerStack::LayerStack() {
	_layers.reserve(10);
}

LayerStack::~LayerStack() {
	for(auto layer : _layers) {
		layer->onPop();
		delete layer;
	}

	_layers.clear();
}

void LayerStack::pushLayer(Layer* layer) {
	_layers.emplace(_layers.begin() + _layerInsert, layer);
	++_layerInsert;
}
void LayerStack::pushOverlay(Layer* overlay) {
	_layers.emplace_back(overlay);
}

void LayerStack::popLayer(Layer* layer) {
	const auto it = std::find(_layers.begin(), _layers.end(), layer);
	if(it != _layers.end()) {
		(*it)->onPop();
		_layers.erase(it);
		--_layerInsert;
	}
}

void LayerStack::popOverlay(Layer* overlay) {
	const auto it = std::find(_layers.begin(), _layers.end(), overlay);
	if(it != _layers.end()) {
		(*it)->onPop();
		_layers.erase(it);
	}
}

std::vector<Layer*>::iterator LayerStack::begin()             { return _layers.begin(); }
std::vector<Layer*>::const_iterator LayerStack::begin() const { return _layers.begin(); }
std::vector<Layer*>::iterator LayerStack::end()               { return _layers.end(); }
std::vector<Layer*>::const_iterator LayerStack::end() const   { return _layers.end(); }