#!bin/sh
# read route/*.h
bTmp="#include \""
eTmp="\""
route_h="incset.route"

# empty incset.route
echo > ${route_h}

# append route/*.h to incset.route
for file in route/*.h
do
    if test -f $file
    then
        echo ${bTmp}${file}${eTmp} >> ${route_h} 
    fi
    if test -d $file
    then
        echo $file 是目录
    fi
done

# append pkg/*.h to incset.route
for file in pkg/*.h
do
    if test -f $file
    then
        echo ${bTmp}${file}${eTmp} >> ${route_h} 
    fi
    if test -d $file
    then
        echo $file 是目录
    fi
done

# append include/*.h to incset.route
for file in include/*.h
do
    if test -f $file
    then
        echo ${bTmp}${file}${eTmp} >> ${route_h} 
    fi
    if test -d $file
    then
        echo $file 是目录
    fi
done


