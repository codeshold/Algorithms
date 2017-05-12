#!/bin/bash

# 程序超时时间，单位秒
TIME_OUT=60

PROGRAM=antSearch
OUTPUTFILE=output.txt


if [ ! -f $PROGRAM ]; then
    echo "编译程序..."
    make clean && make
    [ ! $? -eq 0 ] && echo "编译错误!!!" && exit
    echo 
fi
chmod a+x *.sh $PROGRAM

echo 
echo "======================================================"
read -p "请选择对应的case文件编号（请输入1 或 2 或 3）：" CASENUM
[ ! $CASENUM -eq 1 ] && [ ! $CASENUM -eq 2 ] && [ ! $CASENUM -eq 3 ] && echo "请输入正确的case文件编号！" && exit

echo 
echo "程序运行中......（输入文件为case${CASENUM}.txt，超时时间为${TIME_OUT}秒)"
echo "./$PROGRAM case${CASENUM}.txt ${TIME_OUT}"
./$PROGRAM case${CASENUM}.txt ${TIME_OUT}

[ ! $? -eq 0 ] && echo "程序运行错误(需重新编译)！请重新运行RUNME.sh!" && rm -rf $PROGRAM && exit
echo "程序运行结束，请查看${OUTPUTFILE}文件！"
echo "======================================================"





