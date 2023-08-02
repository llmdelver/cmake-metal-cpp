# 使用cmake和metal-cpp在纯c++程序中做计算
## 使用

1. clone代码

   ```shell
   git clone https://github.com/lingxiao-du/cmake-metal-cpp.git
   ```

2. 在根目录执行

   ```shell
   cmake -B build -S . -GXcode   
   cmake --build ./build --config Release
   cmake --install build --config Release
   ```

3. 上面一步执行完之后，如果没有出错，就会在项目根目录下面的install/bin中生成`acl`和`compute.metallib`。执行下面命令可以查看结果

   ```shell
   cd ./install/bin
   ./acl
   ```

3. 输出

   ```shell
   Metal Computation completed in 226.319 ms for array of size 100000000.
   CPU Computation completed in 606.886 ms for array of size 100000000.
   0
   ```

   在我的电脑上大致有三倍的加速。

## 详细介绍

[使用CMake配合Metal在纯C++程序中做计算](https://mp.weixin.qq.com/s/Gwre4Ju5EnHvL1B74TDUmg)

欢迎关注公众号"探知轩"，一起进步。