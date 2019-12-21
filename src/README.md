
`gcc liteDemo.cpp -I"../include/" -L"../lib/linux/aarch64/" -lGTILibrary -ldl -lm -lstdc++ -o liteDemo`

### CGO 调用C++
cgo提供了一种机制：它能够根据import”C”中引入的头文件，自动找到相应的源文件进行编译链接。这种机制的调用，需要用`go build`命令。 
注意：go build foo.go只会单独编译foo.go文件，而不会自动编译链接相关的C源文件

### .go 文件调用C代码的4种方式：
第一种是将C代码直接嵌入到GO源文件中
第二种是将C代码写在C文件中，再在GO文件中引入
第三种 通过静态库加载
第四种 通过动态库加载
```go
/*
#include <stdio.h>
#include <library.h>
#include <dic32.h>

#cgo CFLAGS: -I/usr/local/include
-I 指定头文件的搜索目录
-L 指定lib库搜索目录,支持绝对、相对路径 -L/usr/local/lib
-l 指定库名称(去掉前面的lib及后面的.so,如libUsbKey.so为-lUsbKey)
 

//静态库方式（后缀名为.a）,编译时库内容会内嵌在最终程序中，执行环境不在需要依赖库(.a)
//#cgo LDFLAGS: ${SRCDIR}/winlin.a -L/home/leen/lib -lUsbKey -lRockey6SmartPlus

//动态库方式(后缀名为.so)，编译时只做链接，不包含在程序中，执行环境lib目录下需要包含同样的依赖库(.so)
#cgo LDFLAGS: -L./ -lUsbKey -lRockey6SmartPlus -Wl,-rpath=./

*/
import "C"   // 切勿换行再写这个必须紧跟在·*/·之后

 
import (
  "fmt"
  "strings"
)
 
func main() {
    //上述c代码中采用动态库方式，静态库已注释，采用静态库方式编译亦可以
  value := C.GoString(C.readUsbKey())
 
  fmt.Println(strings.TrimSpace(value))
}
```

#### I. 使用动态库形式 (当前项目采用的方式)

闭源的动态库 libGTILibrary.so libftd3xx.so 都是C++写的，无法通过CGO直接进行调用。
因此需要对C++的库用C进行一次封装 cgo -> libgti.so -> libGTILibrary.so
1. 我们先封装了一层c语言的动态库源码放在src目录 生成的库文件为 libgti.so
2. 将 libGTILibrary.so libftd3xx.so 和 libgti.so 复制到 lib/目录
3. cgo 中使用动态库编译

//❌ (不需要) 注意的是 我们在C包装函数中使用的库函数，在编译成动态库和在cgo中使用的时候都需要明确指定，
//❌ 否则就会报错 ./lib/libgti.so: undefined reference to `GtiCreateModel'
//❌ 引用的时候只需要引用直接使用的库即可，libGTILibrary.so ->libftd3xx.so  因为 libgti.so没有直接使用libftd3xx.so 所以libftd3xx 就不需要引用了

2. 将C包装函数编译为动态库
`g++ -shared -fPIC -o libgti.so *.cpp`

CGO中的使用也是一样
```go
package main
/*
// 因为libgti的库直接使用到了GTILibrary的库所以在编译的时候都要引用。libftd3xx.so属于间接引用所以不用包含。

#cgo LDFLAGS: -L ${SRCDIR}/lib -lGTILibrary -lgti -ldl -lm -lstdc++
#include "src/gti.h"
*/
import "C"

func main(){
	var argv =[]string{"/sdcard/GTISDK-Linux_aarch64_v4.0.1/Apps/Models/2801/gti_gnet3_fc20_2801.model", "/sdcard/GTISDK-Linux_aarch64_v4.0.1/Apps/Data/Image_lite/bridge_c20.jpg"}
	m:=C.CString(argv[0])
	t:=C.CString(argv[1])
	C.run(C.int(len(argv)),m,t) //main(int argc,char**argv)
}

```

#### II.源码+动态库形式