/***********************************************************************
*
* Copyright (c) 2017-2019 Gyrfalcon Technology Inc. All rights reserved.
* See LICENSE file in the project root for full license information.
*
************************************************************************/

#include <fstream>
#include <cstring>
#include "GTILib.h"
#include "GtiLog.h"
#include "time.h"
using namespace std;
// void * GTI_NewTensor(char * buffer, size_t filesize){
//     GtiTensor *tensor = new GtiTensor;
//     tensor->height=tensor->width=1;
//     tensor->depth=filesize;
//     tensor->buffer=buffer;
//     return tensor;
// }
void * GTI_NewTensor(const char * path){
  
    // int datasize=0;
    // unique_ptr<char[]> buffer = file_to_buffer(path, &datasize);

    //Read 224x224 BGR plannar format image data from file
    std::ifstream fin(path, ios::in | ios::binary );

    if(!fin.is_open())
    {
        cout<<"Could not open file: "<< path << endl; 
        exit(-1);
    }

    fin.seekg(0,std::ios::end);
    size_t filesize=fin.tellg();
    fin.seekg(0,std::ios::beg);

    cout << filesize;

    // unique_ptr<char[]> buf(new char[filesize]);
    // fin.read((char *)buf.get(), filesize);

    char *buf=new char[filesize];
    fin.read(buf, filesize);
    fin.close();
    
    GtiTensor *tensor = new(GtiTensor);
    tensor->height=1;
    tensor->width=1;
    tensor->depth=filesize;
    tensor->buffer=buf;

    cout << "brefor tensor" << tensor << endl;
    cout << "brefor buffer" << tensor->buffer << endl;
    return tensor;
}

static unique_ptr<char[]> file_to_buffer(char* filename, int *sizeptr)
{
    std::ifstream fin(filename, ios::in | ios::binary );
    if(!fin.is_open())
    {
        cout<<"Could not open file: "<< filename << endl; 
        exit(-1);
    }

    fin.seekg(0,std::ios::end);
    *sizeptr=fin.tellg();
    fin.seekg(0,std::ios::beg);
    
    unique_ptr<char[]> buffer(new char[*sizeptr]);

    fin.read((char *)buffer.get(), *sizeptr);
    fin.close();
    return move(buffer);
}

void * GTI_Predict(void *model, char * path){
    cout <<"model addr:" <<model<<endl;
    int filesize=0;
    unique_ptr<char[]> buf = file_to_buffer(path, &filesize);

    GtiTensor *tensor = new(GtiTensor);
    tensor->height=1;
    tensor->width=1;
    tensor->depth=filesize;
    tensor->buffer=buf.get();
    cout << "start Evaluate";
    GtiTensor *Out=GtiEvaluate((GtiModel *)model,tensor);
    
    cout << "prediect finish"<<(char *)Out->buffer;
    return Out;
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
    //Read 224x224 BGR plannar format image data from file
    // std::ifstream fin(argv[2], ios::in | ios::binary );
    // if(!fin.is_open())
    // {
    //     cout<<"Could not open file: "<< argv[2] << endl; 
    //     exit(-1);
    // }

    // fin.seekg(0,std::ios::end);
    // size_t filesize=fin.tellg();
    // fin.seekg(0,std::ios::beg);
    // unique_ptr<char[]> buffer(new char[filesize]);
    // fin.read((char *)buffer.get(), filesize);
    // fin.close();

    GtiTensor *tensor=(GtiTensor *)GTI_NewTensor(argv[2]);
    
    //tensor.height=tensor.width=1;
    //tensor.depth=filesize;
    //tensor.buffer=buffer.get();
    // delete tensor;
    // cout  << (char *)tensor->buffer;
    //Load GTI image classification model   
    GtiModel *model=GtiCreateModel(argv[1]);

   GTI_Predict(model,argv[2]);
    
    exit(-1);
    char label[100];
    const char *p;

    for(int j=0;j<loops;j++)
    {
	clock_t start,finish;
        start=clock(); 
        //Get the inference results from chip
        GtiTensor *tensorOut=GtiEvaluate(model,tensor);

    cout << "after tensor" << tensorOut->height << endl;
    cout << "after buffer" << tensorOut->buffer << endl;
    cout << "after size" << tensorOut->size << endl;
        // GtiTensor *tensorOut=(GtiTensor *)GTI_Evalue(model,argv[2]);
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
