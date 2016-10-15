listType="run"
if [ $# = 1 ]; then
        listType=$1
fi

pid=$(pidof DBCenter)
if [ -n "$pid"  ]; then
        if [ "run" = $listType ]; then
                echo "DBCenter($pid) is running"
        fi
else
        if [ "stop" = $listType ]; then
                echo "DBCenter is not run"
        fi
fi