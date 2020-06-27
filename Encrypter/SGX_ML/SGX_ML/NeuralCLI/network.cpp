

#include "network.hpp"
#include "layer.hpp"
#include "data_handler.hpp"
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <process.h>

const char model_fn[100] = "/MasterThesis/Encrypter/Encrypter/Encrypter/ML_Result/model-neural-network.dat";

std::ofstream myfile;


Network::Network(std::vector<int> hiddenLayerSpec, int inputSize, int numClasses)
{
  inputLayer = new InputLayer(0, inputSize);
  for(int i = 0; i < hiddenLayerSpec.size(); ++i)
  {
    if(i == 0)
      hiddenLayers.push_back(new HiddenLayer(inputLayer->getSize(), hiddenLayerSpec.at(i)));
    else
      hiddenLayers.push_back(new HiddenLayer(hiddenLayers.at(i-1)->getSize(), hiddenLayerSpec.at(i)));
  }
  outputLayer = new OutputLayer(hiddenLayers.at(hiddenLayers.size() - 1)->getSize(), numClasses);
  this->eta = 0.01;
}

Network::~Network()
{
  delete inputLayer;
  delete outputLayer;
  for(HiddenLayer * h : hiddenLayers)
    delete h;
}

void Network::fprop(data *data)
{
  inputLayer->setLayerOutputs(data);
  for(int i = 0; i < hiddenLayers.size(); i++)
  {
    if(i == 0)
      hiddenLayers.at(i)->feedForward(*inputLayer);
    else
      hiddenLayers.at(i)->feedForward(*hiddenLayers.at(i-1));
  }
  outputLayer->feedForward(*hiddenLayers.at(hiddenLayers.size()-1));
  //outputLayer->print();
}

void Network::bprop(data *data)
{
  outputLayer->backProp(data);
  for(int i = hiddenLayers.size()-1; i >= 0; i--)
  {
    if(i == hiddenLayers.size()-1)
      hiddenLayers.at(i)->backProp(*outputLayer);
    else
      hiddenLayers.at(i)->backProp(*hiddenLayers.at(i+1));
  }
}

void Network::updateWeights()
{
  for(int i = 0; i < hiddenLayers.size(); i++)
  {
    if(i == 0)
      hiddenLayers.at(i)->updateWeights(eta, inputLayer);
    else
      hiddenLayers.at(i)->updateWeights(eta, hiddenLayers.at(i-1));
  }
  outputLayer->updateWeights(eta, hiddenLayers.at(hiddenLayers.size()-1));
}

double Network::train()
{
  double numCorrect = 0.0;
  for(data *data : *this->training_data)
  {
    fprop(data);
    bprop(data);
    updateWeights();
    std::vector<double> outputs = outputLayer->getLayerOutputs();
    double maxValue = outputs.at(0);
    int maxIndex = 0;
    for(int i = 1; i < outputs.size(); i++)
    {
      if(maxValue < outputs.at(i))
      {
        maxIndex = i;
        maxValue = outputs.at(i);
      }
    }
    for(int i = 0; i < data->getClassVector().size(); i++)
    {
      if(data->getClassVector().at(i) == 1)
      {
        if(i == maxIndex) numCorrect++;
        break;
      }
    }
  }
  //fprintf(stderr, "Current Performance: %.4f\n", numCorrect / (double)this->training_data->size());
  return (numCorrect / (double)this->training_data->size());
}

double Network::test()
{
  double numCorrect = 0.0;
  for(data *data : *this->test_data)
  {
    fprop(data);
    std::vector<double> outputs = outputLayer->getLayerOutputs();
    double maxValue = outputs.at(0);
    int maxIndex = 0;
    for(int i = 1; i < outputs.size(); i++)
    {
      if(maxValue < outputs.at(i))
      {
        maxIndex = i;
        maxValue = outputs.at(i);
      }
    }
    for(int i = 0; i < data->getClassVector().size(); i++)
    {
      if(data->getClassVector().at(i) == 1)
      {
        if(i == maxIndex) numCorrect++;
        break;
      }
    }
  }
  fprintf(stderr, "Tested Performance: %.4f\n", numCorrect / (double)this->test_data->size());
  return (numCorrect / (double)this->test_data->size());
}

void Network::validate()
{
  
}


int main(int argc, char *argv[])
{//Get number of files from argc
	//get file names from argv
	
	myfile.open(model_fn);

  data_handler *dh = new data_handler();
//#ifdef MNIST
 /* dh->read_input_data("mnist_data/300-train");
  dh->read_label_data("mnist_data/300-labels");
  dh->normalize();*/
//#else 
  std::string  dir("/MasterThesis/Encrypter/Encrypter/Encrypter/.tmp/.decrypted/");
  std::cout << argc << "\n";
  std::cout << argv[0] <<"\n";
  std::cout << argv[1];
  for (int i = 1; i <= argc-1; i++)
  {
	  std::string path;
	  path = dir + argv[i];
	  dh->read_csv(path, ",");
	  std::cout << path << "\n";

  }
  //dh->read_csv("mnist_data/iris2.data", ",");
//#endif
  dh->count_classes();

  dh->split_data();

  std::vector<int> hiddenLayers = {50};
  Network *net = new Network(hiddenLayers, dh->get_test_data()->at(0)->get_normalized_feature_vector()->size(), dh->get_class_counts());
  net->set_training_data(dh->get_training_data());
  net->set_test_data(dh->get_test_data());
  net->set_validation_data(dh->get_validation_data());
  double a = 0.0;
  double testScore = 0.0;
  printf("%s\n", "Training in progress.....");
  printf("%s\n", "This prompt will close as soon as training is over. ");
  printf("%s", "Train and Test results can be accessed at: ");
  printf("%s", model_fn);

  for (int i = 0; i < 1000; i++)
  {
	 //printf("%d\n", i);
	// myfile << i  <<"\n";
	 double b = 0.0;
	 b =  net->train();
	 if (a < b)
	 {
		 a = b;
		 myfile << "Train Performance = " << a << "\n";
	 }
  }
	 
  testScore = net->test();
  myfile << "Test Performance = " << testScore;
  myfile.close();
  return 1;
}
