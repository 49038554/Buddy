listType="run"
if [ $# = 1 ]; then
        listType=$1
fi

pid=$(pidof SNSSvr)
if [ -n "$pid"  ]; then
        if [ "run" = $listType ]; then
                echo "SNSSvr($pid) is running"
        fi
else
        if [ "stop" = $listType ]; then
                echo "SNSSvr is not run"
        fi
fi