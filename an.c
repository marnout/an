/*  
:w | !gcc -std=gnu23 % -o %< -Wall
!./%< 2026


(c) 2026 Mourad Arnout marnout à free.fr
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>
#include <unistd.h>

#define SEC_DAY 86400
// isnit(s) return true if all characters in s are digits
bool isint(char *s);

int main(int argc, char *argv[]) 
{
    // message usage and exit if an uint is not provided
    if(argc != 2 || !isint(argv[1])) {
        puts("Usage : an <unsigned int>"
            "example : an 2027 or an 2035 or an 1948 ...");
        exit(0);
    }
    // file name for the year
    char fname[12];
    sprintf(fname, "%s.html", argv[1]);
    // warn that the file exit and ask wether to overwrite it
    if(access(fname, F_OK) == 0) {
        puts("\nATTENTION!");
        printf("The file %s already exists. Overwrite it anyway [Y/N]? ",
           fname);
        if(toupper(getchar()) != 'Y') {puts("Leaving"); exit(0);}
        else puts("Too late");
    }
    // locale and variables tm, sdate, sdate, year
    setlocale(LC_TIME, "");
    static struct tm tm;
    char sdate[64];
    int year = atoi(argv[1]);
    // initialisations tm, sdate
    tm.tm_year = year - 1900;
    tm.tm_mday = 1; tm.tm_mon = 0; 
    //strftime(sdate, 64, "%A %d %B %Y\n", &tm);
    // t1 : first january of the year
    time_t t1 = mktime(&tm);
    // t2 : last day of the year
    tm.tm_mday = 31; tm.tm_mon = 11;
    time_t t2 = mktime(&tm);
    //strftime(sdate, 64, "%A %d %B %Y\n", &tm);
   
    // open the file year.html
    FILE *fp = fopen(fname, "w");
    // header
    fprintf(fp, 
        "<!DOCTYPE html>\n"
        "<html lang='fr' xml:lang='fr' xmlns='http://www.w3.org/1999/xhtml'>\n"
        "<head>\n"
        "   <meta charset=\"utf-8\">\n"
        "   <title>%d</title>\n"
        "</head>\n"
        "<body>\n",
        year
    );
    // do heades h3 and their textareas
    for(time_t t= t1; t <= t2; t += SEC_DAY) {
        tm = *localtime(&t);
        strftime(sdate, 64, "%A %d %B %Y", &tm);
        fprintf(fp, "\t<h3>%s</h3>\n", sdate);
        fprintf(fp, "\t<textarea name='%d' rows='3' cols='75'>", tm.tm_yday);
        fprintf(fp, "</textarea>\n");
    }
    // footer
    fprintf(fp, "</body>\n</html>");  
    // done
    fclose(fp);
    return 0;
}

bool
isint(char *s)
{
    for(char * p = s; *p; p++)
        if(!isdigit(*p)) return false;
    return true;
}
