1.编译完，make install后在这个目录测试
2. 当前目录有一个音频文件  test.pcm 与一个脚本test.sh
3. ./test.sh param1  param2
   至少传一个参数，第一个参数param1大于0表示测试内存泄漏。在当前目录会生成一个log_men.txt,
   在这个txt文件可以查到是否内存泄露。要安装工具valgrind(ubuntu 下  sudo apt-get install valgrind)。等于0为普通测试
第二个参数param,是待测试音频，默认是当前路径的test.pcm


1. 关于录音频，当前目录的record是linux版本。如果要编译其他版本，打开录音的宏，
   build/record/record二进制文件就是的
2. 二进制文件使用  ./record param1  param2
   param1  是当前设备录出来音频的通道数  param2是你需要的通道数，比如。
   ./record 2 4  表示当前设备录出来2通道数据，但是我需要补两路0，变成4通道

