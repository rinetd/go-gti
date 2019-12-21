/***********************************************************************
*
* Copyright (c) 2017-2019 Gyrfalcon Technology Inc. All rights reserved.
* See LICENSE file in the project root for full license information.
*
************************************************************************/

#include <fstream>
#include <cstring>
#include "include/GTILib.h"
#include "include/GtiLog.h"
#include "time.h"
using namespace std;
void * GTI_GetTensor(char * file){

  //Read 224x224 BGR plannar format image data from file
    std::ifstream fin(file, ios::in | ios::binary );
    if(!fin.is_open())
    {
        cout<<"Could not open file: "<< file << endl; 
        exit(-1);
    }

    fin.seekg(0,std::ios::end);
    size_t filesize=fin.tellg();
    fin.seekg(0,std::ios::beg);
    unique_ptr<char[]> buffer(new char[filesize]);
    fin.read((char *)buffer.get(), filesize);
    fin.close();

    GtiTensor *tensor = new GtiTensor;
    tensor->height=tensor->width=1;
    tensor->depth=filesize;
    tensor->buffer=buffer.get();
    return tensor;
}
int main(int argc, char**argv)
{
    int loops=1;    
    if(argc<3)
    {
        cout<<"usage: "<<argv[0]<<" GTI_model_file image_file [loops]"<<endl; 
        cout<<"   Ex: "<<argv[0]<<" ../Models/2801/gti_gnet3_fc20_2801.model ../Data/Image_lite/bridge_c20.bin"<<endl;
        cout<<"       "<<argv[0]<<" ../Models/2803/gti_mnet_fc40_2803.model ../Data/Image_lite/swimming_c40.jpg 10"<<endl;

        exit(-1);
    }
    if(argc>=4) 
        loops=atoi(argv[3]);

    // putenv((char *)"GTI_EMMC_DELAY=9000");
  
    GtiTensor *tensor=(GtiTensor *)GTI_GetTensor(argv[2]);
    //tensor.height=tensor.width=1;
    //tensor.depth=filesize;
    //tensor.buffer=buffer.get();
    delete tensor;
    
    //Load GTI image classification model   
    GtiModel *model=GtiCreateModel(argv[1]);

    char label[100];
    const char *p;

    for(int j=0;j<loops;j++)
    {
	clock_t start,finish;
        start=clock(); 
        //Get the inference results from chip
        GtiTensor *tensorOut=GtiEvaluate(model,tensor);
        if(tensorOut==0)
        {
            LOG(ERROR)<<"evaluate error,exit";
            break;
        }

        cout<<(char *)tensorOut->buffer<<endl;
        p = std::strstr((char *)tensorOut->buffer + 20, (const char *)"label");
        p = std::strchr((const char *)p+7, ' ');
        for(int k = 0; k < 100; k++)
        {
            if(*p == '\"')
            {
                label[k] = 0;
                break;
            }
            label[k] = *p++;
        }
      finish=clock(); 
	cout<<"算法执行持续时间："<<double(finish-start)/CLOCKS_PER_SEC<<"毫秒"<<endl;
        cout<<"\n识别结果:" << label <<endl;
    }
    GtiDestroyModel(model);
    return 0;
}
