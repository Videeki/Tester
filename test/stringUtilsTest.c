#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "stringUtils.h"

int main(int argc, char* argv[])
{
    //int ret = string_trim_before(argv[1], 0);
    //return ret == atoi(argv[2]) ? EXIT_SUCCESS : EXIT_FAILURE;

    PATH pwd = string_path_init("/home/videeki/Documents");
    pwd = string_path_append(pwd, "GitRepos");
    pwd = string_path_append(pwd, "CCPP");
    pwd = string_path_append(pwd, "02_Projects");
    pwd = string_path_append(pwd, "Tester");

    if(pwd != NULL)
        printf("%s\n", pwd);

    string_path_free(pwd);

    XPATH pwd2 = string_xpath_init("/home/videeki/Documents");
    pwd2 = string_xpath_append_items(pwd2, "GitRepos", "CCPP", "02_Projects", "Tester", NULL);

    if(pwd2 != NULL)
        printf("%s\n", pwd2);

    string_xpath_free(pwd2);
}

