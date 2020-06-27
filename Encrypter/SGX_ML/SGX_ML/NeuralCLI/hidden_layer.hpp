#ifndef _HIDDEN_LAYER_HPP
#define _HIDDEN_LAYER_HPP
#include "cross-platform.hpp"

#include "layer.hpp"

class HiddenLayer : public Layer 
{
  
  public:
    HiddenLayer(int prev, int curr) : Layer(prev, curr){}
    void feedForward(Layer prev);
    void backProp(Layer next);
    void updateWeights(double, Layer*);
};
#endif
