#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#define BUF_SIZE 128
#define MAX_NUM 128

int *input_numbers(void)
{
    int exit = 0, num, i = 0, *arr;
    char buf[BUF_SIZE], *pEnd;
    arr = (int *) malloc(MAX_NUM * sizeof(int));
    puts("###Enter numbers. To stop enter -1");
    while (!exit) {
        fgets(buf, BUF_SIZE, stdin);
        for (pEnd = buf; *pEnd != 0 && !exit;) {
            num = strtol(pEnd, &pEnd, 10);
            if (num == 0) {
                pEnd++;
                continue;
            }
            if (num == -1) {
                exit = 1;
                break;
            }
            arr[i++] = num;
        }
    }
    arr[i] = -1;
    return arr;
}

void add_num_to_binFile(char *bin_name)
{
    FILE *bin_file;
    int i, *arr;
    if (!(bin_file = fopen(bin_name, "wb+"))) {
        puts("###Can't open binary file");
        exit(0);
    }
    arr = input_numbers();
    for (i = 0; arr[i] != -1; i++)
        fwrite(&arr[i], sizeof(int), 1, bin_file);
    free(arr);
    fclose(bin_file);
}

void add_num_to_textFile(char *text_name)
{
    FILE *text_file;
    int i, *arr;
    if (!(text_file = fopen(text_name, "wt+"))) {
        puts("###Can't open text file");
        exit(0);
    }
    arr = input_numbers();
    for (i = 0; arr[i] != -1; i++)
        fprintf(text_file, "%d ", arr[i]);
    free(arr);
    fclose(text_file);
}

void print_textFile(char *text_name)
{
    FILE *text_file;
    char buf[BUF_SIZE];
    if (!(text_file = fopen(text_name, "rt"))) {
        puts("###Can't open text file");
        exit(0);
    }
    puts("###Text file:");
    while (fgets(buf, BUF_SIZE, text_file)) {
        printf("%s", buf);
    }
    puts("");
    fclose(text_file);
}

void print_binFile(char *bin_name)
{
    FILE *bin_file;
    int num;
    if (!(bin_file = fopen(bin_name, "rb"))) {
        puts("###Can't open binary file");
        exit(0);
    }
    puts("###Binary file:");
    while (fread(&num, sizeof(int), 1, bin_file)) {
        printf("%d ", num);
    }
    puts("");
    fclose(bin_file);
}

void writing_new_bin_file(int *firstArray, char *bin_name)
{
    int i;
    FILE *bin_file;
    if (!(bin_file = fopen(bin_name, "wb"))) {
        puts("###Can't open binary file");
        exit(0);
    }
    for (i = 0; firstArray[i] != -1; i++)
        fwrite(&firstArray[i], sizeof(int), 1, bin_file);
    fclose(bin_file);
    free(firstArray);
}

void delete_numbers(char *text_name, char *bin_name)
{
    FILE *bin_file, *text_file;
    int i, j, k, num, *firstArray, *secondArray;
    firstArray = (int *) malloc(MAX_NUM * sizeof(int));
    secondArray = (int *) malloc(MAX_NUM * sizeof(int));
    if (!(bin_file = fopen(bin_name, "rb"))) {
        puts("###Can't open binary file");
        exit(0);
    }
    if (!(text_file = fopen(text_name, "rt"))) {
        puts("###Can't open text file");
        exit(0);
    }

    for (i = 0; fread(&num, sizeof(int), 1, bin_file); i++)
        firstArray[i] = num;
    firstArray[i] = -1;

    for (i = 0; fscanf(text_file, "%d ", &num) != EOF; i++)
        secondArray[i] = num;
    secondArray[i] = -1;

    for (i = 0; firstArray[i] != -1; i++) {
        for (j = 0; secondArray[j] != -1; j++) {
            if (firstArray[i] == secondArray[j]) {
                for (k = i; firstArray[k] != -1; k++)
                    firstArray[k] = firstArray[k + 1];
            }
        }
    }

    fclose(text_file);
    fclose(bin_file);
    free(secondArray);
    writing_new_bin_file(firstArray, bin_name);
}

void print_menu(char *text_name, char *bin_name)
{
    char c;
    int exit = 0;
    while (!exit) {
        puts("Enter num for action:");
        printf("1 - adding numbers to the binary file\n"
               "2 - adding numbers to the text file\n"
               "3 - removal numbers in the text file from binary file\n"
               "4 - printing binary file\n"
               "5 - printing text file\n" "6 - exit\n");
        while ((c = getchar()) == '\n');
        switch (c) {
        case '1':
            add_num_to_binFile(bin_name);
            break;
        case '2':
            add_num_to_textFile(text_name);
            break;
        case '3':
            delete_numbers(text_name, bin_name);
            break;
        case '4':
            print_binFile(bin_name);
            break;
        case '5':
            print_textFile(text_name);
            break;
        case '6':
            exit = 1;
            break;
        default:
            puts("###Wrong input, try again");
            break;
        }
    }
}

void readme(void)
{
    puts("Usage: ./main <options>");
    puts("  -t <file name>         name of text file");
    puts("  -b <file name>         name of binary file");
    puts("  -h                     help");
    exit(0);
}

int main(int argc, char *argv[])
{
    const char *options = "t:b:h";
    int opt;
    char *text_name, *bin_name;
    opt = getopt(argc, argv, options);
    if (argc < 2 && opt != 'h') {
        puts("too small arguments. use -h for help");
        exit(0);
    }
    while (opt != -1) {
        switch (opt) {
        case 't':
            text_name = optarg;
            break;

        case 'b':
            bin_name = optarg;
            break;

        case 'h':
            readme();
            break;

        default:
            break;
        }
        opt = getopt(argc, argv, options);
    }
    print_menu(text_name, bin_name);
    return 0;
}
