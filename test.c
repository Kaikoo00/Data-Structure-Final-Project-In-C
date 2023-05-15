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
    char major[50];
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
    hash_val = hash_val + (unsigned int) str[i];
  }
  return hash_val % hashSize;
}

void import_data(){
    FILE *fp = fopen(PATH, "r");
    if(fp==NULL){
        system("cls");
        printf("\n\n\n");
        printf("                =====================================================================\n");
        printf("                ||                                                                 ||\n");
        printf("                || File Don't Exist Please Input Some New Data And Make A New File ||\n");
        printf("                ||                                                                 ||\n");
        printf("                =====================================================================\n");
        printf("\n\n\n");
        system("pause");
        return;
    }
    node temp;
    node *parent, *curr;
    while(fscanf(fp, "%[^;];%[^;];%[^;];%c;%d;%f;%d;%d\n",temp.name, temp.nim, temp.major, &temp.gender, &temp.age, &temp.gpa, &temp.semester,&temp.hash_result)!=EOF){
        struct node *newnode = (struct node*) malloc(sizeof(node));
        strcpy(newnode->name, temp.name); strcpy(newnode->nim, temp.nim); strcpy(newnode->major, temp.major);
        newnode->gender = temp.gender; newnode->age = temp.age; newnode->gpa = temp.gpa; newnode->semester = temp.semester; newnode->hash_result = temp.hash_result;
        newnode->left = NULL; newnode->right = NULL;
        if(root==NULL){
        root = newnode;
        continue;
        }else{
            parent = root;curr = root;
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

bool NIM_search(node *curr, char *inpNIM){
    if(curr!=NULL){
        bool found_left = NIM_search(curr->left, inpNIM);
        if(!strcmp(inpNIM, curr->nim)){
            system("cls");
            printf("================================================================================================================\n");
            printf("|Name                           |NIM              |Major                     |Gender  |Age   |GPA   |Semester  |\n");
            printf("================================================================================================================\n");
            printf("| %30s| %16s| %25s", curr->name, curr->nim, curr->major);
            if(tolower(curr->gender)=='m'){
                printf("|    Male|");
            }else{
                printf("|  Female|");
            }
            printf(" %5d| %5.2f| %9d|\n", curr->age, curr->gpa, curr->semester);
            printf("----------------------------------------------------------------------------------------------------------------\n");
            system("pause");
            return true;
        }
        bool found_right = NIM_search(curr->right, inpNIM);
        return found_left || found_right;
    }
    return false;
}

bool name_search(char *inp_name){
        bool found = false;
        int temp_hash = hash(inp_name);
        struct node *parent, *curr = root;
        while(curr != NULL){
            if(temp_hash == curr->hash_result && !strcmp(curr->name, inp_name)){
                found = true;
                system("cls");
                printf("================================================================================================================\n");
                printf("|Name                           |NIM              |Major                     |Gender  |Age   |GPA   |Semester  |\n");
                printf("================================================================================================================\n");
                printf("| %30s| %16s| %25s", curr->name, curr->nim, curr->major);
                if(tolower(curr->gender)=='m'){
                    printf("|    Male|");
                }else{
                    printf("|  Female|");
                }
                printf(" %5d| %5.2f| %9d|\n", curr->age, curr->gpa, curr->semester);
                printf("----------------------------------------------------------------------------------------------------------------\n");
                system("pause");
                return true;
            }else if(temp_hash == curr->hash_result && strcmp(curr->name, inp_name)){
                temp_hash+=10;
                curr = curr->right;
            }else if(temp_hash >curr->hash_result){
                curr = curr->right;
            }else if(temp_hash<curr->hash_result){
                curr = curr->left;
            }
            if(curr == NULL){
                break;
            }
        }
        return false;
}

node* create_newnode(){
    node *newnode = malloc(sizeof(node));
    bool existed = false;
    while(true){ //name input
        bool name_status = true;
        printf("Name Input (Input 0 to go back to main menu)\n>>");scanf("%[^\n]", newnode->name);getchar();
        if(newnode->name[0]=='0' && strlen(newnode->name)==1){
            return newnode;
        }
        if(strlen(newnode->name)<6 || strlen(newnode->name)>30){
            printf("Name Invalid\n");
            continue;
        }
        for(int i=0; i<strlen(newnode->name); i++){
            if(!isalpha(newnode->name[i]) && newnode->name[i]!= ' '){
                name_status = false;
                break;
            }else if(newnode->name[0] == '0'){
                return(newnode);
            }
        }
        if(name_status){
            break;
        }else{
            printf("Name Invalid\n");
        }
    }
    existed = name_search(newnode->name);
    if(existed){
        newnode->name[0] = '-';
        return newnode;
    }
    
    while(true){ //NIM input
        printf("NIM Input (Input 0 to cancel the Input)\n>>"); scanf("%[^\n]", newnode->nim);getchar();
        bool nim_status = true;
        if(strlen(newnode->nim)<6 || strlen(newnode->nim)>30){
            printf("NIM Input Invalid\n");
            continue;
        }
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
    if(newnode->nim[0]=='0'&&strlen(newnode->nim)==1){
        newnode->name[0]='-'; return newnode;
    }
    existed = NIM_search(root, newnode->nim);
    if(existed){
        newnode->name[0] = '-';
        return newnode;
    }

    while(true){ //gender input
        printf("Gender Input (M/F) (Input 0 to cancel the Input)\n>>"); scanf("%c",&newnode->gender);getchar();
        if(tolower(newnode->gender)=='m' ||tolower(newnode->gender)=='f' || newnode->gender=='0'){
            break;
        }else{
            printf("Gender Input Invalid\n");
        }
    }
    
    if(newnode->gender=='0'){
        newnode->name[0]='-'; return newnode;
    }

    while(true){ //age input
        printf("Age Input (Input 0 to cancel the Input)\n>>"); scanf("%d", &newnode->age); getchar();
        if(newnode->age>=7 || newnode->age==0){
            break;
        }else{
            printf("Age Input Invalid\n");
        }
    }

    if(newnode->age == 0){
        newnode->name[0]='-'; return newnode;
    }

    while(true){ //Major Input
        printf("Major Input (Input 0 to cancel the Input)\n>>"); scanf("%[^\n]", newnode->major);getchar();
        bool major_status = true;
        for(int i=0; i<strlen(newnode->major); i++){
            if(!isalpha(newnode->major[i]) && newnode->major[i]!= ' ' && newnode->major[0] != '0'){
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

    if(newnode->major[0]=='0'){
        newnode->name[0]='-'; return newnode;
    }

    while(true){ //GPA Input
        printf(" GPA Input (Input -1 to cancel the Input)\n>>"); scanf("%f", &newnode->gpa);getchar();
        if(newnode->gpa==-1){
            newnode->name[0]='-'; return newnode;
        }else if(newnode->gpa<0 && newnode->gpa>4){
            printf("GPA Input Invalid\n");
        }else{
            break;
        }
    }
    
    while(true){ //Semester Input
        printf("Semester Input (Input 0 to cancel the Input)\n>>"); scanf("%d", &newnode->semester);getchar();
        if(newnode->semester>10 || newnode->semester<0){
            printf("Semester Input Invalid\n");
        }else{
            break;
        }
    }
    if(newnode->semester==0){
        newnode->name[0]='-'; return newnode;
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
    if(newnode->name[0] == '0' || newnode->name[0] == '-'){
        printf("Input Function Canceled\n");
        free(newnode);
        system("pause");
        return;
    }
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
    printf("| Name                          | NIM             | Major                    | Gender | Age  | GPA  | Semester |\n");
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
        printf("\n\n\n");
        printf("                =====================================================================\n");
        printf("                ||                                                                 ||\n");
        printf("                || Import the data from existing file first or input some new data ||\n");
        printf("                ||                                                                 ||\n");
        printf("                =====================================================================\n");
        printf("\n\n\n");
        return;
    }
    print_header();
    print_tree(root);
    printf("\n");
    return;
}

void delete_node(){
    FILE *fp = fopen(PATH,"r");
    if(fp==NULL){
        printf("\n\n\n");
        printf("                =====================================================================\n");
        printf("                ||                                                                 ||\n");
        printf("                || Import the data from existing file first or input some new data ||\n");
        printf("                ||                                                                 ||\n");
        printf("                =====================================================================\n");
        printf("\n\n\n");
        system("pause");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    struct node *parent, *curr = root;
    bool delete_status = false;
    int ch = 1;
    while(true){
        system("cls");
        print();
        switch(ch){
            case 1:
                printf("===========================================\n");
                printf("|       What Detail you want to use?      |\n");
                printf("===========================================\n");
                printf("| > NIM --> (Uses NIM to Search)        < |\n");
                printf("|   Name --> (Uses Full Name to Search)   |\n");
                printf("|   Exit --> (Back to Main Menu)          |\n");
                printf("===========================================\n");
                break;
            case 2:
                printf("===========================================\n");
                printf("|       What Detail you want to use?      |\n");
                printf("===========================================\n");
                printf("|   NIM --> (Uses NIM to Search)          |\n");
                printf("| > Name --> (Uses Full Name to Search) < |\n");
                printf("|   Exit --> (Back to Main Menu)          |\n");
                printf("===========================================\n");
                break;
            case 3:
                printf("===========================================\n");
                printf("|       What Detail you want to use?      |\n");
                printf("===========================================\n");
                printf("|   NIM --> (Uses NIM to Search)          |\n");
                printf("|   Name --> (Uses Full Name to Search)   |\n");
                printf("| > Exit --> (Back to Main Menu)        < |\n");
                printf("===========================================\n");
                break;
        }
        int inp = getch();
        if(inp==72 || inp ==75 || inp == 119 || inp == 97){
            if(ch>1) ch--;
            else if(ch==1) ch=3;
            else continue;
        }else if(inp == 80 || inp == 77 || inp == 115 || inp == 100){
            if(ch<3) ch++;
            else if(ch==3) ch=1;
            else continue;
        }else if(inp==13){
            break;
        }
    }
    if(ch==1){
        char inp_nim[30];
        while(true){ //NIM input
            printf("\nInput Data's NIM that you're searching for...\n>>"); scanf("%[^\n]", inp_nim);getchar();
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
    }else if(ch==2){
        char inp_name[30];
        while(true){ //name input
            bool name_status = true;
            printf("\nInput Data's Name that you're searching for...\n>>");scanf("%[^\n]", inp_name);getchar();
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
    }else if(ch==3){
        return;
    }
    if(!delete_status){
        printf("Data Not Found (!)\n");
        system("pause");
    }else{
        printf("Data Deleted Successfully\n");
        system("pause");
    }
    return;
}

void modify_data(){
    FILE *fp = fopen(PATH,"r");
    if(fp==NULL || root == NULL){
        printf("\n\n\n");
        printf("                =====================================================================\n");
        printf("                ||                                                                 ||\n");
        printf("                || Import the data from existing file first or input some new data ||\n");
        printf("                ||                                                                 ||\n");
        printf("                =====================================================================\n");
        printf("\n\n\n");
        system("pause");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    struct node *parent, *curr = root;
    bool modify_status = false;
    int ch = 1;
    while(true){
        system("cls");
        print();
        switch(ch){
            case 1:
                printf("===========================================\n");
                printf("|       What Detail you want to use?      |\n");
                printf("===========================================\n");
                printf("| > NIM --> (Uses NIM to Search)        < |\n");
                printf("|   Name --> (Uses Full Name to Search)   |\n");
                printf("|   Exit --> (Back to Main Menu)          |\n");
                printf("===========================================\n");
                break;
            case 2:
                printf("===========================================\n");
                printf("|       What Detail you want to use?      |\n");
                printf("===========================================\n");
                printf("|   NIM --> (Uses NIM to Search)          |\n");
                printf("| > Name --> (Uses Full Name to Search) < |\n");
                printf("|   Exit --> (Back to Main Menu)          |\n");
                printf("===========================================\n");
                break;
            case 3:
                printf("===========================================\n");
                printf("|       What Detail you want to use?      |\n");
                printf("===========================================\n");
                printf("|   NIM --> (Uses NIM to Search)          |\n");
                printf("|   Name --> (Uses Full Name to Search)   |\n");
                printf("| > Exit --> (Back to Main Menu)        < |\n");
                printf("===========================================\n");
                break;
        }
        int inp = getch();
        if(inp==72 || inp ==75 || inp == 119 || inp == 97){
            if(ch>1) ch--;
            else if(ch==1) ch=3;
            else continue;
        }else if(inp == 80 || inp == 77 || inp == 115 || inp == 100){
            if(ch<3) ch++;
            else if(ch==3) ch=1;
            else continue;
        }else if(inp==13){
            break;
        }
    }
    if(ch==1){
        char inp_nim[30];
        while(true){ //NIM input
            printf("\nInput Data's NIM that you're searching for...\n>>"); scanf("%[^\n]", inp_nim);getchar();
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
                modify_status = true;
                while(true){ //gender input
                    char tempinp;
                    printf("Gender Input (Input 0 if there's no change)\n>>"); scanf("%c",&tempinp);getchar();
                    if(tolower(tempinp)=='m' ||tolower(tempinp)=='f' ||tempinp=='0'){
                        if(tempinp=='0'){
                            break;
                        }else{
                            temp_node.gender = tempinp;
                        }
                        break;
                    }else{
                        printf("Gender Input Invalid\n");
                    }
                }
                
                while(true){ //age input
                    int tempinp;
                    printf("AgeInput (Input 0 if there's no change)\n>>"); scanf("%d", &tempinp); getchar();
                    if(tempinp>=7 || tempinp==0){
                        if(tempinp==0){
                            break;
                        }else{
                            temp_node.age = tempinp;
                            break;
                        }  
                    }else{
                        printf("Age Input Invalid\n");
                    }
                }

                while(true){ //Major Input
                    char tempinp[30];
                    printf("Major Input (Input 0 if there's no change)\n>>"); scanf("%[^\n]", tempinp);getchar();
                    bool major_status = true;
                    for(int i=0; i<strlen(tempinp); i++){
                        if(!isalpha(tempinp[i]) && tempinp[i]!= ' ' &&tempinp[0]!='0'){
                            major_status = false;
                            break;
                        }
                    }
                    if(major_status){
                        if(tempinp[0] == '0' && strlen(tempinp)==1){
                            break;
                        }else{
                            strcpy(temp_node.major,tempinp);
                            break;
                        }
                    }else{
                        printf("Major Invalid\n");
                    }
                }

                while(true){ //GPA Input
                float tempinp;
                    printf("Input GPA (Input 0 if there's no change)\n>>"); scanf("%f", &tempinp);getchar();
                    if(tempinp<0){
                        printf("GPA Input Invalid\n");
                    }else{
                        if(tempinp==0){
                            break;
                        }else{
                            temp_node.gpa=tempinp;
                            break;
                        }
                    }
                }
                
                while(true){ //Semester Input
                int tempinp;
                    printf("Input semester (Input 0 if there's no change)\n>>"); scanf("%d", &tempinp);getchar();
                    if(tempinp>10 || tempinp<0){
                        printf("Semester Input Invalid\n");
                    }else{
                        if(tempinp==0){
                            break;
                        }else{
                            temp_node.semester=tempinp;
                            break;
                        }
                    }
                }
                fprintf(temp, "%s;%s;%s;%c;%d;%.2f;%d;%d\n", temp_node.name, temp_node.nim, temp_node.major, temp_node.gender, temp_node.age, temp_node.gpa, temp_node.semester,temp_node.hash_result);
            }else{
                fprintf(temp, "%s;%s;%s;%c;%d;%.2f;%d;%d\n", temp_node.name, temp_node.nim, temp_node.major, temp_node.gender, temp_node.age, temp_node.gpa, temp_node.semester,temp_node.hash_result);
            }
        }
        fclose(temp); fclose(fp);
        remove("data.txt");
        rename("temp.txt", "data.txt");
        import_data();
    }else if(ch==2){
        char inp_name[30];
        while(true){ //name input
            bool name_status = true;
            printf("\nInput Data's Name that you're searching for...\n>>");scanf("%[^\n]", inp_name);getchar();
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
                modify_status = true;
                while(true){ //gender input
                    char tempinp;
                    printf("Gender Input (Input 0 if there's no change)\n>>"); scanf("%c",&tempinp);getchar();
                    if(tolower(tempinp)=='m' ||tolower(tempinp)=='f' ||tempinp=='0'){
                        if(tempinp=='0'){
                            break;
                        }else{
                            temp_node.gender = tempinp;
                        }
                        break;
                    }else{
                        printf("Gender Input Invalid\n");
                    }
                }
                
                while(true){ //age input
                    int tempinp;
                    printf("AgeInput (Input 0 if there's no change)\n>>"); scanf("%d", &tempinp); getchar();
                    if(tempinp>=7 || tempinp==0){
                        if(tempinp==0){
                            break;
                        }else{
                            temp_node.age = tempinp;
                            break;
                        }  
                    }else{
                        printf("Age Input Invalid\n");
                    }
                }

                while(true){ //Major Input
                    char tempinp[30];
                    printf("Major Input (Input 0 if there's no change)\n>>"); scanf("%[^\n]", tempinp);getchar();
                    bool major_status = true;
                    for(int i=0; i<strlen(tempinp); i++){
                        if(!isalpha(tempinp[i]) && tempinp[i]!= ' ' &&tempinp[0]!='0'){
                            major_status = false;
                            break;
                        }
                    }
                    if(major_status){
                        if(tempinp[0] == '0' && strlen(tempinp)==1){
                            break;
                        }else{
                            strcpy(temp_node.major,tempinp);
                            break;
                        }
                    }else{
                        printf("Major Invalid\n");
                    }
                }

                while(true){ //GPA Input
                float tempinp;
                    printf("Input GPA (Input 0 if there's no change)\n>>"); scanf("%f", &tempinp);getchar();
                    if(tempinp<0){
                        printf("GPA Input Invalid\n");
                    }else{
                        if(tempinp==0){
                            break;
                        }else{
                            temp_node.gpa=tempinp;
                            break;
                        }
                    }
                }
                
                while(true){ //Semester Input
                int tempinp;
                    printf("Input semester (Input 0 if there's no change)\n>>"); scanf("%d", &tempinp);getchar();
                    if(tempinp>10 || tempinp<0){
                        printf("Semester Input Invalid\n");
                    }else{
                        if(tempinp==0){
                            break;
                        }else{
                            temp_node.semester=tempinp;
                            break;
                        }
                    }
                }
                fprintf(temp, "%s;%s;%s;%c;%d;%.2f;%d;%d\n", temp_node.name, temp_node.nim, temp_node.major, temp_node.gender, temp_node.age, temp_node.gpa, temp_node.semester,temp_node.hash_result);
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
    }else if(ch==3){
        return;
    }
    if(!modify_status){
        printf("Data Not Found (!)\n");
        system("pause");
    }else{
        printf("Data Successfully Edited\n");
        system("pause");
    }
    return;
}

void search(){
    if(root==NULL){
        print();
        system("pause");
        return;
    }
    int ch=1;
    bool found = false;
    while(true){
        system("cls");
        print();
        switch(ch){
            case 1:
                printf("===========================================\n");
                printf("|       What Detail you want to use?      |\n");
                printf("===========================================\n");
                printf("| > NIM --> (Uses NIM to Search)        < |\n");
                printf("|   Name --> (Uses Full Name to Search)   |\n");
                printf("|   Exit --> (Back to Main Menu)          |\n");
                printf("===========================================\n");
                break;
            case 2:
                printf("===========================================\n");
                printf("|       What Detail you want to use?      |\n");
                printf("===========================================\n");
                printf("|   NIM --> (Uses NIM to Search)          |\n");
                printf("| > Name --> (Uses Full Name to Search) < |\n");
                printf("|   Exit --> (Back to Main Menu)          |\n");
                printf("===========================================\n");
                break;
            case 3:
                printf("===========================================\n");
                printf("|       What Detail you want to use?      |\n");
                printf("===========================================\n");
                printf("|   NIM --> (Uses NIM to Search)          |\n");
                printf("|   Name --> (Uses Full Name to Search)   |\n");
                printf("| > Exit --> (Back to Main Menu)        < |\n");
                printf("===========================================\n");
                break;
        }
        int inp = getch();
        if(inp==72 || inp ==75 || inp == 119 || inp == 97){
            if(ch>1) ch--;
            else if(ch==1) ch=3;
            else continue;
        }else if(inp == 80 || inp == 77 || inp == 115 || inp == 100){
            if(ch<3) ch++;
            else if(ch==3) ch=1;
            else continue;
        }else if(inp==13){
            break;
        }
    }
    if(ch==1){
        char inp_nim[30];
        while(true){ //NIM input
            printf("\nInput Data's NIM that you're searching for...\n>>"); scanf("%[^\n]", inp_nim);getchar();
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
        found = NIM_search(root, inp_nim);
    }else if(ch==2){
        char inp_name[30];
         while(true){ //name input
            bool name_status = true;
            printf("\nInput Data's Name that you're searching for...\n>>");scanf("%[^\n]", inp_name);getchar();
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
        found =  name_search(inp_name);
    }else if(ch==3){
        return;
    }
    if(!found){
        printf("\nData Not Found (!)\n");
        system("pause");
    }
    return;
}

int main_menu(){
    int ch = 1;
    while(true){
        switch(ch)
        {
        case 1:
            system("cls");
            printf("===========================================================================\n");
            printf("|                       STUDENT DATA CENTER                               |\n");
            printf("===========================================================================\n");
            printf("| > 1. Input New Data --> (Will be saved in data.txt for following use) < |\n");
            printf("|   2. Print Data  --> (Displays the whole data in form of a table)       |\n");
            printf("|   3. Delete Node --> (Deletes Node Permanently)                         |\n");
            printf("|   4. Modify Existing Data --> (Change data's details and Save it)       |\n");
            printf("|   5. Search --> (Display a single data that matches the Input)          |\n");
            printf("|   6. Exit --> (Exits The Program)                                       |\n");
            printf("===========================================================================\n");
            break;
        case 2:
            system("cls");
            printf("===========================================================================\n");
            printf("|                       STUDENT DATA CENTER                               |\n");
            printf("===========================================================================\n");
            printf("|   1. Input New Data --> (Will be saved in data.txt for following use)   |\n");
            printf("| > 2. Print Data  --> (Displays the whole data in form of a table)     < |\n");
            printf("|   3. Delete Node --> (Deletes Node Permanently)                         |\n");
            printf("|   4. Modify Existing Data --> (Change data's details and Save it)       |\n");
            printf("|   5. Search --> (Display a single data that matches the Input)          |\n");
            printf("|   6. Exit --> (Exits The Program)                                       |\n");
            printf("===========================================================================\n");
            break;
        case 3:
            system("cls");
            printf("===========================================================================\n");
            printf("|                       STUDENT DATA CENTER                               |\n");
            printf("===========================================================================\n");
            printf("|   1. Input New Data --> (Will be saved in data.txt for following use)   |\n");
            printf("|   2. Print Data  --> (Displays the whole data in form of a table)       |\n");
            printf("| > 3. Delete Node --> (Deletes Node Permanently)                       < |\n");
            printf("|   4. Modify Existing Data --> (Change data's details and Save it)       |\n");
            printf("|   5. Search --> (Display a single data that matches the Input)          |\n");
            printf("|   6. Exit --> (Exits The Program)                                       |\n");
            printf("===========================================================================\n");
            break;
        case 4:
            system("cls");
            printf("===========================================================================\n");
            printf("|                       STUDENT DATA CENTER                               |\n");
            printf("===========================================================================\n");
            printf("|   1. Input New Data --> (Will be saved in data.txt for following use)   |\n");
            printf("|   2. Print Data  --> (Displays the whole data in form of a table)       |\n");
            printf("|   3. Delete Node --> (Deletes Node Permanently)                         |\n");
            printf("| > 4. Modify Existing Data --> (Change data's details and Save it)     < |\n");
            printf("|   5. Search --> (Display a single data that matches the Input)          |\n");
            printf("|   6. Exit --> (Exits The Program)                                       |\n");
            printf("===========================================================================\n");
            break;
        case 5:
            system("cls");
            printf("===========================================================================\n");
            printf("|                       STUDENT DATA CENTER                               |\n");
            printf("===========================================================================\n");
            printf("|   1. Input New Data --> (Will be saved in data.txt for following use)   |\n");
            printf("|   2. Print Data  --> (Displays the whole data in form of a table)       |\n");
            printf("|   3. Delete Node --> (Deletes Node Permanently)                         |\n");
            printf("|   4. Modify Existing Data --> (Change data's details and Save it)       |\n");
            printf("| > 5. Search --> (Display a single data that matches the Input)        < |\n");
            printf("|   6. Exit --> (Exits The Program)                                       |\n");
            printf("===========================================================================\n");
            break;
        case 6:
            system("cls");
            printf("===========================================================================\n");
            printf("|                       STUDENT DATA CENTER                               |\n");
            printf("===========================================================================\n");
            printf("|   1. Input New Data --> (Will be saved in data.txt for following use)   |\n");
            printf("|   2. Print Data  --> (Displays the whole data in form of a table)       |\n");
            printf("|   3. Delete Node --> (Deletes Node Permanently)                         |\n");
            printf("|   4. Modify Existing Data --> (Change data's details and Save it)       |\n");
            printf("|   5. Search --> (Display a single data that matches the Input)          |\n");
            printf("| > 6. Exit --> (Exits The Program)                                     < |\n");
            printf("===========================================================================\n");
            break;
        }
        int inp = getch();
        if(inp==72 || inp ==75 || inp == 119 || inp == 97){
            if(ch>1) ch--;
            else if(ch==1) ch=6;
            else continue;
        }else if(inp == 80 || inp == 77 || inp == 115 || inp == 100){
            if(ch<6) ch++;
            else if(ch==6) ch=1;
            else continue;
        }else if(inp==13){
            return ch;
        }
    }
}

int main(){
    printf("\033[30m"); printf("\033[47m"); system("cls"); // set background color to white and text color to black
    printf("\n\n\n\n");
    printf("                ===============================================================================\n");
    printf("                ||                                                                           ||\n");
    printf("                ||                            Press ANY Key To Start!                        ||\n");
    printf("                ||                                                                           ||\n");
    printf("                ===============================================================================\n");
    printf("\n\n\n\n");
    int t=getch();
    system("cls");
    int i=0;
    while(i<5){
        printf("\n\n\n");
        printf("                =============================================================================\n");
        printf("                ||                                                                         ||\n");
        printf("                || This Program Uses Keyboard Only to Navigate Through All The Features !  ||\n");
        printf("                ||                                                                         ||\n");
        printf("                =============================================================================\n");
        printf("\n\n\n");
        if(i==0){
            printf("                -----------------------------------------------------------------------------\n");
            printf("                |                                                                           |\n");
            printf("                -----------------------------------------------------------------------------\n");
        }
        else if(i==1){
            printf("                -----------------------------------------------------------------------------\n");
            printf("                |+++++++++++++++++++++                                                      |\n");
            printf("                -----------------------------------------------------------------------------\n");
        }
        else if(i==2){
            printf("                -----------------------------------------------------------------------------\n");
            printf("                |++++++++++++++++++++++++++++++++++++                                         |\n");
            printf("                -----------------------------------------------------------------------------\n");
        }
        else if(i==3){
            printf("                -----------------------------------------------------------------------------\n");
            printf("                |++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++               |\n");
            printf("                -----------------------------------------------------------------------------\n");
        }
        else if(i==4){
            printf("                -----------------------------------------------------------------------------\n");
            printf("                |+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|\n");
            printf("                -----------------------------------------------------------------------------\n");
        }
        sleep(1);
        i++;
        system("cls");
    }
    i=0;
    while(i<5){
        printf("\n\n\n");
        printf("                =============================================================================\n");
        printf("                ||                                                                         ||\n");
        printf("                ||       Use Your Arrow Keys or WASD to Navigate Through the Program !     ||\n");
        printf("                ||                                                                         ||\n");
        printf("                =============================================================================\n");
        printf("\n\n\n");
        if(i==0){
            printf("                -----------------------------------------------------------------------------\n");
            printf("                |                                                                           |\n");
            printf("                -----------------------------------------------------------------------------\n");
        }
        else if(i==1){
            printf("                -----------------------------------------------------------------------------\n");
            printf("                |+++++++++++++++++++++                                                      |\n");
            printf("                -----------------------------------------------------------------------------\n");
        }
        else if(i==2){
            printf("                -----------------------------------------------------------------------------\n");
            printf("                |++++++++++++++++++++++++++++++++++++                                         |\n");
            printf("                -----------------------------------------------------------------------------\n");
        }
        else if(i==3){
            printf("                -----------------------------------------------------------------------------\n");
            printf("                |++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++               |\n");
            printf("                -----------------------------------------------------------------------------\n");
        }
        else if(i==4){
            printf("                -----------------------------------------------------------------------------\n");
            printf("                |+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|\n");
            printf("                -----------------------------------------------------------------------------\n");
        }
        sleep(1);
        i++;
        system("cls");
    }
    int ch = 1;
    while(true){
        system("cls");
        switch(ch){
            case 1:
                printf("\n\n\n");
                printf("                ==============================================\n");
                printf("                || Do you want to import the previous data? ||\n");
                printf("                || If you don't have the file, choose NO    ||\n");
                printf("                ==============================================\n");
                printf("                || > YES <                                  ||\n");
                printf("                ||   NO                                     ||\n");
                printf("                ==============================================\n");
                printf("\n\n\n");
                break;
            case 2:
                printf("\n\n\n");
                printf("                ==============================================\n");
                printf("                || Do you want to import the previous data? ||\n");
                printf("                || If you don't have the file, choose NO    ||\n");
                printf("                ==============================================\n");
                printf("                ||   YES                                    ||\n");
                printf("                || > NO  <                                  ||\n");
                printf("                ==============================================\n");
                printf("\n\n\n");
                break;
        }
        int inp = getch();
        if(inp==72 || inp ==75 || inp == 119 || inp == 97){
            if(ch>1) ch--;
            else if(ch==1)ch=2;
            else continue;
        }else if(inp == 80 || inp == 77 ||inp == 115 || inp == 100){
            if(ch<2) ch++;
            else if(ch==2) ch=1;
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
                delete_node();
                break;
            case 4:
                modify_data();
                break;
            case 5:
                search();
                break;
            case 6:
                exit(0);
        }
    }
    return 0;
}

