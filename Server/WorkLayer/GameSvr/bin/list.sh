listType="run"
if [ $# = 1 ]; then
        listType=$1
fi

pid=$(pidof GameSvr)
if [ -n "$pid"  ]; then
        if [ "run" = $listType ]; then
                echo "GameSvr($pid) is running"
        fi
else
        if [ "stop" = $listType ]; then
                echo "GameSvr is not run"
        fi
fi