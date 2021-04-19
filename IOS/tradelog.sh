#!/bin/sh

# IOS projekt 1 - 2020/21
# xkromp00

POSIXLY_CORRECT=yes

# DATE_AFTER="2021-07-29 22:26:16"
# DATE_BEFORE="2021-07-29 23:00:00"
TICKER=""

NEWLINE=$'\n'

if [ "$1" = "-h" ] || [ "$1" = "--help" ];
then
    echo "Usage: tradelog [-h|--help] [FILTER] [COMMAND] [LOG [LOG2 [...]]]"
    exit 0
fi

while getopts "a:b:t:w:" o; do
    case "${o}" in
        a)
            DATE_AFTER=${OPTARG}
            ;;
        b)
            DATE_BEFORE=${OPTARG}
            ;;
        t)
            TICKER="$TICKER ${OPTARG}"
            ;;
        w)
            if [ -n "$WIDTH" ]; then
                >&2 echo "Duplicate -w option not allowed"
                exit 1
            fi

            WIDTH=${OPTARG}
            ;;
        *)
            exit 1
            ;;
    esac
done
shift $((OPTIND-1))

pos() {
    echo "$filtered" | awk -F ';' '{printf "%s : %s : %s : %s\n", $2, $3, $4, $6}' | sort -t ":" -k1,1 -s | \
    awk -F  ' : ' -v CONVFMT=%.30g '{
        if (NR == 1) {
            ticker = $1;
            maximum = 0;
            if ($2 == "buy")
                volume = $4;
            else
                volume = -$4; price = $3;
        } else {
            if (ticker == $1) {
                price = $3;

                if ($2 == "buy") 
                    volume = volume + $4
                else
                    volume = volume - $4
            } 
            else {
                printf "%-10s:%12.2f\n", ticker, volume * price;
                ticker = $1;
                price = $3;

                if ($2 == "buy")
                    volume = $4;
                else
                    volume = -$4
            } 
        }
    } 
    END {
        printf "%-10s:%12.2f\n", ticker, volume * price;
    }' | sort -t ":" -k2,2 -n | tac
}

case " list-tick pos last-price hist-ord graph-pos profit " in
    *" $1 "*)
        IS_COMMAND="YES"
    ;;
esac

if [ -n "$1" ] && [ "$IS_COMMAND" = "YES" ]; then
    COMMAND="$1"
    shift
else
    COMMAND=none
fi

if [ -n "$1" ]; then
    while [ -n "$1" ]
    do
        IS_GZIP="NO"

        case "$1" in
            *".gz")
                IS_GZIP="YES"
            ;;
        esac

        if [ $IS_GZIP = "YES" ]; then
            if [ -z "$filtered" ]; then 
            filtered=$(zcat "$1")
            else 
            filtered=${filtered}${NEWLINE}$(zcat "$1")
            fi
        else
            if [ -z "$filtered" ]; then 
            filtered=$(awk -F ";" '{ printf "%s\n", $0 }' "$1")
            else 
            filtered=${filtered}${NEWLINE}$(awk -F ";" '{ printf "%s\n", $0 }' "$1")
            fi
        fi
        shift
    done
else
    filtered=$(awk -F ";" '{ printf "%s\n",$0 }')
fi

if [ -n "$DATE_AFTER" ]; then 
    filtered=$(echo "$filtered" | awk -F ';' -v AFTER="$DATE_AFTER" '{ if ($1 > AFTER) printf "%s\n", $0 }')
fi
if [ -n "$DATE_BEFORE" ]; then 
    filtered=$(echo "$filtered" | awk -F ';' -v BEFORE="$DATE_BEFORE" '{ if ($1 < BEFORE) printf "%s\n", $0 }')
fi
if [ -n "$TICKER" ]; then 
    filtered=$(echo "$filtered" | awk -F ';' -v TICKER=" $TICKER " '{ if (TICKER ~ " " $2 " ") printf "%s\n", $0 }')
fi

if [ "$COMMAND" = "list-tick" ];
then
    echo "$filtered" | awk -F ';' '{ print $2 }' | sort | uniq
     
elif [ "$COMMAND" = "profit" ];
then
    echo "$filtered" | awk -v CONVFMT=%.30g -F ';' '{if ($3 == "sell") {vysledny_profit = vysledny_profit + $4 * $6} else {vysledny_profit = vysledny_profit - $4 * $6} } END {printf "%.2f\n", vysledny_profit}'

elif [ "$COMMAND" = "pos" ];
then
    max_digits=$(pos | awk -F ":" '{ if ($2 > MAX) {MAX = $2; MINUS = 0} else if (-$2 > MAX / 10) {MAX = -$2; MINUS = 1}} END { if (MINUS == 0) {printf "%.2f", MAX} else printf "%.2f", MAX * 10 }' | wc -c)
    pos | awk -v max_digits=$max_digits -F ":" '{ printf "%-10s:%*.2f\n", $1, max_digits + 1, $2 }'

elif [ "$COMMAND" = "last-price" ];
then    
    echo "$filtered" | awk -F ';' '{printf "%-10s:%8s\n", $2, $4} ' | tac | sort -t ":" -k1,1 -s -u

elif [ "$COMMAND" = "hist-ord" ];
then
    MAX=$(echo "$filtered" | awk -F ';' '{ print $2 }' | sort | uniq -c | sort -n | tac | head -n1 | awk '{print $1;}')
    
    if [ -z "$WIDTH" ]; then
        WIDTH=$MAX
    fi

    echo "$filtered" | awk -F ';' '{ print $2 }' | sort | uniq -c | awk -v MAX="$MAX" -v WIDTH="$WIDTH"  -F " " '{ printf "%-10s: ", $2; count = $1 * WIDTH / MAX; for (i = 1; i <= count; i++) { printf "#" ;} printf "\n" }'

elif [ "$COMMAND" = "graph-pos" ];
then
    positions=$(pos)
    ABS_MAX=$(echo "$positions" | awk -v CONVFMT=%.17g -F ":" 'BEGIN { ABS_MAX = 0 } { if ( $2 > 0 && $2 > ABS_MAX) ABS_MAX = $2; if ($2 < 0 && -$2 > ABS_MAX) ABS_MAX = -$2; } END { printf "%.2f", ABS_MAX }')

    if [ -z "$WIDTH" ]; then
        WIDTH=$(echo "$ABS_MAX" | awk '{ a = $1 / 1000.0; print a }')
    fi

    echo "$positions" | sort | awk -v CONVFMT=%.30g -F ":" -v ABS_MAX="$ABS_MAX" -v WIDTH="$WIDTH" '{ printf "%-10s:", $1; if ($2 >= ABS_MAX / WIDTH) { printf " "; for (i = 1; i <= $2 * WIDTH / ABS_MAX; i++) { printf "#"; }} else if (-ABS_MAX / WIDTH >= $2){ printf " "; for (i = 1; i <= -$2 * WIDTH / ABS_MAX; i++) { printf "!"; } } printf "\n"}'

elif [ "$COMMAND" = "none" ];
then
    echo "$filtered"
fi
