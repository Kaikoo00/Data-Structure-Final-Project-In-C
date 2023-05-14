#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

typedef struct node{
    char name[30];
    char nim[30];
    char major[30];
    float gpa;
    char gender;
    int semester;
    int age;
    int hash_result;
    struct node *left, *right;
}node;

char PATH[] = "./data.txt";
node *root;
#define hashSize 2000;

void deleteTree(struct node* node)
{
    if (node == NULL) return;
    deleteTree(node->left);
    deleteTree(node->right);
    free(node);
}

unsigned int hash(char *str) {
  unsigned int hash_val = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    hash_val = hash_val * 31 + (unsigned int) str[i];
  }
  return hash_val % hashSize;
}

void import_data(){
    FILE *fp = fopen(PATH, "r");
    node temp;
    while(fscanf(fp, "%[^;];%[^;];%[^;];%c;%d;%f;%d;%d\n",temp.name, temp.nim, temp.major, &temp.gender, &temp.age, &temp.gpa, &temp.semester,&temp.hash_result)!=EOF){
        struct node *newnode = malloc(sizeof(node));
        strcpy(newnode->name, temp.name); strcpy(newnode->nim, temp.nim); strcpy(newnode->major, temp.major);
        newnode->gender = temp.gender; newnode->age = temp.age; newnode->gpa = temp.gpa; newnode->semester = temp.semester; newnode->hash_result = temp.hash_result;
        if(root==NULL){
        root = newnode;
        continue;
        }else{
            node *parent, *curr = root;
            while(true){
                parent = curr;
                if(curr->hash_result>newnode->hash_result){
                    curr = curr->left;
                    if(curr==NULL){
                        parent->left = newnode;
                        break;
                    }
                }else if(curr->hash_result<newnode->hash_result){
                    curr = curr->right;
                    if(curr == NULL){
                        parent->right = newnode;
                        break;
                    }
                }else if(curr->hash_result==newnode->hash_result){
                    newnode->hash_result+=10;
                    curr = curr->right;
                    if(curr == NULL){
                        parent->right = newnode;
                        break;
                    }
                }
            }
        }
    }
    fclose(fp);
    return;
}

node* create_newnode(){
    node *newnode = malloc(sizeof(node));
    while(true){ //name input
        bool name_status = true;
        printf("Input nama mahasiswa\n>>");scanf("%[^\n]", newnode->name);getchar();
        for(int i=0; i<strlen(newnode->name); i++){
            if(!isalpha(newnode->name[i]) && newnode->name[i]!= ' '){
                name_status = false;
                break;
            }
        }
        if(name_status){
            break;
        }else{
            printf("Name Invalid\n");
        }
    }
    
    while(true){ //gender input
        printf("Input Jenis Kelamin\n>>"); scanf("%c",&newnode->gender);getchar();
        if(tolower(newnode->gender)=='m' ||tolower(newnode->gender)=='f'){
            break;
        }else{
            printf("Gender Input Invalid\n");
        }
    }
    
    while(true){ //age input
        printf("Input umur mahasiswa\n>>"); scanf("%d", &newnode->age); getchar();
        if(newnode->age>=7){
            break;
        }else{
            printf("Age Input Invalid\n");
        }
    }
    
    while(true){ //NIM input
        printf("Input NIM mahasiswa\n>>"); scanf("%[^\n]", newnode->nim);getchar();
        bool nim_status = true;
        for(int i=0; i<strlen(newnode->nim); i++){
            if(!isdigit(newnode->nim[i])){
                nim_status = false;
                break;
            }
        }
        if(nim_status){
            break;
        }else{
            printf("NIM Input Invalid\n");
        }
    }

    while(true){ //Major Input
        printf("Input jurusan\n>>"); scanf("%[^\n]", newnode->major);getchar();
        bool major_status = true;
        for(int i=0; i<strlen(newnode->major); i++){
            if(!isalpha(newnode->major[i]) && newnode->major[i]!= ' '){
                major_status = false;
                break;
            }
        }
        if(major_status){
            break;
        }else{
            printf("Major Invalid\n");
        }
    }

    while(true){ //GPA Input
        printf("Input GPA\n>>"); scanf("%f", &newnode->gpa);getchar();
        if(newnode->gpa<0){
            printf("GPA Input Invalid\n");
        }else{
            break;
        }
    }
    
    while(true){ //Semester Input
        printf("Input semester\n>>"); scanf("%d", &newnode->semester);getchar();
        if(newnode->semester>10 || newnode->semester<=0){
            printf("Semester Input Invalid\n");
        }else{
            break;
        }
    }

    newnode->hash_result = hash(newnode->name);
    newnode->left = NULL; newnode->right = NULL;
    FILE *fp = fopen(PATH, "a");
    fprintf(fp, "%s;%s;%s;%c;%d;%.2f;%d;%d\n", newnode->name, newnode->nim, newnode->major, newnode->gender, newnode->age, newnode->gpa, newnode->semester,newnode->hash_result);
    fclose(fp);
    return newnode;
}

