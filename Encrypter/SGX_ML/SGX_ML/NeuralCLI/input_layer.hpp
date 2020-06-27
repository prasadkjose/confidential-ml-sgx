#ifndef INPUT_LAYER_HPP
#define INPUT_LAYER_HPP
#include "cross-platform.hpp"

#include "layer.hpp"
#include "data.hpp"
class InputLayer : public Layer {
  
  public:
    InputLayer(int prev, int curr) : Layer(prev, curr){}
    void setLayerOutputs(data *d);
};
#endif
