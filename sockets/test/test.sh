#!/bin/sh
set -euo pipefail
trap 'kill 0' INT

OPT_GDB=
OPT_SINGLE=

parse_args() {
    local tmp
    tmp=$(getopt -o '' --long gdb:,single -n "$(basename "$0")" -- "$@")
    [ "$?" == 0 ] || exit 1
    eval set -- "$tmp"
    while :; do
        case "$1" in
            --gdb) OPT_GDB="$2"; shift 2;;
            --single) OPT_SINGLE=yes; shift;;
            --) shift; break;;
        esac
    done
    case "$OPT_GDB" in
        ''|client|server)
            if [ "$OPT_SINGLE" -a "$OPT_GDB" == server ]; then
                printf >&2 'Can'\''t gdb automated server.\n'
                exit 1
            fi
            ;;
        *)
            printf >&2 '--gdb: client|server\n'
            exit 1
            ;;
    esac
}

wait_for_server() {
    while ! lsof -i :8000; do
        sleep 1
    done > /dev/null
}

run_in_gdb() {
    local prog
    prog=$1
    shift
    gdb -ex "set args $*" "$prog"
}

parse_args "$@"
cd "$(dirname "$0")"/..
if [ "$OPT_SINGLE" ]; then
    make
    make -C test/single CFLAGS=-g single
    exec test/single/single &
    wait_for_server
    [ "$OPT_GDB" ] \
        && run_in_gdb ./sockets bob 127.0.0.1 \
        || ./sockets bob 127.0.0.1
else
    make
    if [ "$OPT_GDB" == client ]; then
        exec ./sockets billy &
        run_in_gdb ./sockets bob 127.0.0.1
    elif [ "$OPT_GDB" == server ]; then
        (wait_for_server && exec ./sockets bob 127.0.0.1) &
        run_in_gdb ./sockets billy
    else
        exec ./sockets billy &
        (wait_for_server && exec ./sockets bob 127.0.0.1) &
        wait
    fi
fi
kill 0
