#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <termios.h>
#include <fcntl.h>
#include <curl/curl.h>

#define CONFIG_FILE "/.weather_city"
#define MAX_CITY 128

char city[MAX_CITY];
volatile int running = 1;

struct memory 
{
    char *response;
    size_t size;
};

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) 
{
    size_t real_size = size * nmemb;
    struct memory *mem = (struct memory *)userp;

    char *ptr = realloc(mem->response, mem->size + real_size + 1);
    if (!ptr) return 0;

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, real_size);
    mem->size += real_size;
    mem->response[mem->size] = '\0';

    return real_size;
}

char* curl_get(const char *url) 
{
    CURL *curl = curl_easy_init();
    if (!curl) return NULL;

    struct memory chunk = {0};
    chunk.response = malloc(1);
    chunk.size = 0;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) 
{
        free(chunk.response);
        return strdup("Unknown");
    }

    return chunk.response;
}
char* get_condition(const char *city) 
{
    char url[256];
    snprintf(url, sizeof(url), "wttr.in/%s?format=%%C", city);
    return curl_get(url);
}

char* get_temp(const char *city) 
{
    char url[256];
    snprintf(url, sizeof(url), "wttr.in/%s?format=%%t", city);
    return curl_get(url);
}
void print_rain() 
{
    printf("      __   _\n");
    printf("    _{  }_{ }_\n");
    printf("   {_   _    _) it's rainy outside\n");
    printf("  / /{ } {__}\n");
    printf(" / / / / / /\n");
    printf("/ / / / / /\n");
}

void print_sun() 
{
    printf("        .\n");
    printf("      \\ | /\n");
    printf("    '-.:::.-'\n");
    printf("   -==::::::==- it's sunny outside\n");
    printf("    .-':::'-.      (or clear)\n");
    printf("      / | \\\n");
    printf("       '\n");
}

void print_snow() 
{
    printf("     .\\\\/.\n");
    printf("   _\\\\_)(_/_\n");
    printf("     )( \\\\        .\\\\/.\n");
    printf("     '/\\\\'      _\\\\_\\\\/_/_  it's snowing outside\n");
    printf("                 / /\\\\ \\\\\n");
    printf("                   /\\\\'\n");
}

void print_cloud() 
{
    printf("   __   _\n");
    printf(" _{  }_{ }_\n");
    printf("{_   _    _}  it's cloudy outside\n");
    printf("  {_} {__}\n");
}

void print_storm() 
{
    printf("--_-_-_-_---\n");
    printf("    _-_-_\n");
    printf("    -_-_-\n");
    printf("     -__-\n");
    printf("    _-_\n");
    printf("   _\n");
    printf("   -_\n");
    printf("    _-\n");
    printf("There's a thunderstorm outside\n");
}

void check_condition(const char *condition) 
{
    if (strstr(condition, "Rain") || strstr(condition, "Drizzle") ||
        strstr(condition, "Shower")) 
{
        print_rain();
    } else if (strstr(condition, "Snow") || strstr(condition, "Blizzard") ||
               strstr(condition, "Sleet")) 
{
        print_snow();
    } else if (strstr(condition, "Thunder")) 
{
        print_storm();
    } else if (strstr(condition, "Clear") || strstr(condition, "Sunny")) 
{
        print_sun();
    } else if (strstr(condition, "Cloud") || strstr(condition, "Overcast")) 
{
        print_cloud();
    } else if (strstr(condition, "Mist") || strstr(condition, "Fog")) 
{
        print_cloud();
    } else if (strstr(condition, "Hail")) 
{
        print_cloud();
    } else if (strstr(condition, "Freezing")) 
{
        print_snow();
    } else {
        print_cloud();
    }
}

void print_system_info()
{
    struct utsname buf;
    uname(&buf);
    
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    
    // IP через curl
    char *ip = curl_get("ipinfo.io/ip");
    
    printf("\nSystem info\n");
    printf("╔══════════════════════════════════════════╗\n");
    printf("  Hostname: %s\n", hostname);
    printf("  User: %s\n", getenv("USER"));
    printf("  Kernel: %s\n", buf.release);
    printf("  OS: %s\n", buf.sysname);
    printf("  IP: %s\n", ip ? ip : "N/A");
    printf("╚══════════════════════════════════════════╝\n\n");
    
    free(ip);
}
void set_nonblocking_mode() 
{
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_lflag &= ~ECHO;
    ttystate.c_cc[VMIN] = 0;
    ttystate.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}
void restore_terminal() 
{
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag |= ICANON;
    ttystate.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}
void ask_city() 
{
    printf("Is this your first time here?\n");
    printf("Please, enter your city: ");
    fgets(city, MAX_CITY, stdin);
    city[strcspn(city, "\n")] = '\0';
    char config_path[512];
    snprintf(config_path, sizeof(config_path), "%s%s", getenv("HOME"), CONFIG_FILE);
    FILE *f = fopen(config_path, "w");
    if (f) 
{
        fprintf(f, "%s", city);
        fclose(f);
        printf("City saved!\n");
    }
}
void check_config() 
{
    char config_path[512];
    snprintf(config_path, sizeof(config_path), "%s%s", getenv("HOME"), CONFIG_FILE);
    FILE *f = fopen(config_path, "r");
    if (!f) {
        ask_city();
        return;
    }
    if (fgets(city, MAX_CITY, f) == NULL || city[0] == '\n' || city[0] == '\0') 
{
        fclose(f);
        ask_city();
        return;
    }
    city[strcspn(city, "\n")] = '\0';
    fclose(f);
}

int main() 
{
    curl_global_init(CURL_GLOBAL_ALL);
    check_config();
    printf("\033[2J\033[H");

    printf(" _____________________\n");
    printf("< hello, %s >\n", getenv("USER"));
    printf(" ---------------------\n");
    printf("        \\   ^__^\n");
    printf("         \\  (oo)\\_______\n");
    printf("            (__)\\       )\\/\\\n");
    printf("                ||----w |\n");
    printf("                ||     ||\n\n");

    char *condition = get_condition(city);
    printf("Weather\n");
    printf("╔═══════════════════════════════════════╗\n");
    check_condition(condition);
    printf("╚═══════════════════════════════════════╝\n");
    free(condition);
    print_system_info();
    set_nonblocking_mode();

    while (running) 
{
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

        char *temp = get_temp(city);
        printf("\rCurrent time: %s | Weather in %s: %s", time_str, city, temp);
        fflush(stdout);
        free(temp);

        char ch;
        if (read(STDIN_FILENO, &ch, 1) > 0 && ch == 'q') {
            running = 0;
        }

        sleep(1);
    }

    restore_terminal();
    curl_global_cleanup();
    printf("\n");
    return 0;
}