void insert(){
    node *newnode = create_newnode();
    if(root==NULL){
        root = newnode;
        return;
    }else{
        node *parent, *curr = root;
        while(true){
            parent = curr;
            if(curr->hash_result>newnode->hash_result){
                curr = curr->left;
                if(curr==NULL){
                    parent->left = newnode;
                    return;
                }
            }else if(curr->hash_result<newnode->hash_result){
                curr = curr->right;
                if(curr == NULL){
                    parent->right = newnode;
                    return;
                }
            }else if(curr->hash_result==newnode->hash_result){
                newnode->hash_result+=10;
                curr = curr->right;
                if(curr == NULL){
                    parent->right = newnode;
                    return;
                }
            }
        }
    }
}

void print_header(){
    printf("================================================================================================================\n");
    printf("|Name                           |NIM              |Major                     |Gender  |Age   |GPA   |Semester  |\n");
    printf("================================================================================================================\n");
    return;
}

void print_tree(struct node *head){
    if(head==NULL){
        return;
    }
    print_tree(head->right);
    printf("| %30s| %16s| %25s", head->name, head->nim, head->major);
    if(tolower(head->gender)=='m'){
        printf("|    Male|");
    }else{
        printf("|  Female|");
    }
    printf(" %5d| %5.2f| %9d|\n", head->age, head->gpa, head->semester);
    printf("----------------------------------------------------------------------------------------------------------------\n");
    
    print_tree(head->left); 
    return;
}

void print(){
    if(root == NULL){
         printf("Import the data from existing file first or input some new data\n");
         return;
    }
    print_header();
    print_tree(root);
    return;
}

void delete_node(){
    FILE *fp = fopen(PATH,"r");
    FILE *temp = fopen("temp.txt", "w");
    struct node *parent, *curr = root;
    bool delete_status = false;
    int ch = 1;
    while(true){
        system("cls");
        switch(ch){
            case 1:
                printf("What Detail you want to use?\n");
                printf("> Full Name\n");
                printf("NIM\n");
                break;
            case 2:
                printf("What Detail you want to use?\n");
                printf("Full Name\n");
                printf("> NIM\n");
                break;
        }
        int inp = getch();
        if(inp==72 || inp ==75 ){
            if(ch>1) ch--;
            else continue;
        }else if(inp == 80 || inp == 77){
            if(ch<2) ch++;
            else continue;
        }else if(inp==13){
            break;
        }
    }
    if(ch==1){
        char inp_nim[30];
        while(true){ //NIM input
            printf("Input NIM mahasiswa\n>>"); scanf("%[^\n]", inp_nim);getchar();
            bool nim_status = true;
            for(int i=0; i<strlen(inp_nim); i++){
                if(!isdigit(inp_nim[i])){
                    nim_status = false;
                    break;
                }
            }
            if(nim_status){
                break;
            }else{
                printf("NIM Input Invalid\n");
            }
        }
        struct node temp_node;
        while(fscanf(fp, "%[^;];%[^;];%[^;];%c;%d;%f;%d;%d\n",temp_node.name, temp_node.nim, temp_node.major, &temp_node.gender, &temp_node.age, &temp_node.gpa, &temp_node.semester,&temp_node.hash_result)!=EOF){
            if(strcmp(temp_node.nim, inp_nim)==0){
                deleteTree(root);
                root = NULL;
                delete_status = true;
                continue;
            }else{
                fprintf(temp, "%s;%s;%s;%c;%d;%.2f;%d;%d\n", temp_node.name, temp_node.nim, temp_node.major, temp_node.gender, temp_node.age, temp_node.gpa, temp_node.semester,temp_node.hash_result);
            }
        }
        fclose(temp); fclose(fp);
        remove("data.txt");
        rename("temp.txt", "data.txt");
        import_data();
        return;
    }else if(ch==2){
        char inp_name[30];
        while(true){ //name input
            bool name_status = true;
            printf("Input nama mahasiswa\n>>");scanf("%[^\n]", inp_name);getchar();
            for(int i=0; i<strlen(inp_name); i++){
                if(!isalpha(inp_name[i]) && inp_name[i]!= ' '){
                    name_status = false;
                    break;
                }
            }
            if(name_status){
                break;
            }else{
                printf("Name Invalid\n");
            }
        }
        struct node temp_node;
        while(fscanf(fp, "%[^;];%[^;];%[^;];%c;%d;%f;%d;%d\n",temp_node.name, temp_node.nim, temp_node.major, &temp_node.gender, &temp_node.age, &temp_node.gpa, &temp_node.semester,&temp_node.hash_result)!=EOF){
            if(strcmp(temp_node.name, inp_name)==0){
                deleteTree(root);
                root = NULL;
                delete_status = true;
                continue;
            }else{
                fprintf(temp, "%s;%s;%s;%c;%d;%.2f;%d;%d\n", temp_node.name, temp_node.nim, temp_node.major, temp_node.gender, temp_node.age, temp_node.gpa, temp_node.semester,temp_node.hash_result);
            }
        }
        fclose(temp); fclose(fp);
        remove("data.txt");
        rename("temp.txt", "data.txt");
        deleteTree(root);
        root = NULL;
        import_data();
        return;
    }
    return;
}

