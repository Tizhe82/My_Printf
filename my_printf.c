#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

int my_strlen(const char *str) {
    int length = 0;

    while (str[length] != '\0') {
        length++;
    }

    return length;
}

char *my_strcpy(char *dest, const char *src) {
    size_t i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0'; // Don't forget to add the null terminator
    return dest;
}

char *itoa(long int value, char *result, int base, int upper_case) {
    // check that the base if valid
    if (base < 2 || base > 36) {
        *result = '\0';
        return result;
    }
    char *ptr = result, *ptr1 = result, tmp_char;
    long int tmp_value;
    do {
        tmp_value = value;
        value /= base;
        if (upper_case == 1) *ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[35 + (tmp_value - value * base)];
        else *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void fputs_char(va_list *vl, char *buff, int *j) {
    buff[*j] = (char)va_arg(*vl, int);
    *j+= 1;
}

void fputs_du(va_list *vl, char *buff, int *j) {
    char tmp[65];
    itoa(va_arg(*vl, int), tmp, 10, 0);
    my_strcpy(&buff[*j], tmp);
    *j += my_strlen(tmp);
}

void fputs_o(va_list *vl, char *buff, int *j) {
    char tmp[65];
    itoa(va_arg(*vl, int), tmp, 8, 0);
    my_strcpy(&buff[*j], tmp);
    *j += my_strlen(tmp);
}

void fputs_x(va_list *vl, char *buff, int *j) {
    char tmp[65];
    itoa(va_arg(*vl, int), tmp, 16, 1);
    my_strcpy(&buff[*j], tmp);
    *j += my_strlen(tmp);
}

void fputs_pointer(va_list *vl, char *buff, int *j) {
    char tmp[65];
    const void *ptr = va_arg(*vl, void *);
    itoa((unsigned long int)ptr, tmp, 16, 0);
    char *prefix = "0x";
    my_strcpy(&buff[*j], prefix);
    *j += my_strlen(prefix);
    my_strcpy(&buff[*j], tmp);
    *j += my_strlen(tmp);
}

void fputs_s(va_list *vl, char *buff, int *j) {
    char *s;
    s = va_arg(*vl, char *);
    my_strcpy(&buff[*j], s);
    *j += my_strlen(s);
}

int my_printf(char *str, ...) {
    va_list vl;
    int i = 0, j = 0;
    char buff[100] = {0};
    va_start(vl, str);
    while (str && str[i]) {
        if (str[i] == '%') {
            i++;
            if (str[i] == 'c') fputs_char(&vl, buff, &j);
            if (str[i] == 'o') fputs_o(&vl, buff, &j);
            if (str[i] == 'd' || str[i] == 'u') fputs_du(&vl, buff, &j);
            if (str[i] == 'x') fputs_x(&vl, buff, &j);
            if (str[i] == 's') fputs_s(&vl, buff, &j);
            if (str[i] == 'p') fputs_pointer(&vl, buff, &j);
        }
        else {
            buff[j] = str[i];
            j++;
        }
        i++;
    }
    write(1, buff, j); // Write to stdout
    va_end(vl);
    return j;
}

