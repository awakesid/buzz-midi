#include<stdio.h>
#include<string.h>

#define MIDIID "MThd"

int main(){

    char *sid="MThd";
    char hel[5]="hello";
    // strcpy(hel,sid);
    // printf("%ld",sizeof(hel));
    // printf("%ld",strlen(hel));
    printf("%d",strcmp(sid,MIDIID));
    printf("%s", hel);


}
