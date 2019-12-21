package gti

/*
//#cgo CFLAGS: -I${SRCDIR} -DLINUX=1
// 这里gti的库 使用到了GTILibrary的库所以在编译的时候都要引用。
#cgo LDFLAGS: -L ${SRCDIR}/lib -lgti -lGTILibrary -ldl -lm -lstdc++
#include "src/gti.h"
// #include <stdlib.h>
*/
import "C"
import (
	"unsafe"
)

type (
	// GtiDevice C.struct_GtiDevice
	// GtiContext C.struct_GtiContext
	// GtiModel C.struct_GtiModel
	// GtiTensor C.struct_GtiTensor
	Tensor C.tensor_t
 )
//GtiTensor * GTI_GetTensor(char * path)；
func NewTensor(path string) unsafe.Pointer{
	p:=C.CString(path)
	return C.GTI_NewTensor(p)
}
func NewModel(path string ) unsafe.Pointer  {

	//GtiModel *model=GtiCreateModelFromBuffer((void*)modelfile.get(), modelsize);

	p:=C.CString(path)
	// defer C.free(unsafe.Pointer(p))
	model:=C.GTI_NewModel(p)
	return model
}

//GtiTensor * GtiEvaluate(GtiModel *model,GtiTensor * input);
func Evaluate(model unsafe.Pointer,tensor unsafe.Pointer)*Tensor{
	 C.GtiEval(model,tensor)
	return nil
}



func Exec(){
	// CreateModel("aaa")
	// m:=NewModel("/sdcard/GTISDK-Linux_aarch64_v4.0.1/Apps/Models/2801/gti_gnet3_fc20_2801.model")
	// m:=NewTensor("/sdcard/GTISDK-Linux_aarch64_v4.0.1/Apps/Data/Image_lite/bridge_c20.bin")
	// Evaluate(m,t)
	//p:=C.CString("/sdcard/GTISDK-Linux_aarch64_v4.0.1/Apps/Models/2801/gti_gnet3_fc20_2801.model")
	// defer C.free(unsafe.Pointer(p))
	//m:=C.GTI_NewModel(p)
	//str:=C.CString("/sdcard/GTISDK-Linux_aarch64_v4.0.1/Apps/Data/Image_lite/bridge_c20.bin")
	//C.GTI_Predict(m, str)
	//fmt.Println(a)

	// ../Models/2801/gti_gnet3_fc20_2801.model ../Data/Image_lite/bridge_c20.bin
	// var argv =[]string{"./Models/2801/gti_gnet3_fc20_2801.model", "./Data/Image_lite/bridge_c20.bin"}
	var argv =[]string{"/sdcard/GTISDK-Linux_aarch64_v4.0.1/Apps/Models/2801/gti_gnet3_fc20_2801.model", "/sdcard/GTISDK-Linux_aarch64_v4.0.1/Apps/Data/Image_lite/bridge_c20.jpg"}
	m:=C.CString(argv[0])
	t:=C.CString(argv[1])
	C.run(C.int(len(argv)),m,t) //main(int argc,char**argv)
}

// func str2c(){
	//arg := make([](*C.char), 0)  //C语言char*指针创建切片
	//l := len(args)
	//for i,_ := range args{
	//	char := C.CString(args[i])
	//	defer C.free(unsafe.Pointer(char)) //释放内存
	//	strptr := (*_Ctype_char)(unsafe.Pointer(char))
	//	arg = append(arg, strptr)  //将char*指针加入到arg切片
	//}

	//C.test(C.int(l), (**C.char)(unsafe.Pointer(&argv[0])))  //即c语言的main(int argc,char**argv)
// }