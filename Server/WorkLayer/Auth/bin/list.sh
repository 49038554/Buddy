listType="run"
if [ $# = 1 ]; then
        listType=$1
fi

pid=$(pidof Auth)
if [ -n "$pid"  ]; then
        if [ "run" = $listType ]; then
                echo "Auth($pid) is running"
        fi
else
        if [ "stop" = $listType ]; then
                echo "Auth is not run"
        fi
fi