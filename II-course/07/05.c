/*
Submit a solution for up07-5-c/random/passwd-recover-1

Full score: 100
Run penalty:    10
Time limit: 2 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/24 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem up07-5: c/random/passwd-recover-1

Ваша задача — взломать генератор случайных паролей, основанный на использовании псевдослучайных чисел.

Известно, что для генерации паролей используется линейный конгруентный генератор псевдослучайных чисел с параметрами A = 1103515245, C = 12345, M = 231. Символы случайного пароля получаются последовательным получением псевдослучайного числа и его преобразованием в вещественное число на полуинтервале [0;1), а затем в целое число на полуинтервале [0;62), которое преобразовывается в символ цифры, заглавной или строчной латинской буквы в том порядке, в котором они находятся в кодовой таблице ASCII.

Например, если дана затравка 128123812 и требуемая длина пароля — 16 символов, будет получен пароль kD5X7la4Z9LZvD6F. Первое псевдослучайное число будет 1616688397, которое по описанным выше правилам будет преобразовано в букву k и т. д..

На стандартный поток ввода вашей программе подается пароль, в котором часть символов неизвестна. Неизвестные символы обозначаются символом "точка". Длина пароля не превышает 32 символа. Гарантируется, что первый символ всегда известен. Программа должна восстановить пароль полностью и вывести его на стандартный поток вывода. Если пароль невозможно восстановить однозначно, либо такой пароль не может быть сгенерирован описанным выше способом, выведите один символ "решетка".

Examples

Input
a.......
Output
#
Input
aaaaa...
Output
#
Input
aaaab...
Output
aaaabezy
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum 
{
    BUF_SIZE = 34,
    ALPHABET_LENGTH = '9' - '0' + 1 + 'Z' - 'A' + 1 + 'z' - 'a' + 1,
};

enum 
{
    MOD = 0x80000000,
    MY_RAND_MAX = MOD - 1,
    INCREMENT = 12345,
    MULTIPLIER = 1103515245,
};

void form_alphabet(char alphabet[ALPHABET_LENGTH]) {
    int i = 0;
    for (char sym = '0'; sym <= '9'; sym++) {
        alphabet[i++] = sym;
    }
    for (char sym = 'A'; sym <= 'Z'; sym++) {
        alphabet[i++] = sym;
    }
    for (char sym = 'a'; sym <= 'z'; sym++) {
        alphabet[i++] = sym;
    }
}

int gen_next_rand(int previous) {
    __builtin_mul_overflow(MULTIPLIER, previous, &previous);
    __builtin_add_overflow(previous, INCREMENT, &previous);
    return previous % MOD;
}

char rand_to_char(int rand, const char alphabet[ALPHABET_LENGTH]) {
    return alphabet[(int)(rand / (MY_RAND_MAX + 1.) * ALPHABET_LENGTH)];
}

void char_to_rand(char c, int *rand1, int *rand2, const char alphabet[ALPHABET_LENGTH]) {
    int index_in_alphabet = 0;
    if (islower(c)) {
        index_in_alphabet += '9' - '0' + 1 + 'Z' - 'A' + 1;
        index_in_alphabet += c - 'a';
    } else if (isupper(c)) {
        index_in_alphabet += '9' - '0' + 1;
        index_in_alphabet += c - 'A';
    } else {
        index_in_alphabet += c - '0';
    }

    // normalize seed
    int seed = (double) index_in_alphabet / ALPHABET_LENGTH * (MY_RAND_MAX + 1.);
    while (rand_to_char(seed, alphabet) < c) {
        seed++;
    }
    while (rand_to_char(seed, alphabet) > c) {
        seed--;
    }

    int r1 = seed, r2 = seed;
    do {
        r1--;
    } while (rand_to_char(r1, alphabet) == c);
    do {
        r2++;
    } while (rand_to_char(r2, alphabet) == c);

    *rand1 = r1;
    *rand2 = r2;
}

int check_passwd(int seed, const char *passwd_pattern, int passwd_len, 
        const char alphabet[ALPHABET_LENGTH]) {
    for (int i = 0; i < passwd_len; i++) {
        if (passwd_pattern[i] != '.' && rand_to_char(seed, alphabet) != passwd_pattern[i]) {
            return 0;
        }
        seed = gen_next_rand(seed);
    }
    return 1;
}

void gen_passwd_by_seed(int seed, char *passwd, int passwd_len, 
        const char alphabet[ALPHABET_LENGTH]) {
    for (int i = 0; i < passwd_len; i++) {
        passwd[i] = rand_to_char(seed, alphabet);
        seed = gen_next_rand(seed);
    }
    passwd[passwd_len] = '\0';
}

int main(int argc, char const *argv[]) {
    char passwd_pattern[BUF_SIZE] = {};
    if (scanf("%s", passwd_pattern) != 1) {
        return 0;
    }
    int passwd_len = strlen(passwd_pattern);

    char alphabet[ALPHABET_LENGTH];
    form_alphabet(alphabet);

    int seed1, seed2, seed;
    int is_seed_defined = 0;
    char_to_rand(passwd_pattern[0], &seed1, &seed2, alphabet);
    for (int i = seed1; i <= seed2; i++) {
        if (check_passwd(i, passwd_pattern, passwd_len, alphabet)) {
            if (is_seed_defined) {
                is_seed_defined = 0;
                break;
            }
            seed = i;
            is_seed_defined = 1;
        }
    }

    if (!is_seed_defined) {
        printf("#\n");
    } else {
        char passwd[BUF_SIZE] = {};
        gen_passwd_by_seed(seed, passwd, passwd_len, alphabet);
        printf("%s\n", passwd);
    }
    return 0;
}