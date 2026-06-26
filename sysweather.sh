#!/bin/bash
# ============================================
# script with ASCII art
# weather arts are modified versions of classic ASCII art
# ============================================

is_work=false
CONFIG="$HOME/.weather_city"

ask_city(){
    echo "Is this your first time here?"
    echo "Please, enter your city"
    read city
    echo "$city" > "$CONFIG"
    echo "City saved!"
}
check_config()
{
  if [[ ! -f "$CONFIG" ]] || [[ ! -s "$CONFIG" ]]; then
	  ask_city
  else
          city=$(cat "$CONFIG")

	  if [[ -z "$city" ]]; then
		  ask_city
	fi
fi

}

check_pc()
{
echo " "
echo "System info"
echo "╔════════════════════════════════════════════════════════════════════════╗"
echo "  Hostname: $HOSTNAME"
echo "   User:  $USER"
echo "   Kernel: $(uname -r)" 
echo "   OS: $(cat /etc/os-release | grep PRETTY_NAME | cut -d '"' -f2)"
echo "   Uptime: $(uptime -p)"
echo "   Processor model: $(cat /proc/cpuinfo | grep "model name" | head -1 | cut -d ':' -f2 | xargs)"
echo "   IP: $(curl -s ipinfo.io/ip)"
echo "╚════════════════════════════════════════════════════════════════════════╝"
echo " " 
}


rain() {
cat << 'EOF'
      __   _
    _{  }_{ }_
   {_   _    _) it's rainy outside
  / /{ } {__}
 / / / / / /
/ / / / / /
EOF
}
echo " "
sun() {
cat << 'EOF'
        .
      \ | /
    '-.:::.-'
   -==::::::==- it's sunny outside
    .-':::'-.      (or clear)
      / | \
       '
EOF
echo " "
}

snow() {
cat << 'EOF'
     .\\/.
   _\\_)(_/_       
     )( \\        .\\/.
     '/\\'      _\\_\\/_/_  it's snowing outside
                 / /\\ \\
                   /\\'
EOF
echo " "
}

cloud() {
cat << 'EOF'
   __   _
 _{  }_{ }_  
{_   _    _}  it's cloudy outside
  {_} {__}
EOF
echo " "
}
storm() {
cat << 'EOF'
--_-_-_-_---
    _-_-_
    -_-_-
     -__-
    _-_
   _
   -_
    _-
EOF
echo "There's a thunderstorm outside"
echo " "
}
check_condition()
{
if [[ "$condition" == *"Rain"* ]] || [[ "$condition" == *"Drizzle"* ]] || [[ "$condition" == *"Shower"* ]]; then
    rain
elif [[ "$condition" == *"Snow"* ]] || [[ "$condition" == *"Blizzard"* ]] || [[ "$condition" == *"Sleet"* ]]; then
    snow
elif [[ "$condition" == *"Thunder"* ]]; then
    storm
elif [[ "$condition" == *"Clear"* ]] || [[ "$condition" == *"Sunny"* ]]; then
    sun
elif [[ "$condition" == *"Cloudy"* ]] || [[ "$condition" == *"Overcast"* ]] || [[ "$condition" == *"cloud"* ]]; then
    cloud
elif [[ "$condition" == *"Mist"* ]] || [[ "$condition" == *"Fog"* ]]; then
    cloud
elif [[ "$condition" == *"Hail"* ]]; then
    cloud
elif [[ "$condition" == *"Freezing"* ]]; then
    snow
else
    cloud  
fi

}

check_config

condition=$(curl -s "wttr.in/$city?format=%C")


clear
# banner
cowsay hello, $USER

echo " "
echo "Weather"
echo "╔═══════════════════════════════════════╗"
check_condition
echo "╚═══════════════════════════════════════╝"

check_pc
echo "Calendar"
echo "╔══════════════════════════╗"
cal -1 | sed "s/ $(date +%d) / [$(date +%d)] /"
echo "╚══════════════════════════╝"
echo ""

while [ "$is_work" = "false" ]; do
    weather=$(curl -s "wttr.in/$city?format=%t")
    echo -ne "\rCurrent time: $(date) | Weather in $city: $(curl -s "wttr.in/$city?format=%t")"
    sleep 1

   read -t 1 -n 1 key
if [[ "$key" == "q" ]]; then
    echo ""
    break

fi
done

echo ""
