#include<stdio.h>
#include<string.h>

int main(){

    char *sid="hello";
    char hel[5]="hello";
    // strcpy(hel,sid);
    printf("%ld",sizeof(hel));
    printf("%ld",strlen(hel));
    printf("%d",strcmp(hel,sid));
    printf("%s", hel);


}
