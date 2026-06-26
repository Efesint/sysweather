## Sysweather
display of weather, time, and system information

~~~
 _____________
< hello, user >
 -------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
 
Weather
╔═══════════════════════════════════════╗
--_-_-_-_---
    _-_-_
    -_-_-
     -__-
    _-_
   _
   -_
    _-
There's a thunderstorm outside
 
╚═══════════════════════════════════════╝
 
System info
╔════════════════════════════════════════════════════════════════════════╗
  Hostname: archlinux
   User:  user
   Kernel: 7.0.13-arch1-1
   OS: Arch Linux
   Uptime: up 7 hours, 43 minutes
   Processor model: 11th Gen Intel(R) Core(TM) i5-11400H @ 2.70GHz
   IP: 192.168.3.7
╚════════════════════════════════════════════════════════════════════════╝
 
Calendar
╔══════════════════════════╗
      June 2026     
Su Mo Tu We Th Fr Sa
    1  2  3  4  5  6
 7  8  9 10 11 12 13
14 15 16 17 18 19 20
21 22 23 24 25 [26] 27
28 29 30            
                    
╚══════════════════════════╝

~~~

## Requirements
1.Cowsay:
~~~
sudo pacman -S cowsay
~~~
or
~~~
sudo apt install cowsay
~~~
or
~~~
sudo xbps-install -S cowsay
~~~
or
~~~
sudo emerge -av cowsay
~~~
2.Internet connection

## Installation
1. Download sysweather.sh
~~~
git clone https://github.com/Efesint/sysweather
~~~
2. Add script executable
~~~
chmod +x sysweather.sh
~~~
3. Run it
```
./sysweather.sh
```

## ⚠️Disclaimer

I don't know Bash well; it's not my primary programming language. 
I'm not a vibe coder, but I had to use AI.
It's about 70/30.
I'm still learning Linux administration and Bash programming. 
This project is a learning exercise, not production-ready software. 
But for casual use, I think it'll be quite usable. This is my second Bash project. 
Thanks for reading.

## cowsay
This script uses cowsay (https://github.com/tnalpgge/rank-amateur-cowsay) 
under GPL license.
