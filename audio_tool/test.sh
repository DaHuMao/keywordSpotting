
cp ../build/example/kwstest  ./kws_sdk/bin/kwstest
chmod 777 ./kws_sdk/bin/kwstest
echo "start!"
#export LD_LIBRARY_PATH=./hobotspeechapi/lib/:$LD_LIBRARY_PATH
audio_path=record.pcm 
if [ $# -eq 0 ];then
    echo "you should enter a param"
    exit 
fi
if [ $# -eq 2 ];then
    audio_path=$2
fi

if [ $1 -gt 0 ];then
    valgrind --tool=memcheck --leak-check=yes --show-reachable=yes \
    --run-libc-freeres=yes --log-file=./log.txt  ./hrsctest -i $audio_path -o ./ -cfg ../output/hobotspeechapi/hrsc/ 
else
        ./kws_sdk/bin/kwstest -i $audio_path -o ./asr_pcm/ -cfg ./kws_sdk/kws/
fi
