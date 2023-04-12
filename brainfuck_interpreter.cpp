#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

FILE *bfsrc;
char src_name[256];
char code_mem[65536], *code_ptr;
unsigned char data_mem[65536], *data_ptr;
void init_brainfuck();
void read_and_parse_bf_code();
void parse_bf_code();
void print_bf_code();
void print_bf_code_until_code_ptr();
void print_bf_data();

int main(int argc, char *argv[])
{
    if (!argv[1]) {
        cout << "No input file\n" << endl;
        return 1;
    }

    strcpy(src_name, argv[1]);
    bfsrc = fopen(src_name, "r");
    
    if (!bfsrc) {
        cout << "File \'" << src_name << "\' does not exist.\n" << endl;
        return 1;
    }
    
    init_brainfuck();
    read_and_parse_bf_code();
    fclose(bfsrc);
    cout << endl;

    return 0;
}

void init_brainfuck()
{
    for (int i = 0; i < 65536; ++i) {
        data_mem[i] = 0;
        code_mem[i] = 0;
    }

    data_ptr = data_mem;
}

void read_and_parse_bf_code()
{
    int i = 0;
    char c;

    while (1) {
        c = fgetc(bfsrc);
        if (c == EOF) {
            code_mem[i] = 0;
            break;
        }
        switch (c) {
        case '>':
        case '<':
        case ',':
        case '.':
        case '+':
        case '-':
        case '[':
        case ']':
            code_mem[i++] = c;
            break;
        }
    }

    parse_bf_code();
}

void print_bf_code()
{
    for (int i = 0; code_mem[i]; ++i)
        putchar(code_mem[i]);
    
    cout << "\n\n";
}

void parse_bf_code()
{
    code_ptr = code_mem;
    
    while (*code_ptr) {
        switch (*code_ptr) {
        case '>':
            if (++data_ptr - data_mem >= 65536) {
                printf("Brainfuck out of its memory at %ld on symbol %c.\n\n", data_ptr - data_mem, *code_ptr);
                print_bf_code_until_code_ptr();
                print_bf_data();
                return;
            }
            break;
        case '<':
            if (--data_ptr < data_mem) {
                printf("Brainfuck out of its memory at %ld on symbol %c.\n\n", data_ptr - data_mem, *code_ptr);
                print_bf_code_until_code_ptr();
                print_bf_data();
                return;
            }
            break;
        case ',':
            scanf("%c", (char*)data_ptr);
            break;
        case '.':
            putchar((char)*data_ptr);
            break;
        case '+':
            ++*data_ptr;
            break;
        case '-':
            --*data_ptr;
            break;
        case '[':
            if (!*data_ptr) {
                int find = 0, cnt = 0;
                char *walk = code_ptr + 1;
                while (*walk) {
                    if (*walk == ']')
                        if (!cnt) {
                            find = 1;
                            code_ptr = walk;
                            break;
                        } else
                            --cnt;                        
                    if (*walk == '[')
                        ++cnt;
                    ++walk;
                }
                if (!find) {
                    printf("Brainfuck failed due to asymmetric jumping bracket.\n\n");
                    return;
                }
            }
            break;
        case ']':
        {
            int find = 0, cnt = 0;
            char *walk = code_ptr - 1;
            while (walk >= code_mem) {
                if (*walk == '[')
                    if (!cnt) {
                        find = 1;
                        code_ptr = walk - 1;
                        break;
                    } else
                        --cnt;                    
                if (*walk == ']')
                    ++cnt;
                --walk;
            }
            if (!find) {
                cout << "Brainfuck failed due to asymmetric jumping bracket.\n\n";
                return;
            }
            break;
        }
        default:
            cout << "Brainfuck failed due to unrecognized code symbol.\n\n";
            return;
        }
        ++code_ptr;
    }
}

void print_bf_code_until_code_ptr()
{
    char *walk = code_mem;

    while (walk <= code_ptr)
        putchar(*walk++);
    
    cout << "\n\n";
}

void print_bf_data()
{
    for (int i = 0; i < 65536; ++i)
        printf("%d\t", data_mem[i]);

    cout << "\n\n";
}
