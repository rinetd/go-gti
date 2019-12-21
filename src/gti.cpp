// extern "C" {
// 	#include "gti_c.h"
// }

#include <unistd.h>
#include <fstream>
#include <cstring>
#include <iostream>
// #include <memory>
// #include <string>
#include "include/GTILib.h"
#include "include/GtiLog.h"

#include "gti.h"

using namespace std;


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

static void dump_buffer(const string &filename, char* buf, int size)
{
    std::ofstream fout(filename, ios::out | ios::binary );
    if(!fout.is_open())
    {
        cout<<"Could not open file: "<< filename << endl; 
        exit(-1);
    }
    fout.write(buf, size);
    fout.close();
}

void GTI_DestoryTensor(tensor_t *tensor){
if (tensor != nullptr)
	{
        if (tensor->buffer != nullptr)
        {
        //    delete[] tensor->buffer;
           tensor->buffer =nullptr;
        }
        
		delete[] tensor;
        tensor =nullptr;
	}
}

void * GTI_NewModel(char * path){
        //Read model data from file
        //  int modelsize=0;
        // unique_ptr<char[]> modelfile = file_to_buffer(path, &modelsize);

        // //Load GTI image classification model
        // GtiModel *model=GtiCreateModelFromBuffer((void*)modelfile.get(), modelsize);
        // //GtiModel *model=GtiCreateModel(argv[1]);
        // if( model == nullptr){
        //     cout<<"Could not create model: "<< path << endl;
        //     exit(-1);
        // }
      GtiModel *model=GtiCreateModel(path);
    //     void * model;
        //  cout<<"Could not open file: "<< "GtiCreateModel" << endl; 
        cout <<"model addr:" <<model<<endl;
       return model;
    // return nullptr;
}
// 读取文件 返回
void * GTI_NewTensor(char * path){
  //Read 224x224 BGR plannar format image data from file
    GtiTensor *tensor = new(GtiTensor);
    int datasize=0;
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
    unique_ptr<char[]> buffer(new char[filesize]);
    fin.read((char *)buffer.get(), filesize);
    fin.close();
 
    tensor->height=1;
    tensor->width=1;
    tensor->depth=datasize;
    tensor->buffer=buffer.get();
    return tensor;
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

//Tensor * GtiEval(model_t model,tensor_t tensor){
tensor_t * GtiEval(void * model,void *  tensor){

    
   tensor_t * t =new (tensor_t);
    GtiTensor *Out=GtiEvaluate((GtiModel *)model,(GtiTensor *)tensor);
    cout << Out<<endl;
    t = (tensor_t *)Out;
    return t;
}

void GtiEvaLog(tensor_t * tensor){
  char label[100];
  const char *p;
    if(tensor==0)
        {
            return ;
        }

        cout<<(char *)tensor->buffer<<endl;
        p = std::strstr((char *)tensor->buffer + 20, (const char *)"label");
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
        cout<<"\nRESULT:" << label <<endl;
}

int run(int argc, char * mfile,char* tfile){

    //   int loops=1;    
    // if(argc<3)
    // {
    //     cout<<"usage: "<<argv[0]<<" GTI_model_file image_file [loops]"<<endl; 
    //     cout<<"   Ex: "<<argv[0]<<" ../Models/2801/gti_gnet3_fc20_2801.model ../Data/Image_lite/bridge_c20.bin"<<endl;
    //     cout<<"       "<<argv[0]<<" ../Models/2803/gti_mnet_fc40_2803.model ../Data/Image_lite/swimming_c40.jpg 10"<<endl;

    //     exit(-1);
    // }
    cout << mfile<<endl;
    cout << tfile<<endl;

//    void * model= GTI_NewModel(mfile);
const char * v= GtiGetSDKVersion();
cout << "version"<<v<<endl;
      GtiModel *model=GtiCreateModel(mfile);
//    void * tensor= GTI_NewTensor(argv[1]);

//    tensor_t * t= GtiEval(model,tensor);
    // GTI_Predict(model,tfile);
//    GtiEvaLog(t);


    int filesize=0;
    unique_ptr<char[]> buf = file_to_buffer(tfile, &filesize);

    GtiTensor *tensor = new(GtiTensor);
    tensor->height=1;
    tensor->width=1;
    tensor->depth=filesize;
    tensor->buffer=buf.get();
    cout << "start Evaluate" <<tensor;
    
    GtiTensor *Out=GtiEvaluate((GtiModel *)model,tensor);
    cout << "prediect finish"<<(char *)Out->buffer;
   return 0 ;

}
