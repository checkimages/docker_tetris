#!/bin/bash

rm -rf /brick/src/brick_game

if [ -d "/project/src/brick_game" ]; then
    cp -r "/project/src/brick_game/" "/brick/src/"
fi

#mkdir -p /brick/src/brick_game

find /project/src -name 'tetris_fsm.a' -exec cp {} /brick/src/ \;

cd /brick/src && make tetris_fsm.a 1>/dev/null 2>&1

cd /brick/src && make 1>/dev/null 2>&1
TARGET_BUILD=$?
if [ $TARGET_BUILD -eq 0 ]; then
    Tetris "${TCOLOR}" "${TBRIGHT}"
else
    echo "Application build: FAIL"
fi
