# sysweather
Display the weather and system information.
![sysweather in action](image.png)
## Requirements
1.C Compiler


2.`libcurl`


3.Internet connection

## Installation

1. Clone the repository

```sh
git clone https://github.com/Efesint/sysweather
cd sysweather
```

2. Compile the executable

```sh
gcc sysweather.c -o sysweather -lcurl
```

3. Install it

```sh
sudo install -m 755 sysweather /usr/local/bin/sysweather
```

## Enter the city
Write your town like 'Europe/Berlin'.