void modify_data(){
    
}

int main_menu(){
    int ch = 1;
    while(true){
        switch(ch)
        {
        case 1:
            system("cls");
            printf("> 1. Input New Data <\n");
            printf("2. Print Data\n");
            printf("3. Delete Node\n");
            printf("4. Modify Existing Data\n");
            printf("5. Exit\n");
            break;
        case 2:
            system("cls");
            printf("1. Input New Data\n");
            printf("> 2. Print Data <\n");
            printf("3. Delete Node\n");
            printf("4. Modify Existing Data\n");
            printf("5. Exit\n");
            break;
        case 3:
            system("cls");
            printf("1. Input New Data\n");
            printf("2. Print Data\n");
            printf("> 3. Delete Node <\n");
            printf("4. Modify Existing Data\n");
            printf("5. Exit\n");
            break;
        case 4:
            system("cls");
            printf("1. Input New Data\n");
            printf("2. Print Data\n");
            printf("3. Delete Node\n");
            printf("> 4. Modify Existing Data\n");
            printf("5. Exit\n");
            break;
        case 5:
            system("cls");
            printf("1. Input New Data\n");
            printf("2. Print Data\n");
            printf("3. Delete Node\n");
            printf("4. Modify Existing Data <\n");
            printf("> 5. Exit\n");
            break;
        
        }
        int inp = getch();
        if(inp==72 || inp ==75 ){
            if(ch>1) ch--;
            else continue;
        }else if(inp == 80 || inp == 77){
            if(ch<5) ch++;
            else continue;
        }else if(inp==13){
            return ch;
        }
    }
}

int main(){
    printf("\033[30m"); printf("\033[47m"); system("cls"); // set background color to white and text color to black
    int ch = 1;
    while(true){
        system("cls");
        switch(ch){
            case 1:
                printf("Do you want to import the previous data?\n");
                printf("> Y\n");
                printf("N\n");
                break;
            case 2:
                printf("Do you want to import the previous data?\n");
                printf("Y\n");
                printf("> N\n");
                break;
        }
        int inp = getch();
        if(inp==72 || inp ==75 ){
            if(ch>1) ch--;
            else continue;
        }else if(inp == 80 || inp == 77){
            if(ch<2) ch++;
            else continue;
        }else if(inp==13){
            if(ch==1){
                import_data();
            }
            break;
        }
    }

    while(true){
        int ch = main_menu();
        system("cls");
        switch(ch){
            case 1:
                insert();
                break;
            case 2:
                print();
                system("pause");
                break;
            case 3:
                print();
                delete_node();
                break;
            case 4:
                print();
                modify_data();
                break;
            case 5:
                exit(0);
        }
    }
    return 0;
}
